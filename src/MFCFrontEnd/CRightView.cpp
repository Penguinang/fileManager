// CRightView.cpp : implementation file
//

#include "stdafx.h"
#include "CRightView.h"


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CView)

CRightView::CRightView()
{

}

CRightView::~CRightView()
{
}

BEGIN_MESSAGE_MAP(CRightView, CView)
END_MESSAGE_MAP()


// CRightView drawing

void CRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightView message handlers
