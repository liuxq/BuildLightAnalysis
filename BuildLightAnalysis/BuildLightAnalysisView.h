
// BuildLightAnalysisView.h : interface of the CBuildLightAnalysisView class
//

#pragma once
#include "Transform.h"

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
	void optimize();
	Transform& GetTransform()
	{
		return m_transform;
	}
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
	int m_iSelectOutWallIndex;
	int m_iSelectInWallIndex;
	int m_iSelectWindowIndex;
	bool m_bIsPullTranslate;
	Transform m_transform;
	CPoint prePoint;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT, short, CPoint);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditOptimize();
	afx_msg void OnPopAddWindow();
	afx_msg void OnPopAddtoRoom();
	afx_msg void OnEditTranslate();
};

#ifndef _DEBUG  // debug version in BuildLightAnalysisView.cpp
inline CBuildLightAnalysisDoc* CBuildLightAnalysisView::GetDocument() const
   { return reinterpret_cast<CBuildLightAnalysisDoc*>(m_pDocument); }
#endif

