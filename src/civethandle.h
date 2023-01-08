#ifndef TYPHOON_HANDLER_H_
#define TYPHOON_HANDLER_H_

#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include "civetserver.h"

namespace typhoon {

typedef struct mg_connection Connection;
typedef struct mg_request_info RequestInfo;
typedef CivetServer Application;
typedef std::function<void(Application*, Connection*)> Callback;

class WebSocketHandler : public CivetWebSocketHandler {
 public:
  thread_local static std::stringstream data_;
  thread_local static unsigned char current_opcode_;

  explicit WebSocketHandler(const std::string& name);
  ~WebSocketHandler() = default;

  /**
   * @brief 新用户链接
   */
  virtual void Open(Application* app, Connection* conn);

  /**
   * @brief用户发来消息
   *
   * @param msg 消息内容
   * @param op_code 消息类型,text or binary
   */
  virtual void OnMessage(Application* app, Connection* conn,
                         const std::string& msg,
                         int op_code = MG_WEBSOCKET_OPCODE_TEXT);

  /**
   * @brief 像用户发送ping帧
   */
  virtual void OnPong();

  /**
   * @brief 接收客户端发来的ping帧
   */
  virtual void OnPing();

  /**
   * @brief 用户关闭链接
   */
  virtual void OnClose(Application* app, const Connection* conn);

  /**
   * @brief 向客户端发送消息
   *
   * @param conn
   * @param data
   * @param skippable 无法获取lock用户跳过
   * @param op_code 数据格式(文本 二进制...)
   */
  virtual void SendData(Connection* conn, const std::string& data,
                        bool skippable = false,
                        int op_code = MG_WEBSOCKET_OPCODE_TEXT);

  /**
   * @brief 广播
   *
   * @param data
   * @param skippable 无法获取lock用户跳过
   */
  virtual void BroadcastData(const std::string& data, bool skippable,
                             int op_code = MG_WEBSOCKET_OPCODE_TEXT);

  virtual std::string name() const final;

 private:
  /**
   * @brief 客户端打算链接,还未完成链接, before handleReadyState()
   *
   * @param app
   * @param conn
   *
   * @return
   */
  bool handleConnection(Application* app, const Connection* conn) override;

  /**
   * @brief 客户端链接
   *
   * @param app
   * @param conn
   */
  void handleReadyState(Application* app, Connection* conn) override;
  bool handleData(Application* app, Connection* conn, int bits, char* data,
                  size_t data_len) override;
  void handleClose(Application* app, const Connection* conn) override;
  std::string name_;
  mutable std::mutex mutex_;
  std::map<Connection*, std::shared_ptr<std::mutex>> user_pool_;
};

class RequestHandler : public CivetHandler {
 public:
  enum class Method : uint8_t {
    GET = 0,
    POST = 1,
    PUT = 2,
    DELETE = 3,
    PATCH = 4
  };
  virtual void Get(Application* app, Connection* conn);
  virtual void Post(Application* app, Connection* conn);
  virtual void Put(Application* app, Connection* conn);
  virtual void Delete(Application* app, Connection* conn);
  virtual void Patch(Application* app, Connection* conn);
  /**
   * @brief 应答
   *
   * @param app
   * @param conn
   * @param msg 返回数据
   * */
  static void Response(Application* app, Connection* conn,
                       const std::string& msg);
  /**
   * @brief 应答
   *
   * @param app
   * @param conn
   * @param msg 返回数据
   * @param status_code 返回状态码
   * */
  static void Response(
      Application* app, Connection* conn, const char* msg, int status_code,
      std::string content_type = "application/json; charset=utf-8");

  static int Write(Application* app, Connection* conn, const void* data,
                   size_t len);

  static const RequestInfo* GetRequestInfo(Connection* conn);
  /**
   * @brief POST请求, 获取请求体数据
   *
   * @param conn
   * */
  static std::string GetRequestData(Connection* conn);
  /**
   * @brief 解析url参数
   *
   * @param conn
   * @param key url参数key
   *
   * @return url参数value
   * */
  static std::string GetParam(Connection* conn, const char* key,
                              size_t occurrence = 0);
  static std::string GetCookie(Connection* conn, const std::string& name);
  static void SetCookie(Connection* conn, const std::string& value);
  static std::string GetMethod(Connection* conn);
  static int AddResoposeHeader(Connection* conn, const std::string& header,
                               const std::string& value);
  static int SendResponseHeader(Connection* conn);
  void RegisterMethod(Method method, Callback callback);

 private:
  bool handleGet(Application* app, Connection* conn) override;
  bool handlePost(Application* app, Connection* conn) override;
  bool handlePut(Application* app, Connection* conn) override;
  bool handleDelete(Application* app, Connection* conn) override;
  bool handlePatch(Application* app, Connection* conn) override;
  std::map<Method, Callback> callback_;
};

}  // namespace typhoon

#endif  // TYPHOON_HANDLER_H_
