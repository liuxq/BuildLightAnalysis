
// BuildLightAnalysisView.cpp : implementation of the CBuildLightAnalysisView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BuildLightAnalysis.h"
#endif

#include "BuildLightAnalysisDoc.h"
#include "BuildLightAnalysisView.h"
#include "MainFrm.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBuildLightAnalysisView

IMPLEMENT_DYNCREATE(CBuildLightAnalysisView, CView)

BEGIN_MESSAGE_MAP(CBuildLightAnalysisView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CBuildLightAnalysisView construction/destruction

CBuildLightAnalysisView::CBuildLightAnalysisView():m_selectedOutWallPoint(-1)
{
	// TODO: add construction code here

}

CBuildLightAnalysisView::~CBuildLightAnalysisView()
{
}

BOOL CBuildLightAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBuildLightAnalysisView drawing

void CBuildLightAnalysisView::OnDraw(CDC* /*pDC*/)
{
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
	if (!outWallPos)
		return;

	CDC *pDC=GetDC();        //CDC方式创建

	CPen pen(PS_SOLID,8,RGB(100,100,100));    //定义画笔
	
	CPen *pOldPen=pDC->SelectObject(&pen);

	CPoint p, startP;
	for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
	{
		p.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().intVal;
		p.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().intVal;
		if (i == 0)
		{
			startP = p;
			pDC->MoveTo(p); 
		}
		else if (i == outWallPos->GetSubItemsCount() -1)//连成环
		{
			pDC->LineTo(p);
			pDC->LineTo(startP);
		}
		else
		{
			pDC->LineTo(p);
		}
	}

	ReleaseDC(pDC);
}

void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CString xy;
	xy.Format(_T("%d,%d"),point.x, point.y);
	pMain->GetStatusBar().SetPaneText(0,xy);

	//如果有选中的外墙点，则移动它
	if (m_selectedOutWallPoint >= 0)
	{
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(0)->SetValue(point.x);
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(1)->SetValue(point.y);

		Invalidate();
	}

}
void CBuildLightAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
	if (!outWallPos)
		return;

	for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
	{
		int x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().intVal;
		int y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().intVal;

		if (sqrt((double)(x - point.x)*(x - point.x) + (y - point.y)*(y - point.y)) < 10)
		{
			m_selectedOutWallPoint = i;
			break;
		}
	}
}
void CBuildLightAnalysisView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//如果有选中的外墙点，则移动它
	if (m_selectedOutWallPoint >= 0)
	{
		m_selectedOutWallPoint = -1;
	}
}

void CBuildLightAnalysisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBuildLightAnalysisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBuildLightAnalysisView diagnostics

#ifdef _DEBUG
void CBuildLightAnalysisView::AssertValid() const
{
	CView::AssertValid();
}

void CBuildLightAnalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBuildLightAnalysisDoc* CBuildLightAnalysisView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBuildLightAnalysisDoc)));
	return (CBuildLightAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CBuildLightAnalysisView message handlers
