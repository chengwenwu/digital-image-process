#include "Equalization.h"
//��ʼ������˽�г�Ա��
Equalization::Equalization()
{
	memset(this->statistic_R, 0, LEAVE*sizeof(int));
	memset(this->statistic_G, 0, LEAVE * sizeof(int));
	memset(this->statistic_B, 0, LEAVE * sizeof(int));
}

//����Ҫ�����ͼƬ��·��
void Equalization::setImage(Mat &image_in)
{
	this->image = image_in;
}

//�ֱ��ͼƬ��RGBֵ����ֱ��ͼͳ��
void Equalization::statistic()
{
	for (int counter1 = 0; counter1 < this->image.rows; counter1++)
	{
		for (int counter2 = 0; counter2 < this->image.cols; counter2++)
		{
			//�����±����Ҷ�ֵ������洢�����ǸûҶ�ֵ��Ӧ��������
			if (this->image.channels() == 1)//����ǻҶ�ͼ��
			{
				this->statistic_R[this->image.at<uchar>(counter1, counter2)]++;
			}
			else//����ǲ�ɫͼ��
			{
				this->statistic_R[this->image.at<Vec3b>(counter1, counter2)[0]]++;
				this->statistic_G[this->image.at<Vec3b>(counter1, counter2)[1]]++;
				this->statistic_B[this->image.at<Vec3b>(counter1, counter2)[2]]++;
			}
			
			
		}
	}
}
//��ԭͼ���о��⴦��
Mat Equalization::queProcess()
{
	/*����ԭ���ĸ����Ҷ�ֵ���������Ժ�Ӧ�ñ�ɶ���
	temp�����±����ԭ��ͼ��ĻҶ�ֵ
	temp�д洢�����Ǳ任������Ҷ�ֵ��Ӧ�ĻҶ�ֵ
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

			//������������
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
		
	
		//������ؽ��д���
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

