// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "Slg2Doc.h"
#include "MainView.h"
#include "MainFrm.h"
#include "DlgHisto.h"
#include "DlgAlanCurve.h"
#include "DlgExport.h"
#include <winver.h>
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_TIMER_1000			1101

#define MY_MAXIMIZE_VIEW_TIMER 1002
#define MY_REFRESH_VIEW_TIMER 1003


extern CSlg2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

CMainView::CMainView()
	: CFormView(CMainView::IDD)
{
	//{{AFX_DATA_INIT(CMainView)
	m_nTMeaningInd = 0;
	m_dKimpSec = 0.0;
	m_strSoftwareVersion = _T("");
	m_strThisSoftwareVersion = _T("");
	m_strLblCursorDxVal = _T("");
	m_strLblCursorDyVal = _T("");
	m_strCur1X_val = _T("");
	m_strCur1Y_val = _T("");
	m_strCur2X_val = _T("");
	m_strCur2Y_val = _T("");
	m_nRadMeaningGraph1 = 0;
	m_nRadMeaningGraph2 = 0;
	m_nRadMeaningGraph3 = 0;
	m_nRadMeaningGraph4 = 0;
	m_nRadMeaningGraph5 = 0;
	m_nRadMeaningGraph6 = 0;
	m_nRadMeaningGraph7 = 0;
	m_nRadMeaningGraph8 = 0;
	//}}AFX_DATA_INIT
	m_bEmergencyCodeApperance = false;
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));									// Zero out the structure.
	lf.lfHeight = 20;																	// Request a 12-pixel-height font.
	lstrcpy(lf.lfFaceName, _T("Times New Roman"));   // Request a face name "Arial".
	VERIFY(m_pFont.CreateFontIndirect(&lf));							// Create the font.
}

CMainView::~CMainView()
{
  if( m_dlgGraphSetup != NULL) {
    delete m_dlgGraphSetup; m_dlgGraphSetup = NULL;
  }
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainView)
	DDX_Control(pDX, IDC_GRAPH1, m_ctlSmallGraph1);
	DDX_Control(pDX, IDC_GRAPH2, m_ctlSmallGraph2);
	DDX_Control(pDX, IDC_GRAPH3, m_ctlSmallGraph3);
	DDX_Control(pDX, IDC_GRAPH4, m_ctlSmallGraph4);
	DDX_Control(pDX, IDC_GRAPH5, m_ctlSmallGraph5);
	DDX_Control(pDX, IDC_GRAPH6, m_ctlSmallGraph6);
	DDX_Control(pDX, IDC_GRAPH7, m_ctlSmallGraph7);
	DDX_Control(pDX, IDC_GRAPH8, m_ctlSmallGraph8);
	DDX_Control(pDX, IDC_MAIN_GRAPH, m_ctlMainGraph);
	DDX_Radio(pDX, IDC_RAD_MEANING5, m_nTMeaningInd);
	DDX_Text(pDX, IDC_EDT_K_IMP_SEC, m_dKimpSec);
	DDX_Text(pDX, IDC_SOFTWARE_VERSION, m_strSoftwareVersion);
	DDX_Text(pDX, IDC_THIS_SOFTWARE_VERSION, m_strThisSoftwareVersion);
	DDX_Text(pDX, IDC_LBL_CURSOR_DX_VAL, m_strLblCursorDxVal);
	DDX_Text(pDX, IDC_LBL_CURSOR_DY_VAL, m_strLblCursorDyVal);
	DDX_Text(pDX, IDC_LBL_CURSOR1_X_VAL, m_strCur1X_val);
	DDX_Text(pDX, IDC_LBL_CURSOR1_Y_VAL, m_strCur1Y_val);
	DDX_Text(pDX, IDC_LBL_CURSOR2_X_VAL, m_strCur2X_val);
	DDX_Text(pDX, IDC_LBL_CURSOR2_Y_VAL, m_strCur2Y_val);
	DDX_Radio(pDX, IDC_RAD_G1_T0, m_nRadMeaningGraph1);
	DDX_Radio(pDX, IDC_RAD_G2_T0, m_nRadMeaningGraph2);
	DDX_Radio(pDX, IDC_RAD_G3_T0, m_nRadMeaningGraph3);
	DDX_Radio(pDX, IDC_RAD_G4_T0, m_nRadMeaningGraph4);
	DDX_Radio(pDX, IDC_RAD_G5_T0, m_nRadMeaningGraph5);
	DDX_Radio(pDX, IDC_RAD_G6_T0, m_nRadMeaningGraph6);
	DDX_Radio(pDX, IDC_RAD_G7_T0, m_nRadMeaningGraph7);
	DDX_Radio(pDX, IDC_RAD_G8_T0, m_nRadMeaningGraph8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainView, CFormView)
	//{{AFX_MSG_MAP(CMainView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDT_K_IMP_SEC, OnKillfocusEdtKImpSec)
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_COMMAND(ID_ALAN_CURVE, OnAlanCurve)
	ON_BN_CLICKED(IDC_BTN_EXPORT, OnBtnExport)
	ON_BN_CLICKED(IDC_RAD_MEANING1, OnRadMeaning100msec)
	ON_BN_CLICKED(IDC_RAD_MEANING2, OnRadMeaning1sec)
	ON_BN_CLICKED(IDC_RAD_MEANING3, OnRadMeaning10sec)
	ON_BN_CLICKED(IDC_RAD_MEANING4, OnRadMeaning100sec)
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomin)
	ON_UPDATE_COMMAND_UI(ID_RESTORE_ZOOM, OnUpdateRestoreZoom)
  ON_COMMAND(ID_ZOOMIN, OnZoomin)
  ON_COMMAND(ID_RESTORE_ZOOM, OnRestoreZoom)
  ON_COMMAND(ID_SHOW_CURSORS, OnShowCursors)
  ON_BN_CLICKED(IDC_RAD_MEANING5, OnRadMeaning5)
  ON_BN_CLICKED(IDC_RAD_G1_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G1_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G1_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G1_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G1_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G1_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G2_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G3_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G4_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G5_T5, OnRadMeaningGraph)  
  ON_BN_CLICKED(IDC_RAD_G6_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G6_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G6_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G6_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G6_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G6_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G7_T5, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T0, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T1, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T2, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T3, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T4, OnRadMeaningGraph)
  ON_BN_CLICKED(IDC_RAD_G8_T5, OnRadMeaningGraph)
	ON_CBN_CLOSEUP(IDC_CMB_GRAPH1_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH2_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH3_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH4_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH5_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH6_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH7_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH8_X, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH1_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH2_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH3_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH4_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH5_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH6_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH7_Y, OnCloseupCmbGraph)
  ON_CBN_CLOSEUP(IDC_CMB_GRAPH8_Y, OnCloseupCmbGraph)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers

