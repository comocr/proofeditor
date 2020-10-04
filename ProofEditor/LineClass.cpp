# include "LineClass.h"

VOID LineClass::GenTextLine(TextGroupLoadClass& TxGL)
{
	for(UINT iWord = 0;; iWord++)
	{
		Word[iWord]->GenTextLine(TxGL);
		if(iWord == nWord-1)
			break;
		else
			TxGL.Space();
	}
}

UINT LineClass::ObjBeginX(UINT iWord)
{
	return Word[iWord]->BeginX();
}

UINT LineClass::ObjBeginY(UINT iWord)
{
	return Word[iWord]->BeginY();
}

UINT LineClass::ObjEndX(UINT iWord)
{
	return Word[iWord]->EndX();
}

UINT LineClass::ObjEndY(UINT iWord)
{
	return Word[iWord]->EndY();
}

UINT LineClass::ObjWidth(UINT iWord)
{
	return Word[iWord]->Width();
}

UINT LineClass::ObjHeight(UINT iWord)
{
	return Word[iWord]->Height();
}

VOID LineClass::ObjBeginXInc(UINT iWord, UINT d)
{
	Word[iWord]->BeginX(Word[iWord]->BeginX()+d);
}

VOID LineClass::ObjBeginXDec(UINT iWord, UINT d)
{
	Word[iWord]->BeginX(Word[iWord]->BeginX()-d);
}

VOID LineClass::ObjEndXInc(UINT iWord, UINT d)
{
	Word[iWord]->EndX(Word[iWord]->EndX()+d);
}

VOID LineClass::ObjEndXDec(UINT iWord, UINT d)
{
	Word[iWord]->EndX(Word[iWord]->EndX()-d);
}

VOID LineClass::ObjMerge(UINT iWord)
{
	Word[iWord]->EndX(Word[iWord+1]->EndX());
	if(Word[iWord]->BeginY() > Word[iWord+1]->BeginY())
		Word[iWord]->BeginY(Word[iWord+1]->BeginY());
	if(Word[iWord]->EndY() < Word[iWord+1]->EndY())
		Word[iWord]->EndY(Word[iWord+1]->EndY());
	ObjDelete(iWord+1);
}

VOID LineClass::ObjBreak(UINT iWord, UINT w)
{
	WordClass **Word = new WordClass*[nWord+1];
	for(UINT i = 0; i < iWord; i++)
		Word[i] = LineClass::Word[i];
	for(UINT i = nWord; i > iWord; i--)
		Word[i] = LineClass::Word[i-1];

	Word[iWord] = new WordClass;
	Word[iWord]->BeginX(LineClass::Word[iWord]->BeginX());
	Word[iWord]->Width(w);
	Word[iWord]->BeginY(LineClass::Word[iWord]->BeginY());
	Word[iWord]->EndY(LineClass::Word[iWord]->EndY());

	Word[iWord+1]->BeginX(LineClass::Word[iWord]->BeginX()+w);
	Word[iWord+1]->Width(LineClass::Word[iWord]->Width()-w);

	delete LineClass::Word; //[]
	LineClass::Word = Word;
	nWord++;
}

VOID LineClass::ObjDelete(UINT iWord)
{
	for(UINT i = iWord; i < nWord-1; i++)
		Word[i] = Word[i+1];
	nWord--;
}
/*
UINT LineClass::EndY()
{
	return PieceClass::EndY();
}

UINT LineClass::Height()
{
	return PieceClass::Height();
}
*/