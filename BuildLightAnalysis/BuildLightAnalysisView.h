
// BuildLightAnalysisView.h : interface of the CBuildLightAnalysisView class
//

#pragma once


class CBuildLightAnalysisView : public CView
{
protected: // create from serialization only
	CBuildLightAnalysisView();
	DECLARE_DYNCREATE(CBuildLightAnalysisView)

// Attributes
public:
	CBuildLightAnalysisDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CBuildLightAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	int m_selectedOutWallPoint;
	bool m_isDrawInWall;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BuildLightAnalysisView.cpp
inline CBuildLightAnalysisDoc* CBuildLightAnalysisView::GetDocument() const
   { return reinterpret_cast<CBuildLightAnalysisDoc*>(m_pDocument); }
#endif

