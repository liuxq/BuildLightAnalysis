#pragma once

class PropertyGridProperty : public CMFCPropertyGridProperty
{
	friend class PropertyGridCtrl;
};

class PropertyGridCtrl : public CMFCPropertyGridCtrl
{

	friend class PropertyGridProperty;

	// Construction
public:
	void UpdateProperty(PropertyGridProperty* pProp);
	BOOL EndEditItem(BOOL bUpdateData = TRUE);
};

