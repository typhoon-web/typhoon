#ifndef TYPHOON_HANDLER_H_
#define TYPHOON_HANDLER_H_
#include <CivetServer.h>
#include <civetweb.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sstream>
#include <mutex>

namespace trunk {
namespace typhoon {
 
typedef struct mg_connection Connection;
typedef struct mg_request_info RequestInfo;
typedef CivetServer Application;


class WebSocketBase : public CivetWebSocketHandler {
public:
  thread_local static std::stringstream data_;
  thread_local static unsigned char current_opcode_;

  explicit WebSocketBase(const std::string& name);
  ~WebSocketBase()= default;

  /**
   * @brief 新用户链接 
   */
  void Open();

  /**
   * @brief用户发来消息
   *
   * @param msg 消息内容
   */
  void OnMessage(const std::string& msg);

  /**
   * @brief 像用户发送ping帧 
   */
  void OnPong();

  /**
   * @brief 接收客户端发来的ping帧
   */
  void OnPing();

  /**
   * @brief 用户关闭链接
   */
  void OnClose();
 
  /**
   * @brief 向客户端发送消息 
   *
   * @param conn
   * @param data
   * @param skippable 无法获取lock用户跳过
   * @param op_code 数据格式(文本 二进制...) 
   */
  void SendData(mg_connection *conn,
	              const std::string &data,
	              bool skippable = false,
	              int op_code = MG_WEBSOCKET_OPCODE_TEXT);

  /**
   * @brief 广播
   *
   * @param data
   * @param skippable 无法获取lock用户跳过 
   */
  void BroadcastData(const std::string &data, bool skippable);

private:
  
  /**
   * @brief 客户端打算链接,还未完成链接, before handleReadyState()
   *
   * @param app
   * @param conn
   *
   * @return 
   */
  bool handleConnection(Application *app, const Connection *conn) override;
  
  /**
   * @brief 客户端链接
   *
   * @param app
   * @param conn
   */
  void handleReadyState(Application *app, Connection *conn) override;
  bool handleData(Application *app,
                  Connection *conn,
                  int bits, 
                  char *data,
                  size_t data_len) override;
  void handleClose(Application *app, const Connection *conn) override;
  std::string name_;
  mutable std::mutex mutex_;
  std::unordered_map<Connection*, std::shared_ptr<std::mutex>> user_pool_;

};


class RequestBase : public CivetHandler {
public:
  virtual void Get(Application *app,Connection *conn);
  virtual void Post(Application *app,Connection *conn);
  virtual void Put(Application *app,Connection *conn);
  void Response(Application *app,Connection *conn, const std::string& msg);
  void Response(Application *app,Connection *conn, const std::string& msg, int status_code);
  const RequestInfo* GetRequestInfo(Connection *conn);
  std::string GetRequestData(Connection *conn);

private:
  bool handleGet(Application *app,Connection *conn) override;
  bool handlePost(Application *app,Connection *conn) override;
  bool handlePut(Application *app,Connection *conn) override;

};

} // typhoon   
} // trunk  

#endif // TYPHOON_HANDLER_H_
