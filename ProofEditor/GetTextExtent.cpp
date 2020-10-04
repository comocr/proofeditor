# include "Windows.h"

UINT WINAPI GetTextExtentABCPoint( HDC hDC, LPWSTR lpsz, int cb, LPSIZE lpSize )
{
	int          A, C ;
	UINT         nRet = 0 ;
	static LPABC lpABC = new _ABC[256*sizeof(ABC)];

	GetTextExtentPoint( hDC, lpsz, cb, lpSize );

		if (GetCharABCWidths( hDC, 0, 255, lpABC ))
		{
			int         n ;
			for ( lpSize->cx = 0, n = 0 ; n < cb ; n++ )
			{
				lpSize->cx += (
					lpABC[(int)lpsz[n]].abcA +
					lpABC[(int)lpsz[n]].abcB +
					lpABC[(int)lpsz[n]].abcC ) ;
			}

			if( cb )
			{
				A = lpABC[(int)lpsz[0]].abcA;

				if( A > 0 )
				{
					nRet = -A ;
					lpSize->cx -= A;
				}

				C = lpABC[(int)lpsz[cb-1]].abcC ;

				if ( C < 0 )
					lpSize->cx -= C ;    // two minus' make a plus
			}
//		delete lpABC;
		}
		else
			GetTextExtentPoint( hDC, lpsz, cb, lpSize ) ;
//	delete lpABC;
	return nRet ;
}
