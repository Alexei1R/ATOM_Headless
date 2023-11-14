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


        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }


}