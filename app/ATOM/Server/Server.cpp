//
// Created by toor on 12/9/23.
//
#include "Server.h"

namespace Atom {
    ServerLayer::ServerLayer(unsigned int port)
            : Layer("ServerLayer") {
        m_Server = new Server(port);
        m_Server->Start();
        m_Server->SetClientConnectedCallback([&](const ClientInfo &info) {
            ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
        });
        m_Server->SetClientDisconnectedCallback([&](const ClientInfo &info) {
            ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);
        });

        m_Server->SetDataReceivedCallback([&](const ClientInfo &info, const void *data, unsigned int sizeofmessage) {
            ATLOG_INFO("Data Received: {0}", info.ConnectionDesc);
            Message message;
            memcpy(&message, data, sizeofmessage);
            for (auto &callback : m_MessageCallbacks) {
                if (callback.first == message.id) {
                    callback.second(message.data, sizeofmessage - sizeof(message.id));
                }
            }



        });

    }

    ServerLayer::~ServerLayer() {
        if (m_Server != nullptr) {
            delete m_Server;
        }
    }

    void ServerLayer::OnAttach() {}

    void ServerLayer::OnDetach() {}

    void ServerLayer::OnUpdate() {
    }

    void ServerLayer::OnFixedUpdate() {
    }

    void ServerLayer::SendMessage(Message message) {
        m_Server->SendDataToAllClients(message.data, sizeof(message.data));
    }

}
