# ifndef TEXT_GROUP_LOAD_CLASS_HEADER
# define TEXT_GROUP_LOAD_CLASS_HEADER

# include "TextGroupClass.h"
# include "TextLineLoadClass.h"

class TextGroupLoadClass
{
private:
	TextGroupClass* pTxG;
	TextLineLoadClass TxLL;

	UINT iLine;

public:
	
	VOID Change(TextGroupClass* pTxG);
	VOID Init(UINT nLines);
	VOID Create();
	VOID Dummy();
	VOID Put(WCHAR c);
	VOID Space();
	VOID EndLine();
	VOID NewLine();
};

# endif // TEXT_GROUP_LOAD_CLASS_HEADER
