//
// Created by toor on 12/9/23.
//

#ifndef ATOM_SERVERLAYER_H
#define ATOM_SERVERLAYER_H

#include "ATOM/atompch.h"
#include "Networking/Server.h"

namespace Atom {

    typedef struct Message {
        uint8_t id;
        void* data;
    } Message;

    class ServerLayer : public Layer {
    public:
        ServerLayer(unsigned int port);
        ~ServerLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;

        void SendMessage(Message message);

        void RegisterMessageCallbackWithId(uint8_t id, std::function<void(void* , unsigned  int )> callback) { m_MessageCallbacks.push_back(std::make_pair(id, callback)); }



    private:
        Server* m_Server;
        std::vector<std::pair<uint8_t,std::function<void(void* data, unsigned  int size)>>> m_MessageCallbacks;
    };


}


#endif //ATOM_SERVERLAYER_H
