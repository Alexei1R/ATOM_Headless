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

        std::string pipeline = "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
        // cap.open(pipeline, cv::CAP_GSTREAMER);
        cap.open("/home/toor/Downloads/pc.mp4");
        if (!cap.isOpened()) {
            ATLOG_WARN("Error opening the camera");
        }


        m_Server = new Server(27020);
        m_Server->Start();
        m_Server->SetClientConnectedCallback([&](const ClientInfo& info) {
            // ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
                m_VideoWriters.push_back(cv::VideoWriter(
                    "appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=" + info.IP + " port=5000",
                    0, cap.get(cv::CAP_PROP_FPS),
                    cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true));
        });
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


    Application::~Application() {

        if (m_Server) {
            delete m_Server;
        }


    }


    void Application::Run() {
        while (m_IsRuning) {
            for (Layer* layer: m_LayerStack) {
                layer->OnUpdate();
            }
            std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime =
                    std::chrono::high_resolution_clock::now();
            std::chrono::duration<float, std::milli> m_TimeSpan = m_CurrentTime - m_LastTime;

            if (m_TimeSpan.count() >= m_Interval.count()) {
                for (Layer* layer: m_LayerStack) {
                    layer->OnFixedUpdate();
                }
                m_LastTime = m_CurrentTime;
            }
            cap >> frame;
            if (frame.empty()) {
                std::cout << "End of video stream or file" << std::endl;
                break;
            }
            for(auto& writer: m_VideoWriters) {
                writer.write(frame);

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
