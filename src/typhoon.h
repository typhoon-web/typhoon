#ifndef TYPHOON_H_
#define TYPHOON_H_
#include "civethandle.h"
#include "civetserver.h"
#include <cassert>
#include <memory>
#include <string>
#include <unistd.h>
#include <unordered_map>

namespace typhoon {

struct Options {
  size_t port;
  size_t websocket_ping_interval = 0; // ping client time (ms)
  size_t num_threads = 6;
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
  void Spin();
  void SpinSome();
  void AddHandle(const std::string &uri,
                 const std::shared_ptr<WebSocketHandler> &handle);
  void AddHandle(const std::string &uri,
                 const std::shared_ptr<RequestHandler> &handle);
  void Get(const std::string &uri, Callback callback);
  void Post(const std::string &uri, Callback callback);
  void Put(const std::string &uri, Callback callback);
  void Delete(const std::string &uri, Callback callback);
  void Patch(const std::string &uri, Callback callback);

private:
  void Bind(const std::string &uri, RequestHandler::Method method,
            Callback callback);
  int ping_interval_ = 0;
  std::shared_ptr<Application> app_;
  std::unordered_map<std::string, std::shared_ptr<WebSocketHandler>> ws_pool_;
  std::unordered_map<std::string, std::shared_ptr<RequestHandler>>
      handler_pool_;
};

} // namespace typhoon

#endif // TYPHOON_H_
