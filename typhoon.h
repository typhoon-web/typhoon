#ifndef TYPHOON_H_
#define TYPHOON_H_
#include "src/handler.h"
#include <memory>
#include <unistd.h>
#include <string>

namespace trunk {
namespace typhoon {

class WebSocketHandler : public WebSocketBase {
public:
  WebSocketHandler(const std::string& name);
  ~WebSocketHandler();
};

class RequestHandler : public RequestBase {

};

class Server {
public:
  Server(int port);
  Server(std::vector<std::string> options);
  ~Server();
  void Run();
  void Exit();
  void AddHandle(const std::string& uri, const std::shared_ptr<WebSocketHandler>& handle);
  void AddHandle(const std::string& uri, const std::shared_ptr<RequestHandler>& handle);

private:
  std::shared_ptr<Application> app_;

};

}  // namespace typhoon
}  // namespace trunk

#endif  // TYPHOON_H_
