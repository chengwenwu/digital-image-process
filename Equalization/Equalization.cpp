#include "Equalization.h"
//初始化各个私有成员。
Equalization::Equalization()
{
	memset(this->statistic_R, 0, LEAVE*sizeof(int));
	memset(this->statistic_G, 0, LEAVE * sizeof(int));
	memset(this->statistic_B, 0, LEAVE * sizeof(int));
}

//设置要处理的图片的路径
void Equalization::setImage(Mat &image_in)
{
	this->image = image_in;
}

//分别对图片的RGB值进行直方图统计
void Equalization::statistic()
{
	for (int counter1 = 0; counter1 < this->image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < this->image.cols; counter2++)
		{
			//数组下标代表灰度值，里面存储的数是该灰度值对应的像素数
			if (this->image.channels() == 1)//如果是灰度图像
			{
				this->statistic_R[this->image.at<uchar>(counter1, counter2)]++;
			}
			else//如果是彩色图像
			{
				this->statistic_R[this->image.at<Vec3b>(counter1, counter2)[0]]++;
				this->statistic_G[this->image.at<Vec3b>(counter1, counter2)[1]]++;
				this->statistic_B[this->image.at<Vec3b>(counter1, counter2)[2]]++;
			}
			
			
		}
	}
}
//对原图进行均衡处理
Mat Equalization::queProcess()
{
	/*计算原来的各个灰度值经过均衡以后应该变成多少
	temp数组下标代表原先图像的灰度值
	temp中存储的数是变换后各个灰度值对应的灰度值
	*/
	int temp_R[LEAVE], temp_G[LEAVE], temp_B[LEAVE];
	memset(temp_R, 0, LEAVE * sizeof(int));
	memset(temp_G, 0, LEAVE * sizeof(int));
	memset(temp_B, 0, LEAVE * sizeof(int));
	int sumR = 0, sumB = 0, sumG = 0;
	double const_num = (double)(LEAVE - 1) /(double) (this->image.cols*this->image.rows);
	for (int counter0 = 0; counter0 < LEAVE; counter0++)
	{
		if (this->image.channels() == 1)
		{
			sumR += this->statistic_R[counter0];
			temp_R[counter0] = (int)(const_num * sumR + 0.5);
		}
		else
		{
			sumR += this->statistic_R[counter0];
			sumG += this->statistic_G[counter0];
			sumB += this->statistic_B[counter0];

			//进行四舍五入
			temp_R[counter0] = (int)(const_num * sumR + 0.5);
			temp_G[counter0] = (int)(const_num * sumG + 0.5);
			temp_B[counter0] = (int)(const_num * sumB + 0.5);
		}
	}
	Mat imEqu;
	if (image.channels() == 1)
	{
		Mat imEqu1(this->image.rows, this->image.cols, CV_8UC1);
		imEqu1.copyTo(imEqu);
	}
	else
	{
		Mat imEqu1(this->image.rows, this->image.cols, CV_8UC3);
		imEqu1.copyTo(imEqu);
	}
		
	
		//逐个像素进行处理
	for (int counter1 = 0; counter1 < this->image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < this->image.cols; counter2++)
		{
			if (this->image.channels() == 1)
			{
				imEqu.at<uchar>(counter1, counter2) = temp_R[image.at<uchar>(counter1, counter2)];
			}
			else
			{
				imEqu.at<Vec3b>(counter1, counter2)[0] = temp_R[image.at<Vec3b>(counter1, counter2)[0]];
				imEqu.at<Vec3b>(counter1, counter2)[1] = temp_G[image.at<Vec3b>(counter1, counter2)[1]];
				imEqu.at<Vec3b>(counter1, counter2)[2] = temp_B[image.at<Vec3b>(counter1, counter2)[2]];
			}
		}
	}
	return imEqu;
}

