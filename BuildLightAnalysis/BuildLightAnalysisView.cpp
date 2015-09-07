
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
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_OPTIMIZE, &CBuildLightAnalysisView::OnEditOptimize)
	ON_COMMAND(ID_POP_ADD_WINDOW, &CBuildLightAnalysisView::OnPopAddWindow)
	ON_COMMAND(ID_POP_ADDTO_ROOM, &CBuildLightAnalysisView::OnPopAddtoRoom)
	ON_COMMAND(ID_EDIT_TRANSLATE, &CBuildLightAnalysisView::OnEditTranslate)
END_MESSAGE_MAP()

// CBuildLightAnalysisView construction/destruction

CBuildLightAnalysisView::CBuildLightAnalysisView():m_selectedOutWallPoint(-1),m_isDrawInWall(false),
	m_iSelectOutWallIndex(-1), m_iSelectInWallIndex(-1), m_iSelectWindowIndex(-1),m_bIsPullTranslate(false)
{
	// TODO: add construction code here
	m_transform.scale = 0.1;
	m_transform.height = 100;
	m_transform.center = Vec2d(100,100);
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
	CString outMat = pMain->GetOptionProperty().getPropList()->GetProperty(2)->GetValue().bstrVal;
	CString inMat = pMain->GetOptionProperty().getPropList()->GetProperty(3)->GetValue().bstrVal;
	pMain->GetOptimizeWallProperty().DeletePos();
	for (int i = 0; i < outLines.size(); i++)
	{
		if (outLines[i].type == sLine::OUT_WALL)
		{

			pMain->GetOptimizeWallProperty().InsertPos(true, outLines[i].s.x,outLines[i].s.y,outLines[i].e.x,outLines[i].e.y, outMat);
		}
		else
		{
			pMain->GetOptimizeWallProperty().InsertPos(false, outLines[i].s.x,outLines[i].s.y,outLines[i].e.x,outLines[i].e.y, inMat);
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
			startP = m_transform.RealToScreen(startP);

			graph->DrawEllipse(&outPen, (float)startP.x - 2, (float)startP.y - 2, 4.0,4.0);
			lastP = startP;
			for (int i = 1; i < outWallPos->GetSubItemsCount(); i++)
			{
				p.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
				p.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;
				p = m_transform.RealToScreen(p);
				
				graph->DrawEllipse(&outPen, (float)p.x - 2, (float)p.y - 2, 4.0, 4.0);
				graph->DrawLine(&outPen, (float)lastP.x, (float)lastP.y, (float)p.x, (float)p.y);
				lastP = p;
			}
			graph->DrawLine(&outPen, (float)lastP.x, (float)lastP.y, (float)startP.x, (float)startP.y);
		}
		//画内墙
		Gdiplus::Pen inPen(Gdiplus::Color(255,100,100,100), 10);
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			p.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

			graph->DrawLine(&inPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
		}

	}
	else
	{
		//画处理后的外墙
		CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
		if (!optimizeOutWallPos)
			return;

		Gdiplus::Pen outPen(Gdiplus::Color(255,100,100,100), 10);
		Vec2d p,p1;
		for (int i = 0; i < optimizeOutWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeOutWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeOutWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeOutWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeOutWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

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
		CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
		if (!optimizeInWallPos)
			return;

		Gdiplus::Pen inPen(Gdiplus::Color(255,100,100,100), 10);
		for (int i = 0; i < optimizeInWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeInWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeInWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeInWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeInWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

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

		//画窗户
		Gdiplus::Pen winPen(Gdiplus::Color(255,100,255,100), 20);
		PropertyGridCtrl* list = pMain->GetWindowProperty().getPropList();
		for (int i = 0; i < list->GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pWindow = list->GetProperty(i);
			CString wallType;
			wallType = pWindow->GetSubItem(0)->GetValue().bstrVal;
			int wallIndex = pWindow->GetSubItem(1)->GetValue().intVal;
			double pos = pWindow->GetSubItem(2)->GetValue().dblVal;
			double width = pWindow->GetSubItem(5)->GetValue().dblVal;

			CMFCPropertyGridProperty* wallProperty = NULL;
			if (wallType == _T("外墙"))
			{
				wallProperty = optimizeOutWallPos;
			}
			else if (wallType = _T("内墙"))
			{
				wallProperty = optimizeInWallPos;
			}
			p.x = wallProperty->GetSubItem(wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = wallProperty->GetSubItem(wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			p1.x = wallProperty->GetSubItem(wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = wallProperty->GetSubItem(wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

			Vec2d dir = p1 - p;
			Vec2d pWinc = p + dir * pos;
			dir = dir / dir.Length();
			Vec2d pWins = pWinc - dir * width * 0.5;
			Vec2d pWine = pWinc + dir * width * 0.5;

			if (i == m_iSelectWindowIndex)//如果是拾取的外墙
			{
				Gdiplus::Pen selectPen(Gdiplus::Color(255,100,255,100), 25);
				graph->DrawLine(&selectPen, (float)pWins.x, (float)pWins.y, (float)pWine.x, (float)pWine.y);
			}
			else
				graph->DrawLine(&winPen, (float)pWins.x, (float)pWins.y, (float)pWine.x, (float)pWine.y);
			
		}
		//画房间
		Gdiplus::Pen roomPen(Gdiplus::Color(255,100,255,100), 20);
		PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
		for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(i);
			Vec2d pos = Vec2d::ZERO;
			int sumCount = 0;
			for (int j = 0; j < pRoom->GetSubItemsCount(); j++)
			{
				CString na = pRoom->GetSubItem(j)->GetName();
				if (na == _T("外墙号"))
				{
					int index = pRoom->GetSubItem(j)->GetValue().intVal;
					p.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
					p.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
					p1.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
					p1.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
					
					pos += p + p1;
					sumCount += 2;
				}
				else if (na == _T("内墙号"))
				{
					int index = pRoom->GetSubItem(j)->GetValue().intVal;
					p.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
					p.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
					p1.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
					p1.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

					pos += p + p1;
					sumCount += 2;
				}				
			}
			if (sumCount)
			{
				pos /= sumCount;
				pos = m_transform.RealToScreen(pos);
				PointF posf(pos.x, pos.y);
				FontFamily fontFamily(L"幼圆");
				Gdiplus::Font font(&fontFamily,12);
				
				SolidBrush brush(Color(255, 255, 0, 0));
				CString na = pRoom->GetName();
				WCHAR *wch = (WCHAR*)na.GetBuffer(na.GetLength());

				posf.X -= 6 * na.GetLength();
				posf.Y -= 6;
				graph->DrawString(wch ,na.GetLength(), &font,posf, &brush);
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
	

	//如果项目未打开，则不做什么
	if (pMain && !pDoc->m_bIsOpen)
		return;

	Vec2d p(point.x, point.y);
	p = m_transform.ScreenToReal(p);

	CString xy;
	xy.Format(_T("%f,%f"),p.x, p.y);
	pMain->GetStatusBar().SetPaneText(0,xy);

	if (m_bIsPullTranslate && MK_LBUTTON&nFlags > 0)
	{
		m_transform.center.x += point.x - prePoint.x;
		m_transform.center.y += prePoint.y - point.y;
		Invalidate();
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		if (pMain)
		{
			pMain->GetOptionProperty().GetTransform(m_transform.scale, m_transform.center.x, m_transform.center.y);
		}

		prePoint = point;
		return;
	}

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
		//外墙拾取
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

			if (lenOfLinePoint(line, p) < 100.0)
			{
				m_iSelectOutWallIndex = i;
				break;
			}
		}
		//内墙拾取
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

			if (lenOfLinePoint(line, p) < 100.0)
			{
				m_iSelectInWallIndex = i;
				m_iSelectOutWallIndex = -1;
				break;
			}
		}

		//窗户拾取
		m_iSelectWindowIndex = -1;
		PropertyGridCtrl* list = pMain->GetWindowProperty().getPropList();
		Vec2d p1, p2;
		for (int i = 0; i < list->GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pWindow = list->GetProperty(i);
			CString wallType;
			wallType = pWindow->GetSubItem(0)->GetValue().bstrVal;
			int wallIndex = pWindow->GetSubItem(1)->GetValue().intVal;
			double pos = pWindow->GetSubItem(2)->GetValue().dblVal;
			double width = pWindow->GetSubItem(5)->GetValue().dblVal;

			CMFCPropertyGridProperty* wallProperty = NULL;
			if (wallType == _T("外墙"))
			{
				wallProperty = outWallPos;
			}
			else if (wallType = _T("内墙"))
			{
				wallProperty = inWallPos;
			}
			p1.x = wallProperty->GetSubItem(wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p1.y = wallProperty->GetSubItem(wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			p2.x = wallProperty->GetSubItem(wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p2.y = wallProperty->GetSubItem(wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			Vec2d dir = p2 - p1;
			Vec2d pWinc = p1 + dir * pos;
			dir = dir / dir.Length();
			line.s = pWinc - dir * width * 0.5;
			line.e = pWinc + dir * width * 0.5;

			if (lenOfLinePoint(line, p) < 100.0)
			{
				m_iSelectWindowIndex = i;
				m_iSelectInWallIndex = -1;
				m_iSelectOutWallIndex = -1;
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
	Vec2d p(point.x, point.y);
	p = m_transform.ScreenToReal(p);

	//如果是移动模式
	if (m_bIsPullTranslate)
	{
		prePoint = point;
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

			if (sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)) < 100)
			{
				m_selectedOutWallPoint = i;
				break;
			}
		}
	}

	//如果是内墙模式
	if (pMain->GetInWallProperty().IsPaneVisible())
	{
		pMain->GetInWallProperty().InsertPos(p.x,p.y,p.x,p.y);
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
	if (m_iSelectOutWallIndex >= 0 || m_iSelectInWallIndex >= 0 || m_iSelectWindowIndex >= 0)
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

void CBuildLightAnalysisView::OnSize(UINT nType, int cx, int cy)
{
	m_transform.height = cy;
	CView::OnSize(nType,cx,cy);
}
BOOL CBuildLightAnalysisView::OnMouseWheel(UINT nType, short delta, CPoint point)
{
	if (delta > 0)
	{
		m_transform.scale *= 0.909;
	}
	else
	{
		m_transform.scale *= 1.1;
	}
	Invalidate();

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (pMain)
	{
		pMain->GetOptionProperty().GetTransform(m_transform.scale, m_transform.center.x, m_transform.center.y);
	}

	return CView::OnMouseWheel(nType, delta, point);

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
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;
	
	if (pMain->GetWindowProperty().getPropList()->GetPropertyCount() > 0 || pMain->GetRoomProperty().getPropList()->GetPropertyCount() > 0)
	{
		int nRes = AfxMessageBox(_T("检测到已有窗户或房间信息，如果处理的话会删除掉改信息，确认处理吗？"), MB_OKCANCEL | 
			MB_ICONQUESTION);
		if (IDCANCEL == nRes)
		{
			return;
		}
	}
	
	pMain->GetWindowProperty().DeleteAllWindow();
	pMain->GetRoomProperty().DeleteAllRoom();

	optimize();

	pMain->GetOutWallProperty().ShowPane(FALSE,FALSE,TRUE);
	pMain->GetInWallProperty().ShowPane(FALSE,FALSE,TRUE);
	pMain->GetOptimizeWallProperty().ShowPane(TRUE,FALSE,TRUE);

	Invalidate();
	
}


void CBuildLightAnalysisView::OnPopAddWindow()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	pMain->GetWindowProperty().ShowPane(TRUE,FALSE,TRUE);
	pMain->GetWindowProperty().InsertWindow(m_iSelectOutWallIndex, m_iSelectInWallIndex, m_iSelectWindowIndex);
}


void CBuildLightAnalysisView::OnPopAddtoRoom()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	pMain->GetRoomProperty().ShowPane(TRUE,FALSE,TRUE);
	
	if (m_iSelectOutWallIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(_T("外墙号"), m_iSelectOutWallIndex))
	{

	}
	if (m_iSelectInWallIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(_T("内墙号"), m_iSelectInWallIndex))
	{

	}
	if (m_iSelectWindowIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(_T("窗户号"), m_iSelectWindowIndex))
	{

	}
	
}

void CBuildLightAnalysisView::OnEditTranslate()
{
	if (!m_bIsPullTranslate)
	{
		SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_SIZEALL));
		m_bIsPullTranslate = true;
	}
	else
	{
		SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_ARROW));
		m_bIsPullTranslate = false;
	}
	
}