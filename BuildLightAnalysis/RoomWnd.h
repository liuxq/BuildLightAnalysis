/**********************************************************************************************//**
 * \file	RoomWnd.h
 *
 * \brief	房间界面类
 **************************************************************************************************/

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
//需要记录data的一些特殊控件，用于拾取时的识别
enum
{
	ROOM_SINGLE_LUMINAIRE_DATA = 100,
	ROOM_SET_LUMINAIRE_DATA,
	LUM_SINGLE_TYPE_DATA,
	LUM_SET_TYPE_DATA,
	ROOM_CONTROL_SET_DATA,
	ROOM_E_DATA,
	CONTROL_SET_LUMSET_DATA,
	CONTROL_SINGLE_LUMSET_DATA,
	PERSON_CONTROL_SET_DATA,
	PERSON_DATA,
};
//菜单
enum
{
	MENU_CONTROL_SET0 = 2000,
	MENU_DEDETE,
};
//计算点
enum
{
	GRID_OFFSET,
	GRID_MESHLEN,
	GRID_HEIGHT,
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
	CONTROL_SET_ARGS_KEYGRID,//特殊！由于参数数量可变，用于设置data，不是索引
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

	/**********************************************************************************************//**
	 * \fn	PropertyGridProperty* CRoomWnd::AddRoom(CString);
	 *
	 * \brief	添加一个房间，设置好房间的一些默认参数，
	 * 			并将刚添加的房价设置为已选择
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	添加的房间名称
	 *
	 * \return	返回房间的控件指针
	 **************************************************************************************************/

	PropertyGridProperty* AddRoom(CString);

	/**********************************************************************************************//**
	 * \fn	bool CRoomWnd::AddToSelectedRoom(int type, long index);
	 *
	 * \brief	将墙或窗户加入到房间中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	type 	添加的类型 1：外墙， 2：内墙， 3：窗户
	 * \param	index	索引
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool AddToSelectedRoom(int type, long index);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteWindowFromRoom(int winIndex);
	 *
	 * \brief	删除房间中的指定窗户
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	winIndex	窗户编号
	 **************************************************************************************************/

	void DeleteWindowFromRoom(int winIndex);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteAllRoom();
	 *
	 * \brief	删除所有房间
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllRoom();

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::CalGrid(CMFCPropertyGridProperty* pGrid);
	 *
	 * \brief	计算得到计算网格
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pGrid	If non-null, the grid.
	 **************************************************************************************************/

