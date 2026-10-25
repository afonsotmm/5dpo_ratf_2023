//
// Created by afonsotmm on 10/24/26.
//

#ifndef INC_5DPO_RAFT_LDLIDAR14P_H
#define INC_5DPO_RAFT_LDLIDAR14P_H

#include "sdpo_driver_laser_2d/SdpoDriverLaser2D.h"
#include "ldlidar_driver/ldlidar_driver_linux.h"

#include <chrono>
#include <functional>
#include <iostream>

namespace sdpo_driver_laser_2d {

    class LDLIDAR14P : public SdpoDriverLaser2D {
    private:
        ldlidar::LDLidarDriverLinuxInterface *ldlidar_drv_;
        ldlidar::LDType lidar_type_ = ldlidar::LDType::LD_14P;

    public:
        LDLIDAR14P();
        ~LDLIDAR14P() override;

        bool initialize(const bool dbg = false) override;
        void start();
        void stop();
        void restart();

    };
}

#endif //INC_5DPO_RAFT_LDLIDAR14P_H
