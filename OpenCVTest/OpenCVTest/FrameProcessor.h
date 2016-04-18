#pragma once
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
using namespace std;
using namespace cv;

class FrameProcessor;
//֡�������
class FrameProcessor{
public:
	virtual void process(Mat &input, Mat &ouput) = 0;
};

//���������࣬�̳���֡�������
class FeatureTracker : public FrameProcessor{
	Mat gray;  //��ǰ�Ҷ�ͼ
	Mat gray_prev;  //֮ǰ�ĻҶ�ͼ
	vector<Point2f> points[2];//ǰ����֡��������
	vector<Point2f> initial;//��ʼ������
	vector<Point2f> features;//��⵽������
	int max_count; //Ҫ���������������Ŀ
	double qlevel; //��������ָ��
	double minDist;//������֮����С���̾���
	vector<uchar> status; //��������״̬
	vector<float> err; //����ʱ�Ĵ���
public:
	FeatureTracker();
	void process(Mat &frame, Mat &output);

	void detectFeaturePoint();
	bool addNewPoint();

	//����������ǰ����֡�ƶ��ˣ�����Ϊ�õ���Ŀ��㣬�ҿɱ�����
	bool acceptTrackedPoint(int i);

	//��������
	void  handleTrackedPoint(Mat &frame, Mat &output);
};


class VideoProcessor{
private:
	VideoCapture caputure;
	//д��Ƶ������
	VideoWriter writer;
	//����ļ���
	string Outputfile;

	int currentIndex;
	int digits;
	string extension;
	FrameProcessor *frameprocessor;
	//ͼ������ָ��
	void(*process)(Mat &, Mat &);
	bool callIt;
	string WindowNameInput;
	string WindowNameOutput;
	//��ʱ
	int delay;
	long fnumber;
	//��frameToStopֹͣ
	long frameToStop;
	//��ͣ��־
	bool stop;
	//ͼ��������Ϊ������Ƶ��
	vector<string> images;
	//������
public:
	VideoProcessor();
	//����ͼ������
	void setFrameProcessor(void(*process)(Mat &, Mat &));
	//����Ƶ
	bool setInput(string filename);
	//����������Ƶ���Ŵ���
	void displayInput(string wn);
	//���������Ƶ���Ŵ���
	void displayOutput(string wn);
	//���ٴ���
	void dontDisplay();

	//����
	void run();
	//��ͣ����λ
	void stopIt();
	//��ѯ��ͣ��־λ
	bool isStopped();
	//������Ƶ�򿪱�־
	bool isOpened();
	//������ʱ
	void setDelay(int d);
	//��ȡ��һ֡
	bool readNextFrame(Mat &frame);

	void CallProcess();
	void  dontCallProcess();
	//����ֹͣ֡
	void stopAtFrameNo(long frame);
	// ��õ�ǰ֡��λ��
	long getFrameNumber();

	//���֡��С
	Size getFrameSize();

	//��ȡ֡��
	double getFrameRate();
	vector<string>::const_iterator itImg;
	bool setInput(const vector<string> &imgs);

	void  setFrameProcessor(FrameProcessor *frameprocessor);

	//��ñ�������
	int getCodec(char codec[4]);


	bool setOutput(const string &filename, int codec = 0, double framerate = 0.0, bool isColor = true);

	//�����Ƶ֡��ͼƬfileme+currentIndex.ext,��filename001.jpg
	bool setOutput(const string &filename,//·��
		const string &ext,//��չ��
		int numberOfDigits = 3,//����λ��
		int startIndex = 0);

	//д��һ֡
	void writeNextFrame(Mat &frame);

};

//֡��������canny��Ե���
extern void canny(cv::Mat& img, cv::Mat& out);
