# include "ProofEditor.h"

# define WILD_ALL					L"*."

# define MAX_N_FILES				500

# define MAX_SENTENCE_LEN_PEL		2000

# define MAX_PATH_LEN				1000
//# define MAX_N_SYMBOLS_LINE			100

//# define MARGIN_PEN_COLOUR			RGB(0, 255, 0)

# define ERROR_VALIDATION			0x001e

# define S_MESSAGE_TITLE				L"Settings Error"
# define INVALID_REG_SETTINGS			0x0000103
# define S_ERROR_INVALID_REG_SETTINGS	L"Invalid Registry Sttings"

HINSTANCE hInstance;

BOOL bMaskEnable = TRUE;
BOOL bTextMode;
BOOL bBrowseMode;
BOOL bIBMode;
BOOL bIGMode;
BOOL bGReady;
BOOL bExtents;
BOOL bRects;
BOOL bBullets;
BOOL bDoGrayScale;

HWND hWndMain;
HWND hWndEdit;

LocaleClass				Locale;
SettingsClass			Settings(HKEY_CURRENT_USER, SOFTWARE_REG_KEY SETTINGS_KEY, SETTINGS_ID_BASE, SETTINGS_N_VAR, SETTINGS_N_FLOAT, SETTINGS_N_STRING);
NotifyMessageBoxClass	NotifyMessageBox;
MainWindowClass			MainWindow(hWndMain);
EditWindowClass			EditWindow(hWndEdit);
SplashWindowClass		SplashWindow;
ImageClass				Img;
FilesClass				Files;

PageClass				Page;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nShowCmd)
{

	if(!Settings.Load())
	{
		MessageBox(NULL, S_ERROR_INVALID_REG_SETTINGS, S_MESSAGE_TITLE, MB_OK);
		exit(INVALID_REG_SETTINGS);
	}

	::hInstance = hInstance;
	::bTextMode = (Settings.GetValue(TEXT_MODE) != 0);
	::bBrowseMode = (Settings.GetValue(BROWSE_MODE) != 0);
	MSG msg;

	if(lpCmdLine == NULL || lpCmdLine == L"" || lpCmdLine[0] == NULL)
		Files.OpenDialog(ORT_FILE_EXTN);
	else
		Files.InitLoadFileNames(lpCmdLine, WILD_ALL ORT_FILE_EXTN, TRUE);

	MainWindow.Create();
	EditWindow.Create();
	SplashWindow.Create(hInstance, IDI_PROOFEDITOR_ICON, ::hWndMain);

	NotifyMessageBox.Set_hWnd(::hWndMain);

	if(Settings.GetValue(SHOW_SPLASH))
	{
		SplashWindow.BeginFade();
		SplashWindow.Show();
	}
	ShowWindow(::hWndMain, SW_SHOWNORMAL);
	ShowWindow(::hWndEdit, SW_SHOWNORMAL);

	//HACCEL hAccel = LoadAccelerators(hInstance, ACCEL_PE);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//if(!TranslateAccelerator(MainWindow.hWnd, hAccel, &msg))
		//{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//}
	};
	return 0;
}
