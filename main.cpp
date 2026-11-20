#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

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
    cv::Mat frame, gray, mask;
    std::vector<float> background;

    //alpha parameter tells how fast are we forgetting about "old" background and how fast are we accepting current pixel color
    const float alpha = 0.02f;
    const int threshold = 25;

    while (cap.read(frame)) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        if (background.empty()) {
            background.resize(gray.cols * gray.rows);
            for (int y = 0; y < gray.rows; ++y) {
                for (int x = 0; x < gray.cols; ++x) {
                    background[y * gray.cols + x] = gray.at<uchar>(y, x);
                }
            }
            mask = cv::Mat::zeros(gray.rows, gray.cols, CV_8UC1);
        }
        for (int y = 0; y < gray.rows; ++y) {
            const uchar* row = gray.ptr<uchar>(y);
            uchar* maskRow = mask.ptr<uchar>(y);
            for (int x = 0; x < gray.cols; ++x) {
                float& bg = background[y * gray.cols + x];
                float diff = std::abs(row[x] - bg);
                maskRow[x] = (diff > threshold) ? 255 : 0;
                bg = (1.0f - alpha) * bg + alpha * row[x];
            }
        }
        cv::imshow("Gray", gray);
        cv::imshow("Mask", mask);
        if (cv::waitKey(delay) == 27) break;
    }
}
