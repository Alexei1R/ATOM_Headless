//
// Created by toor on 11/14/23.
//

#include "Application.h"

namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

        m_Client = new Client();
        m_Client->ConnectToServer("192.168.1.100:27020");
        m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
            ATLOG_INFO("Data Received: {0} bytes", size);
            //            also print message as string
            ATLOG_INFO("Data Received: {0} bytes", (char*)data);
        });
        m_Client->SetServerConnectedCallback([&]() {
            ATLOG_INFO("Connected to server");
            m_Client->SendData("Hello from client", 17);
        });
        m_Client->SetServerDisconnectedCallback([&]() {
            ATLOG_INFO("Disconnected from server");
        });


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