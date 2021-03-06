//{{AFX_INCLUDES()
#include "NiGraph.h"
#include "NiButton.h"
#include "NiNumEdit.h"
//}}AFX_INCLUDES

#if !defined(AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_)
#define AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainView form view

#include "DlgGraphSetup.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMainView : public CFormView
{
protected:
	CMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMainView)

// Form Data
public:
	//{{AFX_DATA(CMainView)
	enum { IDD = IDD_MAIN_WINDOW };
	CNiGraph	m_ctlSmallGraph1;
	CNiGraph	m_ctlSmallGraph2;
	CNiGraph	m_ctlSmallGraph3;
	CNiGraph	m_ctlSmallGraph4;
	CNiGraph	m_ctlSmallGraph5;
	CNiGraph	m_ctlSmallGraph6;
	CNiGraph	m_ctlSmallGraph7;
	CNiGraph	m_ctlSmallGraph8;
	CNiGraph	m_ctlMainGraph;
	int		m_nTMeaningInd;
	double	m_dKimpSec;
	CString	m_strSoftwareVersion;
	CString	m_strThisSoftwareVersion;
	CString	m_strLblCursorDxVal;
	CString	m_strLblCursorDyVal;
	CString	m_strCur1X_val;
	CString	m_strCur1Y_val;
	CString	m_strCur2X_val;
	CString	m_strCur2Y_val;
	int		m_nRadMeaningGraph1;
	int		m_nRadMeaningGraph2;
	int		m_nRadMeaningGraph3;
	int		m_nRadMeaningGraph4;
	int		m_nRadMeaningGraph5;
	int		m_nRadMeaningGraph6;
	int		m_nRadMeaningGraph7;
	int		m_nRadMeaningGraph8;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void SetRefreshTimer( int nPeriod = 100);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMainView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickGraph1();
	afx_msg void OnClickGraph2();
	afx_msg void OnClickGraph3();
	afx_msg void OnClickGraph4();
	afx_msg void OnClickGraph5();
	afx_msg void OnClickGraph6();
	afx_msg void OnClickGraph7();
	afx_msg void OnClickGraph8();
	afx_msg void OnDestroy();
	afx_msg void OnChangeParam2Edt();
	afx_msg void OnChangeParam1Edt();
	afx_msg void OnChangeParam3Edt();
	afx_msg void OnChangeParam4Edt();
	afx_msg void OnKillfocusEdtKImpSec();
	afx_msg void OnHistogram();
	afx_msg void OnAlanCurve();
	afx_msg void OnBtnExport();
	afx_msg void OnRadMeaning100msec();
	afx_msg void OnRadMeaning1sec();
	afx_msg void OnRadMeaning10sec();
	afx_msg void OnRadMeaning100sec();
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestoreZoom(CCmdUI* pCmdUI);
	afx_msg void OnZoomin();
	afx_msg void OnRestoreZoom();
	afx_msg void OnShowCursors();
	afx_msg void OnCursorChangeMainGraph(long FAR* CursorIndex, VARIANT FAR* XPos, VARIANT FAR* YPos, BOOL FAR* bTracking);
	afx_msg void OnMouseUpGraph1(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph2(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph3(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph4(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph5(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph6(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph7(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph8(short Button, short Shift, long x, long y);
	afx_msg void OnRadMeaning5();
	afx_msg void OnRadMeaningGraph();
	afx_msg void OnCloseupCmbGraph();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bEmergencyCodeApperance;
	CFont m_pFont;
	int m_nMainGraph;
	void RefreshGraphs( void);
  CDlgGraphSetup *m_dlgGraphSetup;
  CNiColor m_clrSelected;
  CNiColor m_clrFree;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_)
