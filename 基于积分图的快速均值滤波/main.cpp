#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
void MeanFilter(const Mat &input, Mat &output, int window_size);
int main()
{
	Mat img1 = imread("pic.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("img", img1);
	Mat img2;
	//加速
	MeanFilter(img1, img2, 3);
	imshow("result", img2);
	waitKey(0);
	system("pause");
	return 0;
}

void MeanFilter(const Mat &input, Mat &output, int window_size)
{
		int border = window_size / 2;
		//镜像处理边缘，借用output保存边缘处理后的图像
		copyMakeBorder(input, output, border, border, border, border, BorderTypes::BORDER_REFLECT);
		
		//申请存放积分图的空间
		int** s =new int*[output.rows];
		for (int counter = 0; counter < output.rows; counter++)
		{
			s[counter] = new int[output.cols];
		}

		
		//cout << output.cols << "  " << output.rows;
		//计算积分图
		int sum;
		for (int counter1 = 0; counter1 < output.rows; counter1++)
		{
			sum = 0;
			for (int counter2 = 0; counter2 < output.cols; counter2++)
			{
				sum = sum + (int)output.at<uchar>(counter1,counter2);
				//cout << sum << endl;
				//cout <<(int) output.at<uchar>(counter1, counter2) << " ";
				if (counter1 - 1 < 0)
				{
					s[counter1][counter2] = sum;
				}
				else
				{
					s[counter1][counter2] = s[counter1 -1][counter2] + sum;
				}
			}
			//cout << endl;
		}
		
		//进行滤波

		//output.resize(input.rows,input.cols);
		border = window_size / 2;
		for (int counter1 = border; counter1 < output.rows-border; counter1++)
		{
			for (int counter2 = border; counter2 < output.cols-border; counter2++)
			{
				int tmp1 = 0, tmp2 = 0, tmp3 = 0;
				if (counter1 - border - 1 < 0 || counter2 - border - 1 < 0)
				{
					tmp1 = 0;
					if (counter1 - border - 1 < 0)
						tmp3 = 0;
					else
						tmp3 = s[counter1 - border - 1][counter2 + border];
					if (counter2 - border - 1 < 0)
						tmp2 = 0;
					else
						tmp2 = s[counter1 + border][counter2 - border - 1];

				}
				else
				{
					tmp1 = s[counter1 - border - 1][counter2 - border - 1];
					tmp2 = s[counter1 + border][counter2 - border - 1];
					tmp3 = s[counter1 - border - 1][counter2 + border];
				}
				
				output.at<uchar>(counter1, counter2) = ((float)1 /(float)( window_size * window_size) )* (s[counter1 + border][counter2 + border] + tmp1 - tmp2 - tmp3);
				
			}
			//cout << endl;
		}
		
		//释放申请的内存
		for (int counter = 0; counter < output.rows; counter++)
		{
			delete[]s[counter];
		}
		delete[]s;
}