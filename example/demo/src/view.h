/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 16:03
*/
#ifndef TYPHOON_VIEW_H
#define TYPHOON_VIEW_H
#include "typhoon/web.h"

namespace typhoon {

    class View : public web::RequestHandler {
    public:
        ~View();
        View();

    public:
        View hello(web::ConPtr con_ptr);

    };

} // namespace typhoon

#endif //TYPHOON_VIEW_H
