//FaceExpression.mm

#import "FaceExpression.h"

#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPLayerRenderer.h"
#import "mediapipe/objc/MPPPlayerInputSource.h"

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/util/cpu_util.h"

typedef NS_ENUM(NSInteger, MediaPipeDemoSourceMode) {
  MediaPipeDemoSourceCamera,
  MediaPipeDemoSourceVideo
};

@interface FaceExpression() <MPPGraphDelegate, MPPInputSourceDelegate>
// The MediaPipe graph currently in use. Initialized in viewDidLoad, started in
// viewWillAppear: and sent video frames on videoQueue.
@property(nonatomic) MPPGraph* mediapipeGraph;
// Handles camera access via AVCaptureSession library.
@property(nonatomic) MPPCameraInputSource* cameraSource;
// Provides data from a video.
@property(nonatomic) MPPPlayerInputSource* videoSource;
// The data source for the demo.
@property(nonatomic) MediaPipeDemoSourceMode sourceMode;
//// Inform the user when camera is unavailable.
//@property(nonatomic) IBOutlet UILabel* noCameraLabel;
//
//// Display the camera preview frames.
//@property(strong, nonatomic) IBOutlet UIView* liveView;
// Render frames in a layer.
@property(nonatomic) MPPLayerRenderer* renderer;
// Process camera frames on this queue.
@property(nonatomic) dispatch_queue_t videoQueue;
// Graph name.
@property(nonatomic) NSString* graphName;
// Graph input stream.
@property(nonatomic) const char* graphInputStream;
// Graph output stream.
@property(nonatomic) const char* graphOutputStream;

@end

#define AVG_CNT         10
#define DETECT_TIMES    2
#define POINT_NUM       4  //输入线性拟和点

static NSString* const kGraphName = @"face_mesh_mobile_gpu";
static const char* kNumFacesInputSidePacket = "num_faces";
static const char* kInputStream = "input_video";
static const char* kOutputStream = "output_video";
static const char* kLandmarksOutputStream = "multi_face_landmarks";
static const char* kVideoQueueLabel = "com.google.mediapipe.example.videoQueue";
// Max number of faces to detect/process.
static const int kNumFaces = 1;

static double brow_width_arr[AVG_CNT];
static double brow_height_arr[AVG_CNT];
static double brow_line_arr[AVG_CNT];
static double brow_mouth_arr[AVG_CNT];
static double brow_height_mouth_arr[AVG_CNT];
static double eye_height_arr[AVG_CNT];
static double eye_width_arr[AVG_CNT];
static double eye_height_mouth_arr[AVG_CNT];
static double mouth_width_arr[AVG_CNT];
static double mouth_height_arr[AVG_CNT];
static double mouth_pull_down_arr[AVG_CNT];
static int arr_cnt = 0;
static int normal_times = 0, suprise_times = 0, sad_times = 0, happy_times = 0, angry_times = 0;
static int total_log_cnt = 0;

@implementation FaceExpression {}

