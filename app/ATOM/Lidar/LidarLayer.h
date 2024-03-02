//
// Created by toor on 2/25/24.
//

#ifndef ATOM_LIDARLAYER_H
#define ATOM_LIDARLAYER_H

#include "ATOM/atompch.h"
#include "Communication/oradar/ord_lidar_c_api_driver.h"
#include "Communication/oradar/ord_lidar_driver.h"


namespace Atom {
    class LidarReadLayer : public Layer {
    public:
        LidarReadLayer(std::string port);
        ~LidarReadLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate() override;

        std::vector<std::pair<float, float>> GetCoordinatesList() { return m_CoordinatesList; }
        bool IsOnline() { return isOnline; }
        double GetLargestDistance() { return m_LargestDistance; }
    private:
        void ReadLidarDataThread();
        void interpolateTo360Points(const std::vector<std::pair<float, float>>& inputPoints,
                               std::vector<std::pair<float, float>>& outputPoints);

    private:
        int baudrate = 230400;
        std::string m_Port;
        bool isOnline = false;

        ordlidar::OrdlidarDriver* m_Device;
        full_scan_data_st m_ScanData;
        unsigned int m_MotorSpeed = 20;

        std::pair<double, double> m_Coordinates;
        std::vector<std::pair<float, float>> m_CoordinatesList;

        std::thread m_LidarThread;
        std::atomic<bool> m_Running{false};
        std::mutex m_CoordinatesListMutex;

        double m_LargestDistance = 0.0;

    };


}



#endif //ATOM_LIDARLAYER_H
