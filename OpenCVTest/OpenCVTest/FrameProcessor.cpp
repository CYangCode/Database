#include "FrameProcessor.h"

void FeatureTracker::handleTrackedPoint(Mat &frame, Mat &output)
{
	for (int i = 0; i < points[1].size(); i++){
		//��ǰ�����㵽��ʼλ����ֱ�߱�ʾ
		Scalar s = CV_RGB(255, 0, 0);
		line(output, initial[i], points[1][i], s);
		//��ǰλ����Ȧ���
		circle(output, points[1][i], 3, s, (-1));
	}
}

bool FeatureTracker::acceptTrackedPoint(int i)
{
	return status[i] &&
		(abs(points[0][i].x - points[1][i].x) +
		abs(points[0][i].y - points[1][i].y) > 2);
}

bool FeatureTracker::addNewPoint()
{
	//����������Ŀ����10����������������
	return points[0].size() <= 10;
}

void FeatureTracker::detectFeaturePoint()
{

	goodFeaturesToTrack(gray,//ͼƬ
		features,//���������
		max_count,//�����������Ŀ
		qlevel,//����ָ��
		30);//��С���̾���
}

void FeatureTracker::process(Mat &frame, Mat &output)
{
	//�õ��Ҷ�ͼ
	cvtColor(frame, gray, CV_BGR2GRAY);
	frame.copyTo(output);
	//������̫���ˣ����¼��������
	if (addNewPoint()){
		detectFeaturePoint();
		//�����⵽��������
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
	}
	//��һ֡
	if (gray_prev.empty()){
		gray.copyTo(gray_prev);
	}
	//����ǰ����֡�Ҷ�ͼ����ǰһ֡�������ڵ�ǰ֡��λ��
	//Ĭ�ϴ�����15*15
	calcOpticalFlowPyrLK(
		gray_prev,//ǰһ֡�Ҷ�ͼ
		gray,//��ǰ֡�Ҷ�ͼ
		points[0],//ǰһ֡������λ��
		points[1],//��ǰ֡������λ��
		status,//�����㱻�ɹ����ٵı�־
		err);//ǰһ֡�������С����͵�ǰ������С�����Ĳ���ݲ�Ĵ�С��ɾ����Щ�˶��仯���ҵĵ�
	int k = 0;
	//ȥ����Щδ�ƶ���������
	for (int i = 0; i < points[1].size(); i++){
		if (acceptTrackedPoint(i)){
			initial[k] = initial[i];
			points[1][k++] = points[1][i];
			/*cout << "vector is ok" << endl;
			cout << "i = " << i << endl;
			cout << "k = " << k << endl;*/
		}
	}
	if (k == 0)	return;
	cout <<"vector is ok" << endl;
	points[1].resize(k);
	initial.resize(k);
	//��Ǳ����ٵ�������
	cout << "this vector is hahaha" << endl;
	handleTrackedPoint(frame, output);
	//Ϊ��һ֡���ٳ�ʼ�������㼯�ͻҶ�ͼ��
	std::swap(points[1], points[0]);
	cout << "hahaha" << endl;
	cv::swap(gray_prev, gray);
	cout << "lalalala" << endl;
}

FeatureTracker::FeatureTracker() :max_count(500), qlevel(0.01), minDist(10.)
{

}

void VideoProcessor::writeNextFrame(Mat &frame)
{
	//�����չ����Ϊ�գ�д��ͼƬ�ļ���
	if (extension.length()){
		stringstream ss;
		ss << Outputfile << setfill('0') << setw(digits) << currentIndex++ << extension;
		imwrite(ss.str(), frame);
	}
	//��֮��д����Ƶ�ļ���
	else{
		writer.write(frame);
	}
}

bool VideoProcessor::setOutput(const string &filename,/*·�� */ const string &ext,/*��չ�� */ int numberOfDigits /*= 3*/,/*����λ�� */ int startIndex /*= 0*/)
{
	//��ʼ����
	if (numberOfDigits < 0)
		return false;
	Outputfile = filename;
	extension = ext;
	digits = numberOfDigits;
	currentIndex = startIndex;
	return true;
}

bool VideoProcessor::setOutput(const string &filename, int codec /*= 0*/, double framerate /*= 0.0*/, bool isColor /*= true*/)
{
	//�����ļ���
	Outputfile = filename;
	//�����չ��
	extension.clear();
	//����֡��
	if (framerate == 0.0){
		framerate = getFrameRate();
	}
	//��ȡ����ԭ��Ƶ�ı��뷽ʽ
	char c[4];
	if (codec == 0){
		codec = getCodec(c);
	}
	return writer.open(Outputfile,
		codec,
		framerate,
		getFrameSize(),
		isColor);
}

