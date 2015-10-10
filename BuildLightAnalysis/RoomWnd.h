#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"

//总目录
enum
{
	ROOM_TYPE = 0,
	ROOM_HEIGHT,
	ROOM_OUT_WALL,
	ROOM_IN_WALL,
	ROOM_WINDOW,
	ROOM_GRID,
	ROOM_SINGLE_LUMINAIRE,
	ROOM_SET_LUMINAIRE,
	ROOM_CONTROL_SET
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
	void AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire, double x, double y);
	void AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, double x, double y);
	void AddControlSet(CMFCPropertyGridProperty* pControlSet);
	void UpdateControlSetArgs(CMFCPropertyGridProperty* pControl);

	void CalLumSet(CMFCPropertyGridProperty* pLum);
	void CalMinXY(CMFCPropertyGridProperty* pRoom, Vec2d& minP);
	void CalMidXY(CMFCPropertyGridProperty* pRoom, Vec2d& midP);

	void OutputToRooms(vector<Room>& rooms);

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
};

