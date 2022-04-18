#include <iostream>
#include <memory>
#include <string>
#include "typhoon.h"



class MyApi1: public typhoon::RequestHandler {
public: 
  void Get(typhoon::Application* app, typhoon::Connection* conn) override {
    std::cout << "simple get" << std::endl; 
    Response(app, conn, "simple http get");
  }
  
  void Post(typhoon::Application* app, typhoon::Connection* conn) override {
    std::cout << "simple post" << std::endl; 
    auto data = GetRequestData(conn);
    std::cout << "data: " << data << std::endl;
    Response(app, conn, "simple http post");
  }

};

class MyApi2: public typhoon::WebSocketHandler {
public:
  MyApi2(const std::string& name) : typhoon::WebSocketHandler(name) {}

  void Open() override {
    std::cout << "ws open" << std::endl;
  }
  
  void OnClose() override {
    std::cout << "ws close" << std::endl;
  }
  
  void OnPong() override {
    std::cout << "---- pong" << std::endl; 
  }

};


int main(int argc, char *argv[]) {
  typhoon::Options options;
  options.port = 9900;
  std::cout << "typhoon simple websocket: " << options.port << std::endl;
  options.websocket_ping_interval = 1000; // 定时向客户端发ping帧. 回调OnPong()  
  typhoon::Server server(options);
  auto api = std::make_shared<MyApi1>();
  auto ws = std::make_shared<MyApi2>("simple");
  server.AddHandle("/api/simple/", api);
  server.AddHandle("/api/ws/", ws);
  server.Spin();
  return 0;
}

