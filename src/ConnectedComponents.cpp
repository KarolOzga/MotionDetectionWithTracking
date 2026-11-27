//
// Created by karol on 21/11/2026.
//

#include "ConnectedComponents.h"
#include <vector>
#include <opencv2/core.hpp>
#include <deque>
#include <algorithm>
#include <cstdint>


ConnectedComponents::ConnectedComponents() = default;


namespace {
    ConnectedComponents::Box findBoxElements(const cv::Mat& mask, std::vector<uint8_t>& visited, int startX,
                                             int startY) {
        //bfs
        struct Point {
            int x = 0;
            int y = 0;
        };
        int minX = startX;
        int minY = startY;
        int maxX = startX;
        int maxY = startY;
        int areaCounter = 1;
        visited[startY * mask.cols + startX] = 1;

        std::deque<Point> Q;
        Point point = {startX, startY};
        Q.push_back(point);
        int rows = mask.rows;
        int cols = mask.cols;
        while (!Q.empty()) {
            Point currentPoint = Q.front();
            Q.pop_front();
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    int newX = currentPoint.x + j;
                    int newY = currentPoint.y + i;
                    if (newX >= 0 && newX < cols && newY >= 0 && newY < rows) {
                        if (visited[newY * cols + newX] == 0 && mask.at<uint8_t>(newY, newX) == 255) {
                            Point newPoint = {newX, newY};
                            maxX = std::max(maxX, newX);
                            maxY = std::max(maxY, newY);
                            minX = std::min(minX, newX);
                            minY = std::min(minY, newY);
                            visited[newY * cols + newX] = 1;
                            areaCounter++;
                            Q.push_back(newPoint);
                        }
                    }
                }
            }
        }
        ConnectedComponents::Box box = {
            minX, minY, maxX - minX + 1, maxY - minY + 1, areaCounter
        };
        return box;
    }
}

std::vector<ConnectedComponents::Box> ConnectedComponents::findObjects(const cv::Mat& mask, int minArea) {
    std::vector<Box> objects;
    std::vector<std::uint8_t> visited(mask.rows * mask.cols, 0);

    for (int i = 0; i < mask.rows; i++) {
        for (int j = 0; j < mask.cols; j++) {
            if (visited[i * mask.cols + j]) continue;

            if (mask.at<uint8_t>(i, j) == 255) {
                Box box = findBoxElements(mask, visited, j, i);
                if (box.area >= minArea) {
                    objects.push_back(box);
                }
            }
        }
    }
    return objects;
}

std::vector<ConnectedComponents::Box>
ConnectedComponents::mergeBoxes(const std::vector<Box>& boxes, int mergeDistance) {
    std::vector<Box> result = boxes;


    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < result.size() && !changed; ++i) {
            for (size_t j = i + 1; j < result.size(); ++j) {
                if (!areBoxesClose(result[i], result[j], mergeDistance)) continue;

                const int x1 = std::min(result[i].x, result[j].x);
                const int y1 = std::min(result[i].y, result[j].y);
                const int x2 = std::max(result[i].x + result[i].width,result[j].x + result[j].width);
                const int y2 = std::max(result[i].y + result[i].height,result[j].y + result[j].height);

                result[i] = {x1, y1, x2 - x1, y2 - y1, result[i].area + result[j].area};
                result.erase(result.begin() + j);
                changed = true;
                break;
            }
        }
    }
    return result;
}


bool ConnectedComponents::areBoxesClose(const Box& box1, const Box& box2, const int mergeDistance) {
    const int left1 = box1.x;
    const int right1 = box1.x + box1.width;
    const int top1 = box1.y;
    const int bottom1 = box1.y + box1.height;

    const int left2 = box2.x;
    const int right2 = box2.x + box2.width;
    const int top2 = box2.y;
    const int bottom2 = box2.y + box2.height;


    const int gapX = std::max(left1, left2) - std::min(right1, right2);
    const int gapY = std::max(top1, top2) - std::min(bottom1, bottom2);

    return gapX <= mergeDistance && gapY <= mergeDistance;
}
