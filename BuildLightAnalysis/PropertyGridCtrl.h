/**********************************************************************************************//**
 * \file	PropertyGridCtrl.h
 *
 * \brief	树状结构界面类拓展
 **************************************************************************************************/

#pragma once

class PropertyGridProperty : public CMFCPropertyGridProperty
{
	friend class PropertyGridCtrl;
public:
	// Group constructor
	PropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

	// Simple property
	PropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);

	/**********************************************************************************************//**
	 * \fn	CString PropertyGridProperty::FormatProperty();
	 *
	 * \brief	特殊处理浮点数的字符串化，精简有效数字显示
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \return	The formatted property.
	 **************************************************************************************************/

	CString FormatProperty();

	/**********************************************************************************************//**
	 * \fn	void PropertyGridProperty::RemoveAllSubItem();
	 *
	 * \brief	删除控件的所有子控件
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void RemoveAllSubItem();

	/**********************************************************************************************//**
	 * \fn	CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* PropertyGridProperty::GetSubItemList()
	 *
	 * \brief	获取子控件指针，主要用于遍历子控件
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \return	null if it fails, else the sub item list.
	 **************************************************************************************************/

	CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* GetSubItemList()
	{
		return &m_lstSubItems;
	}
};



class PropertyGridCtrl : public CMFCPropertyGridCtrl
{

	friend class PropertyGridProperty;

	// Construction
public:

	/**********************************************************************************************//**
	 * \fn	void PropertyGridCtrl::UpdateProperty(PropertyGridProperty* pProp);
	 *
	 * \brief	当动态添加树结构中的分支时，需要调用次函数保证子分支的结构更新到祖先中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pProp	If non-null, the property.
	 **************************************************************************************************/

	void UpdateProperty(PropertyGridProperty* pProp);

	/**********************************************************************************************//**
	 * \fn	void PropertyGridCtrl::ClearSel();
	 *
	 * \brief	清除当前选择的控件，变成未选择任何控件，主要用于删除控件时，被删控件已选择的情况
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void ClearSel();

	/**********************************************************************************************//**
	 * \fn	BOOL PropertyGridCtrl::EndEditItem(BOOL bUpdateData = TRUE);
	 *
	 * \brief	编辑控件后处理函数
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	bUpdateData	true to update data.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	BOOL EndEditItem(BOOL bUpdateData = TRUE);

	/**********************************************************************************************//**
	 * \fn	void PropertyGridCtrl::OnChangeSelection(CMFCPropertyGridProperty* , CMFCPropertyGridProperty* );
	 *
	 * \brief	修改选择的控件时调用
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	parameter1	If non-null, the first parameter.
	 * \param [in,out]	parameter2	If non-null, the second parameter.
	 **************************************************************************************************/

	void OnChangeSelection(CMFCPropertyGridProperty* /*pNewSel*/, CMFCPropertyGridProperty* /*pOldSel*/);
};

