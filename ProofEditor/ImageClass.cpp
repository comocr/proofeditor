# include "ImageClass.h"

using namespace Gdiplus;

# define GIF_BW_PALETTE_N	16

# define GRAY_RED_PC		0.30
# define GRAY_GREEN_PC		0.59
# define GRAY_BLUE_PC		0.11

# define GIF_BW_PALETTE_N	16

# define GRAY_RED_PC		0.30
# define GRAY_GREEN_PC		0.59
# define GRAY_BLUE_PC		0.11

extern SettingsClass Settings;
extern BOOL bGReady;
extern BOOL bDoGrayScale;
//extern BOOL bRects__bBullets;

ImageClass::ImageClass()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

ImageClass::~ImageClass()
{
	GdiplusShutdown(gdiplusToken);
}

BOOL ImageClass::LoadHBMP(LPCWSTR filePath)
{
	static Bitmap img(filePath);
	if(img.GetHBITMAP(Color::White, &hImageI) != Ok)
		return FALSE;
		//(HBITMAP)LoadImage(NULL, Files.CurName(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	img.GetHBITMAP(Color::White, &hImageX);
		//Page->hImageB = (HBITMAP)CopyImage(Page->hImageI, IMAGE_BITMAP, 0, 0, LR_MONOCHROME);

	BITMAP bm;
	GetObject(hImageI, sizeof(BITMAP), &bm);
	width = bm.bmWidth;
	height = bm.bmHeight;
	for(int i = 0, n = -1; i < bm.bmHeight; i++)
		for(int j = 0; j < bm.bmWidth; j++)
		{
			n += 4; // Once per Pixel of 4 Bytes
			((LPBYTE)bm.bmBits)[n] = 0;
		}
	HDC hDC;
	hDC = GetDC(NULL);
	HDC hDCMem = CreateCompatibleDC(hDC);
	HDC hDCSrc = CreateCompatibleDC(hDC);
	hImageB = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	SelectObject(hDCMem, hImageB);
	SelectObject(hDCSrc, hImageI);
	SetBkColor(hDCSrc, RGB(0xFF, 0xFF, 0xFF));
	BitBlt(hDCMem, 0, 0, width, height, hDCSrc, 0, 0, SRCCOPY);
	DeleteDC(hDCMem);
	DeleteDC(hDCSrc);
	ReleaseDC(NULL, hDC);	

/*	::bGReady = FALSE;
	if(::bDoGrayScale)
		if(img.GetPaletteSize() != GIF_BW_PALETTE_N)
			DoGrayThread();
*/
	return TRUE;
}

VOID ImageClass::CleanUp()
{
	if(hImageI)
		DeleteObject(hImageI);
	if(hImageX)
		DeleteObject(hImageX);
	if(hImageI)
		DeleteObject(hImageI);
	if(hImageB)
		DeleteObject(hImageB);
	if(hImageG)
		DeleteObject(hImageG);
	if(hBMRects)
		DeleteObject(hBMRects);
	if(hIRects)
		DeleteObject(hIRects);
	if(hBRects)
		DeleteObject(hBRects);
	if(hGRects)
		DeleteObject(hGRects);
}

