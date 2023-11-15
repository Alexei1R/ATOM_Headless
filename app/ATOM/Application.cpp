//
// Created by toor on 11/14/23.
//

#include "Application.h"

namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

//        m_Client = new Client();
//        m_Client->ConnectToServer("192.168.1.103:27020");
//        m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
//            ATLOG_INFO("Data Received: {0} bytes", size);
//            ATLOG_INFO("Message: {0}", std::string ((char*)data));
//        });
//        m_Client->SetServerConnectedCallback([&]() {
//            ATLOG_INFO("Connected to server");
//            m_Client->SendData("Hello from client", 17);
//        });
//        m_Client->SetServerDisconnectedCallback([&]() {
//            ATLOG_INFO("Disconnected from server");
//        });


    }


    Application::~Application()
    {
//        delete m_Client;
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