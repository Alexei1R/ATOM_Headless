//
// Created by toor on 5/16/24.
//


#include "Autonomous.h"
#include "FindLines.h"


namespace Atom
{
    Autonomous::Autonomous(SerialCommunicationLayer* serialCommunication, ServerLayer* serverLayer, Frame* frame)
        : Layer("Autonomous"), m_SerialCommunication(serialCommunication), m_ServerLayer(serverLayer), m_Frame(frame)
    {
        m_FindLines = new FindLines();


        m_Pid = new PID(PID_KP, PID_KI, PID_KD);

        //register message id 83 to start and stop autonomous
        m_ServerLayer->RegisterMessageWithID(83, [&](Message message)
        {
            int data = *static_cast<int*>(message.payload);
            if (data == 1)
            {
                ATLOG_WARN("Start Autonomous");
                StartAutonomous = true;
            }
            else if (data == 2)
            {
                ATLOG_WARN("Stop Autonomous");
                StartAutonomous = false;
            }
            else
            {
                ATLOG_ERROR("Invalid data: {0}", data);
            }
        });
    }

    Autonomous::~Autonomous()
    {
    }

    void Autonomous::OnAttach()
    {
    }

    void Autonomous::OnDetach()
    {
    }

    void Autonomous::OnUpdate()
    {
    }

    void Autonomous::OnFixedUpdate()
    {
        if(m_ServerLayer->IsServerRunning() && m_ServerLayer->IsAnyClientConnected()){
            ComputePid();


            //id 84 left line points
            if (StartAutonomous)
            {
                auto ptsLeft = m_FindLines->GetLinePoints();
                if (!ptsLeft.empty())
                {
                    Message message;
                    message.id = 84;
                    message.payloadSize = ptsLeft.size() * sizeof(cv::Point2i);
                    message.payload = static_cast<void*>(ptsLeft.data());
                    m_ServerLayer->SendMessage(message);
                }
            }
        }

    }

    void Autonomous::ComputePid()
    {
        if (StartAutonomous)
        {
            m_LocalFrame = m_Frame->GetFrame();
            if(!m_LocalFrame.empty()){
                m_FindLines->PreprocessLine(m_LocalFrame);
            }
            if (!m_LocalFrame.empty())
            {
                auto currentTimePid = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> timeSpanPid =
                    currentTimePid - lastTimePid;
                if (timeSpanPid.count() > 50)
                {
                    lastTimePid = std::chrono::high_resolution_clock::now();
                    float deltaTime = timeSpanPid.count() / 1000.0f;
                    m_PidOut = m_Pid->calculate(0, m_FindLines->PreprocessLine(m_LocalFrame) / 2,
                                                deltaTime, m_MaxSteering, -m_MaxSteering);
                    m_PidOut = std::clamp(m_PidOut, -m_MaxSteering, m_MaxSteering);
                    // ATLOG_INFO("PID Out: {0}", m_PidOut)
                }
            }

            auto currentTimeComand = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> timeSpanComand =
                currentTimeComand - lastTimeCommand;
            if (timeSpanComand.count() > 25)
            {
                lastTimeCommand = std::chrono::high_resolution_clock::now();
                //alterate between speed and steering
                if (m_Alterate)
                {
                    ComandCarSpeed(m_MaxSpeed);
                    m_Alterate = false;
                }
                else
                {
                    ComandCarSteering(m_PidOut);
                    m_Alterate = true;
                }
            }
        }
    }

    void Autonomous::SetLineSettings(LineSettings settings)
    {
        m_FindLines->SetLineSettings(settings);
    }

    void Autonomous::ComandCarSpeed(float speed)
    {
        if (speed >= -20.0 && speed <= 20.0)
        {
            std::stringstream commandStream;
            commandStream << std::fixed << std::setprecision(2) << "#1:" << speed << ";;";
            std::string command = commandStream.str();
            m_SerialCommunication->SendData(command);
        }
        else
        {
            ATLOG_ERROR("Invalid speed value: {0}", speed);
        }
    }

    void Autonomous::ComandCarSteering(float angle)
    {
        if (angle >= -50.0 && angle <= 50.0)
        {
            std::stringstream commandStream;
            commandStream << std::fixed << std::setprecision(2) << "#2:" << angle << ";;";
            std::string command = commandStream.str();
            m_SerialCommunication->SendData(command);
        }
        else
        {
            ATLOG_ERROR("Invalid angle value: {0}", angle);
        }
    }
}
