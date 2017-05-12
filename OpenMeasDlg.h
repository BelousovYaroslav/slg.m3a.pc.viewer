//{{AFX_INCLUDES()
#include "NiNumEdit.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPENMEASDLG_H__084F0A36_491F_4EBF_8D6E_BA988A36552F__INCLUDED_)
#define AFX_OPENMEASDLG_H__084F0A36_491F_4EBF_8D6E_BA988A36552F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenMeasDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenMeasDlg dialog

struct Step1Info {
  long lPacks;
  int nMarkerFails;
  int nCheckSummFails;
  int nCounterFails;  
  double dblTime;
  long lDeviceId;
  char cSyncAsyncUnknown;
  char cHaveRegimedNdU;
  char bSignCoeff;
  double dblDecCoeffMin;
  double dblDecCoeffMax;
  double dblDecCoeffMean;
  int nDecCoeffData;
};

class COpenMeasDlg : public CDialog
{
// Construction
public:
  long m_dn100m;
  int m_nVersionMajor;
  int m_nVersionMiddle;
  int m_nVersionMinor;
	CString m_strOpenFilePathName;
	COpenMeasDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenMeasDlg)
	enum { IDD = IDD_OPEN_MEAS_DLG };
	CProgressCtrl	m_ctlProgress0;
	CProgressCtrl	m_ctlProgress1;
	CProgressCtrl	m_ctlProgress2;
	CNiNumEdit	m_ctlSignCoeff;
	CNiNumEdit	m_ctlSkipPacks;
	int		m_nRadSkip;
	CNiNumEdit	m_ctlSkipMsecs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenMeasDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenMeasDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStopLoad();
	afx_msg void OnBtnGoon();
	afx_msg void OnRadSkipPacks();
	afx_msg void OnRadSkipTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  long m_lFileLen;
  long m_lFilePos;
  long m_lPacks;
  int m_nMarkerFails;
  int m_nCounterFails;
  int m_nCheckSummFails;
  Step1Info m_pStep1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENMEASDLG_H__084F0A36_491F_4EBF_8D6E_BA988A36552F__INCLUDED_)
