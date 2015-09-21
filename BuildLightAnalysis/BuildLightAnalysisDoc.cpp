
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
#include "ProjectOutput.h"
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

		//清空数据
		clear();
		//读取选项
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		//读取材料
		loadMaterial();
		loadCity();
		loadRoomType();
		pMain->GetOptionProperty().loadMaterialTemplateAndCity();
		pMain->GetOptionProperty().SetTransform();

		OnFileSave();
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
		loadCity();
		loadRoomType();
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
void CBuildLightAnalysisDoc::loadCity()
{
	CString path;
	path.Format(_T("%s\\city_list.txt"), m_projectLocation);
	ifstream inputFile(CStringToString(path));
	if (!inputFile.is_open())
	{
		AfxMessageBox(_T("缺少城市模板，请将city_list.txt文件放入工程文件夹下"));
		return;
	}
	string name;
	while(inputFile >> name)
	{
		m_citys.push_back(StringToCString(name));
	}
}
void CBuildLightAnalysisDoc::loadRoomType()
{
	CString path;
	path.Format(_T("%s\\room_type.txt"), m_projectLocation);
	ifstream inputFile(CStringToString(path));
	if (!inputFile.is_open())
	{
		AfxMessageBox(_T("缺少房间类型模板，请将room_type.txt文件放入工程文件夹下"));
		return;
	}
	string name;
	int num;
	while(inputFile >> name >> num )
	{
		RoomType rt;
		CString nameCs = StringToCString(name);
		_tcscpy_s(rt.name, nameCs);
		rt.num = num;
		m_roomTypes.push_back(rt);
	}
}
void CBuildLightAnalysisDoc::clear()
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//清除外墙
	pMain->GetOutWallProperty().DeleteAllPos();

	//清除内墙
	pMain->GetInWallProperty().DeleteAllPos();

	//清除处理后墙
	pMain->GetOptimizeWallProperty().DeleteAllPos();

	//清除窗户
	pMain->GetWindowProperty().DeleteAllWindow();

	//清除房间信息
	pMain->GetRoomProperty().DeleteAllRoom();

	//清除选项
	pMain->GetOptionProperty().ResetAllOption();
	pMain->GetOptionProperty().loadMaterialTemplateAndCity();
	pMain->GetOptionProperty().SetTransform();
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

	//读取模式
	pMain->loadMode(inputFile);

	//读取选项
	pMain->GetOptionProperty().load(inputFile);

	//一些设置
	pMain->GetOptionProperty().loadMaterialTemplateAndCity();
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

	//读取模式
	pMain->saveMode(outputFile);

	//写入选项
	pMain->GetOptionProperty().save(outputFile);

}

void CBuildLightAnalysisDoc::OnFileOutput()
{
	OnFileSave();

	set<CString> mats;
	geometryOutput(CStringToString(m_projectLocation) + "\\"+ CStringToString(m_projectName) + "_geometry.rad", mats);
	materialOutput(CStringToString(m_projectLocation) + "\\"+ CStringToString(m_projectName) + "_material.rad", m_material, mats);
	string grid1file = CStringToString(m_projectLocation) + "\\"+ CStringToString(m_projectName) + "_grid1.pts";
	string grid2file = CStringToString(m_projectLocation) + "\\"+ CStringToString(m_projectName) + "_grid2.pts";
	RoomOutput(CStringToString(m_projectLocation) + "\\"+ CStringToString(m_projectName) +"_room_info.txt", grid1file, grid2file);

	AfxMessageBox(_T("导出成功"));
}