void CMainView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	int nGrWidth = cx / 8;
	CRect rc;

	if( m_ctlSmallGraph1.m_hWnd) {
		GetDlgItem( IDC_CMB_GRAPH1_Y)->SetWindowPos( NULL, 0, 50, nGrWidth, 12, SWP_NOZORDER);
    m_ctlSmallGraph1.GetWindowRect( &rc);
		m_ctlSmallGraph1.SetWindowPos( NULL, 0, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T0)->SetWindowPos( NULL, nGrWidth * 1 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T1)->SetWindowPos( NULL, nGrWidth * 1 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T2)->SetWindowPos( NULL, nGrWidth * 1 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T3)->SetWindowPos( NULL, nGrWidth * 1 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T4)->SetWindowPos( NULL, nGrWidth * 1 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T5)->SetWindowPos( NULL, nGrWidth * 1 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH1_X)->SetWindowPos( NULL, 0, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}
	
	if( m_ctlSmallGraph2.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH2_Y)->SetWindowPos( NULL, nGrWidth, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph2.GetWindowRect( &rc);
		m_ctlSmallGraph2.SetWindowPos( NULL, nGrWidth, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T0)->SetWindowPos( NULL, nGrWidth * 2 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T1)->SetWindowPos( NULL, nGrWidth * 2 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T2)->SetWindowPos( NULL, nGrWidth * 2 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T3)->SetWindowPos( NULL, nGrWidth * 2 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T4)->SetWindowPos( NULL, nGrWidth * 2 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T5)->SetWindowPos( NULL, nGrWidth * 2 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH2_X)->SetWindowPos( NULL, nGrWidth, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph3.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH3_Y)->SetWindowPos( NULL, nGrWidth * 2, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph3.GetWindowRect( &rc);
		m_ctlSmallGraph3.SetWindowPos( NULL, nGrWidth * 2, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T0)->SetWindowPos( NULL, nGrWidth * 3 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T1)->SetWindowPos( NULL, nGrWidth * 3 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T2)->SetWindowPos( NULL, nGrWidth * 3 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T3)->SetWindowPos( NULL, nGrWidth * 3 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T4)->SetWindowPos( NULL, nGrWidth * 3 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T5)->SetWindowPos( NULL, nGrWidth * 3 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH3_X)->SetWindowPos( NULL, nGrWidth * 2, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph4.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH4_Y)->SetWindowPos( NULL, nGrWidth * 3, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph4.GetWindowRect( &rc);
		m_ctlSmallGraph4.SetWindowPos( NULL, nGrWidth * 3, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T0)->SetWindowPos( NULL, nGrWidth * 4 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T1)->SetWindowPos( NULL, nGrWidth * 4 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T2)->SetWindowPos( NULL, nGrWidth * 4 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T3)->SetWindowPos( NULL, nGrWidth * 4 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T4)->SetWindowPos( NULL, nGrWidth * 4 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T5)->SetWindowPos( NULL, nGrWidth * 4 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH4_X)->SetWindowPos( NULL, nGrWidth * 3, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph5.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH5_Y)->SetWindowPos( NULL, nGrWidth * 4, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph5.GetWindowRect( &rc);
		m_ctlSmallGraph5.SetWindowPos( NULL, nGrWidth * 4, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T0)->SetWindowPos( NULL, nGrWidth * 5 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T1)->SetWindowPos( NULL, nGrWidth * 5 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T2)->SetWindowPos( NULL, nGrWidth * 5 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T3)->SetWindowPos( NULL, nGrWidth * 5 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T4)->SetWindowPos( NULL, nGrWidth * 5 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T5)->SetWindowPos( NULL, nGrWidth * 5 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH5_X)->SetWindowPos( NULL, nGrWidth * 4, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph6.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH6_Y)->SetWindowPos( NULL, nGrWidth * 5, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph6.GetWindowRect( &rc);
		m_ctlSmallGraph6.SetWindowPos( NULL, nGrWidth * 5, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T0)->SetWindowPos( NULL, nGrWidth * 6 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T1)->SetWindowPos( NULL, nGrWidth * 6 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T2)->SetWindowPos( NULL, nGrWidth * 6 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T3)->SetWindowPos( NULL, nGrWidth * 6 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T4)->SetWindowPos( NULL, nGrWidth * 6 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T5)->SetWindowPos( NULL, nGrWidth * 6 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH6_X)->SetWindowPos( NULL, nGrWidth * 5, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph7.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH7_Y)->SetWindowPos( NULL, nGrWidth * 6, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph7.GetWindowRect( &rc);
		m_ctlSmallGraph7.SetWindowPos( NULL, nGrWidth * 6, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T0)->SetWindowPos( NULL, nGrWidth * 7 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T1)->SetWindowPos( NULL, nGrWidth * 7 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T2)->SetWindowPos( NULL, nGrWidth * 7 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T3)->SetWindowPos( NULL, nGrWidth * 7 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T4)->SetWindowPos( NULL, nGrWidth * 7 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T5)->SetWindowPos( NULL, nGrWidth * 7 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH7_X)->SetWindowPos( NULL, nGrWidth * 6, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph8.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH8_Y)->SetWindowPos( NULL, nGrWidth * 7, 50, nGrWidth, 12, SWP_NOZORDER);
		m_ctlSmallGraph8.GetWindowRect( &rc);
		m_ctlSmallGraph8.SetWindowPos( NULL, nGrWidth * 7, 70, nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T0)->SetWindowPos( NULL, nGrWidth * 8 - 21, 75, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T1)->SetWindowPos( NULL, nGrWidth * 8 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T2)->SetWindowPos( NULL, nGrWidth * 8 - 21, 105, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T3)->SetWindowPos( NULL, nGrWidth * 8 - 21, 120, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T4)->SetWindowPos( NULL, nGrWidth * 8 - 21, 135, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T5)->SetWindowPos( NULL, nGrWidth * 8 - 21, 150, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH8_X)->SetWindowPos( NULL, nGrWidth * 7, 70 + rc.Height(), nGrWidth, 12, SWP_NOZORDER);
	}

	int base = 95 + rc.Height();
	CWnd *pWnd = GetDlgItem( IDC_GRAPH1_MIN_LABEL);
	if( pWnd != NULL) {
		CRect rc1;
		
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH1_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_MAX_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_RMS_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		
		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH2_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH2_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH3_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH3_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH4_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH4_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH5_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH5_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH6_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH6_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH7_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH7_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 95 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH8_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH8_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();
	}

	CRect m_rcStatusBar;
	if( m_ctlMainGraph.m_hWnd) {
		(( CMainFrame*) this->GetParentFrame())->m_wndStatusBar.GetWindowRect( m_rcStatusBar);
		m_ctlMainGraph.SetWindowPos( NULL, 5, base + 5, cx - 210 - 10, cy - base, SWP_NOZORDER);
	}

	pWnd = GetDlgItem( IDC_STATISTIC_FRAME);
	if( pWnd != NULL) {
		pWnd->SetWindowPos( NULL, cx - 210, base + 5, 210, cy - base, SWP_NOZORDER);
		
		pWnd = GetDlgItem( IDC_MIN_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 20, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 40, 200, 20, SWP_NOZORDER);

		pWnd = GetDlgItem( IDC_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 70, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 90, 200, 20, SWP_NOZORDER);
		
		pWnd = GetDlgItem( IDC_MAX_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 120, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 140, 200, 20, SWP_NOZORDER);
		
		pWnd = GetDlgItem( IDC_RMS_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 170, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 210 + 5, base + 190, 200, 20, SWP_NOZORDER);
	}


}

void CMainView::RefreshGraphs()
{
  BeginWaitCursor();
  CSlg2Doc *doc = ( CSlg2Doc *) GetDocument();
  if( !doc->m_dpW.IsInitialized()) return;

  doc->RecalculateStatistics();

  //цикл по объектам графикам
  for( int i=0; i<8; i++) {
    CNiGraph *graphSmall = NULL;
    CNiGraph *graphBig = NULL;
    switch( i) {
      case 1: graphSmall  = &m_ctlSmallGraph2; break;
      case 2: graphSmall  = &m_ctlSmallGraph3; break;
      case 3: graphSmall  = &m_ctlSmallGraph4; break;
      case 4: graphSmall  = &m_ctlSmallGraph5; break;
      case 5: graphSmall  = &m_ctlSmallGraph6; break;
      case 6: graphSmall  = &m_ctlSmallGraph7; break;
      case 7: graphSmall  = &m_ctlSmallGraph8; break;
      default: graphSmall = &m_ctlSmallGraph1; break;
    }
    
    if( m_nMainGraph == i+1)
      graphBig = &m_ctlMainGraph;

    int nRadMeaning, nComboX, nComboY;
    switch( i) {
      case 1:
        nRadMeaning = m_nRadMeaningGraph2;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_Y))->GetCurSel(); break;
      case 2:
        nRadMeaning = m_nRadMeaningGraph3;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_Y))->GetCurSel(); break;
      case 3:
        nRadMeaning = m_nRadMeaningGraph4;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_Y))->GetCurSel(); break;
      case 4:
        nRadMeaning = m_nRadMeaningGraph5;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_Y))->GetCurSel(); break;
      case 5:
        nRadMeaning = m_nRadMeaningGraph6;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_Y))->GetCurSel(); break;
      case 6:
        nRadMeaning = m_nRadMeaningGraph7;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_Y))->GetCurSel(); break;
      case 7:
        nRadMeaning = m_nRadMeaningGraph8;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_Y))->GetCurSel(); break;
      default:
        nRadMeaning = m_nRadMeaningGraph1;
        nComboX = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_X))->GetCurSel();
        nComboY = ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_Y))->GetCurSel(); break;
    }

    CDispParam *dp;
    switch( nComboY) {
      case 1:  dp = &doc->m_dpI1;   break;   //I1, Разрядный ток 1, [mA]
      case 2:  dp = &doc->m_dpI2;   break;   //I2, Разрядный ток 2, [mA]
      case 3:  dp = &doc->m_dpVpc;  break;   //Vrpc, Напряжение на пьезокрр., [V]
      case 4:  dp = &doc->m_dpAAa;  break;   //Ampl_alt, Амплитуда колебаний (altera), ["]
      case 5:  dp = &doc->m_dpAAd;  break;   //Ampl_dus, Амплитуда колебаний (ДУС), [V]
      case 6:  dp = &doc->m_dpAAr;  break;   //RULA, Задатчик амплитуды, []
      case 7:  dp = &doc->m_dpT1;   break;   //T1, Термодатчик 1, [°C]
      case 8:  dp = &doc->m_dpT2;   break;   //T2, Термодатчик 2, [°C]
      case 9:  dp = &doc->m_dpT3;   break;   //T3, Термодатчик 3, [°C]
      case 10: dp = &doc->m_dpT1;   break;   //dT12, Разница T1 T2, [°C]
      case 11: dp = &doc->m_dpT1;   break;   //dT13, Разница T1 T3, [°C]
      case 12: dp = &doc->m_dpT2;   break;   //dT23, Разница T2 T3, [°C]
      case 13: dp = &doc->m_dpTsa;  break;   //dTsa, Время такта, [msec]
      case 14: dp = &doc->m_dpTsa;  break;   //dTsa, Время такта, [mcsec]
      case 15: dp = &doc->m_dpTsa;  break;   //dTsa, Время такта, [Hz]
      case 16: dp = &doc->m_dpDecCoeff;   break;   //dc, Коэффициент вычета (''/В)

      default:  dp = &doc->m_dpW;    break;   //w, Угловая скорость, ["/sec]


    }

    double *y = dp->GetY( nRadMeaning);
    double *x = dp->GetX( nRadMeaning);
    long llen = dp->GetDataLen( nRadMeaning);
    
    switch( nComboX) {
      case 1:
        x = doc->m_dpT1.GetY( nRadMeaning);
        if( doc->m_dpT1.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT1.GetDataLen( nRadMeaning);
      break;

      case 2:
        x = doc->m_dpT2.GetY( nRadMeaning);
        if( doc->m_dpT2.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT2.GetDataLen( nRadMeaning);
      break;

      case 3:
        x = doc->m_dpT3.GetY( nRadMeaning);
        if( doc->m_dpT3.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT3.GetDataLen( nRadMeaning);
      break;

      case 4:
        x = doc->m_dpT1.GetY( nRadMeaning);
        if( doc->m_dpT1.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT1.GetDataLen( nRadMeaning);
        if( doc->m_dpT2.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT2.GetDataLen( nRadMeaning);
      break;

      case 5:
        x = doc->m_dpT1.GetY( nRadMeaning);
        if( doc->m_dpT1.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT1.GetDataLen( nRadMeaning);
        if( doc->m_dpT3.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT3.GetDataLen( nRadMeaning);
      break;

      case 6:
        x = doc->m_dpT2.GetY( nRadMeaning);
        if( doc->m_dpT2.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT2.GetDataLen( nRadMeaning);
        if( doc->m_dpT3.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT3.GetDataLen( nRadMeaning);
      break;
    }
    
    if( nComboY == 10) {
      if( doc->m_dpT2.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT2.GetDataLen( nRadMeaning);
    }
    if( nComboY == 11) {
      if( doc->m_dpT3.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT3.GetDataLen( nRadMeaning);
    }
    if( nComboY == 12) {
      if( doc->m_dpT3.GetDataLen( nRadMeaning) < llen)
          llen = doc->m_dpT3.GetDataLen( nRadMeaning);
    }

    if( llen > 0) {
      CNiReal64Matrix line( 2, llen, 1.0);
      for( int l = 0; l < llen; l++) {
        switch( nComboY) {
          case 10:  //T1 - T2
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l] - doc->m_dpT2.GetY( nRadMeaning)[ l];
          break;


          case 11:  //T1 - T3
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l] - doc->m_dpT3.GetY( nRadMeaning)[ l];
          break;


          case 12:  //T2 - T3
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l] - doc->m_dpT3.GetY( nRadMeaning)[ l];
          break;


          case 13:  //Tsa, msec
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l] * 1000.;
          break;


          case 14:  //Tsa, mcSec
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l] * 1000000.;
          break;


          case 15:  //Tsa, Hz
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = 1. / y[l];
          break;


          default:
            
            if( nComboX > 3) {
              switch( nComboX) {
                case 4: line.operator()( 0, l) = x[l] - doc->m_dpT2.GetY( nRadMeaning)[l]; break;
                case 5: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
                case 6: line.operator()( 0, l) = x[l] - doc->m_dpT3.GetY( nRadMeaning)[l]; break;
              }
            }
            else
              line.operator()( 0, l) = x[l];

            line.operator()( 1, l) = y[l];
        }
      }

      if( graphSmall != NULL) {
        graphSmall->GetPlots().Item( 1).SetLineColor( theApp.GetSettings()->GetGraphSettings( i)->GetLineColor());
        graphSmall->PlotXY( line, true);
      }
      if( graphBig != NULL) {
        graphBig->GetPlots().Item( 1).SetLineColor( theApp.GetSettings()->GetGraphSettings( i)->GetLineColor());
        graphBig->PlotXY( line, true);
      }

      CWnd *st_min = NULL;
      CWnd *st_avg = NULL;
      CWnd *st_max = NULL;
      CWnd *st_rms = NULL;
      switch( i) {
        case 1:
          st_min = GetDlgItem( IDC_GRAPH2_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH2_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH2_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH2_RMSVAL_LABEL);
        break;

        case 2:
          st_min = GetDlgItem( IDC_GRAPH3_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH3_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH3_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH3_RMSVAL_LABEL);
        break;

        case 3:
          st_min = GetDlgItem( IDC_GRAPH4_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH4_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH4_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH4_RMSVAL_LABEL);
        break;

        case 4:
          st_min = GetDlgItem( IDC_GRAPH5_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH5_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH5_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH5_RMSVAL_LABEL);
        break;

        case 5:
          st_min = GetDlgItem( IDC_GRAPH6_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH6_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH6_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH6_RMSVAL_LABEL);
        break;

        case 6:
          st_min = GetDlgItem( IDC_GRAPH7_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH7_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH7_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH7_RMSVAL_LABEL);
        break;

        case 7:
          st_min = GetDlgItem( IDC_GRAPH8_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH8_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH8_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH8_RMSVAL_LABEL);
        break;

        default:
          st_min = GetDlgItem( IDC_GRAPH1_MINVAL_LABEL);  st_max = GetDlgItem( IDC_GRAPH1_MAXVAL_LABEL);
          st_avg = GetDlgItem( IDC_GRAPH1_MEANVAL_LABEL); st_rms = GetDlgItem( IDC_GRAPH1_RMSVAL_LABEL);
      }

      CString tmp;
      tmp.Format( _T("%.05f"), dp->GetMin( nRadMeaning)); st_min->SetWindowText( tmp);
      tmp.Format( _T("%.05f"), dp->GetMax( nRadMeaning)); st_max->SetWindowText( tmp);
      tmp.Format( _T("%.05f"), dp->GetAvg( nRadMeaning)); st_avg->SetWindowText( tmp);
      tmp.Format( _T("%.05f"), dp->GetRms( nRadMeaning)); st_rms->SetWindowText( tmp);

      if( m_nMainGraph == i+1) {
        
        double dblMin = 1.e99, dblMax = -1.e99, dblAvg = 0., dblRms = 0.;

        if( ( nComboY >= 10 && nComboY <= 15)) {
          if( doc->m_bShowCursors) {
            //это вычисляемый параметр, и есть курсоры
            
	          double dblCurStartX = m_ctlMainGraph.GetAxes().Item( "XAxis").GetMinimum();
	          double dblCurEndX   = m_ctlMainGraph.GetAxes().Item( "XAxis").GetMaximum();

            //double dblCurStartX = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetXPosition();
	          //double dblCurEndX   = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetXPosition();
            
            if( dblCurStartX > dblCurEndX) {
              double tmp = dblCurStartX;
              dblCurStartX = dblCurEndX;
              dblCurEndX = tmp;
            }

            long lEnd = dp->GetDataLen( nRadMeaning);
            double dblSumm = 0.; double dblCalc = 0.;
            for( long l=0; l<lEnd; l++) {
              if( dp->GetX( nRadMeaning)[l] > dblCurStartX) {
                if( dp->GetX( nRadMeaning)[l] > dblCurEndX) break;
                double dt = dp->GetY( nRadMeaning)[l];
                switch( nComboY) {
                  case 10: dt = dt - doc->m_dpT2.GetY( nRadMeaning)[l]; break; //dT12
                  case 11: dt = dt - doc->m_dpT3.GetY( nRadMeaning)[l]; break; //dT13
                  case 12: dt = dt - doc->m_dpT3.GetY( nRadMeaning)[l]; break; //dT23
                  case 13: dt = dt * 1000.; break;  //msec
                  case 14: dt = dt * 1e6; break;   //mcsec
                  case 15: dt = 1. / dt; break;     //Hz
                }
                if( dblMin > dt) dblMin = dt;
                if( dblMax < dt) dblMax = dt;
                dblSumm += dt; dblCalc += 1.;
              }
            }
            dblAvg = dblSumm / dblCalc;
            
            dblSumm = 0.;
            for( l=0; l<lEnd; l++) {
              if( dp->GetX( nRadMeaning)[l] > dblCurStartX) {
                if( dp->GetX( nRadMeaning)[l] > dblCurEndX) break;
                dblSumm += ( dp->GetY( nRadMeaning)[ l] - dblAvg) * ( dp->GetY( nRadMeaning)[ l] - dblAvg);
              }
            }
            dblRms = sqrt( dblSumm / lEnd);
            
            tmp.Format( _T("%.05f"), dblMin); GetDlgItem( IDC_MINVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblMax); GetDlgItem( IDC_MAXVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblAvg); GetDlgItem( IDC_MEANVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblRms); GetDlgItem( IDC_RMSVAL_LABEL)->SetWindowText( tmp);
          }
          else {
            //это вычисляемый параметр, и курсоров нет
            long lEnd = dp->GetDataLen( nRadMeaning);
            double dblSumm = 0.;
            for( long l=0; l<lEnd; l++) {
              double dt = dp->GetY( nRadMeaning)[l];
              switch( nComboY) {
                case 10: dt = dt - doc->m_dpT2.GetY( nRadMeaning)[l]; break; //dT12
                case 11: dt = dt - doc->m_dpT3.GetY( nRadMeaning)[l]; break; //dT13
                case 12: dt = dt - doc->m_dpT3.GetY( nRadMeaning)[l]; break; //dT23
                case 13: dt = dt * 1000.; break;  //msec
                case 14: dt = dt * 1e6; break;   //mcsec
                case 15: dt = 1. / dt; break;     //Hz
              }
              if( dblMin > dt) dblMin = dt;
              if( dblMax < dt) dblMax = dt;
              dblSumm += dt;
            }
            dblAvg = dblSumm / lEnd;
            
            dblSumm = 0.;
            for( l=0; l<lEnd; l++) {
              dblSumm += ( dp->GetY( nRadMeaning)[ l] - dblAvg) * ( dp->GetY( nRadMeaning)[ l] - dblAvg);
            }
            dblRms = sqrt( dblSumm / lEnd);

            tmp.Format( _T("%.05f"), dblMin); GetDlgItem( IDC_MINVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblMax); GetDlgItem( IDC_MAXVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblAvg); GetDlgItem( IDC_MEANVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblRms); GetDlgItem( IDC_RMSVAL_LABEL)->SetWindowText( tmp);
          }
        }
        else {
          if( doc->m_bShowCursors) {
            //это прямой параметр и есть курсоры
            double dblCurStartX = m_ctlMainGraph.GetAxes().Item( "XAxis").GetMinimum();
	          double dblCurEndX   = m_ctlMainGraph.GetAxes().Item( "XAxis").GetMaximum();
            //double dblCurStartX = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetXPosition();
	          //double dblCurEndX = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetXPosition();

            if( dblCurStartX > dblCurEndX) {
              double tmp = dblCurStartX;
              dblCurStartX = dblCurEndX;
              dblCurEndX = tmp;
            }

            long lEnd = dp->GetDataLen( nRadMeaning);
            double dblSumm = 0.;
            double dblCalc = 0;
            for( long l=0; l<lEnd; l++) {
              if( dp->GetX( nRadMeaning)[l] > dblCurStartX) {
                if( dp->GetX( nRadMeaning)[l] > dblCurEndX) break;
                double dt = dp->GetY( nRadMeaning)[l];
                if( dblMin > dt) dblMin = dt;
                if( dblMax < dt) dblMax = dt;
                dblSumm += dt; dblCalc += 1.;
              }
            }
            dblAvg = dblSumm / dblCalc;
            
            dblSumm = 0.;
            for( l=0; l<lEnd; l++) {
              if( dp->GetX( nRadMeaning)[l] > dblCurStartX) {
                if( dp->GetX( nRadMeaning)[l] > dblCurEndX) break;
                dblSumm += ( dp->GetY( nRadMeaning)[ l] - dblAvg) * ( dp->GetY( nRadMeaning)[ l] - dblAvg);
              }
            }
            dblRms = sqrt( dblSumm / lEnd);

            tmp.Format( _T("%.05f"), dblMin); GetDlgItem( IDC_MINVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblMax); GetDlgItem( IDC_MAXVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblAvg); GetDlgItem( IDC_MEANVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dblRms); GetDlgItem( IDC_RMSVAL_LABEL)->SetWindowText( tmp);
          }
          else {
            //это прямой параметр и курсоров нет - что на маленькой статистике, то и вправо
            tmp.Format( _T("%.05f"), dp->GetMin( nRadMeaning)); GetDlgItem( IDC_MINVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dp->GetMax( nRadMeaning)); GetDlgItem( IDC_MAXVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dp->GetAvg( nRadMeaning)); GetDlgItem( IDC_MEANVAL_LABEL)->SetWindowText( tmp);
            tmp.Format( _T("%.05f"), dp->GetRms( nRadMeaning)); GetDlgItem( IDC_RMSVAL_LABEL)->SetWindowText( tmp);
          }
        }
      }
    }
    else {
      if( graphSmall != NULL) graphSmall->ClearData();
      if( graphBig != NULL)   graphBig->ClearData();
    }
  }

	m_strSoftwareVersion = ( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer;
	EndWaitCursor();

	UpdateData( FALSE);
}

void CMainView::OnInitialUpdate() 
{
  m_dlgGraphSetup = NULL;
  m_dKimpSec = theApp.GetSettings()->GetScaleCoeff();

  m_nMainGraph = 1;
  CFormView::OnInitialUpdate();

  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(0)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(1)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(2)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(3)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(4)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(5)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(6)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(7)->GetAxisY());

  m_nRadMeaningGraph1 = theApp.GetSettings()->GetGraphSettings(0)->GetMeaning();
  m_nRadMeaningGraph2 = theApp.GetSettings()->GetGraphSettings(1)->GetMeaning();
  m_nRadMeaningGraph3 = theApp.GetSettings()->GetGraphSettings(2)->GetMeaning();
  m_nRadMeaningGraph4 = theApp.GetSettings()->GetGraphSettings(3)->GetMeaning();
  m_nRadMeaningGraph5 = theApp.GetSettings()->GetGraphSettings(4)->GetMeaning();
  m_nRadMeaningGraph6 = theApp.GetSettings()->GetGraphSettings(5)->GetMeaning();
  m_nRadMeaningGraph7 = theApp.GetSettings()->GetGraphSettings(6)->GetMeaning();
  m_nRadMeaningGraph8 = theApp.GetSettings()->GetGraphSettings(7)->GetMeaning();

  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(0)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(1)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(2)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(3)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(4)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(5)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(6)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(7)->GetAxisX());


  GetDlgItem( IDC_MIN_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MINVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MEAN_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MEANVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MAX_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MAXVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_RMS_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_RMSVAL_LABEL)->SetFont( &m_pFont);

  m_strThisSoftwareVersion.LoadString( IDS_SOFTWARE_VERSION);
  UpdateData( FALSE);

  m_ctlMainGraph.GetCursors().Item( "CursorStart").SetVisible( ((CSlg2Doc *) GetDocument())->m_bShowCursors);
  m_ctlMainGraph.GetCursors().Item( "CursorEnd").SetVisible( ((CSlg2Doc *) GetDocument())->m_bShowCursors);

  GetDlgItem( IDC_IC_RED_CURSOR)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_LBL_CURSOR1_X_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_LBL_CURSOR1_Y_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);

  GetDlgItem( IDC_IC_BLUE_CURSOR)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_LBL_CURSOR2_X_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_LBL_CURSOR2_Y_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);

  GetDlgItem( IDC_LBL_CURSOR_DX_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_LBL_CURSOR_DY_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
  GetDlgItem( IDC_CURSOR_FRAME)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);


  /*
	//блокирование недостуаных для просмотра времен осреднения (для них недостаточно статистики)
	if( ( ( CSlg2Doc *) GetDocument())->m_dn10s < 1.) {
		m_nTMeaningInd = 0;
		GetDlgItem( IDC_RAD_MEANING3)->EnableWindow( false);
	}
	else {
		GetDlgItem( IDC_RAD_MEANING3)->EnableWindow( true);
	}

	if( ( ( CSlg2Doc *) GetDocument())->m_dn100s < 1.) {
		m_nTMeaningInd = 0;
		GetDlgItem( IDC_RAD_MEANING4)->EnableWindow( false);
	}
	else {
		GetDlgItem( IDC_RAD_MEANING4)->EnableWindow( true);
	}
  */

  SetTimer( MY_MAXIMIZE_VIEW_TIMER, 100, NULL);
  SetTimer( MY_TIMER_1000, 1000, NULL);
}

void CMainView::OnTimer(UINT nIDEvent) 
{
  CString strError;
  UpdateData( TRUE);

  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  //событие 1000 мсек
  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  if( nIDEvent == MY_TIMER_1000) {

    //////////////////////////////////////////////////////////////////////
    // Обновляем управленческие параметры
    //////////////////////////////////////////////////////////////////////
    /*
    m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											//код такта подставки
    m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//коэффициент M
    m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		//начальная мода
    m_strParam5Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
    m_strParam6Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
    m_strParam7Val.Format( _T("%.2f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.0));
    m_strParam8Val.Format( _T("%.5f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);
    */

    //////////////////////////////////////////////////////////////////////
    // Обновляем графики
    //////////////////////////////////////////////////////////////////////
    //RefreshGraphs();
    //Beep( 100, 10);
  }

  if( nIDEvent == MY_MAXIMIZE_VIEW_TIMER) {
    KillTimer( MY_MAXIMIZE_VIEW_TIMER);
    (( CSlg2App *) AfxGetApp())->GetMainWnd()->ShowWindow( SW_MAXIMIZE);
  }

  if( nIDEvent == MY_REFRESH_VIEW_TIMER) {
    KillTimer( MY_REFRESH_VIEW_TIMER);

    /*
    m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											//код такта подставки
    m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//коэффициент M
    m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		//начальная мода
    m_strParam5Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
    m_strParam6Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
    m_strParam7Val.Format( _T("%.2f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.0));
    m_strParam8Val.Format( _T("%.5f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);
    */

    RefreshGraphs();
  }

  CFormView::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CMainView, CFormView)
  //{{AFX_EVENTSINK_MAP(CMainView)
	ON_EVENT(CMainView, IDC_GRAPH1, -600 /* Click */, OnClickGraph1, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH2, -600 /* Click */, OnClickGraph2, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH3, -600 /* Click */, OnClickGraph3, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH4, -600 /* Click */, OnClickGraph4, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH5, -600 /* Click */, OnClickGraph5, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH6, -600 /* Click */, OnClickGraph6, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH7, -600 /* Click */, OnClickGraph7, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH8, -600 /* Click */, OnClickGraph8, VTS_NONE)
	ON_EVENT(CMainView, IDC_MAIN_GRAPH, 1 /* CursorChange */, OnCursorChangeMainGraph, VTS_PI4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CMainView, IDC_GRAPH1, -607 /* MouseUp */, OnMouseUpGraph1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH2, -607 /* MouseUp */, OnMouseUpGraph2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH3, -607 /* MouseUp */, OnMouseUpGraph3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH4, -607 /* MouseUp */, OnMouseUpGraph4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH5, -607 /* MouseUp */, OnMouseUpGraph5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH6, -607 /* MouseUp */, OnMouseUpGraph6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH7, -607 /* MouseUp */, OnMouseUpGraph7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH8, -607 /* MouseUp */, OnMouseUpGraph8, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMainView::OnClickGraph1() 
{
	//UpdateData( TRUE);
	//m_nMainGraph = 1;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("''/sec"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph2() 
{
	//m_nMainGraph = 2;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph3() 
{
	//m_nMainGraph = 3;	
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph4() 
{
	//m_nMainGraph = 4;	
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("V"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph5() 
{
	//m_nMainGraph = 5;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("V"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph6() 
{
	//m_nMainGraph = 6;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("°C"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph7() 
{
	//m_nMainGraph = 7;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("°C"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnClickGraph8() 
{
	//m_nMainGraph = 8;
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек"));
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	//m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	//((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	//SetRefreshTimer( 100);
}

void CMainView::OnDestroy() 
{
	UpdateData( true);
	//((CSlg2App *) AfxGetApp())->m_dKimpSec = m_dKimpSec;
  theApp.GetSettings()->SetScaleCoeff( m_dKimpSec);

  theApp.GetSettings()->GetGraphSettings(0)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(1)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(2)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(3)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(4)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(5)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(6)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(7)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_Y))->GetCurSel());

  theApp.GetSettings()->GetGraphSettings(0)->SetMeaning( m_nRadMeaningGraph1);
  theApp.GetSettings()->GetGraphSettings(1)->SetMeaning( m_nRadMeaningGraph2);
  theApp.GetSettings()->GetGraphSettings(2)->SetMeaning( m_nRadMeaningGraph3);
  theApp.GetSettings()->GetGraphSettings(3)->SetMeaning( m_nRadMeaningGraph4);
  theApp.GetSettings()->GetGraphSettings(4)->SetMeaning( m_nRadMeaningGraph5);
  theApp.GetSettings()->GetGraphSettings(5)->SetMeaning( m_nRadMeaningGraph6);
  theApp.GetSettings()->GetGraphSettings(6)->SetMeaning( m_nRadMeaningGraph7);
  theApp.GetSettings()->GetGraphSettings(7)->SetMeaning( m_nRadMeaningGraph8);

  theApp.GetSettings()->GetGraphSettings(0)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(1)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(2)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(3)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(4)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(5)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(6)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(7)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_X))->GetCurSel());

	CFormView::OnDestroy();
	VERIFY( m_pFont.DeleteObject());
}

void CMainView::OnKillfocusEdtKImpSec() 
{
	UpdateData( true);
	if( (( CSlg2Doc *) GetDocument())->m_strCurrentFileName.IsEmpty()) return;
  theApp.GetSettings()->SetScaleCoeff( m_dKimpSec);
	//(( CSlg2Doc *) GetDocument())->ReadDataFile( (( CSlg2Doc *) GetDocument())->m_strCurrentFileName, NULL);
	(( CSlg2Doc *) GetDocument())->RecalculateStatistics();
}

void CMainView::OnHistogram() 
{
	CDlgHisto dlg;
	dlg.m_doc = ((CSlg2Doc *) GetDocument());
	dlg.DoModal();
}

void CMainView::OnAlanCurve() 
{
	/*CDlgAlanCurve dlg;
	dlg.m_nDataFileLines = ((CSlg2Doc *) GetDocument())->m_nDataFileLines;
	dlg.m_strFileName = ((CSlg2Doc *) GetDocument())->m_strCurrentFileName;
	dlg.DoModal();*/
}

void CMainView::OnBtnExport() 
{
	CDlgExport dlg;
	dlg.m_nDataFileLines = ((CSlg2Doc *) GetDocument())->m_nDataFileLines;
	dlg.m_strFileName = ((CSlg2Doc *) GetDocument())->m_strCurrentFileName;
	dlg.m_nStatFileVersion = ((CSlg2Doc *) GetDocument())->m_nStatFileVersion;
	dlg.DoModal();
}

void CMainView::SetRefreshTimer( int nPeriod)
{
  SetTimer( MY_REFRESH_VIEW_TIMER, nPeriod, NULL);
}

void CMainView::OnRadMeaning100msec() 
{
  GetDlgItem( IDC_RAD_G1_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T1)->SendMessage( WM_LBUTTONUP);
	SetRefreshTimer();
}

void CMainView::OnRadMeaning1sec() 
{
  GetDlgItem( IDC_RAD_G1_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T2)->SendMessage( WM_LBUTTONUP);
	SetRefreshTimer();
}

void CMainView::OnRadMeaning10sec() 
{
  GetDlgItem( IDC_RAD_G1_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T3)->SendMessage( WM_LBUTTONUP);
	SetRefreshTimer();
}

void CMainView::OnRadMeaning100sec() 
{
  GetDlgItem( IDC_RAD_G1_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T4)->SendMessage( WM_LBUTTONUP);

	SetRefreshTimer();
}

void CMainView::OnRadMeaning5() 
{
  //NO MEANING
  GetDlgItem( IDC_RAD_G1_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T0)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T0)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T0)->SendMessage( WM_LBUTTONUP);

	//SetRefreshTimer();
}

void CMainView::OnUpdateZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !( ( CSlg2Doc *)GetDocument())->m_strCurrentFileName.IsEmpty()
									&&
									( ( CSlg2Doc *)GetDocument())->m_bShowCursors);
}

