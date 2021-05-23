/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 16:21
*/
#include "web.h"

namespace typhoon {
namespace web {

    RequestHandler::~RequestHandler() = default;
    RequestHandler::RequestHandler() = default;

    Application::~Application() = default;
    Application::Application() {
        m_server_ptr = std::make_shared<IOServer>();
        this->_app_init();
    }

    void RequestHandler::_cross_origin(ConPtr& con_ptr) {
        std::string all = "*";
        std::string methods_value = "GET, POST, PUT, DELETE, OPTIONS";
        std::string type_value = "application/json";
        std::string max_age_value = "1000";
        std::string allow_origin = "Access-Control-Allow-Origin";
        std::string allow_headers = "Access-Control-Allow-Headers";
        std::string max_age = "Access-Control-Max-Age";
        std::string type = "Content-type";
        std::string allow_methods = "Access-Control-Allow-Methods";
        con_ptr->append_header(allow_origin,all);
        con_ptr->append_header(allow_headers,all);
        con_ptr->append_header(max_age,max_age_value);
        con_ptr->append_header(type,type_value);
        con_ptr->append_header(allow_methods,methods_value);
    }

    void RequestHandler::exception_view(ConPtr& con_ptr) {
        con_ptr->set_body("404: Not Found");
        con_ptr->set_status(websocketpp::http::status_code::not_found);
    }

    void RequestHandler::bind_app(const ApplicationPtr& app) {
        m_app_ptr = app;
    }

    nlohmann::json RequestHandler::get_data(ConPtr& con_ptr) {
        nlohmann::json json_data;
        std::string data = con_ptr->get_request_body();

        if (!data.empty()) {
            json_data = nlohmann::json::parse(data);
        }

        return json_data;
    }

    void RequestHandler::response(ConPtr& con_ptr, nlohmann::json& data) {
        this->_cross_origin(con_ptr); // 跨域
        con_ptr->set_body(data.dump(0));
        con_ptr->set_status(websocketpp::http::status_code::ok);
    }

    void Application::_app_init() {
        this->m_server_ptr->set_error_channels(websocketpp::log::elevel::all); // log
        this->m_server_ptr->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
        this->m_server_ptr->clear_access_channels(websocketpp::log::alevel::all); // disable all ws-system log
        this->m_server_ptr->init_asio(); // asio init
        this->m_server_ptr->set_reuse_addr(true); // reset asio listen
        this->m_server_ptr->set_http_handler(std::bind(&Application::on_http, this, std::placeholders::_1));
    }

    void Application::on_http(const ConHdl& hdl) {
    }

    Application& Application::listen(const unsigned int& port) {
        m_server_ptr->listen(port);
        return *this;
    }

    Application& Application::start() {
        m_server_ptr->start_accept();
        m_server_ptr->run();
        return *this;
    }

    Application& Application::shutdown() {
        m_server_ptr->stop_listening();
        return *this;
    }

    ConPtr Application::get_con_from_hdl(const ConHdl& hdl) {
        return m_server_ptr->get_con_from_hdl(hdl);
    }

    const std::string& Application::get_uri(ConPtr& con_ptr) {
        return con_ptr->get_request().get_uri();
    }


} // namespace web
} // namespace typhoon
