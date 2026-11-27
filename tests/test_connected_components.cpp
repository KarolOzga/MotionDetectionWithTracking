//
// Created by karol on 22/11/2026.
//

#include "ConnectedComponents.h"
#include <gtest/gtest.h>


TEST(ConnectedComponents, FindsPixel) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    mask.at<uint8_t>(2, 2) = 255;

    auto boxes = ConnectedComponents().findObjects(mask, 1);

    ASSERT_EQ(boxes.size(), 1);
    EXPECT_EQ(boxes[0].area, 1);
    EXPECT_EQ(boxes[0].x, 2);
    EXPECT_EQ(boxes[0].y, 2);
    EXPECT_EQ(boxes[0].width, 1);
    EXPECT_EQ(boxes[0].height, 1);
}

TEST(ConnectedComponents, FindsNoneForEmptyMask) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);
    auto boxes = ConnectedComponents().findObjects(mask, 1);

    ASSERT_EQ(boxes.size(), 0);
}

TEST(ConnectedComponents, FindsEdgeProperly) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);

    mask.at<uint8_t>(4, 4) = 255;
    mask.at<uint8_t>(3, 3) = 255;
    mask.at<uint8_t>(3, 4) = 255;
    mask.at<uint8_t>(4, 3) = 255;

    auto boxes = ConnectedComponents().findObjects(mask, 1);

    ASSERT_EQ(boxes.size(), 1);
    EXPECT_EQ(boxes[0].area, 4);
    EXPECT_EQ(boxes[0].x, 3);
    EXPECT_EQ(boxes[0].y, 3);
    EXPECT_EQ(boxes[0].width, 2);
    EXPECT_EQ(boxes[0].height, 2);
}

TEST(ConnectedComponents, TreatsDiagonalPixelsAsConnected) {
    cv::Mat mask = cv::Mat::zeros(5, 5, CV_8UC1);

    mask.at<uint8_t>(0, 0) = 255;
    mask.at<uint8_t>(1, 1) = 255;
    mask.at<uint8_t>(2, 2) = 255;
    mask.at<uint8_t>(3, 3) = 255;
    mask.at<uint8_t>(4, 4) = 255;

    auto boxes = ConnectedComponents().findObjects(mask, 1);

    ASSERT_EQ(boxes.size(), 1);
    EXPECT_EQ(boxes[0].area, 5);
    EXPECT_EQ(boxes[0].x, 0);
    EXPECT_EQ(boxes[0].y, 0);
    EXPECT_EQ(boxes[0].width, 5);
    EXPECT_EQ(boxes[0].height, 5);
}


TEST(ConnectedComponents, FindsTwoSeparateObjects) {
    cv::Mat mask = cv::Mat::zeros(8, 8, CV_8UC1);

    // L
    mask.at<uint8_t>(1, 1) = 255;
    mask.at<uint8_t>(1, 2) = 255;
    mask.at<uint8_t>(2, 1) = 255;

    // 2x2 square
    mask.at<uint8_t>(5, 5) = 255;
    mask.at<uint8_t>(5, 6) = 255;
    mask.at<uint8_t>(6, 5) = 255;
    mask.at<uint8_t>(6, 6) = 255;

    auto boxes = ConnectedComponents().findObjects(mask, 1);

    ASSERT_EQ(boxes.size(), 2);

    EXPECT_EQ(boxes[0].area, 3);
    EXPECT_EQ(boxes[0].x, 1);
    EXPECT_EQ(boxes[0].y, 1);
    EXPECT_EQ(boxes[0].width, 2);
    EXPECT_EQ(boxes[0].height, 2);

    EXPECT_EQ(boxes[1].area, 4);
    EXPECT_EQ(boxes[1].x, 5);
    EXPECT_EQ(boxes[1].y, 5);
    EXPECT_EQ(boxes[1].width, 2);
    EXPECT_EQ(boxes[1].height, 2);
}


TEST(ConnectedComponents, MergesCloseObjects) {
    cv::Mat mask = cv::Mat::zeros(8, 8, CV_8UC1);

    // L
    mask.at<uint8_t>(1, 1) = 255;
    mask.at<uint8_t>(1, 2) = 255;
    mask.at<uint8_t>(2, 1) = 255;

    // 2x2 square
    mask.at<uint8_t>(5, 5) = 255;
    mask.at<uint8_t>(5, 6) = 255;
    mask.at<uint8_t>(6, 5) = 255;
    mask.at<uint8_t>(6, 6) = 255;

    auto boxes = ConnectedComponents::findObjects(mask, 1);
    auto mergedBoxes = ConnectedComponents::mergeBoxes(boxes, 3);

    ASSERT_EQ(mergedBoxes.size(), 1);

    EXPECT_EQ(mergedBoxes[0].area, 7);
    EXPECT_EQ(mergedBoxes[0].x, 1);
    EXPECT_EQ(mergedBoxes[0].y, 1);
    EXPECT_EQ(mergedBoxes[0].width, 6);
    EXPECT_EQ(mergedBoxes[0].height, 6);
}
