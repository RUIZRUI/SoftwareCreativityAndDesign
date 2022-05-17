// homework2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int recVideo();
int camera2Img();
int video2Img();
int showCamera();
int readImg();
int optImg();
void LargestConnecttedComponent(Mat srcImage, Mat& dstImage);

int main()
{
	// recVideo();
	



	// 摄像头读取视频流，取5帧，转图片
	// camera2Img();
	// 从AVI文件获取视频流，取5帧，转图片
	// video2Img();
	// 实时显示摄像头的视频流
	// showCamera();
	// 读取图片并操作
	readImg();
	// 操作二值图片
	// optImg();
	return 0;
}

/// <summary>
/// 摄像头读取视频流，取5帧，转图片
/// </summary>
/// <returns></returns>
int camera2Img()
{
	cout << "摄像头读取视频流，取5帧，转图片" << endl;
	// 打开默认摄像头
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "错误！摄像机未工作..." << endl;
		return -1;
	}
	else
	{
		cout << "摄像机正常工作..." << endl;
	}
	Mat frame;
	// 读取5帧
	namedWindow("camera2Img");
	for (int i = 0; i < 550; i++)
	{
		// 从摄像机读取帧
		cap >> frame;
		imshow("camera2Img", frame);
		// 保存图片（每100帧保存一次）
		if (i == 100 || i == 200 || i == 300 || i == 400 || i == 500)
		{
			string imgName = "camera2Img_" + to_string(i) + ".png";
			imwrite(imgName, frame);
			cout << "图片：" << imgName << "保存成功" << endl;
		}
		if (waitKey(33) >= 0)
			break;
	}

	// 释放摄像机
	cap.release();
	return 0;
}


/// <summary>
/// 从AVI文件获取视频流，取5帧，转图片
/// </summary>
/// <returns></returns>
int video2Img()
{
	cout << "从AVI文件获取视频流，取5帧，转图片" << endl;
	// 打开 avi 视频文件
	VideoCapture cap;
	cap.open("video2Img.avi");
	if (!cap.isOpened())
	{
		cout << "错误，不能读取视频" << endl;
		return -1;
	}
	else
	{
		cout << "可以正常读取视频" << endl;
	}
	int frames = cap.get(CAP_PROP_FRAME_COUNT);
	cout << "视频帧数：" << frames << endl;

	Mat frame;
	// 读取5帧
	namedWindow("video2Img");
	for (int i = 0; i < 255; i++)
	{
		// 从视频读取帧
		cap >> frame;
		imshow("video2Img", frame);
		// 保存图片（每100帧保存一次）
		if (i == 50 || i == 100 || i == 150 || i == 200 || i == 250)
		{
			string imgName = "video2Img_" + to_string(i) + ".png";
			imwrite(imgName, frame);
			cout << "图片：" << imgName << "保存成功" << endl;
		}
		if (waitKey(33) >= 0)
			break;
	}
	

	// 释放视频资源
	cap.release();
	return 0;
}

/// <summary>
/// 实时显示摄像头的视频流
/// </summary>
/// <returns></returns>
int showCamera()
{
	cout << "实时显示摄像头的视频流" << endl;
	// 打开默认摄像头
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		cout << "错误！摄像机未工作..." << endl;
		return -1;
	}
	else
	{
		cout << "摄像机正常工作..." << endl;
	}
	Mat frame;
	namedWindow("showCamera");
	while (true)
	{
		// 从摄像机读取帧
		cap >> frame;
		imshow("showCamera", frame);
		if (waitKey(33) >= 0)
			break;
	}

	// 释放摄像机
	cap.release();
	return 0;
}


/// <summary>
/// 读取图片并操作
/// </summary>
/// <returns></returns>
int readImg()
{
	cout << "读取图片并操作" << endl;
	// 读取图片
	Mat img = imread("readImg.jpg", IMREAD_UNCHANGED);
	int width = img.cols;
	int height = img.rows;
	cout << "图片宽度：" << width << ", 高度：" << height << endl;
	// 旋转缩放
	Mat img2 = Mat(height, width, img.depth());
	// 旋转
	transpose(img, img2);
	flip(img2, img2, 0);
	// 缩放
	resize(img2, img2, Size(height / 2, width / 2));
	namedWindow("readImg1");
	imshow("readImg1", img);
	// namedWindow("readImg2");
	imshow("readImg3", img2);
	waitKey(0);
	return 0;
}



