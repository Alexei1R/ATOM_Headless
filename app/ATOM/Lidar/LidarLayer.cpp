//
// Created by toor on 2/25/24.
//

#include "LidarLayer.h"


namespace Atom {
    LidarReadLayer::LidarReadLayer(std::string port)
            : Layer("LidarReadLayer"), m_Port(port)
    {



        m_Running = true;
        m_LidarThread = std::thread(&LidarReadLayer::ReadLidarDataThread, this);



    }

    LidarReadLayer::~LidarReadLayer() {
        if (m_Running) {
            m_Running = false;
            if (m_LidarThread.joinable()) {
                m_LidarThread.join();
            }
        }

    }

    void LidarReadLayer::OnAttach() {}

    void LidarReadLayer::OnDetach() {}

    void LidarReadLayer::OnUpdate() {
//        if(isOnline) {
//            std::cout << "Lidar is online" << std::endl;
//        } else {
//            std::cout << "Lidar is offline" << std::endl;
//        }

    }

    void LidarReadLayer::OnFixedUpdate() {

    }

    void LidarReadLayer::interpolateTo360Points(const std::vector<std::pair<float, float>>& inputPoints,
                                                std::vector<std::pair<float, float>>& outputPoints) {
        const int targetPoints = 360;

        // Ensure that inputPoints is not empty
        if (inputPoints.empty()) {
            outputPoints.clear();
            return;
        }

        // Sort input points based on angle
        std::vector<std::pair<float, float>> sortedPoints = inputPoints;
        std::sort(sortedPoints.begin(), sortedPoints.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        // Interpolate between input points to get target number of points
        outputPoints.clear();
        outputPoints.reserve(targetPoints);

        for (int i = 0; i < targetPoints; ++i) {
            float targetAngle = i * 2 * M_PI / targetPoints;

            // Find the two nearest points in sortedPoints
            auto it = std::lower_bound(sortedPoints.begin(), sortedPoints.end(), targetAngle,
                                       [](const auto& a, float b) { return a.first < b; });

            // Check if the iterator is valid
            if (it != sortedPoints.begin() && it != sortedPoints.end()) {
                auto& p1 = *(it - 1);
                auto& p2 = *it;

                // Linear interpolation between p1 and p2
                float t = (targetAngle - p1.first) / (p2.first - p1.first);
                float interpolatedRange = (1.0 - t) * p1.second + t * p2.second;

                outputPoints.push_back(std::make_pair(targetAngle, interpolatedRange));
            }
        }
    }

    void LidarReadLayer::ReadLidarDataThread() {


        m_Device = new ordlidar::OrdlidarDriver(ORADAR_TYPE_SERIAL , ORADAR_MS200);
        m_Device->SetSerialPort(m_Port.c_str() ,baudrate);

        std::cout << "Attempting to connect to LIDAR at port: " << m_Port << " with baudrate: " << baudrate << std::endl;

        if (m_Device->Connect()) {
            std::cout << "LIDAR connected successfully." << std::endl;
        } else {
            std::cerr << "Failed to connect to LIDAR." << std::endl;
            return;
        }

        double min_thr = (double)m_MotorSpeed - ((double)m_MotorSpeed * 0.1);
        double max_thr = (double)m_MotorSpeed + ((double)m_MotorSpeed * 0.1);
        double cur_speed = m_Device->GetRotationSpeed();
        if (cur_speed < min_thr || cur_speed > max_thr) {
            m_Device->SetRotationSpeed(m_MotorSpeed);
        }

        double largestDistance = 0.0;
        while (m_Running) {
            if (m_Device->GrabFullScanBlocking(m_ScanData, 4000)) {
                isOnline = true;
                largestDistance = 0.0;

                std::vector<std::pair<float, float>> localCoordinatesList;

                for (int i = 0; i < m_ScanData.vailtidy_point_num; i++) {
                    double range = m_ScanData.data[i].distance;
                    double angle = m_ScanData.data[i].angle *  M_PI / 180.0;
                    angle += M_PI;// / 2.0;

                    // Ensure the angle is within [0, 2*pi)
                    angle = fmod(angle, 2.0 * M_PI);
                    localCoordinatesList.push_back(std::make_pair(angle, range));
                }

                // Interpolate to get 360 points
                std::vector<std::pair<float, float>> interpolatedCoordinates;
                interpolateTo360Points(localCoordinatesList, interpolatedCoordinates);




                // Lock the mutex before updating the shared resource
                std::lock_guard<std::mutex> lock(m_CoordinatesListMutex);
                m_CoordinatesList = interpolatedCoordinates;
                m_LargestDistance = largestDistance;
            }
        }

    }

}
