// https://github.com/opencv/opencv/blob/2.4/modules/nonfree/include/opencv2/nonfree/features2d.hpp
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include <iostream>
#include <stdarg.h>
#include <math.h>  //for function 'exp'. �߰�.

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;
/*!
	SIFT implementation.
	class implements SIFT algorithm by D. Lowe.
*/
class CV_EXPORTS_W SIFT : public Feature2D
{
	public:
		CV_WRAP explicit SIFT(int nfeatures = 0, int nOctaveLayers = 3,
			double contrastThreshold = 0.04, double edgeThreshold = 10,
			double sigma = 1.6);

		//! returns the descriptor size in floats (128)
		CV_WRAP int descriptorSize() const;

		//! returns the descriptor type
		CV_WRAP int descriptorType() const;

		//! finds the keypoints using SIFT algorithm
		void operator()(InputArray img, InputArray mask,
			vector<KeyPoint>& keypoints) const;
		void operator()(InputArray img,// InputArray mask,
			vector<KeyPoint>& keypoints) const;
		//! finds the keypoints and computes descriptors for them using SIFT algorithm.
		//! Optionally it can compute descriptors for the user-provided keypoints
		void operator()(InputArray img, InputArray mask,
			vector<KeyPoint>& keypoints,
			OutputArray descriptors,
			bool useProvidedKeypoints = false) const;
		void operator()(InputArray img, //InputArray mask,
			vector<KeyPoint>& keypoints,
			OutputArray descriptors,
			bool useProvidedKeypoints = false) const;

		void buildGaussianPyramid(const Mat& base, vector<Mat>& pyr, int nOctaves) const;
		void buildDoGPyramid(const vector<Mat>& pyr, vector<Mat>& dogpyr) const;
		void findScaleSpaceExtrema(const vector<Mat>& gauss_pyr, const vector<Mat>& dog_pyr,
			vector<KeyPoint>& keypoints) const;

	protected:
		void detectImpl(const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask = Mat()) const;
		void detectImpl(const Mat& image, vector<KeyPoint>& keypoints) const;
		void computeImpl(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) const;

		CV_PROP_RW int nfeatures;
		CV_PROP_RW int nOctaveLayers;
		CV_PROP_RW double contrastThreshold;
		CV_PROP_RW double edgeThreshold;
		CV_PROP_RW double sigma;
};

typedef SIFT SiftFeatureDetector;
typedef SIFT SiftDescriptorExtractor;

//�ǽ����� ������ �Լ�
void customGaussianBlur(const Mat &src, Mat &dst, double sigma);
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx);