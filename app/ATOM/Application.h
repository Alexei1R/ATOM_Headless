//
// Created by toor on 11/14/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H
#include "ATOM/atompch.h"
#include "ATOM/Core/Logging/Logging.h"
#include "ATOM/EmbededPlatform/SerialCommunication.h"
#include <opencv2/opencv.hpp>

#include "Networking/Server.h"
#include "ATOM/Core/VideoServer/Frame.h"
#include "ATOM/Core/ServerLayer.h"
#include "ATOM/Lidar/LidarLayer.h"
#include "ATOM/RoadDetect/Autonomous.h"


namespace Atom {
    typedef struct CameraUsers {
        std::string ip;
        bool IsCreatedVideoWriter = false;
    } CameraUsers;



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

        [[nodiscard]] inline Autonomous &GetAutonomous() const { return *m_Autonomous; }


    private:
        static void SignalHandler(int signal);
    private:

        LayerStack m_LayerStack;
        bool m_IsRuning = true;



        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds m_Interval; // 250 ms


        SerialCommunicationLayer* m_SerialCommunication;
        LidarReadLayer* m_LidarLayer;

        std::vector<CameraUsers> m_CameraUsers;
        bool m_IsCameraOpen = false;
        Autonomous* m_Autonomous;

        Atom::ServerLayer* m_ServerLayer;
        Frame* m_Frame;


        int GPIO_PIN = 8;
        int gpioState = 0;
        unsigned long timestamp;

        bool StartAutonomous = false;



    };
}



#endif //ATOM_APPLICATION_H
