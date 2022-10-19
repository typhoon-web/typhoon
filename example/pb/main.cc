#include <iostream>
#include <memory>
#include <string>

#include "test.pb.h"
#include "typhoon.h"

class MyApi : public typhoon::WebSocketHandler {
 public:
  MyApi(const std::string& name) : typhoon::WebSocketHandler(name) {}

  void Open() override { std::cout << "ws open" << std::endl; }

  void OnClose() override { std::cout << "ws close" << std::endl; }

  void OnPong() override {
    std::cout << "---- pong" << std::endl;
    std::string loc_str;
    auto pose = Pose();
    pose.mutable_position()->set_x(1);
    pose.mutable_position()->set_y(2);
    pose.SerializeToString(&loc_str);
    BroadcastData(loc_str, true, MG_WEBSOCKET_OPCODE_BINARY);
  }

  void OnMessage(const std::string& msg, int op_code) override {
    if (MG_WEBSOCKET_OPCODE_TEXT == op_code) {
      std::cout << "data:" << msg << std::endl;
    } else if (MG_WEBSOCKET_OPCODE_BINARY == op_code) {
      auto pose = Pose();
      pose.ParseFromString(msg);
      std::cout << "pose position x: " << pose.position().x() << std::endl;
      std::cout << "pose position y: " << pose.position().y() << std::endl;
    } else {
    }
  }
};

int main(int argc, char* argv[]) {
  typhoon::Options options;
  options.port = 9900;
  std::cout << "typhoon simple protobuf websocket: " << options.port
            << std::endl;
  options.websocket_ping_interval = 1000;  // 定时向客户端发ping帧. 回调OnPong()
  typhoon::Server server(options);
  auto ws = std::make_shared<MyApi>("simple");
  server.AddHandle("/api/ws/", ws);
  server.Spin();
  return 0;
}
