// UpdateDialog.cpp : implementation file
//
#include "stdafx.h"

#include <memory>
using std::shared_ptr;
#include "DBConnection.h"

#include "UpdateDialog.h"
#include "afxdialogex.h"
#include "resource.h"

#include "Scanner.h"


// UpdateDialog dialog

IMPLEMENT_DYNAMIC(UpdateDialog, CDialogEx)

UpdateDialog::UpdateDialog(shared_ptr<DBConnection> &conn, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UpdateDialog, pParent), conn(conn)
{

}

UpdateDialog::~UpdateDialog()
{
	if (updatingThread) {
		TerminateThread(updatingThread->m_hThread, 0);
		CloseHandle(updatingThread);
	}
}

void UpdateMessageProcess(const string &msg, void *dlgparam){
	UpdateDialog *dlg = static_cast<UpdateDialog*>(dlgparam);
	dlg->SetProgressText(msg.c_str());
}

UINT UpdateDatabaseThread(LPVOID p) {
	UpdateDialog *dlg = static_cast<UpdateDialog*>(p);
	Updater updater({ {
		TEXT("mp3"),  TEXT("wav"),  TEXT("jpg"),  TEXT("png"),  TEXT("gif"),  TEXT("jpeg"),
		TEXT("mp4"),  TEXT("mkv"),  TEXT("doc"),  TEXT("docx"),  TEXT("pdf") } }, {}, UpdateMessageProcess, dlg);
	UINT_PTR timer = dlg->SetTimer(1, 2000, NULL);
	shared_ptr<DBConnection> conn = dlg->conn;
	updater.Update(*conn);

	// Completed
	dlg->prgsCtrl.SetPos(100);
	dlg->KillTimer(timer);
	MessageBox(nullptr, "Updating Completed !", "Completed", 0);
	dlg->SetProgressText("Completed !");

	return 1;
}

void UpdateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPDATE_PROGRESS, prgsCtrl);
	prgsCtrl.SetRange(0, 100);
	prgsCtrl.SetPos(0);

	DDX_Control(pDX, IDC_UPDATEMESSAGE, prgsText);
	prgsText.SetWindowTextA("Processing ...");

	// Create a thread to update database
	if (updatingThread) {
		TerminateThread(updatingThread->m_hThread, 0);
		CloseHandle(updatingThread);
	}
	updatingThread = AfxBeginThread(UpdateDatabaseThread, this);
}


BEGIN_MESSAGE_MAP(UpdateDialog, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void UpdateDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnTimer(nIDEvent);	
	prgsCtrl.SetPos(prgsCtrl.GetPos() + 1);
}

void UpdateDialog::SetProgressText(LPCTSTR text) {
	prgsText.SetWindowTextA(text);
}
