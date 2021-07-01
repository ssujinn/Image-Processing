
// swLab19fDoc.cpp: CswLab19fDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "swLab19f.h"
#endif

#include "swLab19fDoc.h"

#include <propkey.h>

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"
#include "s171640L01_Mfc.h"
#include "s171640L01_ext.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CswLab19fDoc

IMPLEMENT_DYNCREATE(CswLab19fDoc, CDocument)

BEGIN_MESSAGE_MAP(CswLab19fDoc, CDocument)

	ON_COMMAND(ID_FOPEN01, &CswLab19fDoc::On_FOpen1)
	ON_COMMAND(ID_FOPEN02, &CswLab19fDoc::On_FOpen2)
	ON_COMMAND(ID_PopUpIM01, &CswLab19fDoc::OnPopUpIM01)
	ON_COMMAND(ID_PopUpIM02, &CswLab19fDoc::OnPopUpIM02)
	ON_COMMAND(ID_PopUpIMR, &CswLab19fDoc::OnPopUpIMR)
	ON_COMMAND(ID_SAVE_IMG, &CswLab19fDoc::OnSaveImage)
END_MESSAGE_MAP()


// CswLab19fDoc 생성/소멸

CswLab19fDoc::CswLab19fDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CswLab19fDoc::~CswLab19fDoc()
{
}

BOOL CswLab19fDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CswLab19fDoc serialization

void CswLab19fDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CswLab19fDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CswLab19fDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CswLab19fDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CswLab19fDoc 진단

#ifdef _DEBUG
void CswLab19fDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CswLab19fDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CswLab19fDoc 명령


void CswLab19fDoc::On_FOpen1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlgfile(TRUE);
	// show file open dialog
	if (IDOK == dlgfile.DoModal()) {
		CString pathName = dlgfile.GetPathName();
		SWL01_inst.readImage(pathName, 1);
	}
}


void CswLab19fDoc::On_FOpen2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlgfile(TRUE);
	// show file open dialog
	if (IDOK == dlgfile.DoModal()) {
		CString pathName = dlgfile.GetPathName();
		SWL01_inst.readImage(pathName, 2);
	}
}

void CswLab19fDoc::OnPopUpIM01()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (SWL01_inst.readImageF1 == true)
		SWL01_inst.drawMatPopUp(1);
}

void CswLab19fDoc::OnPopUpIM02()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (SWL01_inst.readImageF2 == true)
		SWL01_inst.drawMatPopUp(2);
}


void CswLab19fDoc::OnPopUpIMR()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (SWL01_inst.processedF == true)
		SWL01_inst.drawMatPopUp(3);
}

void CswLab19fDoc::OnSaveImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (SWL01_inst.processedF == true) {
		CFileDialog dlgfile(FALSE); // show file open dialog for save
		if (IDOK == dlgfile.DoModal()) {
			CString pathName = dlgfile.GetPathName();
			SWL01_inst.saveImage(pathName);
		}
	}
	else {
		AfxMessageBox(L"Save one after doing somthing!", MB_OK, 0);
		return;
	}
}