	void CalGrid(CMFCPropertyGridProperty* pGrid);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire,Vec3d p, WCHAR type[80] = _T("FAC21280P-23W"), double lm = 2100.0, double w = 23.0 ,Vec3d np = Vec3d(0,0,-1));
	 *
	 * \brief	添加一个单个灯具
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLuminaire	灯具控件指针
	 * \param	p				  	灯具位置
	 * \param	type			  	灯具类型
	 * \param	lm				  	灯具照度
	 * \param	w				  	灯具功率
	 * \param	np				  	灯具法矢
	 **************************************************************************************************/

	void AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire,Vec3d p, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0,	double w = 23.0 ,Vec3d np = Vec3d(0,0,-1));

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, Vec2d originP, double z, WCHAR type[80] = _T("FAC21280P-23W"), double lm = 2100.0, double w = 23.0, Vec3d np = Vec3d(0,0,-1), int rowN = 5,int colN = 5,double rowL = 200.0,double colL = 200.0);
	 *
	 * \brief	添加一组灯具
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLuminaire	灯具组控件指针
	 * \param	originP			  	初始点位置
	 * \param	z				  	The z coordinate.
	 * \param	type			  	The type.
	 * \param	lm				  	照度
	 * \param	w				  	The width.
	 * \param	np				  	法矢
	 * \param	rowN			  	行数
	 * \param	colN			  	列数
	 * \param	rowL			  	行宽
	 * \param	colL			  	列宽
	 **************************************************************************************************/

	void AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, Vec2d originP, double z, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0, double w = 23.0, Vec3d np = Vec3d(0,0,-1),
		int rowN = 5,int colN = 5,double rowL = 200.0,double colL = 200.0);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddControlSet(CMFCPropertyGridProperty* pControlSet, WCHAR type[80] = _T("mannual_on_auto_off"), int keyGrid = -1);
	 *
	 * \brief	添加控制分组
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pControlSet	If non-null, set the control belongs to.
	 * \param	type			   	The type.
	 * \param	keyGrid			   	The key grid.
	 **************************************************************************************************/

	void AddControlSet(CMFCPropertyGridProperty* pControlSet, WCHAR type[80] = _T("mannual_on_auto_off"), int keyGrid = -1);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddPerson(CMFCPropertyGridProperty* pPerson, WCHAR schedule_type[80] = _T("Management"), WCHAR behavior_type[80] = _T("Economic_type"));
	 *
	 * \brief	添加人员
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pPerson	If non-null, the person.
	 * \param	schedule_type  	Type of the schedule.
	 * \param	behavior_type  	Type of the behavior.
	 **************************************************************************************************/

	void AddPerson(CMFCPropertyGridProperty* pPerson, WCHAR schedule_type[80] = _T("Management"),  WCHAR behavior_type[80] = _T("Economic_type"));

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::UpdateRoomE(CMFCPropertyGridProperty* pRoom);
	 *
	 * \brief	更新房间照度
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 **************************************************************************************************/

	void UpdateRoomE(CMFCPropertyGridProperty* pRoom);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::UpdateLumArgs(CMFCPropertyGridProperty* pLum);
	 *
	 * \brief	更新灯具信息
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLum	If non-null, the lum.
	 **************************************************************************************************/

	void UpdateLumArgs(CMFCPropertyGridProperty* pLum);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::UpdateControlSetArgs(CMFCPropertyGridProperty* pControl, int keyGrid);
	 *
	 * \brief	更新控制分组参数
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pControl	If non-null, the control.
	 * \param	keyGrid				The key grid.
	 **************************************************************************************************/

	void UpdateControlSetArgs(CMFCPropertyGridProperty* pControl, int keyGrid);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::GetKeyGrid(CMFCPropertyGridProperty* pRoom, vector<int>& keys);
	 *
	 * \brief	取得房间内所有关键点
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param [in,out]	keys 	The keys.
	 **************************************************************************************************/

	void GetKeyGrid(CMFCPropertyGridProperty* pRoom, vector<int>& keys);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::CalLumSet(CMFCPropertyGridProperty* pLum);
	 *
	 * \brief	设置灯具组
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLum	If non-null, the lum.
	 **************************************************************************************************/

	void CalLumSet(CMFCPropertyGridProperty* pLum);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::CalMinXY(CMFCPropertyGridProperty* pRoom, Vec2d& minP);
	 *
	 * \brief	计算房间的左下角坐标
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param [in,out]	minP 	The minimum p.
	 **************************************************************************************************/

	void CalMinXY(CMFCPropertyGridProperty* pRoom, Vec2d& minP);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::CalMidXY(CMFCPropertyGridProperty* pRoom, Vec2d& midP);
	 *
	 * \brief	计算房间的中心坐标
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param [in,out]	midP 	The middle p.
	 **************************************************************************************************/

	void CalMidXY(CMFCPropertyGridProperty* pRoom, Vec2d& midP);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::OutputToRooms(vector<Room>& rooms);
	 *
	 * \brief	导出房间信息到集合中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	rooms	The rooms.
	 **************************************************************************************************/

	void OutputToRooms(vector<Room>& rooms);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::OutputToLums(vector<vector<OutLumSingle>>& lumSingles, vector<vector<OutLumSet>>& lumSets, vector<vector<OutControlSet>>& controlSets, vector<vector<OutPerson>>& persons);
	 *
	 * \brief	导出灯具信息到集合中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	lumSingles 	The lum singles.
	 * \param [in,out]	lumSets	   	Sets the lum belongs to.
	 * \param [in,out]	controlSets	Sets the control belongs to.
	 * \param [in,out]	persons	   	The persons.
	 **************************************************************************************************/

	void OutputToLums(vector<vector<OutLumSingle>>& lumSingles, vector<vector<OutLumSet>>& lumSets,
		vector<vector<OutControlSet>>& controlSets, vector<vector<OutPerson>>& persons);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteLumByIndex(CMFCPropertyGridProperty* pRoom, int index, bool isSingle);
	 *
	 * \brief	按照灯具索引删除灯具
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param	index		 	灯具索引
	 * \param	isSingle	 	是否是单个灯具
	 **************************************************************************************************/

	void DeleteLumByIndex(CMFCPropertyGridProperty* pRoom, int index, bool isSingle);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteControSetByIndex(CMFCPropertyGridProperty* pRoom, int index);
	 *
	 * \brief	按照索引删除控制分组
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param	index		 	Zero-based index of the.
	 **************************************************************************************************/

	void DeleteControSetByIndex(CMFCPropertyGridProperty* pRoom, int index);

	/**********************************************************************************************//**
	* \fn	void CRoomWnd::DeleteControSetByIndex(CMFCPropertyGridProperty* pRoom, int index);
	*
	* \brief	按照索引删除人员
	*
	* \author	Liuxq
	* \date	2015/11/18
	*
	* \param [in,out]	pRoom	If non-null, the room.
	* \param	index		 	Zero-based index of the.
	**************************************************************************************************/

	void DeletePersonByIndex(CMFCPropertyGridProperty* pRoom, int index);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::save(ofstream& out);
	 *
	 * \brief	保存房间信息到工程文件中
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	out	The out to save.
	 **************************************************************************************************/

	void save(ofstream& out);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::load(ifstream& in);
	 *
	 * \brief	从工程文件导入房间信息
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	in	The in to load.
	 **************************************************************************************************/

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
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnNewRoom();
	 *
	 * \brief	对应新建房间按钮
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnNewRoom();
	afx_msg void OnUpdateButton(CCmdUI* pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnDeleteRoom();
	 *
	 * \brief	对应删除房间按钮
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnDeleteRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg LRESULT CRoomWnd::OnPropertyChanged(WPARAM,LPARAM);
	 *
	 * \brief	当房间的属性改变时调用
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	The wParam field of the message.
	 * \param	parameter2	The lParam field of the message.
	 *
	 * \return	A LRESULT.
	 **************************************************************************************************/

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnContextMenu(CWnd* pWnd, CPoint point);
	 *
	 * \brief	房间界面的右键菜单处理函数
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pWnd	If non-null, the window.
	 * \param	point			鼠标点击点
	 **************************************************************************************************/

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
public:
	afx_msg void OnRoomCalGrid();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddLuminaireSingle();
	 *
	 * \brief	对应添加单个灯具菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSingle();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddLuminaireSet();
	 *
	 * \brief	对应添加灯具组菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddControlSet();
	 *
	 * \brief	对应添加控制分组菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddControlSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddPerson();
	 *
	 * \brief	对应添加人员菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddPerson();
};

