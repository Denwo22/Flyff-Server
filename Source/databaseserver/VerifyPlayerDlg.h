#if !defined(AFX_PROGRESSDLG_H__D52E5A0B_1757_4023_B9AC_E3E899BADC72__INCLUDED_)
#define AFX_PROGRESSDLG_H__D52E5A0B_1757_4023_B9AC_E3E899BADC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerifyPlayerDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CVerifyPlayerDlg dialog

class CVerifyPlayerDlg : public CDialog
{
// Construction
public:
	CVerifyPlayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifyPlayerDlg)
	enum { IDD = IDD_VERIFYPLAYER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifyPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVerifyPlayerDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__D52E5A0B_1757_4023_B9AC_E3E899BADC72__INCLUDED_)
