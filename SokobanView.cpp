
// SokobanView.cpp : implementation of the CSokobanView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Sokoban.h"
#endif

#include "SokobanDoc.h"
#include "SokobanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSokobanView
IMPLEMENT_DYNCREATE(CSokobanView, CView)

BEGIN_MESSAGE_MAP(CSokobanView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSokobanView construction/destruction

CSokobanView::CSokobanView() noexcept
{
	// TODO: add construction code here

}

CSokobanView::~CSokobanView()
{
}

BOOL CSokobanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSokobanView drawing

void CSokobanView::OnDraw(CDC* pDC)
{
	CSokobanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CString aString;
	CBitmap arrival, blank, box, destination, wall, worker;
	arrival.LoadBitmap(IDB_BITarrival);
	blank.LoadBitmap(IDB_BITblank);
	box.LoadBitmap(IDB_BITbox);
	destination.LoadBitmap(IDB_BITdestination);
	wall.LoadBitmap(IDB_BITwall);
	worker.LoadBitmap(IDB_BITworker);
	CClientDC aDC(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&aDC);
	if (pDoc->flag == 0)
	{
		pDoc->Readmap(pDoc->level);
		pDoc->flag = 1;
	}
    for (int i = 0; i < pDoc->r; i++)
        for (int j = 0; j < pDoc->c; j++)
            switch (pDoc->map[i][j])
            {
            case 'H':
				memDC.SelectObject(&wall);
				aDC.BitBlt(j*32, i*32, 32, 32, &memDC, 0 , 0 , SRCCOPY);
                break;
            case 'B':
				memDC.SelectObject(&box);
				aDC.BitBlt(j * 32, i * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
                break;
			case 'E':
            case 'W':
				memDC.SelectObject(&worker);
				aDC.BitBlt(j * 32, i * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
                break;
            case 'D':
				memDC.SelectObject(&destination);
				aDC.BitBlt(j * 32, i * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
                break;
            case 'C':
				memDC.SelectObject(&arrival);
				aDC.BitBlt(j * 32, i * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
                break;
            default:
				memDC.SelectObject(&blank);
				aDC.BitBlt(j * 32, i * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
                break;
            }
	aString.Format(L"Level: %d", pDoc->level);
	pDC->TextOut(pDoc->c*32+32, 30, aString);
	aString.Format(L"Destinaton: %d", pDoc->D);
	pDC->TextOut(pDoc->c * 32 + 32, 60, aString);
	aString.Format(L"Arrival: %d", pDoc->A);
	pDC->TextOut(pDoc->c * 32 + 32, 90, aString);
	aString.Format(L"Step: %d", pDoc->step);
	pDC->TextOut(pDoc->c * 32 + 32, 120, aString);
}


// CSokobanView printing

BOOL CSokobanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSokobanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSokobanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSokobanView diagnostics

#ifdef _DEBUG
void CSokobanView::AssertValid() const
{
	CView::AssertValid();
}

void CSokobanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSokobanDoc* CSokobanView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSokobanDoc)));
	return (CSokobanDoc*)m_pDocument;
}
#endif //_DEBUG


// CSokobanView message handlers


void CSokobanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CSokobanDoc* pDoc = GetDocument();
	switch (nChar)
	{
		case 38:
		case 'W':
			nChar = 'J';
			break;
		case 37:
		case 'A':
			nChar = 'H';
			break;
		case 40:
		case 'S':
			nChar = 'K';
			break;
		case 39:
		case 'D':
			nChar = 'L';
			break;
	}
	if (nChar == 'H' || nChar == 'J' || nChar == 'K' || nChar == 'L') {
		moveworker(nChar);
		pDoc->step++;
	}
	else if(nChar == 'R')
		pDoc->flag = 0;
	else if (nChar == 33) {
		pDoc->level--;
		pDoc->flag = 0;
	}
	else if (nChar == 34) {
		pDoc->level++;
		pDoc->flag = 0;
	}
	pDoc->UpdateAllViews(NULL);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSokobanView::moveworker(UINT nChar)
{
	CClientDC aDC(this);
	CSokobanDoc* pDoc = GetDocument();
	char temp = 0, temp2 = pDoc->map[pDoc->y][pDoc->x], tx = pDoc->x, ty = pDoc->y;
	if (pDoc->map[pDoc->y][pDoc->x] == 'W')
		pDoc->map[pDoc->y][pDoc->x] = ' ';
	else if (pDoc->map[pDoc->y][pDoc->x] == 'E')
		pDoc->map[pDoc->y][pDoc->x] = 'D';
	if (nChar == 'H')
		pDoc->x--;
	else if(nChar == 'J')
		pDoc->y--;
	else if (nChar == 'K')
		pDoc->y++;
	else if (nChar == 'L')
		pDoc->x++;

	temp = NextToWorker(nChar);
	switch (temp){
	case' ':
		pDoc->map[pDoc->y][pDoc->x] = 'W';
		break;

	case'H':
		pDoc->x = tx; pDoc->y = ty;
		pDoc->map[pDoc->y][pDoc->x] = temp2;
		pDoc->step--;
		break;

	case'D':
		pDoc->map[pDoc->y][pDoc->x] = 'E';
		break;
	case'B':
	case'C':
		MoveBox(nChar, tx, ty, temp2);
		break;
	}
	check();
}

char CSokobanView::NextToWorker(UINT nChar)
{
	CSokobanDoc* pDoc = GetDocument();
	return pDoc->map[pDoc->y][pDoc->x];
}

void CSokobanView::MoveBox(UINT d, int tx, int ty, char temp3)
{
	CSokobanDoc* pDoc = GetDocument();
	char temp, bx = pDoc->x, by = pDoc->y, temp2 = pDoc->map[pDoc->y][pDoc->x];
	temp = NextNextToWorker(d);
	if (pDoc->map[pDoc->y][pDoc->x] == 'B')
		pDoc->map[pDoc->y][pDoc->x] = ' ';
	else if (pDoc->map[pDoc->y][pDoc->x] == 'C')
		pDoc->map[pDoc->y][pDoc->x] = 'D';
	switch (temp)
	{
	case'H':
		pDoc->map[pDoc->y][pDoc->x] = temp2;
		pDoc->y = ty; pDoc->x = tx;
		pDoc->map[pDoc->y][pDoc->x] = temp3;
		pDoc->step--;
		break;
	case' ':
		if (temp2 == 'B')
		{
			pDoc->map[pDoc->y1][pDoc->x1] = 'B';
			pDoc->map[pDoc->y][pDoc->x] = 'W';
		}
		else
		{
			pDoc->map[pDoc->y1][pDoc->x1] = 'B';
			pDoc->map[pDoc->y][pDoc->x] = 'E';
		}
		break;
	case'D':
		pDoc->map[pDoc->y][pDoc->x] = 'W';
		pDoc->map[pDoc->y1][pDoc->x1] = 'C';
		if (temp2 == 'C')
			pDoc->map[pDoc->y][pDoc->x] = 'E';
		break;
	case'C':
	case'B':
		pDoc->map[pDoc->y][pDoc->x] = temp2;
		pDoc->y = ty; pDoc->x = tx;
		pDoc->map[pDoc->y][pDoc->x] = temp3;
		pDoc->step--;
		break;
	}

}

char CSokobanView::NextNextToWorker(UINT d)
{
	CSokobanDoc* pDoc = GetDocument();
	pDoc->x1 = pDoc->x, pDoc->y1 = pDoc->y;
	if (d == 'K')
		pDoc->y1++;
	else if(d == 'L')
		pDoc->x1++;
	else if (d == 'H')
		pDoc->x1--;
	else if (d == 'J')
		pDoc->y1--;
	return pDoc->map[pDoc->y1][pDoc->x1];
}

void CSokobanView::check()
{
	CSokobanDoc* pDoc = GetDocument();
	pDoc->A = 0;
	for (int i = 0; i < pDoc->r; i++)
		for (int j = 0; j < pDoc->c; j++)
			if (pDoc->map[i][j] == 'C')
				pDoc->A++;
	if (pDoc->A == pDoc->B)
	{
		pDoc->UpdateAllViews(NULL);
		AfxMessageBox(L"Next level!", MB_OK);
		pDoc->level++;
		pDoc->flag = 0;
		pDoc->UpdateAllViews(NULL);
	}
}
