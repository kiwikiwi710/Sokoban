
// SokobanDoc.cpp : implementation of the CSokobanDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Sokoban.h"
#endif

#include "SokobanDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSokobanDoc

IMPLEMENT_DYNCREATE(CSokobanDoc, CDocument)

BEGIN_MESSAGE_MAP(CSokobanDoc, CDocument)
END_MESSAGE_MAP()


// CSokobanDoc construction/destruction

CSokobanDoc::CSokobanDoc() noexcept
{
	// TODO: add one-time construction code here
	level = 1;
	A = 0;
	B = 0;
	D = 0;
	x = 0;
	y = 0;
	r = 0;
	c = 0;
	step = 0;
	flag = 0;
	x1 = 0;
	y1 = 0;
	for (int i = 0; i < MAX; i++)
		for (int j = 0; j < MAX; j++)
			map[i][j] = 0;
}

CSokobanDoc::~CSokobanDoc()
{
}

BOOL CSokobanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	level = 1;
	A = 0;
	B = 0;
	D = 0;
	x = 0;
	y = 0;
	r = 0;
	c = 0;
	step = 0;
	flag = 0;
	x1 = 0;
	y1 = 0;
	for (int i = 0; i < MAX; i++)
		for (int j = 0; j < MAX; j++)
			map[i][j] = 0;
	return TRUE;
}




// CSokobanDoc serialization

void CSokobanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << A << B << c << D << flag << level << r << step << x << x1 << y << y1;
		for (int i = 0; i < MAX; i++)
			for (int j = 0; j < MAX; j++)
				ar << map[i][j];
	}
	else
	{
		// TODO: add loading code here
		ar >> A >> B >> c >> D >> flag >> level >> r >> step >> x >> x1 >> y >> y1;
		for (int i = 0; i < MAX; i++)
			for (int j = 0; j < MAX; j++)
				ar >> map[i][j];
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSokobanDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSokobanDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSokobanDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSokobanDoc diagnostics

#ifdef _DEBUG
void CSokobanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSokobanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSokobanDoc commands
void CSokobanDoc::Readmap(int n = 0)
{
	int W = 0, F = 0;
	char temp=0, name[FILELEN]={};
	A = 0;
	B = 0;
	D = 0;
	x = 0;
	y = 0;
	r = 0;
	c = 0;
	step = 0;
	if (n > 150)
	{
		AfxMessageBox(L"Congratulation!", MB_OK);
		level = 1;
		n = 1;
	}
	else if (n < 0)
		level = 0;
	sprintf_s(name, "map%03d.txt", n);
	FILE* f;
	errno_t err = fopen_s(&f, name, "r");
	CString aString;

	if (err != 0)
	{
		AfxMessageBox(L"File is not found", MB_OK);
		level++;
		Readmap(level);
		r = 0;
		c = 0;
	}
	else
	{
		for (int i = 0; i < MAX; i++)
			for (int j = 0; j < MAX; j++)
				map[i][j] = ' ';
		for (int i = 0; i < MAX; i++)
			for (int j = 0; j < MAX; j++)
			{
				if (!(feof(f)))
					temp = fgetc(f);
				switch (temp)
				{
				case 'D':
					map[i][j] = 'D';
					D++;
					break;
				case 'C':
					map[i][j] = 'C';
					B++; D++; A++;
					break;
				case '\t':
					for (int k = 0; k < 7; k++, j++)
						map[i][j] = ' ';
					map[i][j] = ' ';
					break;
				case '\n':
					j = MAX;
					break;
				case 'W':
					map[i][j] = 'W';
					W++;
					y = i;
					x = j;
					break;
				case 'H':
					map[i][j] = 'H';
					break;
				case 'B':
					map[i][j] = 'B';
					B++;
					break;
				case '\r':
				default:
					map[i][j] = ' ';
					break;
				}
			}
		for (int i = 0; i < MAX; i++)
		{
			int temp = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (map[i][j] == ' ')
					temp++;
			}
			if (temp != MAX)
			{
				F = 1;
				r++;
			}
			else if (F == 0)
				r++;
		}
		F = 0;
		for (int j = 0; j < MAX; j++)
		{
			int temp = 0;
			for (int i = 0; i < MAX; i++)
			{
				if (map[i][j] == ' ')
					temp++;
			}
			if (temp != MAX)
			{
				F = 1;
				c++;
			}
			else if (F == 0)
				c++;
		}
	}
	if (!err && f != NULL)
		fclose(f);
}