- (void)initialize:(UIView*)preview {
    //for uiview used
    self.renderer = [[MPPLayerRenderer alloc] init];
    self.renderer.layer.frame = preview.layer.bounds;
    [preview.layer addSublayer:self.renderer.layer];
    self.renderer.frameScaleMode = MPPFrameScaleModeFillAndCrop;

    dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(
        DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, /*relative_priority=*/0);
    self.videoQueue = dispatch_queue_create(kVideoQueueLabel, qosAttribute);

    self.graphName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphName"];
    self.graphInputStream =
        [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphInputStream"] UTF8String];
    self.graphOutputStream =
        [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphOutputStream"] UTF8String];

    self.mediapipeGraph = [[self class] loadGraphFromResource:self.graphName];
    [self.mediapipeGraph addFrameOutputStream:self.graphOutputStream
                             outputPacketType:MPPPacketTypePixelBuffer];

    self.mediapipeGraph.delegate = self;
    
    //for camera used
    self.cameraSource = [[MPPCameraInputSource alloc] init];
    [self.cameraSource setDelegate:self queue:self.videoQueue];
    self.cameraSource.sessionPreset = AVCaptureSessionPresetHigh;

    NSString* cameraPosition =
        [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CameraPosition"];
    if (cameraPosition.length > 0 && [cameraPosition isEqualToString:@"back"]) {
      self.cameraSource.cameraPosition = AVCaptureDevicePositionBack;
    } else {
      self.cameraSource.cameraPosition = AVCaptureDevicePositionFront;
      // When using the front camera, mirror the input for a more natural look.
      _cameraSource.videoMirrored = YES;
    }

    // The frame's native format is rotated with respect to the portrait orientation.
    _cameraSource.orientation = AVCaptureVideoOrientationPortrait;

    [self.cameraSource requestCameraAccessWithCompletionHandler:^void(BOOL granted) {
      if (granted) {
        [self startGraph];
//        dispatch_async(dispatch_get_main_queue(), ^{
//          self.noCameraLabel.hidden = YES;
//        });
      }
    }];
}

/**
 四舍五入字符串
 @param round 小数位 eg: 4
 @param numberString 数字 eg 0.125678
 @return 四舍五入之后的 eg: 0.1257
 */
- (double)getRound:(double)val Num:(int)round {
    NSString* valString = [NSString stringWithFormat:@"%f", val];
    if (valString == nil) {
        return 0;
    }
    NSDecimalNumberHandler *roundingBehavior    = [NSDecimalNumberHandler decimalNumberHandlerWithRoundingMode:NSRoundPlain scale:round raiseOnExactness:NO raiseOnOverflow:NO raiseOnUnderflow:NO raiseOnDivideByZero:NO];
    NSDecimalNumber *aDN                        = [[NSDecimalNumber alloc] initWithString:valString];
    NSDecimalNumber *resultDN                   = [aDN decimalNumberByRoundingAccordingToBehavior:roundingBehavior];
    return resultDN.doubleValue;
}

#pragma mark - Cleanup methods

- (void)dealloc {
  self.mediapipeGraph.delegate = nil;
  [self.mediapipeGraph cancel];
  // Ignore errors since we're cleaning up.
  [self.mediapipeGraph closeAllInputStreamsWithError:nil];
  [self.mediapipeGraph waitUntilDoneWithError:nil];
}

#pragma mark - MediaPipe graph methods

+ (MPPGraph*)loadGraphFromResource:(NSString*)resource {
    // Load the graph config resource.
    NSError* configLoadError = nil;
    NSBundle* bundle = [NSBundle bundleForClass:[self class]];
    if (!resource || resource.length == 0) {
        return nil;
    }
    NSURL* graphURL = [bundle URLForResource:resource withExtension:@"binarypb"];
    NSData* data = [NSData dataWithContentsOfURL:graphURL options:0 error:&configLoadError];
    if (!data) {
        NSLog(@"Failed to load MediaPipe graph config: %@", configLoadError);
        return nil;
    }

    // Parse the graph config resource into mediapipe::CalculatorGraphConfig proto object.
    mediapipe::CalculatorGraphConfig config;
    config.ParseFromArray(data.bytes, data.length);

    // Create MediaPipe graph with mediapipe::CalculatorGraphConfig proto object.
    MPPGraph* newGraph = [[MPPGraph alloc] initWithGraphConfig:config];
    [newGraph addFrameOutputStream:kOutputStream outputPacketType:MPPPacketTypePixelBuffer];
    [newGraph addFrameOutputStream:kLandmarksOutputStream outputPacketType:MPPPacketTypeRaw];
    NSLog(@"%s, %d, newGraph(%x)\n", __FUNCTION__, __LINE__, newGraph);
    return newGraph;
}

- (void)startGraph {
    // Start running self.mediapipeGraph.
    NSError* error;
    if (![self.mediapipeGraph startWithError:&error]) {
        NSLog(@"Failed to start graph: %@", error);
    }

    // Start fetching frames from the camera.
    dispatch_async(self.videoQueue, ^{
        [self.cameraSource start];
    });
}

- (void)processGraph {
    [self.mediapipeGraph setSidePacket:(mediapipe::MakePacket<int>(kNumFaces))
                                 named:kNumFacesInputSidePacket];
    [self.mediapipeGraph addFrameOutputStream:kLandmarksOutputStream
                             outputPacketType:MPPPacketTypeRaw];
}

#pragma mark - MPPInputSourceDelegate methods

// Must be invoked on self.videoQueue.
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer
                timestamp:(CMTime)timestamp
               fromSource:(MPPInputSource*)source {
    if (source != self.cameraSource && source != self.videoSource) {
        NSLog(@"Unknown source: %@", source);
        return;
    }

    [self.mediapipeGraph sendPixelBuffer:imageBuffer
                              intoStream:kInputStream
                              packetType:MPPPacketTypePixelBuffer];
}

#pragma mark - MPPGraphDelegate methods

// Receives a raw packet from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
       didOutputPacket:(const ::mediapipe::Packet&)packet
            fromStream:(const std::string&)streamName {
    if (streamName == kLandmarksOutputStream) {
        if (packet.IsEmpty()) {
          NSLog(@"[TS:%lld] No face landmarks", packet.Timestamp().Value());
          return;
        }
        const auto& multi_face_landmarks = packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();
    //    NSLog(@"[TS:%lld] Number of face instances with landmarks: %lu", packet.Timestamp().Value(),
    //          multi_face_landmarks.size());
        for (int face_index = 0; face_index < multi_face_landmarks.size(); ++face_index) {
          const auto& landmarks = multi_face_landmarks[face_index];
    //      NSLog(@"\tNumber of landmarks for face[%d]: %d", face_index, landmarks.landmark_size());
    //      for (int i = 0; i < landmarks.landmark_size(); ++i) {
    //        NSLog(@"\t\tLandmark[%d]: (%f, %f, %f)", i, landmarks.landmark(i).x(),
    //              landmarks.landmark(i).y(), landmarks.landmark(i).z());
    //      }
    //        for (int i = 0; i < landmarkList.getLandmarkCount(); i++) {
    //            faceLandmarksStr  += "\t\tLandmark ["
    //                                + i + "], "
    //                                + landmarks.landmark(i).x() + ", "
    //                                + landmarks.landmark(i).y() + ", "
    //                                + landmarks.landmark(i).z() + ")\n";
    //            Log.i(TAG, faceLandmarksStr);
    //        }
            // 1、计算人脸识别框边长(注: 脸Y坐标 下 > 上, X坐标 右 > 左)
            double face_width = landmarks.landmark(361).x() - landmarks.landmark(132).x();
            double face_height = landmarks.landmark(152).y() - landmarks.landmark(10).y();
            double face_ratio = (landmarks.landmark(362).y() - landmarks.landmark(133).y())/(landmarks.landmark(362).x() - landmarks.landmark(133).x());

            //2、眉毛宽度(注: 脸Y坐标 下 > 上, X坐标 右 > 左 眉毛变短程度: 皱变短(恐惧、愤怒、悲伤))
            double brow_width = landmarks.landmark(296).x()-landmarks.landmark(53).x() +
                         landmarks.landmark(334).x()-landmarks.landmark(52).x() +
                         landmarks.landmark(293).x()-landmarks.landmark(65).x() +
                         landmarks.landmark(300).x()-landmarks.landmark(55).x() +
                         landmarks.landmark(285).x()-landmarks.landmark(70).x() +
                         landmarks.landmark(295).x()-landmarks.landmark(63).x() +
                         landmarks.landmark(282).x()-landmarks.landmark(105).x() +
                         landmarks.landmark(283).x()-landmarks.landmark(66).x();

            //2.1、眉毛高度之和
            double brow_left_height =      landmarks.landmark(53).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(52).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(65).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(55).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(70).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(63).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(105).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(66).y() - landmarks.landmark(10).y();
            double brow_right_height =     landmarks.landmark(283).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(282).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(295).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(285).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(300).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(293).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(334).y() - landmarks.landmark(10).y() +
                                    landmarks.landmark(296).y() - landmarks.landmark(10).y();
            double brow_hight = brow_left_height + brow_right_height;
            //2.2、眉毛高度与识别框高度之比: 眉毛抬高(惊奇、恐惧、悲伤), 眉毛压低(厌恶, 愤怒)
            double brow_hight_rate = brow_hight/16;
            double brow_width_rate = brow_width/8;
            //左眉拟合曲线(53-52-65-55-70-63-105-66)
            double brow_line_points_x[POINT_NUM];
            brow_line_points_x[0] = landmarks.landmark(55).x();
            brow_line_points_x[1] = landmarks.landmark(70).x();
            brow_line_points_x[2] = landmarks.landmark(105).x();
            brow_line_points_x[3] = landmarks.landmark(107).x();
            double brow_line_points_y[POINT_NUM];
            brow_line_points_y[0] = landmarks.landmark(55).y();
            brow_line_points_y[1] = landmarks.landmark(70).y();
            brow_line_points_y[2] = landmarks.landmark(105).y();
            brow_line_points_y[3] = landmarks.landmark(107).y();

            //2.3、眉毛变化程度: 变弯(高兴、惊奇) - 上扬  - 下拉
            double brow_line_left = (-10) * (::mediapipe::getCurveFit_android(brow_line_points_x, brow_line_points_y, POINT_NUM)); //调函数拟合直线
            double brow_line_rate = brow_line_left;
            //3、眼睛高度 (注: 眼睛Y坐标 下 > 上, X坐标 右 > 左)
            double eye_left_height = landmarks.landmark(23).y() - landmarks.landmark(27).y();   //中心 以后尝试修改为 Y(145) - Y(159) -> Y(23) - Y(27)
            double eye_left_width = landmarks.landmark(133).x() - landmarks.landmark(33).x();
            double eye_right_height = landmarks.landmark(253).y() - landmarks.landmark(257).y();  // 中心 以后尝试修改为 Y(374) - Y(386) -> Y(253) - Y(257)
            double eye_right_width = landmarks.landmark(263).x() - landmarks.landmark(362).x();

            //3.1、眼睛睁开程度: 上下眼睑拉大距离(惊奇、恐惧)
            double eye_height = (eye_left_height + eye_right_height)/2;
            double eye_width = (eye_left_width + eye_right_width)/2;

            //4、嘴巴宽高(两嘴角间距离- 用于计算嘴巴的宽度 注: 嘴巴Y坐标 上 > 下, X坐标 右 > 左 嘴巴睁开程度- 用于计算嘴巴的高度: 上下嘴唇拉大距离(惊奇、恐惧、愤怒、高兴))
            double mouth_width = landmarks.landmark(308).x() - landmarks.landmark(78).x();
            double mouth_height = landmarks.landmark(17).y() - landmarks.landmark(0).y();  // 中心

            //4.1、嘴角下拉(厌恶、愤怒、悲伤),    > 1 上扬， < 1 下拉
            double mouth_pull_down = (landmarks.landmark(14).y() - landmarks.landmark(324).y())/(landmarks.landmark(14).y() + landmarks.landmark(324).x());
            //对嘴角进行一阶拟合，曲线斜率
            double lips_line_points_x[POINT_NUM];
            lips_line_points_x[0] = landmarks.landmark(318).x();
            lips_line_points_x[1] = landmarks.landmark(324).x();
            lips_line_points_x[2] = landmarks.landmark(308).x();
            lips_line_points_x[3] = landmarks.landmark(291).x();
            double lips_line_points_y[POINT_NUM];
            lips_line_points_y[0] = landmarks.landmark(318).y();
            lips_line_points_y[1] = landmarks.landmark(324).y();
            lips_line_points_y[2] = landmarks.landmark(308).y();
            lips_line_points_y[3] = landmarks.landmark(291).y();
            double mouth_pull_down_rate = (-10) * (::mediapipe::getCurveFit_android(lips_line_points_x, lips_line_points_y, POINT_NUM)); //调函数拟合直线

            //5、两侧眼角到同侧嘴角距离
            double distance_eye_left_mouth = landmarks.landmark(78).y() - landmarks.landmark(133).y();
            double distance_eye_right_mouth = landmarks.landmark(308).y() - landmarks.landmark(362).y();
            double distance_eye_mouth = distance_eye_left_mouth + distance_eye_right_mouth;

            //6、归一化
            double MM = 0, NN = 0, PP = 0, QQ = 0;
            double dis_eye_mouth_rate = (2 * mouth_width)/distance_eye_mouth;             // 嘴角 / 眼角嘴角距离, 高兴(0.85),愤怒/生气(0.7),惊讶(0.6),大哭(0.75)
            double distance_brow = landmarks.landmark(296).x() - landmarks.landmark(66).x();
            double dis_brow_mouth_rate = mouth_width/distance_brow;                       // 嘴角 / 两眉间距
            double dis_eye_height_mouth_rate = mouth_width/eye_height;                    // 嘴角 / 上下眼睑距离
            double dis_brow_height_mouth_rate = (2 * mouth_width)/(landmarks.landmark(145).y() - landmarks.landmark(70).y());

            //7、 求连续多次的平均值
            if(arr_cnt < AVG_CNT) {
                brow_mouth_arr[arr_cnt] = dis_brow_mouth_rate;
                brow_height_mouth_arr[arr_cnt] = dis_brow_height_mouth_rate;
                brow_width_arr[arr_cnt] = brow_width_rate;
                brow_height_arr[arr_cnt] = brow_hight_rate;
                brow_line_arr[arr_cnt] = brow_line_rate;
                eye_height_arr[arr_cnt] = eye_height;
                eye_width_arr[arr_cnt] = eye_width;
                eye_height_mouth_arr[arr_cnt] = dis_eye_height_mouth_rate;
                mouth_width_arr[arr_cnt] = mouth_width;
                mouth_height_arr[arr_cnt] = mouth_height;
                mouth_pull_down_arr[arr_cnt] = mouth_pull_down_rate;
            }
            double brow_mouth_avg = 0, brow_height_mouth_avg = 0;
            double brow_width_avg = 0, brow_height_avg = 0, brow_line_avg = 0;
            double eye_height_avg = 0, eye_width_avg = 0, eye_height_mouth_avg = 0;
            double mouth_width_avg = 0, mouth_height_avg = 0, mouth_pull_down_avg = 0;
            arr_cnt++;
            if(arr_cnt >= AVG_CNT) {
                brow_mouth_avg = ::mediapipe::getAverage_android(brow_mouth_arr, AVG_CNT);
                brow_height_mouth_avg = ::mediapipe::getAverage_android(brow_height_mouth_arr, AVG_CNT);
                brow_width_avg = ::mediapipe::getAverage_android(brow_width_arr, AVG_CNT);
                brow_height_avg = ::mediapipe::getAverage_android(brow_height_arr, AVG_CNT);
                brow_line_avg = ::mediapipe::getAverage_android(brow_line_arr, AVG_CNT);
                eye_height_avg = ::mediapipe::getAverage_android(eye_height_arr, AVG_CNT);
                eye_width_avg = ::mediapipe::getAverage_android(eye_width_arr, AVG_CNT);
                eye_height_mouth_avg = ::mediapipe::getAverage_android(eye_height_mouth_arr, AVG_CNT);
                mouth_width_avg = ::mediapipe::getAverage_android(mouth_width_arr, AVG_CNT);
                mouth_height_avg = ::mediapipe::getAverage_android(mouth_height_arr, AVG_CNT);
                mouth_pull_down_avg = ::mediapipe::getAverage_android(mouth_pull_down_arr, AVG_CNT);
                arr_cnt = 0;
            }
            if(arr_cnt == 0) {
                memset(brow_mouth_arr, 0, sizeof(brow_mouth_arr));
                memset(brow_height_mouth_arr, 0, sizeof(brow_height_mouth_arr));
                memset(brow_width_arr, 0, sizeof(brow_width_arr));
                memset(brow_height_arr, 0, sizeof(brow_height_arr));
                memset(brow_line_arr, 0, sizeof(brow_line_arr));
                memset(eye_height_arr, 0, sizeof(eye_height_arr));
                memset(eye_width_arr, 0, sizeof(eye_width_arr));
                memset(eye_height_mouth_arr, 0, sizeof(eye_height_mouth_arr));
                memset(mouth_width_arr, 0, sizeof(mouth_width_arr));
                memset(mouth_height_arr, 0, sizeof(mouth_height_arr));
                memset(mouth_pull_down_arr, 0, sizeof(mouth_pull_down_arr));
            }

            total_log_cnt++;
            if(total_log_cnt >= AVG_CNT) {
                //8、表情算法
                ::mediapipe::setFaceExpressionFace(face_width, face_height, face_ratio);
                ::mediapipe::setFaceExpressionBrow(brow_width_avg, brow_height_avg, brow_line_avg);
                ::mediapipe::setFaceExpressionEye(eye_width_avg, eye_height_avg, dis_eye_mouth_rate);
                ::mediapipe::setFaceExpressionMouth(mouth_width_avg, mouth_height_avg, mouth_pull_down_avg, brow_height_mouth_avg);

                int expression = ::mediapipe::getFaceExpressionType();
                //9、抛出表情结果, 回调结果给上层
//                [_delegate faceExpression:self Type:expression];
                total_log_cnt = 0;
            }
        }
      }

}

// Receives CVPixelBufferRef from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
  didOutputPixelBuffer:(CVPixelBufferRef)pixelBuffer
            fromStream:(const std::string&)streamName {
    
    if (streamName == self.graphOutputStream) {
        // Display the captured image on the screen.
        CVPixelBufferRetain(pixelBuffer);
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.renderer renderPixelBuffer:pixelBuffer];
            CVPixelBufferRelease(pixelBuffer);
        });
    }
}


@end
