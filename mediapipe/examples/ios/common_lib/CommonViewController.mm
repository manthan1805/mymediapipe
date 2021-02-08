// Copyright 2019 The MediaPipe Authors.
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

#import "CommonViewController.h"

#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPPlayerInputSource.h"

typedef NS_ENUM(NSInteger, MediaPipeDemoSourceMode) {
  MediaPipeDemoSourceCamera,
  MediaPipeDemoSourceVideo
};

@interface CommonViewController() <MPPInputSourceDelegate>
// Handles camera access via AVCaptureSession library.
@property(nonatomic) MPPCameraInputSource* cameraSource;

// Process camera frames on this queue.
@property(nonatomic) dispatch_queue_t videoQueue;

@end

static const char* kVideoQueueLabel = "com.google.mediapipe.example.videoQueue";
static const char* kInputStream = "input_video";
static const char* kOutputStream = "output_video";
static NSString* const kGraphName = @"face_mesh_mobile_gpu";

@implementation CommonViewController

// This provides a hook to replace the basic ViewController with a subclass when it's created from a
// storyboard, without having to change the storyboard itself.
+ (instancetype)allocWithZone:(struct _NSZone*)zone {
  NSString* subclassName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"MainViewController"];
  if (subclassName.length > 0) {
    Class customClass = NSClassFromString(subclassName);
    Class baseClass = [CommonViewController class];
    NSAssert([customClass isSubclassOfClass:baseClass], @"%@ must be a subclass of %@", customClass,
             baseClass);
    if (self == baseClass) return [customClass allocWithZone:zone];
  }
  return [super allocWithZone:zone];
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
  return newGraph;
}

#pragma mark - UIViewController methods

- (void)viewDidLoad {
  [super viewDidLoad];

//  self.renderer = [[MPPLayerRenderer alloc] init];
//  self.renderer.layer.frame = self.liveView.layer.bounds;
//  [self.liveView.layer addSublayer:self.renderer.layer];
  self.renderer.frameScaleMode = MPPFrameScaleModeFillAndCrop;

  dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(
      DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, /*relative_priority=*/0);
  self.videoQueue = dispatch_queue_create(kVideoQueueLabel, qosAttribute);

//  self.graphName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphName"];
//  self.graphInputStream =
//      [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphInputStream"] UTF8String];
//  self.graphOutputStream =
//      [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"GraphOutputStream"] UTF8String];

  self.mediapipeGraph = [[self class] loadGraphFromResource:kGraphName];
  [self.mediapipeGraph addFrameOutputStream:kOutputStream
                           outputPacketType:MPPPacketTypePixelBuffer];

  self.mediapipeGraph.delegate = self;
}

// In this application, there is only one ViewController which has no navigation to other view
// controllers, and there is only one View with live display showing the result of running the
// MediaPipe graph on the live video feed. If more view controllers are needed later, the graph
// setup/teardown and camera start/stop logic should be updated appropriately in response to the
// appearance/disappearance of this ViewController, as viewWillAppear: can be invoked multiple times
// depending on the application navigation flow in that case.
- (void)viewWillAppear:(BOOL)animated {
  [super viewWillAppear:animated];

//  switch (self.sourceMode) {
//    case MediaPipeDemoSourceVideo: {
//      NSString* videoName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"VideoName"];
//      AVAsset* video = [AVAsset assetWithURL:[[NSBundle mainBundle] URLForResource:videoName
//                                                                     withExtension:@"mov"]];
//      self.videoSource = [[MPPPlayerInputSource alloc] initWithAVAsset:video];
//      [self.videoSource setDelegate:self queue:self.videoQueue];
//      dispatch_async(self.videoQueue, ^{
//        [self.videoSource start];
//      });
//      break;
//    }
//    case MediaPipeDemoSourceCamera: {
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
          [self startGraphAndCamera];
//          dispatch_async(dispatch_get_main_queue(), ^{
//            self.noCameraLabel.hidden = YES;
//          });
        }
      }];

//      break;
//    }
//  }
}

- (void)startGraphAndCamera {
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

#pragma mark - MPPInputSourceDelegate methods

// Must be invoked on self.videoQueue.
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer
                timestamp:(CMTime)timestamp
               fromSource:(MPPInputSource*)source {
  if (source != self.cameraSource /*&& source != self.videoSource*/) {
    NSLog(@"Unknown source: %@", source);
    return;
  }

  [self.mediapipeGraph sendPixelBuffer:imageBuffer
                            intoStream:kInputStream
                            packetType:MPPPacketTypePixelBuffer];
}

#pragma mark - MPPGraphDelegate methods

// Receives CVPixelBufferRef from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
    didOutputPixelBuffer:(CVPixelBufferRef)pixelBuffer
              fromStream:(const std::string&)streamName {
  if (streamName == kOutputStream) {
    // Display the captured image on the screen.
    CVPixelBufferRetain(pixelBuffer);
    dispatch_async(dispatch_get_main_queue(), ^{
      [self.renderer renderPixelBuffer:pixelBuffer];
      CVPixelBufferRelease(pixelBuffer);
    });
  }
}

@end