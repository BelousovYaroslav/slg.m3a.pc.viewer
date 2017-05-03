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
	m_strGraphMaxVal = _T("");
	m_strGraphMeanVal = _T("");
	m_strGraphMinVal = _T("");
	m_strGraphRmsVal = _T("");
	m_strSmGr1_max = _T("");
	m_strSmGr1_mean = _T("");
	m_strSmGr1_min = _T("");
	m_strSmGr1_rms = _T("");
	m_strSmGr2_max = _T("");
	m_strSmGr2_mean = _T("");
	m_strSmGr2_min = _T("");
	m_strSmGr2_rms = _T("");
	m_strSmGr3_max = _T("");
	m_strSmGr3_mean = _T("");
	m_strSmGr3_min = _T("");
	m_strSmGr3_rms = _T("");
	m_strSmGr4_max = _T("");
	m_strSmGr4_mean = _T("");
	m_strSmGr4_min = _T("");
	m_strSmGr4_rms = _T("");
	m_strSmGr5_max = _T("");
	m_strSmGr5_mean = _T("");
	m_strSmGr5_min = _T("");
	m_strSmGr5_rms = _T("");
	m_strSmGr6_max = _T("");
	m_strSmGr6_mean = _T("");
	m_strSmGr6_min = _T("");
	m_strSmGr6_rms = _T("");
	m_strSmGr7_max = _T("");
	m_strSmGr7_mean = _T("");
	m_strSmGr7_min = _T("");
	m_strSmGr7_rms = _T("");
	m_strSmGr8_max = _T("");
	m_strSmGr8_mean = _T("");
	m_strSmGr8_min = _T("");
	m_strSmGr8_rms = _T("");
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
	m_nCounterSkippedPoints = 0;
	b1stPointSkipped = false;	
	m_bEmergencyCodeApperance = false;
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));									// Zero out the structure.
	lf.lfHeight = 20;																	// Request a 12-pixel-height font.
	lstrcpy(lf.lfFaceName, _T("Times New Roman"));   // Request a face name "Arial".
	VERIFY(m_pFont.CreateFontIndirect(&lf));							// Create the font.
}

