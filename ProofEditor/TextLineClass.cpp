# include "TextLineClass.h"

# define SPACE	L' '

VOID TextLineClass::SelWord(UINT iWord)
{
	iBChar = 0;
	for(; iWord; iWord--)
	{
		for(; txlBuffer[iBChar] != SPACE; iBChar++)
			if(txlBuffer[iBChar] == NULL)
			{
				iWord = 0;
				iBChar = 0;
				break; // ToDo: Exception
			}
		iBChar++;
	}
	iXChar = iBChar;
	CalcIEChar();
}

VOID TextLineClass::BrkWord(FLOAT f)
{
	iEChar -= (UINT)((float)(iEChar-iBChar+1)*f+0.5);

	lLength++;
	nWord++;

	for(UINT i = lLength; i > iEChar; i--)
		txlBuffer[i] = txlBuffer[i-1];
	txlBuffer[iEChar+1] = SPACE;
}

VOID TextLineClass::MrgWord()
{
	{
	UINT i;
	for(i = iEChar+1; i <= lLength && txlBuffer[i+1] != SPACE; i++)
		txlBuffer[i] = txlBuffer[i+1];
	for(iEChar = i-1; i <= lLength; i++)
		txlBuffer[i] = txlBuffer[i+1];
	}
	lLength--;
	nWord--;
}

VOID TextLineClass::DelWord()
{
	if(iEChar+1 == lLength)	// Last word
	{
		for(; iBChar && txlBuffer[iBChar-1] != SPACE; iBChar--);
		CalcIEChar();
		txlBuffer[iEChar+1] = NULL;
	}
	else
	{
		UINT iR;
		UINT iL;
		for(iL = iBChar, iR = iEChar+2; iR < lLength && txlBuffer[iR] != SPACE; iL++, iR++)
			txlBuffer[iL] = txlBuffer[iR];
		iEChar = iL-1;
		for(; iR <= lLength; iL++, iR++)
			txlBuffer[iL] = txlBuffer[iR];
	}
	nWord--;
}

LPWSTR TextLineClass::LineText()
{
	return txlBuffer;
}

UINT TextLineClass::LineLen()
{
	return lLength;
}

VOID TextLineClass::LineLenInc()
{
	lLength++;
}

WCHAR TextLineClass::LineChar()
{
	return txlBuffer[iBChar];
}

VOID TextLineClass::RefWord()
{
	if(iBChar)
		iBChar--;
	while(iBChar)
	{
		iBChar--;
		if(txlBuffer[iBChar] == SPACE)
			break;
	}
	CalcIEChar();
}

VOID TextLineClass::IncWord()
{
	while(txlBuffer[iBChar++] != SPACE);
	CalcIEChar();
	iXChar = iBChar;
}

VOID TextLineClass::DecWord()
{
	for(iBChar--; iBChar && txlBuffer[iBChar-1] != SPACE; iBChar--);
	CalcIEChar();
	iXChar = iEChar+1;
}

VOID TextLineClass::Home()
{
	iBChar = 0;
	CalcIEChar();
	iXChar = 0;
}

VOID TextLineClass::End()
{
	for(iBChar = lLength-1; iBChar && txlBuffer[iBChar-1] != SPACE; iBChar--);
	CalcIEChar();
	iXChar = iEChar+1;
}

inline UINT TextLineClass::LLength()
{
	return lLength;
}

UINT TextLineClass::IBChar()
{
	return iBChar;
}

UINT TextLineClass::IEChar()
{
	return iEChar;
}

UINT TextLineClass::IXChar()
{
	return iXChar;
}

VOID TextLineClass::IncChar()
{
	iXChar++;
}

VOID TextLineClass::DecChar()
{
	iXChar--;
}

VOID TextLineClass::AddChar(WCHAR c)
{
	lLength++;
	for(UINT i = lLength; i > iXChar; i--)
		txlBuffer[i] = txlBuffer[i-1];
	txlBuffer[iXChar] = c;
	CalcIEChar();
}

VOID TextLineClass::DelChar(BOOL bPrev)
{
	lLength--;
	if(bPrev)
		iXChar--;
	for(UINT i = iXChar; i <= lLength; i++)
		txlBuffer[i] = txlBuffer[i+1];
	iEChar--;
}

VOID TextLineClass::CalcIEChar()
{
	for(iEChar = iBChar; iEChar+1 != lLength && txlBuffer[iEChar+1] != SPACE; iEChar++);
}
