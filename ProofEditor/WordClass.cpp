# include "WordClass.h"

VOID WordClass::GenTextLine(TextGroupLoadClass &TxGL)
{
	for(UINT iLiteral = 0; iLiteral < nLiterals; iLiteral++)
		TxGL.Put(Literal[iLiteral]->code);
}
