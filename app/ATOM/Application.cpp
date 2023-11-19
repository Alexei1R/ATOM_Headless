//
// Created by toor on 11/14/23.
//

#include "Application.h"

namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;




    }


    Application::~Application()
    {
    }







    void Application::Run()
    {


        while (m_IsRuning)
        {
//            if(m_Client->GetConnectionStatus() == Client::ConnectionStatus::Connected){
//                m_Client->SendData("Hello from client", 17);
//
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }


}