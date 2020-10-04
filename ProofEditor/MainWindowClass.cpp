# include "MainWindowClass.h"

# define MASK_ALPHA_VALUE			220
# define MAX_WINDOW_CLASS_LEN		1000
# define MAX_WINDOW_TITLE_LEN		1000

# define MAX_INDEX_S_LEN			100

# define INDEX_X					10
# define INDEX_Y					10

# define TIMER_RESOLUTION			100
# define HEIGHT_RATIO_RES			(float)0.01
# define SCALE_RES					(float)0.1

# define SEPERATOR					L" - "

extern HINSTANCE hInstance;
extern BOOL bMaskEnable;
extern BOOL bTextMode;
extern BOOL bBrowseMode;
extern BOOL bIGMode;
extern BOOL bIBMode;
extern BOOL bGReady;
extern BOOL bExtents;
extern BOOL bDoGrayScale;

extern PageClass Page;
extern FilesClass Files;
extern LocaleClass Locale;
extern NotifyMessageBoxClass NotifyMessageBox;
extern SettingsClass Settings;

extern HWND hWndEdit;

MainWindowClass::MainWindowClass(HWND& hWnd) : hWnd(hWnd)
{
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = MainProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(0);
	wc.lpszMenuName = NULL;	// = MAIN_WINDOW_CLASS;

	bFullScreen = FALSE;
	margin = FALSE;
	blend_function.BlendOp = AC_SRC_OVER;
	blend_function.BlendFlags = 0;
	blend_function.SourceConstantAlpha = MASK_ALPHA_VALUE;
	blend_function.AlphaFormat = NULL;

	szWindowClass = new WCHAR[MAX_WINDOW_CLASS_LEN];
	szWindowTitle = new WCHAR[MAX_WINDOW_TITLE_LEN];
	szIndex = new WCHAR[MAX_INDEX_S_LEN];
	bTimer = FALSE;
	bBreaking = FALSE;
	bExtendingR = FALSE;
	bExtendingL = FALSE;
	heightRatio = 1;								// Required
	bShowIndex = TRUE;
	bCurExtent = TRUE;
};

VOID MainWindowClass::SetHWnd(HWND hWnd)
{
	MainWindowClass::hWnd = hWnd;
}

