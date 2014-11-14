// TestGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestGame.h"
#include "TestGameDlg.h"
#include "..\HsUserUtil\HsUserUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGameDlg dialog

CTestGameDlg::CTestGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestGameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestGameDlg)
	m_szPath = _T("C:\\Program Files");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bUseDll = FALSE;
	m_pfn_AhnHSUserUtil_CreateUser = NULL;
	m_pfn_AhnHSUserUtil_SetFolderPermissionA = NULL;
	m_pfn_AhnHSUserUtil_SetFolderPermissionW = NULL;
	m_pfn_AhnHSUserUtil_DeleteUser = NULL;
	m_pfn_AhnHSUserUtil_IsEnableHSAdminRights = NULL;
	m_pfn_AhnHSUserUtil_CheckHSShadowAccount = NULL;
	m_pfn_AhnHSUserUtil_IsAdmin = NULL;
	m_hUserUtil = NULL;
}

void CTestGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestGameDlg)
	DDX_Text(pDX, IDC_EDIT_PATH, m_szPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestGameDlg, CDialog)
	//{{AFX_MSG_MAP(CTestGameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_USERGEN, OnBtnUsergen)
	ON_BN_CLICKED(IDC_BTN_SETPATH, OnBtnSetpath)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_USERDEL, OnBtnUserdel)
	ON_BN_CLICKED(IDC_BTN_ISADMIN, OnBtnIsAdmin)
	ON_BN_CLICKED(IDC_BTN_EXIST_SHUSER, OnBtnExistHShuser)
	ON_BN_CLICKED(IDC_BTN_IS_ENABLE_HS, OnBtnIsEnableHs)
	ON_BN_CLICKED(IDC_USE_DLL, OnCheckUseDll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGameDlg message handlers

BOOL CTestGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestGameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestGameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestGameDlg::OnBtnUsergen() 
{
	// TODO: Add your control notification handler code here
	DWORD dwRet = HSUSERUTIL_ERR_OK;

	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_CreateUser != NULL )
	{
		dwRet = m_pfn_AhnHSUserUtil_CreateUser( );
	}
	else
	{	
		dwRet = _AhnHsUserUtil_CreateUser ( );
	}

	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_NOT_ADMIN:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_ADMIN");
			break;
		case HSUSERUTIL_ERR_DELSHADOWACNT_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_DELSHADOWACNT_FAIL" );
			break;
        case HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL" );
			break;
		case HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_ADDSHADOWACNT_FAIL" );
			break;
		case HSUSERUTIL_ERR_OK:
			AfxMessageBox ( "HSUSERUTIL_ERR_OK" );
			break;
		default:
			AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}

}

void CTestGameDlg::OnBtnSetpath() 
{

	// TODO: Add your control notification handler code here
	
	DWORD dwRet = HSUSERUTIL_ERR_OK;
	TCHAR szbuf[MAX_PATH] = { 0, };

	UpdateData(TRUE);
	_stprintf(szbuf, _T("%s"), m_szPath);

#if defined(UNICODE) | defined(_UNICODE)
	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_SetFolderPermissionW != NULL )
	{
		
		dwRet = m_pfn_AhnHSUserUtil_SetFolderPermissionW(szbuf);
	}
	else
	{	
		dwRet = _AhnHsUserUtil_SetFolderPermission(szbuf); 
	}
#else
	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_SetFolderPermissionA != NULL )
	{
		
		dwRet = m_pfn_AhnHSUserUtil_SetFolderPermissionA(szbuf);
	}
	else
	{	
		dwRet = _AhnHsUserUtil_SetFolderPermission(szbuf); 
	}
