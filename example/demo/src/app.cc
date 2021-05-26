/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/23 17:43
*/
#include "app.h"

namespace typhoon {
namespace app {

    Application::~Application() = default;
    Application::Application() : web::Application() {
        m_view_ptr = std::make_shared<View>();
    }

    int Application::reverse_url(const std::string& uri) {
        std::unordered_map<std::string, unsigned int>::iterator it;
        int target = 999;
        it = this->router.find(uri);
        if (it != this->router.end()) { // find
            target = (*it).second;
        }
        return target;
    }

    void Application::on_http(const web::ConHdl& hdl) {
        auto con_ptr = get_con_from_hdl(hdl);
        switch (this->reverse_url(this->get_uri(con_ptr))) {
            case url::ok:
                m_view_ptr->hello(con_ptr);break;
            default:
                m_view_ptr->exception_view(con_ptr);
                break;
        }; // switch
    }

    Application& Application::set_timer(long duration) {
        m_timer = m_server_ptr->set_timer(duration, std::bind(&Application::on_timer,this, std::placeholders::_1, duration));
        std::cout << "set_timer" << std::endl;
        return *this;
    }

    void Application::on_timer(const websocketpp::lib::error_code& ec, long duration) {
        std::cout << "on_timer" << std::endl;
        if (ec) {
            // there was an error, stop telemetry
            return;
        }
        // set timer for next telemetry check
        set_timer(duration);
        std::cout << "on_timer endl" << std::endl;
    }


} // namespace app
} // namespace typhoon

