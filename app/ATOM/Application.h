//
// Created by toor on 11/14/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H
#include "ATOM/atompch.h"
#include "Networking/Client.h"
#include "ATOM/Core/Logging/Logging.h"

namespace Atom {
    class Application {
    public:
        Application();
        ~Application();
        void Run();
        static Application* s_Instance;
        inline static Application& GetApp() { return *s_Instance; }
        void WindowClose();
    private:
        bool m_IsRuning = true;
    };
}



#endif //ATOM_APPLICATION_H
