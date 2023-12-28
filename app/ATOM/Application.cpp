//
// Created by toor on 11/14/23.
//
#include "Application.h"



namespace Atom {
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        s_Instance = (Application *) this;

        m_Interval = std::chrono::milliseconds(250);

        m_SerialCommunication = new SerialCommunicationLayer("/dev/ttyUSB0", 9600);
        PushLayer(m_SerialCommunication);


        m_Frame = new Frame("/home/toor/Downloads/test.mp4");
        PushLayer(m_Frame);

        m_ServerLayer = new ServerLayer(27020);
        PushLayer(m_ServerLayer);

        m_ServerLayer->SetServerConnectedCallback([&](std::string ip) {
            ATLOG_INFO("Connected to client: {0}", ip);
            m_Frame->PushNewVideoWriterWithIP(ip);
        });

        m_ServerLayer->RegisterMessageWithID(1, [&](Message message) {
            ATLOG_INFO("Message Received ID = 1: {0}", std::string((char*)message.payload));
        });

        //id 2 , type int
        m_ServerLayer->RegisterMessageWithID(2, [&](Message message) {
            ATLOG_INFO("Message Received: ID = 2 {0}", *(int*)message.payload);

        });

        //id 3 , glm::vec3
        m_ServerLayer->RegisterMessageWithID(3, [&](Message message) {
            // ATLOG_INFO("Message Received: ID = 3 {0}", *(glm::vec3*)message.payload);
                //print all 3 values of glm::vec3
            ATLOG_INFO("Message Received: ID = 3 {0}", ((glm::vec3*)message.payload)->x);
            ATLOG_INFO("Message Received: ID = 3 {0}", ((glm::vec3*)message.payload)->y);
            ATLOG_INFO("Message Received: ID = 3 {0}", ((glm::vec3*)message.payload)->z);
        });






    }


    Application::~Application() {

        if (m_ServerLayer) {
            delete m_ServerLayer;
        }


    }


    void Application::Run() {
        while (m_IsRuning) {
            for (Layer* layer: m_LayerStack) {
                layer->OnUpdate();
            }
            std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float, std::milli> m_TimeSpan = m_CurrentTime - m_LastTime;

            if (m_TimeSpan.count() >= m_Interval.count()) {
                for (Layer* layer: m_LayerStack) {
                    layer->OnFixedUpdate();
                }
                m_LastTime = m_CurrentTime;
            }

        }
    }


    void Application::WindowClose() {
        m_IsRuning = false;
    }


    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
    }
}
