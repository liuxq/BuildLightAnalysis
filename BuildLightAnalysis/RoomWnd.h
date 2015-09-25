#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"
enum
{
	ROOM_TYPE = 0,
	ROOM_HEIGHT,
	ROOM_OUT_WALL,
	ROOM_IN_WALL,
	ROOM_WINDOW,
	ROOM_GRID
};
enum
{
	GRID_OFFSET,
	GRID_MESHLEN,
	GRID_POINTS
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
};

