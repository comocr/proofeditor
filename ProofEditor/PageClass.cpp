# include "PageClass.h"

# define BULLET_SIZE			6

# define ORT_FILE_EXTN			L"ort"
# define BAK_FILE_EXTN			L"bak"
# define MAX_SENTENCE_LEN		200

# define MAX_THREAD_WAIT_TRY	100

# define PAGE_SCROLL_LINES		4

extern BOOL bTextMode;
extern BOOL bIGMode;
extern BOOL bIBMode;
extern BOOL bGReady;
extern BOOL bDoGrayScale;
extern BOOL bRects;
extern BOOL bBullets;

extern FilesClass Files;
extern SettingsClass Settings;
extern NotifyMessageBoxClass NotifyMessageBox;

PageClass::PageClass()
{
	nEditToSave = Settings.GetValue(N_EDIT_TO_SAVE);
	::bIGMode = Settings.GetValue(GRAY_MODE)? TRUE : FALSE;
	::bIBMode = Settings.GetValue(BW_MODE)? TRUE : FALSE;
	::bDoGrayScale = Settings.GetValue(DO_GRAY)? TRUE : FALSE;
	imgFileExtn = Settings.GetString(IMG_D_FILE_EXTN);
	::bBullets = Settings.GetValue(DRAW_BULLETS)? TRUE : FALSE;
}

PageClass::~PageClass()
{
}

VOID PageClass::LoadPage()
{
	Object.LoadHBMP(Files.CurName(imgFileExtn));
	PageClass_1::ReadORT(GLayout);
	PageClass_1::NGLine(GLayout);
//	NLWord(GLayout);
	//nLines = ;
	LoadTextBuffer(GLayout);

	iGroup = 0;
	iSection = 0;
	iLine = 0;
	iWord = 0;
	nPEdit = 0;
	bOChanged = FALSE;
	bTChanged = FALSE;
//	TxG[iGroup].XHome();
//	lSzText = (UINT)wcslen(lines[0]);
//	iBChar = -1;
}

VOID PageClass::CleanUp()
{
/*	if(lines)
	{
		for(UINT i = 0; i < nLines; i++)
			delete lines[i];
		delete lines;
	}
	if(pageObjects)
	{
		for(UINT i = 0; i < nLines; i++)
			delete pageObjects[i];
		delete pageObjects;
	}
	if(nLWords)
		delete nLWords;
	if(yAxes)
		delete yAxes;
*/
}

VOID PageClass::SavePage()
{
	LPWSTR newName = new WCHAR[wcslen(Files.CurName())+5];

	Files.CurName()[wcslen(Files.CurName())-4] = NULL;
	wcscat_s(Files.CurName(), (wcslen(Files.CurName())+4)*2, L"." ORT_FILE_EXTN);
	wcscpy_s(newName, wcslen(Files.CurName())+5, Files.CurName());
	wcscat_s(newName, wcslen(Files.CurName())+5, L"." BAK_FILE_EXTN);
	if(!MoveFile(Files.CurName(), newName))
	{
		DeleteFile(newName);
		if(!MoveFile(Files.CurName(), newName))
			NotifyMessageBox.NotifyMessage(IDS_ERROR, IDS_ERROR, MB_ICONERROR | MB_OK);
	}
	delete newName;

	PageClass_1::WriteORT(GLayout);
/*VOID PageClass::SaveTextPage()
	{
		for(j = 0; lines[i][j]; j++, k++)
			buffer[k] = lines[i][j];
		buffer[k] = L'\r';
		k++;
		buffer[k] = L'\n';
		k++;
	}
*/
	Files.CurName()[wcslen(Files.CurName())-4] = NULL;
	wcscat_s(Files.CurName(), (wcslen(Files.CurName())+4)*2, L"." ORT_FILE_EXTN);
	bOChanged = FALSE;
}

