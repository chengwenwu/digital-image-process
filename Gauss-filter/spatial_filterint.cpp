#include "spatial_filtering.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

Spafilt::Spafilt()
{
	this->filterSize = 0;
	this->filter = NULL;
	this->imageMat = NULL;
}

Spafilt::~Spafilt()
{
	if(this->filter != NULL)
		delete []this->filter;
}

Mat Spafilt::linearFilter(const Mat &image_in, float filter_in[], int sizeOfFilter_in)
{
	this->setImage(image_in);
	this->setFilter(sizeOfFilter_in, filter_in);
	this->borderProcessing();

	Mat image_out(this->imageSource.rows, this->imageSource.cols, CV_8UC1);
	int add = (int)(this->filterSize / 2);
	for (int counter1 = 0; counter1 < image_out.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image_out.cols; counter2++)
		{
			float sum = 0;
			int coun = 0;
			for (int counter_1 = counter1; counter_1 < this->filterSize + counter1; counter_1++)
			{
				for (int counter_2 = counter2; counter_2 < this->filterSize + counter2; counter_2++)
				{
					sum += this->filter[coun] * this->imageAfterBorderProcess.at<uchar>(counter_1, counter_2);
					coun++;
				}
			}
			image_out.at<uchar>(counter1, counter2) = sum;
		}
	}
	return image_out;
}

Mat Spafilt::addSaltAndPepperNoise(Mat image_in, float rate)
{
	Mat image_out = image_in.clone();
	int numberOfNoise = rate * image_in.rows*image_in.cols;
	srand((unsigned)time(NULL));
	for (int counter = 0; counter < numberOfNoise; counter++)
	{
		int row = rand() % (image_in.rows),
			col = rand() % (image_in.cols);
		if (counter % 2 == 0)
		{
			image_out.at<uchar>(row, col) = 255;
		}
		else
		{
			image_out.at<uchar>(row, col) = 0;
		}
	}
	return image_out;
}

Mat Spafilt::medianFilter(Mat &image_in, int sizeOfFilter_in)
{
	this->setImage(image_in);
	this->setFilterSize(sizeOfFilter_in);
	this->borderProcessing();
	Mat image_out(this->imageSource.rows, this->imageSource.cols, CV_8UC1);
	int add = (int)(this->filterSize / 2);
	for (int counter1 = 0; counter1 < image_out.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image_out.cols; counter2++)
		{

			int* temp = new int[this->filterSize*this->filterSize];
			int coun = 0;
			for (int counter_1 = counter1; counter_1 < this->filterSize + counter1; counter_1++)
			{
				for (int counter_2 = counter2; counter_2 < this->filterSize + counter2; counter_2++)
				{
					//排序算法
					int counter3;
					for (counter3 = coun; counter3 > 0; counter3--)
					{
						if (this->imageAfterBorderProcess.at<uchar>(counter_1, counter_2) < temp[counter3 - 1])
						{
							temp[counter3] = temp[counter3 - 1];
						}
						else
						{
							break;
						}
					}
					temp[counter3] = this->imageAfterBorderProcess.at<uchar>(counter_1, counter_2);
					coun++;
				}
			}
			image_out.at<uchar>(counter1, counter2) = temp[this->filterSize*this->filterSize / 2];
		}
	}
	return image_out;
}

Mat Spafilt::maxFilter(Mat &image_in, int sizeOfFilter_in )
{
	this->setImage(image_in);
	this->setFilterSize(sizeOfFilter_in);
	this->borderProcessing();

	Mat image_out(this->imageSource.rows, this->imageSource.cols, CV_8UC1);
	int add = (int)(this->filterSize / 2);
	for (int counter1 = 0; counter1 < image_out.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image_out.cols; counter2++)
		{
			int max = imageAfterBorderProcess.at<uchar>(counter1, counter2);
			for (int counter_1 = counter1; counter_1 < this->filterSize + counter1; counter_1++)
			{
				for (int counter_2 = counter2; counter_2 < this->filterSize + counter2; counter_2++)
				{
					if (imageAfterBorderProcess.at<uchar>(counter_1, counter_2) > max)
						max = imageAfterBorderProcess.at<uchar>(counter_1, counter_2);
				}
			}
			image_out.at<uchar>(counter1, counter2) = max;
		}
	}
	return image_out;
}

