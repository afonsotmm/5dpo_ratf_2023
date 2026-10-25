//
// Created by afonsotmm on 10/25/26.
//
#include "sdpo_driver_laser_2d/LDLIDAR14P.h"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    sdpo_driver_laser_2d::LDLIDAR14P lidar;

    lidar.setSerialPortParam("/dev/ttyUSB0", 230400);

    if (!lidar.initialize()) {
        std::cerr << "Failed to initialize LD14P" << std::endl;
        return 1;
    }

    lidar.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    lidar.stop();

    return 0;
}