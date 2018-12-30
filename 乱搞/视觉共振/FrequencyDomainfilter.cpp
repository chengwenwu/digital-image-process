#include <iostream>
#include "FrequencDomainfilter.h"


FDF::FDF(string image_spa_in)
{
	this->setImage(image_spa_in);
}

void FDF::setImage(string image_spa_in)
{
	this->image_spa = imread(image_spa_in, CV_LOAD_IMAGE_GRAYSCALE);
	if (this->image_spa.empty())
	{
		cout << " load image failed\n";
		system("pause");
	}
	else
	{
		cout << " load image success\n";
	}
}

Mat FDF::getImageSpa()
{
	return this->image_spa;
}

Mat	FDF::getImageFre()
{
	this->setFreImg();
	return this->Image_fre;
}

void FDF::dft_()
{
	//! [expand]
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(image_spa.rows);
	int n = getOptimalDFTSize(image_spa.cols); // on the border add zero values
	copyMakeBorder(image_spa, padded, 0, m - image_spa.rows, 0, n - image_spa.cols, BORDER_CONSTANT, Scalar::all(0));
	//! [expand]

	//! [complex_and_real]
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
//! [complex_and_real]

//! [dft]
	dft(complexI, complexI);         // this way the result may fit in the source matrix
//! [dft]
	splitAndCenter();
}

void FDF::ILPF(float d0)
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				if (dist(imat[counter0].rows/2, imat[counter0].cols/2, counter1, counter2) > d0)
					imat[counter0].at<float>(counter1,counter2) = 0;
			}
		}

	}
}


void FDF::IHPF(float d0)
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				if (dist(imat[counter0].rows / 2, imat[counter0].cols / 2, counter1, counter2) <= d0)
					imat[counter0].at<float>(counter1, counter2) = 0;
			}
		}

	}
}

void FDF::BLPF(int n, float d0)
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				float tmp = (float)1/(1 + pow(dist(imat[counter0].rows / 2, imat[counter0].cols / 2, counter1, counter2) / d0, 2 * n));
				imat[counter0].at<float>(counter1, counter2) = imat[counter0].at<float>(counter1, counter2)*tmp;
			}
		}

	}
}

void FDF::GLPF(float d0)
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				float tmp = exp(-pow(dist(imat[counter0].rows / 2, imat[counter0].cols / 2, counter1, counter2),2) / (d0*d0) );
				imat[counter0].at<float>(counter1, counter2) = imat[counter0].at<float>(counter1, counter2)*tmp;
			}
		}

	}
}

void FDF::Laplacian()
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				float tmp = -(pow(imat[counter0].rows / 2 - counter1, 2) + pow(imat[counter0].cols / 2 - counter2, 2));
				imat[counter0].at<float>(counter1, counter2) = imat[counter0].at<float>(counter1, counter2)*tmp;
			}
		}

	}
	
}

void FDF::addNoise()
{
	splitAndCenter();
	for (int counter0 = 0; counter0 < 2; counter0++)
	{
		for (int counter1 = 0; counter1 < imat[counter0].rows; counter1++)
		{
			for (int counter2 = 0; counter2 < imat[counter0].cols; counter2++)
			{
				if (dist(imat[counter0].rows / 2, imat[counter0].cols / 2, counter1, counter2) == 70)
					imat[counter0].at<float>(counter1, counter2) = 9999;
			}
		}
	}
}

void FDF::setFreImg()
{
	magnitude(imat[0], imat[1], imat[0]);// planes[0] = magnitude
	Mat magI = imat[0];
	//! [magnitude]

	//! [log]
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	normalize(magI, magI, 0, 1, NORM_MINMAX);
	this->Image_fre = magI;
}

void FDF::idft_()
{
	center(imat[0]);
	center(imat[1]);
	Mat tmp;
	merge(imat,2,tmp);
	idft(tmp, tmp);
	Mat planes[] = { Mat_<float>(Image_fre.size()), Mat::zeros(Image_fre.size(), CV_32F) };
	split(tmp, planes);
	normalize(planes[0], image_spa, 0, 1, NORM_MINMAX);
	center(imat[0]);
	center(imat[1]);
}

void FDF::splitAndCenter()
{
	split(complexI, imat);
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	center(imat[0]);
	center(imat[1]);
		//! [crop_rearrange]
}
void FDF::center(Mat& imag)
{
	imag = imag(Rect(0, 0, imag.cols & -2, imag.rows & -2));
	int cx = imag.cols / 2;
	int cy = imag.rows / 2;

	Mat q0(imag, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(imag, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(imag, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(imag, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

float dist(const float& a, const float& b, const float& c, const float& d)
{
	return sqrt(pow((a - c), 2) + pow((b - d), 2));
}

/*
*将频域的两个图像加起来。
*/
void addtwoFreImg(FDF& img1,FDF& img2)
{
	int row = img1.imat[0].rows < img2.imat[0].rows ? img1.imat[0].rows : img2.imat[0].rows;
	int col = img1.imat[0].cols < img2.imat[0].cols ? img1.imat[0].cols : img2.imat[0].cols;
	for (int counter1 = 0; counter1 < row; counter1++)
	{
		for (int counter2 = 0; counter2 < col; counter2++)
		{
			img1.imat[0].at<float>(counter1, counter2) += img2.imat[0].at<float>(counter1, counter2);
			img1.imat[1].at<float>(counter1, counter2) += img2.imat[1].at<float>(counter1, counter2);
			//cout << "666666";
		}
	}
}