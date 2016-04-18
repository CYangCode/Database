//#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/video/background_segm.hpp>
#include "MultiWindows_Display.h"
#include <iostream>
#include <fstream>
#include "FrameProcessor.h"

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	VideoProcessor processor;
	FeatureTracker tracker;
	//打开输入视频
	processor.setInput("Statics.avi");
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");
	//设置每一帧的延时
	processor.setDelay(1000. / processor.getFrameRate());
	//设置帧处理函数，可以任意
	processor.setFrameProcessor(&tracker);
	//   processor.setOutput ("./bikeout.avi");
	//    processor.setOutput ("bikeout",".jpg");
	processor.run();
	processor.setOutput("pic", ".jpg");
	return 0;
}
 