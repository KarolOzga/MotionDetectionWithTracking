# Motion Detector

A small C++17 project that detects moving objects in a video from a static camera.
The core algorithms (background model, connected components) are implemented from scratch;
OpenCV is used only for reading the video, grayscale conversion and drawing.

> **Status:** work in progress. Tracking is planned.

<!-- TODO: add a GIF/screenshot of the result here, e.g. docs/demo.gif -->

## How it works

Each frame goes through the following pipeline:

1. **Grayscale conversion.** The frame is converted to a single 8-bit channel, so every
   pixel is one brightness value (0-255) instead of three colour values.
2. **Background model** (`BackgroundModel`). For every pixel the program keeps a running
   average of its brightness:

   ```
   background = (1 - alpha) * background + alpha * pixel
   ```

   A pixel is marked as motion when `|pixel - background| > threshold`. The result is a
   binary mask (255 = motion, 0 = background). Slow lighting changes are absorbed by the
   background, fast movement is not.
3. **Connected components** (`ConnectedComponents`). White pixels of the mask are grouped
   into objects with a BFS over 8-neighbourhood. For every object the program computes a
   bounding box and its area (number of pixels). Objects smaller than `minArea` are
   discarded as noise.
4. **Visualisation.** Bounding boxes are drawn on the original frame.

### Complexity

Both the background update and the connected components search visit every pixel a
constant number of times, so one frame costs O(width * height).

### Parameters

| Parameter   | Meaning                                              | Current value |
|-------------|------------------------------------------------------|---------------|
| `alpha`     | how fast the background adapts (0-1)                 | 0.02          |
| `threshold` | minimum brightness difference counted as motion      | 25            |
| `minArea`   | minimum object size in pixels                        | <!-- TODO --> |

## Project structure

```
MotionDetector/
├── CMakeLists.txt
├── include/
│   ├── BackgroundModel.h
│   └── ConnectedComponents.h
├── src/
│   ├── main.cpp
│   ├── BackgroundModel.cpp
│   └── ConnectedComponents.cpp
└── tests/
    └── test_connected_components.cpp
```

The logic lives in a library (`motion_lib`) that is linked both into the program and into
the tests.

## Build and run

Requirements: a C++17 compiler, CMake 3.16+, OpenCV. On Fedora:

```bash
sudo dnf install gcc-c++ cmake opencv-devel
```

Build:

```bash
cmake -S . -B build
cmake --build build
```

Run on a video file (press `ESC` to quit):

```bash
./build/motion_detector path/to/video.mp4
```

## Tests

Unit tests use GoogleTest (downloaded automatically by CMake). They check the connected
components code against small hand-made masks with known results.

```bash
cd build
ctest --output-on-failure
```

## Known limitations

- The camera must be static. Camera movement turns the whole frame into "motion".
- Shadows are detected as motion, because they change pixel brightness.
- One object can be split into several boxes when parts of it have a brightness close to
  the background (for example windows of a car).
- Moving foliage and flickering light produce small false detections.