VOID MainWindowClass::Create()
{
	scale = (float)Settings.GetValue(DEFAULT_SCALE);
	bAutoScale = Settings.GetValue(AUTO_SCALE);
	wBorder = Settings.GetValue(WIDTH_BORDER);
	hBorder = Settings.GetValue(HEIGHT_BORDER);
	timerDelay = Settings.GetValue(PLAY_DELAY);
	browseX = Settings.GetValue(BROWSE_X);
	browseY = Settings.GetValue(BROWSE_Y);
	browseScale = Settings.GetFValue(BROWSE_SCALE);

	HDC hDC = GetDC(NULL);
	LONG lfHeight = -MulDiv(Settings.GetValue(I_FONT_SIZE), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);
	hfIFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, Settings.GetString(I_FONT_NAME));
	hPen = CreatePen(PS_SOLID, 1, Settings.GetValue(MARGIN_COLOUR));
	hCr_Hand = LoadCursor(NULL, IDC_SIZEALL);
	hCr_Norm = LoadCursor(NULL, IDC_ARROW);
	::bExtents = Settings.GetValue(DRAW_EXTENTS)? TRUE : FALSE;
	UINT fullHeight = Settings.GetValue(WINDOW_HEIGHT);
	UINT fullWidth = Settings.GetValue(WINDOW_WIDTH);
	if(Settings.GetValue(AUTO_PLAY))
		ToggleTimer();
	Locale.GetString(IDS_MAIN_WINDOW_CLASS, szWindowClass, MAX_WINDOW_CLASS_LEN);
	Locale.GetString(IDS_MAIN_WINDOW_TITLE, szWindowTitle, MAX_WINDOW_TITLE_LEN);
	wc.lpszClassName = szWindowClass;
	wc.hInstance = ::hInstance;
	wc.hIcon = LoadIcon(::hInstance, MAKEINTRESOURCE(IDI_PROOFEDITOR_ICON));
	wc.hIconSm = LoadIcon(::hInstance, MAKEINTRESOURCE(IDI_PROOFEDITOR_ICON));
	if(!RegisterClassEx(&wc))
	{
		NotifyMessageBox.NotifyMessage(IDS_W_REG_FAIL, IDS_ERROR, MB_ICONERROR | MB_OK);
		exit(-1);
	}

	hWnd = CreateWindowEx(
		//WS_EX_COMPOSITED,
		0,
		szWindowClass,
		szWindowTitle,
		WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - fullWidth)/2,
		//(GetSystemMetrics(SM_CYSCREEN) - fullHeight)/2
		Settings.GetValue(WINDOW_Y_POSITION), fullWidth, fullHeight,
		NULL, NULL, ::hInstance, NULL);

	if(hWnd == NULL)
	{
		int x;
		x = GetLastError();
		NotifyMessageBox.NotifyMessage(IDS_W_CREATE_FAIL, IDS_ERROR, MB_ICONERROR | MB_OK);
		exit(-2);
	}

	RECT wRect;
	GetClientRect(hWnd, &wRect);
	lftOffClient = (fullWidth-(wRect.right-wRect.left))/2;
	topOffClient = (fullHeight-(wRect.bottom-wRect.top))/2;
	Page.LoadPage();
	if(Page.bText)
		if(::bTextMode)
			if(::bBrowseMode)
				heightRatio = Settings.GetFValue(HEIGHT_RATIO_BROWSE);
			else
				heightRatio = Settings.GetFValue(HEIGHT_RATIO_RECT);
	Locale.GetString(IDS_MAIN_WINDOW_TITLE, szWindowTitle, MAX_WINDOW_TITLE_LEN);
	wcscat_s(szWindowTitle, MAX_WINDOW_TITLE_LEN, L" - ");
	wcscat_s(szWindowTitle, MAX_WINDOW_TITLE_LEN, Files.CurName());
	szWindowTitle[wcslen(szWindowTitle)-4] = NULL;
	SetWindowText(hWnd, szWindowTitle);
	Page.RenderRects();
	PrepIndex();
}

BOOL MainWindowClass::Load(BOOL bInc)
{
	if(bInc)
	{
		if(Files.iFile+1 >= Files.nFiles)
			return FALSE;
		Files.iFile++;
	}
	else
	{
		if(!Files.iFile)
			return FALSE;
		Files.iFile--;
	}
	Page.LoadPage();
	if(Page.bText)
	{
		if(::bTextMode)
		{
			heightRatio = Settings.GetFValue(HEIGHT_RATIO_RECT);
			ShowWindow(::hWndEdit, SW_SHOW);
			OnSize();
		}
	}
	else
	{
		if(::bTextMode)
		{
			heightRatio = 1;
			ShowWindow(::hWndEdit, SW_HIDE);
			OnSize();
		}
	}
	Locale.GetString(IDS_MAIN_WINDOW_TITLE, szWindowTitle, MAX_WINDOW_TITLE_LEN);
	wcscat_s(szWindowTitle, MAX_WINDOW_TITLE_LEN, SEPERATOR);
	wcscat_s(szWindowTitle, MAX_WINDOW_TITLE_LEN, Files.CurName());
	szWindowTitle[wcslen(szWindowTitle)-4] = NULL;
	SetWindowText(hWnd, szWindowTitle);
	Page.RenderRects();
	PrepIndex();
	return TRUE;
}

VOID MainWindowClass::OnMove()
{
	RECT wRect;
	GetWindowRect(hWnd, &wRect);
	Settings.SetValue(WINDOW_Y_POSITION, wRect.top);
}

