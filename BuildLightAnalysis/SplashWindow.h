/**********************************************************************************************//**
 * \file	SplashWindow.h
 *
 * \brief	程序欢迎图片
 * 			要改变请在构造函数中修改IDB_BITMAP_WELCOME资源
 **************************************************************************************************/

class CSplashWindow : public CWnd
{
private:
	CDC MemDC;
	BITMAP bmBitmap;
	CBitmap m_Bitmap;
	CBitmap *Old_Bitmap;

public:
	CSplashWindow();
	~CSplashWindow();
	void CreateSplash();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};