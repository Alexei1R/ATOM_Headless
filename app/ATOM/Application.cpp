//
// Created by toor on 11/14/23.
//

#include "Application.h"



namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

        // Initialize the serial port
        try {
            std::vector<serial::PortInfo> devices_found = serial::list_ports();
            if (!devices_found.empty()) {
                //print devices
                for (auto &device : devices_found) {
                    ATLOG_INFO("Device: {} - {}", device.port, device.description);
                }

//                m_Serial = new serial::Serial(devices_found[0].port, 9600, serial::Timeout::simpleTimeout(1000));
                m_Serial = new serial::Serial("/dev/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));
                if (m_Serial->isOpen()) {
                    ATLOG_INFO("Connected to {}", devices_found[0].port);
                }
            } else {
                ATLOG_WARN("No serial ports found.");
                m_Serial = nullptr;
            }
        } catch (const std::exception& e) {
            ATLOG_ERROR("Exception: {}", e.what());
            m_Serial = nullptr;
        }


//
//        std::string pipeline = "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
//
//
//
//        cap.open(pipeline, cv::CAP_GSTREAMER);
//
//        if (!cap.isOpened()) {
//            ATLOG_WARN("Error opening the camera");
//        }
//
//
//
//
//        m_VideoWriter.open("appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000", 0, cap.get(cv::CAP_PROP_FPS), cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true);
//
//
//        m_Server = new Server(27020);
//        m_Server->Start();
//        m_Server->SetClientConnectedCallback([&](const ClientInfo& info) {
//            ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
//        });
//        m_Server->SetClientDisconnectedCallback([&](const ClientInfo& info) {
//            ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);
//        });
//
//        m_Server->SetDataReceivedCallback([&](const ClientInfo& info, const void* data, unsigned int size) {
//            ATLOG_WARN("[CLIENT] : {0} from {1}: size {2} bytes", (char*)data,info.ConnectionDesc, size);
//            if(std::string((char*)data) == "echo") {
//                m_Server->SendDataToClient(info.ID, "Echo msg recived", 5);
//            }
//        });




    }


    Application::~Application()
    {
        if (m_Serial != nullptr) {
            m_Serial->close();
            delete m_Serial;
        }
    }






    void Application::Run()
    {


        while (m_IsRuning)
        {
//            cap >> frame;
//
//            if (frame.empty()) {
//                std::cout << "End of video stream or file" << std::endl;
//                break;
//            }
//
//
//
//            m_VideoWriter.write(frame);

            if (m_Serial != nullptr && m_Serial->isOpen()) {
                if (m_Serial->available()) {
                    std::string data = m_Serial->read();
                    ATLOG_INFO("Received: {}", data);
                    m_Serial->flush();
                }
            }

        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }




}