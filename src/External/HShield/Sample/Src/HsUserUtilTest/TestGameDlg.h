// TestGameDlg.h : header file
//

#if !defined(AFX_TESTGAMEDLG_H__3DD2D212_D846_4F02_B4E7_557D243B4083__INCLUDED_)
#define AFX_TESTGAMEDLG_H__3DD2D212_D846_4F02_B4E7_557D243B4083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef DWORD (WINAPI* PFN_AhnHSUserUtil_CreateUser) ();
typedef DWORD (WINAPI* PFN_AhnHSUserUtil_SetFolderPermissionA) ( LPSTR szPathA );
typedef DWORD (WINAPI* PFN_AhnHSUserUtil_SetFolderPermissionW) ( LPWSTR szPathW );
typedef DWORD (WINAPI* PFN_AhnHsUserUtil_DeleteUser ) ();
typedef DWORD (WINAPI* PFN_AhnHSUserUtil_IsEnableHSAdminRights) ();
typedef DWORD (WINAPI* PFN_AhnHSUserUtil_CheckHSShadowAccount) ();
typedef BOOL  (WINAPI* PFN_AhnHSUserUtil_IsAdmin) ();

/////////////////////////////////////////////////////////////////////////////
// CTestGameDlg dialog

class CTestGameDlg : public CDialog
{
// Construction
public:
	CTestGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestGameDlg)
	enum { IDD = IDD_TESTGAME_DIALOG };
	CString	m_szPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnCheckUseDll();
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestGameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnUsergen();
	afx_msg void OnBtnSetpath();
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnUserdel();
	afx_msg void OnBtnIsAdmin();
	afx_msg void OnBtnExistHShuser();
	afx_msg void OnBtnIsEnableHs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUseDll;
	PFN_AhnHSUserUtil_CreateUser			m_pfn_AhnHSUserUtil_CreateUser;
	PFN_AhnHSUserUtil_SetFolderPermissionA	m_pfn_AhnHSUserUtil_SetFolderPermissionA;
	PFN_AhnHSUserUtil_SetFolderPermissionW	m_pfn_AhnHSUserUtil_SetFolderPermissionW;
	PFN_AhnHsUserUtil_DeleteUser			m_pfn_AhnHSUserUtil_DeleteUser;
	PFN_AhnHSUserUtil_IsEnableHSAdminRights m_pfn_AhnHSUserUtil_IsEnableHSAdminRights;
	PFN_AhnHSUserUtil_CheckHSShadowAccount	m_pfn_AhnHSUserUtil_CheckHSShadowAccount;
	PFN_AhnHSUserUtil_IsAdmin				m_pfn_AhnHSUserUtil_IsAdmin;
	HMODULE	m_hUserUtil;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGAMEDLG_H__3DD2D212_D846_4F02_B4E7_557D243B4083__INCLUDED_)
