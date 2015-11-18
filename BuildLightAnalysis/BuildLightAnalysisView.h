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
	 * \brief	对建模的内外墙进行优化，包括去掉多余的补充遗漏的边
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
	 * \brief	总的绘制类，项目中主界面的所有绘制相关逻辑都在这个方法中
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

	/** \brief	外墙模式中，当前选择的外墙索引. */
	int m_outModeOutWallSelectIndex;

	/** \brief	内墙模式中，当前选择的内墙索引. */
	int m_inModeInWallSelectIndex;

	/** \brief	外墙模式中，移动外墙节点的索引. */
	int m_buttonDownOutWallPoint;

	/** \brief	内墙模式中，是否画内墙的状态（鼠标是否按下）. */
	bool m_isDrawInWall;

	/** \brief	处理后模式中，当前选择的外墙索引. */
	int m_iSelectOutWallIndex;

	/** \brief	处理后模式中，当前选择的内墙索引. */
	int m_iSelectInWallIndex;

	/** \brief	处理后模式中，当前选择的窗户索引. */
	int m_iSelectWindowIndex;

	/** \brief	选择的关键点房间号. */
	int m_iSelectGridRoomIndex;

	/** \brief	选择的关键点的关键点编号. */
	int m_iSelectGridIndex;

	/** \brief	是否在拖动状态. */
	bool m_bIsPullTranslate;

	/** \brief	变换对象. */
	Transform m_transform;

	/** \brief	上一个记录点. */
	CPoint prePoint;

// Generated message map functions
protected:

	afx_msg void OnFilePrintPreview();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnRButtonUp(UINT nFlags, CPoint point);
	 *
	 * \brief	右键点起事件对应方法
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
	 * \brief	右键菜单对应处理方法
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 *
	 * \param [in,out]	pWnd	If non-null, the window.
	 * \param	point			点击的位置.
	 **************************************************************************************************/

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnMouseMove(UINT nFlags, CPoint point);
	 *
	 * \brief	鼠标移动对应的方法.
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
	 * \brief	鼠标左键按下对应的方法
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
	 * \brief	鼠标左键点起对应的方法
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
	 * \brief	删除背景的方法，用于swap刷新
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
	 * \brief	修改主界面大小的对应方法，主要处理修改transform类
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
	 * \brief	鼠标滚轮滚动事件处理
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
	 * \brief	对应优化后模式菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditOptimize();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopAddWindow();
	 *
	 * \brief	对应添加窗户菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopAddWindow();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopAddtoRoom();
	 *
	 * \brief	对应添加房间菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopAddtoRoom();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnEditTranslate();
	 *
	 * \brief	对应移动菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditTranslate();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnEditDoOptimize();
	 *
	 * \brief	对应优化菜单
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnEditDoOptimize();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnUpdateEditTranslate(CCmdUI *pCmdUI);
	 *
	 * \brief	对应移动菜单的点亮逻辑
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
	 * \brief	对应删除墙
	 *
	 * \author	Liuxq
	 * \date	2015/11/18
	 **************************************************************************************************/

	afx_msg void OnPopDeleteWall();

	/**********************************************************************************************//**
	 * \fn	afx_msg void CBuildLightAnalysisView::OnPopDeleteWin();
	 *
	 * \brief	对应删除窗户
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

