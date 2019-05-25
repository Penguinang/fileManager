
// ManagerGUIView.cpp : implementation of the CManagerGUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ManagerGUI.h"
#endif

#include "Scanner.h"
#include "FileSystem.h"
#include "ManagerGUIDoc.h"
#include "ManagerGUIView.h"
#include "CRightView.h"
#include <vector>
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CManagerGUIView

IMPLEMENT_DYNCREATE(CManagerGUIView, CListView)

BEGIN_MESSAGE_MAP(CManagerGUIView, CListView)
	ON_WM_STYLECHANGED()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CManagerGUIView::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CManagerGUIView::OnLvnItemchanged)
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
	CListCtrl &guilist = GetListCtrl();
	guilist.ModifyStyle(NULL, LVS_REPORT);
	guilist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	guilist.DeleteAllItems();

	while (guilist.DeleteColumn(0));

	guilist.InsertColumn(0, TEXT("Path"), LVCFMT_LEFT, 400);
	guilist.InsertColumn(1, TEXT("Name"), LVCFMT_LEFT, 80);	
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
	guilist.DeleteAllItems();


	vector<FileInfo> &contents = GetDocument()->GetList();
	unsigned int count = 0;
	unsigned int length = guilist.GetItemCount();
	for (auto &fInfo : contents) {
		guilist.InsertItem(count + length, fInfo.path.c_str());
		guilist.SetItemText(count + length, 1, fInfo.name.c_str());

		count++;
		if(count > maxRows)
			break;
	}
}


void CManagerGUIView::OnLoadmore()
{
	// TODO: Add your command handler code here
	CListCtrl &guilist = GetListCtrl();

	vector<FileInfo> &contents = GetDocument()->GetList();
	unsigned int length = guilist.GetItemCount();
	for (size_t i = length; i < contents.size() && i < maxRows + length; ++ i) {
		const auto &fInfo = contents[i];
		guilist.InsertItem(i , fInfo.path.c_str());
		guilist.SetItemText(i , 1, fInfo.name.c_str());
	}
}


void CManagerGUIView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	CListCtrl &guilist = GetListCtrl();
	int index = pNMItemActivate->iItem;
	CString path = guilist.GetItemText(index, 0);
	CString name = guilist.GetItemText(index, 1);
	ShellExecute(0, 0, combinePathAndName(path.GetString(), name.GetString()).c_str(), 0, 0, SW_SHOW);

	*pResult = 0;
}


void CManagerGUIView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CListCtrl &guilist = GetListCtrl();
	int index = pNMLV->iItem;
	CString path = guilist.GetItemText(index, 0);
	CString name = guilist.GetItemText(index, 1);
	GetDocument()->rightView->OnTargetChanged(path, name);
	*pResult = 0;
}