VOID MainWindowClass::OnSize()
{
	RECT wRect;
	GetClientRect(hWnd, &wRect);
	width = wRect.right-wRect.left;
	height = (UINT)((wRect.bottom-wRect.top)*heightRatio);
	btmOffClient = (wRect.bottom-wRect.top)-height;
	MoveWindow(::hWndEdit, 
				0,
				height,
				width,
				btmOffClient,
				FALSE);
	GetWindowRect(hWnd, &wRect);
	Settings.SetValue(WINDOW_Y_POSITION, wRect.top);
	Settings.SetValue(WINDOW_WIDTH, wRect.right-wRect.left);
	Settings.SetValue(WINDOW_HEIGHT, wRect.bottom-wRect.top);
	if(bAutoScale)
		AutoScale();

	Page.RefWord();
}

VOID MainWindowClass::AutoBrowseScale()
{
	for(; Page.Width()*browseScale < width || Page.Height()*browseScale < height; browseScale += (float)0.01);
	for(; Page.Width()*browseScale > width || Page.Height()*browseScale > height; browseScale -= (float)0.01);
	Settings.SetFValue(BROWSE_SCALE, browseScale);
	browseX = (int)(width-Page.Width()*browseScale)/2;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale)/2;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::AutoBrowseSWidth()
{
	for(; Page.Width()*browseScale < width; browseScale += (float)0.01);
	for(; Page.Width()*browseScale > width; browseScale -= (float)0.01);
	Settings.SetFValue(BROWSE_SCALE, browseScale);
	browseX = (int)(width-Page.Width()*browseScale)/2;
	Settings.SetValue(BROWSE_X, browseX);
}

