
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

//���д���
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

	//���ô�����ǽ
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

void CBuildLightAnalysisView::OnDraw(CDC* /*pDC*/)
{
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();

	if (pMain && !pDoc->m_bIsOpen)//���û�д���Ŀ�򲻽��л���
		return;

	CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
	CMFCPropertyGridProperty* inWallPos = pMain->GetInWallProperty().getCoodGroup();
	if (!outWallPos || !inWallPos)
		return;

	if (!pMain->GetOptimizeWallProperty().IsPaneVisible())
	{
		CDC *pDC=GetDC();        //CDC��ʽ����

		//����ǽ
		CPen pen(PS_SOLID,8,RGB(100,100,100));    //���廭��

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
			else if (i == outWallPos->GetSubItemsCount() -1)//���ɻ�
			{
				pDC->LineTo(p);
				pDC->LineTo(startP);
			}
			else
			{
				pDC->LineTo(p);
			}
		}

		//����ǽ
		CPen pen1(PS_SOLID,4,RGB(100,100,100));    //���廭��
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
		//����������ǽ
		CMFCPropertyGridProperty* optimizeWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
		if (!optimizeWallPos)
			return;

		CDC *pDC=GetDC();						  //CDC��ʽ����
		CPen pen(PS_SOLID,6,RGB(100,100,100));    //���廭��
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

		//����������ǽ
		CMFCPropertyGridProperty* optimizeinWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
		if (!optimizeinWallPos)
			return;

		CPen pen1(PS_SOLID,6,RGB(255,0,255));    //���廭��
		pOldPen=pDC->SelectObject(&pen1);
		for (int i = 0; i < optimizeinWallPos->GetSubItemsCount(); i++)
		{
			p.x = optimizeinWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeinWallPos->GetSubItem(i)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
			pDC->MoveTo(p);

			p.x = optimizeinWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
			p.y = optimizeinWallPos->GetSubItem(i)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
			pDC->LineTo(p);
		}
	}
	
	
}

void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CBuildLightAnalysisDoc* pDoc = GetDocument();
	CString xy;
	xy.Format(_T("%d,%d"),point.x, point.y);
	pMain->GetStatusBar().SetPaneText(0,xy);

	//�����Ŀδ�򿪣�����ʲô
	if (pMain && !pDoc->m_bIsOpen)
		return;

	//�������ǽģʽ����ѡ�е���ǽ�㣬���ƶ���
	if (pMain->GetOutWallProperty().IsPaneVisible() && m_selectedOutWallPoint >= 0)
	{
		CMFCPropertyGridProperty* outWallPos = pMain->GetOutWallProperty().getCoodGroup();
		if (!outWallPos)
			return;
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(0)->SetValue((double)point.x);
		outWallPos->GetSubItem(m_selectedOutWallPoint)->GetSubItem(1)->SetValue((double)point.y);

		Invalidate();
	}
	//�������ǽģʽ
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
	//����Ǵ����ģʽ
	if (pMain->GetOptimizeWallProperty().IsPaneVisible())
	{

	}

}
void CBuildLightAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	CBuildLightAnalysisDoc* pDoc = GetDocument();

	if (pMain && !pDoc->m_bIsOpen)
	{
		AfxMessageBox(_T("��Ŀ��δ���������½������Ŀ"));
		return; 
	}
	//�������ǽģʽ
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

	//�������ǽģʽ
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
	//�����Ŀδ�򿪣�����ʲô
	if (pMain && !pDoc->m_bIsOpen)
		return;

	//�������ǽģʽ��������ѡ�е���ǽ�㣬���ƶ���
	if (pMain->GetOutWallProperty().IsPaneVisible() && m_selectedOutWallPoint >= 0)
	{
		m_selectedOutWallPoint = -1;
	}
	//�������ǽģʽ
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
