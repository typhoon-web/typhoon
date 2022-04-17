#ifndef TYPHOON_H_
#define TYPHOON_H_
#include "src/civethandle.h"
#include "src/civetserver.h"
#include <memory>
#include <unordered_map>
#include <unistd.h>
#include <string>

namespace typhoon {

struct Options {
  int port;
  int websocket_ping_interval = 0; // ping client time (ms) 
  std::string root = ".";
  std::string access_control_allow_headers = "*";
  std::string access_control_allow_methods = "*";
  std::string access_control_allow_origin = "*";
};

class Server {
public:
  Server(int port);
  Server(Options options);
  ~Server();
  void Start();
  void AddHandle(const std::string& uri, const std::shared_ptr<WebSocketHandler>& handle);
  void AddHandle(const std::string& uri, const std::shared_ptr<RequestHandler>& handle);

private:
  int ping_interval_ = 0; 
  std::shared_ptr<Application> app_;
  std::unordered_map<std::string, std::shared_ptr<WebSocketHandler>> ws_pool_;
};

}  // namespace typhoon 

#endif  // TYPHOON_H_

