#ifndef _MultiWindows_Display_H
#define _MultiWindows_Display_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
using namespace std;
using namespace cv;
//#include "MultiWindows_Display.h"
extern Mat imshowMany(const vector<Mat>& _imgs, int _pic_width, int _pic_height);

#endif