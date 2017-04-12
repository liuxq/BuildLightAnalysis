/**********************************************************************************************//**
 * \file	RoomWnd.h
 *
 * \brief	���������
 **************************************************************************************************/

#pragma once

#include "PropertyGridCtrl.h"
#include "CommonData.h"

//��Ŀ¼
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
//��Ҫ��¼data��һЩ����ؼ�������ʰȡʱ��ʶ��
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
//�˵�
enum
{
	MENU_CONTROL_SET0 = 2000,
	MENU_DEDETE,
};
//�����
enum
{
	GRID_OFFSET,
	GRID_MESHLEN,
	GRID_HEIGHT,
	GRID_POINTS
};

//�����ƾ�
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
//�ƾ���
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
//���Ʒ���
enum
{
	CONTROL_SET_LUM,
	CONTROL_SET_TYPE,

	CONTROL_SET_ARGS = 1001,//���⣡���ڲ��������ɱ䣬��������data����������
	CONTROL_SET_ARGS_KEYGRID,//���⣡���ڲ��������ɱ䣬��������data����������
};

//��Ա
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
	 * \brief	���һ�����䣬���ú÷����һЩĬ�ϲ�����
	 * 			��������ӵķ�������Ϊ��ѡ��
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	��ӵķ�������
	 *
	 * \return	���ط���Ŀؼ�ָ��
	 **************************************************************************************************/

	PropertyGridProperty* AddRoom(CString);

	/**********************************************************************************************//**
	 * \fn	bool CRoomWnd::AddToSelectedRoom(int type, long index);
	 *
	 * \brief	��ǽ�򴰻����뵽������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	type 	��ӵ����� 1����ǽ�� 2����ǽ�� 3������
	 * \param	index	����
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	bool AddToSelectedRoom(int type, long index);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteWindowFromRoom(int winIndex);
	 *
	 * \brief	ɾ�������е�ָ������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	winIndex	�������
	 **************************************************************************************************/

	void DeleteWindowFromRoom(int winIndex);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteAllRoom();
	 *
	 * \brief	ɾ�����з���
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void DeleteAllRoom();

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::CalGrid(CMFCPropertyGridProperty* pGrid);
	 *
	 * \brief	����õ���������
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
	 * \brief	���һ�������ƾ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLuminaire	�ƾ߿ؼ�ָ��
	 * \param	p				  	�ƾ�λ��
	 * \param	type			  	�ƾ�����
	 * \param	lm				  	�ƾ��ն�
	 * \param	w				  	�ƾ߹���
	 * \param	np				  	�ƾ߷�ʸ
	 **************************************************************************************************/

	void AddSingleLuminaire(CMFCPropertyGridProperty* pLuminaire,Vec3d p, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0,	double w = 23.0 ,Vec3d np = Vec3d(0,0,-1));

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, Vec2d originP, double z, WCHAR type[80] = _T("FAC21280P-23W"), double lm = 2100.0, double w = 23.0, Vec3d np = Vec3d(0,0,-1), int rowN = 5,int colN = 5,double rowL = 200.0,double colL = 200.0);
	 *
	 * \brief	���һ��ƾ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pLuminaire	�ƾ���ؼ�ָ��
	 * \param	originP			  	��ʼ��λ��
	 * \param	z				  	The z coordinate.
	 * \param	type			  	The type.
	 * \param	lm				  	�ն�
	 * \param	w				  	The width.
	 * \param	np				  	��ʸ
	 * \param	rowN			  	����
	 * \param	colN			  	����
	 * \param	rowL			  	�п�
	 * \param	colL			  	�п�
	 **************************************************************************************************/

	void AddSetLuminaire(CMFCPropertyGridProperty* pLuminaire, Vec2d originP, double z, WCHAR type[80] = _T("FAC21280P-23W"),
		double lm = 2100.0, double w = 23.0, Vec3d np = Vec3d(0,0,-1),
		int rowN = 5,int colN = 5,double rowL = 200.0,double colL = 200.0);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::AddControlSet(CMFCPropertyGridProperty* pControlSet, WCHAR type[80] = _T("mannual_on_auto_off"), int keyGrid = -1);
	 *
	 * \brief	��ӿ��Ʒ���
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
	 * \brief	�����Ա
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
	 * \brief	���·����ն�
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
	 * \brief	���µƾ���Ϣ
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
	 * \brief	���¿��Ʒ������
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
	 * \brief	ȡ�÷��������йؼ���
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
	 * \brief	���õƾ���
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
	 * \brief	���㷿������½�����
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
	 * \brief	���㷿�����������
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
	 * \brief	����������Ϣ��������
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
	 * \brief	�����ƾ���Ϣ��������
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
	 * \brief	���յƾ�����ɾ���ƾ�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pRoom	If non-null, the room.
	 * \param	index		 	�ƾ�����
	 * \param	isSingle	 	�Ƿ��ǵ����ƾ�
	 **************************************************************************************************/

	void DeleteLumByIndex(CMFCPropertyGridProperty* pRoom, int index, bool isSingle);

	/**********************************************************************************************//**
	 * \fn	void CRoomWnd::DeleteControSetByIndex(CMFCPropertyGridProperty* pRoom, int index);
	 *
	 * \brief	��������ɾ�����Ʒ���
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
	* \brief	��������ɾ����Ա
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
	 * \brief	���淿����Ϣ�������ļ���
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
	 * \brief	�ӹ����ļ����뷿����Ϣ
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
	 * \brief	��Ӧ�½����䰴ť
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnNewRoom();
	afx_msg void OnUpdateButton(CCmdUI* pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnDeleteRoom();
	 *
	 * \brief	��Ӧɾ�����䰴ť
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnDeleteRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg LRESULT CRoomWnd::OnPropertyChanged(WPARAM,LPARAM);
	 *
	 * \brief	����������Ըı�ʱ����
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
	 * \brief	���������Ҽ��˵�������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pWnd	If non-null, the window.
	 * \param	point			�������
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
	 * \brief	��Ӧ��ӵ����ƾ߲˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSingle();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddLuminaireSet();
	 *
	 * \brief	��Ӧ��ӵƾ���˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddLuminaireSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddControlSet();
	 *
	 * \brief	��Ӧ��ӿ��Ʒ���˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddControlSet();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CRoomWnd::OnRoomAddPerson();
	 *
	 * \brief	��Ӧ�����Ա�˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnRoomAddPerson();
};

