//
// Created by toor on 11/14/23.
//
#include "Application.h"



namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

        m_Interval = std::chrono::milliseconds(250);

        m_SerialCommunication = new SerialCommunication("/dev/ttyUSB0", 9600);
        PushLayer(m_SerialCommunication);





    }


    Application::~Application()
    {

    }





    void Application::Run()
    {


        while (m_IsRuning)
        {
            for (Layer *layer: m_LayerStack) {
                layer->OnUpdate();
            }
            std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float, std::milli> m_TimeSpan = m_CurrentTime - m_LastTime;

            if (m_TimeSpan.count() >= m_Interval.count()) {
                for(Layer *layer: m_LayerStack) {
                    layer->OnFixedUpdate();
                }
                m_LastTime = m_CurrentTime;
            }

        }
    }


    void Application::WindowClose() {
        m_IsRuning = false;
    }







    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
    }

}