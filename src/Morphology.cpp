//
// Created by karol on 25/11/2026.
//
#pragma once
#include <Morphology.h>
#include <stdint.h>
#include <algorithm>
#include <cstdint>

cv::Mat Morphology::dilate(const cv::Mat& input, const int kernelSize) {
    cv::Mat output(input.rows, input.cols, CV_8UC1);
    cv::Mat temp(input.rows, input.cols, CV_8UC1);
    const int halfKernel = kernelSize / 2;

    //vertival
    for (int y = 0; y < input.rows; ++y) {
        auto* tempRow = temp.ptr<uint8_t>(y);
        for (int x = 0; x < input.cols; ++x) {
            uint8_t maxVal = 0;
            for (int dy = -halfKernel; dy <= halfKernel; ++dy) {
                int ny = y + dy;
                if (ny >= 0 && ny < input.rows) {
                    maxVal = std::max(maxVal, input.at<uint8_t>(ny, x));
                }
                if (maxVal == 255) break;
            }
            tempRow[x] = maxVal;
        }
    }
    //horizontal
    for (int y = 0; y < input.rows; ++y) {
        const uint8_t* tempRow = temp.ptr<uint8_t>(y);
        auto* outRow = output.ptr<uint8_t>(y);
        for (int x = 0; x < input.cols; ++x) {
            uint8_t maxVal = 0;
            for (int dx = -halfKernel; dx <= halfKernel; ++dx) {
                int nx = x + dx;
                if (nx >= 0 && nx < temp.cols) {
                    maxVal = std::max(maxVal, tempRow[nx]);
                }
                if (maxVal == 255) break;
            }
            outRow[x] = maxVal;
        }
    }

    return output;
}

cv::Mat Morphology::erode(const cv::Mat& input, const int kernelSize) {

    cv::Mat output(input.rows, input.cols, CV_8UC1);
    cv::Mat temp(input.rows, input.cols, CV_8UC1);

    const int halfKernel = kernelSize / 2;

    //vertical
    for (int y = 0; y < input.rows; ++y) {
        auto* tempRow = temp.ptr<uint8_t>(y);

        for (int x = 0; x < input.cols; ++x) {
            uint8_t minVal = 255;
            for (int dy = -halfKernel; dy <= halfKernel; ++dy) {
                int ny = y + dy;
                if (ny >= 0 && ny < input.rows) {
                    minVal = std::min(minVal, input.at<uint8_t>(ny, x));
                }
                if (minVal == 0) break;
            }
            tempRow[x] = minVal;
        }
    }
    //horizontal
    for (int y = 0; y < input.rows; ++y) {
        const uint8_t* tempRow = temp.ptr<uint8_t>(y);
        auto* outRow = output.ptr<uint8_t>(y);
        for (int x = 0; x < input.cols; ++x) {
            uint8_t minVal = 255;
            for (int dx = -halfKernel; dx <= halfKernel; ++dx) {
                int nx = x + dx;
                if (nx >= 0 && nx < input.cols) {
                    minVal = std::min(minVal, tempRow[nx]);
                }
                if (minVal == 0) break;
            }
            outRow[x] = minVal;
        }
    }
    return output;

}

cv::Mat Morphology::close(const cv::Mat& input, int kernelSize) {
    cv::Mat dilated = dilate(input, kernelSize);
    cv::Mat closed = erode(dilated, kernelSize);
    return closed;
}

cv::Mat Morphology::open(const cv::Mat& input, int kernelSize) {
    cv::Mat eroded = erode(input, kernelSize);
    cv::Mat opened = dilate(eroded, kernelSize);
    return opened;
}
