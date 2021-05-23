/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 16:03
*/
#include "view.h"

namespace typhoon {

    View::~View() = default;
    View::View() : web::RequestHandler() {

    }

//    void View::on_http(const websocketpp::connection_hdl& hdl) {
//        auto con_ptr = this->get_con_from_hdl(hdl);
//        std::cout << "host:" << con_ptr->get_request().get_uri() << std::endl;
//        std::cout << "host:" << con_ptr->get_host() << std::endl;
//        std::cout << "get_remote_close_reason:" << con_ptr->get_remote_close_reason() << std::endl;
//        std::cout << "get_resource:" << con_ptr->get_resource() << std::endl;
//        std::cout << "get_response_msg:" << con_ptr->get_response_msg() << std::endl;
//        std::cout << "get_request_body:" << con_ptr->get_request().get_method() << std::endl;
//    }

    View View::hello(web::ConPtr con_ptr) {
        nlohmann::json response_data;
        nlohmann::json request_data = this->get_data(con_ptr);
        response_data["success"] = true;
        response_data["code"] = 10000;
        response_data["msg"] = "hello";
        this->response(con_ptr,response_data);
        return *this;
    }


} // namespace typhoon
