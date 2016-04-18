#include "FrameProcessor.h"

void FeatureTracker::handleTrackedPoint(Mat &frame, Mat &output)
{
	for (int i = 0; i < points[1].size(); i++){
		//当前特征点到初始位置用直线表示
		Scalar s = CV_RGB(255, 0, 0);
		line(output, initial[i], points[1][i], s);
		//当前位置用圈标出
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
	//若特征点数目少于10，则决定添加特征点
	return points[0].size() <= 10;
}

void FeatureTracker::detectFeaturePoint()
{

	goodFeaturesToTrack(gray,//图片
		features,//输出特征点
		max_count,//特征点最大数目
		qlevel,//质量指标
		30);//最小容忍距离
}

void FeatureTracker::process(Mat &frame, Mat &output)
{
	//得到灰度图
	cvtColor(frame, gray, CV_BGR2GRAY);
	frame.copyTo(output);
	//特征点太少了，重新检测特征点
	if (addNewPoint()){
		detectFeaturePoint();
		//插入检测到的特征点
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
	}
	//第一帧
	if (gray_prev.empty()){
		gray.copyTo(gray_prev);
	}
	//根据前后两帧灰度图估计前一帧特征点在当前帧的位置
	//默认窗口是15*15
	calcOpticalFlowPyrLK(
		gray_prev,//前一帧灰度图
		gray,//当前帧灰度图
		points[0],//前一帧特征点位置
		points[1],//当前帧特征点位置
		status,//特征点被成功跟踪的标志
		err);//前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点
	int k = 0;
	//去除那些未移动的特征点
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
	//标记被跟踪的特征点
	cout << "this vector is hahaha" << endl;
	handleTrackedPoint(frame, output);
	//为下一帧跟踪初始化特征点集和灰度图像
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
	//如果扩展名不为空，写到图片文件中
	if (extension.length()){
		stringstream ss;
		ss << Outputfile << setfill('0') << setw(digits) << currentIndex++ << extension;
		imwrite(ss.str(), frame);
	}
	//反之，写到视频文件中
	else{
		writer.write(frame);
	}
}

bool VideoProcessor::setOutput(const string &filename,/*路径 */ const string &ext,/*扩展名 */ int numberOfDigits /*= 3*/,/*数字位数 */ int startIndex /*= 0*/)
{
	//起始索引
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
	//设置文件名
	Outputfile = filename;
	//清空扩展名
	extension.clear();
	//设置帧率
	if (framerate == 0.0){
		framerate = getFrameRate();
	}
	//获取输入原视频的编码方式
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
	union { // 数据结构4-char
		int value;
		char code[4];
	} returned;
	//获得编码值
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
	//若已打开，释放重新打开
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
		// 从视频流获得帧大小
		int w = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));
		int h = static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));
		return Size(w, h);
	}
	else {
		//从图像获得帧大小
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
		//读取下一帧
		if (!readNextFrame(frame))
			break;
		if (WindowNameInput.length() != 0)
			imshow(WindowNameInput, frame);
		//处理该帧
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
		//按键暂停，继续按键继续
		//if (delay >= 0 && waitKey(delay) >= 0)
		cvWaitKey(0);
 		//到达指定暂停键，退出
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
	//灰度变换
	if (img.channels() == 3)
		cvtColor(img, out, CV_BGR2GRAY);
	// canny算子求边缘
	Canny(out, out, 100, 200);
	//颜色反转，看起来更舒服些
	threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}