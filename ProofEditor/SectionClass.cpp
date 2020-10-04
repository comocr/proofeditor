# include "SectionClass.h"

VOID SectionClass::GenTextLine(TextGroupLoadClass& TxGL, BOOL bLeft)
{
	for(UINT iLine = 0; iLine < nLines; iLine++)
	{
		Line[iLine]->GenTextLine(TxGL);
		if(!bLeft)
			if(iLine != nLines-1)
				TxGL.NewLine();
	}
}

UINT SectionClass::NLWord(UINT iLine)
{
	return Line[iLine]->NWord();
}

UINT SectionClass::ObjBeginX(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjBeginX(iWord);
}

UINT SectionClass::ObjBeginY(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjBeginY(iWord);
}

UINT SectionClass::ObjEndX(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjEndX(iWord);
}

UINT SectionClass::ObjEndY(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjEndY(iWord);
}

UINT SectionClass::ObjWidth(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjWidth(iWord);
}

UINT SectionClass::ObjHeight(UINT iLine, UINT iWord)
{
	return Line[iLine]->ObjHeight(iWord);
}

UINT SectionClass::LineEndY(UINT iLine)
{
	return Line[iLine]->EndY();
}

UINT SectionClass::LineHeight(UINT iLine)
{
	return Line[iLine]->Height();
}

VOID SectionClass::ObjBeginXInc(UINT iLine, UINT iWord, UINT d)
{
	Line[iLine]->ObjBeginXInc(iWord, d);
}

VOID SectionClass::ObjBeginXDec(UINT iLine, UINT iWord, UINT d)
{
	Line[iLine]->ObjBeginXDec(iWord, d);
}

VOID SectionClass::ObjEndXInc(UINT iLine, UINT iWord, UINT d)
{
	Line[iLine]->ObjEndXInc(iWord, d);
}

VOID SectionClass::ObjEndXDec(UINT iLine, UINT iWord, UINT d)
{
	Line[iLine]->ObjEndXDec(iWord, d);
}

VOID SectionClass::ObjMerge(UINT iLine, UINT iWord)
{
	Line[iLine]->ObjMerge(iWord);
}

VOID SectionClass::ObjBreak(UINT iLine, UINT iWord, UINT x)
{
	Line[iLine]->ObjBreak(iWord, x);
}

VOID SectionClass::ObjDelete(UINT iLine, UINT iWord)
{
	Line[iLine]->ObjDelete(iWord);
}
