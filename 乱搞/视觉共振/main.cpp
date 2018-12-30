#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "FrequencDomainfilter.h"
#include <iostream>

using namespace cv;
using namespace std;

static void help(void)
{
	cout << endl
		<< "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
		<< "The dft of an image is taken and it's power spectrum is displayed." << endl
		<< "Usage:" << endl
		<< "./discrete_fourier_transform [image_name -- default ../data/lena.jpg]" << endl;
}

int main()
{
	help();

	FDF fdf("you.jpg");

	imshow("Input Image", fdf.getImageSpa());    // Show the result
	fdf.dft_();

	//imshow("spectrum magnitude", fdf.getImageFre());
	

	fdf.ILPF(25);
	fdf.idft_();
	imshow("ILPF", fdf.getImageSpa());
	//imshow("ILPF_f", fdf.getImageFre());

	FDF fdf2("me.jpg");

	imshow("Input Image", fdf2.getImageSpa());    // Show the result
	fdf2.dft_();
	fdf2.IHPF(25);
	fdf2.idft_();
	imshow("IHPF", fdf2.getImageSpa());
	addtwoFreImg(fdf,fdf2);
	fdf.idft_();
	imshow("hahhaha", fdf.getImageSpa());
	//imshow("IHPF_f", fdf.getImageFre());

	/*fdf.BLPF(2,50);
	fdf.idft_();
	imshow("BLPF", fdf.getImageSpa());
	imshow("BLPF_f", fdf.getImageFre());

	fdf.GLPF(50);
	fdf.idft_();
	imshow("GLPF", fdf.getImageSpa());
	imshow("GLPF_f", fdf.getImageFre());

	fdf.Laplacian();
	fdf.idft_();
	imshow("Laplacian", fdf.getImageSpa());
	imshow("Laplacian_f", fdf.getImageFre());

	fdf.addNoise();
	fdf.idft_();
	imshow("addNoise", fdf.getImageSpa());
	imshow("addNoise_f", fdf.getImageFre());*/

	waitKey();
	system("pause");
	return 0;
}
