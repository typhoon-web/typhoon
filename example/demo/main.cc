/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 15:59
*/
#include <iostream>
#include "src/view.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello first demo typhoon" << std::endl;
    typhoon::View http_server(21522);
    http_server.run();
    return 0;
}