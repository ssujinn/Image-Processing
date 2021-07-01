#include "SIFT.h"

string type2str(int type);
void convertToDMatch(vector<int> &matchingIdx, vector<DMatch> &matches);

int main(int argc, char *argv[]){
	Mat im1 = imread(argv[1]);
	Mat im2 = imread(argv[2]);
	cout << "Image 1 size :" << im1.size() << ", Type:" << type2str(im1.type()) << endl;
	cout << "Image 2 size :" << im2.size() << ", Type:" << type2str(im2.type()) << endl;

	//SIFT 파라미터
	int nfeatures = 0;
	int nOctaveLayers = 3;
	double contrastThreshold = 0.04;
	double edgeThreshold = 10;
	double sigma = 1.6;

	//SIFT 클래스 생성자
	SIFT *sift = new SIFT( nfeatures,  nOctaveLayers,
						contrastThreshold,  edgeThreshold, sigma);

	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptor1, descriptor2;

	sift->operator()(im1, keypoints1, descriptor1);
	printf("IMG1 Num kpt : %d\n", keypoints1.size());
	
	sift->operator()(im2, keypoints2, descriptor2);
	printf("IMG2 Num kpt : %d\n", keypoints2.size());

	vector<int> matchingIdx;
	int numMatches = MatchDescriptor(descriptor1, descriptor2, matchingIdx);
	printf("matches:%d\n", numMatches);

	vector<DMatch> matches1to2;
	convertToDMatch(matchingIdx, matches1to2);
	Mat outIm;
	drawMatches(im1, keypoints1, im2, keypoints2, matches1to2, outIm, 
		Scalar(0, 255, 0), Scalar(0, 0, 255),vector<char>(), DrawMatchesFlags::DEFAULT);
	imshow("result", outIm);
	waitKey(0);

	return 0;
}

void convertToDMatch(vector<int> &matchingIdx, vector<DMatch> &matches) {
	for (int i = 0; i < (int)matchingIdx.size(); i++) {
		int im1_idx = i;
		int im2_idx = matchingIdx[i];
		if (im2_idx != -1) {
			DMatch tmp = DMatch(im1_idx, im2_idx, 0, 0);
			matches.push_back(tmp);
		}
	}
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
