//
// Created by toor on 12/9/23.
//

#ifndef ATOM_SERIALCOMMUNICATION_H
#define ATOM_SERIALCOMMUNICATION_H
#include "ATOM/atompch.h"
#include "Communication/Serial/Serial.h"


namespace Atom {
    class SerialCommunicationLayer : public Layer {
    public:
        SerialCommunicationLayer(std::string port, int baudrate);
        ~SerialCommunicationLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;

        void SendData(std::string data);



    private:
        serial::Serial* m_Serial;
        std::string m_AccumulatedData;
        std::string m_AvailableSerialPorts;
    };


}


#endif //ATOM_SERIALCOMMUNICATION_H
