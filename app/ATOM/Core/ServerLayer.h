//
// Created by toor on 12/28/23.
//

#ifndef SERVERLAYER_H
#define SERVERLAYER_H
#include "ATOM/atompch.h"
#include "Networking/Server.h"

namespace Atom {
    typedef struct Message {
        uint8_t id;
        size_t payloadSize;
        void* payload;
    } Message;

    class ServerLayer :public Layer{
    public:
        using ServerConnectedCallback = std::function<void(std::string ip)>;
        using ServerDisconnectedCallback = std::function<void(std::string ip)>;
    public:
        ServerLayer(int port);
        ~ServerLayer();

        virtual void OnAttach()     override;
        virtual void OnDetach()     override;
        virtual void OnUpdate()     override;
        virtual void OnFixedUpdate()override;

        void SetServerConnectedCallback(const ServerConnectedCallback& function) { m_ServerConnectedCallback = function; }
        void SetServerDisconnectedCallback(const ServerDisconnectedCallback& function) { m_ServerDisconnectedCallback = function; }
        void RegisterMessageWithID(uint8_t id, const std::function<void(Message message)>& function){ m_RegistedMessageCallbacks.push_back({{id,function}}); }


        void SendMessage(const Message& message);
        void SendMessageToClient(const Message& message, std::string ip);


    private:
        Server* m_Server;

        ServerConnectedCallback m_ServerConnectedCallback;
        ServerDisconnectedCallback m_ServerDisconnectedCallback;
        std::vector<std::map<uint8_t,std::function<void(Message message)>>> m_RegistedMessageCallbacks;


    };
}



#endif //SERVERLAYER_H
