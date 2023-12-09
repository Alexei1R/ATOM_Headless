//
// Created by toor on 11/14/23.
//
#include <sys/stat.h> // Include for chmod
#include "Application.h"



namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

        int result = chmod("/dev/ttyUSB0", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // Read/write permissions for user, group, others
        if (result != 0) {
            ATLOG_WARN("Failed to change permissions on /dev/ttyUSB0");
        } else {
            ATLOG_INFO("Changed permissions on /dev/ttyUSB0");
        }


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
        std::string accumulatedData;

        while (m_IsRuning)
        {
            if (m_Serial != nullptr && m_Serial->isOpen()) {
                if (m_Serial->available()) {
                    std::string data = m_Serial->read();
                    accumulatedData += data;

                    size_t start = 0;
                    size_t newStart = 0;
                    while ((start = accumulatedData.find('@', start)) != std::string::npos) {
                        size_t end = accumulatedData.find(";;", start);
                        if (end != std::string::npos) {
                            std::string token = accumulatedData.substr(start, end - start);
                            size_t colonPos = token.find(':');
                            if (colonPos != std::string::npos && colonPos + 1 < token.length()) {
                                std::string id = token.substr(1, colonPos - 1);
                                std::string value = token.substr(colonPos + 1);
                                ATLOG_INFO("ID: {}, Value: {}", id, value);
                            }
                            newStart = end + 2;
                            start = newStart;
                        } else {
                            break;
                        }
                    }

                    if (newStart > 0) {
                        accumulatedData = accumulatedData.substr(newStart);
                    }

                    m_Serial->flush();
                }
            }
        }
    }


    void Application::WindowClose() {
        m_IsRuning = false;
    }




}