
// ManagerGUIView.cpp : implementation of the CManagerGUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ManagerGUI.h"
#endif

#include "ManagerGUIDoc.h"
#include "ManagerGUIView.h"

#include <vector>
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CManagerGUIView

IMPLEMENT_DYNCREATE(CManagerGUIView, CListView)

BEGIN_MESSAGE_MAP(CManagerGUIView, CListView)
	ON_WM_STYLECHANGED()
END_MESSAGE_MAP()

// CManagerGUIView construction/destruction

CManagerGUIView::CManagerGUIView() noexcept
{
	// TODO: add construction code here

}

CManagerGUIView::~CManagerGUIView()
{
}

BOOL CManagerGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CManagerGUIView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// CManagerGUIView diagnostics

#ifdef _DEBUG
void CManagerGUIView::AssertValid() const
{
	CListView::AssertValid();
}

void CManagerGUIView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CManagerGUIDoc* CManagerGUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CManagerGUIDoc)));
	return (CManagerGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CManagerGUIView message handlers
void CManagerGUIView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);
}


void CManagerGUIView::OnListUpdated()
{
	CListCtrl &guilist = GetListCtrl();
	guilist.ModifyStyle(NULL, LVS_REPORT);
	guilist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	guilist.DeleteAllItems();

	while (guilist.DeleteColumn(0));

	guilist.InsertColumn(0, TEXT("Path"), LVCFMT_LEFT, 80);


	vector<tstring> &contents = GetDocument()->list;
	for (auto &str : contents) {
		guilist.InsertItem(0, str.c_str());
	}
}
