# include "TextLineLoadClass.h"

# define SPACE					L' '
# define MAX_LINE_BUFFER_SIZE	1000

VOID TextLineLoadClass::Change(TextLineClass* pTxL)
{
	TextLineLoadClass::pTxL = pTxL;
	iChar = 0;
}

VOID TextLineLoadClass::Create()
{
	pTxL->txlBuffer = new WCHAR[MAX_LINE_BUFFER_SIZE];
}

VOID TextLineLoadClass::Dummy()
{
	pTxL->txlBuffer = new WCHAR(NULL);			//txlBuffer[iLine][0] = NULL;
}

VOID TextLineLoadClass::Put(WCHAR c)
{
	(pTxL->txlBuffer)[iChar++] = c;
}

VOID TextLineLoadClass::Space()
{
	(pTxL->txlBuffer)[iChar++] = SPACE;
}

VOID TextLineLoadClass::EndLine()
{
	pTxL->lLength = iChar;
	(pTxL->txlBuffer)[iChar] = NULL;
	iChar = 0;
	pTxL->iBChar = 0;
	pTxL->iXChar = 0;
	pTxL->CalcIEChar();
}
