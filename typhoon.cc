#include "typhoon.h"
#include "src/handler.h"
#include <memory>
#include <string>

namespace trunk {
namespace typhoon {

WebSocketHandler::WebSocketHandler(const std::string& name) : WebSocketBase(name) {}
WebSocketHandler::~WebSocketHandler() {}

Server::~Server() {}

Server::Server(std::vector<std::string> options) {
  mg_init_library(0);
  app_ = std::make_shared<CivetServer>(options);
}

Server::Server(int port) {
  std::cout << "port:" << port << std::endl;
  mg_init_library(0);
  std::vector<std::string> options;
  options = {
      "document_root",".",
      "listening_ports", std::to_string(port),
      "access_control_allow_headers","*",
      "access_control_allow_methods","*",
      "access_control_allow_origin","*",
  };
  app_ = std::make_shared<Application>(options);
}

void Server::Run() {
  while (true) {
    sleep(1);
  }
}

void Server::Exit() { 
  this->app_->close();
  mg_exit_library();
}

void Server::AddHandle(const std::string& uri,
                       const std::shared_ptr<WebSocketHandler>& handle) {
  this->app_->addWebSocketHandler(uri, *handle);
}

void Server::AddHandle(const std::string& uri, 
                       const std::shared_ptr<RequestHandler>& handle) {
  this->app_->addHandler(uri, *handle);
}


}  // namespace typhoon
}  // namespace trunk

