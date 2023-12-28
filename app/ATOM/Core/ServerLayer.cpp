//
// Created by toor on 12/28/23.
//

#include "ServerLayer.h"

Atom::ServerLayer::ServerLayer(int port) {


    m_Server = new Server(port);
    m_Server->Start();
    m_Server->SetClientConnectedCallback([&](const ClientInfo& info) {
        if(m_ServerConnectedCallback){
            m_ServerConnectedCallback(info.IP);
        }

    });
    m_Server->SetClientDisconnectedCallback([&](const ClientInfo& info) {
        ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);

    });

    m_Server->SetDataReceivedCallback([&](const ClientInfo& info, const void* data, unsigned int size) {
        ATLOG_INFO("Data Received from {0}: {1} bytes", info.ConnectionDesc, size);

        try {
            if (size < sizeof(uint8_t) + sizeof(size_t)) {
                throw std::runtime_error("Received data is too small to contain a valid message.");
            }
            const char* buffer = static_cast<const char*>(data);
            uint8_t messageID = *buffer;
            //std::vector<std::map<uint8_t,std::function<void(Message message)>>> m_RegistedMessageCallbacks;

            for (auto map : m_RegistedMessageCallbacks) {
                if (map.find(messageID) != map.end()) {
                    Message message;
                    message.id = messageID;
                    message.payloadSize = size - sizeof(uint8_t) - sizeof(size_t);
                    message.payload = (void*)(buffer + sizeof(uint8_t) + sizeof(size_t));
                    map[messageID](message);
                }
            }

        } catch (std::exception& e) {
            ATLOG_ERROR("Error: {0}", e.what());
        }
    });
}

Atom::ServerLayer::~ServerLayer() {
}

void Atom::ServerLayer::OnAttach() {
}

void Atom::ServerLayer::OnDetach() {
}

void Atom::ServerLayer::OnUpdate() {
}

void Atom::ServerLayer::OnFixedUpdate() {


}

void Atom::ServerLayer::SendMessage(const Message& message) {
    size_t totalSize = sizeof(message.id) + sizeof(message.payloadSize) + message.payloadSize;

    // Allocate buffer for the entire message
    char* buffer = new char[totalSize];

    // Copy data to buffer
    std::memcpy(buffer, &message.id, sizeof(message.id));
    std::memcpy(buffer + sizeof(message.id), &message.payloadSize, sizeof(message.payloadSize));
    std::memcpy(buffer + sizeof(message.id) + sizeof(message.payloadSize), message.payload, message.payloadSize);

    // Send the buffer
    m_Server->SendDataPointerToClient(buffer, totalSize);

    // Free the allocated buffer after sending
    delete[] buffer;
}


