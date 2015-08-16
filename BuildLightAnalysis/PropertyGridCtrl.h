
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
};

