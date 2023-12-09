//
// Created by toor on 11/14/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H
#include "ATOM/atompch.h"
#include "ATOM/Core/Logging/Logging.h"
#include "ATOM/EmbededPlatform/SerialCommunication.h"
#include "ATOM/Server/Server.h"



namespace Atom {
    class Application {
    public:
        Application();
        ~Application();
        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        static Application* s_Instance;
        inline static Application& GetApp() { return *s_Instance; }
        void WindowClose();
    private:
        LayerStack m_LayerStack;
        bool m_IsRuning = true;



        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds m_Interval; // 250 ms


        SerialCommunicationLayer* m_SerialCommunication;
        ServerLayer* m_ServerLayer;

    };
}



#endif //ATOM_APPLICATION_H
