
// ManagerGUIDoc.h : interface of the CManagerGUIDoc class
//


#pragma once

class Scanner;
class DBConnection;

#include <memory>
using std::shared_ptr;
using std::unique_ptr;
#include <vector>
using std::vector;

#include "tstring.h"

enum class MEDIA{
	SONG,
	MOVIES,
	PICTURES,
	DOCUMENTS,
	ALL
};

class CManagerGUIDoc : public CDocument
{
protected: // create from serialization only
	CManagerGUIDoc() noexcept;
	DECLARE_DYNCREATE(CManagerGUIDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CManagerGUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	shared_ptr<Locater> locater;
	shared_ptr<DBConnection> conn;

public:
	vector<FileInfo> list;
	void OnTreeSelChanged(MEDIA type);
	afx_msg void OnUpdateDB();
};