void CMainView::OnUpdateRestoreZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( !( ( CSlg2Doc *)GetDocument())->m_strCurrentFileName.IsEmpty()
									&&
									( ( CSlg2Doc *)GetDocument())->m_bZoomed);
}

void CMainView::OnZoomin() 
{
	//m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("''/sec"));
	double CurStartX = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetXPosition();
	double CurStartY = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetYPosition();
	double CurEndX = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetXPosition();
	double CurEndY = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetYPosition();

	double swap;
	if( CurStartX > CurEndX) {
		swap = CurStartX;
		CurStartX = CurEndX;
		CurEndX = swap;
	}

	if( CurStartY > CurEndY) {
		swap = CurStartY;
		CurStartY = CurEndY;
		CurEndY = swap;
	}

	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetMinimum( CurStartY);
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetMaximum( CurEndY);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetMinimum( CurStartX);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetMaximum( CurEndX);
	
	((CSlg2Doc *) GetDocument())->m_bZoomed = true;
	SetRefreshTimer();
}

void CMainView::OnRestoreZoom() 
{
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer();
}

void CMainView::OnShowCursors() 
{
	((CSlg2Doc *) GetDocument())->m_bShowCursors ^= 1;
	m_ctlMainGraph.GetCursors().Item( "CursorStart").SetVisible( ((CSlg2Doc *) GetDocument())->m_bShowCursors);
	m_ctlMainGraph.GetCursors().Item( "CursorEnd").SetVisible( ((CSlg2Doc *) GetDocument())->m_bShowCursors);

	GetDlgItem( IDC_IC_RED_CURSOR)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_LBL_CURSOR1_X_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_LBL_CURSOR1_Y_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);

	GetDlgItem( IDC_IC_BLUE_CURSOR)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_LBL_CURSOR2_X_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_LBL_CURSOR2_Y_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);

	GetDlgItem( IDC_LBL_CURSOR_DX_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_LBL_CURSOR_DY_VAL)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	GetDlgItem( IDC_CURSOR_FRAME)->ShowWindow( ((CSlg2Doc *) GetDocument())->m_bShowCursors ? SW_SHOW : SW_HIDE);
	
}

