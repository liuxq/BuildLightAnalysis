
// BuildLightAnalysisDoc.cpp : implementation of the CBuildLightAnalysisDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BuildLightAnalysis.h"
#endif

#include "BuildLightAnalysisDoc.h"

#include <propkey.h>
#include "Serializer.h"
#include "MainFrm.h"
#include "DlgProjectNew.h"
#include "MathUtility.h"
#include <fstream>
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBuildLightAnalysisDoc

IMPLEMENT_DYNCREATE(CBuildLightAnalysisDoc, CDocument)

BEGIN_MESSAGE_MAP(CBuildLightAnalysisDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CBuildLightAnalysisDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CBuildLightAnalysisDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, &CBuildLightAnalysisDoc::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CBuildLightAnalysisDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_OUTPUT, &CBuildLightAnalysisDoc::OnFileOutput)
END_MESSAGE_MAP()


// CBuildLightAnalysisDoc construction/destruction

CBuildLightAnalysisDoc::CBuildLightAnalysisDoc():m_bIsOpen(false)
{
	// TODO: add one-time construction code here

}

CBuildLightAnalysisDoc::~CBuildLightAnalysisDoc()
{
}

BOOL CBuildLightAnalysisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CBuildLightAnalysisDoc serialization

void CBuildLightAnalysisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CBuildLightAnalysisDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CBuildLightAnalysisDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBuildLightAnalysisDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBuildLightAnalysisDoc diagnostics

#ifdef _DEBUG
void CBuildLightAnalysisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBuildLightAnalysisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBuildLightAnalysisDoc commands


