//
// Created by karol on 21/11/2026.
//
#pragma once
#ifndef MOTION_DETECTOR_CONNECTEDCOMPONENTS_H
#define MOTION_DETECTOR_CONNECTEDCOMPONENTS_H
#include <vector>
#include <opencv2/core.hpp>


class ConnectedComponents {


public:
    ConnectedComponents();
    struct Box {
        int x;
        int y;
        int width;
        int height;
        int area;
    };
    std::vector<Box> findObjects(const cv::Mat& mask, int minArea);


};



#endif //MOTION_DETECTOR_CONNECTEDCOMPONENTS_H
