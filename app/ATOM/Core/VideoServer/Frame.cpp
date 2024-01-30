//
// Created by toor on 12/27/23.
//
#include "Frame.h"
#include "ATOM/Application.h"

namespace Atom {
    Frame::Frame() : Layer("Frame") {

    }

    Frame::~Frame() {
        if (m_ThreadIsRunning) {
            m_ThreadIsRunning = false;
            if (m_VideoThread.joinable()) {
                m_VideoThread.join();
            }
        }
    }

    void Frame::PushNewVideoWriterWithIP(std::string ip) {
        m_VideoWriters.push_back(cv::VideoWriter(
                    "appsrc ! videoconvert ! video/x-raw,format=I420 ! jpegenc ! rtpjpegpay ! udpsink host=" + ip + " port=5000",
                    0, cap.get(cv::CAP_PROP_FPS),
                    cv::Size(m_FrameWidth,m_FrameHeight), true));
    }

    void Frame::RemoveVideoWriterWithIP(std::string ip) {
        //remove from verctor video writer with ip
        for (int i = 0; i < m_VideoWriters.size(); i++) {
            if (m_VideoWriters[i].getBackendName() == ip) {
                m_VideoWriters.erase(m_VideoWriters.begin() + i);
                return;
            }
        }
    }

    void Frame::OnAttach() {}

    void Frame::OnDetach() {}

    void Frame::OnUpdate() {
        // if (!frame.empty()) {
        //     cv::imshow("Frame", frame);
        //     cv::waitKey(1);
        // }
    }

    void Frame::OnFixedUpdate() {}

    void Frame::VideoProcessingThread() {
        while (m_ThreadIsRunning) {
            cv::Mat localFrame;
            cap >> localFrame;
            if (localFrame.empty()) {
                std::cout << "End of video stream or file" << std::endl;
                break;
            }
            cv::resize(localFrame, localFrame, cv::Size(m_FrameWidth, m_FrameHeight));
            {
                std::lock_guard<std::mutex> guard();
                frame = localFrame.clone();
            }
            if (m_VideoWriters.size() > 0) {
                for (auto& writer: m_VideoWriters) {
                    writer.write(localFrame);
                }
            }
        }
    }

    void Frame::OpenCamera(std::string source, int apiPreference) {
        // std::string pipeline = "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
        // cap.open(pipeline, cv::CAP_GSTREAMER);
        cap.open(source, apiPreference);
        if (!cap.isOpened()) {
            ATLOG_WARN("Error opening the camera");
        }

        m_VideoThread = std::thread(&Frame::VideoProcessingThread, this);

    }
}