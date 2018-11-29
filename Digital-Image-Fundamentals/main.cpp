#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include "change.h"

using namespace std;
using namespace cv;
int main()
{
	//��ԭͼ
	Mat image;
	image = imread("pic.jpg", CV_LOAD_IMAGE_COLOR);

	if (!image.data)
	{
		cout << "�Ҳ���ͼƬ���޷���ͼƬ\n";
		return -1;
	}
	namedWindow("image_source", CV_WINDOW_AUTOSIZE);
	imshow("ԭͼ", image);
	
	//ƽ��
	shift(image, 10, 20);

	//����
	scale(image, 0.8);

	//��ת
	rotation(image, 45);

	//��ֱƫ��
	voffset(image, 1);

	//ˮƽƫ��
	hoffset(image, 1);
	//�������ű任���ͼ��
	string picname = "";
	Mat image2;
	for (int counter = 1; counter < 6; counter++)
	{
		picname = '0'+ counter;
		picname += ".jpg";
		image2 = imread(picname,CV_LOAD_IMAGE_COLOR);
		imshow("�任���ͼ��", image2);	
		waitKey(3000);
	}

	waitKey(0);
	return 0;
}