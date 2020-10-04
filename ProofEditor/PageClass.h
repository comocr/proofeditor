# ifndef PAGE_CLASS_HEADER
# define PAGE_CLASS_HEADER

# include "..\\..\\Engine\\Engine\\PageClass_1.h"
//# include "ImageClass2.h"
# include "TextGroupClass.h"
# include "TextGroupLoadClass.h"
# include "..\\Utilities\\Settings\\SettingsClass.h"

class PageClass : public PageClass_1
{
private:
	LPWSTR imgFileExtn;

//	UINT width;
//	UINT height;

public:
	UINT iGroup;
	UINT iSection;
	UINT iLine;
	UINT iWord;

	UINT* yAxes;

	BOOL bTChanged;
	BOOL bOChanged;

	UINT nEditToSave;
	UINT nPEdit;
	UINT nOEdit;

	BOOL bText;
	HANDLE hThread;

	GLayoutClass* GLayout;		// Level up in Engine
	TextGroupClass* TxG;

	PageClass();
	~PageClass();
	VOID CleanUp();
	VOID LoadPage();
	VOID SavePage();
	VOID LoadTextBuffer(GLayoutClass* GLayout);

	UINT ObjBeginX();
	UINT ObjBeginY();
	UINT ObjEndX();
	UINT ObjEndY();
	UINT ObjWidth();
	UINT ObjHeight();

	UINT ObjBeginX(UINT iLine, UINT iWord);
	UINT ObjBeginY(UINT iLine, UINT iWord);
	UINT ObjEndX(UINT iLine, UINT iWord);
	UINT ObjEndY(UINT iLine, UINT iWord);
	UINT ObjWidth(UINT iLine, UINT iWord);
	UINT ObjHeight(UINT iLine, UINT iWord);

	UINT ObjBeginX(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjBeginY(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndX(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndY(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjWidth(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjHeight(UINT iSection, UINT iLine, UINT iWord);

	UINT ObjBeginX(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);
	UINT ObjBeginY(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndX(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndY(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);
	UINT ObjWidth(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);
	UINT ObjHeight(UINT iGroup, UINT iSection, UINT iLine, UINT iWord);

	VOID ObjBeginXInc(UINT d);
	VOID ObjBeginXDec(UINT d);
	VOID ObjEndXInc(UINT d);
	VOID ObjEndXDec(UINT d);

	VOID ObjBreak(UINT xWidth);
	VOID ObjMerge(BOOL bNext);
	VOID ObjDelete(BOOL bText);

	VOID ObjSelect(UINT iGroup, UINT iSection, UINT iLine, UINT iLWord);

	LPWSTR LineText();
	UINT LineLen();
	VOID LineLenInc();
	WCHAR LineChar();

	VOID RenderRects();
	VOID RenderBullets();
	VOID DoGrayThread();

	HBITMAP HImageI();
	HBITMAP HImageB();
	HBITMAP HImageG();
	HBITMAP HBRects();
	HBITMAP HGRects();
	HBITMAP HIRects();

	VOID SelWord();
	VOID RefWord();

	BOOL IncWord();
	BOOL DecWord();
	BOOL IncLine();
	BOOL DecLine();
	BOOL Home();
	BOOL XHome();
	BOOL End();
	BOOL XEnd();
	BOOL PageUp();
	BOOL PageDn();

	UINT IBChar();
	UINT IEChar();
	UINT IXChar();

	VOID IncChar();
	VOID DecChar();

	VOID AddChar(WCHAR c);
	VOID DelChar(BOOL bPrev);

	UINT LineEndY();
	UINT LineHeight();

	UINT Width();
	UINT Height();
};

# endif // PAGE_CLASS_HEADER
