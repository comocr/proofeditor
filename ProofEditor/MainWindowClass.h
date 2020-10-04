# ifndef MAIN_WINDOW_CLASS_HEADER
# define MAIN_WINDOW_CLASS_HEADER

# include "Resource.h"

# pragma comment(lib, "Msimg32.lib")

# include "..\\Utilities\\Locale\\LocaleClass.h"
# include "..\\Utilities\\NotifyMessageBox\\NotifyMessageBoxClass.h"
# include "..\\Utilities\\Settings\\SettingsClass.h"
# include "EditWindowClass.h"
# include "PageClass.h"

LRESULT CALLBACK MainProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

class MainWindowClass
{
private:
	FLOAT scale;
	UINT wBorder;
	UINT hBorder;
	UINT xWBorder;
	UINT xHBorder;
	RECT windowRect;
	UINT width;
	UINT height;
	UINT lftOffClient;
	UINT topOffClient;
	UINT btmOffClient;
	//UINT rgtOffClient;
	BOOL bFullScreen;
	BOOL margin;
	BOOL bAutoScale;
	BOOL bTimer;
	UINT timerDelay;
	BLENDFUNCTION blend_function;
	WNDCLASSEX wc;
	LPWSTR szWindowClass;
	LPWSTR szWindowTitle;
	HPEN hPen;
	HFONT hfIFont;
	HWND& hWnd;

public:
	FLOAT heightRatio;
	BOOL bBreaking;
	BOOL bExtendingR;
	BOOL bExtendingL;
	UINT xWidth;
	BOOL browseMode;
	INT browseX;
	INT browseY;
	FLOAT browseScale;
	BOOL bShowIndex;

	LPWSTR szIndex;
	UINT lIndex;

	HCURSOR hCr_Hand;
	HCURSOR hCr_Norm;
	BOOL bCurExtent;

	MainWindowClass(HWND& hWnd);
	VOID SetHWnd(HWND hWnd);

	VOID Create();
	BOOL Load(BOOL Inc);
	VOID OnMove();
	VOID OnSize();
	VOID AutoBrowseScale();
	VOID AutoBrowseSWidth();
	VOID AutoBrowseSHeight();
	VOID OnPaint();
	VOID OnBrowsePaint();
	VOID ToggleFullScreen();
	VOID OnEsc();
	VOID OnEdit(BOOL bOEdit, BOOL bTEdit);
	VOID OnSave();
	BOOL OnClose();
	VOID OnFocus();
	VOID ToggleTimer();
	VOID ToggleMargin();
	VOID IncHMargin();
	VOID DecHMargin();
	VOID IncTDelay();
	VOID DecTDelay();
	//VOID IncWSync();
	//VOID DecWSync();
	VOID SelWord();
	VOID IncWord();
	VOID DecWord();
	VOID IncLine();
	VOID DecLine();
	VOID IncWindowH();
	VOID DecWindowH();
	VOID ToggleTextWindow();
	VOID FocusEditWindow();
	VOID AutoScale();
	VOID ToggleAutoScale();
	VOID ZoomIn();
	VOID ZoomOut();
	VOID Home();
	VOID XHome();
	VOID End();
	VOID XEnd();
	BOOL PageUp();
	BOOL PageDown();
	VOID BrowseCenterCur();
	VOID BrowseLeftTop();
	VOID BrowseTopCenter();
	VOID BrowseRightTop();
	VOID BrowseLeftCenter();
	VOID BrowseCenter();
	VOID BrowseRightCenter();
	VOID BrowseLeftBottom();
	VOID BrowseBottomCenter();
	VOID BrowseRightBottom();
	BOOL RectSelect(POINT dPoint);
	VOID ToggleBrowseMode();
	VOID PrepIndex();

	VOID AddChar(WCHAR c);
	VOID DelChar(BOOL bPrev);

	VOID DoGrayThread();
	VOID RenderRects();
	VOID RenderBullets();

	VOID InitBreak();
	VOID ObjBreak();
	
	VOID ObjBeginXInc(UINT l);
	VOID ObjBeginXDec(UINT l);
	VOID ObjEndXInc(UINT l);
	VOID ObjEndXDec(UINT l);

	VOID XWidthInc(UINT l);
	VOID XWidthDec(UINT l);

	VOID ObjDelete(BOOL bPrev, BOOL bOEdit, BOOL bTEdit);
	VOID ObjMerge(BOOL bPrev);
};

# endif
