/**********************************************************************************************//**
 * \file	SplashWindow.h
 *
 * \brief	����ӭͼƬ
 * 			Ҫ�ı����ڹ��캯�����޸�IDB_BITMAP_WELCOME��Դ
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