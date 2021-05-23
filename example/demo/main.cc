/*
* Copyright (C) Trunk Technology, Inc. - All Rights Reserved
*
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*
* Written by Huang Minhang <huangminhang@trunk.tech>, 2021/5/21 15:59
*/
#include <iostream>
#include "src/app.h"

int main(int argc, char *argv[]) {

    typhoon::app::Application app;
    app.listen(21522).start();

    return 0;
}