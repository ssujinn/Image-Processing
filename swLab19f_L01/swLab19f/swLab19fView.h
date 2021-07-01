
// swLab19fView.h: CswLab19fView 클래스의 인터페이스
//

#pragma once


class CswLab19fView : public CView
{
protected: // serialization에서만 만들어집니다.
	CswLab19fView() noexcept;
	DECLARE_DYNCREATE(CswLab19fView)

// 특성입니다.
public:
	CswLab19fDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CswLab19fView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // swLab19fView.cpp의 디버그 버전
inline CswLab19fDoc* CswLab19fView::GetDocument() const
   { return reinterpret_cast<CswLab19fDoc*>(m_pDocument); }
#endif

