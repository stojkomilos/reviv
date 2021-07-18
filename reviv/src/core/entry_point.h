#include"rv_pch.hpp"

#include"application.h"

using std::cin; using std::cout; using std::endl;

extern Application* createApplication();

int main(int argc, char** argv)
{
    cout << "Reviv engine" << endl;

    auto app = createApplication();
    app->run();
    delete app;

    return 0;
}