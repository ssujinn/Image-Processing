#include "SIFT.h"
#define DIST_RATIO_THR 0.49

void customGaussianBlur(const Mat &src, Mat &dst, double sigma){
	////////////////////////////////////////////////////////////
	// ����þ� Ŀ���� ������ �̹����� �����ϴ� �Լ�
	// Ŀ���� ũ��� 2*(4*sigma)+1 (�Ҽ����� �ø� �Ǵ� �ݿø����� ó���Ѵ�)
	// ����þ� Ŀ���� float���� ���� ����
	// ����þ� Ŀ���� Ŀ�� �� ��� �� ��ü�� �������� ������ 0~1������ ���� �������� ����ȭ
	// ����þ� Ŀ���� �̹����� �����Ҷ��� filter2D() �Լ��� ���
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size;
	Mat gau_kernel;
	//���� ����
	kernel_size = 2 * (int)ceil(4 * sigma) + 1;

	gau_kernel.release();
	gau_kernel = Mat(kernel_size, kernel_size, CV_32FC1);

	// Compute gaussian kernel value
	//���� ����
	double sum = 0.0;
	double tmp;
	int i, j;
	float *kernel = (float*)gau_kernel.data;
	int mid = kernel_size / 2;

	for (i = 0; i < kernel_size; i++) {
		for (j = 0; j < kernel_size; j++) {
			tmp = exp(-0.5 * (pow((i - mid) / sigma, 2.0) + pow((j - mid) / sigma, 2.0))) / (2 * CV_PI * sigma * sigma);
			kernel[i * kernel_size + j] = (float)tmp;
			sum += tmp;
		}
	}

	// Normalize the kernel
	//���� ����
	for (i = 0; i < kernel_size * kernel_size; i++) {
		tmp = (double)kernel[i];
		tmp /= sum;
		kernel[i] = (float)tmp;
	}

	// Apply Gaussian kernel
	//���� ����
	filter2D(src, dst, -1, gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
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
	
	machingIdx.resize(kpoint1);

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
			machingIdx[i] = i1;
			numMatches++;
		}
		else
			machingIdx[i] = -1;
	}

	return numMatches;
}