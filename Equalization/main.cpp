#include "Equalization.h"
#include <opencv2/highgui/highgui.hpp>
int main()
{
	namedWindow("原图", CV_WINDOW_AUTOSIZE);
	Mat image1 = imread("pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("原灰度图", image1);
	Equalization equ1;
	equ1.setImage(image1);
	equ1.statistic();
	Mat new_im1 = equ1.queProcess();

	imshow("变换后灰度图像", new_im1);
	//直方图均衡

	Mat image = imread("pic2.jpg", CV_LOAD_IMAGE_COLOR);

	
	imshow("原彩色图", image);

	Equalization equ;
	equ.setImage(image);
	equ.statistic();
	Mat new_im = equ.queProcess();

	imshow("变换后彩色图像", new_im);
	waitKey(0);
	return 0;
}