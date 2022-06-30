#include "civethandle.h"
#include <string>

namespace typhoon {

WebSocketHandler::WebSocketHandler(const std::string &name) : name_(name) {}

void WebSocketHandler::Open() { std::cout << "on open" << std::endl; }

void WebSocketHandler::OnMessage(const std::string &msg) {}

void WebSocketHandler::OnPong() { std::cout << "on pong" << std::endl; }

void WebSocketHandler::OnPing() {}

void WebSocketHandler::OnClose() { std::cout << "on close" << std::endl; }

void WebSocketHandler::SendData(mg_connection *conn, const std::string &data,
                                bool skippable, int op_code) {
  std::shared_ptr<std::mutex> connection_lock;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    connection_lock = user_pool_[conn];
  }

  if (!connection_lock->try_lock()) {
    if (skippable) {
      return;
    }
    connection_lock->lock();
    std::unique_lock<std::mutex> lock(mutex_);
  }

  int ret = mg_websocket_write(conn, op_code, data.c_str(), data.size());
  connection_lock->unlock();

  if (ret != static_cast<int>(data.size())) {
    if (data.empty() && ret == 2) {
      return;
    }
    return;
  }
}

void WebSocketHandler::BroadcastData(const std::string &data, bool skippable) {
  std::vector<Connection *> connections_to_send;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (user_pool_.empty()) {
      return;
    }
    for (auto &kv : user_pool_) {
      Connection *conn = kv.first;
      connections_to_send.push_back(conn);
    }
  }

  for (Connection *conn : connections_to_send) {
    SendData(conn, data, skippable);
  }
}

std::string WebSocketHandler::name() const { return name_; }

bool WebSocketHandler::handleConnection(Application *app,
                                        const mg_connection *conn) {
  return true;
}

void WebSocketHandler::handleReadyState(Application *app, Connection *conn) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    user_pool_.emplace(conn, std::make_shared<std::mutex>());
  }
  this->Open();
}

bool WebSocketHandler::handleData(Application *app, Connection *conn, int bits,
                                  char *data, size_t data_len) {
  if ((bits & 0x0F) == MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE) {
    return false;
  }
  data_.write(data, data_len);
  if (current_opcode_ == 0x00) {
    current_opcode_ = bits & 0x7f;
  }
  bool is_final_fragment = bits & 0x80;
  if (is_final_fragment) {
    switch (current_opcode_) {
    case MG_WEBSOCKET_OPCODE_TEXT:
      std::cout << data_.str() << std::endl; // print request data
      this->OnMessage(data_.str());
      break;
    default:
      break;
    }
    current_opcode_ = 0x00;
    data_.clear();
    data_.str(std::string());
  }

  return true;
}

void WebSocketHandler::handleClose(Application *app, const Connection *conn) {
  auto *connection = const_cast<Connection *>(conn);

  std::shared_ptr<std::mutex> user_lock;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    user_lock = user_pool_[connection];
  }

  {
    std::unique_lock<std::mutex> lock_connection(*user_lock);
    std::unique_lock<std::mutex> lock(mutex_);
    user_pool_.erase(connection);
  }
  this->OnClose();
}

thread_local unsigned char WebSocketHandler::current_opcode_ = 0x00;
thread_local std::stringstream WebSocketHandler::data_;

void RequestHandler::Get(Application *app, Connection *conn) {
  Response(app, conn, "");
}

void RequestHandler::Post(Application *app, Connection *conn) {
  Response(app, conn, "");
}

void RequestHandler::Put(Application *app, Connection *conn) {
  Response(app, conn, "");
}

void RequestHandler::Delete(Application *app, Connection *conn) {
  Response(app, conn, "");
}

void RequestHandler::Patch(Application *app, Connection *conn) {
  Response(app, conn, "");
}

void RequestHandler::Response(Application *app, Connection *conn,
                              const std::string &msg) {
  char *data = const_cast<char *>(msg.c_str());
  mg_send_http_ok(conn, "application/json; charset=utf-8", msg.size());
  mg_write(conn, data, msg.size());
}

void RequestHandler::Response(Application *app, Connection *conn,
                              const std::string &msg, int status_code) {
  char *data = const_cast<char *>(msg.c_str());
  if (status_code >= 200 && status_code < 300) {
    mg_send_http_ok(conn, "application/json; charset=utf-8", msg.size());
  } else if (status_code >= 300 && status_code < 400) {
    mg_send_http_redirect(conn, "", status_code);
  } else if (status_code >= 400 && status_code < 500) {
    mg_send_http_ok(conn, "application/json; charset=utf-8", msg.size());
  } else {
    mg_send_http_ok(conn, "application/json; charset=utf-8", msg.size());
  }
  mg_write(conn, data, msg.size());
}

std::string RequestHandler::GetRequestData(Connection *conn) {
  return Application::getPostData(conn);
}

const RequestInfo *RequestHandler::GetRequestInfo(Connection *conn) {
  return mg_get_request_info(conn);
}

std::string RequestHandler::GetCookie(Connection *conn,
                                      const std::string &name) {
  std::string s;
  Application::getCookie(conn, name, s);
  return s;
}

std::string RequestHandler::GetMethod(Connection *conn) {
  return Application::getMethod(conn);
}

bool RequestHandler::handleGet(Application *app, Connection *conn) {
  if (0 == callback_.count(Method::GET)) {
    this->Get(app, conn);
  } else {
    callback_[Method::GET](app, conn);
  }
  return true;
}

bool RequestHandler::handlePost(Application *app, Connection *conn) {
  if (0 == callback_.count(Method::POST)) {
    this->Post(app, conn);
  } else {
    callback_[Method::POST](app, conn);
  }
  return true;
}

bool RequestHandler::handlePut(Application *app, Connection *conn) {
  if (0 == callback_.count(Method::PUT)) {
    this->Put(app, conn);
  } else {
    callback_[Method::PUT](app, conn);
  }
  return true;
}

bool RequestHandler::handleDelete(Application *app, Connection *conn) {
  if (0 == callback_.count(Method::DELETE)) {
    this->Delete(app, conn);
  } else {
    callback_[Method::DELETE](app, conn);
  }
  return true;
}

bool RequestHandler::handlePatch(Application *app, Connection *conn) {
  if (0 == callback_.count(Method::PATCH)) {
    this->Patch(app, conn);
  } else {
    callback_[Method::PATCH](app, conn);
  }
  return true;
}

void RequestHandler::RegisterMethod(Method method, Callback callback) {
  callback_[method] = callback;
}

} // namespace typhoon