CMainView::~CMainView()
{
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
	DDX_Text(pDX, IDC_MAXVAL_LABEL, m_strGraphMaxVal);
	DDX_Text(pDX, IDC_MEANVAL_LABEL, m_strGraphMeanVal);
	DDX_Text(pDX, IDC_MINVAL_LABEL, m_strGraphMinVal);
	DDX_Text(pDX, IDC_RMSVAL_LABEL, m_strGraphRmsVal);
	DDX_Text(pDX, IDC_GRAPH1_MAXVAL_LABEL, m_strSmGr1_max);
	DDX_Text(pDX, IDC_GRAPH1_MEANVAL_LABEL, m_strSmGr1_mean);
	DDX_Text(pDX, IDC_GRAPH1_MINVAL_LABEL, m_strSmGr1_min);
	DDX_Text(pDX, IDC_GRAPH1_RMSVAL_LABEL, m_strSmGr1_rms);
	DDX_Text(pDX, IDC_GRAPH2_MAXVAL_LABEL, m_strSmGr2_max);
	DDX_Text(pDX, IDC_GRAPH2_MEANVAL_LABEL, m_strSmGr2_mean);
	DDX_Text(pDX, IDC_GRAPH2_MINVAL_LABEL, m_strSmGr2_min);
	DDX_Text(pDX, IDC_GRAPH2_RMSVAL_LABEL, m_strSmGr2_rms);
	DDX_Text(pDX, IDC_GRAPH3_MAXVAL_LABEL, m_strSmGr3_max);
	DDX_Text(pDX, IDC_GRAPH3_MEANVAL_LABEL, m_strSmGr3_mean);
	DDX_Text(pDX, IDC_GRAPH3_MINVAL_LABEL, m_strSmGr3_min);
	DDX_Text(pDX, IDC_GRAPH3_RMSVAL_LABEL, m_strSmGr3_rms);
	DDX_Text(pDX, IDC_GRAPH4_MAXVAL_LABEL, m_strSmGr4_max);
	DDX_Text(pDX, IDC_GRAPH4_MEANVAL_LABEL, m_strSmGr4_mean);
	DDX_Text(pDX, IDC_GRAPH4_MINVAL_LABEL, m_strSmGr4_min);
	DDX_Text(pDX, IDC_GRAPH4_RMSVAL_LABEL, m_strSmGr4_rms);
	DDX_Text(pDX, IDC_GRAPH5_MAXVAL_LABEL, m_strSmGr5_max);
	DDX_Text(pDX, IDC_GRAPH5_MEANVAL_LABEL, m_strSmGr5_mean);
	DDX_Text(pDX, IDC_GRAPH5_MINVAL_LABEL, m_strSmGr5_min);
	DDX_Text(pDX, IDC_GRAPH5_RMSVAL_LABEL, m_strSmGr5_rms);
	DDX_Text(pDX, IDC_GRAPH6_MAXVAL_LABEL, m_strSmGr6_max);
	DDX_Text(pDX, IDC_GRAPH6_MEANVAL_LABEL, m_strSmGr6_mean);
	DDX_Text(pDX, IDC_GRAPH6_MINVAL_LABEL, m_strSmGr6_min);
	DDX_Text(pDX, IDC_GRAPH6_RMSVAL_LABEL, m_strSmGr6_rms);
	DDX_Text(pDX, IDC_GRAPH7_MAXVAL_LABEL, m_strSmGr7_max);
	DDX_Text(pDX, IDC_GRAPH7_MEANVAL_LABEL, m_strSmGr7_mean);
	DDX_Text(pDX, IDC_GRAPH7_MINVAL_LABEL, m_strSmGr7_min);
	DDX_Text(pDX, IDC_GRAPH7_RMSVAL_LABEL, m_strSmGr7_rms);
	DDX_Text(pDX, IDC_GRAPH8_MAXVAL_LABEL, m_strSmGr8_max);
	DDX_Text(pDX, IDC_GRAPH8_MEANVAL_LABEL, m_strSmGr8_mean);
	DDX_Text(pDX, IDC_GRAPH8_MINVAL_LABEL, m_strSmGr8_min);
	DDX_Text(pDX, IDC_GRAPH8_RMSVAL_LABEL, m_strSmGr8_rms);
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
	ON_BN_CLICKED(IDC_RAD_T1_TD1, OnRadT1Td1)
	ON_BN_CLICKED(IDC_RAD_T1_TD2, OnRadT1Td2)
	ON_BN_CLICKED(IDC_RAD_T1_TD3, OnRadT1Td3)
	ON_BN_CLICKED(IDC_RAD_T2_TD1, OnRadT2Td1)
	ON_BN_CLICKED(IDC_RAD_T2_TD2, OnRadT2Td2)
	ON_BN_CLICKED(IDC_RAD_T2_TD3, OnRadT2Td3)
	ON_BN_CLICKED(IDC_RAD_MEANING5, OnRadMeaning5)
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
	CSlg2Doc *doc = (CSlg2Doc *) GetDocument();
	if( doc->m_dn100m == 0.) return;
	if( m_nTMeaningInd == 1 && doc->m_dn1s == 0.) return;
	if( m_nTMeaningInd == 2 && doc->m_dn10s == 0.) return;
	if( m_nTMeaningInd == 3 && doc->m_dn100s == 0.) return;

	//100-���� �����
	CNiReal64Matrix line_w100m( 2, doc->m_dn100m, 1.0);
	for( int i = 0; i < doc->m_dn100m; i++) {
		line_w100m( 0, i) = doc->m_dx100m[i];
		line_w100m( 1, i) = doc->m_dy100m[i];
	}
	
	//1-��������� �����
	CNiReal64Matrix line_w1s( 2, doc->m_dn1s, 1.0);
	for( i = 0; i < doc->m_dn1s; i++) {
		line_w1s( 0, i) = doc->m_dx1s[i];
		line_w1s( 1, i) = doc->m_dy1s[i];
	}

	//10-��������� �����
	CNiReal64Matrix line_w10s( 2, doc->m_dn10s, 1.0);
	for( i = 0; i < doc->m_dn10s; i++) {
		line_w10s( 0, i) = doc->m_dx10s[i];
		line_w10s( 1, i) = doc->m_dy10s[i];
	}

	//100-��� �����
	CNiReal64Matrix line_w100s( 2, doc->m_dn100s, 1.0);
	for( i = 0; i < doc->m_dn100s; i++) {
		line_w100s( 0, i) = doc->m_dx100s[i];
		line_w100s( 1, i) = doc->m_dy100s[i];
	}

	//I1 �����
	CNiReal64Matrix line_i1( 2, doc->m_dn_i1, 1.0);
	for( i = 0; i < doc->m_dn_i1; i++) {
		line_i1( 0, i) = doc->m_dx_i1[i];
		line_i1( 1, i) = doc->m_dy_i1[i];
	}

	//I2 �����
	CNiReal64Matrix line_i2( 2, doc->m_dn_i2, 1.0);
	for( i = 0; i < doc->m_dn_i2; i++) {
		line_i2( 0, i) = doc->m_dx_i2[i];
		line_i2( 1, i) = doc->m_dy_i2[i];
	}

	//Vpc �����
	CNiReal64Matrix line_vpc( 2, doc->m_dn_vpc, 1.0);
	for( i = 0; i < doc->m_dn_vpc; i++) {
		line_vpc( 0, i) = doc->m_dx_vpc[i];
		line_vpc( 1, i) = doc->m_dy_vpc[i];
	}

	//AmplAng �����
	CNiReal64Matrix line_aa( 2, doc->m_dn_aa, 1.0);
	for( i = 0; i < doc->m_dn_aa; i++) {
		line_aa( 0, i) = doc->m_dx_aa[i];
		line_aa( 1, i) = doc->m_dy_aa[i];
	}

	//T1 �����
	CNiReal64Matrix line_t1( 2, doc->m_dn_t1, 1.0);
	for( i = 0; i < doc->m_dn_t1; i++) {
		line_t1( 0, i) = doc->m_dx_t1[i];
		line_t1( 1, i) = doc->m_dy_t1[i];
	}

	//T2 �����
	CNiReal64Matrix line_t2( 2, doc->m_dn_t2, 1.0);
	for( i = 0; i < doc->m_dn_t2; i++) {
		line_t2( 0, i) = doc->m_dx_t2[i];
		line_t2( 1, i) = doc->m_dy_t2[i];
	}

  //T3 �����
	CNiReal64Matrix line_t3( 2, doc->m_dn_t3, 1.0);
	for( i = 0; i < doc->m_dn_t3; i++) {
		line_t3( 0, i) = doc->m_dx_t3[i];
		line_t3( 1, i) = doc->m_dy_t3[i];
	}

	//Tsa �����
	CNiReal64Matrix line_tsa( 2, doc->m_dn_tsa, 1.0);
	for( i = 0; i < doc->m_dn_tsa; i++) {
		line_tsa( 0, i) = doc->m_dx_tsa[i];
		line_tsa( 1, i) = doc->m_dy_tsa[i];
	}

	
	//��������� ���������
	switch( m_nTMeaningInd) {
		case 0: m_ctlSmallGraph1.PlotXY( line_w100m, true); break;
		case 1: m_ctlSmallGraph1.PlotXY( line_w1s, true); break;
		case 2: m_ctlSmallGraph1.PlotXY( line_w10s, true); break;
		case 3: m_ctlSmallGraph1.PlotXY( line_w100s, true); break;
	}

	m_ctlSmallGraph2.PlotXY( line_i1, true);
	m_ctlSmallGraph3.PlotXY( line_i2, true);
	m_ctlSmallGraph4.PlotXY( line_vpc, true);
	m_ctlSmallGraph5.PlotXY( line_aa, true);
	
  m_ctlSmallGraph6.PlotXY( line_t1, true);
  //switch( m_nRadT1) {
  //  case 0: m_ctlSmallGraph6.PlotXY( line_t1, true); break;
  //  case 1: m_ctlSmallGraph6.PlotXY( line_t2, true); break;
  //  case 2: m_ctlSmallGraph6.PlotXY( line_t3, true); break;
  //}
	
  m_ctlSmallGraph7.PlotXY( line_t2, true);
  //switch( m_nRadT2) {
  //  case 0: m_ctlSmallGraph7.PlotXY( line_t1, true); break;
  //  case 1: m_ctlSmallGraph7.PlotXY( line_t2, true); break;
  //  case 2: m_ctlSmallGraph7.PlotXY( line_t3, true); break;
  //}

	m_ctlSmallGraph8.PlotXY( line_tsa, true);

	//doc->RecalculateStatistics();
	m_strSoftwareVersion = ( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer;
	switch( m_nMainGraph) {
		case 1:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));

			if( m_nTMeaningInd == 0) {
				m_ctlMainGraph.PlotXY( line_w100m, true);
				m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_100m_min);
				m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_100m_mean);
				m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_100m_max);
				m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_100m_rms);
			}

			if( m_nTMeaningInd == 1) {
				m_ctlMainGraph.PlotXY( line_w1s, true);
				m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_1s_min);
				m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_1s_mean);
				m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_1s_max);
				m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_1s_rms);
			}
			
			if( m_nTMeaningInd == 2) {
				m_ctlMainGraph.PlotXY( line_w10s, true);
				m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_10s_min);
				m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_10s_mean);
				m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_10s_max);
				m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_10s_rms);
			}

			if( m_nTMeaningInd == 3) {
				m_ctlMainGraph.PlotXY( line_w100s, true);
				m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_100s_min);
				m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_100s_mean);
				m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_100s_max);
				m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_100s_rms);
			}
			
		break;

		case 2:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
			m_ctlMainGraph.PlotXY( line_i1, true);
			m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_i1_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_i1_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_i1_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_i1_rms);
		break;

		case 3:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
			m_ctlMainGraph.PlotXY( line_i2, true);
			m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_i2_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_i2_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_i2_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_i2_rms);
		break;

		case 4:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
			m_ctlMainGraph.PlotXY( line_vpc, true);
			m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_vpc_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_vpc_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_vpc_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_vpc_rms);
		break;

		case 5:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
			m_ctlMainGraph.PlotXY( line_aa, true);
			m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_aa_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_aa_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_aa_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_aa_rms);
		break;

		case 6:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
			
      m_ctlMainGraph.PlotXY( line_t1, true);
      m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t1_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t1_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t1_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t1_rms);

      /*switch( m_nRadT1) {
        case 0:
          m_ctlMainGraph.PlotXY( line_t1, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t1_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t1_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t1_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t1_rms);
        break;
        case 1:
          m_ctlMainGraph.PlotXY( line_t2, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t2_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t2_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t2_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t2_rms);
        break;
        case 2:
          m_ctlMainGraph.PlotXY( line_t3, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t3_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t3_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t3_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t3_rms);
        break;
      }*/
		break;

		case 7:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
			
      m_ctlMainGraph.PlotXY( line_t2, true);
      m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t2_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t2_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t2_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t2_rms);

      /*
      switch( m_nRadT2) {
        case 0:
          m_ctlMainGraph.PlotXY( line_t1, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t1_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t1_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t1_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t1_rms);
        break;
        case 1:
          m_ctlMainGraph.PlotXY( line_t2, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t2_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t2_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t2_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t2_rms);
        break;
        case 2:
          m_ctlMainGraph.PlotXY( line_t3, true);
          m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_t3_min);
			    m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_t3_mean);
			    m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_t3_max);
			    m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_t3_rms);
        break;
      }
      */
		break;

		case 8:
			m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
			m_ctlMainGraph.PlotXY( line_tsa, true);
			m_strGraphMinVal.Format( _T("%.4f"), doc->m_d_tsa_min);
			m_strGraphMeanVal.Format( _T("%.4f"), doc->m_d_tsa_mean);
			m_strGraphMaxVal.Format( _T("%.4f"), doc->m_d_tsa_max);
			m_strGraphRmsVal.Format( _T("%.4f"), doc->m_d_tsa_rms);
		break;

		default:
			m_nMainGraph = 1;
		break;
	}
	
	//���������� �� ������� ���������
	switch( m_nTMeaningInd) {
		case 0:
			m_strSmGr1_min.Format( _T("%.4f"), doc->m_d_100m_min);
			m_strSmGr1_mean.Format( _T("%.4f"), doc->m_d_100m_mean);
			m_strSmGr1_max.Format( _T("%.4f"), doc->m_d_100m_max);
			m_strSmGr1_rms.Format( _T("%.4f"), doc->m_d_100m_rms);
		break;

		case 1:
			m_strSmGr1_min.Format( _T("%.4f"), doc->m_d_1s_min);
			m_strSmGr1_mean.Format( _T("%.4f"), doc->m_d_1s_mean);
			m_strSmGr1_max.Format( _T("%.4f"), doc->m_d_1s_max);
			m_strSmGr1_rms.Format( _T("%.4f"), doc->m_d_1s_rms);
		break;

		case 2:
			m_strSmGr1_min.Format( _T("%.4f"), doc->m_d_10s_min);
			m_strSmGr1_mean.Format( _T("%.4f"), doc->m_d_10s_mean);
			m_strSmGr1_max.Format( _T("%.4f"), doc->m_d_10s_max);
			m_strSmGr1_rms.Format( _T("%.4f"), doc->m_d_10s_rms);
		break;

		case 3:
			m_strSmGr1_min.Format( _T("%.4f"), doc->m_d_100s_min);
			m_strSmGr1_mean.Format( _T("%.4f"), doc->m_d_100s_mean);
			m_strSmGr1_max.Format( _T("%.4f"), doc->m_d_100s_max);
			m_strSmGr1_rms.Format( _T("%.4f"), doc->m_d_100s_rms);
		break;
	}

	m_strSmGr2_min.Format( _T("%.4f"), doc->m_d_i1_min);
	m_strSmGr2_mean.Format( _T("%.4f"), doc->m_d_i1_mean);
	m_strSmGr2_max.Format( _T("%.4f"), doc->m_d_i1_max);
	m_strSmGr2_rms.Format( _T("%.4f"), doc->m_d_i1_rms);

	m_strSmGr3_min.Format( _T("%.4f"), doc->m_d_i2_min);
	m_strSmGr3_mean.Format( _T("%.4f"), doc->m_d_i2_mean);
	m_strSmGr3_max.Format( _T("%.4f"), doc->m_d_i2_max);
	m_strSmGr3_rms.Format( _T("%.4f"), doc->m_d_i2_rms);
	
	m_strSmGr4_min.Format( _T("%.4f"), doc->m_d_vpc_min);
	m_strSmGr4_mean.Format( _T("%.4f"), doc->m_d_vpc_mean);
	m_strSmGr4_max.Format( _T("%.4f"), doc->m_d_vpc_max);
	m_strSmGr4_rms.Format( _T("%.4f"), doc->m_d_vpc_rms);
	
	m_strSmGr5_min.Format( _T("%.4f"), doc->m_d_aa_min);
	m_strSmGr5_mean.Format( _T("%.4f"), doc->m_d_aa_mean);
	m_strSmGr5_max.Format( _T("%.4f"), doc->m_d_aa_max);
	m_strSmGr5_rms.Format( _T("%.4f"), doc->m_d_aa_rms);
	
  m_strSmGr6_min.Format( _T("%.4f"), doc->m_d_t1_min);
	m_strSmGr6_mean.Format( _T("%.4f"), doc->m_d_t1_mean);
	m_strSmGr6_max.Format( _T("%.4f"), doc->m_d_t1_max);
	m_strSmGr6_rms.Format( _T("%.4f"), doc->m_d_t1_rms);

  /*
  switch( m_nRadT1) {
    case 0:
      m_strSmGr6_min.Format( _T("%.4f"), doc->m_d_t1_min);
	    m_strSmGr6_mean.Format( _T("%.4f"), doc->m_d_t1_mean);
	    m_strSmGr6_max.Format( _T("%.4f"), doc->m_d_t1_max);
	    m_strSmGr6_rms.Format( _T("%.4f"), doc->m_d_t1_rms);
    break;
    case 1:
      m_strSmGr6_min.Format( _T("%.4f"), doc->m_d_t2_min);
	    m_strSmGr6_mean.Format( _T("%.4f"), doc->m_d_t2_mean);
	    m_strSmGr6_max.Format( _T("%.4f"), doc->m_d_t2_max);
	    m_strSmGr6_rms.Format( _T("%.4f"), doc->m_d_t2_rms);
    break;
    case 2:
      m_strSmGr6_min.Format( _T("%.4f"), doc->m_d_t3_min);
	    m_strSmGr6_mean.Format( _T("%.4f"), doc->m_d_t3_mean);
	    m_strSmGr6_max.Format( _T("%.4f"), doc->m_d_t3_max);
	    m_strSmGr6_rms.Format( _T("%.4f"), doc->m_d_t3_rms);
    break;
  }*/
	
  m_strSmGr7_min.Format( _T("%.4f"), doc->m_d_t2_min);
	m_strSmGr7_mean.Format( _T("%.4f"), doc->m_d_t2_mean);
	m_strSmGr7_max.Format( _T("%.4f"), doc->m_d_t2_max);
	m_strSmGr7_rms.Format( _T("%.4f"), doc->m_d_t2_rms);
	/*
  switch( m_nRadT2) {
    case 0:
      m_strSmGr7_min.Format( _T("%.4f"), doc->m_d_t1_min);
	    m_strSmGr7_mean.Format( _T("%.4f"), doc->m_d_t1_mean);
	    m_strSmGr7_max.Format( _T("%.4f"), doc->m_d_t1_max);
	    m_strSmGr7_rms.Format( _T("%.4f"), doc->m_d_t1_rms);
    break;
    case 1:
      m_strSmGr7_min.Format( _T("%.4f"), doc->m_d_t2_min);
	    m_strSmGr7_mean.Format( _T("%.4f"), doc->m_d_t2_mean);
	    m_strSmGr7_max.Format( _T("%.4f"), doc->m_d_t2_max);
	    m_strSmGr7_rms.Format( _T("%.4f"), doc->m_d_t2_rms);
    break;
    case 2:
      m_strSmGr7_min.Format( _T("%.4f"), doc->m_d_t3_min);
	    m_strSmGr7_mean.Format( _T("%.4f"), doc->m_d_t3_mean);
	    m_strSmGr7_max.Format( _T("%.4f"), doc->m_d_t3_max);
	    m_strSmGr7_rms.Format( _T("%.4f"), doc->m_d_t3_rms);
    break;
  }
  */
	
	m_strSmGr8_min.Format( _T("%.4f"), doc->m_d_tsa_min);
	m_strSmGr8_mean.Format( _T("%.4f"), doc->m_d_tsa_mean);
	m_strSmGr8_max.Format( _T("%.4f"), doc->m_d_tsa_max);
	m_strSmGr8_rms.Format( _T("%.4f"), doc->m_d_tsa_rms);

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


	//������������ ����������� ��� ��������� ������ ���������� (��� ��� ������������ ����������)
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

	SetTimer( MY_MAXIMIZE_VIEW_TIMER, 100, NULL);
	SetTimer( MY_TIMER_1000, 1000, NULL);
}

