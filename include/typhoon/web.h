/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 16:21
*/
#ifndef TYPHOON_WEB_H
#define TYPHOON_WEB_H
#include <list>
#include <memory>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace typhoon {
namespace web {

    typedef class Application ApplicationType;
    typedef websocketpp::server<websocketpp::config::asio> IOServer;
    typedef websocketpp::connection_hdl ConHdl;
    typedef IOServer::connection_ptr ConPtr;
    typedef std::shared_ptr<IOServer> IOServerPtr;
    typedef std::shared_ptr<ApplicationType> ApplicationPtr;

    class RequestHandler {
    public:
        ~RequestHandler();
        RequestHandler();

        virtual nlohmann::json get_data(ConPtr&) final;
        virtual std::string get_method(ConPtr&) final;
        virtual std::string get_host(ConPtr&) final;
        virtual void response(ConPtr&, nlohmann::json&) final;
        void exception_view(ConPtr&);

    private:
        void _cross_origin(ConPtr& con_ptr);

    }; // RequestHandler

    class Application {
    public:
        ~Application();
        Application();

        virtual ConPtr get_con_from_hdl(const ConHdl& hdl) final;
        virtual const std::string& get_uri(ConPtr&) final;
        virtual const std::string& get_host(ConPtr&) final;
        virtual Application& listen(const unsigned int& port) final; // 监听端口号
        virtual Application& start() final; // block
        virtual Application& shutdown() final; // stop

    protected:
        virtual void on_http(const ConHdl&); // http 钩子
        void on_open(const ConHdl& hdl); // open callback
        void on_close(const ConHdl& hdl); // close callback
        void on_message(const ConHdl& hdl, IOServer::message_ptr msg); // close callback
        void add_user(const ConHdl& hdl); // 新增用户
        void del_user(const ConHdl& hdl); // 删除当前用户

        IOServerPtr m_server_ptr;

    private:
        void _app_init();

        std::list<ConHdl> m_user_list; // 用户列表

    };

} // namespace web
} // namespace typhoon

#endif //TYPHOON_WEB_H
