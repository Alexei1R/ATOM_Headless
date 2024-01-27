//
// Created by toor on 12/27/23.
//

#ifndef FRAME_H
#define FRAME_H

#include "ATOM/atompch.h"
#include "opencv2/opencv.hpp"


namespace Atom {
    class Frame : public Layer {
    public:
        Frame();
        ~Frame();

        void OpenCamera(std::string source,int apiPreference = cv::CAP_ANY);

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;

        void PushNewVideoWriterWithIP(std::string ip);
    private:
        void VideoProcessingThread();

    private:

        std::atomic<bool> m_ThreadIsRunning = true;
        std::thread m_VideoThread;
        std::mutex frameCloneMutex;

        int m_FrameWidth = 640;
        int m_FrameHeight = 480;
        cv::Mat frame;
        cv::VideoCapture cap;
        std::vector<cv::VideoWriter> m_VideoWriters;





    };


}


#endif //FRAME_H
