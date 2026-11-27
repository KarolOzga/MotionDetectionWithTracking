#include "BackgroundModel.h"

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector>

BackgroundModel::BackgroundModel(float alpha, int threshold): alpha_(alpha), threshold_(threshold) {
    if (alpha < 0.0f || alpha > 1.0f) {
        throw std::invalid_argument("alpha must be between 0 and 1");
    }
}

cv::Mat BackgroundModel::update(const cv::Mat& gray) {
    if (gray.empty() || gray.type() != CV_8UC1) {
        throw std::invalid_argument("BackgroundModel expects a non-empty grayscale CV_8UC1 image");
    }

    if (background_.empty()) {
        background_.resize(gray.cols * gray.rows);
        for (int y = 0; y < gray.rows; ++y) {
            const uchar* row = gray.ptr<uchar>(y);
            for (int x = 0; x < gray.cols; ++x) {
                background_[y * gray.cols + x] = row[x];
            }
        }
    } else if (background_.size() != static_cast<size_t>(gray.cols * gray.rows)) {
        throw std::invalid_argument("Frame dimensions cannot change");
    }

    cv::Mat mask = cv::Mat::zeros(gray.rows, gray.cols, CV_8UC1);
    for (int y = 0; y < gray.rows; ++y) {
        const uchar* row = gray.ptr<uchar>(y);
        uchar* mask_row = mask.ptr<uchar>(y);
        for (int x = 0; x < gray.cols; ++x) {
            float& background = background_[y * gray.cols + x];
            const float difference = std::abs(static_cast<float>(row[x]) - background);
            mask_row[x] = (difference > threshold_) ? 255 : 0;
            background = (1.0f - alpha_) * background + alpha_ * row[x];
        }
    }

    return mask;
}

void BackgroundModel::initialize(const std::vector<cv::Mat>& frames) {
    if (frames.empty()) return;
    const size_t size = frames.front().total();
    background_.resize(size);
    std::vector<uchar> values(frames.size());
    for (size_t i = 0; i < size; ++i) {
        for (size_t k = 0; k < frames.size(); ++k) values[k] = frames[k].data[i];
        auto middle = values.begin() + values.size() / 2;
        std::nth_element(values.begin(), middle, values.end());
        background_[i] = *middle;
    }
}