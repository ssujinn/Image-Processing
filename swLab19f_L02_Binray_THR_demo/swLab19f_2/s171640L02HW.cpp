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

void movingAvrg(char* kernel, void *userData)
{
	string &win_name = *(static_cast<string*>(userData));
	int ksize = atoi(kernel);

	start_time = clock();

	if (ksize >= 0 && ksize < 3)
		ksize = 1;
	else
		if (!(ksize % 2))
			ksize--;

	int x = input_im.rows;
	int y = input_im.cols;
	int i, j, m, n;
	double sum = 0, tmp, prevsum;

	dst.release();
	dst = Mat(x, y, CV_8UC1);

	unsigned char *pDibData = input_im.data;
	unsigned char *pMatData = dst.data;

	// average blur
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			if (j == 0 && i == 0) {
				for (m = -(ksize / 2); m <= (ksize / 2); m++) {
					for (n = -(ksize / 2); n <= (ksize / 2); n++) {
						if (((i + m) >= 0) && ((i + m) < x) && ((j + n) >= 0) && ((j + n)) < y) {
							tmp = (double)pDibData[(i + m)*y + (j + n)];
							sum += tmp;
						}
					}
				}
				prevsum = sum;
			}
			else if (j == 0) {
				sum = prevsum;
				n = i - (ksize / 2) - 1;
				for (m = 0; m <= ksize / 2; m++) {
					if (n >= 0 && n < x && m >= 0 && m < y) {
						tmp = (double)pDibData[n * y + m];
						sum -= tmp;
					}
				}
				n = i + (ksize / 2);
				for (m = 0; m <= ksize / 2; m++) {
					if (n >= 0 && n < x && m >= 0 && m < y) {
						tmp = (double)pDibData[n * y + m];
						sum += tmp;
					}
				}
				prevsum = sum;
			}
			else {
				n = j - (ksize / 2) - 1;
				for (m = -(ksize / 2); m <= ksize / 2; m++) {
					if ((i + m) >= 0 && (i + m) < x && n < y && n >= 0) {
						tmp = (double)pDibData[(i + m) * y + n];
						sum -= tmp;
					}
				}
				n = j + (ksize / 2);
				for (m = -(ksize / 2); m <= ksize / 2; m++) {
					if ((i + m) >= 0 && (i + m) < x && n < y && n >= 0) {
						tmp = (double)pDibData[(i + m) * y + n];
						sum += tmp;
					}
				}
			}
			tmp = sum / (ksize * ksize);
			pMatData[i * y + j] = (unsigned char)tmp;
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
		cout << "명령문 : snnnnnnHW01.exe imageFile kernalSize<ent>" << endl;
		return 0;
	}
	//파일 주소는 argv[1]로 들어옴
	input_im = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	cout << "Filename = " << argv[1];
	if (input_im.empty())
	{
		cout << "File open Error!" << endl;
		return -1;
	}
	cout << "Image size :" << input_im.size() << ", Type:" << type2str(input_im.type()) << endl;

	string window_name = "movingAvrg HW";
	namedWindow(window_name);

	movingAvrg(argv[2], &window_name);

	waitKey(0);

	return 0;
}