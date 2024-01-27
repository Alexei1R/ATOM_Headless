//
// Created by toor on 11/14/23.
//
#include "Application.h"



namespace Atom {
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        signal(SIGINT, Application::SignalHandler);
        signal(SIGTERM, Application::SignalHandler);

        s_Instance = (Application *) this;

        m_Interval = std::chrono::milliseconds(1000);

        m_SerialCommunication = new SerialCommunicationLayer("/dev/ttyACM0", 19200);
        PushLayer(m_SerialCommunication);

        m_ServerLayer = new ServerLayer(27020);
        PushLayer(m_ServerLayer);

        m_Frame = new Frame();
        PushLayer(m_Frame);




        m_ServerLayer->SetServerConnectedCallback([&](std::string ip) {
            ATLOG_INFO("Connected to client: {0}", ip);
            m_Frame->PushNewVideoWriterWithIP(ip);
        });



        // //id 1 , speed
        m_ServerLayer->RegisterMessageWithID(1, [&](Message message) {
            float speed = *(float*)message.payload;
            ATLOG_INFO("Message Received: ID = 1 {0}", speed);
            if (speed >= -50.0 && speed <= 50.0) {
                std::stringstream commandStream;
                commandStream << std::fixed << std::setprecision(2) << "#1:" << speed << ";;";
                std::string command = commandStream.str();
                m_SerialCommunication->SendData(command);
            } else {
                ATLOG_ERROR("Invalid speed value: {0}", speed);
            }
        });
        //id 2 , angle
        m_ServerLayer->RegisterMessageWithID(2, [&](Message message) {
            float angle = *(float*)message.payload;
            ATLOG_INFO("Message Received: ID = 2 {0}", angle);
            if (angle >= -50.0 && angle <= 50.0) {
                std::stringstream commandStream;
                commandStream << std::fixed << std::setprecision(2) << "#2:" << angle << ";;";
                std::string command = commandStream.str();
                m_SerialCommunication->SendData(command);
            } else {
                ATLOG_ERROR("Invalid angle value: {0}", angle);
            }
        });

        //id 50 , string camera pipeline
        m_ServerLayer->RegisterMessageWithID(50, [&](Message message) {
            std::string pipeline = (char*)message.payload;
            ATLOG_INFO("Message Received: ID = 50 {0}", pipeline);
            if(!m_IsCameraOpen){
                m_Frame->OpenCamera(pipeline);
                m_IsCameraOpen = true;
            }
            Message response;
            response.id = 50;
            response.payloadSize = 2;
            response.payload = (void*)"OK";
            m_ServerLayer->SendMessage(response);

        });









    }


    Application::~Application() {

        if (m_ServerLayer) {
            delete m_ServerLayer;
        }
        if (m_Frame) {
            delete m_Frame;
        }

        if (m_SerialCommunication) {
            delete m_SerialCommunication;
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
        m_SerialCommunication->SendData("#1:0.0;;");
        m_SerialCommunication->SendData("#2:0.0;;");
        ATLOG_WARN("Motor Stopped");
        m_IsRuning = false;
    }


    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::SignalHandler(int signal) {
        // Handle different types of signals, e.g., SIGINT, SIGTERM
        if (signal == SIGINT || signal == SIGTERM) {
            std::cout << "Signal (" << signal << ") received. Shutting down." << std::endl;
            Application::GetApp().WindowClose(); // Set the running flag to false
        }
    }
}