Mat Spafilt::minFilter(Mat &image_in, int sizeOfFilter_in)
{
	this->setImage(image_in);
	this->setFilterSize(sizeOfFilter_in);
	this->borderProcessing();

	Mat image_out(this->imageSource.rows, this->imageSource.cols, CV_8UC1);
	int add = (int)(this->filterSize / 2);
	for (int counter1 = 0; counter1 < image_out.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image_out.cols; counter2++)
		{
			int min = imageAfterBorderProcess.at<uchar>(counter1, counter2);
			for (int counter_1 = counter1; counter_1 < this->filterSize + counter1; counter_1++)
			{
				for (int counter_2 = counter2; counter_2 < this->filterSize + counter2; counter_2++)
				{
					if (imageAfterBorderProcess.at<uchar>(counter_1, counter_2) < min)
						min = imageAfterBorderProcess.at<uchar>(counter_1, counter_2);
				}
			}
			image_out.at<uchar>(counter1, counter2) = min;
		}
	}
	return image_out;
}

Mat Spafilt::basicHighPassFilter(Mat &image_in, float filter_in[], int sizeOfFilter_in)
{
	this->setImage(image_in);
	this->setFilter(sizeOfFilter_in, filter_in);
	this->borderProcessing();
	this->preHighPassFilter();
	return this->scale();
}

Mat Spafilt::roberts(Mat &image_in)
{
	this->setImage(image_in);

	if (this->imageMat != NULL)
		delete[]imageMat;
	this->imageMat = new int[imageSource.rows*imageSource.cols];
	int counter = 0;
	for (int counter1 = 0; counter1 < imageSource.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < imageSource.cols; counter2++)
		{
			int Gx = 0, Gy = 0;
			if (counter1 + 1 != imageSource.rows && counter2 + 1 != imageSource.cols)
			{
				Gx = imageSource.at<uchar>(counter1 + 1, counter2 + 1) - imageSource.at<uchar>(counter1, counter2);
				Gy = imageSource.at<uchar>(counter1 + 1, counter2) - imageSource.at<uchar>(counter1, counter2 + 1);
			}
			else if (counter1 + 1 == imageSource.rows && counter2 + 1 != imageSource.cols)
			{
				//对称同时滤波
				Gx = imageSource.at<uchar>(counter1, counter2 + 1) - imageSource.at<uchar>(counter1, counter2);
				Gy = imageSource.at<uchar>(counter1, counter2) - imageSource.at<uchar>(counter1, counter2 + 1);
			}
			else if (counter2 + 1 == imageSource.cols && counter1 + 1 != imageSource.rows)
			{
				Gx = Gy = imageSource.at<uchar>(counter1 + 1, counter2) - imageSource.at<uchar>(counter1, counter2);
			}
			else if (counter2 + 1 == imageSource.cols && counter1 + 1 == imageSource.rows)
			{
				Gx = 128 - imageSource.at<uchar>(counter1, counter2);
				Gy = 0;
			}
			this->imageMat[counter] = abs(Gx) + abs(Gy);
			counter++;
		}
	}
	return scale();
}

