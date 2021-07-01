#include "stdafx.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"

#include "s171640L01_Mfc.h"
#include "CDIB.h"
#include "s171640L01_ext.h"

void SWL01::DarkenGrayscaleImage(void) {
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	Mat *pMat;
	pMat = &m_Mat1;
	string s1 = "8UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
		return;
	}

	m_MatR.release();
	m_MatR = Mat(m_height1, m_width1, CV_8UC1);

	/*
	for (int r = 0; r < m_height1; r++)
		for (int c = 0; c < m_width1; c++)
				m_MatR.at<uchar>(r, c) = m_Mat1.at<uchar>(r, c)/2;
	*/

	unsigned char *pMatData = m_MatR.data;
	unsigned char *pDibData = m_Mat1.data;

	for (int p = 0; p < m_height1 * m_width1; p++)
		*pMatData++ = *pDibData++/2;

	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다
}

void SWL01::BrightenGrayscaleImage(void) {
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	Mat *pMat;
	pMat = &m_Mat1;
	string s1 = "8UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
		return;
	}

	m_MatR.release();
	m_MatR = Mat(m_height1, m_width1, CV_8UC1);

	unsigned char *pMatData = m_MatR.data;
	unsigned char *pDibData = m_Mat1.data;

	for (int p = 0; p < m_height1 * m_width1; p++) {
		int tmp = *pDibData;
		tmp *= 2;
		if (tmp > 255)
			tmp = 255;
		*pMatData = tmp;
	
		pMatData++;
		pDibData++;
	}

	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다
}

void SWL01::Color24toGrayscale(void) {
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	Mat *pMat;
	pMat = &m_Mat1;
	string s1 = "8UC3";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 24bit color can be shown!", MB_OK, 0);
		return;
	}

	m_MatR.release();
	m_MatR = Mat(m_height1, m_width1, CV_8UC1);

	unsigned char *pMatData = m_MatR.data;
	unsigned char *pDibData = m_Mat1.data;

	for (int p = 0; p < m_height1 * m_width1; p++) {
		double tmp;
		tmp = 0.114*(double)(*(pDibData + 2)) + 0.587*(double)(*(pDibData + 1)) + 0.299*(double)(*(pDibData));
		*pMatData = (unsigned char)tmp;
		pMatData++;
		pDibData += 3;
	}

	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다
}

void SWL01::Color16toGrayscale(void) {
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	Mat *pMat;
	pMat = &m_Mat1;
	string s1 = "16UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 16bit color can be shown!", MB_OK, 0);
		return;
	}
	
	m_MatR.release();
	m_MatR = Mat(m_height1, m_width1, CV_8UC1);

	unsigned char *pMatData = m_MatR.data;
	unsigned char *pDibData = m_Mat1.data;

	for (int p = 0; p < m_height1 * m_width1; p++) {
		unsigned short tmp = 0;
		double sum = 0;

		tmp = ((unsigned short)(*(pDibData + 1)) << 8) + (unsigned short)(*pDibData);
		sum = (double)(tmp & 31) * 0.114 + (double)((tmp >> 5) & 31) * 0.587 + (double)((tmp >> 10) & 31) * 0.299;
		sum = sum / 31 * 255;

		*pMatData++ = (unsigned char)sum;
		pDibData += 2;
	}

	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다
}