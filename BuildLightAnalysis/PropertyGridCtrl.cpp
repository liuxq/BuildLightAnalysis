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