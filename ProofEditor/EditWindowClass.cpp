# include "EditWindowClass.h"

# define MASK_ALPHA_VALUE			220
# define MAX_WINDOW_CLASS_LEN		1000
# define MAX_WINDOW_TITLE_LEN		1000

extern HINSTANCE hInstance;
extern HFONT hfFont;
extern BOOL bMaskEnable;

extern LocaleClass Locale;
extern NotifyMessageBoxClass NotifyMessageBox;
extern SettingsClass Settings;
extern PageClass Page;

extern HWND hWndMain;

EditWindowClass::EditWindowClass(HWND& hWnd) : hWnd(hWnd)
{
	xc.cbSize = sizeof(WNDCLASSEX);
	xc.style = 0;
	xc.lpfnWndProc = EditProc;
	xc.cbClsExtra = 0;
	xc.cbWndExtra = 0;
	xc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	xc.hCursor = LoadCursor(NULL, IDC_ARROW);
	xc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	xc.lpszMenuName = NULL;
	xc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	blend_function.BlendOp = AC_SRC_OVER;
	blend_function.BlendFlags = 0;
	blend_function.SourceConstantAlpha = MASK_ALPHA_VALUE;
	blend_function.AlphaFormat = NULL;

	szWindowClass = new WCHAR[MAX_WINDOW_CLASS_LEN];
	szWindowTitle = new WCHAR[MAX_WINDOW_TITLE_LEN];
}

EditWindowClass::~EditWindowClass()
{
	ReleaseDC(hWnd, hDC);
}

VOID EditWindowClass::SetHWnd(HWND hWnd)
{
	EditWindowClass::hWnd = hWnd;
}

VOID EditWindowClass::Create()
{
	HDC hDC = GetDC(NULL);
	LONG lfHeight = -MulDiv(Settings.GetValue(T_FONT_SIZE), GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);
	hfFont = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, Settings.GetString(T_FONT_NAME));

	Locale.GetString(IDS_EDIT_WINDOW_CLASS, szWindowClass, MAX_WINDOW_CLASS_LEN);
	Locale.GetString(IDS_EDIT_WINDOW_TITLE, szWindowTitle, MAX_WINDOW_TITLE_LEN);
	xc.hInstance = ::hInstance;
	xc.lpszClassName = szWindowClass;
	if(!RegisterClassEx(&xc))
	{	
		NotifyMessageBox.NotifyMessage(IDS_W_REG_FAIL, IDS_ERROR, MB_ICONERROR | MB_OK);
		exit(-1);
	}

	hWnd = CreateWindowEx(
		NULL,
		szWindowClass,
		szWindowTitle,
		WS_CHILD|WS_VISIBLE,
		0, Settings.GetValue(WINDOW_HEIGHT)-height, width, height,
		::hWndMain, NULL, ::hInstance, NULL);

	if(hWnd == NULL)
	{
		NotifyMessageBox.NotifyMessage(IDS_W_CREATE_FAIL, IDS_ERROR, MB_ICONERROR | MB_OK);
		exit(-2);
	}
}

VOID EditWindowClass::OnInit()
{
	hDC = GetDC(hWnd);

	SelectObject(hDC, hfFont);
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	caretWidth = 0;
	caretHeight = tm.tmHeight + tm.tmExternalLeading;
	wordMargin = tm.tmAveCharWidth / 4;
	yCur = 0;

//	RECT rect;
//	GetClientRect(hWnd, &rect);
//	width = rect.right-rect.left;
//	height = rect.bottom-rect.top;
}

VOID EditWindowClass::OnSize()
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	width = rect.right-rect.left;
	height = rect.bottom-rect.top;
}

VOID EditWindowClass::OnPaint()
{
	PAINTSTRUCT Ps;
	HDC hDC = BeginPaint(hWnd, &Ps);
	HDC hDCMemMask = CreateCompatibleDC(hDC);
	HDC hDCBuffer = CreateCompatibleDC(hDC);
	HBITMAP hBMPMask = CreateCompatibleBitmap(hDC, width, height);
	HBITMAP hBMPBuffer = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hDCMemMask, hBMPMask);
	SelectObject(hDCBuffer, hBMPBuffer);
	SelectObject(hDCBuffer, hfFont);
	PatBlt(hDCMemMask, 0, 0, width, height, WHITENESS); 
	PatBlt(hDCBuffer, 0, 0, width, height, WHITENESS); 
	DrawText(hDCBuffer, Page.LineText(), -1, &prc, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);//| DT_CENTER
	AlphaBlend(hDCBuffer, 0, 0, lMaskEnd, height, hDCMemMask, 0, 0, lMaskEnd, height, blend_function);
	AlphaBlend(hDCBuffer, rMaskBegin, 0, width-rMaskBegin, height, hDCMemMask, 0, 0, lMaskEnd, height, blend_function);
	BitBlt(hDC, 0, 0, width, height, hDCBuffer, 0, 0, SRCCOPY);

	EndPaint(hWnd, &Ps);
	SetCaretPos(xCur, yCur);

	DeleteObject(hBMPBuffer);
	DeleteObject(hBMPMask);
	DeleteDC(hDC);
	DeleteDC(hDCMemMask);
	DeleteDC(hDCBuffer);
};

VOID EditWindowClass::Update()
{
	if(Page.IBChar() < Page.LineLen())
	{
		SIZE bSize, eSize;
		GetTextExtentPoint32(hDC, Page.LineText(), Page.IBChar(), &bSize);
		GetTextExtentPoint32(hDC, Page.LineText(), Page.IEChar()+1, &eSize);

		xOffset = (int)(width - (bSize.cx+eSize.cx))/2;

		lMaskEnd = bSize.cx + (int)xOffset - (int)wordMargin;
		rMaskBegin = eSize.cx + (int)xOffset + (int)wordMargin;

		prc.left = xOffset;
		prc.right = 200;
		prc.top = (height-caretHeight)/2;
		prc.bottom = 200;

		yCur = prc.top;

		SIZE sSize;
		GetTextExtentPoint(hDC, Page.LineText(), Page.IXChar(), &sSize);
		xCur = (int)xOffset + sSize.cx;

		SetCaretPos(xCur, yCur);
	}
}

VOID EditWindowClass::OnRight()
{
	if(Page.IXChar() > Page.IEChar())
		SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_RIGHT, NULL);
	else
	{
		Page.IncChar();
		SIZE sSize;
		GetTextExtentPoint(hDC, Page.LineText(), Page.IXChar(), &sSize);
		xCur = (int)xOffset + sSize.cx;
		SetCaretPos(xCur, yCur);
	}
}

VOID EditWindowClass::OnLeft()
{
	if(Page.IXChar() == Page.IBChar())
		SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_LEFT, NULL);
	else
	{
		Page.DecChar();
		SIZE sSize;
		GetTextExtentPoint(hDC, Page.LineText(), Page.IXChar(), &sSize);
		xCur = (int)xOffset + sSize.cx;
		SetCaretPos(xCur, yCur);
	}
}

VOID EditWindowClass::OnSetFocus()
{
	CreateCaret(hWnd, (HBITMAP) NULL, caretWidth, caretHeight); 
	SetCaretPos(xCur, yCur); 
	ShowCaret(hWnd); 
}

VOID EditWindowClass::FocusMainWindow()
{
	SetFocus(::hWndMain);
}
