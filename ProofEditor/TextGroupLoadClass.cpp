# include "TextGroupLoadClass.h"

# define SPACE					L' '
# define MAX_LINE_BUFFER_SIZE	1000

VOID TextGroupLoadClass::Change(TextGroupClass* pTxG)
{
	TextGroupLoadClass::pTxG = pTxG;
	iLine = 0;
}

VOID TextGroupLoadClass::Init(UINT nLine)
{
	pTxG->nLine = nLine;
	pTxG->TxL = new TextLineClass[nLine];
}

VOID TextGroupLoadClass::Create()
{
	for(UINT iLine = 0; iLine < pTxG->nLine; iLine++)
	{
		TxLL.Change(&pTxG->TxL[iLine]);
		TxLL.Create();
	}
	TxLL.Change(&pTxG->TxL[0]);
}

VOID TextGroupLoadClass::Dummy()
{
	for(UINT iLine = 0; iLine < pTxG->nLine; iLine++)
	{
		TxLL.Change(&pTxG->TxL[iLine]);
		TxLL.Dummy();
	}
}

VOID TextGroupLoadClass::Put(WCHAR c)
{
	TxLL.Put(c);
}

VOID TextGroupLoadClass::Space()
{
	TxLL.Space();
}

VOID TextGroupLoadClass::EndLine()
{
	TxLL.EndLine();
}

VOID TextGroupLoadClass::NewLine()
{
	EndLine();
	iLine++;
	TxLL.Change(&((pTxG->TxL)[iLine]));
}
