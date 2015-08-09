
// BuildLightView.h : interface of the CBuildLightView class
//

#pragma once


class CBuildLightView : public CView
{
protected: // create from serialization only
	CBuildLightView();
	DECLARE_DYNCREATE(CBuildLightView)

// Attributes
public:
	CBuildLightDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CBuildLightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BuildLightView.cpp
inline CBuildLightDoc* CBuildLightView::GetDocument() const
   { return reinterpret_cast<CBuildLightDoc*>(m_pDocument); }
#endif

