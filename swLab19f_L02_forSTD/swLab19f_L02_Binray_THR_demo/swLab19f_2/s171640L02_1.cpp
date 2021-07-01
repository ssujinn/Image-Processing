#pragma once
#include <iostream>
#include <time.h>// 시간 측정 위해서
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

Mat input_im, dst;
clock_t start_time, end_time;
string type2str(int type);

using namespace std;

void Average_opencvCommand(int ksize, void *userData)
{	
	string &win_name = *(static_cast<string*>(userData));

	start_time = clock();

	if (ksize < 3)
		ksize = 1;
	else
		if (!(ksize % 2))
			ksize--;

	blur(input_im, dst, Size(ksize, ksize));

	end_time = clock();
	
	cout << "Kernel Size : "<< ksize <<", Exec Time : " << (double)(end_time - start_time) << " (msec)" << endl;

	imshow(win_name, dst);
}

void Average_userCommand(int ksize, void *userData)
{
	string &win_name = *(static_cast<string*>(userData));

	start_time = clock();

	if (ksize < 3)
		ksize = 1;
	else
		if (!(ksize % 2))
			ksize--;

	int x = input_im.rows;
	int y = input_im.cols;

	dst.release();
	dst = Mat(x, y, CV_8UC1);

	unsigned char *pDibData = input_im.data;
	unsigned char *pMatData = dst.data;

	int r, c, m, n;
	double tmp, sum;

	for (r = 0; r < x; r++) {
		for (c = 0; c < y; c++) {
			sum = 0;
			if (ksize != 1) {
				for (m = -(ksize / 2); m <= (ksize / 2); m++) {
					for (n = -(ksize / 2); n <= (ksize / 2); n++) {
						if (((r + n) >= 0) && ((r + n) < x) && ((c + m) >= 0) && ((c + m)) < y){
							tmp = (double)pDibData[(r + n) * y + (c + m)];
							tmp /= (double)(ksize * ksize);
							sum += tmp;
						}
					}
				}
				pMatData[r * y + c] = (unsigned char)sum;
			}
			else {
				pMatData[r * y + c] = pDibData[r * y + c];
			}

		}
	}


	end_time = clock();
	cout << "Kernel Size : " << ksize << ", Exec Time : " << (double)(end_time - start_time) << " (msec)" << endl;

	imshow(win_name, dst);
}


string type2str(int type) {
	// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "[프로그램 사용법]" << endl;
		cout << "명령문 : snnnnnnHW01.exe lena.bmp flag<ent>" << endl;
		cout << "flag : 1 - opencv function 사용" << endl;
		cout << "flag : 2 - loop로 brute force 사용" << endl;
		return 0;
	}
	//파일 주소는 argv[1]로 들어옴
	input_im = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (input_im.empty())
	{
		cout << "File open Error!" << endl;
		return -1;
	}
	cout << "Image size :" << input_im.size() << ", Type:" << type2str(input_im.type()) << endl;

	string window_name = "Average Filtering Window";
	namedWindow(window_name);

	//Create track bar to change kernel size
	int start_value = 3;
	int max_value = 15;

	if (!strcmp(argv[2], "1")) {
		createTrackbar("KernelSize", window_name, &start_value, max_value, Average_opencvCommand, static_cast<void*>(&window_name));
		cout << "OpenCV Function" << endl;
	}
	else if (!strcmp(argv[2], "2")) {
		createTrackbar("KernelSize", window_name, &start_value, max_value, Average_userCommand, static_cast<void*>(&window_name));
		cout << "Brute force" << endl;
	}
	else
		cout << "Wrong flag!!" << endl;

	imshow(window_name, input_im);
	waitKey(0);
	
	return 0;
}