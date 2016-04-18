#pragma once
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
using namespace std;
using namespace cv;

class FrameProcessor;
//帧处理基类
class FrameProcessor{
public:
	virtual void process(Mat &input, Mat &ouput) = 0;
};

//特征跟踪类，继承自帧处理基类
class FeatureTracker : public FrameProcessor{
	Mat gray;  //当前灰度图
	Mat gray_prev;  //之前的灰度图
	vector<Point2f> points[2];//前后两帧的特征点
	vector<Point2f> initial;//初始特征点
	vector<Point2f> features;//检测到的特征
	int max_count; //要跟踪特征的最大数目
	double qlevel; //特征检测的指标
	double minDist;//特征点之间最小容忍距离
	vector<uchar> status; //特征跟踪状态
	vector<float> err; //跟踪时的错误
public:
	FeatureTracker();
	void process(Mat &frame, Mat &output);

	void detectFeaturePoint();
	bool addNewPoint();

	//若特征点在前后两帧移动了，则认为该点是目标点，且可被跟踪
	bool acceptTrackedPoint(int i);

	//画特征点
	void  handleTrackedPoint(Mat &frame, Mat &output);
};


class VideoProcessor{
private:
	VideoCapture caputure;
	//写视频流对象
	VideoWriter writer;
	//输出文件名
	string Outputfile;

	int currentIndex;
	int digits;
	string extension;
	FrameProcessor *frameprocessor;
	//图像处理函数指针
	void(*process)(Mat &, Mat &);
	bool callIt;
	string WindowNameInput;
	string WindowNameOutput;
	//延时
	int delay;
	long fnumber;
	//第frameToStop停止
	long frameToStop;
	//暂停标志
	bool stop;
	//图像序列作为输入视频流
	vector<string> images;
	//迭代器
public:
	VideoProcessor();
	//设置图像处理函数
	void setFrameProcessor(void(*process)(Mat &, Mat &));
	//打开视频
	bool setInput(string filename);
	//设置输入视频播放窗口
	void displayInput(string wn);
	//设置输出视频播放窗口
	void displayOutput(string wn);
	//销毁窗口
	void dontDisplay();

	//启动
	void run();
	//暂停键置位
	void stopIt();
	//查询暂停标志位
	bool isStopped();
	//返回视频打开标志
	bool isOpened();
	//设置延时
	void setDelay(int d);
	//读取下一帧
	bool readNextFrame(Mat &frame);

	void CallProcess();
	void  dontCallProcess();
	//设置停止帧
	void stopAtFrameNo(long frame);
	// 获得当前帧的位置
	long getFrameNumber();

	//获得帧大小
	Size getFrameSize();

	//获取帧率
	double getFrameRate();
	vector<string>::const_iterator itImg;
	bool setInput(const vector<string> &imgs);

	void  setFrameProcessor(FrameProcessor *frameprocessor);

	//获得编码类型
	int getCodec(char codec[4]);


	bool setOutput(const string &filename, int codec = 0, double framerate = 0.0, bool isColor = true);

	//输出视频帧到图片fileme+currentIndex.ext,如filename001.jpg
	bool setOutput(const string &filename,//路径
		const string &ext,//扩展名
		int numberOfDigits = 3,//数字位数
		int startIndex = 0);

	//写下一帧
	void writeNextFrame(Mat &frame);

};

//帧处理函数：canny边缘检测
extern void canny(cv::Mat& img, cv::Mat& out);
