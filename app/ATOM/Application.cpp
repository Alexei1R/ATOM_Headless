//
// Created by toor on 11/14/23.
//

#include "Application.h"

static  void OnClientConnected(const Atom::ClientInfo& info) {
    ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
}


namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;


        m_Server = new Server(27020);
        m_Server->Start();
        m_Server->SetClientConnectedCallback(OnClientConnected);
        m_Server->SetClientDisconnectedCallback([&](const ClientInfo& info) {
            ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);
        });

        m_Server->SetDataReceivedCallback([&](const ClientInfo& info, const void* data, unsigned int size) {
            ATLOG_INFO("Data Received from {0}: {1} bytes", info.ConnectionDesc, size);
//            also print message as string
            ATLOG_INFO("Recived message from {0}: {1} bytes", info.ConnectionDesc, (char*)data);
            m_Server->SendDataToClient(info.ID, "Some data", 9);
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