#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//�ǽ����� �ۼ��� �ڵ� ���
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
	///////////////////////////////////////////////////////////////
	// �̹���1�� ��ũ���͵�� �̹���2�� ��ũ���͵� �� ��Ī�Ǵ� ����� ������ ����ϴ� �Լ�
	// �̹���1�� 1���� ��ũ���Ϳ� �̹���2�� ��� ��ũ���� ������ L2 �Ÿ��� ��� 
	// ���� ����� �Ÿ� (d1) �� �ι�°�� ����� �Ÿ�(d2) �� ���� (d1/d2) �� ���� ������ �Ӱ谪 (DIST_RATIO_THR) ���� �۴ٸ� ��Ī���� �Ǵ�
	// machingIdx ���� ��Ī�� �� �ε����� ���� (machingIdx[idx1] = idx2, idx1�� �̹���1�� ��ũ������ �ε����̰�, idx2�� ��Ī�� �̹���2�� ��ũ������ �ε���)
	// �Լ��� ���ϰ����δ� ��Ī�� ����� ������ ���
	//////////////////////////////////////////////////////////////
	int numMatches = 0;

	//���� ����
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
	//�ΰ��� �̹����� �ϳ��� �̹����� ��ġ�� �Լ�
	//�ΰ��� �̹��� ũ�Ⱑ �ٸ� �� �ֱ� ������ opencv�� hconcat, hstack�� ������� �ʰ�, copyTo �Լ��� ����Ѵ�.
	//�ΰ��� �̹����� �����ִ� ū �ϳ��� Mat stacked�� ����� (Mat type�� �̹���1, 2�� ���� �Ѵ�),
	//copyTo �Լ��� ����� �̹���1�� �̹���2�� stacked�� �����Ѵ�.

	Mat stacked;

	//img1, img2�� stacked�� ����
	//���� ����

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
	//��Ī����� ������ �޾Ƽ� ��Ī�� Ű����Ʈ�� ��ġ�� ��Ī ��� ������ ���� ���� �׸��� �Լ�
	//stack_imgs() �Լ��� ����� 2���� �̹����� ������ �ϳ��� �̹����� ����
	//Ű����Ʈ�� ��ġ�� opencv�� circle() �Լ� ���, ���ἱ�� opencv�� line() �Լ� ���

	Mat result_im;
	result_im = stack_imgs(im1, im2);//

	//Draw keypoint location and line between matching pair
	//���� ����


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