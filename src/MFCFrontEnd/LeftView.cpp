
// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "ManagerGUI.h"

#include "Scanner.h"
#include "ManagerGUIDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CManagerGUIDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CManagerGUIDoc)));
	return (CManagerGUIDoc*)m_pDocument;
}

#endif //_DEBUG


void CLeftView::CreateItems() {
	CTreeCtrl &tree = GetTreeCtrl();

	HTREEITEM root;
	root = tree.InsertItem(_T("ALL"), 0, 1);
	tree.InsertItem(_T("Songs"), 2, 3, root);
	tree.InsertItem(_T("Movies"), 2, 3, root);
	tree.InsertItem(_T("Pictures"), 2, 3, root);
	tree.InsertItem(_T("Documents"), 2, 3, root);
}

// CLeftView message handlers


void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CTreeCtrl &tree = GetTreeCtrl();
	CString text = tree.GetItemText(pNMTreeView->itemNew.hItem);
	

	// TODO: Add your control notification handler code here
	if (text == TEXT("Songs")) {
		GetDocument()->OnTreeSelChanged(MEDIA::SONG);
	} else if (text == TEXT("Movies")) {
		GetDocument()->OnTreeSelChanged(MEDIA::MOVIES);
	} else if (text == TEXT("Pictures")) {
		GetDocument()->OnTreeSelChanged(MEDIA::PICTURES);
	} else if (text == TEXT("Documents")) {
		GetDocument()->OnTreeSelChanged(MEDIA::DOCUMENTS);
	} else if (text == TEXT("ALL")) {
		GetDocument()->OnTreeSelChanged(MEDIA::ALL);
	}

	*pResult = 0;
}