void CMainView::OnTimer(UINT nIDEvent) 
{
	CString strError;
	UpdateData( TRUE);
	
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//������� 1000 ����
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_TIMER_1000) {

		//////////////////////////////////////////////////////////////////////
		// ��������� �������������� ���������
		//////////////////////////////////////////////////////////////////////
		/*
		m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											//��� ����� ���������
		m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//����������� M
		m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		//��������� ����
		m_strParam5Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
		m_strParam6Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
		m_strParam7Val.Format( _T("%.2f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.0));
		m_strParam8Val.Format( _T("%.5f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);
		*/

		//////////////////////////////////////////////////////////////////////
		// ��������� �������
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
		m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											//��� ����� ���������
		m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//����������� M
		m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		//��������� ����
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
	UpdateData( TRUE);
	m_nMainGraph = 1;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("''/sec"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph2() 
{
	m_nMainGraph = 2;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph3() 
{
	m_nMainGraph = 3;	
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph4() 
{
	m_nMainGraph = 4;	
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("V"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph5() 
{
	m_nMainGraph = 5;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("V"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph6() 
{
	m_nMainGraph = 6;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("�C"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph7() 
{
	m_nMainGraph = 7;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("�C"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
}

void CMainView::OnClickGraph8() 
{
	m_nMainGraph = 8;
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("����"));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetAutoScale( true);
	m_ctlMainGraph.GetAxes().Item( "XAxis").SetAutoScale( true);
	((CSlg2Doc *) GetDocument())->m_bZoomed = false;
	SetRefreshTimer( 100);
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
	(( CSlg2Doc *) GetDocument())->ReadDataFile( (( CSlg2Doc *) GetDocument())->m_strCurrentFileName, NULL);
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

void CMainView::OnRadT1Td1() 
{
	m_ctlSmallGraph6.SetCaption( _T("����������� 1 [��], [�C]"));
  SetRefreshTimer( 100);
}

void CMainView::OnRadT1Td2() 
{
	m_ctlSmallGraph6.SetCaption( _T("����������� 2 [��], [�C]"));
  SetRefreshTimer( 100);
}

void CMainView::OnRadT1Td3() 
{
	m_ctlSmallGraph6.SetCaption( _T("����������� 3 [������], [�C]"));
  SetRefreshTimer( 100);
}

void CMainView::OnRadT2Td1() 
{
	m_ctlSmallGraph7.SetCaption( _T("����������� 1 [��], [�C]"));
  SetRefreshTimer( 100);
}

void CMainView::OnRadT2Td2() 
{
	m_ctlSmallGraph7.SetCaption( _T("����������� 2 [��], [�C]"));
  SetRefreshTimer( 100);
}

void CMainView::OnRadT2Td3() 
{
	m_ctlSmallGraph7.SetCaption( _T("����������� 3 [������], [�C]"));
  SetRefreshTimer( 100);
}
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
}


