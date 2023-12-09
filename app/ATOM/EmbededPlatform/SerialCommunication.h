//
// Created by toor on 12/9/23.
//

#ifndef ATOM_SERIALCOMMUNICATION_H
#define ATOM_SERIALCOMMUNICATION_H
#include "ATOM/atompch.h"
#include "ATOM/Communication/Serial/Serial.h"


namespace Atom {
    class SerialCommunication : public Layer {
    public:
        SerialCommunication(std::string port, int baudrate);
        ~SerialCommunication();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;



    private:
        serial::Serial* m_Serial;
        std::string m_AccumulatedData;
    };


}


#endif //ATOM_SERIALCOMMUNICATION_H
