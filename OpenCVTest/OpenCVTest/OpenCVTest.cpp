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
	//��������Ƶ
	processor.setInput("Statics.avi");
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");
	//����ÿһ֡����ʱ
	processor.setDelay(1000. / processor.getFrameRate());
	//����֡����������������
	processor.setFrameProcessor(&tracker);
	//   processor.setOutput ("./bikeout.avi");
	//    processor.setOutput ("bikeout",".jpg");
	processor.run();
	processor.setOutput("pic", ".jpg");
	return 0;
}
 