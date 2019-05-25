// CRightView.cpp : implementation file
//

#include "stdafx.h"
#include "CRightView.h"

#include "FileSystem.h"
#include <shlobj.h>
#include <thumbcache.h>
#include <GdiPlus.h>


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CView)

CRightView::CRightView()
{
}

CRightView::~CRightView()
{
}

BEGIN_MESSAGE_MAP(CRightView, CView)
	ON_WM_CREATE()
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


int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	text.Create("text", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(10, 10, 400, 800), this);
	picture.Create("pic", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, CRect(10, 10, 400, 400), this);
	
	picture.BringWindowToTop();
	

	return 0;
}


void CRightView::OnTargetChanged(CString &path, CString &name)
{
	int dotIndex = name.ReverseFind('.');
	if (dotIndex == -1)
		return;
	int nSize = 300;
	HBITMAP thumbnail;

	tstring abName = combinePathAndName(path.GetString(), name.GetString()).c_str();
	CA2W ca2wName(abName.c_str());
	PCWSTR wpath = ca2wName.m_psz;
	HRESULT hr;

	IShellItem *psi;
	int l = lstrlenW(wpath);
	int m = MAX_PATH;
	//hr = ::SHCreateItemFromParsingName(L"D:\\\\1.jpg", NULL, IID_PPV_ARGS(&psi));
	hr = ::SHCreateItemFromParsingName(wpath, NULL, IID_PPV_ARGS(&psi));
	if (!SUCCEEDED(hr)) {
		return;
	}
	IThumbnailProvider *pThumbProvider;
	hr = psi->BindToHandler(NULL, BHID_ThumbnailHandler, IID_PPV_ARGS(&pThumbProvider));
	if (!SUCCEEDED(hr)) {
		return;
	}
	WTS_ALPHATYPE wtsAlpha;
	hr = pThumbProvider->GetThumbnail(nSize, &thumbnail, &wtsAlpha);
	if (!SUCCEEDED(hr)) {
		return;
	}
	

	//CString extension = name.Right(name.GetLength() - dotIndex - 1);
	//if (extension == "jpg" || extension == "png" || extension == "gif" || extension == "jpeg") {
	//	CImage image;
		//image.Load(combinePathAndName(path.GetString(), name.GetString()).c_str());
		text.BringWindowToTop();
	//	picture.SetBitmap(HBITMAP(image));
		picture.SetBitmap(thumbnail);
	//	RedrawWindow();
	//}
	//else {

	//}
}
