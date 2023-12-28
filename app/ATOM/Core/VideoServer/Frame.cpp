//
// Created by toor on 12/27/23.
//
#include "Frame.h"
#include "ATOM/Application.h"

namespace Atom {
    Frame::Frame(std::string source, int apiPreference) : Layer("Frame") {
        std::string pipeline = "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
        // cap.open(pipeline, cv::CAP_GSTREAMER);
        cap.open(source, apiPreference);
        if (!cap.isOpened()) {
            ATLOG_WARN("Error opening the camera");
        }

        m_VideoThread = std::thread(&Frame::VideoProcessingThread, this);
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
                    cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), true));
    }

    void Frame::OnAttach() {}

    void Frame::OnDetach() {}

    void Frame::OnUpdate() {
        // Main thread update tasks (if any)
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
}