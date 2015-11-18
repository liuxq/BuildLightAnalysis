/**********************************************************************************************//**
 * \file	BuildLightAnalysisView.h
 *
 * \brief	Interface of the CBuildLightAnalysisView class
 **************************************************************************************************/

#pragma once
#include "Transform.h"

class CBuildLightAnalysisView : public CView
{
protected: // create from serialization only
	CBuildLightAnalysisView();
	DECLARE_DYNCREATE(CBuildLightAnalysisView)

// Attributes
public:
	CBuildLightAnalysisDoc* GetDocument() const;

// Operations
public:

	/**********************************************************************************************//**
	 * \fn	void CBuildLightAnalysisView::optimize();
	 *
	 * \brief	�Խ�ģ������ǽ�����Ż�������ȥ������Ĳ�����©�ı�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	void optimize();
	Transform& GetTransform()
	{
		return m_transform;
	}
// Overrides
public:

	/**********************************************************************************************//**
	 * \fn	virtual void CBuildLightAnalysisView::OnDraw(CDC* pDC);
	 *
	 * \brief	�ܵĻ����࣬��Ŀ������������л�������߼��������������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pDC	If non-null, the device-context.
	 **************************************************************************************************/

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

// Implementation
public:
	virtual ~CBuildLightAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:

	/** \brief	��ǽģʽ�У���ǰѡ�����ǽ����. */
	int m_outModeOutWallSelectIndex;

	/** \brief	��ǽģʽ�У���ǰѡ�����ǽ����. */
	int m_inModeInWallSelectIndex;

	/** \brief	��ǽģʽ�У��ƶ���ǽ�ڵ������. */
	int m_buttonDownOutWallPoint;

	/** \brief	��ǽģʽ�У��Ƿ���ǽ��״̬������Ƿ��£�. */
	bool m_isDrawInWall;

	/** \brief	�����ģʽ�У���ǰѡ�����ǽ����. */
	int m_iSelectOutWallIndex;

	/** \brief	�����ģʽ�У���ǰѡ�����ǽ����. */
	int m_iSelectInWallIndex;

	/** \brief	�����ģʽ�У���ǰѡ��Ĵ�������. */
	int m_iSelectWindowIndex;

	/** \brief	ѡ��Ĺؼ��㷿���. */
	int m_iSelectGridRoomIndex;

	/** \brief	ѡ��Ĺؼ���Ĺؼ�����. */
	int m_iSelectGridIndex;

	/** \brief	�Ƿ����϶�״̬. */
	bool m_bIsPullTranslate;

	/** \brief	�任����. */
	Transform m_transform;

	/** \brief	��һ����¼��. */
	CPoint prePoint;

// Generated message map functions
protected:

	afx_msg void OnFilePrintPreview();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnRButtonUp(UINT nFlags, CPoint point);
	 *
	 * \brief	�Ҽ������¼���Ӧ����
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	nFlags	The flags.
	 * \param	point 	The point.
	 **************************************************************************************************/

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnContextMenu(CWnd* pWnd, CPoint point);
	 *
	 * \brief	�Ҽ��˵���Ӧ������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pWnd	If non-null, the window.
	 * \param	point			�����λ��.
	 **************************************************************************************************/

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point);
	 *
	 * \brief	����ƶ���Ӧ�ķ���.
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	nFlags	The flags.
	 * \param	point 	The point.
	 **************************************************************************************************/

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnLButtonDown(UINT nFlags, CPoint point);
	 *
	 * \brief	���������¶�Ӧ�ķ���
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	nFlags	The flags.
	 * \param	point 	The point.
	 **************************************************************************************************/

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnLButtonUp(UINT nFlags, CPoint point);
	 *
	 * \brief	�����������Ӧ�ķ���
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	nFlags	The flags.
	 * \param	point 	The point.
	 **************************************************************************************************/

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg BOOL CBuildLightAnalysisView::OnEraseBkgnd(CDC*);
	 *
	 * \brief	ɾ�������ķ���������swapˢ��
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	parameter1	If non-null, the first parameter.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	afx_msg BOOL OnEraseBkgnd(CDC*);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnSize(UINT nType, int cx, int cy);
	 *
	 * \brief	�޸��������С�Ķ�Ӧ��������Ҫ�����޸�transform��
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	nType	The type.
	 * \param	cx   	The cx.
	 * \param	cy   	The cy.
	 **************************************************************************************************/

	afx_msg void OnSize(UINT nType, int cx, int cy);

	/**********************************************************************************************//**
	 * \fn	afx_msg BOOL CBuildLightAnalysisView::OnMouseWheel(UINT, short, CPoint);
	 *
	 * \brief	�����ֹ����¼�����
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param	parameter1	The first parameter.
	 * \param	parameter2	The second parameter.
	 * \param	parameter3	The third parameter.
	 *
	 * \return	true if it succeeds, false if it fails.
	 **************************************************************************************************/

	afx_msg BOOL OnMouseWheel(UINT, short, CPoint);

	DECLARE_MESSAGE_MAP()
public:

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnEditOptimize();
	 *
	 * \brief	��Ӧ�Ż���ģʽ�˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOptimize();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopAddWindow();
	 *
	 * \brief	��Ӧ��Ӵ����˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopAddWindow();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopAddtoRoom();
	 *
	 * \brief	��Ӧ��ӷ���˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopAddtoRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnEditTranslate();
	 *
	 * \brief	��Ӧ�ƶ��˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditTranslate();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnEditDoOptimize();
	 *
	 * \brief	��Ӧ�Ż��˵�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditDoOptimize();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnUpdateEditTranslate(CCmdUI *pCmdUI);
	 *
	 * \brief	��Ӧ�ƶ��˵��ĵ����߼�
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pCmdUI	If non-null, the command user interface.
	 **************************************************************************************************/

	afx_msg void OnUpdateEditTranslate(CCmdUI *pCmdUI);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopDeleteWall();
	 *
	 * \brief	��Ӧɾ��ǽ
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopDeleteWall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopDeleteWin();
	 *
	 * \brief	��Ӧɾ������
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopDeleteWin();
};

#ifndef _DEBUG  // debug version in BuildLightAnalysisView.cpp
inline CBuildLightAnalysisDoc* CBuildLightAnalysisView::GetDocument() const
   { return reinterpret_cast<CBuildLightAnalysisDoc*>(m_pDocument); }
#endif

