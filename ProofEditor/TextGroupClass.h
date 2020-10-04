# ifndef TEXT_GROUP_CLASS_HEADER
# define TEXT_GROUP_CLASS_HEADER

# include <Windows.h>

# include "TextLineClass.h"

class TextGroupClass
{
friend class TextGroupLoadClass;

private:
	TextLineClass* TxL;
	UINT nLine;

public:
	VOID SelWord(UINT iLine, UINT iWord);

	VOID BrkWord(UINT iLine, FLOAT f);
	VOID MrgWord(UINT iLine);
	VOID DelWord(UINT iLine);

	LPWSTR LineText(UINT iLine);
	UINT LineLen(UINT iLine);
	VOID LineLenInc(UINT iLine);
	WCHAR LineChar(UINT iLine);

	VOID RefWord(UINT iLine);
	VOID IncWord(UINT iLine);
	VOID DecWord(UINT iLine);

	VOID Home(UINT iLine);
	VOID End(UINT iLine);
	VOID XHome();
	VOID XEnd(UINT iLine);

	UINT IBChar(UINT iLine);
	UINT IEChar(UINT iLine);
	UINT IXChar(UINT iLine);

	VOID IncChar(UINT iLine);
	VOID DecChar(UINT iLine);

	VOID AddChar(UINT iLine, WCHAR c);
	VOID DelChar(UINT iLine, BOOL bPrev);

	VOID IncLine(UINT iLine);
	VOID DecLine(UINT iLine);
	VOID PageUp(UINT iLine);
	VOID PageDn(UINT iLine);
};

# endif // TEXT_GROUP_CLASS_HEADER
