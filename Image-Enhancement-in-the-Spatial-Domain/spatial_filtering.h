#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class Spafilt {
public:
	Spafilt();
	~Spafilt();

	/*
	���Ե�ͨ�˲���
	@image_in:��Ҫ�����ͼƬ��
	@filter_in[]:ʹ�õ��˲���ģ�塣
	@sizeOfFilter_in���˲�����С
	@return��������ͼƬ
	*/
	Mat linearFilter(Mat &image_in, float filter_in[], int sizeOfFilter_in);
	
	/*
	��ͼƬ���Ͻ�������
	@Image_in����Ҫ�����ͼƬ
	@rate:�����̶ȣ����飨0-0.5��
	@return: �����������ͼƬ
	*/
	Mat addSaltAndPepperNoise(Mat image_in, float rate = 0.2);

	/*
	��ֵ�˲���
	@image_in:��Ҫ�����ͼƬ��
	@sizeOfFiter_in;�˲�����С
	@return��������ͼƬ��
	*/
	Mat medianFilter(Mat &image_in, int sizeOfFilter_in = 3);

	/*
	���ֵ�˲���
	@image_in:��Ҫ�����ͼƬ��
	@sizeOfFiter_in;�˲�����С
	@return��������ͼƬ��
	*/
	Mat maxFilter(Mat &image_in, int sizeOfFilter_in = 3);

	/*
	��Сֵ�˲���
	@image_in:��Ҫ�����ͼƬ��
	@sizeOfFiter_in;�˲�����С
	@return��������ͼƬ��
	*/
	Mat minFilter(Mat &image_in, int sizeOfFilter_in = 3);

	/*
	������ͨ�˲���
	@image_in:��Ҫ�����ͼƬ��
	@filter_in[]:ʹ�õ��˲���ģ�塣
	@sizeOfFilter_in���˲�����С
	@return��������ͼƬ
	*/
	Mat basicHighPassFilter(Mat &image_in, float filter_in[], int sizeOfFilter_in);

	/*
	�޲�������
	@image_in:��Ҫ�����ͼƬ
	@return��������ͼƬ
	*/
	Mat roberts(Mat &image_in);

	/*
	prewitt����
	@image_in:��Ҫ�����ͼƬ
	@return��������ͼƬ
	*/
	Mat prewitt(Mat &image_in);

	/*
	Sobel����
	*/
	Mat sobel(Mat &image_in);

	/*
	@image_in:��Ҫ�����ͼƬ
	@id:ѡ��ʹ���ĸ�ģ�崦��idӦ����1-4�е�ĳһ���������ʵ�鱨�档
	@return�� Laplacian������ͼƬ
	*/
	Mat laplacian(Mat &image_in, int id);
	
	/*
	��Ƶ�����˲���
	@k;��Ƶ������ģ���Ȩ��,Ĭ��Ϊ1
	*/
	Mat highBoostFilter(Mat &image_in, float k = 1);
	
	/*
	@ԭ����һ��Э���ҵ��Գ���ĺ���
	@����������������ʹ�ýضϷ�ʽ�궨������
	@��Mat scale(int k = 255);�е�����
	*/
	Mat Debug();
private:
	Mat imageSource;//�������ͼƬ
	Mat imageAfterBorderProcess;//�Ա�Ե���д�����ͼƬ��
	int* imageMat;//��ž���ĳЩ����󣬻Ҷ�ֵ����0-255�����ͼƬ���ȴ����������Ӧ��
	int filterSize;//�˲�����С
	float *filter;//�˲���ģ��

	/*
	�����˲�����С��ģ�壬����ֻ���Ƿ��ε��˲�����������ȣ�
	@size_i:�˲�����С��ֻ���Ƿ����˲�����������һ�߳���
	@fil:�˲���ģ��
	*/
	void setFilter(int size_i, float *fil);

	/*
	�����˲�����С����ֻ���Ƿ��ε��˲�����������ȣ�
	@size_i:�˲�����С��ֻ���Ƿ����˲�����������һ�߳���
	*/
	void setFilterSize(int size_i);

	/*
	���ý�Ҫ�����ͼƬ
	@image_i:��Ҫ�����ͼƬ
	*/
	void setImage(Mat &image_i);

	/*
	��ͼ��ı�Ե���д���
	* �������imageSourceͼƬ
	* �����Ľ�������imageAfterBorderProcess��
	* ���þ�����취�����䴦��ֵ��Ϊ128
	*/
	void borderProcessing();

	/*
	* ����������ͼ��Ĵ�Ȩ�Ĳ���ߺͣ�
	* ������ͼ���ͼ���ͼ�������֮������
	* ��һ��ϵ�������Ƽӻ��߼���������ϵ��
	* �����������imageMat��
	@coefficient:ϵ������������ͼ��֮�������ô���Ĳ���
	@image_in: ������ò����������imageSource��image_in֮������㣬
	           �������imageSource��imageMat֮������
	*/
	void addOrSubtractOfTwoImage(float coefficient, Mat* image_in = NULL);

	/*
	* ʹ�������˲�����ͼƬ�����˲�
	* ���ڸ�ͨ�˲���ģ�崦���Ҷ�ֵ�п�����������Խ������imageMat�У�����һ������
	* ʹ��˽�б����е�filter��imageSource�����˲�
	*/
	void preHighPassFilter();

	/*
	�����������ڵ����еĻҶ�ֵ�궨��ָ���Ҷ����䲢��Mat����ʽ���س���
	���޸���������
	@����k:��ʾ�궨���Ͻ�
	�½�Ϊ��
	@return�����ر궨���ͼ��
	*/
	Mat scale(int k = 255);

	/*
	�����Ը�ͨ3x3�˲���
	@filterR:��СΪ3x3���˲���ģ��
	@filterC:��СΪ3x3���˲���ģ��
	@return������󾭹��궨��ͼ��
	*/
	Mat nonLinearHighPass3x3filter(float* filterR, float* filterC);

};
