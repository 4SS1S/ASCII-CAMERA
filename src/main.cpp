#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<sys/ioctl.h>
#include<unistd.h>
#include<iostream>
#include<chrono>
#include<thread>

using namespace std;
using namespace cv;

// Get the pixel and returns it in pixel
string pixelToASCII(int pixel_intensity) {
  const string ASCII_CHARS = " ._-=+*!&#%$@";
  
  return string(
    1,
    ASCII_CHARS[pixel_intensity * ASCII_CHARS.length() / 256]
  );
}

int main() {
  // Video variables
  Mat frame, gray_frame, resized_frame;

  VideoCapture capture(0, CAP_ANY);

  double fps = capture.get(CAP_PROP_FPS);
  int frame_duration_ms = 1000 / fps;

  // Get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int width = w.ws_col;
  int height = w.ws_row;

  if(!capture.isOpened()) {
    cerr<<"Error";
    return -1;
  }

  // Frames size
  int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);
  int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);

  height = (w.ws_col * frame_height / frame_width) * 0.4194;
  // namedWindow("w", 1);

  while(true) {
    capture >> frame;

    // Stop if it's empty
    if (frame.empty()) {
      break;
    }

    // Get the image as B&W
    cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

    resize(
      gray_frame,
      resized_frame,
      Size(width, height),
      0, 
      0,
      INTER_LINEAR
    );

    waitKey(20);

    string ascii_frame;

    for (int i = 0; i < height; i ++) {
      for (int j = 0; j < width; j++) {
        ascii_frame += pixelToASCII(resized_frame.at<uchar>(i, j));
      }
      ascii_frame += "\n";
    }

    // imshow("w", gray_frame);
    cout<<ascii_frame<<endl;

    // Clear screem
    cout<<"\x1B[2J\x1B[H";

    
    this_thread::sleep_for(chrono::milliseconds(frame_duration_ms));
  }

  
  return 0;
}