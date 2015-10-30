
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

#define SELECT_LEN 50.0

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
	ON_COMMAND(ID_EDIT_DO_OPTIMIZE, &CBuildLightAnalysisView::OnEditDoOptimize)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TRANSLATE, &CBuildLightAnalysisView::OnUpdateEditTranslate)
	ON_COMMAND(ID_POP_DELETE_WIN, &CBuildLightAnalysisView::OnPopDeleteWin)
END_MESSAGE_MAP()

// CBuildLightAnalysisView construction/destruction

CBuildLightAnalysisView::CBuildLightAnalysisView():m_buttonDownOutWallPoint(-1),m_isDrawInWall(false),
	m_iSelectOutWallIndex(-1), m_iSelectInWallIndex(-1), m_iSelectWindowIndex(-1),m_bIsPullTranslate(false),
	m_iSelectGridRoomIndex(-1),m_iSelectGridIndex(-1),m_outModeOutWallSelectIndex(-1),m_inModeInWallSelectIndex(-1)
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
	
	double wTh = pMain->GetOptionProperty().GetDataDouble(OPTION_OPTIMIZE_TH);

	OptimizeLine(sLines,outLines, wTh);

	//设置处理后的墙
	CString outMat = pMain->GetOptionProperty().GetDataCString(OPTION_OUTWALL_MAT);
	CString inMat = pMain->GetOptionProperty().GetDataCString(OPTION_OUTWALL_MAT);
	pMain->GetOptimizeWallProperty().DeleteAllPos();
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


	//绘制坐标系
	Vec2d Oaxis(0,0);
	Vec2d Xaxis(2000,0);
	Vec2d Yaxis(0,2000);
	Oaxis = m_transform.RealToScreen(Oaxis);
	Xaxis = m_transform.RealToScreen(Xaxis);
	Yaxis = m_transform.RealToScreen(Yaxis);
	AdjustableArrowCap cap(4,4,true);
	
	Gdiplus::Pen ArrowXPen(Gdiplus::Color(255,255,0,0), 3);
	Gdiplus::Pen ArrowYPen(Gdiplus::Color(255,0,0,255), 3);
	ArrowXPen.SetCustomEndCap(&cap);
	ArrowYPen.SetCustomEndCap(&cap);
	graph->DrawLine(&ArrowXPen, (float)Oaxis.x, (float)Oaxis.y, (float)Xaxis.x, (float)Xaxis.y);
	graph->DrawLine(&ArrowYPen, (float)Oaxis.x, (float)Oaxis.y, (float)Yaxis.x, (float)Yaxis.y);

	//绘制指北针
	AdjustableArrowCap NorthCap(15,4,true);
	Gdiplus::Pen ArrowNorthPen(Gdiplus::Color(255,255,0,0), 3);
	ArrowNorthPen.SetCustomEndCap(&NorthCap);
	double angle = pMain->GetOptionProperty().GetDataDouble(OPTION_NORTH) + 90.0;
	double anglep = angle / 180 * PI;
	Vec2d angleDir(cos(anglep),-sin(anglep));
	double offsetX = 80;
	double offsetY = 80;
	double arrowLen = 50;
	Vec2d northArrowP(rect.right - offsetX, rect.top + offsetY);
	Vec2d northArrowS = northArrowP - arrowLen*0.5*angleDir;
	Vec2d northArrowE = northArrowP + arrowLen*0.5*angleDir;
	Vec2d northArrowT = northArrowP + arrowLen*0.8*angleDir;
	graph->DrawLine(&ArrowNorthPen, (float)northArrowS.x, (float)northArrowS.y, (float)northArrowE.x, (float)northArrowE.y);

	PointF posNorth(northArrowT.x, northArrowT.y);
	FontFamily fontFamilyNorth(L"幼圆");
	Gdiplus::Font font(&fontFamilyNorth,12);

	SolidBrush brushNorth(Color(255, 255, 0, 0));
	CString na = _T("北");
	WCHAR *wch = (WCHAR*)na.GetBuffer(na.GetLength());
	posNorth.X -= 10;
	posNorth.Y -= 8;
	graph->DrawString(wch ,na.GetLength(), &font,posNorth, &brushNorth);

	//绘制比例尺
	double scaleOption = pMain->GetOptionProperty().GetDataDouble(OPTION_PIX_MM_SCALE);
	double pixLen = 80;
	Gdiplus::Pen ScalePen(Gdiplus::Color(255,0,0,0), 3);
	Vec2d scaleRight(rect.right - 20, rect.bottom - 20);
	Vec2d scaleLeft(scaleRight.x - pixLen, scaleRight.y);
	graph->DrawLine(&ScalePen, (float)scaleLeft.x, (float)scaleLeft.y, (float)scaleRight.x, (float)scaleRight.y);
	graph->DrawLine(&ScalePen, (float)scaleLeft.x, (float)scaleLeft.y - 3, (float)scaleLeft.x, (float)scaleLeft.y+2);
	graph->DrawLine(&ScalePen, (float)scaleRight.x, (float)scaleRight.y - 3, (float)scaleRight.x, (float)scaleRight.y+2);

	PointF posScale(scaleLeft.x, scaleLeft.y);
	FontFamily fontFamilyScale(L"幼圆");
	Gdiplus::Font fontScale(&fontFamilyScale,12);

	SolidBrush brushScale(Color(255, 0, 0, 0));
	CString ScaleStr;
	ScaleStr.Format(_T("%0.2fmm"), pixLen/scaleOption);
	WCHAR *wchScale = (WCHAR*)ScaleStr.GetBuffer(ScaleStr.GetLength());
	posScale.X += 0;
	posScale.Y -= 20;
	graph->DrawString(wchScale ,ScaleStr.GetLength(), &fontScale, posScale, &brushScale);

	//取配置中的内外墙颜色
	COLORREF outWallColor = pMain->GetOptionProperty().GetDataInt(OPTION_OUTWALL_COLOR);
	COLORREF inWallColor = pMain->GetOptionProperty().GetDataInt(OPTION_INWALL_COLOR);
	COLORREF keyGridColor = pMain->GetOptionProperty().GetDataInt(OPTION_KEYGRID_COLOR);

	Gdiplus::SolidBrush pointBrush(Gdiplus::Color(255,100,100,100));

	if (pMain->GetMode() != MODE_OPTIMIZE)
	{
		//画外墙
		Pen outPen(Color(255,GetRValue(outWallColor),GetGValue(outWallColor),GetBValue(outWallColor)), 10);
		SolidBrush outPointBrush(Color(255,GetRValue(outWallColor),GetGValue(outWallColor),GetBValue(outWallColor)));
		Vec2d p, p1, startP, lastP;
		if (outWallPos->GetSubItemsCount() > 0)
		{
			for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
			{
				p.x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
				p.y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;
				p = m_transform.RealToScreen(p);
				//画节点
				if (i == m_outModeOutWallSelectIndex || outWallPos->GetSubItem(i)->IsSelected())
				{
					graph->FillEllipse(&outPointBrush, (float)p.x - 8, (float)p.y - 8, 16.0, 16.0);
				}
				else 
					graph->FillEllipse(&outPointBrush, (float)p.x - 5, (float)p.y - 5, 10.0, 10.0);
				if (i == 0)
				{
					startP = p;
					lastP = p;
					continue;
				}
				//画线段
				graph->DrawLine(&outPen, (float)lastP.x, (float)lastP.y, (float)p.x, (float)p.y);
				lastP = p;
			}
			graph->DrawLine(&outPen, (float)lastP.x, (float)lastP.y, (float)startP.x, (float)startP.y);
		}
		//画内墙
		Gdiplus::Pen inPen(Gdiplus::Color(255,GetRValue(inWallColor),GetGValue(inWallColor),GetBValue(inWallColor)), 6);
		Gdiplus::Pen inSelectPen(Gdiplus::Color(255,GetRValue(inWallColor),GetGValue(inWallColor),GetBValue(inWallColor)), 9);
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			p.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

			//画节点
			if (i == m_inModeInWallSelectIndex || inWallPos->GetSubItem(i)->IsSelected())
				graph->DrawLine(&inSelectPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			else
				graph->DrawLine(&inPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
		}

	}
	else
	{
		//画处理后的外墙
		CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
		if (!optimizeOutWallPos)
			return;

		Pen outPen(Color(255,GetRValue(outWallColor),GetGValue(outWallColor),GetBValue(outWallColor)), 10);
		Vec2d p,p1;
		for (int i = 0; i < optimizeOutWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeOutWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeOutWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeOutWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeOutWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

			if (i == m_iSelectOutWallIndex || optimizeOutWallPos->GetSubItem(i)->IsSelected())//如果是拾取的外墙
			{
				Gdiplus::Pen selectPen(Color(255,GetRValue(outWallColor),GetGValue(outWallColor),GetBValue(outWallColor)), 16);
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

		Gdiplus::Pen inPen(Gdiplus::Color(255,GetRValue(inWallColor),GetGValue(inWallColor),GetBValue(inWallColor)), 6);
		for (int i = 0; i < optimizeInWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeInWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeInWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			p1.x = optimizeInWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p1.y = optimizeInWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			p = m_transform.RealToScreen(p);
			p1 = m_transform.RealToScreen(p1);

			if (i == m_iSelectInWallIndex || optimizeInWallPos->GetSubItem(i)->IsSelected())//如果是拾取的内墙
			{
				Gdiplus::Pen selectPen(Gdiplus::Color(255,GetRValue(inWallColor),GetGValue(inWallColor),GetBValue(inWallColor)), 12);
				graph->DrawLine(&selectPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
			else
			{
				graph->DrawLine(&inPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
			}
		}

		//画窗户
		Gdiplus::Pen winPen(Gdiplus::Color(255,100,255,100), 9);
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

			Vec2d dir = p1 - p;
			Vec2d pWinc = p + dir * pos;
			dir = dir / dir.Length();
			Vec2d pWins = pWinc - dir * width * 0.5;
			Vec2d pWine = pWinc + dir * width * 0.5;

			pWins = m_transform.RealToScreen(pWins);
			pWine = m_transform.RealToScreen(pWine);

			if (i == m_iSelectWindowIndex || pWindow->IsSelected())//如果是拾取的窗户
			{
				Gdiplus::Pen selectPen(Gdiplus::Color(255,100,255,100), 15);
				graph->DrawLine(&selectPen, (float)pWins.x, (float)pWins.y, (float)pWine.x, (float)pWine.y);
			}
			else
				graph->DrawLine(&winPen, (float)pWins.x, (float)pWins.y, (float)pWine.x, (float)pWine.y);
			
		}
		//画房间
		//包含：房间名，计算点，灯具
		Gdiplus::Pen selectGridPen(Gdiplus::Color(180,GetRValue(keyGridColor),GetGValue(keyGridColor),GetBValue(keyGridColor)),3);
		Gdiplus::Pen keyGridPen(Gdiplus::Color(255,GetRValue(keyGridColor),GetGValue(keyGridColor),GetBValue(keyGridColor)),3);
		Gdiplus::Pen roomPen(Gdiplus::Color(255,100,255,100), 20);
		Gdiplus::Pen roomLinesPen(Gdiplus::Color(255,255,255,100), 4);
		PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
		for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(i);
			CMFCPropertyGridProperty* pOutWall = pRoom->GetSubItem(ROOM_OUT_WALL);
			CMFCPropertyGridProperty* pInWall = pRoom->GetSubItem(ROOM_IN_WALL);
			CMFCPropertyGridProperty* pWindow = pRoom->GetSubItem(ROOM_WINDOW);
			Vec2d pos = Vec2d::ZERO;
			int sumCount = 0;
			for (int j = 0; j < pOutWall->GetSubItemsCount(); j++)
			{
				int index = pOutWall->GetSubItem(j)->GetValue().intVal;
				p.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				p = m_transform.RealToScreen(p);
				p1 = m_transform.RealToScreen(p1);
				graph->DrawLine(&roomLinesPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
				pos += p + p1;
				sumCount += 2;
			}
			for (int j = 0; j < pInWall->GetSubItemsCount(); j++)
			{
				int index = pInWall->GetSubItem(j)->GetValue().intVal;
				p.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				p = m_transform.RealToScreen(p);
				p1 = m_transform.RealToScreen(p1);
				graph->DrawLine(&roomLinesPen, (float)p.x, (float)p.y, (float)p1.x, (float)p1.y);
				pos += p + p1;
				sumCount += 2;
			}
			if (sumCount)
			{
				pos /= sumCount;
				//pos = m_transform.RealToScreen(pos);
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
			//计算点
			CMFCPropertyGridProperty* pGrid = pRoom->GetSubItem(ROOM_GRID);
			if (pGrid->GetSubItemsCount() == GRID_POINTS+1)//如果存在计算点
			{
				CMFCPropertyGridProperty* pPoints = pGrid->GetSubItem(GRID_POINTS);

				CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* pPointsList = ((PropertyGridProperty*)pPoints)->GetSubItemList();
				POSITION interatorP = pPointsList->GetHeadPosition(); 
				int j = 0;
				while(interatorP != NULL)   
				{   
					CMFCPropertyGridProperty* curPoint = pPointsList->GetNext(interatorP);

					p.x = curPoint->GetSubItem(0)->GetValue().dblVal;
					p.y = curPoint->GetSubItem(1)->GetValue().dblVal;
					p = m_transform.RealToScreen(p);
					CString _name = curPoint->GetName();
					if (i == m_iSelectGridRoomIndex && j == m_iSelectGridIndex)//如果是拾取的计算点
					{
						graph->DrawLine(&selectGridPen, (float)p.x - 4, (float)p.y, (float)p.x + 4, (float)p.y);
						graph->DrawLine(&selectGridPen, (float)p.x, (float)p.y - 4, (float)p.x, (float)p.y + 4);
					}
					else if (_name.Left(3) == _T("关键点"))
					{
						graph->DrawLine(&keyGridPen, (float)p.x - 4, (float)p.y, (float)p.x + 4, (float)p.y);
						graph->DrawLine(&keyGridPen, (float)p.x, (float)p.y - 4, (float)p.x, (float)p.y + 4);

					}
					else
						graph->FillEllipse(&pointBrush, (float)p.x - 2, (float)p.y - 2, 4.0, 4.0);

					j++;
				}
			}

			Gdiplus::Pen lumPen(Gdiplus::Color(255,0,0,0),1);
			//单个灯具
			CMFCPropertyGridProperty* pSingleLum = pRoom->GetSubItem(ROOM_SINGLE_LUMINAIRE);
			for (int j = 0; j < pSingleLum->GetSubItemsCount(); j++)
			{
				CMFCPropertyGridProperty* plum = pSingleLum->GetSubItem(j);
				p.x = plum->GetSubItem(LUM_SINGLE_X)->GetValue().dblVal;
				p.y = plum->GetSubItem(LUM_SINGLE_Y)->GetValue().dblVal;
				p = m_transform.RealToScreen(p);					
				PointF pointsOut[] = {PointF(p.x-5,p.y-3),PointF(p.x-5,p.y+3),PointF(p.x+5,p.y+3),PointF(p.x+5,p.y-3),PointF(p.x-5,p.y-3)};
				PointF pointsIn[] = {PointF(p.x-5,p.y-1),PointF(p.x-5,p.y+1),PointF(p.x+5,p.y+1),PointF(p.x+5,p.y-1),PointF(p.x-5,p.y-1)};
				graph->DrawLines(&lumPen,pointsOut,5);
				graph->DrawLines(&lumPen,pointsIn,5);
			}
			//组灯具
			CMFCPropertyGridProperty* pSetLum = pRoom->GetSubItem(ROOM_SET_LUMINAIRE);
			for (int j = 0; j < pSetLum->GetSubItemsCount(); j++)
			{
				CMFCPropertyGridProperty* pSet = pSetLum->GetSubItem(j);
				CMFCPropertyGridProperty* pPoints = pSet->GetSubItem(LUM_SET_POINTS);

				CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* pPointsList = ((PropertyGridProperty*)pPoints)->GetSubItemList();
				POSITION interatorP = pPointsList->GetHeadPosition(); 
				while(interatorP != NULL)   
				{   
					CMFCPropertyGridProperty* curPoint = pPointsList->GetNext(interatorP);
					p.x = curPoint->GetSubItem(0)->GetValue().dblVal;
					p.y = curPoint->GetSubItem(1)->GetValue().dblVal;
					p = m_transform.RealToScreen(p);					
					PointF pointsOut[] = {PointF(p.x-5,p.y-3),PointF(p.x-5,p.y+3),PointF(p.x+5,p.y+3),PointF(p.x+5,p.y-3),PointF(p.x-5,p.y-3)};
					PointF pointsIn[] = {PointF(p.x-5,p.y-1),PointF(p.x-5,p.y+1),PointF(p.x+5,p.y+1),PointF(p.x+5,p.y-1),PointF(p.x-5,p.y-1)};
					graph->DrawLines(&lumPen,pointsOut,5);
					graph->DrawLines(&lumPen,pointsIn,5);
				}
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

	//移动
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
	if (pMain->GetMode() == MODE_OUTWALL && m_buttonDownOutWallPoint >= 0)
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		outWallPos->GetSubItem(m_buttonDownOutWallPoint)->GetSubItem(0)->SetValue(p.x);
		outWallPos->GetSubItem(m_buttonDownOutWallPoint)->GetSubItem(1)->SetValue(p.y);

		Invalidate();
	}
	//如果是外墙模式，拾取节点
	if (pMain->GetMode() == MODE_OUTWALL)
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		m_outModeOutWallSelectIndex = -1;
		for (int i = 0; i < outWallPos->GetSubItemsCount(); i++)
		{
			double x = outWallPos->GetSubItem(i)->GetSubItem(0)->GetValue().dblVal;
			double y = outWallPos->GetSubItem(i)->GetSubItem(1)->GetValue().dblVal;

			if (sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y)) < 100)
			{
				m_outModeOutWallSelectIndex = i;
				break;
			}
		}
		Invalidate();
	}
	
	//如果是内墙模式
	if (pMain->GetMode() == MODE_INWALL && m_isDrawInWall)
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
	//如果是内墙模式，拾取节点
	if (pMain->GetMode() == MODE_INWALL)
	{
		CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
		if (!inWallPos)
			return;

		m_inModeInWallSelectIndex = -1;
		sLine line;
		for (int i = 0; i < inWallPos->GetSubItemsCount(); i++)
		{
			line.s.x = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			line.s.y = inWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;

			line.e.x = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			line.e.y = inWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;

			if (lenOfLinePoint(line, p) < SELECT_LEN)
			{
				m_inModeInWallSelectIndex = i;
				break;
			}
		}
		Invalidate();
	}
	//如果是处理后模式
	if (pMain->GetMode() == MODE_OPTIMIZE)
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

			if (lenOfLinePoint(line, p) < SELECT_LEN)
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

			if (lenOfLinePoint(line, p) < SELECT_LEN)
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

			if (lenOfLinePoint(line, p) < SELECT_LEN)
			{
				m_iSelectWindowIndex = i;
				m_iSelectInWallIndex = -1;
				m_iSelectOutWallIndex = -1;
				break;
			}
		}

		//计算点拾取
		m_iSelectGridRoomIndex = -1;
		m_iSelectGridIndex = -1;
		Vec2d gridP;
		PropertyGridCtrl* pRoomList = pMain->GetRoomProperty().getPropList();
		for (int i = 0; i < pRoomList->GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pGrid = pRoomList->GetProperty(i)->GetSubItem(ROOM_GRID);
			if (pGrid->GetSubItemsCount() < GRID_POINTS+1)
				continue;
			bool flag = false;
			CMFCPropertyGridProperty* points = pGrid->GetSubItem(GRID_POINTS);
			for (int j = 0; j < points->GetSubItemsCount(); j++)
			{
				gridP.x = points->GetSubItem(j)->GetSubItem(0)->GetValue().dblVal;
				gridP.y = points->GetSubItem(j)->GetSubItem(1)->GetValue().dblVal;
				
				if ((p-gridP).Length() < SELECT_LEN)
				{
					m_iSelectGridRoomIndex = i;
					m_iSelectGridIndex = j;
					flag = true;
					break;
				}
			}
			if (flag)
				break;
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
	if (pMain->GetMode() == MODE_OUTWALL)
	{
		if (MK_CONTROL&nFlags)//如果ctrl键，则添加外墙点
		{
			pMain->GetOutWallProperty().InsertPos(p.x, p.y);
		}
		else//否则是拖动外墙点
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
					m_buttonDownOutWallPoint = i;
					break;
				}
			}
		}
	}

	//如果是内墙模式
	if (pMain->GetMode() == MODE_INWALL)
	{
		pMain->GetInWallProperty().InsertPos(p.x,p.y,p.x,p.y);
		m_isDrawInWall = true;
	}

	//添加关键计算点
	if (pMain->GetMode() == MODE_OPTIMIZE && MK_CONTROL&nFlags && m_iSelectGridRoomIndex >= 0 && m_iSelectGridIndex >= 0)
	{
		CMFCPropertyGridProperty* pPoint = pMain->GetRoomProperty().getPropList()->GetProperty(m_iSelectGridRoomIndex)->GetSubItem(ROOM_GRID)->GetSubItem(GRID_POINTS)->GetSubItem(m_iSelectGridIndex);
		CString _name = pPoint->GetName();
		if (_name.Left(3) == _T("关键点"))
		{
			pPoint->SetName(_T("点(mm)"));
		}
		else
		{
			pPoint->SetName(_T("关键点(mm)"));
		}
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
	if (pMain->GetMode() == MODE_OUTWALL && m_buttonDownOutWallPoint >= 0)
	{
		m_buttonDownOutWallPoint = -1;
	}
	//如果是内墙模式
	if (pMain->GetMode() == MODE_INWALL && m_isDrawInWall)
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


// 优化操作
void CBuildLightAnalysisView::OnEditDoOptimize()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	if (pMain->GetWindowProperty().getPropList()->GetPropertyCount() > 0 || pMain->GetRoomProperty().getPropList()->GetPropertyCount() > 0)
	{
		int nRes = AfxMessageBox(_T("检测到已有窗户或房间信息，如果优化处理的话会删除掉该信息，确认处理吗？"), MB_OKCANCEL | 
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
	pMain->SetMode(MODE_OPTIMIZE);
	Invalidate();
}


void CBuildLightAnalysisView::OnEditOptimize()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	pMain->GetOutWallProperty().ShowPane(FALSE,FALSE,TRUE);
	pMain->GetInWallProperty().ShowPane(FALSE,FALSE,TRUE);
	if (pMain->GetOptimizeWallProperty().IsPaneVisible())
		pMain->GetOptimizeWallProperty().ShowPane(FALSE,FALSE,TRUE);
	else
		pMain->GetOptimizeWallProperty().ShowPane(TRUE,FALSE,TRUE);
	pMain->SetMode(MODE_OPTIMIZE);
	Invalidate();
	
}


void CBuildLightAnalysisView::OnPopAddWindow()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	pMain->GetWindowProperty().ShowPane(TRUE,FALSE,TRUE);

	pMain->GetWindowProperty().InsertWindow(m_iSelectOutWallIndex, m_iSelectInWallIndex, m_iSelectWindowIndex, pMain->GetOptionProperty().GetDataCString(OPTION_WINDOW_MAT));
}


void CBuildLightAnalysisView::OnPopAddtoRoom()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	pMain->GetRoomProperty().ShowPane(TRUE,FALSE,TRUE);
	
	if (m_iSelectOutWallIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(ROOM_OUT_WALL, m_iSelectOutWallIndex))
	{

	}
	if (m_iSelectInWallIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(ROOM_IN_WALL, m_iSelectInWallIndex))
	{

	}
	if (m_iSelectWindowIndex >= 0 && pMain->GetRoomProperty().AddToSelectedRoom(ROOM_WINDOW, m_iSelectWindowIndex))
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



void CBuildLightAnalysisView::OnUpdateEditTranslate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsPullTranslate);
}


void CBuildLightAnalysisView::OnPopDeleteWin()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	pMain->GetWindowProperty().DeleteWindowByIndex(m_iSelectWindowIndex);
}
