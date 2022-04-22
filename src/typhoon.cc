#include "typhoon.h"


namespace typhoon {

Server::~Server() {
  this->app_->close();
  mg_exit_library();

}

Server::Server(Options options) {
  mg_init_library(0);
  std::vector<std::string> op = {
    "document_root", options.root,
    "listening_ports", std::to_string(options.port),
    "access_control_allow_headers", options.access_control_allow_headers,
    "access_control_allow_methods", options.access_control_allow_methods,
    "access_control_allow_origin", options.access_control_allow_origin,
  };
  ping_interval_ = options.websocket_ping_interval;
  app_ = std::make_shared<Application>(op);
}

Server::Server(int port) {
  std::cout << "port:" << port << std::endl;
  mg_init_library(0);
  std::vector<std::string> op;
  op = {
      "document_root",".",
      "listening_ports", std::to_string(port),
      "access_control_allow_headers","*",
      "access_control_allow_methods","*",
      "access_control_allow_origin","*",
  };
  app_ = std::make_shared<Application>(op);
}

void Server::Spin() {
  if (ping_interval_ > 0) {
    while (true) {
      for (const auto& ws : ws_pool_) {
          ws.second->OnPong(); 
      }
      usleep(ping_interval_*1000);
    }
  } else {
    while (true) {
      sleep(1);
    }
  }
}

void Server::SpinSome() {
  for (const auto& ws : ws_pool_) {
    ws.second->OnPong();
  } 
}

void Server::AddHandle(const std::string& uri,
                       const std::shared_ptr<WebSocketHandler>& handle) {
  assert(ws_pool_.count(handle->name()) == 0);
  ws_pool_[handle->name()] = handle;
  this->app_->addWebSocketHandler(uri, *handle);
}

void Server::AddHandle(const std::string& uri, 
                       const std::shared_ptr<RequestHandler>& handle) {
  this->app_->addHandler(uri, *handle);
}


}  // namespace typhoon 