int VideoProcessor::getCodec(char codec[4])
{
	if (images.size() != 0)
		return -1;
	union { // ���ݽṹ4-char
		int value;
		char code[4];
	} returned;
	//��ñ���ֵ
	returned.value = static_cast<int>(
		caputure.get(CV_CAP_PROP_FOURCC));
	// get the 4 characters
	codec[0] = returned.code[0];
	codec[1] = returned.code[1];
	codec[2] = returned.code[2];
	codec[3] = returned.code[3];
	return returned.value;
}

void VideoProcessor::setFrameProcessor(FrameProcessor *frameprocessor)
{
	process = 0;
	this->frameprocessor = frameprocessor;
	CallProcess();
}

void VideoProcessor::setFrameProcessor(void(*process)(Mat &, Mat &))
{
	frameprocessor = 0;
	this->process = process;
	CallProcess();
}

bool VideoProcessor::setInput(const vector<string> &imgs)
{
	fnumber = 0;
	caputure.release();
	images = imgs;
	itImg = images.begin();
	return true;
}

bool VideoProcessor::setInput(string filename)
{
	fnumber = 0;
	//���Ѵ򿪣��ͷ����´�
	caputure.release();
	return caputure.open(filename);
}

double VideoProcessor::getFrameRate()
{
	return caputure.get(CV_CAP_PROP_FPS);
}

Size VideoProcessor::getFrameSize()
{
	if (images.size() == 0) {
		// ����Ƶ�����֡��С
		int w = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));
		int h = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));
		return Size(w, h);
	}
	else {
		//��ͼ����֡��С
		cv::Mat tmp = cv::imread(images[0]);
		return (tmp.data) ? (tmp.size()) : (Size(0, 0));
	}
}

long VideoProcessor::getFrameNumber()
{
	long fnumber = static_cast<long>(caputure.get((CV_CAP_PROP_POS_FRAMES)));
	return fnumber;
}

void VideoProcessor::stopAtFrameNo(long frame)
{
	frameToStop = frame;
}

void VideoProcessor::dontCallProcess()
{
	callIt = false;
}

void VideoProcessor::CallProcess()
{
	callIt = true;
}

bool VideoProcessor::readNextFrame(Mat &frame)
{
	if (images.size() == 0)
		return caputure.read(frame);
	else{
		if (itImg != images.end()){
			frame = imread(*itImg);
			itImg++;
			return frame.data ? 1 : 0;
		}
		else
			return false;
	}
}

void VideoProcessor::setDelay(int d)
{
	delay = d;
}

bool VideoProcessor::isOpened()
{
	return  caputure.isOpened() || !images.empty();
}

bool VideoProcessor::isStopped()
{
	return stop;
}

void VideoProcessor::stopIt()
{
	stop = true;
}

void VideoProcessor::run()
{
	Mat frame;
	Mat output;
	if (!isOpened())
		return;
	stop = false;
	while (!isStopped()){
		//��ȡ��һ֡
		if (!readNextFrame(frame))
			break;
		if (WindowNameInput.length() != 0)
			imshow(WindowNameInput, frame);
		//�����֡
		if (callIt){
			if (process)
				process(frame, output);
			else if (frameprocessor)
				frameprocessor->process(frame, output);
		}
		else{
			output = frame;
		}
		if (Outputfile.length()){
			cvtColor(output, output, CV_GRAY2BGR);
			writeNextFrame(output);
		}
		if (WindowNameOutput.length() != 0)
			imshow(WindowNameOutput, output);
		//������ͣ��������������
		//if (delay >= 0 && waitKey(delay) >= 0)
		cvWaitKey(0);
 		//����ָ����ͣ�����˳�
		if (frameToStop >= 0 && getFrameNumber() == frameToStop)
			stopIt();
	}
}

void VideoProcessor::dontDisplay()
{
	destroyWindow(WindowNameInput);
	destroyWindow(WindowNameOutput);
	WindowNameInput.clear();
	WindowNameOutput.clear();
}

void VideoProcessor::displayOutput(string wn)
{
	WindowNameOutput = wn;
	namedWindow(WindowNameOutput);
}

void VideoProcessor::displayInput(string wn)
{
	WindowNameInput = wn;
	namedWindow(WindowNameInput);
}

VideoProcessor::VideoProcessor() : callIt(true), delay(0), fnumber(0), stop(false), digits(0), frameToStop(-1)
{

}

void canny(cv::Mat& img, cv::Mat& out) {
	//�Ҷȱ任
	if (img.channels() == 3)
		cvtColor(img, out, CV_BGR2GRAY);
	// canny�������Ե
	Canny(out, out, 100, 200);
	//��ɫ��ת�������������Щ
	threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}