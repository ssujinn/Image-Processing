#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//실습에서 작성한 코드 사용
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
	///////////////////////////////////////////////////////////////
	// 이미지1의 디스크립터들과 이미지2의 디스크립터들 중 매칭되는 페어의 개수를 계산하는 함수
	// 이미지1의 1개의 디스크립터와 이미지2의 모든 디스크립터 사이의 L2 거리를 계산 
	// 가장 가까운 거리 (d1) 와 두번째로 가까운 거리(d2) 의 비율 (d1/d2) 을 구해 설정된 임계값 (DIST_RATIO_THR) 보다 작다면 매칭으로 판단
	// machingIdx 에는 매칭이 된 인덱스를 저장 (machingIdx[idx1] = idx2, idx1은 이미지1의 디스크립터의 인덱스이고, idx2는 매칭된 이미지2의 디스크립터의 인덱스)
	// 함수의 리턴값으로는 매칭된 페어의 개수를 출력
	//////////////////////////////////////////////////////////////
	int numMatches = 0;

	//내용 구현
	int kpoint1, kpoint2;
	float *ds1 = (float*)descriptor1.data;
	float *ds2 = (float*)descriptor2.data;
	float tmp, sum = 0, dist;
	float d1 = 0, d2 = 0;
	int i1 = -1, i2 = -1;

	kpoint1 = descriptor1.rows;
	kpoint2 = descriptor2.rows;

	matchingIdx.resize(kpoint1);

	int i = 0, j = 0;

	for (i = 0; i < kpoint1; i++) {
		d1 = 0;
		d2 = 0;
		i1 = -1;
		i2 = -1;
		for (j = 0; j < kpoint2; j++) {
			sum = 0;
			for (int k = 0; k < 128; k++) {
				tmp = ds1[i * 128 + k] - ds2[j * 128 + k];
				sum += pow(tmp, 2);
			}
			dist = sqrt(sum);

			if (j == 0) {
				d1 = dist;
				i1 = j;
			}
			else if (d2 == 0 && dist > d1) {
				d2 = dist;
				i2 = j;
			}
			else if (dist > d1 && dist < d2 && d2 != 0) {
				d2 = dist;
				i2 = j;
			}
			else if (dist < d1) {
				swap(d1, d2);
				swap(i1, i2);
				d1 = dist;
				i1 = j;
			}
		}

		if (i2 >= 0 && (d1 / d2) < DIST_RATIO_THR) {
			matchingIdx[i] = i1;
			numMatches++;
		}
		else
			matchingIdx[i] = -1;
	}
	
	return numMatches;
}


Mat stack_imgs(const Mat &im1, const Mat &im2) {
	//두개의 이미지를 하나의 이미지로 합치는 함수
	//두개의 이미지 크기가 다를 수 있기 때문에 opencv의 hconcat, hstack을 사용하지 않고, copyTo 함수를 사용한다.
	//두개의 이미지가 들어갈수있는 큰 하나의 Mat stacked을 만들고 (Mat type은 이미지1, 2와 같게 한다),
	//copyTo 함수를 사용해 이미지1과 이미지2를 stacked에 복사한다.

	Mat stacked;

	//img1, img2를 stacked에 복사
	//내용 구현

	int x1 = im1.rows;
	int x2 = im2.rows;
	int y1 = im1.cols;
	int y2 = im2.cols;
	int x;

	if (x1 < x2)
		x = x2;
	else
		x = x1;

	stacked = Mat::zeros(x, y1 + y2, im1.type());
	im1.copyTo(stacked(Rect(0, 0, y1, x1)));
	im2.copyTo(stacked(Rect(y1, 0, y2, x2)));

	return stacked;
}


Mat DrawMatches(const Mat &im1, const Mat &im2, vector<KeyPoint> &keypoints1, vector<KeyPoint> &keypoints2, vector<int> &matchingIdx) {
	//매칭페어의 정보를 받아서 매칭된 키포인트의 위치와 매칭 페어 사이의 연결 선을 그리는 함수
	//stack_imgs() 함수를 사용해 2개의 이미지를 연결해 하나의 이미지로 만듦
	//키포인트의 위치는 opencv의 circle() 함수 사용, 연결선은 opencv의 line() 함수 사용

	Mat result_im;
	result_im = stack_imgs(im1, im2);//

	//Draw keypoint location and line between matching pair
	//내용 구현


	for (int i = 0; i < matchingIdx.size(); i++) {
		if (matchingIdx[i] != -1) {
			Point point1 = keypoints1[i].pt;
			Point point2 = Point2f(keypoints2[matchingIdx[i]].pt.x + im1.cols, keypoints2[matchingIdx[i]].pt.y);
			circle(result_im, point1, 1, Scalar(0, 255, 0));
			circle(result_im, point2, 1, Scalar(0, 255, 0));
			line(result_im, point1, point2, Scalar(0, 255, 0));
		}
	}

	return result_im;
}