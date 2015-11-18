/**********************************************************************************************//**
 * \file	OptionWnd.h
 *
 * \brief	选项界面
 **************************************************************************************************/

#include "PropertyGridCtrl.h"
#pragma once


enum
{
	OPTION_START = 0,
	OPTION_LEVEL_HEIGHT = OPTION_START,  //层高
	OPTION_OPTIMIZE_TH, //处理阈值，毫米
	OPTION_OUTWALL_MAT,	//外墙材质
	OPTION_INWALL_MAT,
	OPTION_WINDOW_MAT,
	OPTION_FLOOR_MAT,
	OPTION_ROOF_MAT,
	OPTION_PIX_MM_SCALE,
	OPTION_ORIGIN_X,
	OPTION_ORIGIN_Y,
	OPTION_OUTWALL_COLOR,
	OPTION_INWALL_COLOR,
	OPTION_KEYGRID_COLOR,
	OPTION_CITY,
	OPTION_NORTH,

	OPTION_NUM
};
enum
{
	OPTION_TYPE_INT = 0,
	OPTION_TYPE_DOUBLE,
	OPTION_TYPE_STRING,
	OPTION_TYPE_COLOR
};


class COptionWnd : public CDockablePane
{
public:
	
// Construction
public:
	COptionWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	PropertyGridCtrl* getPropList()
	{
		return &m_wndPropList;
	}

	int GetDataInt(int dataIndex)
	{
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			return data->GetValue().intVal;
		}
		return 0;
	}
	double GetDataDouble(int dataIndex)
	{
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			return data->GetValue().dblVal;
		}
		return 0.0;
	}
	CString GetDataCString(int dataIndex)
	{
		CString tmp;
		CMFCPropertyGridProperty* data = m_wndPropList.GetProperty(dataIndex);
		if (data)
		{
			tmp = data->GetValue().bstrVal;
		}
		return tmp;
	}

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::loadMaterialTemplateAndCity();
	 *
	 * \brief	导出材料模板和城市模板文件
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void loadMaterialTemplateAndCity();

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::GetTransform(double s, double centerX, double centerY);
	 *
	 * \brief	将变换参数记录到选项中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	s	   	scale
	 * \param	centerX	The center x coordinate.
	 * \param	centerY	The center y coordinate.
	 **************************************************************************************************/

	void GetTransform(double s, double centerX, double centerY);

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::SetTransform();
	 *
	 * \brief	从选项中年读取变换参数
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void SetTransform();

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::save(ofstream& out);
	 *
	 * \brief	保存选项
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::load(ifstream& in);
	 *
	 * \brief	记录选项
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	in	The in to load.
	 **************************************************************************************************/

	void load(ifstream& in);

	/**********************************************************************************************//**
	 * \fn	void COptionWnd::ResetAllOption();
	 *
	 * \brief	刪除所有记录
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void ResetAllOption();
	

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;

	int m_DataType[OPTION_NUM];

// Implementation
public:
	virtual ~COptionWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

