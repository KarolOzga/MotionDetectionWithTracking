#include <iostream>
#include <opencv2/opencv.hpp>
#include "BackgroundModel.h"

constexpr int ESC_KEY = 27;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "\n";
        return 1;
    }
    //Converting video to monochrome to simplify calculations in future
    cv::VideoCapture cap(argv[1]); // open a video file
    if (!cap.isOpened()) {
        std::cerr << "Cannot open the file: " << argv[1] << "\n";
        return 1;
    }
    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = (fps > 0) ? static_cast<int>(1000.0 / fps) : 33;
    cv::Mat frame, gray;
    BackgroundModel background_model(0.02f, 25);

    while (cap.read(frame)) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        const cv::Mat mask = background_model.update(gray);

        cv::imshow("Gray", gray);
        cv::imshow("Mask", mask);
        if (cv::waitKey(delay) == ESC_KEY) break;
    }
}
