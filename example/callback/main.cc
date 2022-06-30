#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "civethandle.h"
#include "typhoon.h"

struct Data {
  std::string name;
  size_t age;
};

class MyApi {
 public:
  ~MyApi() = default;
  MyApi() = default;
  void GetInfo(typhoon::Application* app, typhoon::Connection* conn) {
    std::cout << "simple3 get info " << std::endl;
    typhoon::RequestHandler::Response(app, conn, "simple3 get");
  }

  void UpdateInfo(typhoon::Application* app, typhoon::Connection* conn,
                  Data* data) {
    std::cout << "simple3 post info" << std::endl;
    std::cout << "name: " << data->name << std::endl;
    typhoon::RequestHandler::Response(app, conn, "simple3 post");
  }
};

int main(int argc, char* argv[]) {
  typhoon::Options options;
  options.port = 9900;
  std::cout << "typhoon simple func: " << options.port << std::endl;
  typhoon::Server server(options);

  Data data;

  server.Get(
      "/api/simple", [&](typhoon::Application* app, typhoon::Connection* conn) {
        std::cout << "simple callback get" << std::endl;
        data.name = "trunk";
        data.age = 6;

        std::cout << "name:" << data.name << std::endl;
        typhoon::RequestHandler::Response(app, conn, "simple callback post");
      });

  server.Post(
      "/api/simple", [&](typhoon::Application* app, typhoon::Connection* conn) {
        std::cout << "simple callback post" << std::endl;
        std::cout << "name:" << data.name << std::endl;
        std::cout << "age:" << data.age << std::endl;
        data.name = "uuuyyyy";
        typhoon::RequestHandler::Response(app, conn, "simple callback post");
      });

  server.Get("/api/simple2", [&](typhoon::Application* app,
                                 typhoon::Connection* conn) {
    std::cout << "simple2 callback post" << std::endl;
    std::cout << "name: " << data.name << std::endl;
    typhoon::RequestHandler::Response(app, conn, "simple2 callback get");
  });
  
  MyApi api1;
  std::function<void(typhoon::Application*, typhoon::Connection*)> callback =
      std::bind(&MyApi::GetInfo, &api1, std::placeholders::_1,
                std::placeholders::_2);
  std::function<void(typhoon::Application*, typhoon::Connection*)> callback2 =
      std::bind(&MyApi::UpdateInfo, &api1, std::placeholders::_1,
                std::placeholders::_2, &data);

  server.Get("/api/simple3", callback);
  server.Post("/api/simple3", callback2);

  server.Spin();
  return 0;
}
