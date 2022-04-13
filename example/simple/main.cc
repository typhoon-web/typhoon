#include <CivetServer.h>
#include <iostream>
#include <memory>
#include "../../typhoon.h"

using namespace trunk::typhoon;

class MyApi1: public RequestHandler {
public: 
  void Get(CivetServer* server, Connection* conn) override {
    std::cout << "simple get" << std::endl; 
    Response(server, conn, "simple http get");
  }
  
  void Post(CivetServer* server, Connection* conn) override {
    std::cout << "simple post" << std::endl; 
    auto data = GetRequestData(conn);
    std::cout << "data: " << data << std::endl;
    Response(server, conn, "simple http post");
  }

};

int main(int argc, char *argv[]) {
  std::cout << "typhoon simple" << std::endl;
  trunk::typhoon::Server server(9900);
  auto api1 = std::make_shared<MyApi1>();
  auto http = std::make_shared<trunk::typhoon::RequestHandler>();
  auto ws = std::make_shared<trunk::typhoon::WebSocketHandler>("simple");
  server.AddHandle("/api/ok/", http);
  server.AddHandle("/api/api1/", api1);
  server.AddHandle("/api/ws/", ws);
  server.Run();
  server.Exit();
  return 0;
}
