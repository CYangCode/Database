#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\video\video.hpp>
#include <iosfwd>
#include <fstream>
using namespace std;
using namespace cv;
//将mvname的视频每一帧截成图片保存在imags文件夹下
void DivideMvToPics(const char * mvname)
{
	CvCapture* capture = cvCaptureFromAVI(mvname);
	int i = 0;
	IplImage* img = 0;
	char image_name[25];
	cvNamedWindow("vivi");
	//读取和显示
	while (1)
	{
		img = cvQueryFrame(capture); //获取一帧图片
		if (img == NULL)
			break;

		cvShowImage("vivi", img); //将其显示
		char key = cvWaitKey(10);
		sprintf(image_name, "%s%03d%s", "imags\\image", ++i, ".jpg");//保存的图片名
		cvSaveImage(image_name, img);   //保存一帧图片
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("vivi");

}

char filename[100];
char windowname[100];
IplImage* pScr;
unsigned char *Readfigsmethod1(int num)// 读入num个图片
{
	for (int i = 1; i <= num; i++)
	{
		sprintf(filename, "imags/image%03d.jpg", i);// 将图片以数字命名：例如1.jpg 2.jpg等，放入D:/test/文件夹下
		sprintf(windowname, "window%d.jpg", i);
		pScr = cvLoadImage(filename, 1);//导入图片
		cvNamedWindow(windowname, CV_WINDOW_AUTOSIZE);
		cvShowImage(windowname, pScr);//显示图片
		//cvWaitKey(0);

	}
	cvWaitKey(0);
	cvReleaseImage(&pScr);//释放图片
	cvDestroyAllWindows();//销毁窗口

	return 0;
}
//将imags文件夹下所有的图片按顺序合成avi_name为名的avi视频，并将其保存在avi文件夹下
int MergePicsToAvi(const char * avi_name)
{
	system("dir /b imags2 > filelist.txt");

	ifstream imglist("filelist.txt");
	cout << "the file is opened" << endl;
	string buf;
	/*while (imglist)
	{
		if (getline(imglist, buf))
		{
			numline++;
		}
	}*/

	string open_dir = "imags2/";
	string line;
	getline(imglist, line);
	IplImage *frame = cvLoadImage(open_dir.append(line).c_str());
	imglist.seekg(ios::beg);
	if (!frame) {
		cout << "frame did not work" << endl;
		return 0;
	}
	CvVideoWriter *videoWriter = cvCreateVideoWriter(avi_name, -1, 30, cvSize(frame->width, frame->height));
		

	int ret = 0;
	ret = cvWriteFrame(videoWriter, frame);
	if (ret != 1)
	{
		cout << "failed to write the first frame." << endl;
		return 0;
	}
	cvReleaseImage(&frame);
	frame = NULL;

	while (imglist)
	{
		if (getline(imglist, buf))
		{
			cout << "processing...img/" << buf << endl;
			char filename[256];
			sprintf(filename, "imags2/%s", buf.c_str());
			frame = cvLoadImage(filename);
			if (!frame)
			{
				cout << "can not open the image: " << filename << endl;
				return 0;
			}
			ret = cvWriteFrame(videoWriter, frame);
			if (ret != 1)
			{
				cout << "can not write to the video." << endl;
				return 0;
			}
			cvReleaseImage(&frame);
			frame = NULL;
		}
	}
	imglist.close();
	cout << "processing...completed" << endl;

	/*     cvReleaseImage(&frame);*/
	cvReleaseVideoWriter(&videoWriter);
	return 0;
}
//对imags文件夹下所有的图片调整分辨率，并将其保存在imags2文件夹下
void Resize()
{
	system("dir /b imags > filelist2.txt");
	ifstream imglist("filelist2.txt");
	while (imglist)
	{
		string buff;
		if (getline(imglist, buff))
		{
			char img_name[100];
			sprintf(img_name, "imags/%s", buff.c_str());
			IplImage *org_img = 0, *rsz_image = 0, *rsz_image2;
			org_img = cvLoadImage(img_name, 1);
			if (!org_img) printf("can't load an image/n");
			rsz_image = cvCreateImage(cvSize(32, 64), IPL_DEPTH_8U, 3);
			cvResize(org_img, rsz_image, CV_INTER_LINEAR);
			rsz_image2 = cvCreateImage(cvSize(320, 640), IPL_DEPTH_8U, 3);
			cvResize(rsz_image, rsz_image2, CV_INTER_LINEAR);

			sprintf(img_name, "imags2/%s", buff.c_str());
			cvSaveImage(img_name, rsz_image2);
			cvReleaseImage(&org_img);
			cvReleaseImage(&rsz_image);
			cvReleaseImage(&rsz_image2);
		}
	}
	imglist.close();
}

int main()
{
	//DivideMvToPics("01.mp4");
	//Resize();
	MergePicsToAvi("avi/Dynamic.avi");
	system("pause");
	return 0;
}