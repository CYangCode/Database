//--------------------------------------------------------------------------------------//
//	name:		display_multi_windows
//	
//	input:		the array of picture:	_imgs[4]
//				width of the video:		_pic_width
//				height of the video:	_pic_height
//	output:		a big windows that display 4 small windows
//	attention:  this function can only display color video or black_and_white video (use: CV_8UC3 or CV_8UC1) 

//--------------------------------------------------------------------------------------//
//#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Mat imshowMany(const vector<Mat>& _imgs, int _pic_width, int _pic_height)  
{  
	int nImg = (int)_imgs.size();  
	Size frameSize( (_pic_width*2 + 60), (_pic_height*2 + 60));
	Mat dispImg;  
	//dispImg.create(Size(100 + size*w, -60 + size*h),CV_8UC1);  
	dispImg.create(frameSize,CV_8UC1);			//CV_8UC1:显示单通道图像（黑白图像）		CV_8UC3:显示3通道图像（彩色图像）

	int size;  
	int x, y;  

	// w - Maximum number of images in a row   
	// h - Maximum number of images in a column   
	int w, h;  
	// scale - How much we have to resize the image  
	//float scale;  
	//int max;  

	if (nImg <= 0)   
	{  
		printf("Number of arguments too small....\n");  
		return Mat::zeros(100,100,CV_8UC1);  
	}  
	else if (nImg > 12)  
	{  
		printf("Number of arguments too large....\n");  
		return Mat::zeros(100,100,CV_8UC1);  
	}  

	else if (nImg == 1)  
	{  
		w = h = 1;  
		size = 300;  
	}  
	else if (nImg == 2)  
	{  
		w = 2; h = 1;  
		size = 300;  
	}  
	else if (nImg == 3 || nImg == 4)  
	{  
		w = 2; h = 2;  
		size = 300;  
	}  
	else if (nImg == 5 || nImg == 6)  
	{  
		w = 3; h = 2;  
		size = 200;  
	}  
	else if (nImg == 7 || nImg == 8)  
	{  
		w = 4; h = 2;  
		size = 200;  
	}  
	else  
	{  
		w = 4; h = 3;  
		size = 150;  
	}  


	for (int i= 0, m=20, n=30; i<nImg; i++, m+=(20+size+20))  
	{  
		x = _imgs[i].cols;  
		y = _imgs[i].rows;  

		if (i%w==0 && m!=20)  
		{  
			m = 20;  
			n += 30 + _pic_height;  
		}  

		Mat imgROI = dispImg(Rect(m, n, x, y));  
		resize(_imgs[i], imgROI, Size(x, y));  

		//每幅图片显示标题
		string words = "";
		switch(i)
		{
		case 0: words = "pic_1"; 
			break;
		case 1: words = "pic_2";
			break;
		case 2: words = "pic_3";
			break;
		case 3: words = "pic_4";
			break;
		default:words = "nothing";
			break;
		}
		putText( dispImg, words, Point( m + _pic_width/3 ,n - 10),FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 0, 0) );	//文本显示的位置

	}  

	return dispImg;
	//namedWindow(_winName);  
	//imshow(_winName, dispImg);  
} 