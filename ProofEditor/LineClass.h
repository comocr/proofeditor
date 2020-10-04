# ifndef LINE_CLASS_HEADER
# define LINE_CLASS_HEADER

# include "..\\..\\Engine\\Engine\\LineClass_1.h"
# include "TextGroupLoadClass.h"

class LineClass : public LineClass_1
{
private:
	UINT endY;
	UINT height;

public:
	VOID GenTextLine(TextGroupLoadClass& TxGL);

	UINT ObjBeginX(UINT i);
	UINT ObjBeginY(UINT i);
	UINT ObjEndX(UINT i);
	UINT ObjEndY(UINT i);
	UINT ObjWidth(UINT i);
	UINT ObjHeight(UINT i);

	VOID ObjBeginXInc(UINT iWord, UINT x);
	VOID ObjBeginXDec(UINT iWord, UINT x);
	VOID ObjEndXInc(UINT iWord, UINT x);
	VOID ObjEndXDec(UINT iWord, UINT x);

	VOID ObjMerge(UINT iWord);
	VOID ObjBreak(UINT iWord, UINT x);
	VOID ObjDelete(UINT iWord);

	//UINT EndY();
	//UINT Height();
};

# endif // LINE_CLASS_HEADER
