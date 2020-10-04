# include "TextGroupClass.h"

LPWSTR TextGroupClass::LineText(UINT iLine)
{
	return TxL[iLine].LineText();
}

UINT TextGroupClass::LineLen(UINT iLine)
{
	return TxL[iLine].LineLen();
}

VOID TextGroupClass::LineLenInc(UINT iLine)
{
	TxL[iLine].LineLenInc();
}

WCHAR TextGroupClass::LineChar(UINT iLine)
{
	return TxL[iLine].LineChar();
}

VOID TextGroupClass::SelWord(UINT iLine, UINT iWord)
{
	TxL[iLine].SelWord(iWord);
}
	
VOID TextGroupClass::BrkWord(UINT iLine, FLOAT f)
{
	TxL[iLine].BrkWord(f);
}

VOID TextGroupClass::MrgWord(UINT iLine)
{
	TxL[iLine].MrgWord();
}

VOID TextGroupClass::DelWord(UINT iLine)
{
	TxL[iLine].DelWord();
}

VOID TextGroupClass::RefWord(UINT iLine)
{
	TxL[iLine].RefWord();
}

VOID TextGroupClass::IncWord(UINT iLine)
{
	TxL[iLine].IncWord();
}

VOID TextGroupClass::DecWord(UINT iLine)
{
	TxL[iLine].DecWord();
}

VOID TextGroupClass::Home(UINT iLine)
{
	TxL[iLine].Home();
}

VOID TextGroupClass::End(UINT iLine)
{
	TxL[iLine].End();
}

VOID TextGroupClass::XHome()
{
	TxL[0].Home();
}

VOID TextGroupClass::XEnd(UINT iLine)
{
	TxL[iLine].End();
}

UINT TextGroupClass::IBChar(UINT iLine)
{
	return TxL[iLine].IBChar();
}

UINT TextGroupClass::IEChar(UINT iLine)
{
	return TxL[iLine].IEChar();
}

UINT TextGroupClass::IXChar(UINT iLine)
{
	return TxL[iLine].IXChar();
}

VOID TextGroupClass::IncChar(UINT iLine)
{
	TxL[iLine].IncChar();
}

VOID TextGroupClass::DecChar(UINT iLine)
{
	TxL[iLine].DecChar();
}

VOID TextGroupClass::AddChar(UINT iLine, WCHAR c)
{
	TxL[iLine].AddChar(c);
}

VOID TextGroupClass::DelChar(UINT iLine, BOOL bPrev)
{
	TxL[iLine].DelChar(bPrev);
}

VOID TextGroupClass::IncLine(UINT iLine)
{
	TxL[iLine].Home();
}

VOID TextGroupClass::DecLine(UINT iLine)
{
	TxL[iLine].Home();
}

VOID TextGroupClass::PageUp(UINT iLine)
{
	TxL[iLine].Home();
}

VOID TextGroupClass::PageDn(UINT iLine)
{
	TxL[iLine].Home();
}
