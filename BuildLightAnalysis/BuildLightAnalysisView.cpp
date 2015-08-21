
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
#include "OptimizeLine.h"

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
	ON_COMMAND(ID_EDIT_OPTIMIZE, &CBuildLightAnalysisView::OnEditOptimize)
END_MESSAGE_MAP()

// CBuildLightAnalysisView construction/destruction

CBuildLightAnalysisView::CBuildLightAnalysisView():m_selectedOutWallPoint(-1),m_isDrawInWall(false)
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

//进行处理
void CBuildLightAnalysisView::optimize()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
	CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
	if (!outWallPos || !inWallPos)
		return;

	vector<sLine> sLines, outLines;
	Vec2d ps, pe, pFirst;
	//外墙
	for (int i = 0; i < outWallPos->GetSubItemsCount()-1; i++)
	{
		ps.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
		ps.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;

		pe.x = outWallPos->GetSubItem(i+1)->GetSubItem(0)->GetValue().dblVal;
		pe.y = outWallPos->GetSubItem(i+1)->GetSubItem(1)->GetValue().dblVal;

		if (i == 0)
		{
			pFirst = ps;
		}
		sLines.push_back(sLine(ps,pe));
	}
	if (outWallPos->GetSubItemsCount() > 2)
		sLines.push_back(sLine(pe,pFirst));

	//内墙
	for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
	{
		ps.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
		ps.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

		pe.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
		pe.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

		sLines.push_back(sLine(ps,pe));
	}
	double wTh = pMain->GetOptionProperty().getProperty(1);

	OptimizeLine(sLines,outLines, wTh);

	//设置处理后的墙
	pMain->GetOptimizeWallProperty().DeletePos();
	for (int i = 0; i < outLines.size(); i++)
	{
		pMain->GetOptimizeWallProperty().InsertPos(outLines[i].s.x,outLines[i].s.y,outLines[i].e.x,outLines[i].e.y);
	}
	

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
	CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
	if (!outWallPos || !inWallPos)
		return;

	if (!pMain->GetOptimizeWallProperty().IsPaneVisible())
	{
		CDC *pDC=GetDC();        //CDC方式创建

		CPen pen(PS_SOLID,8,RGB(100,100,100));    //定义画笔

		CPen *pOldPen=pDC->SelectObject(&pen);

		CPoint p, startP;
		for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
		{
			p.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
			p.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;
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

		//画内墙
		CPen pen1(PS_SOLID,4,RGB(100,100,100));    //定义画笔
		pOldPen=pDC->SelectObject(&pen1);
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			p.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			pDC->MoveTo(p);

			p.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			pDC->LineTo(p);

		}

		ReleaseDC(pDC);
	}
	else
	{
		//画处理后的墙
		CMFCPropertyGridProperty* optimizeWallPos = pMain->GetOptimizeWallProperty().getCoodGroup();
		if (!optimizeWallPos)
			return;

		CDC *pDC=GetDC();						  //CDC方式创建
		CPen pen(PS_SOLID,6,RGB(100,100,100));    //定义画笔
		CPen* pOldPen=pDC->SelectObject(&pen);
		CPoint p;
		for (int i = 0; i < optimizeWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			pDC->MoveTo(p);

			p.x = optimizeWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			pDC->LineTo(p);
		}
	}
	
	
}

void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CString xy;
	xy.Format(_T("%d,%d"),point.x, point.y);
	pMain->GetStatusBar().SetPaneText(0,xy);

	//如果是外墙模式，有选中的外墙点，则移动它
	if (pMain->GetOutWallProperty().IsPaneVisible() && m_selectedOutWallPoint >= 0)
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(0)->SetValue((double)point.x);
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(1)->SetValue((double)point.y);

		Invalidate();
	}
	//如果是内墙模式
	if (m_isDrawInWall && pMain->GetInWallProperty().IsPaneVisible())
	{
		CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
		if (!inWallPos)
			return;

		int count = inWallPos->GetSubItemsCount();
		if (!count)
			return;
		inWallPos->GetSubItem(count-1)->GetSubItem(1)->GetSubItem(0)->SetValue((double)point.x);
		inWallPos->GetSubItem(count-1)->GetSubItem(1)->GetSubItem(1)->SetValue((double)point.y);
		Invalidate();
	}

}
void CBuildLightAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	
	//如果是外墙模式
	if (pMain->GetOutWallProperty().IsPaneVisible())
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
		{
			double x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
			double y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;

			if (sqrt((x - point.x)*(x - point.x) + (y - point.y)*(y - point.y)) < 10)
			{
				m_selectedOutWallPoint = i;
				break;
			}
		}
	}

	//如果是内墙模式
	if (pMain->GetInWallProperty().IsPaneVisible())
	{
		pMain->GetInWallProperty().InsertPos(point.x,point.y,point.x,point.y);
		m_isDrawInWall = true;
	}
	
}
void CBuildLightAnalysisView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//如果是外墙模式，并且有选中的外墙点，则移动它
	if (pMain->GetOutWallProperty().IsPaneVisible() && m_selectedOutWallPoint >= 0)
	{
		m_selectedOutWallPoint = -1;
	}
	//如果是内墙模式
	if (m_isDrawInWall && pMain->GetInWallProperty().IsPaneVisible())
	{
		m_isDrawInWall = false;
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


void CBuildLightAnalysisView::OnEditOptimize()
{
	optimize();

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();

	pMain->GetOutWallProperty().ShowPane(FALSE,FALSE,TRUE);
	pMain->GetInWallProperty().ShowPane(FALSE,FALSE,TRUE);
	pMain->GetOptimizeWallProperty().ShowPane(TRUE,FALSE,TRUE);

	Invalidate();
	
}