#endif
	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_SETFLDRPERMISSION_FAIL" );
			break;
        case HSUSERUTIL_ERR_GETGROUPSID_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_GETGROUPSID_FAIL" );
			break;
		case HSUSERUTIL_ERR_GETSECINFO_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_GETSECINFO_FAIL" );
			break;
		case HSUSERUTIL_ERR_ADDNEWACE_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_ADDNEWACE_FAIL" );
			break;
		case HSUSERUTIL_ERR_ADDNEWDACL_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_ADDNEWDACL_FAIL" );
			break;
		case HSUSERUTIL_ERR_COPYOLDDACL_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_COPYOLDDACL_FAIL" );
			break;
		case HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_ADDNEWACETONEWDACL_FAIL" );
			break;
		case HSUSERUTIL_ERR_OK:
			AfxMessageBox ( "HSUSERUTIL_ERR_OK" );
			break;
		default:
			TRACE("ERROR CODE : %X \n", dwRet);
			AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}

}


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM dwData)
{
    switch (uMsg) 
	{
		case BFFM_SELCHANGED:
			TCHAR szText[_MAX_PATH] = {0};
			SHGetPathFromIDList(reinterpret_cast<LPITEMIDLIST>(lParam), szText);
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, reinterpret_cast<LPARAM>(szText));
			break;
    }
    return 0;
}

void CTestGameDlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
	TCHAR szPath[_MAX_PATH] = { 0, };

    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner	= this->m_hWnd;			// 현재 핸들
    bi.pidlRoot		= NULL;
    bi.lpszTitle	= "HSUSERUTIL PATH";    // 폴더선택창 열었을때 보여지는 문구
    bi.lpfn			= BrowseCallbackProc;   // 콜백함수
    bi.lParam		= 0;
    bi.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_STATUSTEXT;

    LPITEMIDLIST pidlFolder = SHBrowseForFolder(&bi);
    if (pidlFolder == NULL)
		return;
 
    if(!SHGetPathFromIDList(pidlFolder, szPath))
	{
		AfxMessageBox("HSUSERUTIL FIND FOLDER FAIL");
		return;
	}

    if (strlen(szPath) == 3) {
        szPath[2] = '\0';
    }

	m_szPath = szPath;
	UpdateData(FALSE);
}

void CTestGameDlg::OnBtnUserdel() 
{
	DWORD dwRet = HSUSERUTIL_ERR_OK;

	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_DeleteUser != NULL )
	{
		dwRet = m_pfn_AhnHSUserUtil_DeleteUser();
	}
	else
	{	
		dwRet = _AhnHsUserUtil_DeleteUser();
	}

	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_NOT_ADMIN:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_ADMIN");
			break;
		case HSUSERUTIL_ERR_DELSHADOWACNT_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_DELSHADOWACNT_FAIL" );
			break;
        case HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL:
			AfxMessageBox ( "HSUSERUTIL_ERR_DELHIDEIDINFO_FAIL" );
			break;
		case HSUSERUTIL_ERR_OK:
			AfxMessageBox ( "HSUSERUTIL_ERR_OK" );
			break;
		default:
			AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}	
}

void CTestGameDlg::OnBtnIsAdmin() 
{

	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_IsAdmin != NULL )
	{
		if ( TRUE == m_pfn_AhnHSUserUtil_IsAdmin() )
		{
			AfxMessageBox ( "TRUE" );
		}
		else
		{
			AfxMessageBox ( "FALSE" );
		}
	}
	else
	{	
		if ( TRUE == _AhnHSUserUtil_IsAdmin() )
		{
			AfxMessageBox ( "TRUE" );
		}
		else
		{
			AfxMessageBox ( "FALSE" );
		}
	}

}

void CTestGameDlg::OnBtnExistHShuser() 
{

	DWORD dwRet = HSUSERUTIL_ERR_OK;

	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_CheckHSShadowAccount != NULL )
	{
		dwRet = m_pfn_AhnHSUserUtil_CheckHSShadowAccount();
	}
	else
	{	
		dwRet = _AhnHsUserUtil_CheckHSShadowAccount();
	}

	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_OK:
			AfxMessageBox ( "HSUSERUTIL_ERR_OK" );
			break;
		case HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST:
			AfxMessageBox ( "HSUSERUTIL_ERR_SHADOWACNT_NOT_EXIST" );
			break;
		default:
			AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}	
	
}

