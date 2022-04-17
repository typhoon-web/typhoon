#include <iostream>
#include <memory>
#include <string>
#include "typhoon.h"
#include <nlohmann/json.hpp>



class MyApi1: public typhoon::RequestHandler {
public: 
  void Get(typhoon::Application* app, typhoon::Connection* conn) override {
    std::cout << "simple get" << std::endl; 
    Response(app, conn, "simple http get");
  }
  
  void Post(typhoon::Application* app, typhoon::Connection* conn) override {
    std::cout << "simple post json" << std::endl; 
    auto data = GetRequestData(conn);
    std::cout << "data: " << data << std::endl;

    std::cout << "-------------str-json--------" << std::endl;
    nlohmann::json obj = nlohmann::json::parse(data);
    std::cout << "json:" << obj << std::endl;

    std::cout << "-------------json-str--------" << std::endl;
    nlohmann::json ret;
    ret["code"] = "100001";
    ret["msg"] = "ok";
    std::string s = ret.dump(0);
    Response(app, conn, s);
  }

};


int main(int argc, char *argv[]) {
  typhoon::Options options;
  options.port = 9900;
  std::cout << "typhoon simple json: " << options.port << std::endl;
  typhoon::Server server(options);
  auto api = std::make_shared<MyApi1>();
  server.AddHandle("/api/simple/", api);
  server.Start();
  return 0;
}

