# include "GroupClass.h"

# define GROUP_LINE					0
# define GROUP_BLOCK				1
# define GROUP_LINE_FIRST			0
# define GROUP_LINE_LAST			1
# define GROUP_BLOCK_PX				0
# define GROUP_BLOCK_REMAIN			1

VOID GroupClass::GenTextLine(TextGroupLoadClass& TxGL, GLayoutClass& GLayout)
{
	UINT iLine = 0;
	UINT iWord = 0;
	for(UINT i = 0; i < nSections; i++)
	{
		BOOL bLeft = ((GLayout.GetPart(i)) == SECTION_LEFT);
		Section[i].GenTextLine(TxGL, bLeft);
		if(bLeft)
			TxGL.Space();
	}
}

UINT GroupClass::ObjBeginX(UINT iSection, UINT iLine, UINT iWord)
{
	return Section[iSection].ObjBeginX(iLine, iWord);
}

UINT GroupClass::ObjBeginY(UINT iSection, UINT iLine, UINT iWord)
{
	return Section[iSection].ObjBeginY(iLine, iWord);
}

UINT GroupClass::ObjEndX(UINT iSection, UINT iLine, UINT iWord)
{
	return Section[iSection].ObjEndX(iLine, iWord);
}

UINT GroupClass::ObjEndY(UINT iSection, UINT iLine, UINT iWord)
{
	return Section[iSection].ObjEndY(iLine, iWord);
}

UINT GroupClass::ObjWidth(UINT iSection, UINT iLine, UINT iWord)
{

	return Section[iSection].ObjWidth(iLine, iWord);
}

UINT GroupClass::ObjHeight(UINT iSection, UINT iLine, UINT iWord)
{

	return Section[iSection].ObjHeight(iLine, iWord);
}

UINT GroupClass::LineEndY(UINT iSection, UINT iLine)
{
	return Section[iSection].LineEndY(iLine);		// Same line with different per section
}

UINT GroupClass::LineHeight(UINT iSection, UINT iLine)
{
	return Section[iSection].LineHeight(iLine);		// Same line with different per section
}

VOID GroupClass::ObjBeginXInc(UINT iSection, UINT iLine, UINT iWord, UINT d)
{
	Section[iSection].ObjBeginXInc(iLine, iWord, d);
}

VOID GroupClass::ObjBeginXDec(UINT iSection, UINT iLine, UINT iWord, UINT d)
{
	Section[iSection].ObjBeginXDec(iLine, iWord, d);
}

VOID GroupClass::ObjEndXInc(UINT iSection, UINT iLine, UINT iWord, UINT d)
{
	Section[iSection].ObjEndXInc(iLine, iWord, d);
}

VOID GroupClass::ObjEndXDec(UINT iSection, UINT iLine, UINT iWord, UINT d)
{
	Section[iSection].ObjEndXDec(iLine, iWord, d);
}

VOID GroupClass::ObjMerge(UINT iSection, UINT iLine, UINT iWord)
{
	Section[iSection].ObjMerge(iLine, iWord);
}

VOID GroupClass::ObjBreak(UINT iSection, UINT iLine, UINT iWord, UINT x)
{
	Section[iSection].ObjBreak(iLine, iWord, x);
}

VOID GroupClass::ObjDelete(UINT iSection, UINT iLine, UINT iWord)
{
	Section[iSection].ObjDelete(iLine, iWord);
}