Mat Spafilt::prewitt(Mat &image_in)
{
	this->setImage(image_in);

	float filterR[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 },
		filterC[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	return nonLinearHighPass3x3filter(filterR, filterC);
}

Mat Spafilt::sobel(Mat &image_in)
{
	this->setImage(image_in);

	float filterR[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 },
		filterC[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	return nonLinearHighPass3x3filter(filterR, filterC);
}

Mat Spafilt::laplacian(Mat &image_in, int id)
{
	this->setImage(image_in);

	switch (id)
	{
	case 1:
	{
		float filter[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
		setFilter(3, filter);
		break;
	}
	case 2:
	{
		float filter[9] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };
		setFilter(3, filter);
		break;
	}
	case 3:
	{
		float filter[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
		setFilter(3, filter);
		break;
	}
	case 4:
	{
		float filter[9] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
		setFilter(3, filter);
		break;
	}
	default:
	{
		cout << "Laplacian mask is wrong!" << endl;
		system("pause");
		exit(-1);
	}

	}

	this->borderProcessing();
	this->preHighPassFilter();

	float coefficient = 1;
	if (filter[4] < 0)
		coefficient = -1;
	this->addOrSubtractOfTwoImage(coefficient);

	return this->scale();
}

Mat Spafilt::highBoostFilter(Mat &image_in, float k)
{
	this->setImage(image_in);
	float filter_in[9];
	for (int counter = 0; counter < 9; counter++)
		filter_in[counter] = (float)1 / 9;
	Mat smoothingImage = linearFilter(this->imageSource, filter_in, 3);

	addOrSubtractOfTwoImage(-1, &smoothingImage);
	addOrSubtractOfTwoImage(k);

	return scale();
}

void Spafilt::setFilter(int size_i, float *fil)//设置滤波器大小，这里只考虑方形的滤波器（长宽相等）
{
	this->filterSize = size_i;
	this->filter = new float[size_i * size_i];
	for (int counter = 0; counter < size_i * size_i; counter++)
	{
		this->filter[counter] = fil[counter];
	}
}

void Spafilt::setFilterSize(int size_i)
{
	this->filterSize = size_i;
}

void Spafilt::setImage(const Mat &image_i)//设置将要处理的图片
{
	imageSource.create(cvSize(image_i.cols, image_i.rows), CV_8UC1);
	this->imageSource = image_i;
}

void Spafilt::borderProcessing()//对边缘进行处理
{
	int add = (int)(this->filterSize / 2) * 2;
	imageAfterBorderProcess.create(cvSize( imageSource.cols + add, imageSource.rows + add), CV_8UC1);
	for (int counter1 = 0; counter1 < imageAfterBorderProcess.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < imageAfterBorderProcess.cols; counter2++)
		{
			//当在四个角落的时候直接用128填充
			if ((counter1 < add/2 && counter2 < add/2) || (counter1 >= add/2 + imageSource.rows && counter2 < add/2)
				|| (counter2 >= add/2 + imageSource.cols && counter1 < add/2) || (counter1 >= add/2 + imageSource.rows && counter2 >= add/2 + imageSource.cols))
			{
				imageAfterBorderProcess.at<uchar>(counter1, counter2) = 128;
			}
			//在中间部分的时候
			else if ((counter1 >= add/2 && counter1 < add/2 + imageSource.rows) && (counter2 >= add/2 && counter2 < add/2 + imageSource.cols))
			{
				imageAfterBorderProcess.at<uchar>(counter1, counter2) = imageSource.at<uchar>(counter1 - (add / 2), counter2 - (add / 2));
			}
			//在非角落的边缘部分的时候
			else
			{
				if (counter1 < add / 2)
				{
					imageAfterBorderProcess.at<uchar>(counter1, counter2) = imageSource.at<uchar>(add / 2 - 1 - counter1, counter2 - add / 2);
				}
				if (counter1 >= add / 2 + imageSource.rows)
				{
					imageAfterBorderProcess.at<uchar>(counter1, counter2) = imageSource.at<uchar>(  (imageSource.rows - add/4 )+  ((imageSource.rows - add/4 -1 ) -( counter1 - add))-1, counter2 - add / 2);
				}
				if (counter2 < add / 2)
				{
					imageAfterBorderProcess.at<uchar>(counter1, counter2) = imageSource.at<uchar>(counter1 - add / 2, add / 2 - 1 - counter2);
				}
				if (counter2 >= add / 2 + imageSource.cols) 
				{
					imageAfterBorderProcess.at<uchar>(counter1, counter2) = imageSource.at<uchar>(counter1 - add / 2, (imageSource.cols - add / 4) + ((imageSource.cols - add / 4 - 1) - (counter2 - add)) - 1);
				}
			}
		}
	}
	/*string s = "borderprocess"+ rand();
	imshow(s, this->imageAfterBorderProcess);*/
}

void Spafilt::addOrSubtractOfTwoImage(float coefficient, Mat* image_in )
{
//	cout << this->imageMat[123] << endl;
	if (image_in == NULL)//没有传进来图片
	{
		int counter = 0;
		for (int counter1 = 0; counter1 < imageSource.rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imageSource.cols; counter2++)
			{
				this->imageMat[counter] = this->imageSource.at<uchar>(counter1, counter2) + coefficient * imageMat[counter];
				counter++;
			}
		}
	}
	else
	{
		if (this->imageMat != NULL)
			delete []imageMat;
		this->imageMat = new int[imageSource.rows*imageSource.cols];
		int counter = 0;
		for (int counter1 = 0; counter1 < imageSource.rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imageSource.cols; counter2++)
			{
				this->imageMat[counter] = this->imageSource.at<uchar>(counter1,counter2) + coefficient* (*image_in).at<uchar>(counter1,counter2);
				counter++;
			}
		}
	}
}

