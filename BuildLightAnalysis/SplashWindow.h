

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