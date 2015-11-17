/**********************************************************************************************//**
 * \file	BuildLightAnalysisDoc.h
 *
 * \brief	Interface of the CBuildLightAnalysisDoc class
 **************************************************************************************************/

#pragma once

#include "Material.h"
#include "CommonData.h"
#include <map>


class CBuildLightAnalysisDoc : public CDocument
{
protected: // create from serialization only
	CBuildLightAnalysisDoc();
	DECLARE_DYNCREATE(CBuildLightAnalysisDoc)

// Attributes
public:
	CString m_projectName;//��������
	CString m_projectLocation;//����·��
	bool m_bIsOpen;//�����Ƿ��Ѿ���
public:
	vector<Material>& getMaterials()
	{
		return m_material.m_materials;
	}
	vector<CString>& getCitys()
	{
		return m_citys;
	}
	vector<RoomType>& getRoomTypes()
	{
		return m_roomTypes;
	}
	vector<LuminaireTem>& getLuminaireTems()
	{
		return m_LumTems;
	}
	vector<ControlSetTem>& getControlSetTems()
	{
		return m_ControlSetTems;
	}
	map<string, string>& getTranslate()
	{
		return m_translate;
	}
private:
	MaterialSet m_material;
	vector<CString> m_citys;
	vector<RoomType> m_roomTypes;
	vector<LuminaireTem> m_LumTems;
	vector<ControlSetTem> m_ControlSetTems;
	map<string, string> m_translate;
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CBuildLightAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::clear();
	 *
	 * \brief	�����������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/
	void clear();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::load(ifstream& in);
	 *
	 * \brief	��ȡ��������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 *
	 * \param [in,out]	in	The in to load.
	 **************************************************************************************************/
	void load(ifstream& in);

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadMaterial();
	 *
	 * \brief	��ȡ��������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadMaterial();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadCity();
	 *
	 * \brief	��ȡ��������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadCity();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadRoomType();
	 *
	 * \brief	��ȡ����������Ϣ
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadRoomType();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadLumTems();
	 *
	 * \brief	��ȡ�ƾ�ģ������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadLumTems();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadControlSetTems();
	 *
	 * \brief	��ȡ���Ʒ���ģ��
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadControlSetTems();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::save(ofstream& out);
	 *
	 * \brief	���湤������
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileSave();
	 *
	 * \brief	��Ӧ�˵��еı��水ť�����й������ݣ�.bla���ı���
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileSave();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileSaveAs();
	 *
	 * \brief	��Ӧ�˵��е����Ϊ��ť
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileSaveAs();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileNew();
	 *
	 * \brief	��Ӧ�˵��е��½���ť������һ���µĹ���
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileNew();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileOpen();
	 *
	 * \brief	��Ӧ�˵��еĴ򿪰�ť���򿪹�������(.bla)
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileOpen();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileOutput();
	 *
	 * \brief	��Ӧ�˵��еĵ�����ť������.rad, .pts, .mtl, .gx, .occ, .txt�ļ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileOutput();
};
