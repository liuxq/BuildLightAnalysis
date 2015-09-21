
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
			strMsg.Format (_T("������Ŀ�ļ�\"%s\"ʧ��"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//д���ļ�
		save(outFile);
	}	
}


void CBuildLightAnalysisDoc::OnFileSaveAs()
{
	CString FilePathName;
	CFileDialog dlg(FALSE, //FALSEΪSAVE AS�Ի���
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BLA Files (*.bla)|*.bla|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��
		int pos = FilePathName.ReverseFind('.');
		CString lxq = FilePathName.Mid(pos+1);
		if (pos == -1 || FilePathName.Mid(pos+1) != _T("bla"))
		{
			CString strMsg;
			strMsg.Format (_T("������Ŀ�ļ�\"%s\"ʧ�ܣ���׺����"), FilePathName); 
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
			strMsg.Format (_T("������Ŀ�ļ�\"%s\"ʧ��"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//д���ļ�
		save(outFile);
		SetTitle(m_projectName);//�����ĵ�����
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
		SetTitle(m_projectName);//�����ĵ�����

		//�������
		clear();
		//��ȡѡ��
		CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
		//��ȡ����
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
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BLA Files (*.bla)|*.bla|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��


		int pos = FilePathName.ReverseFind('.');
		CString lxq = FilePathName.Mid(pos+1);
		if (pos == -1 || FilePathName.Mid(pos+1) != _T("bla"))
		{
			CString strMsg;
			strMsg.Format (_T("����Ŀ�ļ�\"%s\"ʧ�ܣ���׺����"), FilePathName); 
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
			strMsg.Format (_T("����Ŀ�ļ�\"%s\"ʧ��"), cPath); 
			AfxMessageBox(strMsg);
			return; 
		}
		//��ȡ�ļ�
		loadMaterial();
		loadCity();
		loadRoomType();
		load(inFile);
		

		m_bIsOpen = true;
		SetTitle(m_projectName);//�����ĵ�����
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
		AfxMessageBox(_T("ȱ�ٳ���ģ�壬�뽫city_list.txt�ļ����빤���ļ�����"));
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
		AfxMessageBox(_T("ȱ�ٷ�������ģ�壬�뽫room_type.txt�ļ����빤���ļ�����"));
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
	//�����ǽ
	pMain->GetOutWallProperty().DeleteAllPos();

	//�����ǽ
	pMain->GetInWallProperty().DeleteAllPos();

	//��������ǽ
	pMain->GetOptimizeWallProperty().DeleteAllPos();

	//�������
	pMain->GetWindowProperty().DeleteAllWindow();

	//���������Ϣ
	pMain->GetRoomProperty().DeleteAllRoom();

	//���ѡ��
	pMain->GetOptionProperty().ResetAllOption();
	pMain->GetOptionProperty().loadMaterialTemplateAndCity();
	pMain->GetOptionProperty().SetTransform();
}
void CBuildLightAnalysisDoc::load(ifstream& inputFile)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//��ȡ��ǽ
	pMain->GetOutWallProperty().load(inputFile);

	//��ȡ��ǽ
	pMain->GetInWallProperty().load(inputFile);

	//��ȡ�����ǽ
	pMain->GetOptimizeWallProperty().load(inputFile);

	//��ȡ����
	pMain->GetWindowProperty().load(inputFile);

	//��ȡ������Ϣ
	pMain->GetRoomProperty().load(inputFile);

	//��ȡģʽ
	pMain->loadMode(inputFile);

	//��ȡѡ��
	pMain->GetOptionProperty().load(inputFile);

	//һЩ����
	pMain->GetOptionProperty().loadMaterialTemplateAndCity();
	pMain->GetOptionProperty().SetTransform();
}
void CBuildLightAnalysisDoc::save(ofstream& outputFile)
{
	CMainFrame *pMain =(CMainFrame*)AfxGetMainWnd();
	//д����ǽ
	pMain->GetOutWallProperty().save(outputFile);

	//д����ǽ
	pMain->GetInWallProperty().save(outputFile);

	//д�봦���ǽ
	pMain->GetOptimizeWallProperty().save(outputFile);

	//д�봰��
	pMain->GetWindowProperty().save(outputFile);

	//д�뷿����Ϣ
	pMain->GetRoomProperty().save(outputFile);

	//��ȡģʽ
	pMain->saveMode(outputFile);

	//д��ѡ��
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

	AfxMessageBox(_T("�����ɹ�"));
}
