# ifndef GROUP_CLASS_HEADER
# define GROUP_CLASS_HEADER

# include "..\\..\\Engine\\Engine\\GroupClass_1.h"
# include "TextGroupLoadClass.h"

class GroupClass : public GroupClass_1
{
public:
	VOID GenTextLine(TextGroupLoadClass& TxLL, GLayoutClass& GLayout);

	UINT ObjBeginX(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjBeginY(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndX(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjEndY(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjWidth(UINT iSection, UINT iLine, UINT iWord);
	UINT ObjHeight(UINT iSection, UINT iLine, UINT iWord);

	UINT LineEndY(UINT iSection, UINT iLine);
	UINT LineHeight(UINT iSection, UINT iLine);

	VOID ObjBeginXInc(UINT iSection, UINT iLine, UINT iWord, UINT d);
	VOID ObjBeginXDec(UINT iSection, UINT iLine, UINT iWord, UINT d);
	VOID ObjEndXInc(UINT iSection, UINT iLine, UINT iWord, UINT d);
	VOID ObjEndXDec(UINT iSection, UINT iLine, UINT iWord, UINT d);
	VOID ObjMerge(UINT iSection, UINT iLine, UINT iWord);
	VOID ObjBreak(UINT iSection, UINT iLine, UINT iWord, UINT x);
	VOID ObjDelete(UINT iSection, UINT iLine, UINT iWord);
	//VOID ObjChangeBeginX(UINT iSection, UINT iLine, UINT iWord, int x);
	//VOID ObjChangeWidth(UINT iSection, UINT iLine, UINT iWord, int x);
};

# endif // GROUP_CLASS_HEADER
