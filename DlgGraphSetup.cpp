// DlgGraphSetup.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DlgGraphSetup.h"
#include "MainView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSlg2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup dialog


CDlgGraphSetup::CDlgGraphSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGraphSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSetup)
	DDX_Control(pDX, IDC_LBL_LINE_COLOR, m_ctlColorLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSetup)
	ON_EN_CHANGE(IDC_EDT_R, OnChangeEdtR)
	ON_EN_CHANGE(IDC_EDT_G, OnChangeEdtG)
	ON_EN_CHANGE(IDC_EDT_B, OnChangeEdtB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup message handlers

//DEL void CDlgGraphSetup::OnLblLineColor() 
//DEL {
//DEL   m_ctlCommonDialog.ShowColor();
//DEL   unsigned long col = m_ctlCommonDialog.GetColor();
//DEL   unsigned short nRed   = ( unsigned short) ( col & 0xFF);
//DEL   unsigned short nGreen = ( unsigned short) (( col & 0xFF00) >> 8);
//DEL   unsigned short nBlue  = ( unsigned short) (( col & 0xFF0000) >> 16);	
//DEL   
//DEL   m_ctlColorLabel.SetBgColor( RGB( nRed, nGreen, nBlue));
//DEL   m_ctlColorLabel.Invalidate();
//DEL }

void CDlgGraphSetup::Init( int nGraph)
{
  CString str;

  m_nGraph = nGraph;

  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph)->Get_bMinY());

  str.Format( _T("%.03f"), theApp.GetSettings()->GetGraphSettings( nGraph)->Get_dblMinY());
  ( ( CWnd *) GetDlgItem( IDC_EDT_Y_MIN))->SetWindowText( str);


  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph)->Get_bMaxY());
  str.Format( _T("%.03f"), theApp.GetSettings()->GetGraphSettings( nGraph)->Get_dblMaxY());
  ( ( CWnd *) GetDlgItem( IDC_EDT_Y_MAX))->SetWindowText( str);

  m_ctlColorLabel.SetBgColor( theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor());
}

void CDlgGraphSetup::OnOK() 
{
  BOOL bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->GetCheck();
  theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_bMinY( bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MIN))->GetWindowText( str);
    theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_dblMinY( atof( str));
  }
  
  bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->GetCheck();
  theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_bMaxY( bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MAX))->GetWindowText( str);
    theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_dblMaxY( atof( str));
  }

  //theApp.m_ctlColorLabel.SetBgColor( theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor());
  theApp.GetSettings()->GetGraphSettings( m_nGraph)->SetLineColor( m_ctlColorLabel.GetBgColor());

  
  CDialog::OnOK();
  CMainFrame *frm = ( CMainFrame *) theApp.GetMainWnd();
  CMainView *view = ( CMainView *) frm->GetActiveView();
  view->SetRefreshTimer( 100);
}

void CDlgGraphSetup::OnChangeEdtR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	ColorChanged();
}

void CDlgGraphSetup::OnChangeEdtG() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	ColorChanged();
}

void CDlgGraphSetup::OnChangeEdtB() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	ColorChanged();
}

void CDlgGraphSetup::ColorChanged()
{
  CString tmp;
  UINT r, g, b;
	
  ( ( CWnd *) GetDlgItem( IDC_EDT_R))->GetWindowText( tmp);
  r = atoi( tmp);

  ( ( CWnd *) GetDlgItem( IDC_EDT_G))->GetWindowText( tmp);
  g = atoi( tmp);

  ( ( CWnd *) GetDlgItem( IDC_EDT_B))->GetWindowText( tmp);
  b = atoi( tmp);
	
  m_ctlColorLabel.SetBgColor( RGB( r, g, b));
  m_ctlColorLabel.Invalidate();
}
