#include "SIFT.h"

string type2str(int type);

int main(int argc, char *argv[]){
	Mat im1 = imread(argv[1]);
	Mat im2 = imread(argv[2]);
	cout << "Image 1 size :" << im1.size() << ", Type:" << type2str(im1.type()) << endl;
	cout << "Image 2 size :" << im2.size() << ", Type:" << type2str(im2.type()) << endl;
	
	//SIFT �Ķ����
	int nfeatures = 0;
	int nOctaveLayers = 3;
	double contrastThreshold = 0.04;
	double edgeThreshold = 10;
	double sigma = 1.6;

	//SIFT Ŭ���� ������
	SIFT *sift = new SIFT( nfeatures,  nOctaveLayers,
		 contrastThreshold,  edgeThreshold,
		 sigma);

	//Mat mask;//�Ķ���͸� ä���ִ� ����
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptor1, descriptor2;

	sift->operator()(im1, keypoints1, descriptor1);
	printf("IMG1 Num kpt : %d\n", keypoints1.size());
	
	sift->operator()(im2, keypoints2, descriptor2);
	printf("IMG2 Num kpt : %d\n", keypoints2.size());

	vector<int> machingIdx;
	int numMatches = MatchDescriptor(descriptor1, descriptor2, machingIdx);
	printf("matches:%d\n", numMatches);

	Mat im_matches = DrawMatches(im1,im2, keypoints1, keypoints2, machingIdx);
	imwrite("matches.jpg", im_matches);
	imshow("match_result", im_matches);
	waitKey(0);
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

