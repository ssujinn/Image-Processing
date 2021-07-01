
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "swLab19f.h"

#include "MainFrm.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MsgView.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"
#include "s171640L01_Mfc.h"
#include "s171640L01_ext.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE() // 추가
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	g_pMainF = this;
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	BOOL flag = m_wndSplitter.CreateStatic(this, 2, 1);		// *** 창을 2 열로 나누고
	if (flag)
	{	// *** 각 창별 뷰를 선언한다
		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CswLab19fView), CSize(100, 100), pContext);
		m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CRichEditView), CSize(100, 100), pContext);
		initMessage((CRichEditView*)m_wndSplitter.GetPane(1, 0));
		m_bSplitter = true;
	}
	return flag;
	/*
	return m_wndSplitter.Create(this,
		2, 2,               // TODO: 행 및 열의 개수를 조정합니다.
		CSize(10, 10),      // TODO: 최소 창 크기를 조정합니다.
		pContext);
	*/
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
void CMainFrame::OnSize(UINT nType, int cx, int cy) {
	CFrameWnd::OnSize(nType, cx, cy);
	if (m_bSplitter) {
		m_wndSplitter.SetRowInfo(0, int(cy * 0.8), 10);
		m_wndSplitter.RecalcLayout();
	}
}

