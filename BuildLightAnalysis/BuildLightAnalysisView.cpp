
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
#include "MathUtility.h"
#include <Gdiplus.h>

using namespace Gdiplus;

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
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_POP_ADD_WINDOW, &CBuildLightAnalysisView::OnPopAddWindow)
END_MESSAGE_MAP()

// CBuildLightAnalysisView construction/destruction

CBuildLightAnalysisView::CBuildLightAnalysisView():m_selectedOutWallPoint(-1),m_isDrawInWall(false),
	m_iSelectOutWallIndex(-1), m_iSelectInWallIndex(-1)
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
	pMain->GetOutWallProperty().OutputToLines(sLines);
	pMain->GetInWallProperty().OutputToLines(sLines);
	
	double wTh = pMain->GetOptionProperty().getProperty(1);

	OptimizeLine(sLines,outLines, wTh);

	//设置处理后的墙
	pMain->GetOptimizeWallProperty().DeletePos();
	for (int i = 0; i < outLines.size(); i++)
	{
		if (outLines[i].type == sLine::OUT_WALL)
		{
			pMain->GetOptimizeWallProperty().InsertPos(true, outLines[i].s.x,outLines[i].s.y,outLines[i].e.x,outLines[i].e.y);
		}
		else
		{
			pMain->GetOptimizeWallProperty().InsertPos(false, outLines[i].s.x,outLines[i].s.y,outLines[i].e.x,outLines[i].e.y);
		}
		
	}
	

}
// CBuildLightAnalysisView drawing

void CBuildLightAnalysisView::OnDraw(CDC* pDC)
{
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();

	if (pMain && !pDoc->m_bIsOpen)//如果没有打开项目则不进行绘制
		return;

	CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
	CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
	if (!outWallPos || !inWallPos)
		return;

	CRect rect;
	GetClientRect(&rect);
	Bitmap bmp(rect.right, rect.bottom);
	
	Graphics* graph = Graphics::FromImage(&bmp);
	graph->Clear(Color::White);
	graph->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality); 

	if (!pMain->GetOptimizeWallProperty().IsPaneVisible())
	{
		//画外墙
		Gdiplus::Pen outPen(Gdiplus::Color(255,100,100,100), 10);
		Vec2d p, p1, startP, lastP;
		if (outWallPos->GetSubItemsCount() > 0)
		{
			startP.x = outWallPos->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			startP.y = outWallPos->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			lastP = startP;
			for (int i = 1; i < outWallPos->GetSubItemsCount(); i++)
			{
				p.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
				p.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;
				
				graph->DrawLine(&outPen, (float)lastP.x, (float)lastP.y, (float)p.x, (float)p.y);
				lastP = p;
			}
		}
		//画内墙
		Gdiplus::Pen inPen(Gdiplus::Color(255,100,100,100), 10);
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			p.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			graph->DrawLine(&inPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
		}

	}
	else
	{
		//画处理后的外墙
		CMFCPropertyGridProperty* optimizeWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
		if (!optimizeWallPos)
			return;

		Gdiplus::Pen outPen(Gdiplus::Color(255,100,100,100), 10);
		CPoint p,p1;
		for (int i = 0; i < optimizeWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			if (i == m_iSelectOutWallIndex)//如果是拾取的外墙
			{
				Gdiplus::Pen selectPen(Gdiplus::Color(255,100,100,100), 16);
				graph->DrawLine(&selectPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
			else
			{
				graph->DrawLine(&outPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
		}

		//画处理后的内墙
		CMFCPropertyGridProperty* optimizeinWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
		if (!optimizeinWallPos)
			return;

		Gdiplus::Pen inPen(Gdiplus::Color(255,100,100,100), 10);
		for (int i = 0; i < optimizeinWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeinWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeinWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeinWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeinWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			if (i == m_iSelectInWallIndex)//如果是拾取的外墙
			{
				Gdiplus::Pen selectPen(Gdiplus::Color(255,100,100,100), 16);
				graph->DrawLine(&selectPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
			else
			{
				graph->DrawLine(&inPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
		}
	}
	/*Graphics graphics(pDC->GetSafeHdc());
	CachedBitmap cachedBmp(&bmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,0,0);*/

	Graphics graphics(pDC->GetSafeHdc());
	graphics.DrawImage(&bmp, rect.left, rect.top, rect.right, rect.bottom);
	delete graph;
}

void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	CString xy;
	xy.Format(_T("%d,%d"),point.x, point.y);
	pMain->GetStatusBar().SetPaneText(0,xy);

	//如果项目未打开，则不做什么
	if (pMain && !pDoc->m_bIsOpen)
		return;

	Vec2d p(point.x, point.y);

	//如果是外墙模式，有选中的外墙点，则移动它
	if (pMain->GetOutWallProperty().IsPaneVisible() && m_selectedOutWallPoint >= 0)
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(0)->SetValue(p.x);
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(1)->SetValue(p.y);

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
		inWallPos->GetSubItem(count-1)->GetSubItem(1)->GetSubItem(0)->SetValue(p.x);
		inWallPos->GetSubItem(count-1)->GetSubItem(1)->GetSubItem(1)->SetValue(p.y);
		Invalidate();
	}
	//如果是处理后模式
	if (pMain->GetOptimizeWallProperty().IsPaneVisible())
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
		if (!outWallPos)
			return;

		sLine line;
		m_iSelectOutWallIndex = -1;
		for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
		{
			line.s.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			line.s.y = outWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			line.e.x = outWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			line.e.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			if (lenOfLinePoint(line, p) < 10)
			{
				m_iSelectOutWallIndex = i;
				break;
			}
		}

		CMFCPropertyGridProperty* inWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
		if (!inWallPos)
			return;
		m_iSelectInWallIndex = -1;
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			line.s.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			line.s.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			line.e.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			line.e.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			if (lenOfLinePoint(line, p) < 10)
			{
				m_iSelectInWallIndex = i;
				break;
			}
		}
		Invalidate();
	}

}
void CBuildLightAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CBuildLightAnalysisDoc* pDoc = GetDocument();

	if (pMain && !pDoc->m_bIsOpen)
	{
		AfxMessageBox(_T("项目还未建立，请新建或打开项目"));
		return; 
	}
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
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	//如果项目未打开，则不做什么
	if (pMain && !pDoc->m_bIsOpen)
		return;

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
	if (m_iSelectOutWallIndex >= 0 || m_iSelectInWallIndex >= 0)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}
}

BOOL CBuildLightAnalysisView::OnEraseBkgnd(CDC* pdc)
{
	return TRUE;
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


void CBuildLightAnalysisView::OnPopAddWindow()
{
	// TODO: Add your command handler code here
}
