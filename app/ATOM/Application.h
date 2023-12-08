//
// Created by toor on 11/14/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H
#include "ATOM/atompch.h"
#include "ATOM/Core/Logging/Logging.h"
#include "Networking/Server.h"
#include "ATOM/Communication/Serial/Serial.h"
#include "opencv2/opencv.hpp"

namespace Atom {
    class Application {
    public:
        Application();
        ~Application();
        void Run();
        static Application* s_Instance;
        inline static Application& GetApp() { return *s_Instance; }
        void WindowClose();
    private:
        bool m_IsRuning = true;

        int frameSizeBytes;
        Server* m_Server;
        cv::Mat frame;
        cv::VideoCapture cap;
        cv::VideoWriter m_VideoWriter;
        serial::Serial* m_Serial;
    };
}



#endif //ATOM_APPLICATION_H
