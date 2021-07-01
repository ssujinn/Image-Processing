#include "SIFT.h"
#define DIST_RATIO_THR 0.49

void customGaussianBlur(const Mat &src, Mat &dst, double sigma){
	////////////////////////////////////////////////////////////
	// 가우시안 커널을 생성해 이미지에 적용하는 함수
	// 커널의 크기는 2*(4*sigma)+1 (소수점은 올림 또는 반올림으로 처리한다)
	// 가우시안 커널은 float형의 값을 가짐
	// 가우시안 커널은 커널 값 계산 후 전체의 총합으로 나눠서 0~1사이의 값을 가지도록 정규화
	// 가우시안 커널을 이미지에 적용할때는 filter2D() 함수를 사용
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size;
	Mat gau_kernel;
	//내용 구현
	kernel_size = 2 * (int)ceil(4 * sigma) + 1;

	gau_kernel.release();
	gau_kernel = Mat(kernel_size, kernel_size, CV_32FC1);

	// Compute gaussian kernel value
	//내용 구현
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
	//내용 구현
	for (i = 0; i < kernel_size * kernel_size; i++) {
		tmp = (double)kernel[i];
		tmp /= sum;
		kernel[i] = (float)tmp;
	}

	// Apply Gaussian kernel
	//내용 구현
	filter2D(src, dst, -1, gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
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