
// ManagerGUIView.h : interface of the CManagerGUIView class
//

#pragma once


class CManagerGUIView : public CListView
{
protected: // create from serialization only
	CManagerGUIView() noexcept;
	DECLARE_DYNCREATE(CManagerGUIView)

// Attributes
public:
	CManagerGUIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CManagerGUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ManagerGUIView.cpp
inline CManagerGUIDoc* CManagerGUIView::GetDocument() const
   { return reinterpret_cast<CManagerGUIDoc*>(m_pDocument); }
#endif

