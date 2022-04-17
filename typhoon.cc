#include "typhoon.h"
#include <unistd.h>


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
  app_ = std::make_shared<CivetServer>(op);
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

void Server::Start() {
  while (true) {
    if (ping_interval_ > 0) {
      for (const auto& ws : ws_pool_) {
        ws.second->OnPong(); 
      }
      usleep(ping_interval_*1000);
    } else {
      sleep(1);
    }
  }
}

void Server::AddHandle(const std::string& uri,
                       const std::shared_ptr<WebSocketHandler>& handle) {
  ws_pool_[handle->name()] = handle;
  this->app_->addWebSocketHandler(uri, *handle);
}

void Server::AddHandle(const std::string& uri, 
                       const std::shared_ptr<RequestHandler>& handle) {
  this->app_->addHandler(uri, *handle);
}


}  // namespace typhoon 

