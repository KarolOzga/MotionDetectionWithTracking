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

    static std::vector<Box> findObjects(const cv::Mat& mask, int minArea);
    static std::vector<Box> mergeBoxes(const std::vector<Box>& boxes, int mergeDistance);
    static bool areBoxesClose(const Box& box1, const Box& box2, int mergeDistance);


};



#endif //MOTION_DETECTOR_CONNECTEDCOMPONENTS_H