void CMainView::OnCursorChangeMainGraph(long FAR* CursorIndex, VARIANT FAR* XPos, VARIANT FAR* YPos, BOOL FAR* bTracking) 
{
	double CurStartX = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetXPosition();
	double CurStartY = m_ctlMainGraph.GetCursors().Item( "CursorStart").GetYPosition();
	double CurEndX = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetXPosition();
	double CurEndY = m_ctlMainGraph.GetCursors().Item( "CursorEnd").GetYPosition();

	m_strLblCursorDxVal.Format( _T("%.3f"), fabs( CurStartX - CurEndX));
	m_strLblCursorDyVal.Format( _T("%.3f"), fabs( CurStartY - CurEndY));

	m_strCur1X_val.Format( _T("%.3f"), CurStartX);
	m_strCur1Y_val.Format( _T("%.3f"), CurStartY);
	m_strCur2X_val.Format( _T("%.3f"), CurEndX);
	m_strCur2Y_val.Format( _T("%.3f"), CurEndY);

	UpdateData( FALSE);
}

//DEL void CMainView::OnRadT1Td1()
//DEL {
//DEL   m_ctlSmallGraph6.SetCaption( _T("Термодатчик 1 [КЛ], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

//DEL void CMainView::OnRadT1Td2()
//DEL {
//DEL   m_ctlSmallGraph6.SetCaption( _T("Термодатчик 2 [КЛ], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

