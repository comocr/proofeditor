# ifndef TEXT_LINE_LOAD_CLASS_HEADER
# define TEXT_LINE_LOAD_CLASS_HEADER

# include "TextLineClass.h"

class TextLineLoadClass
{
private:
	TextLineClass* pTxL;
	UINT iChar;

public:
	VOID Change(TextLineClass* pTxL);

	VOID Create();
	VOID Dummy();
	VOID Put(WCHAR c);
	VOID Space();
	VOID EndLine();
};

# endif TEXT_LINE_LOAD_CLASS_HEADER
