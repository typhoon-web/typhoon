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
    View::View(const unsigned int& port) : web::RequestHandler(port) {

    }

    void View::on_http(const websocketpp::connection_hdl& hdl) {
        auto con_ptr = this->server.get_con_from_hdl(hdl);
    }


} // namespace typhoon
