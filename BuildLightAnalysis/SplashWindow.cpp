#include "stdafx.h"
#include "resource.h"
#include "SplashWindow.h"

BEGIN_MESSAGE_MAP(CSplashWindow, CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()

CSplashWindow::CSplashWindow()
{
  m_Bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_WELCOME)); //自己加载想要的位图资源
  m_Bitmap.GetBitmap(&bmBitmap); //Get Bitmap Info
}

CSplashWindow::~CSplashWindow()
{
}

void CSplashWindow::CreateSplash()
{
  //Create Splash Window
  CWnd::CreateEx(0,
  AfxRegisterWndClass(
  0,
  AfxGetApp()->LoadStandardCursor(IDC_UPARROW)),
  NULL,
// "SplashWindow Sample",
  WS_POPUP,
  0,
  0,
  bmBitmap.bmWidth, //Bitmap Width = Splash Window Width
  bmBitmap.bmHeight, //Bitmap Height = Splash Window Height
  NULL,
  NULL,
  NULL);
}

void CSplashWindow::OnPaint()
{
  CPaintDC dc(this);

  CBrush brush;
  brush.CreateSolidBrush(RGB(64,64,255));
  dc.SelectObject(&brush);
  dc.Rectangle(0,0,bmBitmap.bmWidth,bmBitmap.bmHeight);

  MemDC.CreateCompatibleDC(NULL); //Create Memory DC
  Old_Bitmap = MemDC.SelectObject(&m_Bitmap); //Select DC

  int num = bmBitmap.bmWidth/40;
  dc.StretchBlt(0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,&MemDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,SRCCOPY);

/* for(int k=0; k <40; k++) //百叶窗效果
  {
  for(int m=0; m <num+1; m++)
  {
  dc.BitBlt(m*40+k,0,1,bmBitmap.bmHeight,&MemDC,m*40+k,0,SRCCOPY);
  }
  Sleep(1);
  }*/
    
  MemDC.SelectObject(Old_Bitmap); //Select Bitmap
}