void CBuildLightAnalysisDoc::OnFileSave()
{
	if (m_bIsOpen)
	{
		CString cPath;
		cPath.Format(_T("%s\\%s.bla"), m_projectLocation, m_projectName);
		CStringA stra(cPath.GetBuffer(0));
		cPath.ReleaseBuffer();
		string path=stra.GetBuffer(0);
		stra.ReleaseBuffer();

		ofstream outFile(path, ios::binary|ios::ate);
		if (!outFile.is_open())
		{
			CString strMsg;
			strMsg.Format (_T("保存项目文件\"%s\"失败"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//写入文件
		save(outFile);
	}	
}


void CBuildLightAnalysisDoc::OnFileSaveAs()
{
	CString FilePathName;
	CFileDialog dlg(FALSE, //FALSE为SAVE AS对话框
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BLA Files (*.bla)|*.bla|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
		int pos = FilePathName.ReverseFind('.');
		CString lxq = FilePathName.Mid(pos+1);
		if (pos == -1 || FilePathName.Mid(pos+1) != _T("bla"))
		{
			CString strMsg;
			strMsg.Format (_T("保存项目文件\"%s\"失败，后缀错误"), FilePathName); 
			AfxMessageBox(strMsg);
			return; 
		}

		int pos_ = FilePathName.ReverseFind('\\');

		m_projectLocation = FilePathName.Left(pos_);
		m_projectName = FilePathName.Mid(pos_+1, pos - 1 - pos_);

		CString cPath;
		cPath.Format(_T("%s\\%s.bla"), m_projectLocation, m_projectName);
		string path = CStringToString(cPath);

		ofstream outFile(path, ios::binary|ios::ate);
		if (!outFile.is_open())
		{
			CString strMsg;
			strMsg.Format (_T("保存项目文件\"%s\"失败"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//写入文件
		save(outFile);
		SetTitle(m_projectName);//设置文档名称
	}
	else
	{
		return;
	}
}


void CBuildLightAnalysisDoc::OnFileNew()
{
	DlgProjectNew dlgPn;
	if(dlgPn.DoModal()==IDOK)
	{
		m_projectName = dlgPn.m_projectName;
		m_projectLocation = dlgPn.m_projectLocation;
		m_bIsOpen = true;
		SetTitle(m_projectName);//设置文档名称

		//读取选项
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		//读取外墙
		loadMaterial();
		pMain->GetOptionProperty().loadMaterialTemplate();
		pMain->GetOptionProperty().SetTransform();
	}
	else
		return;
}


void CBuildLightAnalysisDoc::OnFileOpen()
{
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BLA Files (*.bla)|*.bla|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里


		int pos = FilePathName.ReverseFind('.');
		CString lxq = FilePathName.Mid(pos+1);
		if (pos == -1 || FilePathName.Mid(pos+1) != _T("bla"))
		{
			CString strMsg;
			strMsg.Format (_T("打开项目文件\"%s\"失败，后缀错误"), FilePathName); 
			AfxMessageBox(strMsg);
			return; 
		}

		int pos_ = FilePathName.ReverseFind('\\');

		m_projectLocation = FilePathName.Left(pos_);
		m_projectName = FilePathName.Mid(pos_+1, pos - 1 - pos_);

		CString cPath;
		cPath.Format(_T("%s\\%s.bla"), m_projectLocation, m_projectName);
		string path = CStringToString(cPath);

		ifstream inFile(path, ios::binary);
		if (!inFile.is_open())
		{
			CString strMsg;
			strMsg.Format (_T("打开项目文件\"%s\"失败"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//读取文件
		loadMaterial();
		load(inFile);
		

		m_bIsOpen = true;
		SetTitle(m_projectName);//设置文档名称
	}
	else
	{
		return;
	}
}

void CBuildLightAnalysisDoc::loadMaterial()
{
	CString path;
	path.Format(_T("%s\\Template_material.rad"), m_projectLocation);
	m_material.loadTemplate(path);
}

void CBuildLightAnalysisDoc::load(ifstream& inputFile)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//读取外墙
	pMain->GetOutWallProperty().load(inputFile);

	//读取内墙
	pMain->GetInWallProperty().load(inputFile);

	//读取处理后墙
	pMain->GetOptimizeWallProperty().load(inputFile);

	//读取窗户
	pMain->GetWindowProperty().load(inputFile);

	//读取房间信息
	pMain->GetRoomProperty().load(inputFile);

	//读取选项
	pMain->GetOptionProperty().load(inputFile);
	pMain->GetOptionProperty().loadMaterialTemplate();
	pMain->GetOptionProperty().SetTransform();
}
void CBuildLightAnalysisDoc::save(ofstream& outputFile)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//写入外墙
	pMain->GetOutWallProperty().save(outputFile);

	//写入内墙
	pMain->GetInWallProperty().save(outputFile);

	//写入处理后墙
	pMain->GetOptimizeWallProperty().save(outputFile);

	//写入窗户
	pMain->GetWindowProperty().save(outputFile);

	//写入房间信息
	pMain->GetRoomProperty().save(outputFile);

	//写入选项
	pMain->GetOptionProperty().save(outputFile);

}

void CBuildLightAnalysisDoc::OnFileOutput()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	if (!pMain)
		return;

	ofstream out("9p-noshelf_geometry.rad");
	if (!out.is_open())
	{
		return;
	}

	PropertyGridCtrl* pOption = pMain->GetOptionProperty().getPropList();
	PropertyGridCtrl* pRoomlist = pMain->GetRoomProperty().getPropList();
	CMFCPropertyGridProperty* optimizeOutWallPos = pMain->GetOptimizeWallProperty().getCoodOutWallGroup();
	CMFCPropertyGridProperty* optimizeInWallPos = pMain->GetOptimizeWallProperty().getCoodInWallGroup();
	PropertyGridCtrl* pWindowlist = pMain->GetWindowProperty().getPropList();

	double h = pOption->GetProperty(0)->GetValue().dblVal;
	for (int i = 0; i < pRoomlist->GetPropertyCount(); i++)
	{
		//导出第i个房间
		CMFCPropertyGridProperty* pRoom = pRoomlist->GetProperty(i);
		Wall wall;
		list<Wall> roomWalls;
		stWindow window;
		vector<stWindow> roomWindows;
		for (int j = 0; j < pRoom->GetSubItemsCount(); j++)
		{
			CString wallType = pRoom->GetSubItem(j)->GetName();
			if (wallType == _T("外墙号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				wall.line.s.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				wall.line.s.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				wall.line.e.x = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				wall.line.e.y = optimizeOutWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				wall.wallInfo.index = index;
				wall.wallInfo.type = sLine::OUT_WALL;
				wall.isOrder = true;
				roomWalls.push_back(wall);
			}
			else if (wallType == _T("内墙号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				wall.line.s.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				wall.line.s.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				wall.line.e.x = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				wall.line.e.y = optimizeInWallPos->GetSubItem(index)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				wall.wallInfo.index = index;
				wall.wallInfo.type = sLine::IN_WALL;
				wall.isOrder = true;
				roomWalls.push_back(wall);
			}
			else if (wallType == _T("窗户号"))
			{
				int index = pRoom->GetSubItem(j)->GetValue().intVal;
				stWindow win;
				CMFCPropertyGridProperty* pWin = pWindowlist->GetProperty(index);
				CString wallType = pWin->GetSubItem(0)->GetValue().bstrVal;
				_tcscpy(win.wallType, wallType);
				win.wallIndex = pWin->GetSubItem(1)->GetValue().intVal;
				win.pos = pWin->GetSubItem(2)->GetValue().dblVal;
				win.WinUpHeight = pWin->GetSubItem(3)->GetValue().dblVal;
				win.WinDownHeight = pWin->GetSubItem(4)->GetValue().dblVal;
				win.WinWidth = pWin->GetSubItem(5)->GetValue().dblVal;
				CString mat = pWin->GetSubItem(6)->GetValue().bstrVal;
				_tcscpy(win.WinMaterial, mat);

				roomWindows.push_back(win);
			}
					
		}
		vector<Vec2d> outPolygon;
		vector<Wall> outWalls;
		if (!CalClosedPolygon(roomWalls, outWalls, outPolygon))
		{
			CString msg;
			msg.Format(_T("房间%d没有封闭，不能导出"),i);
			AfxMessageBox(msg);
		}
		if (isAntiClock(outPolygon))//逆时针
		{
			//地面
			out << "room" << i << ".Floor";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(5)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = 0; j < outPolygon.size(); j++)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(6)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< h << endl;
			}
			out << endl;
			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				
				out << "room" << i << ".Flank" << j;
				out << " polygon " << CStringToString(mat) << endl; 
				out << "0 0 9 ";
				if (outWalls[j].isOrder)
				{
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
				}
				else
				{
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
				}
				out << endl;
			}
			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				out << "room" << i << ".Window" << j;
				out << " polygon " << CStringToString(CString(roomWindows[j].WinMaterial)) << endl; 
				out << "0 0 9 ";

				CMFCPropertyGridProperty* wallProperty = NULL;
				sLine::W_TYPE type;
				if (CString(roomWindows[j].wallType) == _T("外墙"))
				{
					wallProperty = optimizeOutWallPos;
					type = sLine::OUT_WALL;
				}
				else if (CString(roomWindows[j].wallType) = _T("内墙"))
				{
					wallProperty = optimizeInWallPos;
					type = sLine::IN_WALL;
				}
				bool isOrder = true;
				for (int k = 0; k < outWalls.size(); k++)
				{
					if (outWalls[k].wallInfo.type == type && outWalls[k].wallInfo.index == roomWindows[j].wallIndex)
					{
						isOrder = outWalls[k].isOrder;
						break;
					}
				}

				p.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				Vec2d dir = p1 - p;
				Vec2d pWinc = p + dir * roomWindows[j].pos;
				dir = dir / dir.Length();
				Vec2d pWins = pWinc - dir * roomWindows[j].WinWidth * 0.5;
				Vec2d pWine = pWinc + dir * roomWindows[j].WinWidth * 0.5;

				double hUp = roomWindows[j].WinUpHeight;


				if (isOrder)
				{
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				else
				{
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				out << endl;
			}
		}
		else
		{
			//地面
			out << "room" << i << ".Floor";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(5)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = outPolygon.size()-1; j >= 0 ; j--)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< 0.0 << endl;
			}
			out << endl;
			//棚顶
			out << "room" << i << ".Roof";
			out << " polygon " << CStringToString(CString(pOption->GetProperty(6)->GetValue().bstrVal)) << endl; 
			out << "0 0 9 ";
			for (int j = 0; j < outPolygon.size(); j++)
			{
				out << outPolygon[j].x << " " << outPolygon[j].y << " "<< h << endl;
			}
			out << endl;
			//侧墙
			for (int j = 0; j < outWalls.size(); j++)
			{
				CString mat;
				if (outWalls[j].wallInfo.type == sLine::OUT_WALL)
					mat = optimizeOutWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;
				else
					mat = optimizeInWallPos->GetSubItem(outWalls[j].wallInfo.index)->GetSubItem(2)->GetValue().bstrVal;

				out << "room" << i << ".Flank" << j;
				out << " polygon " << CStringToString(mat) << endl; 
				out << "0 0 9 ";
				if (!outWalls[j].isOrder)
				{
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
				}
				else
				{
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< h << endl;
					out << outWalls[j].line.s.x << " "<< outWalls[j].line.s.y << " "<< 0.0 << endl;
					out << outWalls[j].line.e.x << " "<< outWalls[j].line.e.y << " "<< 0.0 << endl;
				}
				out << endl;

			}

			//窗户
			Vec2d p,p1;
			for (int j = 0; j < roomWindows.size(); j ++)
			{
				out << "room" << i << ".Window" << j;
				out << " polygon " << CStringToString(CString(roomWindows[j].WinMaterial)) << endl; 
				out << "0 0 9 ";

				CMFCPropertyGridProperty* wallProperty = NULL;
				sLine::W_TYPE type;
				if (CString(roomWindows[j].wallType) == _T("外墙"))
				{
					wallProperty = optimizeOutWallPos;
					type = sLine::OUT_WALL;
				}
				else if (CString(roomWindows[j].wallType) = _T("内墙"))
				{
					wallProperty = optimizeInWallPos;
					type = sLine::IN_WALL;
				}
				bool isOrder = true;
				for (int k = 0; k < outWalls.size(); k++)
				{
					if (outWalls[k].wallInfo.type == type && outWalls[k].wallInfo.index == roomWindows[j].wallIndex)
					{
						isOrder = outWalls[k].isOrder;
						break;
					}
				}

				p.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(0)->GetValue().dblVal;
				p.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(0)->GetSubItem(1)->GetValue().dblVal;
				p1.x = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(0)->GetValue().dblVal;
				p1.y = wallProperty->GetSubItem(roomWindows[j].wallIndex)->GetSubItem(1)->GetSubItem(1)->GetValue().dblVal;
				Vec2d dir = p1 - p;
				Vec2d pWinc = p + dir * roomWindows[j].pos;
				dir = dir / dir.Length();
				Vec2d pWins = pWinc - dir * roomWindows[j].WinWidth * 0.5;
				Vec2d pWine = pWinc + dir * roomWindows[j].WinWidth * 0.5;

				double hUp = roomWindows[j].WinUpHeight;


				if (!isOrder)
				{
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				else
				{
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinUpHeight << endl;
					out << pWins.x << " "<< pWins.y << " "<< roomWindows[j].WinDownHeight << endl;
					out << pWine.x << " "<< pWine.y << " "<< roomWindows[j].WinDownHeight << endl;
				}
				out << endl;
			}
		}
	}
}
