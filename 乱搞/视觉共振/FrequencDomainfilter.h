#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
using namespace std;
using namespace cv;

class FDF {
public:
	/*
	构造函数，可以直接传入文件名来创建
	*/
	FDF(string image_spa_in);

	/*
	如果创建对象时没有穿入文件名，可以后期设置文件名
	*/
	void setImage(string image_spa_in);

	/*
	*返回空域图像
	每次滤波时不会保存上次滤波的结果
	所以在每次滤波以后要及时调用该函数取得空域图像
	*/
	Mat getImageSpa();

	/*
	*返回频域图像
	每次滤波时不会保存上次滤波的结果
	所以在每次滤波以后要及时调用该函数取得频域图像
	*/
	Mat getImageFre();

	/*
	傅里叶变换，只需一开始调用一次，得到频域图像后结果会长期保存，无需多次调用
	*/
	void dft_();

	/*
	*傅里叶反变换
	每次对图像滤波结束以后可以用它来得到滤波后空域的图像
	*/
	void idft_();

	/*
	*下面是几种低通滤波器
	*/
	void ILPF(float d0);
	void IHPF(float d0);
	void BLPF(int n, float d0);
	void GLPF(float d0);


	

	/*
	*拉普拉斯滤波器
	*/
	void Laplacian();

	/*
	*这个函数用来设置频域图像
	*由于滤波操作是在复数（2通道Mat）上进行的，所以要显示频域图像的话首先要进行一些处理
	*/
	void setFreImg();//这个函数用来将各种滤波后的中间结果转变为频域图像存储在Image_fre中

	/*
	*添加噪音
	*/
	void addNoise();

	/*
	*将频域的两个图像加起来。
	*结果放在第一个图像的imat中
	*/
	friend void addtwoFreImg(FDF& img1,FDF& img2);
private:
	Mat image_spa;//存放原始空域图像
	Mat complexI;//存放复数形式的图像
	Mat imat[2];//这个是提供给各种滤波的，是将complexI分开并移中后的结果
	Mat Image_fre;//存放滤波后频域的图像

	void splitAndCenter();//将complexI分开并移中后的存储到imat2[]中
	void center(Mat& imag);//移中
	
};
float dist(const float& a, const float& b, const float& c, const float& d);

