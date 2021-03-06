#pragma once


// UpdateDialog dialog

class UpdateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(UpdateDialog)

public:
	UpdateDialog(shared_ptr<DBConnection> &conn, CWnd* pParent = nullptr);   // standard constructor
	virtual ~UpdateDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UpdateDialog };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl prgsCtrl;
	CStatic prgsText;

public:
	shared_ptr<DBConnection> conn;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetProgressText(LPCTSTR text);

private:
	CWinThread *updatingThread = nullptr;
};
