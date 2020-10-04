# include "MainWindowProc.h"

# define SPEED_FACTOR_MIN			1
# define SPEED_FACTOR_MED			16
# define SPEED_FACTOR_MAX			32

# define WHEEL_SPEED_FACTOR_MIN		(float)0.01
# define WHEEL_SPEED_FACTOR_MAX		1
# define WHEEL_SPEED_FACTOR_MED		0.5
# define VSCROLL_FACTOR				10

# define CHECK_KEY					0x8000

# define OBJECT_BREAK_HOP			3

# define VK_LEFT_BRACKET			VK_OEM_4
# define VK_RIGHT_BRACKET			VK_OEM_6

extern BOOL bMaskEnable;
extern BOOL bTextMode;
extern BOOL bBrowseMode;
extern BOOL bIBMode;
extern BOOL bIGMode;
extern BOOL bGReady;
extern BOOL bExtents;
extern BOOL bDoGrayScale;
extern BOOL bRects;
extern BOOL bBullets;

extern MainWindowClass		MainWindow;
extern HWND					hWndEdit;

extern SplashWindowClass	SplashWindow;

extern SettingsClass		Settings;

LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINT prvPoint;
	static POINT curPoint;
	switch(msg)
	{
	case WM_SETFOCUS:
		//MainWindow.OnFocus();
		break;

	case WM_TIMER:
		MainWindow.IncWord();
		SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		break;

	case CM_CHANGED:
		MainWindow.OnEdit(FALSE, TRUE);
		return 0;

	case CM_ADDCHAR:
		MainWindow.AddChar((WCHAR)wParam);
		SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		return 0;

	case CM_DELCHAR:
		MainWindow.DelChar((BOOL)wParam);
		SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		return 0;

	case WM_PAINT:
		if(::bBrowseMode)
			MainWindow.OnBrowsePaint();
		else
			MainWindow.OnPaint();
		return 0;

	case WM_ERASEBKGND:
		return 1;

	case WM_CLOSE:
		MainWindow.OnClose();
		//DeleteObject(::hfFont);
		//DeleteObject(Page.hImageI);
		PostQuitMessage(WM_QUIT);
		return 0;

	case WM_CREATE:
		MainWindow.SetHWnd(hWnd);
		return 0;

	case WM_MOVE:
		MainWindow.OnMove();
		break;

	case WM_SIZE:
		MainWindow.OnSize();
		SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		break;

	case WM_LBUTTONDOWN:
		if(::bBrowseMode)
		{
			SetCursor(MainWindow.hCr_Hand);
			GetCursorPos(&prvPoint);
		}
		else
		{
			MainWindow.DecWord();
			SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		}
		break;

	case WM_LBUTTONUP:
		if(::bBrowseMode)
		{
			SetCursor(MainWindow.hCr_Norm);
			POINT curPoint;
			GetCursorPos(&curPoint);
			if(prvPoint.x == curPoint.x && prvPoint.y == curPoint.y)
				if(MainWindow.RectSelect(curPoint))
				{
					MainWindow.SelWord();
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				}
		}
		break;

	case WM_MOUSEMOVE:
		{
			GetCursorPos(&curPoint);

			if(wParam & MK_LBUTTON)
			{
				UINT speedFactor;
				if(wParam & MK_CONTROL)
					speedFactor = SPEED_FACTOR_MIN;
				else if(wParam & MK_SHIFT)
					speedFactor = SPEED_FACTOR_MAX;
				else
					speedFactor = SPEED_FACTOR_MED;
				speedFactor = (UINT)(ceil(MainWindow.browseScale*speedFactor));
				if(curPoint.x < prvPoint.x)
				{
					MainWindow.browseX -= speedFactor;
					Settings.SetValue(BROWSE_X, MainWindow.browseX);
				}
				else if(curPoint.x > prvPoint.x)
				{
					MainWindow.browseX += speedFactor;
					Settings.SetValue(BROWSE_X, MainWindow.browseX);
				}
				if(curPoint.y < prvPoint.y)
				{
					MainWindow.browseY -= speedFactor;
					Settings.SetValue(BROWSE_Y, MainWindow.browseY);
				}
				else if(curPoint.y > prvPoint.y)
				{
					MainWindow.browseY += speedFactor;
					Settings.SetValue(BROWSE_Y, MainWindow.browseY);
				}
			}
			else
				prvPoint = curPoint;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			POINT dPoint;
			GetCursorPos(&dPoint);
			if(MainWindow.RectSelect(dPoint))
			{
				MainWindow.SelWord();
				MainWindow.ToggleBrowseMode();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
			}
		}
		break;

	case WM_RBUTTONDOWN:
		MainWindow.IncWord();
		SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
		break;

	case WM_MOUSEWHEEL:
		if((short)HIWORD(wParam) < 0)
		{
			if((LOWORD(wParam) & MK_CONTROL)&&(CHECK_KEY & GetKeyState(VK_MENU))&&(LOWORD(wParam) & MK_SHIFT))
			{
				MainWindow.IncWindowH();
				MainWindow.OnSize();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
			}
			else if(::bBrowseMode)
			{
				float speedFactor;
				if(LOWORD(wParam) & MK_CONTROL)
					speedFactor = WHEEL_SPEED_FACTOR_MIN;
				else if(LOWORD(wParam) & MK_SHIFT)
					speedFactor = WHEEL_SPEED_FACTOR_MAX;
				else
					speedFactor = WHEEL_SPEED_FACTOR_MED;
				if(CHECK_KEY & GetKeyState(VK_MENU))
				{
					if(MainWindow.browseScale - speedFactor > 0)
					{
						MainWindow.browseScale -= speedFactor;
						Settings.SetFValue(BROWSE_SCALE, MainWindow.browseScale);
						//MainWindow.BrowseCenter();
					}
				}
				else
				{
					MainWindow.browseY -= (UINT)ceil(MainWindow.browseScale*speedFactor*VSCROLL_FACTOR);
				}
			}
			else
			{
				if(CHECK_KEY & GetKeyState(VK_MENU))
					MainWindow.ZoomOut();
				else
				{
					MainWindow.IncLine();
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				}
			}
		}
		else
		{
			if((LOWORD(wParam) & MK_CONTROL)&&(CHECK_KEY & GetKeyState(VK_MENU))&&(LOWORD(wParam) & MK_SHIFT))
			{
				MainWindow.DecWindowH();
				MainWindow.OnSize();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
			}
			else if(::bBrowseMode)
			{
				float speedFactor;
				if(LOWORD(wParam) & MK_CONTROL)
					speedFactor = WHEEL_SPEED_FACTOR_MIN;
				else if(LOWORD(wParam) & MK_SHIFT)
					speedFactor = WHEEL_SPEED_FACTOR_MAX;
				else
					speedFactor = WHEEL_SPEED_FACTOR_MED;
				if(CHECK_KEY & GetKeyState(VK_MENU))
				{
					MainWindow.browseScale += speedFactor;
					Settings.SetFValue(BROWSE_SCALE, MainWindow.browseScale);
					//MainWindow.BrowseCenter();
				}
				else
				{
					MainWindow.browseY += (UINT)(ceil(MainWindow.browseScale*speedFactor)*VSCROLL_FACTOR);
				}
			}
			else
			{
				if(CHECK_KEY & GetKeyState(VK_MENU))
					MainWindow.ZoomIn();
				else
				{
					MainWindow.DecLine();
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				}
			}
		};
		break;

	//case WM_SETCURSOR:
		//if(bBrowseMoving)
		//	SetCursor(::hCr_Hand);
		//else
		//	SetCursor(
		//break;

	case WM_SYSCHAR:
		switch(wParam)
		{
		case VK_RETURN:
#pragma region F1
			MainWindow.ToggleFullScreen();
			return 0;
#pragma endregion
		case VK_SPACE:
			MainWindow.IncHMargin();
			break;
		case 'a':
		case 'A':
			MainWindow.ToggleAutoScale();
			break;
		case 'i':
		case 'I':
			MainWindow.bShowIndex = !MainWindow.bShowIndex;
			break;
		case 's':
		case 'S':
			MainWindow.AutoScale();
			break;
		case 'b':
		case 'B':
			::bIBMode = !::bIBMode;
			Settings.SetValue(BW_MODE, ::bIBMode);
			break;
		case 'c':
		case 'C':
			if(!::bDoGrayScale && !::bGReady)
			{
				MainWindow.DoGrayThread();
				::bIGMode = TRUE;
			}
			else
				::bIGMode = !::bIGMode;
			Settings.SetValue(GRAY_MODE, ::bIGMode);
			break;
		case 'e':
		case 'E':
			::bExtents = !::bExtents;
			Settings.SetValue(DRAW_EXTENTS, ::bExtents);
			break;
		case 'f':
		case 'F':
			::bRects = !::bRects;
			MainWindow.RenderRects();
			break;
		case 'g':
		case 'G':
			::bBullets = !::bBullets;
			MainWindow.RenderBullets();
		case 'x':
		case 'X':
			MainWindow.bCurExtent = !MainWindow.bCurExtent;
			break;
		case 't':
		case 'T':
			MainWindow.ToggleTextWindow();
			//break;
		}
		break;
	//case WM_COMMAND:
	//	{
	//		switch(LOWORD(wParam))
	//		{
	//		}
	//	}
	//	break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				if(MainWindow.bBreaking)
					MainWindow.bBreaking = FALSE;
				else
					MainWindow.OnEsc();
				break;
			case VK_RETURN:
				if(MainWindow.bExtendingL)
				{
					MainWindow.bExtendingL = FALSE;
					MainWindow.OnEdit(TRUE, FALSE);
				}
				else if(MainWindow.bExtendingR)
				{
					MainWindow.bExtendingR = FALSE;
					MainWindow.OnEdit(TRUE, FALSE);
				}
				else if(MainWindow.bBreaking)
				{
					MainWindow.ObjBreak();
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				}
				break;
			case VK_UP:
				MainWindow.DecLine();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_DOWN:
				//if(CHECK_KEY & GetKeyState(VK_CONTROL))
				//{
				//	MainWindow.DecWSync();
				//	return 0;
				//}
				MainWindow.IncLine();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_RIGHT:
				if(MainWindow.bExtendingL)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 1;
					else if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 6;
					else
						l = 3;
					MainWindow.ObjBeginXInc(l);
				}
				else if(MainWindow.bExtendingR)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 1;
					else if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 6;
					else
						l = 3;
					MainWindow.ObjEndXInc(l);
				}
				else if(MainWindow.bBreaking)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = OBJECT_BREAK_HOP;
					else
						l = 1;
					MainWindow.XWidthInc(l);
				}
				else
				{
					//if(CHECK_KEY & GetKeyState(VK_CONTROL))
					//	MainWindow.IncWSync();
					//else
					//{
						MainWindow.IncWord();
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
					//}
				}
				break;
			case VK_LEFT:
				if(MainWindow.bExtendingL)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 1;
					else if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 6;
					else
						l = 3;
					MainWindow.ObjBeginXDec(l);
				}
				else if(MainWindow.bExtendingR)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 1;
					else if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = 6;
					else
						l = 3;
					MainWindow.ObjEndXDec(l);
				}
				else if(MainWindow.bBreaking)
				{
					UINT l;
					if(CHECK_KEY & GetKeyState(VK_CONTROL))
						l = OBJECT_BREAK_HOP;
					else
						l = 1;
					MainWindow.XWidthDec(l);
				}
				else
				{
					//if(CHECK_KEY & GetKeyState(VK_CONTROL))
					//	MainWindow.DecWSync();
					//else
					//{
						MainWindow.DecWord();
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
					//}
				}
				break;
			case VK_HOME:
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
				{
					MainWindow.XHome();
					if(lParam)
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
						//SendMessage(::hWndEdit, WM_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)FALSE);
				}
				else
				{
					MainWindow.Home();
					if(lParam)
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
						//SendMessage(::hWndEdit, WM_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)FALSE);
				}
				break;
			case VK_END:
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
				{
					MainWindow.XEnd();
					if(lParam)
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
						//SendMessage(::hWndEdit, WM_KEYDOWN, (WPARAM)VK_END, (LPARAM)FALSE);
				}
				else
				{
					MainWindow.End();
					if(lParam)
						SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
						//SendMessage(::hWndEdit, WM_KEYDOWN, (WPARAM)VK_END, (LPARAM)FALSE);
				}
				break;
			case VK_PRIOR:
				if(MainWindow.PageUp())
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);	// SendMessage(::hWndEdit, M_REF_WORD_I, 0, 0);
				break;
			case VK_NEXT:
				if(MainWindow.PageDown())
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);	// SendMessage(::hWndEdit, M_REF_WORD_I, 0, 0);
				break;
			case VK_SPACE:
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
					MainWindow.DecHMargin();
				else if(!MainWindow.bBreaking && !MainWindow.bExtendingL && !MainWindow.bExtendingR)
					MainWindow.ToggleTimer();
				else
					MessageBeep(MB_ICONASTERISK);
				break;
			case VK_DELETE:
				{
				BOOL bPrev, bOEdit, bTEdit;
				if(CHECK_KEY & GetKeyState(VK_SHIFT))
					bPrev = TRUE, bOEdit = TRUE, bTEdit = TRUE;
				else
					bPrev = FALSE, bOEdit = TRUE, bTEdit = FALSE;
				MainWindow.ObjDelete(bPrev, bOEdit, bTEdit);
				}
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_OEM_COMMA:
				//MainWindow.DecWSync();
				break;
			case VK_OEM_PERIOD:
				//MainWindow.IncWSync();
				break;
			case VK_DIVIDE:
				MainWindow.AutoBrowseSWidth();
				MainWindow.BrowseCenterCur();
				break;
			case VK_MULTIPLY:
				MainWindow.BrowseCenterCur();
				break;
			case VK_ADD:
				MainWindow.AutoBrowseSWidth();
				break;
			case VK_SUBTRACT:
				MainWindow.AutoBrowseSHeight();
				break;
			case '0':
			case VK_NUMPAD0:
				MainWindow.AutoBrowseScale();
				break;
			case '1':
			case VK_NUMPAD1:
				MainWindow.BrowseLeftBottom();
				break;
			case '2':
			case VK_NUMPAD2:
				MainWindow.BrowseBottomCenter();
				break;
			case '3':
			case VK_NUMPAD3:
				MainWindow.BrowseRightBottom();
				break;
			case '4':
			case VK_NUMPAD4:
				MainWindow.BrowseLeftCenter();
				break;
			case '5':
			case VK_NUMPAD5:
				MainWindow.BrowseCenter();
				break;
			case '6':
			case VK_NUMPAD6:
				MainWindow.BrowseRightCenter();
				break;
			case '7':
			case VK_NUMPAD7:
				MainWindow.BrowseLeftTop();
				break;
			case '8':
			case VK_NUMPAD8:
				MainWindow.BrowseTopCenter();
				break;
			case '9':
			case VK_NUMPAD9:
				MainWindow.BrowseRightTop();
				break;
			//case 'b':
			case 'B':
				MainWindow.bExtendingR = TRUE;
				break;
			//case 'i':
			case 'I':
				SplashWindow.BeginFade();
				SplashWindow.Show();
				break;
			//case 'm':
			case 'M':
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
					::bMaskEnable = !::bMaskEnable;
				else
				{
					{
					BOOL bPrev;
					if(CHECK_KEY & GetKeyState(VK_SHIFT))
						bPrev = FALSE;
					else
						bPrev = TRUE;
					MainWindow.ObjMerge(bPrev);
					}
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				}
				break;
			case 'n':
			case 'N':
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
					MainWindow.ToggleMargin();
				else
					MainWindow.InitBreak();
				break;
			case 's':
			case 'S':
				if(CHECK_KEY & GetKeyState(VK_CONTROL))
				{
					if((CHECK_KEY & GetKeyState(VK_LMENU)) || (CHECK_KEY & GetKeyState(VK_RMENU)))
						Settings.Save();
					else
						MainWindow.OnSave();
				}
				break;
			case 'v':
			case 'V':
				MainWindow.bExtendingL = TRUE;
				break;
			case 'z':
			case 'Z':
				MainWindow.ToggleBrowseMode();
				SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_TAB:
				if(::bTextMode)
				{
					if(!MainWindow.bBreaking && !MainWindow.bExtendingL && !MainWindow.bExtendingR)
						MainWindow.FocusEditWindow();
					else
						MessageBeep(MB_ICONASTERISK);
				}
				break;
			case VK_OEM_PLUS:
				MainWindow.OnClose();
				if(MainWindow.Load(TRUE))
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_OEM_MINUS:
				MainWindow.OnClose();
				if(MainWindow.Load(FALSE))
					SendMessage(::hWndEdit, CM_UPDATE, 0, 0);
				break;
			case VK_RIGHT_BRACKET:
				MainWindow.DecTDelay();
				break;
			case VK_LEFT_BRACKET:
				MainWindow.IncTDelay();
				break;
			default:
				return 0;
			}
		}
		break;
	case WM_SYSCOMMAND:
		if(wParam == SC_KEYMENU)
			return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
	return 0;
}
