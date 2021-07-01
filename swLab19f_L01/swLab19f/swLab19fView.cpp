
// swLab19fView.cpp: CswLab19fView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "swLab19f.h"
#endif

#include "swLab19fDoc.h"
#include "swLab19fView.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"
#include "s171640L01_Mfc.h"
#include "s171640L01_ext.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CswLab19fView

IMPLEMENT_DYNCREATE(CswLab19fView, CView)

BEGIN_MESSAGE_MAP(CswLab19fView, CView)
	 ON_WM_SIZE()	//추가
END_MESSAGE_MAP()

// CswLab19fView 생성/소멸

CswLab19fView::CswLab19fView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CswLab19fView::~CswLab19fView()
{
}

BOOL CswLab19fView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	g_pView = this;
	return CView::PreCreateWindow(cs);
}

// CswLab19fView 그리기

void CswLab19fView::OnDraw(CDC* pDC)
{
	CswLab19fDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (SWL01_inst.readImageF1 == true) {
		SWL01_inst.drawImage(pDC, 20, 20, 1);
	}
	if (SWL01_inst.readImageF2 == true &&
		SWL01_inst.processedF == false) {
		SWL01_inst.drawImage(pDC, 20, 20, 2);
	}
	if (SWL01_inst.processedF == true) {
		SWL01_inst.drawImage(pDC, 20, 20, 3);
	}
}



// CswLab19fView 진단

#ifdef _DEBUG
void CswLab19fView::AssertValid() const
{
	CView::AssertValid();
}

void CswLab19fView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CswLab19fDoc* CswLab19fView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CswLab19fDoc)));
	return (CswLab19fDoc*)m_pDocument;
}
#endif //_DEBUG


// CswLab19fView 메시지 처리기
