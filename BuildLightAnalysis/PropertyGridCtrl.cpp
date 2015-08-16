#include "stdafx.h"
#include "PropertyGridCtrl.h"

void PropertyGridCtrl::UpdateProperty(PropertyGridProperty* pProp)
{
	pProp->SetOwnerList(this);
}