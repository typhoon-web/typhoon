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

    nlohmann::json RequestHandler::get_data(ConPtr& con_ptr) {
        nlohmann::json json_data;
        std::string data = con_ptr->get_request_body();
        if (!data.empty()) {
            json_data = nlohmann::json::parse(data);
        }
        return json_data;
    }

    std::string RequestHandler::get_method(ConPtr& con_ptr) {
        return con_ptr->get_request().get_method();
    }

    std::string RequestHandler::get_host(ConPtr& con_ptr) {
        return con_ptr->get_host();
    }

    void RequestHandler::response(ConPtr& con_ptr, nlohmann::json& data) {
        this->_cross_origin(con_ptr); // 跨域
        con_ptr->set_body(data.dump(0));
        con_ptr->set_status(websocketpp::http::status_code::ok);
        std::cout << con_ptr->get_response().get_status_code() << " " << con_ptr->get_request().get_method() << " " << con_ptr->get_request().get_uri() << std::endl;
    }


    // Application


    void Application::_app_init() {
        m_server_ptr->set_error_channels(websocketpp::log::elevel::all); // log
        m_server_ptr->set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
        m_server_ptr->clear_access_channels(websocketpp::log::alevel::all); // disable all ws-system log
        m_server_ptr->init_asio(); // asio init
        m_server_ptr->set_reuse_addr(true); // reset asio listen
        m_server_ptr->set_http_handler(std::bind(&Application::on_http, this, std::placeholders::_1));
        m_server_ptr->set_open_handler(std::bind(&Application::on_open, this, std::placeholders::_1));
        m_server_ptr->set_close_handler(std::bind(&Application::on_close, this, std::placeholders::_1));
        m_server_ptr->set_message_handler(std::bind(&Application::on_message, this,std::placeholders::_1, std::placeholders::_2));
    }

    void Application::on_http(const ConHdl& hdl) {

    }

    void Application::on_message(const ConHdl& hdl, IOServer::message_ptr msg) {
        for (auto& user : m_user_list) {
            m_server_ptr->send(user, msg->get_payload(), msg->get_opcode()); // 发送消息 将客户端发来的消息 发回去
        }
    }

    void Application::on_open(const ConHdl& hdl) {
        this->add_user(hdl);
    }

    void Application::add_user(const ConHdl& hdl) {
        m_user_list.push_back(hdl); // add user
        auto con = this->get_con_from_hdl(hdl);
        std::cout << "add user " << this->get_host(con) << std::endl;
    }

    void Application::on_close(const ConHdl& hdl) {
        this->del_user(hdl); // delete user
        auto con = this->get_con_from_hdl(hdl);
        std::cout << "delete user " << this->get_host(con) << std::endl;
    }

    void Application::del_user(const ConHdl& hdl) {
        std::list<websocketpp::connection_hdl>::iterator users_begin, users_end;
        users_begin = m_user_list.begin();
        users_end = m_user_list.end();
        if (m_user_list.empty()) { ;
        } else if (m_user_list.size() == 1) {
            m_user_list.clear(); // delete user list
        } else { // user count > 1
            for (; users_begin != users_end; users_begin++) {
                ConPtr input_con = m_server_ptr->get_con_from_hdl(hdl);
                ConPtr foo_con = m_server_ptr->get_con_from_hdl(*users_begin);
                if (input_con == foo_con) {
                    m_user_list.erase(users_begin);
                    break;
                }
            }
        } // else
    }

    Application& Application::listen(const unsigned int& port) {
        m_server_ptr->listen(port);
        std::cout << "listen: " << port << std::endl;
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

    Application& Application::set_timer(long duration) {
        return *this;
    }

    ConPtr Application::get_con_from_hdl(const ConHdl& hdl) {
        return m_server_ptr->get_con_from_hdl(hdl);
    }

    const std::string& Application::get_uri(ConPtr& con_ptr) {
        return con_ptr->get_request().get_uri();
    }

    const std::string& Application::get_host(ConPtr& con_ptr) {
        return con_ptr->get_host();
    }


} // namespace web
} // namespace typhoon