void CTestGameDlg::OnBtnIsEnableHs() 
{
	// TODO: Add your control notification handler code here

	DWORD dwRet = HSUSERUTIL_ERR_OK;

	if ( m_bUseDll == TRUE && m_pfn_AhnHSUserUtil_IsEnableHSAdminRights != NULL )
	{
		dwRet = m_pfn_AhnHSUserUtil_IsEnableHSAdminRights();
	}
	else
	{	
		dwRet = _AhnHsUserUtil_IsEnableHSAdminRights();
	}	

	switch ( dwRet )
	{
		case HSUSERUTIL_ERR_NOT_NT:
			AfxMessageBox ( "HSUSERUTIL_ERR_NOT_NT" );
			break;
		case HSUSERUTIL_ERR_OK:
			AfxMessageBox ( "HSUSERUTIL_ERR_OK" );
			break;
		default:
			AfxMessageBox ( "HSUSERUTIL_ERR_UNKNOWN" );
			break;	
	}	
	
}




void CTestGameDlg::OnCheckUseDll()
{

	if ( m_bUseDll == TRUE )
	{
		m_bUseDll = FALSE;
		return;
	}

	if ( m_hUserUtil != NULL )
	{
		m_bUseDll = TRUE;
		return;
	}

	CHAR szUserUtilDll_Dll[] = "HsUserUtilDll.dll";
	m_hUserUtil = LoadLibraryA ( szUserUtilDll_Dll );
	if ( m_hUserUtil == NULL )
	{
		CString strMsg;
		strMsg.Format("%s Load Fail. %d", szUserUtilDll_Dll, GetLastError());
		AfxMessageBox( strMsg );
		return;
	}

	m_pfn_AhnHSUserUtil_CreateUser = (PFN_AhnHSUserUtil_CreateUser)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 1, 0 ));
	if ( NULL == m_pfn_AhnHSUserUtil_CreateUser )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_CreateUser GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_CreateUser = NULL;
	}

#if defined(UNICODE) | defined(_UNICODE)
	m_pfn_AhnHSUserUtil_SetFolderPermissionW = (PFN_AhnHSUserUtil_SetFolderPermissionW)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 7, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_SetFolderPermissionW )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_SetFolderPermissionW GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_SetFolderPermissionW = NULL;
	}
#else
	m_pfn_AhnHSUserUtil_SetFolderPermissionA = (PFN_AhnHSUserUtil_SetFolderPermissionA)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 2, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_SetFolderPermissionA )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_SetFolderPermissionA GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_SetFolderPermissionA = NULL;
	}
#endif
	

	m_pfn_AhnHSUserUtil_DeleteUser = (PFN_AhnHsUserUtil_DeleteUser)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 3, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_DeleteUser )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_DeleteUser GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_DeleteUser = NULL;
	}

	m_pfn_AhnHSUserUtil_IsEnableHSAdminRights = (PFN_AhnHSUserUtil_IsEnableHSAdminRights)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 4, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_IsEnableHSAdminRights )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_IsEnableHSAdminRights GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_IsEnableHSAdminRights = NULL;
	}

	m_pfn_AhnHSUserUtil_CheckHSShadowAccount = (PFN_AhnHSUserUtil_CheckHSShadowAccount)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 5, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_CheckHSShadowAccount )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_CheckHSShadowAccount GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_CheckHSShadowAccount = NULL;
	}

	m_pfn_AhnHSUserUtil_IsAdmin = (PFN_AhnHSUserUtil_IsAdmin)GetProcAddress ( m_hUserUtil, (const char*)MAKELONG ( 6, 0 ));
	if ( NULL ==  m_pfn_AhnHSUserUtil_IsAdmin )
	{
		AfxMessageBox( "m_pfn_AhnHSUserUtil_IsAdmin GetProcAddress Fail. %d", GetLastError() );
		m_pfn_AhnHSUserUtil_IsAdmin = NULL;
	}

	m_bUseDll = TRUE;

	CString strMsg;
	strMsg.Format("%s Load Complete!", szUserUtilDll_Dll);
	AfxMessageBox ( strMsg );

	return;

}
