#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "spatial_filtering.h"
using namespace std;
using namespace cv;
void Gaussian(const Mat &input, Mat &output, double sigma, bool accelerate = false);
int main()
{
	Mat img1 = imread("pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2;
	//不加速
	imshow("source_image", img1);
	Gaussian(img1, img2, 0.7);
	imshow("result1", img2);
	//加速
	Gaussian(img1, img2, 0.7,true);
	imshow("result2", img2);
	waitKey(0);
	system("pause");
	return 0;
}

void Gaussian(const Mat &input, Mat &output, double sigma, bool accelerate)
{
	int tmpint = 6 * sigma - 1 + 0.5;//进行了四射五入
	int size = tmpint/2 * 2 + 1;//计算模板大小
	
	if (!accelerate)//不加速
	{
		//由于我下面进行滤波时要用一维数组传参，所以这里用一维数组存放模板
		float* templa = new float[size*size];

		float x = 0, y = 0, center = size / 2;
		float a = 0;
		int counter0 = 0;
		for (int counter1 = 0; counter1 < size; counter1++)
		{
			x = counter1 - center;
			for (int counter2 = 0; counter2 < size; counter2++)
			{
				y = counter2 - center;
				float tmpf = exp(-(float)(x*x + y * y) / (float)(2 * sigma*sigma));
				templa[counter0] = tmpf;
				counter0++;
				a += tmpf;
			}
		}
		for (int counter1 = 0; counter1 < size*size; counter1++)
		{//归一化处理
			templa[counter1] /= (float)a;
		}
		//进行滤波
		Spafilt spt;
		//线性低通滤波器
		output = spt.linearFilter(input, templa, size);
		//释放申请的空间
		delete[] templa;
	}
	else//加速
	{
		float *templa = new float[size];
		float sum = 0;
		int origin = size / 2;
		for (int counter = 0; counter < size; counter++)
		{//求一维高斯函数模板
			float g = exp(-(counter - origin) * (counter - origin) / (2 * sigma * sigma));
			sum += g;
			templa[counter] = g;
		}
		// 归一化
		for (int counter = 0; counter < size; counter++)
			templa[counter] /= sum;

		// 进行滤波
		int border = size / 2;
		//镜像处理边缘
		copyMakeBorder(input, output, border, border, border, border, BorderTypes::BORDER_REFLECT);
		int rows = output.rows - border;
		int cols = output.cols - border;
		// 水平方向
		for (int counter0 = border; counter0 < rows; counter0++)
		{
			for (int counter1 = border; counter1 < cols; counter1++)
			{
				float sum = 0;
				for (int counter2 = -border; counter2 <= border; counter2++)
				{
					sum += templa[border + counter2] * output.at<uchar>(counter0, counter1 + counter2); 
				}
				//进行截断处理，防止溢出
				if (sum < 0)
					sum = 0;
				else if (sum > 255)
					sum = 255;
				
				output.at<uchar>(counter0, counter1) = sum;
			}
		}
		// 竖直方向
		for (int counter0 = border; counter0 < rows; counter0++)
		{
			for (int counter1 = border; counter1 < cols; counter1++)
			{
				float sum = 0;
				for (int counter2 = -border; counter2 <= border; counter2++)
				{
					sum += templa[border + counter2] * output.at<uchar>(counter0 + counter2, counter1); 
				}
			
				if (sum < 0)
					sum = 0;
				else if (sum > 255)
					sum = 255;
				
				output.at<uchar>(counter0, counter1) = sum;
			}
		}
		delete[] templa;
	}
}