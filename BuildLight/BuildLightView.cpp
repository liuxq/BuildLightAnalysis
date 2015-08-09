
// BuildLightView.cpp : implementation of the CBuildLightView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BuildLight.h"
#endif

#include "BuildLightDoc.h"
#include "BuildLightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBuildLightView

IMPLEMENT_DYNCREATE(CBuildLightView, CView)

BEGIN_MESSAGE_MAP(CBuildLightView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBuildLightView construction/destruction

CBuildLightView::CBuildLightView()
{
	// TODO: add construction code here

}

CBuildLightView::~CBuildLightView()
{
}

BOOL CBuildLightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBuildLightView drawing

void CBuildLightView::OnDraw(CDC* /*pDC*/)
{
	CBuildLightDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CBuildLightView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBuildLightView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBuildLightView diagnostics

#ifdef _DEBUG
void CBuildLightView::AssertValid() const
{
	CView::AssertValid();
}

void CBuildLightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBuildLightDoc* CBuildLightView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBuildLightDoc)));
	return (CBuildLightDoc*)m_pDocument;
}
#endif //_DEBUG


// CBuildLightView message handlers
