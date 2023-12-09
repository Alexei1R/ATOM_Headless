//
// Created by toor on 12/9/23.
//

#include "SerialCommunication.h"
#include <sys/stat.h>


namespace Atom {
    SerialCommunication::SerialCommunication(std::string port, int baudrate)
    : Layer("SerialCommunication") {

        //Change permissions
        int result = chmod(port.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // Read/write permissions for user, group, others
        if (result != 0) {
            ATLOG_WARN("Failed to change permissions on [{}]", port);
        } else {
            ATLOG_INFO("Changed permissions on [{}]", port);
        }
        // Initialize the serial port
        try {
            std::vector<serial::PortInfo> devices_found = serial::list_ports();
            if (!devices_found.empty()) {
                //print devices
                for (auto &device : devices_found) {
                    ATLOG_INFO("Device: {} - {}", device.port, device.description);
                }
                //check if the port is in list of devices
                if(std::find_if(devices_found.begin(), devices_found.end(), [&](const serial::PortInfo& p) { return p.port == port; }) == devices_found.end()) {
                    ATLOG_WARN("Port [{}] not found.", port);
                    return;
                }


                m_Serial = new serial::Serial(port, baudrate, serial::Timeout::simpleTimeout(1000));
                if (m_Serial->isOpen()) {
                    ATLOG_INFO("Connected to {}", port);
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

    SerialCommunication::~SerialCommunication() {
        if (m_Serial != nullptr) {
            m_Serial->close();
            delete m_Serial;
        }
    }

    void SerialCommunication::OnAttach() {}

    void SerialCommunication::OnDetach() {}

    void SerialCommunication::OnUpdate() {
        if (m_Serial != nullptr && m_Serial->isOpen()) {
            if (m_Serial->available()) {
                std::string data = m_Serial->read();
                m_AccumulatedData += data;

                size_t start = 0;
                size_t newStart = 0;
                while ((start = m_AccumulatedData.find('@', start)) != std::string::npos) {
                    size_t end = m_AccumulatedData.find(";;", start);
                    if (end != std::string::npos) {
                        std::string token = m_AccumulatedData.substr(start, end - start);
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
                    m_AccumulatedData = m_AccumulatedData.substr(newStart);
                }

                m_Serial->flush();
            }
        }
    }

    void SerialCommunication::OnFixedUpdate() {
    }
}