/// <summary>
/// 操作二值图片
/// </summary>
/// <returns></returns>
int optImg()
{
	cout << "操作二值图片" << endl;
	// 读取图片（灰度图）
	Mat img = imread("optImg.jpg", IMREAD_GRAYSCALE);
	// 二值图
	Mat img2;
	threshold(img, img2,127, 255, THRESH_BINARY);
	namedWindow("optImg1");
	imshow("optImg1", img2);

	// 腐蚀、膨胀、开、闭
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	// 腐蚀，减少高亮部分
	Mat img3;
	erode(img2, img3, kernel);
	imshow("腐蚀", img3);
	imwrite("腐蚀.jpg", img3);
	cout << "腐蚀文件写入成功" << endl;
	// 膨胀，增加高亮部分
	Mat img4;
	dilate(img2, img4, kernel);
	imshow("膨胀", img4);
	imwrite("膨胀.jpg", img4);
	cout << "膨胀文件写入成功" << endl;
	// 开操作
	Mat img5;
	morphologyEx(img2, img5, CV_MOP_OPEN, kernel);
	imshow("开操作", img5);
	imwrite("开操作.jpg", img5);
	cout << "开操作文件写入成功" << endl;
	// 闭操作
	Mat img6;
	morphologyEx(img2, img6, CV_MOP_CLOSE, kernel);
	imshow("闭操作", img6);
	imwrite("闭操作.jpg", img6);
	cout << "闭操作文件写入成功" << endl;
	// 提取连通分量
	Mat img7;
	LargestConnecttedComponent(img2, img7);
	imshow("提取最大两个连通分量", img7);
	imwrite("提取最大两个连通分量.jpg", img7);
	cout << "提取最大两个连通分量文件写入成功" << endl;

	waitKey(0);
	return 0;
}



void LargestConnecttedComponent(Mat srcImage, Mat& dstImage)
{
	Mat temp;
	Mat labels;
	srcImage.copyTo(temp);

	//1. 标记连通域
	int n_comps = connectedComponents(temp, labels, 4, CV_16U);
	vector<int> histogram_of_labels;
	for (int i = 0; i < n_comps; i++)//初始化labels的个数为0
	{
		histogram_of_labels.push_back(0);
	}

	int rows = labels.rows;
	int cols = labels.cols;
	for (int row = 0; row < rows; row++) //计算每个labels的个数
	{
		for (int col = 0; col < cols; col++)
		{
			histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
		}
	}
	histogram_of_labels.at(0) = 0; //将背景的labels个数设置为0

	//2. 计算最大的连通域labels索引
	int maximum = 0;
	int max_idx = 0;
	for (int i = 0; i < n_comps; i++)
	{
		if (histogram_of_labels.at(i) > maximum)
		{
			maximum = histogram_of_labels.at(i);
			max_idx = i;
		}
	}

	//3. 将最大连通域标记为1
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (labels.at<unsigned short>(row, col) == max_idx)
			{
				labels.at<unsigned short>(row, col) = 255;
			}
			else
			{
				labels.at<unsigned short>(row, col) = 0;
			}
		}
	}

	//4. 将图像更改为CV_8U格式
	labels.convertTo(dstImage, CV_8U);
}



int recVideo()
{
	cout << "-----------------使用摄像机-----------------" << endl;
	Mat in_frame, out_frame;
	const char win1[] = "抓取中...", win2[] = "记录中...";
	// 每秒帧数
	double fps = 30;
	char file_out[] = "记录.avi";

	// 打开默认摄像机
	VideoCapture inVid(0);
	// 检查错误
	if (!inVid.isOpened())
	{
		cout << "错误！摄像机未工作..." << endl;
		return -1;
	}

	// 获取输入视频的宽度和高度
	int width = (int)inVid.get(CAP_PROP_FRAME_WIDTH);
	int height = (int)inVid.get(CAP_PROP_FRAME_HEIGHT);
	cout << "输入视频的宽度：" << width << ", 高度：" << height << endl;
	// VideoWriter recVid(file_out, VideoWriter::fourcc('M', 'S', 'V', 'C'), fps, Size(width, height));
	VideoWriter recVid(file_out, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(width, height));
	if (!recVid.isOpened())
	{
		cout << "错误！视频文件没有打开..." << endl;
		return -1;
	}
	/* if (recVid.isOpened())
	{
		cout << "视频文件已经打开..." << endl;
	}
	else
	{
		cout << "视频文件没有打开..." << endl;
	} */

	// 为原始视频和最终视频创建两个窗口
	namedWindow(win1);
	namedWindow(win2);
	while (true)
	{
		// 从摄像机读取帧（抓取并解码）
		inVid >> in_frame;
		// 将帧转换为灰度
		cvtColor(in_frame, out_frame, COLOR_BGR2GRAY);
		// 将帧写入视频文件（编码并保存）
		recVid << out_frame; 
		// 在窗口中显示帧
		imshow(win1, in_frame);
		// 在窗口中显示帧
		imshow(win2, out_frame);
		if (waitKey(1000 / fps) >= 0)
			break;
	}

	// 关闭摄像机
	inVid.release();
	return 0; 
}