//DEL void CMainView::OnRadT1Td3()
//DEL {
//DEL   m_ctlSmallGraph6.SetCaption( _T("Термодатчик 3 [Корпус], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

//DEL void CMainView::OnRadT2Td1()
//DEL {
//DEL   m_ctlSmallGraph7.SetCaption( _T("Термодатчик 1 [КЛ], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

//DEL void CMainView::OnRadT2Td2()
//DEL {
//DEL   m_ctlSmallGraph7.SetCaption( _T("Термодатчик 2 [КЛ], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

//DEL void CMainView::OnRadT2Td3()
//DEL {
//DEL   m_ctlSmallGraph7.SetCaption( _T("Термодатчик 3 [Корпус], [°C]"));
//DEL   SetRefreshTimer( 100);
//DEL }

void CMainView::OnMouseUpGraph1(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 0);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 1;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph2(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 1);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 2;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph3(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 2);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 3;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph4(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 3);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 4;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph5(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 4);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 5;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph6(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 5);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 6;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph7(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 6);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 7;
    SetRefreshTimer( 100);
  }
}

void CMainView::OnMouseUpGraph8(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 7);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
  else if( Button == MK_LBUTTON) {
    m_nMainGraph = 8;
    SetRefreshTimer( 100);
  }
}


void CMainView::OnRadMeaningGraph() 
{
  SetRefreshTimer( 100);
}

void CMainView::OnCloseupCmbGraph() 
{
  SetRefreshTimer( 100);
}
