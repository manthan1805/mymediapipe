// Copyright 2020 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <cmath>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_options.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

// add by jacky
// #define DEBUG_PRINT

namespace mediapipe {

namespace {

constexpr char kNormalizedLandmarksTag[] = "NORM_LANDMARKS";
constexpr char kNormRectTag[] = "NORM_RECT";
constexpr char kImageSizeTag[] = "IMAGE_SIZE";
constexpr int kWristJoint = 0;
constexpr int kMiddleFingerPIPJoint = 6;
constexpr int kIndexFingerPIPJoint = 4;
constexpr int kRingFingerPIPJoint = 8;
constexpr float kTargetAngle = M_PI * 0.5f;
#ifdef DEBUG_PRINT
int _initLog = 0;
#endif

inline float NormalizeRadians(float angle) {
  return angle - 2 * M_PI * std::floor((angle - (-M_PI)) / (2 * M_PI));
}

float ComputeRotation(const NormalizedLandmarkList& landmarks,
                      const std::pair<int, int>& image_size) {
  const float x0 = landmarks.landmark(kWristJoint).x() * image_size.first;
  const float y0 = landmarks.landmark(kWristJoint).y() * image_size.second;

  float x1 = (landmarks.landmark(kIndexFingerPIPJoint).x() +
              landmarks.landmark(kRingFingerPIPJoint).x()) /
             2.f;
  float y1 = (landmarks.landmark(kIndexFingerPIPJoint).y() +
              landmarks.landmark(kRingFingerPIPJoint).y()) /
             2.f;
  x1 = (x1 + landmarks.landmark(kMiddleFingerPIPJoint).x()) / 2.f *
       image_size.first;
  y1 = (y1 + landmarks.landmark(kMiddleFingerPIPJoint).y()) / 2.f *
       image_size.second;

  const float rotation =
      NormalizeRadians(kTargetAngle - std::atan2(-(y1 - y0), x1 - x0));
  return rotation;
}

::mediapipe::Status NormalizedLandmarkListToRect(
    const NormalizedLandmarkList& landmarks,
    const std::pair<int, int>& image_size, NormalizedRect* rect) {
  const float rotation = ComputeRotation(landmarks, image_size);
  const float reverse_angle = NormalizeRadians(-rotation);

  // Find boundaries of landmarks.
  float max_x = std::numeric_limits<float>::min();
  float max_y = std::numeric_limits<float>::min();
  float min_x = std::numeric_limits<float>::max();
  float min_y = std::numeric_limits<float>::max();
  for (int i = 0; i < landmarks.landmark_size(); ++i) {
    max_x = std::max(max_x, landmarks.landmark(i).x());
    max_y = std::max(max_y, landmarks.landmark(i).y());
    min_x = std::min(min_x, landmarks.landmark(i).x());
    min_y = std::min(min_y, landmarks.landmark(i).y());
  }
  const float axis_aligned_center_x = (max_x + min_x) / 2.f;
  const float axis_aligned_center_y = (max_y + min_y) / 2.f;
#ifdef DEBUG_PRINT
  {
    char str[1024];
    sprintf(str, "LandmarkDebug max_x(%f), max_y(%f), min_x(%f), min_y(%f), center_x(%f), center_y(%f)\n",
                                    max_x, max_y, 
                                    min_x, min_y,
                                    axis_aligned_center_x, axis_aligned_center_y);
    LOG(INFO) << str;
  }
#endif

  // Find boundaries of rotated landmarks.
  max_x = std::numeric_limits<float>::min();
  max_y = std::numeric_limits<float>::min();
  min_x = std::numeric_limits<float>::max();
  min_y = std::numeric_limits<float>::max();
  for (int i = 0; i < landmarks.landmark_size(); ++i) {
    const float original_x =
        (landmarks.landmark(i).x() - axis_aligned_center_x) * image_size.first;
    const float original_y =
        (landmarks.landmark(i).y() - axis_aligned_center_y) * image_size.second;

    const float projected_x = original_x * std::cos(reverse_angle) -
                              original_y * std::sin(reverse_angle);
    const float projected_y = original_x * std::sin(reverse_angle) +
                              original_y * std::cos(reverse_angle);

    max_x = std::max(max_x, projected_x);
    max_y = std::max(max_y, projected_y);
    min_x = std::min(min_x, projected_x);
    min_y = std::min(min_y, projected_y);
  }
  const float projected_center_x = (max_x + min_x) / 2.f;
  const float projected_center_y = (max_y + min_y) / 2.f;

  const float center_x = projected_center_x * std::cos(rotation) -
                         projected_center_y * std::sin(rotation) +
                         image_size.first * axis_aligned_center_x;
  const float center_y = projected_center_x * std::sin(rotation) +
                         projected_center_y * std::cos(rotation) +
                         image_size.second * axis_aligned_center_y;
  const float width = (max_x - min_x) / image_size.first;
  const float height = (max_y - min_y) / image_size.second;

  rect->set_x_center(center_x / image_size.first);
  rect->set_y_center(center_y / image_size.second);
  rect->set_width(width);
  rect->set_height(height);
  rect->set_rotation(rotation);

  return ::mediapipe::OkStatus();
}

}  // namespace

// A calculator that converts subset of hand landmarks to a bounding box
// NormalizedRect. The rotation angle of the bounding box is computed based on
// 1) the wrist joint and 2) the average of PIP joints of index finger, middle
// finger and ring finger. After rotation, the vector from the wrist to the mean
// of PIP joints is expected to be vertical with wrist at the bottom and the
// mean of PIP joints at the top.
class HandLandmarksToRectCalculator : public CalculatorBase {
 public:
  static ::mediapipe::Status GetContract(CalculatorContract* cc) {
    cc->Inputs().Tag(kNormalizedLandmarksTag).Set<NormalizedLandmarkList>();
    cc->Inputs().Tag(kImageSizeTag).Set<std::pair<int, int>>();
    cc->Outputs().Tag(kNormRectTag).Set<NormalizedRect>();
    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(TimestampDiff(0));
    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status Process(CalculatorContext* cc) override {
    if (cc->Inputs().Tag(kNormalizedLandmarksTag).IsEmpty()) {
      return ::mediapipe::OkStatus();
    }
    RET_CHECK(!cc->Inputs().Tag(kImageSizeTag).IsEmpty());

#ifdef DEBUG_PRINT
    if(_initLog == 0) {
      FLAGS_logbufsecs = 0;
      google::InitGoogleLogging("irislog");
      google::SetLogDestination(google::GLOG_INFO, "/sdcard/irislog.log");
      google::SetStderrLogging(google::GLOG_INFO);
      FLAGS_max_log_size = 50;
      FLAGS_colorlogtostderr=true;
      FLAGS_timestamp_in_logfile_name = false;
      _initLog = 1;
    }
#endif

    std::pair<int, int> image_size =
        cc->Inputs().Tag(kImageSizeTag).Get<std::pair<int, int>>();
    const auto& landmarks =
        cc->Inputs().Tag(kNormalizedLandmarksTag).Get<NormalizedLandmarkList>();
#ifdef DEBUG_PRINT
    int len = landmarks.landmark_size();
    for(int i = 0; i < len; i++) {
      char str[1024];
      sprintf(str, "LandmarkDebug[%d] x(%f), y(%f), z(%f)\n", i,
                                      landmarks.landmark(i).x(), 
                                      landmarks.landmark(i).y(),
                                      landmarks.landmark(i).z());
      LOG(INFO) << str;
    }
#endif
    auto output_rect = absl::make_unique<NormalizedRect>();
    MP_RETURN_IF_ERROR(
        NormalizedLandmarkListToRect(landmarks, image_size, output_rect.get()));
    cc->Outputs()
        .Tag(kNormRectTag)
        .Add(output_rect.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }

#ifdef DEBUG_PRINT
  ~HandLandmarksToRectCalculator() {
    _initLog = 0;
    google::ShutdownGoogleLogging();
  }
#endif
};
REGISTER_CALCULATOR(HandLandmarksToRectCalculator);

}  // namespace mediapipe
