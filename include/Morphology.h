//
// Created by karol on 25/11/2026.
//

#ifndef MOTION_DETECTOR_MORPHOLOGY_H
#define MOTION_DETECTOR_MORPHOLOGY_H

#endif //MOTION_DETECTOR_MORPHOLOGY_H

#include <opencv2/core.hpp>

namespace Morphology {
    cv::Mat dilate(const cv::Mat& input, int kernelSize);
    cv::Mat erode(const cv::Mat& input, int kernelSize);
    cv::Mat open(const cv::Mat& input, int kernelSize);
    cv::Mat close(const cv::Mat& input, int kernelSize);
}