void  Spafilt::preHighPassFilter()
{
	int add = (int)(this->filterSize / 2), counter = 0;
	if (this->imageMat != NULL)
		delete[]imageMat;
	this->imageMat = new int[this->imageSource.rows*this->imageSource.cols];
	for (int counter1 = 0; counter1 < imageSource.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < imageSource.cols; counter2++)
		{
			float sum = 0;
			int coun = 0;
			for (int counter_1 = counter1; counter_1 < this->filterSize + counter1; counter_1++)
			{
				for (int counter_2 = counter2; counter_2 < this->filterSize + counter2; counter_2++)
				{
					sum += this->filter[coun] * this->imageAfterBorderProcess.at<uchar>(counter_1, counter_2);
					coun++;
				}
			}
			this->imageMat[counter] = (int)sum;
			counter++;
		}
	}
}

Mat Spafilt::scale(int k)
{
	return Debug();
	//查找最小灰度值（可能是负数）， 和最大灰度值。
	int minGray = this->imageMat[0],  maxGray = this->imageMat[0];
	for (int counter = 1; counter < this->imageSource.rows*this->imageSource.cols; counter++)
	{
		if (this->imageMat[counter] < minGray)
		{
			minGray = this->imageMat[counter];
		}
		if (this->imageMat[counter] > maxGray)
		{
			maxGray = this->imageMat[counter];
		}
	}
	//进行标定操作
	Mat image_out = this->imageSource.clone();
	int counter = 0;
	float f, maxf = (maxGray - minGray),fs;
	for (int counter1 = 0; counter1 < image_out.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < image_out.cols; counter2++)
		{
			f = (imageMat[counter] - minGray);
			fs = (float)(f / maxf);
			if (fs == 1)
			{
				cout << "Yes";
			}
			image_out.at<uchar>(counter1, counter2) =  k * fs;
			counter++;
		}
	}
	return image_out;
}

Mat  Spafilt::nonLinearHighPass3x3filter(float* filterR, float* filterC)
{
	this->setFilterSize(3);
	this->borderProcessing();

	this->setFilter(3, filterR);
	this->preHighPassFilter();
	int* temp1 = new int[imageSource.rows*imageSource.cols];
	memcpy(temp1, this->imageMat, sizeof(int)*imageSource.rows*imageSource.cols);

	this->setFilter(3, filterC);
	this->preHighPassFilter();

	for (int counter = 0; counter < imageSource.rows*imageSource.cols; counter++)
	{
		this->imageMat[counter] = abs(this->imageMat[counter]) + temp1[counter];
	}
	delete[]temp1;

	return scale();
}

Mat Spafilt::Debug()
{
	Mat image_out(imageSource.rows, imageSource.cols, CV_8UC1);
	/*imshow("边缘处理后的图像", this->imageAfterBorderProcess);
	imwrite("pic1.jpg", this->imageAfterBorderProcess);*/
	/*this->imageMat = new int[this->imageSource.rows*this->imageSource.cols];
	for (int counter = 0; counter < this->imageSource.rows*this->imageSource.cols; counter++)
	{
		if (counter < imageSource.rows / 2 * imageSource.cols)
			this->imageMat[counter] =0;
		else
			this->imageMat[counter] =255;
	}*/
	int counter = 0;
	for (int counter1 = 0; counter1 < imageSource.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < imageSource.cols; counter2++)
		{
			if (this->imageMat[counter] < 0)
			{
				image_out.at<uchar>(counter1, counter2) = 0;
			}
			else if (this->imageMat[counter] > 255)
			{
				image_out.at<uchar>(counter1, counter2) = 255;
			}
			else
			{
				image_out.at<uchar>(counter1, counter2) = imageMat[counter];
			}
			
			counter++;
		}
	}
	return image_out;
	/*srand((unsigned )time(NULL));
	string s = "borderprocess"+ rand();
	imshow(s,image_out);*/
}