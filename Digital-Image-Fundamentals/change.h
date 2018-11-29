#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>

using namespace std;
using namespace cv;

//��ͼ�����ƽ�Ʋ���
void shift(Mat &image, int dx, int dy)
{
	Mat image_shift(image.rows + dx, image.cols + dy, CV_8UC3);//�½�һ������ԭͼ��ͼ�񣬱���
	//һ�������ؽ��д���
	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			//��ԭ�������ƽ�ƣ�����ԭ�����ϵ�RGBֵ����������
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_shift.at<Vec3b>(counter1 + dx, counter2 + dy)[2] = image.at<Vec3b>(counter1, counter2)[2];
		}
	}
	//imshow("ƽ��", image_shift);
	imwrite("1.jpg", image_shift);//��ƽ�ƺ��ͼ��洢Ϊ����Ϊ1.jpg���ļ�
}

//���г߶ȱ任
void scale(Mat &image, double mul_num)
{
	int row_num = (int)floor(image.rows * mul_num), col_num = (int)floor(image.cols * mul_num);
	Mat image_scale(row_num, col_num, CV_8UC3);
	for (int counter1 = 0; counter1 < row_num; counter1++)
	{
		for (int counter2 = 0; counter2 < col_num; counter2++)
		{
			//��ɸ����С�����ϻ��Ĵ�С
			int x = (int)1.0 / mul_num * counter1, y = (int)1.0 / mul_num * counter2;
			image_scale.at<Vec3b>(counter1, counter2)[0] = image.at<Vec3b>(x, y)[0];
			image_scale.at<Vec3b>(counter1, counter2)[1] = image.at<Vec3b>(x, y)[1];
			image_scale.at<Vec3b>(counter1, counter2)[2] = image.at<Vec3b>(x, y)[2];
		}
	}
	//imshow("�߶ȱ任", image_scale);
	imwrite("2.jpg", image_scale);//�洢�任���ͼ��
}

//������ת
void rotation(Mat &image, double angle_in)
{
	double angle = angle_in * 3.1415926 / 180.0;//�ѽǶȻ��ɻ���
	int dis = ceil(sqrt(pow(image.rows, 2) + pow(image.cols, 2)));//��ͼ��ĳ���Ӧ����ԭͼ��ĶԽ��߳���
	Mat image_nrota(dis, dis, CV_8UC3);

	for (int counter1 = 0; counter1 < image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image.cols; counter2++)
		{
			//����Ӧ��ƽ�Ƶľ���
			float cenX = image_nrota.rows / 2 - ((image.rows / 2) * cos(angle) - (image.cols / 2) * sin(angle)),
				cenY = image_nrota.cols / 2 - ((image.rows / 2) * sin(angle) + (image.cols / 2) * cos(angle));
			//���շ�������������ƽ��
			int x = counter1 * cos(angle) - counter2 * sin(angle) + cenX,
				y = counter1 * sin(angle) + counter2 * cos(angle) + cenY;
			image_nrota.at<Vec3b>(x, y)[0] = image.at<Vec3b>(counter1, counter2)[0];
			image_nrota.at<Vec3b>(x, y)[1] = image.at<Vec3b>(counter1, counter2)[1];
			image_nrota.at<Vec3b>(x, y)[2] = image.at<Vec3b>(counter1, counter2)[2];

		}
	}
	//imshow("��ת", image_nrota);
	imwrite("3.jpg", image_nrota);
}

//��ֱƫ�Ʊ任
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
	//imshow("��ֱƫ��", image_voffset);
	imwrite("4.jpg", image_voffset);
}

//ˮƽƫ�Ʊ任
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
	//imshow("ˮƽƫ��", image_hoffset);
	imwrite("5.jpg", image_hoffset);
}



