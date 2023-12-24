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

        m_SerialCommunication = new SerialCommunicationLayer("/dev/ttyUSB0", 9600);
        PushLayer(m_SerialCommunication);

        m_ServerLayer = new ServerLayer(27020);
        m_ServerLayer->OnClientConnectedCallback([&](const ClientInfo &info) {
            ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
            ATLOG_WARN("IP Address: {0}", info.IP);

            cv::VideoWriter m_VideoWriter;
            m_VideoWriter.open("appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=" + info.IP + " port=5000", 0, cap.get(cv::CAP_PROP_FPS), cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true);
            m_VideoWriters.push_back(m_VideoWriter);

        });
        PushLayer(m_ServerLayer);



        m_ServerLayer->RegisterMessageCallbackWithId(1, [](void * data , unsigned int size) {
            ATLOG_INFO("Message 1 received");


            //cast data to the type you want

        });



        std::string pipeline = "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";



        cap.open(pipeline, cv::CAP_GSTREAMER);

        if (!cap.isOpened()) {
            ATLOG_WARN("Error opening the camera");
        }




        // m_VideoWriter.open("appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 0, cap.get(cv::CAP_PROP_FPS), cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true);






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
            cap >> frame;

            if (frame.empty()) {
                std::cout << "End of video stream or file" << std::endl;
                break;
            }


            for(auto videoWriter : m_VideoWriters) {
                videoWriter.write(frame);
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