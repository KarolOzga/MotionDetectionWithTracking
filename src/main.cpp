#include <iostream>
#include <opencv2/opencv.hpp>
#include "BackgroundModel.h"
#include "ConnectedComponents.h"
#include "Morphology.h"
constexpr int ESC_KEY = 27;
constexpr int WORK_WIDTH = 960;

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


    BackgroundModel background_model(0.02f, 35);
    //alpha and threshold values can be adjusted based on the video characteristics

    cv::namedWindow("Mask", cv::WINDOW_NORMAL);
    cv::namedWindow("Gray", cv::WINDOW_NORMAL);
    cv::namedWindow("Frame", cv::WINDOW_NORMAL);
    cv::resizeWindow("Frame", 960, 960);
    cv::resizeWindow("Mask", 960, 960);
    cv::resizeWindow("Gray", 960, 960);
    cv::moveWindow("Gray", 0, 0);
    cv::moveWindow("Mask", 960, 0);
    cv::moveWindow("Frame", 0, 600);

    std::vector<cv::Mat> initialFrames;
    for (int i = 0; initialFrames.size() < 40 && cap.read(frame); ++i) {
        if (i % 6 != 0) continue;
        const double scale = static_cast<double>(WORK_WIDTH) / frame.cols;
        cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
        initialFrames.push_back(gray.clone());
    }
    background_model.initialize(initialFrames);
    cap.set(cv::CAP_PROP_POS_FRAMES, 0);

    while (cap.read(frame)) {
        const double scale = static_cast<double>(WORK_WIDTH) / frame.cols;
        cv::resize(frame, frame, cv::Size(), scale, scale, cv::INTER_AREA);

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

        cv::Mat mask = background_model.update(gray);
        mask = Morphology::open(mask, 3);
        mask = Morphology::close(mask, 9);
        auto boxes = ConnectedComponents::findObjects(mask, 190);
        // boxes = ConnectedComponents::mergeBoxes(boxes, 5);


        for (const auto& box : boxes) {
            cv::rectangle(frame, cv::Rect(box.x, box.y, box.width, box.height), cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Gray", gray);
        cv::imshow("Mask", mask);
        cv::imshow("Frame", frame);
        if (cv::waitKey(delay) == ESC_KEY) break;
    }
}