DWORD WINAPI ImageClass::MakeBW(ImageClass* Image)
{
	HDC hDC;
	HDC hDCMem, hDCSrc;
	hDC = GetDC(NULL);
	hDCMem = CreateCompatibleDC(NULL);
	hDCSrc = CreateCompatibleDC(NULL);

	Image->hImageG = CreateCompatibleBitmap(hDC, Image->width, Image->height);//, 1, 32, NULL);
	SelectObject(hDCSrc, Image->hImageX);
	SelectObject(hDCMem, Image->hImageG);
//	Image->syncHeight = Image->height;
//	Image->syncWidth = Image->width;

	PatBlt(hDCMem, 0, 0, Image->width, Image->height, WHITENESS);
	for(UINT i = 0; i < Image->/*syncW*/width; i++)
		for(UINT j = 0; j < Image->/*syncH*/height; j++)
		{
			//SetPixel(hDCMem, i, j, GetPixel(hDCSrc, i, j)==0xFFFFFF? RGB(0xFF, 0xFF, 0xFF) : RGB(0x00, 0x00, 0x00));
			UINT p = GetPixel(hDCSrc, i, j);
			UINT g = UINT(GetRValue(p)*GRAY_RED_PC+GetGValue(p)*GRAY_GREEN_PC+GetBValue(p)*GRAY_BLUE_PC);
			SetPixel(hDCMem, i, j, RGB(g, g, g));
		}

	DeleteDC(hDCMem);
	DeleteDC(hDCSrc);

	hDCMem = CreateCompatibleDC(NULL);
	hDCSrc = CreateCompatibleDC(NULL);
	HDC hDCRects = CreateCompatibleDC(hDC);
	SelectObject(hDCRects, Image->hBMRects);
	Image->hGRects = CreateCompatibleBitmap(hDC, Image->width, Image->height);
	SelectObject(hDCSrc, Image->hImageG);
	SelectObject(hDCMem, Image->hGRects);
	BitBlt(hDCMem, 0, 0, Image->width, Image->height, hDCRects, 0, 0, SRCCOPY);
	BitBlt(hDCMem, 0, 0, Image->width, Image->height, hDCSrc, 0, 0, SRCAND);

	DeleteDC(hDCRects);
	DeleteDC(hDCMem);
	DeleteDC(hDCSrc);

	ReleaseDC(NULL, hDC);
	::bGReady = TRUE;
	return TRUE;
}

VOID ImageClass::SelectRects()
{
	hDC = GetDC(NULL);
	hDCRects = CreateCompatibleDC(hDC);
	if(hBMRects)
		DeleteObject(hBMRects);
	hBMRects = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hDCRects, hBMRects);
	PatBlt(hDCRects, 0, 0, width, height, WHITENESS); 

	COLORREF clr;
	//if(::bRects__bBullets)
		clr = Settings.GetValue(MARGIN_COLOUR);
	//else
	//	clr = Settings.GetValue(BULLET_COLOUR);
	hPen = CreatePen(PS_SOLID, 1, clr);
	hBrush = CreateSolidBrush(clr);
	SelectObject(hDCRects, hBrush);
	SelectObject(hDCRects, hPen);
}

VOID ImageClass::ReleaseRects()
{
	DeleteObject(hBrush);
	DeleteObject(hPen);
	ReleaseDC(NULL, hDC);
	DeleteDC(hDCRects);
	PaintRects();
}

VOID ImageClass::RenderRect(POINT *rect)
{
	Polyline(hDCRects, rect, 3);
}

VOID ImageClass::RenderBullet(POINT *extn)
{
	Ellipse(hDCRects, extn[0].x, extn[0].y, extn[1].x, extn[1].y);
}

VOID ImageClass::PaintRects()
{
	HDC hDC = GetDC(NULL);
	HDC hDCRects = CreateCompatibleDC(hDC);
	HDC hDCImg = CreateCompatibleDC(hDC);
	HDC hDCMem = CreateCompatibleDC(hDC);
	SelectObject(hDCRects, hBMRects);

	if(::bGReady)
	{
		if(hGRects)
			DeleteObject(hGRects);
		hGRects = CreateCompatibleBitmap(hDC, width, height);
		SelectObject(hDCImg, hImageG);
		SelectObject(hDCMem, hGRects);
		BitBlt(hDCMem, 0, 0, width, height, hDCRects, 0, 0, SRCCOPY);
		BitBlt(hDCMem, 0, 0, width, height, hDCImg, 0, 0, SRCAND);
	}

	if(hIRects)
		DeleteObject(hIRects);
	hIRects = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hDCImg, hImageI);
	SelectObject(hDCMem, hIRects);
	BitBlt(hDCMem, 0, 0, width, height, hDCRects, 0, 0, SRCCOPY);
	BitBlt(hDCMem, 0, 0, width, height, hDCImg, 0, 0, SRCAND);
	if(hBRects)
		DeleteObject(hBRects);
	hBRects = CreateCompatibleBitmap(hDC, width, height);
	SelectObject(hDCImg, hImageB);
	SelectObject(hDCMem, hBRects);
	BitBlt(hDCMem, 0, 0, width, height, hDCRects, 0, 0, SRCCOPY);
	BitBlt(hDCMem, 0, 0, width, height, hDCImg, 0, 0, SRCAND);

	DeleteDC(hDCRects);
	DeleteDC(hDCImg);
	DeleteDC(hDCMem);
	ReleaseDC(NULL, hDC);
}
