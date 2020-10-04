# ifndef TEXT_LINE_CLASS_HEADER
# define TEXT_LINE_CLASS_HEADER

# include <Windows.h>

class TextLineClass
{
friend class TextLineLoadClass;

private:
	LPWSTR txlBuffer;
	UINT nWord;
	UINT lLength;

	UINT iBChar;
	UINT iEChar;
	UINT iXChar;

	VOID CalcIEChar();

public:
	VOID SelWord(UINT iWord);

	VOID BrkWord(FLOAT f);
	VOID MrgWord();
	VOID DelWord();

	LPWSTR LineText();
	UINT LineLen();
	VOID LineLenInc();
	WCHAR LineChar();

	VOID RefWord();
	VOID IncWord();
	VOID DecWord();

	VOID Home();
	VOID End();

	UINT LLength();

	UINT IBChar();
	UINT IEChar();
	UINT IXChar();

	VOID IncChar();
	VOID DecChar();

	VOID AddChar(WCHAR c);
	VOID DelChar(BOOL bPrev);
};

# endif // TEXT_LINE_CLASS_HEADER
