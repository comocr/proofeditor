# ifndef IMAGE_CLASS_HEADER
# define IMAGE_CLASS_HEADER

# include "Lang\\CommonLangIDs.h"

# include <Windows.h>
# include <GDIPlus.h>

# include "..\\Utilities\\NotifyMessageBox\\NotifyMessageBoxClass.h"
# include "..\\Utilities\\Settings\\SettingsClass.h"
# include "..\\Utilities\\Image\\ImageClass_0.h"

class ImageClass : public ImageClass_0
{
private:
//	ULONG_PTR gdiplusToken;

public:
	ImageClass();
	~ImageClass();
	VOID CleanUp();
	HBITMAP hImageX;			// Copy for hImageBMP
	HBITMAP hImageI;
	HBITMAP hImageB;
	HBITMAP hImageG;
	HBITMAP hBMRects;
	HBITMAP hIRects;
	HBITMAP hBRects;
	HBITMAP hGRects;

	HDC hDC;
	HDC hDCRects;
	HPEN hPen;
	HBRUSH hBrush;

	BOOL LoadHBMP(LPCWSTR filePath);
	static DWORD WINAPI MakeBW(ImageClass* Image);
	VOID SelectRects();
	VOID ReleaseRects();
	VOID RenderRect(POINT *rect);
	VOID RenderBullet(POINT *extn);
	VOID PaintRects();
	VOID DoThread();
};

# endif // IMAGE_CLASS_HEADER
