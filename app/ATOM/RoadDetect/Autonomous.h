//
// Created by toor on 5/16/24.
//

#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include "ATOM/atompch.h"
#include "ATOM/macros.h"
#include "ATOM/PID/pid.h"
#include "ATOM/RoadDetect/FindLines.h"
#include "ATOM/Application.h"



namespace Atom {
    class Autonomous : public Layer {
    public:
        Autonomous(SerialCommunicationLayer* serialCommunication, ServerLayer* serverLayer, Frame* frame);
        ~Autonomous();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;

        void SetLineSettings(LineSettings settings);



        
    private:


        void ComputePid();


        void ComandCarSpeed(float speed);

        void ComandCarSteering(float angle);



    private:

        bool StartAutonomous = false;

        // PID
        PID* m_Pid;
        // PID Constants
        float PID_KP = 1.55f;
        float PID_KI = 0.75;
        float PID_KD = 0.05f;
        // Pid Out
        float m_PidOut = 0;
        float error;

        // lastLime pid out
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTimePid =
            std::chrono::high_resolution_clock::now();

        //timer to send command to car
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTimeCommand =
            std::chrono::high_resolution_clock::now();
        //bool to alterate between speed and steering
        bool m_Alterate = false;


        // Speed Multiplier
        float m_MaxSpeed = -12;
        float m_MaxSteering = 45;
        float m_OffsetSteering = 0;


        FindLines *m_FindLines;


        SerialCommunicationLayer* m_SerialCommunication;
        ServerLayer* m_ServerLayer;
        Frame* m_Frame;
        cv::Mat m_LocalFrame;



    };


}



#endif //Autonomous_H
