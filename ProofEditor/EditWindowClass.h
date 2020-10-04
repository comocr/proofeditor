# ifndef EDIT_WINDOW_CLASS_HEADER
# define EDIT_WINDOW_CLASS_HEADER

# include "Resource.h"

# pragma comment(lib, "Msimg32.lib")

# include "..\\Utilities\\Locale\\LocaleClass.h"
# include "..\\Utilities\\NotifyMessageBox\\NotifyMessageBoxClass.h"
# include "..\\Utilities\\Settings\\SettingsClass.h"
# include "MainWindowClass.h"
# include "PageClass.h"

LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class EditWindowClass
{
private:
	int xCur;           // horizontal coordinate of cursor 
	int yCur;           // vertical coordinate of cursor 
	int caretWidth;
	int caretHeight;
	RECT prc;
	UINT width;
	UINT height;
	UINT xOffset;
	UINT lMaskEnd;
	UINT rMaskBegin;
	UINT wordMargin;
	BLENDFUNCTION blend_function;
	WNDCLASSEX xc;
	LPWSTR szWindowClass;
	LPWSTR szWindowTitle;
	HDC hDC;
	HFONT hfFont;
	HWND& hWnd;

public:

	EditWindowClass(HWND& hWnd);
	~EditWindowClass();
	VOID SetHWnd(HWND hWnd);
	VOID Create();
	VOID OnInit();
	VOID OnSize();
	VOID OnPaint();
	VOID OnSave();
	VOID Update();
	VOID OnRight();
	VOID OnLeft();
	VOID OnSetFocus();
	VOID FocusMainWindow();
	VOID OnHome();
	VOID OnEnd();
};

# endif // EDIT_WINDOW_CLASS_HEADER
