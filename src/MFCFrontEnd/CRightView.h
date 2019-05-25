#pragma once


// CRightView view

class CRightView : public CView
{
	DECLARE_DYNCREATE(CRightView)

protected:
	CRightView();           // protected constructor used by dynamic creation
	virtual ~CRightView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CStatic picture;
	CStatic text;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnTargetChanged(CString &path, CString &name);
};


