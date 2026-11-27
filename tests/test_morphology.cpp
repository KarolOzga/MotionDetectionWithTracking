//
// Created by karol on 27/11/2026.
//


#include "Morphology.h"
#include <gtest/gtest.h>

TEST(Morphology, ErodesSinglePixel) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    mask.at<uint8_t>(2, 2) = 255;

    const auto eroded = Morphology::erode(mask, 2);


    ASSERT_EQ(cv::countNonZero(eroded), 0);
}

TEST(Morphology, DilatesSinglePixel) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    mask.at<uint8_t>(2, 2) = 255;

    const auto dilated = Morphology::dilate(mask, 2);

    ASSERT_EQ(cv::countNonZero(dilated), 9);
}

TEST(Morphology, ErodesSquare) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    mask.at<uint8_t>(1, 1) = 255;
    mask.at<uint8_t>(1, 2) = 255;
    mask.at<uint8_t>(2, 1) = 255;
    mask.at<uint8_t>(2, 2) = 255;

    const auto eroded = Morphology::erode(mask, 2);

    ASSERT_EQ(cv::countNonZero(eroded), 0);
}

TEST(Morphology, OpeningRemovesIsolatedPixel) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    mask.at<uint8_t>(2, 2) = 255;

    const auto opened = Morphology::open(mask, 3);

    EXPECT_EQ(cv::countNonZero(opened), 0);
}

TEST(Morphology, ClosingFillsSmallHole) {
    cv::Mat mask(5, 5, CV_8UC1, cv::Scalar(255));
    mask.at<uint8_t>(2, 2) = 0;

    const auto closed = Morphology::close(mask, 3);

    EXPECT_EQ(cv::countNonZero(closed), 25);
}
