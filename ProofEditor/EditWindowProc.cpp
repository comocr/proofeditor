# include "EditWindowProc.h"

# define CHECK_KEY					0x8000

extern HWND				hWndMain;
extern EditWindowClass	EditWindow;

extern BOOL				bMaskEnable;

LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_PAINT:
		EditWindow.OnPaint();
		return 0;

	case WM_ERASEBKGND:
		return 1;

	case WM_CREATE:
		EditWindow.SetHWnd(hWnd);
		EditWindow.OnInit();
		return 0;
	
	case WM_SIZE:
		EditWindow.OnSize();
		return 0;

	case CM_UPDATE:
		EditWindow.Update();
		return 0;

	case WM_SETFOCUS:
		EditWindow.OnSetFocus();
		return 0;

	case WM_KILLFOCUS: 
		DestroyCaret(); 
		return 0;

	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			EditWindow.FocusMainWindow();
			return 0;

		case VK_HOME:
			if(lParam)
				SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_HOME, (LPARAM)FALSE);
			EditWindow.Update();
			break;

		case VK_END:
			if(lParam)
				SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_END, (LPARAM)FALSE);
			EditWindow.Update();
			break;

		case VK_PRIOR:
			SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_PRIOR, (LPARAM)FALSE);
			break;
		case VK_NEXT:
			SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_NEXT, (LPARAM)FALSE);
			break;
		case 'm':
		case 'M':
			if(CHECK_KEY & GetKeyState(VK_CONTROL))
				::bMaskEnable = !::bMaskEnable;
			break;
		case 's':
		case 'S':
			SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)'s', (LPARAM)FALSE);
			break;
		case VK_TAB:
			EditWindow.FocusMainWindow();
			break;

//		case VK_INSERT:
//			break;	ToDo

		case VK_LEFT:
			if(CHECK_KEY & GetKeyState(VK_CONTROL))
				SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_LEFT, (LPARAM)FALSE);
			else
				EditWindow.OnLeft();
			break;

		case VK_RIGHT:
			if(CHECK_KEY & GetKeyState(VK_CONTROL))
				SendMessage(::hWndMain, WM_KEYDOWN, (WPARAM)VK_RIGHT, (LPARAM)FALSE);
			else
				EditWindow.OnRight();
			break;

//		case VK_UP:
//			break;

//		case VK_DOWN:
//			break;

		case VK_DELETE:
			SendMessage(::hWndMain, CM_DELCHAR, (WPARAM)FALSE, (LPARAM)FALSE);
			break;

		case VK_BACK:
			SendMessage(::hWndMain, CM_DELCHAR, (WPARAM)TRUE, (LPARAM)FALSE);
			break;

//		case VK_RETURN:
//			break;

		default:
			return 0;
		}
		break;

	case WM_CHAR:
		{
			switch(wParam)
			{
			case VK_BACK:
			case VK_TAB:
			case VK_ESCAPE:
			case VK_RETURN:
			case 0x0A:		//Line Feed
				break;
			default:
				if(!(CHECK_KEY & GetKeyState(VK_CONTROL)))
					SendMessage(::hWndMain, CM_ADDCHAR, wParam, (LPARAM)FALSE);
			}
		}
		break;

	default: 
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
	return 0;
}
