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
#include <nlohmann/json.hpp>
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace typhoon {
namespace web {

    typedef websocketpp::server<websocketpp::config::asio> io_server;

    class RequestHandler {
    public:
        ~RequestHandler();
        explicit RequestHandler(const unsigned int& port);
        virtual void run() final;
        virtual void shutdown() final;

    protected:
        void cross_origin(io_server::connection_ptr&);
        const std::string& get_uri(io_server::connection_ptr&);
        void exception_view(io_server::connection_ptr&);
        virtual nlohmann::json get_data(io_server::connection_ptr&) final;
        virtual void response(io_server::connection_ptr&,nlohmann::json&) final;
        virtual void on_http(const websocketpp::connection_hdl&);

    protected:
        io_server server;

    private:
        const unsigned int _http_port;

    };

} // namespace web
} // namespace typhoon

#endif //TYPHOON_WEB_H
