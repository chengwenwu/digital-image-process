#include "Equalization.h"
#include <opencv2/highgui/highgui.hpp>
int main()
{
	namedWindow("ԭͼ", CV_WINDOW_AUTOSIZE);
	Mat image1 = imread("pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("ԭ�Ҷ�ͼ", image1);
	Equalization equ1;
	equ1.setImage(image1);
	equ1.statistic();
	Mat new_im1 = equ1.queProcess();

	imshow("�任��Ҷ�ͼ��", new_im1);
	//ֱ��ͼ����

	Mat image = imread("pic2.jpg", CV_LOAD_IMAGE_COLOR);

	
	imshow("ԭ��ɫͼ", image);

	Equalization equ;
	equ.setImage(image);
	equ.statistic();
	Mat new_im = equ.queProcess();

	imshow("�任���ɫͼ��", new_im);
	waitKey(0);
	return 0;
}