#if !defined(AFX_DLGSELECTMCVER_H__555204D3_21E5_441E_9BDA_D2625A691F4A__INCLUDED_)
#define AFX_DLGSELECTMCVER_H__555204D3_21E5_441E_9BDA_D2625A691F4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectMcVer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectMcVer dialog

class CDlgSelectMcVer : public CDialog
{
// Construction
public:
	CDlgSelectMcVer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectMcVer)
	enum { IDD = IDD_DLG_SELECT_MC_VER };
	int		m_nRadSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectMcVer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectMcVer)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTMCVER_H__555204D3_21E5_441E_9BDA_D2625A691F4A__INCLUDED_)
