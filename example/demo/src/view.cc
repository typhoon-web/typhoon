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