VOID MainWindowClass::AutoBrowseSHeight()
{
	for(; Page.Height()*browseScale < height; browseScale += (float)0.01);
	for(; Page.Height()*browseScale > height; browseScale -= (float)0.01);
	Settings.SetFValue(BROWSE_SCALE, browseScale);
	browseY = (int)(height-Page.Height()*browseScale)/2;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::OnPaint()
{
	HDC         hDC;
	PAINTSTRUCT Ps;

	hDC = BeginPaint(hWnd, &Ps);
	HDC hDCMem = CreateCompatibleDC(hDC);
	HDC hDCBuffer = CreateCompatibleDC(hDC);
	HDC hDCMemMask = CreateCompatibleDC(hDC);
    HBITMAP hBMBuffer = CreateCompatibleBitmap(hDC, width, height);
	HBITMAP hBMPMask = CreateCompatibleBitmap(hDC, width, height);
    SelectObject(hDCBuffer, hBMBuffer);
	if(::bIBMode)
		SelectObject(hDCMem, Page.HImageB());
	else if(::bIGMode&&::bGReady)
		SelectObject(hDCMem, Page.HImageG());
	else
		SelectObject(hDCMem, Page.HImageI());
	SelectObject(hDCMemMask, hBMPMask);

	PatBlt(hDCBuffer, 0, 0, width, height, WHITENESS); 
	PatBlt(hDCMemMask, 0, 0, width, height, WHITENESS); 
	UINT t_double_wBorder = wBorder*2;
	UINT t_double_hBorder = hBorder*2;
	float t1w = (Page.ObjWidth()+t_double_wBorder)*scale;
	float t1h = (Page.LineHeight())*scale; // +t_double_hBorder
	StretchBlt(hDCBuffer,
		(UINT)((width-t1w)/2),
		(UINT)((height-t1h)/2),
		(UINT)(t1w),
		(UINT)(t1h),
		hDCMem,
		Page.ObjBeginX()-wBorder,
		Page.LineEndY(),//-hBorder
		Page.ObjWidth()+t_double_wBorder,
		Page.LineHeight(),//+t_double_hBorder,
		SRCCOPY);
	if(::bMaskEnable)
	{
		float t2h = (Page.LineHeight())*scale;//ObjHeight
		float t2w = (Page.ObjWidth())*scale;
		AlphaBlend(hDCBuffer, 0, 0, width, (UINT)ceil(((float)height-t2h)/2), hDCMemMask, 0, 0, width, height, blend_function);
		AlphaBlend(hDCBuffer, 0, 0, (UINT)ceil(((float)width-t2w)/2), height, hDCMemMask, 0, 0, width, height, blend_function);			//Left
		AlphaBlend(hDCBuffer, 0, (UINT)ceil(((float)height+t2h))/2, width, height, hDCMemMask, 0, 0, width, height, blend_function);
		AlphaBlend(hDCBuffer, (UINT)ceil(((float)width+t2w)/2), 0, width, height, hDCMemMask, 0, 0, width, height, blend_function);		//Right
	}

	if(::bExtents || bBreaking)
	{
		POINT rect[3];
		rect[0].x = (UINT)ceil((width-Page.ObjWidth()*scale)/2);
		rect[0].y = (UINT)ceil(((float)height-(float)Page.ObjHeight()*scale)/2)-1;
		rect[1].x = rect[0].x+(UINT)ceil(Page.ObjWidth()*scale)-1;
		rect[1].y = rect[0].y;
		rect[2].x = rect[1].x;
		rect[2].y = rect[1].y+(UINT)ceil(Page.ObjHeight()*scale);

		SelectObject(hDCBuffer, hPen);
		if(bBreaking)
		{
			MoveToEx(hDCBuffer, rect[0].x+(UINT)ceil(xWidth*scale)-1, rect[0].y, NULL);
			LineTo(hDCBuffer, rect[0].x+(UINT)ceil(xWidth*scale)-1, rect[2].y);
		}
		else
			Polyline(hDCBuffer, rect, 3);
	}

	if(bShowIndex)
	{
		SelectObject(hDCBuffer, hfIFont);
		SetBkMode(hDCBuffer, TRANSPARENT);
		TextOut(hDCBuffer, INDEX_X, INDEX_Y, szIndex, lIndex);
	}

	BitBlt(hDC, 0, 0, width, height, hDCBuffer, 0, 0, SRCCOPY);
	EndPaint(hWnd, &Ps);

	DeleteObject(hBMPMask);
	DeleteObject(hBMBuffer);
	DeleteDC(hDCMemMask);
	DeleteDC(hDCBuffer);
	DeleteDC(hDCMem);
//	DeleteDC(hDC);
}

VOID MainWindowClass::OnBrowsePaint()
{
	HDC         hDC;
	PAINTSTRUCT Ps;

	hDC = BeginPaint(hWnd, &Ps);
	HDC hDCBuffer = CreateCompatibleDC(hDC);
	HDC hDCMem = CreateCompatibleDC(hDC);
    HBITMAP hBMBuffer = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hDCBuffer, hBMBuffer);
	if(::bIBMode)
		SelectObject(hDCMem, ::bExtents? Page.HBRects() : Page.HImageB());
	else if(::bIGMode&&::bGReady)
		SelectObject(hDCMem, ::bExtents? Page.HGRects() : Page.HImageG());
	else
		SelectObject(hDCMem, ::bExtents? Page.HIRects() : Page.HImageI());
	PatBlt(hDCBuffer, 0, 0, width, height, WHITENESS); 
	StretchBlt(hDCBuffer,
		browseX,
		browseY,
		(UINT)(Page.Width()*browseScale),
		(UINT)(Page.Height()*browseScale),
		hDCMem,
		0,
		0,
		Page.Width(),
		Page.Height(),
		SRCCOPY);

	if(bShowIndex)
	{
		SelectObject(hDCBuffer, hfIFont);
		SetBkMode(hDCBuffer, TRANSPARENT);
		TextOut(hDCBuffer, INDEX_X, INDEX_Y, szIndex, lIndex);
	}

	if(bExtendingR||bExtendingL||bBreaking)
	{
		SelectObject(hDCBuffer, hPen);
		UINT x, y, l;
		if(bExtendingR)
			x = (UINT)(Page.ObjBeginX()*browseScale+browseX+Page.ObjWidth()*browseScale);
		else if(bExtendingL)
			x = (UINT)ceil(Page.ObjBeginX()*browseScale+browseX);							// Ceiling may case 1px loss - check!
		else	//if(bBreaking)
			x = (UINT)ceil(Page.ObjBeginX()*browseScale+browseX+xWidth*browseScale);			// Ceiling may case 1px loss - check!
		y = (UINT)ceil(Page.ObjEndY()*browseScale+browseY);
		l = (UINT)ceil(Page.ObjHeight()*browseScale);

		MoveToEx(hDCBuffer, x, y, NULL);
		LineTo(hDCBuffer, x, y+l);
	}

	if(bCurExtent)
	{
		POINT rect[5];
		rect[0].x = (UINT)(Page.ObjBeginX()*browseScale+browseX);
		rect[0].y = (UINT)(Page.ObjEndY()*browseScale+browseY);
		rect[1].x = rect[0].x+(UINT)ceil(Page.ObjWidth()*browseScale);
		rect[1].y = rect[0].y;
		rect[2].x = rect[1].x;
		rect[2].y = rect[0].y+(UINT)ceil(Page.ObjHeight()*browseScale);
		rect[3].x = rect[0].x;
		rect[3].y = rect[2].y;
		rect[4].x = rect[0].x;
		rect[4].y = rect[0].y;

		SelectObject(hDCBuffer, hPen);
		Polyline(hDCBuffer, rect, 5);
	}

	BitBlt(hDC, 0, 0, width, height, hDCBuffer, 0, 0, SRCCOPY);
	EndPaint(hWnd, &Ps);
	DeleteObject(hBMBuffer);
//	DeleteDC(hDC);
	DeleteDC(hDCMem);
 	DeleteDC(hDCBuffer);
}

