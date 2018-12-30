#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
int main()
{
	Mat image = imread("pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	for (int counter = 0; counter < image.rows; counter++)
	{
		for (int counter1 = 0; counter1 < image.cols; counter1++)
		{
			if (image.at<uchar>(counter, counter1) > 85)
			{
				image.at<uchar>(counter, counter1) = 255;
			}
		}
	}
	imwrite("mylogo.png", image);
}