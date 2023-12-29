//
// Created by toor on 12/9/23.
//

#include "SerialCommunication.h"
#include <sys/stat.h>


namespace Atom {
    SerialCommunicationLayer::SerialCommunicationLayer(std::string port, int baudrate)
    : Layer("SerialCommunicationLayer") {


        // Initialize the serial port
        try {
            std::vector<serial::PortInfo> devices_found = serial::list_ports();
            if (!devices_found.empty()) {
                //print devices
                for (auto &device : devices_found) {
                    m_AvailableSerialPorts += device.port + " || ";
                }
                ATLOG_TRACE("Available serial ports: {}", m_AvailableSerialPorts);
                //check if the port is in list of devices
                if(std::find_if(devices_found.begin(), devices_found.end(), [&](const serial::PortInfo& p) { return p.port == port; }) == devices_found.end()) {
                    ATLOG_WARN("Port [{}] not found in list of available ports.", port);
                    return;
                }
                else{
                    //Change permissions
                    int result = chmod(port.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // Read/write permissions for user, group, others
                    if (result == 0) {
                        ATLOG_CRITICAL("Failed to change permissions on [{}]", port);
                    } else {
                        ATLOG_WARN("Changed permissions on [{}]", port);
                    }
                }


                m_Serial = new serial::Serial(port, baudrate, serial::Timeout::simpleTimeout(1000));
                if (m_Serial->isOpen()) {
                    ATLOG_WARN("Connected to {}", port);
                }
            } else {
                ATLOG_CRITICAL("No serial ports found.");
                m_Serial = nullptr;
            }
        } catch (const std::exception& e) {
            ATLOG_CRITICAL("Exception: {}", e.what());
            m_Serial = nullptr;
        }
    }

    SerialCommunicationLayer::~SerialCommunicationLayer() {

        if (m_Serial != nullptr) {
            m_Serial->close();
            delete m_Serial;
        }
    }

    void SerialCommunicationLayer::OnAttach() {}

    void SerialCommunicationLayer::OnDetach() {}

    void SerialCommunicationLayer::OnUpdate() {
        if (m_Serial != nullptr && m_Serial->isOpen()) {
            if (m_Serial->available()) {
                std::string data = m_Serial->read();
                ATLOG_WARN("Data Received: {}", data);
                m_Serial->flush();
            }
        }
    }

    void SerialCommunicationLayer::OnFixedUpdate() {
    }

    void SerialCommunicationLayer::SendData(std::string data) {
        if (m_Serial != nullptr && m_Serial->isOpen()) {
            m_Serial->write(data + "\r\n");
            ATLOG_WARN("Data Sent: {}", data);
        }
    }
}
