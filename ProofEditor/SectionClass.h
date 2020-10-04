# ifndef SECTION_CLASS_HEADER
# define SECTION_CLASS_HEADER

# include "..\\..\\Engine\\Engine\\SectionClass_1.h"
# include "TextGroupLoadClass.h"

class SectionClass : public SectionClass_1
{
public:
	VOID GenTextLine(TextGroupLoadClass& TxGL, BOOL bLeft);
	UINT NLWord(UINT iLine);
	UINT ObjBeginX(UINT iLine, UINT iWord);
	UINT ObjBeginY(UINT iLine, UINT iWord);
	UINT ObjEndX(UINT iLine, UINT iWord);
	UINT ObjEndY(UINT iLine, UINT iWord);
	UINT ObjWidth(UINT iLine, UINT iWord);
	UINT ObjHeight(UINT iLine, UINT iWord);

	UINT LineEndY(UINT iLine);
	UINT LineHeight(UINT iLine);

	VOID ObjBeginXInc(UINT iLine, UINT iWord, UINT d);
	VOID ObjBeginXDec(UINT iLine, UINT iWord, UINT d);
	VOID ObjEndXInc(UINT iLine, UINT iWord, UINT d);
	VOID ObjEndXDec(UINT iLine, UINT iWord, UINT d);
	VOID ObjMerge(UINT iLine, UINT iWord);
	VOID ObjBreak(UINT iLine, UINT iWord, UINT x);
	VOID ObjDelete(UINT iLine, UINT iWord);
};

# endif SECTION_CLASS_HEADER
