/**********************************************************************************************//**
 * \file	PropertyGridCtrl.h
 *
 * \brief	��״�ṹ��������չ
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
	 * \brief	���⴦���������ַ�������������Ч������ʾ
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
	 * \brief	ɾ���ؼ��������ӿؼ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void RemoveAllSubItem();

	/**********************************************************************************************//**
	 * \fn	CList<CMFCPropertyGridProperty*, CMFCPropertyGridProperty*>* PropertyGridProperty::GetSubItemList()
	 *
	 * \brief	��ȡ�ӿؼ�ָ�룬��Ҫ���ڱ����ӿؼ�
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
	 * \brief	����̬������ṹ�еķ�֧ʱ����Ҫ���ôκ�����֤�ӷ�֧�Ľṹ���µ�������
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
	 * \brief	�����ǰѡ��Ŀؼ������δѡ���κοؼ�����Ҫ����ɾ���ؼ�ʱ����ɾ�ؼ���ѡ������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void ClearSel();

	/**********************************************************************************************//**
	 * \fn	BOOL PropertyGridCtrl::EndEditItem(BOOL bUpdateData = TRUE);
	 *
	 * \brief	�༭�ؼ�������
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
	 * \brief	�޸�ѡ��Ŀؼ�ʱ����
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	parameter1	If non-null, the first parameter.
	 * \param [in,out]	parameter2	If non-null, the second parameter.
	 **************************************************************************************************/

	void OnChangeSelection(CMFCPropertyGridProperty* /*pNewSel*/, CMFCPropertyGridProperty* /*pOldSel*/);
};

