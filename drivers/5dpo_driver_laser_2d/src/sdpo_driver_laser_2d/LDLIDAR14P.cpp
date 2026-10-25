//
// Created by afonsotmm on 10/24/26.
//

#include "sdpo_driver_laser_2d/LDLIDAR14P.h"
#include "sdpo_driver_laser_2d/utils.h"

namespace sdpo_driver_laser_2d {

    // from https://github.com/ldrobotSensorTeam/ldlidar_ros/blob/master/src/main.cpp
    uint64_t GetTimestamp(void) {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp =
          std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
        auto tmp = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
        return ((uint64_t)tmp.count());
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

}



git add *
git commit -m "initialize LDLIDAR14P"
git push