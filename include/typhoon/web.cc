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
    RequestHandler::RequestHandler(const unsigned int& port) : _http_port(port) {
        this->server.set_error_channels(websocketpp::log::elevel::all); // log
        this->server.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
        this->server.clear_access_channels(websocketpp::log::alevel::all); // disable all ws-system log
        this->server.init_asio(); // asio init
        this->server.set_reuse_addr(true); // reset asio listen
        this->server.set_http_handler(std::bind(&RequestHandler::on_http, this, std::placeholders::_1));
    }

    void RequestHandler::run() {
        this->server.listen(this->_http_port);
        this->server.start_accept();
        this->server.run(); // block
    }

    void RequestHandler::shutdown() {
        this->server.stop_listening();
    }

    void RequestHandler::cross_origin(io_server::connection_ptr& con_ptr) {
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

    const std::string& RequestHandler::get_uri(io_server::connection_ptr& con_ptr) {
        return con_ptr->get_request().get_uri();
    }

    void RequestHandler::exception_view(io_server::connection_ptr& con_ptr) {
        con_ptr->set_body("404: Not Found");
        con_ptr->set_status(websocketpp::http::status_code::not_found);
    }

    nlohmann::json RequestHandler::get_data(io_server::connection_ptr& con_ptr) {
        nlohmann::json json_data;
        std::string data = con_ptr->get_request_body();

        if (!data.empty()) {
            json_data = nlohmann::json::parse(data);
        }

        return json_data;
    }

    void RequestHandler::response(io_server::connection_ptr& con_ptr, nlohmann::json& data) {
        this->cross_origin(con_ptr); // 跨域
        con_ptr->set_body(data.dump(0));
        con_ptr->set_status(websocketpp::http::status_code::ok);
//        httplog->info("{} {} {}", con_ptr->get_response().get_status_code(), con_ptr->get_request().get_method(), con_ptr->get_request().get_uri());
    }

    void RequestHandler::on_http(const websocketpp::connection_hdl& hdl) {

    }


} // namespace web
} // namespace typhoon
