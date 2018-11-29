#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <iostream>
#define LEAVE 256
using namespace cv;

class Equalization {
public:
	//��ʼ������˽�г�Ա��
	Equalization();
	//����Ҫ�����ͼƬ
	void setImage(Mat &image_in );
	//�ֱ��ͼƬ��RGBֵ����ֱ��ͼͳ��
	void statistic();
	//��ԭͼ���о��⴦��
	Mat queProcess();
private:
	//����˽�б����ֱ�ͳ��channel 0, 1,2�ĻҶȼ������ظ���
	int statistic_R[LEAVE];
	int statistic_G[LEAVE];
	int statistic_B[LEAVE];
	//��Ҫ�����ͼ��
	Mat image;
};
