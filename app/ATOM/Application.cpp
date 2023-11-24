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


//        m_Server = new Server(27020);
//        m_Server->Start();
//        m_Server->SetClientConnectedCallback(OnClientConnected);
//        m_Server->SetClientDisconnectedCallback([&](const ClientInfo& info) {
//            ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);
//        });
//
//        m_Server->SetDataReceivedCallback([&](const ClientInfo& info, const void* data, unsigned int size) {
//            ATLOG_INFO("Data Received from {0}: {1} bytes", info.ConnectionDesc, size);
////            also print message as string
//            ATLOG_INFO("Recived message from {0}: {1} bytes", info.ConnectionDesc, (char*)data);
//            m_Server->SendDataToClient(info.ID, std::to_string(frameSizeBytes), 4);
//        });
//
        cap.open("/home/toor/Downloads/pc.mp4");

        if (!cap.isOpened()) {
            ATLOG_WARN("Error opening the camera");
        }




        m_VideoWriter.open("appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 0, cap.get(cv::CAP_PROP_FPS), cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true);

    }


    Application::~Application()
    {
    }






    void Application::Run()
    {


        while (m_IsRuning)
        {
            cap >> frame;

            if (frame.empty()) {
                std::cout << "End of video stream or file" << std::endl;
                break;
            }



            m_VideoWriter.write(frame);


        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }




}