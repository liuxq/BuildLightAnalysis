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
	CString FormatProperty();
};



class PropertyGridCtrl : public CMFCPropertyGridCtrl
{

	friend class PropertyGridProperty;

	// Construction
public:
	void UpdateProperty(PropertyGridProperty* pProp);
	BOOL EndEditItem(BOOL bUpdateData = TRUE);
};

