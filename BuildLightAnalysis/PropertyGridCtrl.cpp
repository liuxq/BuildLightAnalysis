#include "stdafx.h"
#include "PropertyGridCtrl.h"
#include "MainFrm.h"

void PropertyGridCtrl::UpdateProperty(PropertyGridProperty* pProp)
{
	pProp->SetOwnerList(this);
}
void PropertyGridCtrl::ClearSel()
{
	m_pSel = NULL;
}

BOOL PropertyGridCtrl::EndEditItem(BOOL bUpdateData)
{  
	CMFCPropertyGridCtrl::EndEditItem(bUpdateData);

	//编辑属性后，重绘主视图
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if (pMain && pMain->GetActiveView())
	{
		pMain->GetActiveView()->Invalidate(); 
	}
	
	return TRUE;  
}  

void PropertyGridCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel) 
{
	while (pNewSel && pNewSel->GetParent())
	{
		pNewSel = pNewSel->GetParent();
	}
	if (pNewSel)
	{
		CString name = pNewSel->GetName();
		if (name == _T("处理后外墙") || name == _T("处理后内墙"))
		{
			//编辑属性后，重绘主视图
			CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd; 
			if (pMain && pMain->GetActiveView())
			{
				pMain->GetActiveView()->Invalidate(); 
			}
		}
	}
}

PropertyGridProperty::PropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyGridProperty(strName,varValue,lpszDescr,dwData,lpszEditMask,lpszEditTemplate, lpszValidChars)
{
	
}

PropertyGridProperty::PropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	:CMFCPropertyGridProperty(strGroupName,dwData,bIsValueList)
{
	
}


CString PropertyGridProperty::FormatProperty()
{
	COleVariant& var = m_varValue;
	CString strVal = CMFCPropertyGridProperty::FormatProperty();

	if (var.vt == VT_R8)//浮点型特殊处理显示格式
	{
		int i = strVal.GetLength()-1;
		while(i > 2 && strVal[i] == '0')
			i--;
		strVal = strVal.Left(i+1);
		if (strVal[strVal.GetLength()-1] == '.')
			strVal = strVal.Left(strVal.GetLength()-1);
	}
	return strVal;
}

void PropertyGridProperty::RemoveAllSubItem()
{
	for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
	{
		POSITION posSaved = pos;

		CMFCPropertyGridProperty* pListProp = m_lstSubItems.GetNext(pos);
		ASSERT_VALID(pListProp);

		m_lstSubItems.RemoveAt(posSaved);
		delete pListProp;
	}
	((PropertyGridCtrl*)m_pWndList)->ClearSel();

}
