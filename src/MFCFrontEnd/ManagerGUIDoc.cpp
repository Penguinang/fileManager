
// ManagerGUIDoc.cpp : implementation of the CManagerGUIDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ManagerGUI.h"
#endif

#include "ManagerGUIDoc.h"
#include "ManagerGUIView.h"
#include "MainFrm.h"
#include "LeftView.h"

#include "Scanner.h"
#include "DBConnection.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManagerGUIDoc

IMPLEMENT_DYNCREATE(CManagerGUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CManagerGUIDoc, CDocument)
END_MESSAGE_MAP()


// CManagerGUIDoc construction/destruction

CManagerGUIDoc::CManagerGUIDoc() noexcept : 
	conn(new CachedDBConnection("db/media.db")), 
	scanner(new Scanner({ 
		TEXT("mp3"),  TEXT("wav"),  TEXT("jpg"),  TEXT("png"),  TEXT("gif"),  TEXT("jpeg"),  
		TEXT("mp4"),  TEXT("mkv"),  TEXT("doc"),  TEXT("docx"),  TEXT("pdf") }))
{
	// TODO: add one-time construction code here
	//scanner->Update(*conn);
}

CManagerGUIDoc::~CManagerGUIDoc()
{
}

BOOL CManagerGUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	POSITION ps = GetFirstViewPosition();
	CView *view = GetNextView(ps);
	CMainFrame *frame = DYNAMIC_DOWNCAST(CMainFrame, view->GetParentFrame());
	frame->GetLeftPane()->CreateItems();



	return TRUE;
}




// CManagerGUIDoc serialization

void CManagerGUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CManagerGUIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CManagerGUIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CManagerGUIDoc::SetSearchContent(const CString& value)
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

// CManagerGUIDoc diagnostics

#ifdef _DEBUG
void CManagerGUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CManagerGUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CManagerGUIDoc commands


void CManagerGUIDoc::OnTreeSelChanged(MEDIA type)
{
	switch (type)
	{
	case MEDIA::SONG:
		list = scanner->Locate(TEXT(".mp3"), *conn);
		break;
	case MEDIA::MOVIES:
		list = scanner->Locate(TEXT(".mp3"), *conn);
		break;
	case MEDIA::PICTURES:
		list = scanner->Locate(TEXT(".mp3"), *conn);
		break;
	case MEDIA::DOCUMENTS:
		list = scanner->Locate(TEXT(".mp3"), *conn);
		break;
	case MEDIA::ALL:
		list = scanner->Locate(TEXT(".mp3"), *conn);
		break;
	default:
		break;
	}

	POSITION ps = GetFirstViewPosition();
	CView *view = GetNextView(ps);
	CMainFrame *frame = DYNAMIC_DOWNCAST(CMainFrame, view->GetParentFrame());
	frame->GetMiddlePane()->OnListUpdated();

	
}