BOOL MainWindowClass::RectSelect(POINT xPoint)
{
	ScreenToClient(hWnd, &xPoint);
	UINT x = (UINT)((xPoint.x-lftOffClient-browseX)/browseScale);
	UINT y = (UINT)((xPoint.y-topOffClient-browseY+btmOffClient)/browseScale);
	for(UINT g = 0; g < Page.NGroup(); g++)
		for(UINT s = 0; s < Page.NGSection(g); s++)
			for(UINT l = 0; l < Page.NGSLine(g, s); l++)
				for(UINT w = 0; w < Page.NGSLWord(g, s, l); w++)
					if(y > (Page.Height()-Page.ObjBeginY(g, s, l, w)-1))
						if(y < (Page.Height()-Page.ObjBeginY(g, s, l, w)-1+Page.ObjHeight(g, s, l, w)))
							if(x > Page.ObjBeginX(g, s, l, w))
								if(x < (Page.ObjBeginX(g, s, l, w)+Page.ObjWidth(g, s, l, w)))
								{
									Page.ObjSelect(g, s, l, w);
									PrepIndex();
									return TRUE;
								}
	return FALSE;
}

VOID MainWindowClass::ToggleFullScreen()
{
	if(bAutoScale)
		AutoScale();
	LONG style = GetWindowLong(hWnd,GWL_STYLE);
	if(bFullScreen)
	{
		ShowCursor(TRUE);
		style |= (WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
		bFullScreen = FALSE;
		SetWindowLong(hWnd, GWL_STYLE, style);
		MoveWindow(hWnd, windowRect.left, windowRect.top, windowRect.right-windowRect.left, windowRect.bottom-windowRect.top, TRUE);
	}
	else
	{
		if(!::bBrowseMode)
			ShowCursor(FALSE);
		GetWindowRect(hWnd, &windowRect); 
		style &= ~(WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
		bFullScreen = TRUE;
		SetWindowLong(hWnd, GWL_STYLE, style);
		MoveWindow(hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
	}
};

VOID MainWindowClass::OnEsc()
{
	if(bFullScreen)
		ToggleFullScreen();
}

VOID MainWindowClass::BrowseCenterCur()
{
	browseX = UINT(width/2-(Page.ObjBeginX()+Page.ObjWidth()/2)*browseScale);
	browseY = UINT(height/2-(Page.ObjEndY()+Page.ObjHeight()/2)*browseScale);
}

VOID MainWindowClass::BrowseLeftTop()
{
	browseX = 0;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = 0;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseTopCenter()
{
	browseX = (int)(width-Page.Width()*browseScale)/2;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = 0;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseRightTop()
{
	browseX = (int)(width-Page.Width()*browseScale);
	Settings.SetValue(BROWSE_X, browseX);
	browseY = 0;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseLeftCenter()
{
	browseX = 0;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale)/2;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseCenter()
{
	browseX = (int)(width-Page.Width()*browseScale)/2;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale)/2;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseRightCenter()
{
	browseX = (int)(width-Page.Width()*browseScale);
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale)/2;
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseLeftBottom()
{
	browseX = 0;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale);
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseBottomCenter()
{
	browseX = (int)(width-Page.Width()*browseScale)/2;
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale);
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::BrowseRightBottom()
{
	browseX = (int)(width-Page.Width()*browseScale);
	Settings.SetValue(BROWSE_X, browseX);
	browseY = (int)(height-Page.Height()*browseScale);
	Settings.SetValue(BROWSE_Y, browseY);
}

VOID MainWindowClass::OnEdit(BOOL bOEdit, BOOL bTEdit)
{
	Page.nPEdit++;
	if(!Page.bOChanged && !Page.bTChanged)
	{
		if(bOEdit)
			Page.bOChanged = TRUE;
		else if(bTEdit)
			Page.bTChanged = TRUE;
		wcscat_s(szWindowTitle, MAX_WINDOW_TITLE_LEN, L"*");
		SetWindowText(hWnd, szWindowTitle);
	}
	if(Page.nPEdit == Page.nEditToSave)
	{
		Page.bOChanged = TRUE;
		Page.bTChanged = TRUE;
		Page.nPEdit = 0;
		OnSave();
	}
	if(bOEdit)
		Page.RenderRects();
}

VOID MainWindowClass::OnSave()
{
	if(Page.bOChanged || Page.bTChanged)
	{
		szWindowTitle[wcslen(szWindowTitle)-1] = NULL;
		SetWindowText(hWnd, szWindowTitle);
		Page.SavePage();
	}
}

BOOL MainWindowClass::OnClose()
{
	if(Page.bOChanged || Page.bTChanged)
	{
		switch(NotifyMessageBox.NotifyMessage(IDS_SAVE_Q, FALSE, MB_YESNO|MB_APPLMODAL))
		{
		case IDYES:
			OnSave();
		case IDNO:
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

VOID MainWindowClass::OnFocus()
{
	if(bTimer)
		SetTimer(hWnd, ID_TIMER_1, timerDelay, NULL);
}

VOID MainWindowClass::ToggleTimer()
{
	if(bTimer)
	{
		KillTimer(hWnd, ID_TIMER_1);
		bTimer = FALSE;
	}
	else
	{
		SetTimer(hWnd, ID_TIMER_1, timerDelay, NULL);
		bTimer = TRUE;
	}
	Settings.SetValue(AUTO_PLAY, bTimer);
}

VOID MainWindowClass::ToggleMargin()
{
	if(margin)
	{
		margin = FALSE;
		wBorder = xWBorder;
		hBorder = xWBorder;
	}
	else
	{
		xWBorder = wBorder;
		xHBorder = hBorder;
		wBorder = 1;
		hBorder = 1;
		margin = TRUE;
	}
}

VOID MainWindowClass::IncHMargin()
{
	wBorder++;
	Settings.SetValue(WIDTH_BORDER, wBorder);
	if(bAutoScale)
		AutoScale();
}

VOID MainWindowClass::DecHMargin()
{
	if(wBorder)
		wBorder--;
	Settings.SetValue(WIDTH_BORDER, wBorder);
	if(bAutoScale)
		AutoScale();
}

VOID MainWindowClass::IncTDelay()
{
	timerDelay += TIMER_RESOLUTION;
	if(bTimer)
		SetTimer(hWnd, ID_TIMER_1, timerDelay, NULL);
	Settings.SetValue(PLAY_DELAY, timerDelay);
}

VOID MainWindowClass::DecTDelay()
{
	if((timerDelay-TIMER_RESOLUTION) > 0)
	{
		timerDelay -= TIMER_RESOLUTION;
		if(bTimer)
			SetTimer(hWnd, ID_TIMER_1, timerDelay, NULL);
		Settings.SetValue(PLAY_DELAY, timerDelay);
	}
}
/*
VOID MainWindowClass::IncWSync()
{
	Page.iWord++;
	if(Page.iWord == Page.nLWords[Page.iLine])
		Page.iWord--;
	if(bAutoScale)
		AutoScale();
}

VOID MainWindowClass::DecWSync()
{
	if(Page.iWord)
		Page.iWord--;
	if(bAutoScale)
		AutoScale();
}
*/
VOID MainWindowClass::SelWord()
{
	Page.SelWord();
	PrepIndex();
}

VOID MainWindowClass::IncWord()
{
	if(!Page.IncWord())
		return;

	if(bAutoScale)
		AutoScale();
	PrepIndex();
}

VOID MainWindowClass::DecWord()
{
	if(!Page.DecWord())
		return;

	if(bAutoScale)
		AutoScale();
	PrepIndex();
}

VOID MainWindowClass::IncLine()
{
	if(!Page.IncLine())
		return;
	if(bAutoScale)
		AutoScale();
	PrepIndex();
}

VOID MainWindowClass::DecLine()
{
	if(!Page.DecLine())
		return;
	if(bAutoScale)
		AutoScale();
	PrepIndex();
}

VOID MainWindowClass::IncWindowH()
{
	if(heightRatio < 1)
	{
		heightRatio += HEIGHT_RATIO_RES;
		if(::bBrowseMode)
			Settings.SetFValue(HEIGHT_RATIO_BROWSE, heightRatio);
		else
			Settings.SetFValue(HEIGHT_RATIO_RECT, heightRatio);
	}
}

VOID MainWindowClass::DecWindowH()
{
	if(heightRatio)
	{
		heightRatio -= HEIGHT_RATIO_RES;
		if(::bBrowseMode)
			Settings.SetFValue(HEIGHT_RATIO_BROWSE, heightRatio);
		else
			Settings.SetFValue(HEIGHT_RATIO_RECT, heightRatio);
	}
}

VOID MainWindowClass::ToggleTextWindow()
{
	if(::bTextMode)
	{
		if(Page.bText)
		{
			heightRatio = 1;
			OnSize();
			ShowWindow(::hWndEdit, SW_HIDE);
			SendMessage(::hWndEdit, CM_REF_WORD_I, 0, 0);
		}
		::bTextMode = FALSE;
	}
	else
	{
		if(Page.bText)
		{
			if(::bBrowseMode)
				heightRatio = Settings.GetFValue(HEIGHT_RATIO_BROWSE);
			else
				heightRatio = Settings.GetFValue(HEIGHT_RATIO_RECT);
			OnSize();
			ShowWindow(::hWndEdit, SW_SHOW);
			SendMessage(::hWndEdit, CM_REF_WORD_I, 0, 0);
		}
		::bTextMode = TRUE;
	}
}

VOID MainWindowClass::FocusEditWindow()
{
//	if(bTimer)
//	{
		KillTimer(hWnd, ID_TIMER_1);
//		bTimer = FALSE;
//	}
	SetFocus(::hWndEdit);
	PrepIndex();
}

VOID MainWindowClass::Home()
{
	Page.Home();
	PrepIndex();
}

VOID MainWindowClass::XHome()
{
	Page.XHome();
	PrepIndex();
}

VOID MainWindowClass::End()
{
	Page.End();
	PrepIndex();
}

VOID MainWindowClass::XEnd()
{
	Page.XEnd();
	PrepIndex();
}

BOOL MainWindowClass::PageUp()
{
	BOOL bRet = Page.PageUp();
	PrepIndex();
	return bRet;
}

BOOL MainWindowClass::PageDown()
{
	BOOL bRet = Page.PageDn();
	PrepIndex();
	return bRet;
}

VOID MainWindowClass::AutoScale()
{
	while(((float)Page.ObjWidth()+wBorder*2)*scale >= width ||
		((float)Page.ObjHeight()+hBorder*2)*scale >= height)
		scale -= (float)SCALE_RES;
//	scale += (float)SCALE_RES;

	while(((float)Page.ObjWidth()+wBorder*2)*scale <= width &&
		((float)Page.ObjHeight()+hBorder*2)*scale <= height)
		scale += (float)SCALE_RES;
//	scale -= (float)(SCALE_RES*2.0);
}

VOID MainWindowClass::ToggleAutoScale()
{
	if(bAutoScale)
		bAutoScale = FALSE;
	else
	{
		bAutoScale = TRUE;
		AutoScale();
	}
	Settings.SetValue(AUTO_SCALE, bAutoScale);
}

VOID MainWindowClass::ZoomIn()
{
	scale += (float)SCALE_RES;
	Settings.SetValue(DEFAULT_SCALE, (UINT)scale);
}

VOID MainWindowClass::ZoomOut()
{
	if(scale > SCALE_RES)
	{
		scale -= (float)SCALE_RES;
		Settings.SetValue(DEFAULT_SCALE, (UINT)scale);
	}
}

VOID MainWindowClass::ToggleBrowseMode()
{
	if(::bBrowseMode)
	{
		if(bFullScreen)
			ShowCursor(FALSE);
		::bBrowseMode = FALSE;
		if(Page.bText)
		{
			heightRatio = Settings.GetFValue(HEIGHT_RATIO_RECT);
//			SendMessage(::hWndEdit, CM_REF_WORD_I, 0, 0);
		}
	}
	else if(!bBreaking && !bExtendingL && !bExtendingR)
	{
		if(bFullScreen)
			ShowCursor(TRUE);
		::bBrowseMode = TRUE;
		if(Page.bText)
		{
			heightRatio = Settings.GetFValue(HEIGHT_RATIO_BROWSE);
//			SendMessage(::hWndEdit, CM_REF_WORD_I, 0, 0);
		}
	}
	else
	{
		MessageBeep(MB_ICONASTERISK);
		return;
	}
	Settings.SetValue(BROWSE_MODE, ::bBrowseMode);
	OnSize();
}

VOID MainWindowClass::PrepIndex()
{
	lIndex = wsprintf(szIndex, L"%d | %d | %d", Page.iGroup+1, Page.iLine+1, Page.iWord+1);
}

VOID MainWindowClass::AddChar(WCHAR c)
{
	Page.AddChar(c);
	OnEdit(FALSE, TRUE);
}

VOID MainWindowClass::DelChar(BOOL bPrev)
{
	Page.DelChar(bPrev);
}

VOID MainWindowClass::DoGrayThread()
{
	Page.DoGrayThread();
}

VOID MainWindowClass::RenderRects()
{
	Page.RenderRects();
}

VOID MainWindowClass::RenderBullets()
{
	Page.RenderBullets();
}

VOID MainWindowClass::InitBreak()
{
	bBreaking = TRUE;
	xWidth = Page.ObjWidth()/2;
}

VOID MainWindowClass::ObjBreak()
{
	Page.ObjBreak(xWidth);
	bBreaking = FALSE;
	OnEdit(TRUE, TRUE);
}

VOID MainWindowClass::ObjBeginXInc(UINT l)
{
	Page.ObjBeginXInc(l);
}

VOID MainWindowClass::ObjBeginXDec(UINT l)
{
	Page.ObjBeginXDec(l);
}

VOID MainWindowClass::ObjEndXInc(UINT l)
{
	Page.ObjEndXInc(l);	// ToDo Bounds
}

VOID MainWindowClass::ObjEndXDec(UINT l)
{
	Page.ObjEndXDec(l);	// ToDo Bounds
}

VOID MainWindowClass::XWidthInc(UINT l)
{
	if(xWidth+l > Page.ObjWidth()-1)
		xWidth = Page.ObjWidth()-1;
	else
		xWidth += l;
}

VOID MainWindowClass::XWidthDec(UINT l)
{
	if((int)xWidth-l < 0)
		xWidth = 0;
	else
		xWidth -= l;
}

VOID MainWindowClass::ObjDelete(BOOL bPrev, BOOL bOEdit, BOOL bTEdit)
{
	Page.ObjDelete(bPrev);
	OnEdit(bOEdit, bTEdit);
}

VOID MainWindowClass::ObjMerge(BOOL bPrev)
{
	Page.ObjMerge(bPrev);
	OnEdit(TRUE, TRUE);
}
