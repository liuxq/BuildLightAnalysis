#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"

//总目录
enum
{
	ROOM_TYPE = 0,
	ROOM_E,
	ROOM_HEIGHT,
	ROOM_OUT_WALL,
	ROOM_IN_WALL,
	ROOM_WINDOW,
	ROOM_GRID,
	ROOM_SINGLE_LUMINAIRE,
	ROOM_SET_LUMINAIRE,
	ROOM_CONTROL_SET,
	ROOM_PERSON,
};
enum
{
	ROOM_SINGLE_LUMINAIRE_DATA = 100,
	ROOM_SET_LUMINAIRE_DATA = 101,
	LUM_SINGLE_TYPE_DATA = 102,
	LUM_SET_TYPE_DATA = 103,
	ROOM_E_DATA = 104
};
//菜单
enum
{
	MENU_CONTROL_SET0 = 2000,
};
//计算点
enum
{
	GRID_OFFSET,
	GRID_MESHLEN,
	GRID_POINTS
};

//单个灯具
enum
{
	LUM_SINGLE_TYPE,
	LUM_SINGLE_LM,
	LUM_SINGLE_W,
	LUM_SINGLE_X,
	LUM_SINGLE_Y,
	LUM_SINGLE_Z,
	LUM_SINGLE_NX,
	LUM_SINGLE_NY,
	LUM_SINGLE_NZ,
};
//灯具组
enum
{
	LUM_SET_TYPE,
	LUM_SET_LM,
	LUM_SET_W,
	LUM_SET_Z,
	LUM_SET_NX,
	LUM_SET_NY,
	LUM_SET_NZ,
	LUM_SET_ORIGIN_X,
	LUM_SET_ORIGIN_Y,
	LUM_SET_ROW_N,
	LUM_SET_COL_N,
	LUM_SET_ROW_L,
	LUM_SET_COL_L,
	LUM_SET_POINTS
};
//控制分组
enum
{
	CONTROL_SET_LUM,
	CONTROL_SET_TYPE,

	CONTROL_SET_ARGS = 1001,//特殊！由于参数数量可变，用于设置data，不是索引
};

//人员
enum
{
	PERSON_SCHEDULE_TYPE,
	PERSON_BEHAVIOR_TYPE,
	PERSON_CONTROL_SET
};


class CRoomWnd : public CDockablePane
{
// Construction
public:
	CRoomWnd();

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

	PropertyGridProperty* AddRoom(CString);
	bool AddToSelectedRoom(int type, int index);
	void DeleteWindowFromRoom(int winIndex);
	void DeleteAllRoom();
	CMFCPropertyGridProperty* InsertGrid(double offset, double meshLen);
	void CalGrid(CMFCPropertyGridProperty* pGrid);
	void AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire,Vec3d p, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0,	double w = 23.0 ,Vec3d np = Vec3d(0,0,-1));
	void AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, Vec2d originP, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0, double w = 23.0, double z = 750.0, Vec3d np = Vec3d(0,0,-1),
		int rowN = 5,int colN = 5,double rowL = 200.0,double colL = 200.0);
	void AddControlSet(CMFCPropertyGridProperty* pControlSet, WCHAR type[80] = _T("mannual_on_auto_off"), int keyGrid = -1);
	void AddPerson(CMFCPropertyGridProperty* pPerson, WCHAR schedule_type[80] = _T("行政"),  WCHAR behavior_type[80] = _T("经济型"));

	void UpdateRoomE(CMFCPropertyGridProperty* pRoom);
	void UpdateLumArgs(CMFCPropertyGridProperty* pLum);
	void UpdateControlSetArgs(CMFCPropertyGridProperty* pControl, int keyGrid);

	void GetKeyGrid(CMFCPropertyGridProperty* pRoom, vector<int>& keys);
	void CalLumSet(CMFCPropertyGridProperty* pLum);
	void CalMinXY(CMFCPropertyGridProperty* pRoom, Vec2d& minP);
	void CalMidXY(CMFCPropertyGridProperty* pRoom, Vec2d& midP);

	void OutputToRooms(vector<Room>& rooms);
	void OutputToLums(vector<vector<OutLumSingle>>& lumSingles, vector<vector<OutLumSet>>& lumSets,
		vector<vector<OutControlSet>>& controlSets, vector<vector<OutPerson>>& persons);

	void save(ofstream& out);
	void load(ifstream& in);

protected:
	CFont m_fntPropList;
	PropertyGridCtrl m_wndPropList;
	CButton m_insertButton;
	CButton m_deleteButton;
// Implementation
public:
	virtual ~CRoomWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnNewRoom();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnDeleteRoom();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
public:
	afx_msg void OnRoomCalGrid();

	afx_msg void OnRoomAddLuminaireSingle();
	afx_msg void OnRoomAddLuminaireSet();
	afx_msg void OnRoomAddControlSet();
	afx_msg void OnRoomAddPerson();
};