VOID PageClass::LoadTextBuffer(GLayoutClass* GLayout)
{
	TxG = new TextGroupClass[nGroups];
	TextGroupLoadClass TxGL;
	for(UINT iGroup = 0; iGroup < nGroups; iGroup++)
	{
		TxGL.Change(&(TxG[iGroup]));

		TxGL.Init(Group[iGroup].NLine(GLayout[iGroup]));
bText = true;
		if(bText)
		{
			TxGL.Create();
			Group[iGroup].GenTextLine(TxGL, GLayout[iGroup]);
			TxGL.EndLine();
		}
		else
			TxGL.Dummy();
/*		if(iSection != nSections-1)
		{
			if(bLeft)
				TxG[iGroup].Space();
			else
				TxG[iGroup].NewLine();
		}
*/
	}
}

UINT PageClass::ObjBeginX()
{
	return Group[iGroup].ObjBeginX(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginY()
{
	return Group[iGroup].ObjBeginY(iSection, iLine, iWord);
}

UINT PageClass::ObjEndX()
{
	return Group[iGroup].ObjEndX(iSection, iLine, iWord);
}

UINT PageClass::ObjEndY()
{
	return Height()-Group[iGroup].ObjEndY(iSection, iLine, iWord)-1;
}

UINT PageClass::ObjWidth()
{
	return Group[iGroup].ObjWidth(iSection, iLine, iWord);
}

UINT PageClass::ObjHeight()
{
	return Group[iGroup].ObjHeight(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginX(UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginX(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginY(UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginY(iSection, iLine, iWord);
}

UINT PageClass::ObjEndX(UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjEndX(iSection, iLine, iWord);
}

UINT PageClass::ObjEndY(UINT iLine, UINT iWord)
{
	return Height()-Group[iGroup].ObjEndY(iSection, iLine, iWord)-1;
}

UINT PageClass::ObjWidth(UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjWidth(iSection, iLine, iWord);
}

UINT PageClass::ObjHeight(UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjHeight(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginX(UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginX(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginY(UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginY(iSection, iLine, iWord);
}

UINT PageClass::ObjEndX(UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjEndX(iSection, iLine, iWord);
}

UINT PageClass::ObjEndY(UINT iSection, UINT iLine, UINT iWord)
{
	return Height()-Group[iGroup].ObjEndY(iSection, iLine, iWord)-1;
}

UINT PageClass::ObjWidth(UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjWidth(iSection, iLine, iWord);
}

UINT PageClass::ObjHeight(UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjHeight(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginX(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginX(iSection, iLine, iWord);
}

UINT PageClass::ObjBeginY(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjBeginY(iSection, iLine, iWord);
}

UINT PageClass::ObjEndX(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjEndX(iSection, iLine, iWord);
}

UINT PageClass::ObjEndY(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjEndY(iSection, iLine, iWord);
}

UINT PageClass::ObjHeight(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjHeight(iSection, iLine, iWord);
}

UINT PageClass::ObjWidth(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	return Group[iGroup].ObjWidth(iSection, iLine, iWord);
}

VOID PageClass::ObjBeginXInc(UINT d)
{
	if(ObjBeginX() < Group[iGroup].EndY()-1 && ObjWidth() > 1)
		Group[iGroup].ObjBeginXInc(iSection, iLine, iWord, d);
}

VOID PageClass::ObjBeginXDec(UINT d)
{
	if(ObjBeginX() > Group[iGroup].BeginX())
		Group[iGroup].ObjBeginXDec(iSection, iLine, iWord, d);
}

VOID PageClass::ObjEndXInc(UINT d)
{
	if(ObjEndX() < Group[iGroup].Width()-1)
		Group[iGroup].ObjEndXInc(iSection, iLine, iWord, d);
}

VOID PageClass::ObjEndXDec(UINT d)
{
	if(ObjWidth())
		Group[iGroup].ObjEndXDec(iSection, iLine, iWord, d);
}

VOID PageClass::ObjBreak(UINT xWidth)
{
	TxG[iGroup].BrkWord(iLine, (float)xWidth/(float)ObjWidth());
	Group[iGroup].ObjBreak(iSection, iLine, iWord, xWidth);
}

VOID PageClass::ObjMerge(BOOL bNext__bPrev)
{
	if(bNext__bPrev)
	{
		if(iWord == NGSLWord(iGroup, iSection, iLine)-1)
			return;
	}
	else
	{
		if(iWord == 0)
			return;
		else
			DecWord();
	}
	Group[iGroup].ObjMerge(iSection, iLine, iWord);
	TxG[iGroup].MrgWord(iLine);
}

VOID PageClass::ObjDelete(BOOL bText)
{
	Group[iGroup].ObjDelete(iSection, iLine, iWord);
	if(bText)
		TxG[iGroup].DelWord(iLine);
}

VOID PageClass::ObjSelect(UINT iGroup, UINT iSection, UINT iLine, UINT iWord)
{
	PageClass::iGroup = iGroup;
	PageClass::iSection = iSection;
	PageClass::iLine = iLine;
	PageClass::iWord = iWord;
	//TxG[iGroup].Select(iSection, iLine, iWord);		// Section in same group compose common line
}

LPWSTR PageClass::LineText()
{
	return TxG[iGroup].LineText(iLine);
}

UINT PageClass::LineLen()
{
	return TxG[iGroup].LineLen(iLine);
}

VOID PageClass::LineLenInc()
{
	TxG[iGroup].LineLenInc(iLine);
}

WCHAR PageClass::LineChar()
{
	return TxG[iGroup].LineChar(iLine);
}

VOID PageClass::RenderRects()
{
	for(UINT i = 0; i < NGroup(); i++)
		for(UINT j = 0; j < NGSection(i); j++)
			for(UINT k = 0; k < NGSLine(i, j); k++)
				for(UINT l = 0; l < NGSLWord(i, j, k); l++)
				{
					POINT rect[3];
					rect[0].x = ObjBeginX(i, j, k, l);
					rect[0].y = ObjEndY(i, j, k, l);
					rect[1].x = rect[0].x+ObjWidth(i, j, k, l)-1;
					rect[1].y = rect[0].y;
					rect[2].x = rect[1].x;
					rect[2].y = rect[1].y+ObjHeight(i, j, k, l)-1;
					Object.RenderRect(rect);						//PolyLine Draws - not the last point
				}
}

VOID PageClass::RenderBullets()
{
	for(UINT i = 0; i < NGroup(); i++)
		for(UINT j = 0; j < NGSection(i); j++)
			for(UINT k = 0; k < NGSLine(i, j); k++)
				for(UINT l = 0; l < NGSLWord(i, j, k); l++)
					if(l)
					{
					POINT extn[2];
					UINT x = ObjBeginX(i, j, k, l-1)+ObjWidth(i, j, k, l-1);
					extn[0].x = x+(ObjBeginX(i, j, k, l)-x)/2-BULLET_SIZE/2;
					extn[0].y = yAxes[i]-BULLET_SIZE/2;
					extn[1].x = extn[0].x+BULLET_SIZE;
					extn[1].y = extn[0].y+BULLET_SIZE;
					Object.RenderBullet(extn);
					}
}

VOID PageClass::DoGrayThread()
{
	if(hThread)
	{
		DWORD tExitCode;
		GetExitCodeThread(hThread, &tExitCode);
		if(tExitCode == STILL_ACTIVE)
		{
			//syncWidth = 0;
			//syncHeight = 0;
			UINT i = 0;
			do
				GetExitCodeThread(hThread, &tExitCode);
			while(tExitCode == STILL_ACTIVE && i++ < MAX_THREAD_WAIT_TRY);
			if(i == MAX_THREAD_WAIT_TRY)
				exit(-435);
		}
	}
//	DeleteObject(hImageG);
	::bGReady = FALSE;
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Object.MakeBW,	(LPVOID)&Object, 0, NULL);
	if(hThread == NULL)
		NotifyMessageBox.NotifyMessage(IDS_THREAD_FAIL, IDS_ERROR, MB_ICONERROR | MB_OK);
}

HBITMAP PageClass::HImageI()
{
	return Object.hImageI;
}

HBITMAP PageClass::HImageB()
{
	return Object.hImageB;
}

HBITMAP PageClass::HImageG()
{
	return Object.hImageG;
}

HBITMAP PageClass::HBRects()
{
	return Object.hBRects;
}

HBITMAP PageClass::HGRects()
{
	return Object.hGRects;
}

HBITMAP PageClass::HIRects()
{
	return Object.hIRects;
}

VOID PageClass::SelWord()
{
	TxG[iGroup].SelWord(iLine, iWord);
}

VOID PageClass::RefWord()
{
	TxG[iGroup].RefWord(iLine);
}

BOOL PageClass::IncWord()
{
	if(iGroup == NGroup()-1 && iSection == NGSection(iGroup)-1 && iLine == NGSLine(iGroup, iSection)-1 && iWord == NGSLWord(iGroup, iSection, iLine)-1)
		return FALSE;
	else if(iSection == NGSection(iGroup)-1 && iLine == NGSLine(iGroup, iSection)-1 && iWord == NGSLWord(iGroup, iSection, iLine)-1)
	{
		iGroup++;
		iSection = 0;
		iLine = 0;
		iWord = 0;
		TxG[iGroup].XHome();
	}
	else if(iLine == NGSLine(iGroup, iSection)-1 && iWord == NGSLWord(iGroup, iSection, iLine)-1)
	{
		iSection++;
		iLine = 0;
		iWord = 0;
		TxG[iGroup].IncWord(iLine);
	}
	else if(iWord == NGSLWord(iGroup, iSection, iLine)-1)
	{
		iLine++;
		iWord = 0;
		TxG[iGroup].Home(iLine);
	}
	else
	{
		iWord++;
		TxG[iGroup].IncWord(iLine);
	}

	return TRUE;
}

BOOL PageClass::DecWord()
{
	if(!iGroup && !iSection && !iLine && !iWord)
		return FALSE;
	else if(!iSection && !iLine && !iWord)
	{
		iGroup--;
		iSection = NGSection(iGroup)-1;
		iLine = NGSLine(iGroup, iSection)-1;
		iWord = NGSLWord(iGroup, iSection, iLine)-1;
		TxG[iGroup].XEnd(iLine);
	}
	else if(!iLine && !iWord)
	{
		iSection--;
		iLine = NGSLine(iGroup, iSection)-1;
		iWord = NGSLWord(iGroup, iSection, iLine)-1;
		TxG[iGroup].DecWord(iLine);
	}
	else if(!iWord)
	{
		iLine--;
		iWord = NGSLWord(iGroup, iSection, iLine)-1;
		TxG[iGroup].End(iLine);
	}
	else
	{
		iWord--;
		TxG[iGroup].DecWord(iLine);
	}

	return TRUE;
}

BOOL PageClass::IncLine()
{
	if(iGroup == NGroup()-1 && iLine == NGLine(iGroup)-1)
		return FALSE;
	else
	{
		if(iLine == NGLine(iGroup)-1)
		{
			iGroup++;
			iSection = 0;
			iLine = 0;
		}
		else
		{
			iSection = 0;				// ToDo: Check appropriate Section
			iLine++;
		}

		iWord = 0;
		TxG[iGroup].IncLine(iLine);
		return TRUE;
	}
}

BOOL PageClass::DecLine()
{
	if(!iGroup && !iLine)
		return FALSE;
	else
	{
		if(!iLine)
		{
			iGroup--;
			iSection = 0;				// ToDo: Check appropriate Group
			iLine = NGLine(iGroup)-1;
		}
		else
			iLine--;

		iWord = 0;						// not Matching word : Todo
		TxG[iGroup].DecLine(iLine);
		return TRUE;
	}
}

BOOL PageClass::Home()
{
	if(!iSection && !iWord)
	{
		TxG[iGroup].Home(iLine);
		return FALSE;
	}
	else
	{
		iSection = 0;
		iWord = 0;
		TxG[iGroup].Home(iLine);
		return TRUE;
	}
}

BOOL PageClass::End()
{
	if(iSection == NGSection(iGroup)-1 && iWord == NGSLWord(iGroup, iSection, iLine) - 1)
	{
		TxG[iGroup].End(iLine);
		return FALSE;
	}
	else
	{
		iSection = NGSection(iGroup)-1;
		iWord = NGSLWord(iGroup, iSection, iLine)-1;
		TxG[iGroup].End(iLine);
		return TRUE;
	}
}

BOOL PageClass::XHome()
{
	if(!iGroup && !iSection && !iLine && !iWord)
		return FALSE;
	else
	{
		iGroup = 0;
		iSection = 0;
		iLine = 0;
		iWord = 0;
		TxG[iGroup].XHome();
		return TRUE;
	}
}

BOOL PageClass::XEnd()
{
	if(iGroup != NGroup()-1 && iSection == NGSection(iGroup)-1 && iLine == NGSLine(iGroup, iSection)-1 && iWord == NGSLWord(iGroup, iSection, iLine)-1)
		return FALSE;
	else
	{
		iGroup = NGroup()-1;
		iSection = NGSection(iGroup)-1;
		iLine = NGSLine(iGroup, iSection)-1;
		iWord = NGSLWord(iGroup, iSection, iLine)-1;
		TxG[iGroup].XEnd(iLine);
		return TRUE;
	}
}

BOOL PageClass::PageUp()
{
	if(!iGroup && !iSection && !iLine && !iWord)
		return FALSE;
	else
	{
		iLine -= PAGE_SCROLL_LINES;
		if((int)iLine < 0)
		{
			if(iGroup)
				iGroup--;
			//iLine = NSLine()-1+iLine;
			//if((int)iLine <= 0)
			//	iLine = 0;
			iSection = NGSection(iGroup)-1;
			iLine = NGSLine(iGroup, iSection)-1;
		}
		iWord = 0;
		TxG[iGroup].PageUp(iLine);
		return TRUE;
	}
}

BOOL PageClass::PageDn()
{
	if(iGroup == NGroup() && iSection == NGSection(iGroup)-1 && iLine == NGSLine(iGroup, iSection)-1 && iWord == NGSLWord(iGroup, iSection, iLine)-1)
		return FALSE;
	else
	{
		iLine += PAGE_SCROLL_LINES;
		if(iLine > NGSLine(iGroup, iSection)-1)
		{
			iLine -= NGSLine(iGroup, iSection)-1;
			if(iGroup != NGroup()-1)
				iGroup++;
			//if((iLine > NSLine()-1)
			//	iLine = NSLine() - 1;
			iSection = 0;
			iLine = 0;
		}
		iWord = 0;
		TxG[iGroup].PageDn(iLine);
		return TRUE;
	}
}
/*
VOID PageClass::NLWord(LayoutClass *Layout)
{
	nLWords = new UINT[nLines];
	UINT iLine = 0;
	for(UINT i = 0; i < nSections; i++)
		for(UINT j = 0; j < Section[i].NLine(); j++)
			if(Layout[i].GetPart() == SECTION_LEFT)
				nLWords[iLine] = Section[i].NLWord(j);
			else if(Layout[i].GetPart() == SECTION_RIGHT)	// or SECTION_LEFT
				nLWords[iLine] += Section[i].NLWord(j);
			else
				nLWords[iLine++] = Section[i].NLWord(j);
}
*/

UINT PageClass::IBChar()
{
	return TxG[iGroup].IBChar(iLine);
}

UINT PageClass::IEChar()
{
	return TxG[iGroup].IEChar(iLine);
}

UINT PageClass::IXChar()
{
	return TxG[iGroup].IXChar(iLine);
}

VOID PageClass::IncChar()
{
	TxG[iGroup].IncChar(iLine);
}

VOID PageClass::DecChar()
{
	TxG[iGroup].DecChar(iLine);
}

VOID PageClass::AddChar(WCHAR c)
{
	TxG[iGroup].AddChar(iLine, c);
}

VOID PageClass::DelChar(BOOL bPrev)
{
	TxG[iGroup].DelChar(iLine, bPrev);
}

UINT PageClass::LineEndY()
{
	return Height()-Group[iGroup].LineEndY(iSection, iLine)-1;
}

UINT PageClass::LineHeight()
{
	return Group[iGroup].LineHeight(iSection, iLine);
}

UINT PageClass::Width()
{
	return Object.Width();
}

UINT PageClass::Height()
{
	return Object.Height();
}
