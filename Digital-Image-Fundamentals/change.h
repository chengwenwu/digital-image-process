#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>

using namespace std;
using namespace cv;

//对图像进行平移操作
void shift(Mat &image, int dx, int dy)
{
	Mat image_shift(image.rows + dx, image.cols + dy, CV_8UC3);//新建一个大于原图的图像，备用
	//一个个像素进行处理
	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			//将原坐标进行平移，并将原坐标上的RGB值赋给新坐标
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[2] = image.at<Vec3b>(counter1, counter2)[2];
		}
	}
	//imshow("平移", image_shift);
	imwrite("1.jpg", image_shift);//将平移后的图像存储为名称为1.jpg的文件
}

//进行尺度变换
void scale(Mat &image, double mul_num)
{
	int row_num = (int)floor(image.rows * mul_num), col_num = (int)floor(image.cols * mul_num);
	Mat image_scale(row_num, col_num, CV_8UC3);
	for (int counter1 = 0; counter1 < row_num; counter1++)
	{
		for (int counter2 = 0; counter2 < col_num; counter2++)
		{
			//将筛孔缩小，对上画的大小
			int x = (int)1.0 / mul_num * counter1, y = (int)1.0 / mul_num * counter2;
			image_scale.at<Vec3b>(counter1, counter2)[0] = image.at<Vec3b>(x, y)[0];
			image_scale.at<Vec3b>(counter1, counter2)[1] = image.at<Vec3b>(x, y)[1];
			image_scale.at<Vec3b>(counter1, counter2)[2] = image.at<Vec3b>(x, y)[2];
		}
	}
	//imshow("尺度变换", image_scale);
	imwrite("2.jpg", image_scale);//存储变换后的图像
}

//进行旋转
void rotation(Mat &image, double angle_in)
{
	double angle = angle_in * 3.1415926 / 180.0;//把角度化成弧度
	int dis = ceil(sqrt(pow(image.rows, 2) + pow(image.cols, 2)));//新图像的长宽应该是原图像的对角线长度
	Mat image_nrota(dis, dis, CV_8UC3);

	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			//计算应该平移的距离
			float cenX = image_nrota.rows / 2 - ((image.rows / 2) * cos(angle) - (image.cols / 2) * sin(angle)),
				cenY = image_nrota.cols / 2 - ((image.rows / 2) * sin(angle) + (image.cols / 2) * cos(angle));
			//按照仿射矩阵计算后进行平移
			int x = counter1 * cos(angle) - counter2 * sin(angle) + cenX,
				y = counter1 * sin(angle) + counter2 * cos(angle) + cenY;
			image_nrota.at<Vec3b>(x, y)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_nrota.at<Vec3b>(x, y)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_nrota.at<Vec3b>(x, y)[2] = image.at<Vec3b>(counter1, counter2)[2];

		}
	}
	//imshow("旋转", image_nrota);
	imwrite("3.jpg", image_nrota);
}

//垂直偏移变换
void voffset(Mat &image, double sv)
{
	Mat image_voffset(sv*image.rows + image.cols, image.cols, CV_8UC3);
	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			int x = counter1 * sv + counter2;

			image_voffset.at<Vec3b>(x, counter2)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_voffset.at<Vec3b>(x, counter2)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_voffset.at<Vec3b>(x, counter2)[2] = image.at<Vec3b>(counter1, counter2)[2];

		}
	}
	//imshow("垂直偏移", image_voffset);
	imwrite("4.jpg", image_voffset);
}

//水平偏移变换
void hoffset(Mat &image, double sh)
{
	Mat image_hoffset(image.rows, sh*image.rows + image.cols, CV_8UC3);
	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			int y = counter1 * sh + counter2;

			image_hoffset.at<Vec3b>(counter1, y)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_hoffset.at<Vec3b>(counter1, y)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_hoffset.at<Vec3b>(counter1, y)[2] = image.at<Vec3b>(counter1, counter2)[2];
		}
	}
	//imshow("水平偏移", image_hoffset);
	imwrite("5.jpg", image_hoffset);
}



