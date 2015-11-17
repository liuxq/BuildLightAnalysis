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
	CString m_projectName;//工程名字
	CString m_projectLocation;//工程路径
	bool m_bIsOpen;//工程是否已经打开
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
	 * \brief	清除工程数据
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/
	void clear();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::load(ifstream& in);
	 *
	 * \brief	读取工程数据
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
	 * \brief	读取材质数据
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadMaterial();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadCity();
	 *
	 * \brief	读取城市数据
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadCity();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadRoomType();
	 *
	 * \brief	读取房间类型信息
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadRoomType();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadLumTems();
	 *
	 * \brief	读取灯具模板数据
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadLumTems();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::loadControlSetTems();
	 *
	 * \brief	读取控制分组模板
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	void loadControlSetTems();

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisDoc::save(ofstream& out);
	 *
	 * \brief	保存工程数据
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
	 * \brief	对应菜单中的保存按钮，进行工程数据（.bla）的保存
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileSave();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileSaveAs();
	 *
	 * \brief	对应菜单中的另存为按钮
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileSaveAs();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileNew();
	 *
	 * \brief	对应菜单中的新建按钮，创建一个新的工程
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileNew();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileOpen();
	 *
	 * \brief	对应菜单中的打开按钮，打开工程数据(.bla)
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileOpen();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisDoc::OnFileOutput();
	 *
	 * \brief	对应菜单中的导出按钮，导出.rad, .pts, .mtl, .gx, .occ, .txt文件
	 *
	 * \author	Liuxq
	 * \date	2015/11/17
	 **************************************************************************************************/

	afx_msg void OnFileOutput();
};
