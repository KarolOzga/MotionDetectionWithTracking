#ifndef MOTION_DETECTOR_BACKGROUNDMODEL_H
#define MOTION_DETECTOR_BACKGROUNDMODEL_H

#include <opencv2/core.hpp>
#include <vector>

class BackgroundModel {
public:
    BackgroundModel(float alpha, int threshold);
    cv::Mat update(const cv::Mat& gray);

private:
    float alpha_;
    int threshold_;
    std::vector<float> background_;
};

#endif // MOTION_DETECTOR_BACKGROUNDMODEL_H
