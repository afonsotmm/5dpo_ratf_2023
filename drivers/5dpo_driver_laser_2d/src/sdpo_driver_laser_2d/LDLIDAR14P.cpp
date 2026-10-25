//
// Created by afonsotmm on 10/24/26.
//

#include "sdpo_driver_laser_2d/LDLIDAR14P.h"

#include <iostream>
#include <ostream>

#include "sdpo_driver_laser_2d/utils.h"

// from https://github.com/ldrobotSensorTeam/ldlidar_ros/blob/master/src/main.cpp
namespace {
    uint64_t GetTimestamp(void) {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp =
          std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
        auto tmp = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
        return ((uint64_t)tmp.count());
    }
}

namespace sdpo_driver_laser_2d {

    LDLIDAR14P::~LDLIDAR14P() {
        stop();

        if (ldlidar_drv_ != nullptr) {

            ldlidar_drv_->Disconnect();

            ldlidar::LDLidarDriverLinuxInterface::Destory(
                ldlidar_drv_);

            ldlidar_drv_ = nullptr;
        }
    }

    bool LDLIDAR14P::initialize(const bool dbg) {

        ldlidar_drv_ = ldlidar::LDLidarDriverLinuxInterface::Create();
        if (ldlidar_drv_==nullptr) return false;

        ldlidar_drv_->RegisterGetTimestampFunctional(std::bind(&GetTimestamp));
        ldlidar_drv_->EnablePointCloudDataFilter(true);

        if (ldlidar_drv_->Connect(lidar_type_, serial_port_name_, baud_rate_)) {
            std::cout << "ldlidar serial connect is success" << std::endl;
        } else {
            std::cout << "ldlidar serial connect is fail" << std::endl;
            return false;
        }

        if (ldlidar_drv_->WaitLidarComm(3500)) {
            std::cout << "ldlidar communication is normal." << std::endl;
        } else {
            std::cout << "ldlidar communication is abnormal." << std::endl;
            return false;
        }

        return true;
    }

    void LDLIDAR14P::start() {
        if (running_) return;
        if (ldlidar_drv_ == nullptr) return;
        if (!ldlidar_drv_->Start()) {
            std::cout << "ldlidar start is fail" << std::endl;
            return;
        }

        std::cout << "ldlidar start is success" << std::endl;
        running_ = true;

        // start scans acquisition thread
        acquisition_thread_ = std::thread(&LDLIDAR14P::acquisitionLoop, this);
    }

    void LDLIDAR14P::acquisitionLoop() {

        ldlidar::Points2D laser_scan_points;

        while (running_) {
            switch (ldlidar_drv_->GetLaserScanData(laser_scan_points, 1500)) {

                case ldlidar::LidarStatus::NORMAL:
                    std::cout
                        << "LDLiDAR scan received: "
                        << laser_scan_points.size()
                        << " points"
                        << std::endl;
                    break;

                case ldlidar::LidarStatus::DATA_TIME_OUT:
                    std::cerr
                        << "LDLiDAR data timeout."
                        << std::endl;
                    break;

                case ldlidar::LidarStatus::DATA_WAIT:
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(1));
                    break;

                default:
                    break;
            }
        } // end while
        
    }

    void LDLIDAR14P::stop() {
        if (!running_) return;
        running_ = false;

        if (acquisition_thread_.joinable()) acquisition_thread_.join();
        if (ldlidar_drv_!=nullptr) ldlidar_drv_->Stop();

        std::cout << "ldlidar stop is success" << std::endl;
    }

    void LDLIDAR14P::restart() {
        stop();
        start();
    }

}

