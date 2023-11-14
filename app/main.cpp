#include "ATOM/Application.h"
#include "ATOM/Core/Logging/Logging.h"

int main(){
    Atom::Log::Init();
    ATLOG_INFO("App started");
    Atom::Application* app = new Atom::Application();
    app->Run();
    delete app;
}