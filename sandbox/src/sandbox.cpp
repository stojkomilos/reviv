#include<reviv.h>

class Sandbox : public Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }

}

Application* createApplication()
{
    return new Sandbox();
}