#include "stdafx.h"
#include "PropertyGridCtrl.h"
#include "MainFrm.h"

void PropertyGridCtrl::UpdateProperty(PropertyGridProperty* pProp)
{
	pProp->SetOwnerList(this);
}

BOOL PropertyGridCtrl::EndEditItem(BOOL bUpdateData)
{  
	CMFCPropertyGridCtrl::EndEditItem(bUpdateData);

	//�༭���Ժ��ػ�����ͼ
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd; 
	if (pMain && pMain->GetActiveView())
	{
		pMain->GetActiveView()->Invalidate(); 
	}
	
	return TRUE;  
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

	if (var.vt == VT_R8)//���������⴦����ʾ��ʽ
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