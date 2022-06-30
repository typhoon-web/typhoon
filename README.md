[TOC]

# Typhoon

> C++ web server 

## #1 环境 

```shell
Ubuntu 
macOS 
```

## #2 安装 


```shell
mkdir build && cd build
cmake ..
make 
sudo make install
```

## #3 快速开始 

```txt
find_package(PkgConfig REQUIRED)
pkg_check_modules(Typhoon REQUIRED typhoon)
include_directories(
    ${Typhoon_INCLUDE_DIRS}
}

link_directories (
    ${Typhoon_LIBRARY_DIRS}
}
target_link_libraries(${TARGET_NAME}
  ${Typhoon_LIBRARIES}
)
```


### #3.1 http

> simple 

```cpp
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

int main(int argc, char *argv[]) {
  typhoon::Options options;
  options.port = 9900;
  std::cout << "typhoon simple http: " << options.port << std::endl;
  typhoon::Server server(options);
  auto api = std::make_shared<MyApi1>();
  server.AddHandle("/api/simple/", api);
  server.Spin();
  return 0;
}

```

> callback

```cpp
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
```

### #3.2 websocket

```cpp
#include <iostream>
#include <memory>
#include <string>
#include "typhoon.h"

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
  auto ws = std::make_shared<MyApi2>("simple");
  server.AddHandle("/api/ws/", ws);
  server.Spin();
  return 0;
}

```



### #3.3 json 

> 依赖`nlohmannjson`: https://github.com/nlohmann/json

```cpp
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
  server.Spin();
  return 0;
}

```

![json_view](./docs/http-json1.png)
![json_view](./docs/http-json2.png)


