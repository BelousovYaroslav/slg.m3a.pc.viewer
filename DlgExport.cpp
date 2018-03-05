// DlgExport.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DlgExport.h"
#include "AnalogueParamsConstList.h"
#include "MainFrm.h"
#include "MainView.h"
#include "slg2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSlg2App theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgExport dialog


CDlgExport::CDlgExport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExport)
	m_bChkAA = FALSE;
	m_bChkI1 = FALSE;
	m_bChkI2 = FALSE;
	m_bChkT1 = FALSE;
	m_bChkT2 = FALSE;
	m_bChkVpp = FALSE;
	m_nTmean = 0;
	m_strOutputFileName = _T("");
	m_bChkTimeSA = FALSE;
	m_bChkTglobal = FALSE;
	m_bChkOmega = FALSE;
	m_bChkDt = FALSE;
	m_bChkdPhi = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExport)
	DDX_Control(pDX, IDC_PRG_ACTION, m_ctlProgressBar);
	DDX_Check(pDX, IDC_CHK_AA, m_bChkAA);
	DDX_Check(pDX, IDC_CHK_I1, m_bChkI1);
	DDX_Check(pDX, IDC_CHK_I2, m_bChkI2);
	DDX_Check(pDX, IDC_CHK_T1, m_bChkT1);
	DDX_Check(pDX, IDC_CHK_T2, m_bChkT2);
	DDX_Check(pDX, IDC_CHK_V_PP, m_bChkVpp);
	DDX_Radio(pDX, IDC_NO_MEANING, m_nTmean);
	DDX_Text(pDX, IDC_LBL_OUTPUT_FILENAME, m_strOutputFileName);
	DDX_Check(pDX, IDC_CHK_TSA, m_bChkTimeSA);
	DDX_Check(pDX, IDC_CHK_TGLOBAL, m_bChkTglobal);
	DDX_Check(pDX, IDC_CHK_OMEGA, m_bChkOmega);
	DDX_Check(pDX, IDC_CHK_DT, m_bChkDt);
	DDX_Check(pDX, IDC_CHK_DPHI, m_bChkdPhi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExport, CDialog)
	//{{AFX_MSG_MAP(CDlgExport)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_NO_MEANING, OnNoMeaning)
	ON_BN_CLICKED(IDC_RAD_100M, OnRad100m)
	ON_BN_CLICKED(IDC_RAD_100S, OnRad100s)
	ON_BN_CLICKED(IDC_RAD_10S, OnRad10s)
	ON_BN_CLICKED(IDC_RAD_1S, OnRad1s)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExport message handlers

void CDlgExport::OnOK() 
{
	UpdateData( TRUE);

  //если ничего не выбрали - то и экспортировать нечего
	if( !m_bChkTglobal && !m_bChkDt && !m_bChkdPhi && !m_bChkOmega &&
      !m_bChkI1 && !m_bChkI2 && !m_bChkVpp && !m_bChkAA &&
			!m_bChkT1 && !m_bChkT2) {

		AfxMessageBox( _T("Нет отмеченных данных для экспорта!"));
		return;
	}

	FILE *fhOut = fopen( m_strOutputFileName, _T("w"));

	if( m_bChkTglobal)fprintf( fhOut, _T("%-12s"), _T("t,[сек]"));
	if( m_bChkDt)     fprintf( fhOut, _T("%-12s"), _T("dt,[сек]"));
  if( m_bChkdPhi)   fprintf( fhOut, _T("%-12s"), _T("dphi,['']"));
  if( m_bChkOmega)  fprintf( fhOut, _T("%-12s"), _T("w[''/сек]"));
	if( m_bChkI1)     fprintf( fhOut, _T("%-12s"), _T("I1,[мА]"));
	if( m_bChkI2)     fprintf( fhOut, _T("%-12s"), _T("I2,[мА]"));
	if( m_bChkVpp)    fprintf( fhOut, _T("%-12s"), _T("Vpc,[В]"));
	if( m_bChkAA)     fprintf( fhOut, _T("%-12s"), _T("AA,['']"));
	if( m_bChkT1)     fprintf( fhOut, _T("%-12s"), _T("T1(HIRO),[°C]"));
	if( m_bChkT2)     fprintf( fhOut, _T("%-12s"), _T("T2(BODY),[°C]"));

	fprintf( fhOut, _T("\n"));

	FILE *fh;
	/*switch( m_nStatFileVersion) {
		case 1: fh = fopen( m_strFileName, "r"); break;
		case 2: fh = fopen( m_strFileName, "rb"); break;
		default: CDialog::OnOK(); return;
	}*/
  fh = fopen( m_strFileName, "rb");
	
  CMainFrame *pFrm = ( CMainFrame *) AfxGetApp()->GetMainWnd();
  CMainView *pView = ( CMainView *) pFrm->GetActiveView();
  CSlg2Doc *pDoc =   ( CSlg2Doc *) pView->GetDocument();
	m_ctlProgressBar.SetRange32( 0, ( long) pDoc->m_dblMeasDuration);

	
  char nf_package[14];

  double w_Summ = 0., w_Counter = 0.;
  double i1_Summ = 0., i1_Counter = 0.;
  double i2_Summ = 0., i2_Counter = 0.;
  double vpc_Summ = 0., vpc_Counter = 0.;
  double aa_Summ = 0., aa_Counter = 0.;
  double t1_Summ = 0., t1_Counter = 0., t1_prev = 0.;
  double t2_Summ = 0., t2_Counter = 0., t2_prev = 0.;
  double tsa_Summ = 0.;
  double dGlobalTime = 0.;

  double tMean;
  switch( m_nTmean) {
    case 1: tMean = 0.1; break;
    case 2: tMean = 1.; break;
    case 3: tMean = 10.; break;
    case 4: tMean = 100.; break;
  }

  while( 1) {

    fread( nf_package, 14, 1, fh);



    double dbl1secInTacts = 2607104;

    unsigned short nSaTime = ( nf_package[10] << 8) + nf_package[9];
    double dblTime = nSaTime / dbl1secInTacts;


    //*****************************************************
    //ФЛАГИ
    BOOL bVeracity    = ( nf_package[12] & 0x80) ? TRUE : FALSE;
    BOOL bLockBit     = ( nf_package[12] & 0x40) ? TRUE : FALSE;
    BOOL bSyncAsync   = ( nf_package[12] & 0x20) ? TRUE : FALSE;
    BOOL bdWdNdU      = ( nf_package[12] & 0x10) ? TRUE : FALSE;
    short shErrorCode =  nf_package[12] & 0x0F;

    //*****************************************************
    //АНАЛОГОВЫЙ ПАРАМЕТР
    //индикатор аналогового параметра
    int nAnParam = nf_package[6];

    //аналоговый параметр
    unsigned short shCur1 = ( nf_package[8] << 8) + nf_package[7];
    double dblCur1 = ( double) shCur1;

    double dblAnParamValue;
    switch( nAnParam) {
      case UTD1:    t1_Summ += shCur1;  t1_Counter += 1.;    break;
      case UTD2:    t2_Summ += shCur1;  t2_Counter += 1.;    break;
      //case UTD3:    t3_Summ += shCur1;  t3_Counter += 1.;    break;

      case I1:      i1_Summ += shCur1;  i1_Counter += 1.;    break;
      case I2:      i2_Summ += shCur1;  i2_Counter += 1.;    break;
      

      case CNTRPC:  vpc_Summ += shCur1; vpc_Counter += 1.;   break;

      case AMPLANG_ALTERA:
                    aa_Summ += shCur1;   aa_Counter += 1.;   break;
    
    }

    double dblPhi;
    if( bdWdNdU == TRUE) {
      fclose( fh);
      fclose( fhOut);
      AfxMessageBox( "В файле данных есть пачка в которой передаётся dN dU.\nТакие пачки экспорт обрабатываеть не умеет.\nСбрасываем процесс экспорта.");
      CDialog::OnOK();
      return;
    }
    else {
      //рабочий режим (передаётся phi)

      //float f_dN;
      int n_dN;
      char *ptr;      
    
      ptr = ( char *) &n_dN;
		
      ptr[0] = nf_package[2];
		  ptr[1] = nf_package[3];
		  ptr[2] = nf_package[4];
		  ptr[3] = nf_package[5];

		  dblPhi = ( ( double) n_dN / 2147483647. * 99310.);
    }

    dblPhi *= theApp.GetSettings()->GetScaleCoeff() / 4.;
    w_Summ += dblPhi;


    tsa_Summ += dblTime;
    dGlobalTime += dblTime;



    if( feof( fh))
      break;

    if( !m_nTmean) {

      //тактные значения без осреднения
			if( m_bChkTglobal) fprintf( fhOut, _T("%-12.4f"), dGlobalTime);
    	if( m_bChkDt)      fprintf( fhOut, _T("%-12.4f"), dblTime);
      if( m_bChkdPhi)    fprintf( fhOut, _T("%-12.4f"), dblPhi);
      if( m_bChkOmega)   fprintf( fhOut, _T("%-12.4f"), dblPhi / dblTime);
			if( m_bChkI1) fprintf( fhOut, _T("%-12.4f"), ( 2.5 - i1_Summ / i1_Counter / 4096. * 3.) / 2.5); //i1_Summ / i1_Counter / 4096. * 3. / 3.973);
			if( m_bChkI2) fprintf( fhOut, _T("%-12.4f"), ( 2.5 - i2_Summ / i2_Counter / 4096. * 3.) / 2.5); //i2_Summ / i2_Counter / 4096. * 3. / 3.973);
			if( m_bChkVpp) fprintf( fhOut, _T("%-12.4f"), (( vpc_Summ / vpc_Counter / 4096. * 3.) - 2.048) * 100.);
			if( m_bChkAA) fprintf( fhOut, _T("%-12.4f"), ( aa_Summ / aa_Counter / 4096. * 3.) / 0.5);
			
			if( m_bChkT1) {
				//fprintf( fhOut, _T("%-12.4f"), 70.24586 * ( t1_Summ / t1_Counter / 4096. * 3.) - 128.209);
				if( t1_Counter != 0) {
					t1_prev = t1_Summ / t1_Counter / 65535. * 200. - 100.;
					fprintf( fhOut, _T("%-12.4f"), t1_prev);
				}
				else
					fprintf( fhOut, _T("%-12.4f"), t1_prev);
			}

			if( m_bChkT2) {
				//fprintf( fhOut, _T("%-12.4f"), 70.24586 * ( t2_Summ / t2_Counter / 4096. * 3.) - 128.209);
				if( t2_Counter != 0) {
					t2_prev = t2_Summ / t2_Counter / 65535. * 200. - 100.;
					fprintf( fhOut, _T("%-12.4f"), t2_prev);
				}
				else
					fprintf( fhOut, _T("%-12.4f"), t2_prev);
			}

			fprintf( fhOut, _T("\n"));

			tsa_Summ = 0.;
			w_Summ = w_Counter = 0.;
			i1_Summ = i1_Counter = 0.;
			i2_Summ = i2_Counter = 0.;
			vpc_Summ = vpc_Counter = 0.;
			aa_Summ = aa_Counter = 0.;
			t1_Summ = t1_Counter = 0.;
			t2_Summ = t2_Counter = 0.;
		}
		else if( tsa_Summ > tMean) {
      //если набрали времени больше чем время осреденения

      if( m_bChkTglobal) fprintf( fhOut, _T("%-12.4f"), dGlobalTime);
    	if( m_bChkDt)      fprintf( fhOut, _T("%-12.4f"), tsa_Summ);
      if( m_bChkdPhi)    fprintf( fhOut, _T("%-12.4f"), w_Summ);
      if( m_bChkOmega)   fprintf( fhOut, _T("%-12.4f"), w_Summ / tsa_Summ);
			
      if( m_bChkI1)
        fprintf( fhOut, _T("%-12.4f"), ( 2.5 - i1_Summ / i1_Counter / 4096. * 3.) / 2.5); //i1_Summ / i1_Counter / 4096. * 3. / 3.973);
			
      if( m_bChkI2)
        fprintf( fhOut, _T("%-12.4f"), ( 2.5 - i2_Summ / i2_Counter / 4096. * 3.) / 2.5); //i2_Summ / i2_Counter / 4096. * 3. / 3.973);

			if( m_bChkVpp)
        fprintf( fhOut, _T("%-12.4f"), (( vpc_Summ / vpc_Counter / 4096. * 3.) - 2.048) * 100.);
			
			if( m_bChkAA)
				fprintf( fhOut, _T("%-12.4f"), aa_Summ / aa_Counter / 4. * theApp.GetSettings()->GetScaleCoeff());//( aa_Summ / aa_Counter / 4096. * 3.) / 0.5);
				
			if( m_bChkT1)
				//fprintf( fhOut, _T("%-12.4f"), 70.24586 * ( t1_Summ / t1_Counter / 4096. * 3.) - 128.209);
				fprintf( fhOut, _T("%-12.4f"), t1_Summ / t1_Counter / 65535. * 200. - 100.);

			if( m_bChkT2) 
				//fprintf( fhOut, _T("%-12.4f"), 70.24586 * ( t2_Summ / t2_Counter / 4096. * 3.) - 128.209);
				fprintf( fhOut, _T("%-12.4f"), t2_Summ / t2_Counter / 65535. * 200. - 100.);

			fprintf( fhOut, _T("\n"));

			tsa_Summ = 0.;
			w_Summ = w_Counter = 0.;
			i1_Summ = i1_Counter = 0.;
			i2_Summ = i2_Counter = 0.;
			vpc_Summ = vpc_Counter = 0.;
			aa_Summ = aa_Counter = 0.;
			t1_Summ = t1_Counter = 0.;
			t2_Summ = t2_Counter = 0.;
		}

		
		m_ctlProgressBar.SetPos( ( long) dGlobalTime);

	}

	fclose(fh);
	fclose(fhOut);

	CDialog::OnOK();
}

BOOL CDlgExport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	(( CButton *) GetDlgItem( IDC_CHK_TSA))->SetCheck( BST_CHECKED);
	(( CButton *) GetDlgItem( IDC_CHK_ROT_SPEED))->SetCheck( BST_CHECKED);

	::GetCurrentDirectory( 1024, m_strOutputFileName.GetBuffer( 1024));
	m_strOutputFileName.ReleaseBuffer();
	m_strOutputFileName += _T("\\export.txt");

	UpdateData( FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExport::OnBtnBrowse() 
{
	UpdateData( TRUE);
	char szFilters[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
  CFileDialog fileDlg( FALSE, "txt", "*.txt", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal() == IDOK) {
		m_strOutputFileName = fileDlg.GetPathName();
		UpdateData( FALSE);
	}
}

void CDlgExport::OnNoMeaning() 
{
	//GetDlgItem( IDC_CHK_ROT_SPEED)->SetWindowText( _T("Угол поворота"));
}

void CDlgExport::OnRad100m() 
{
	//GetDlgItem( IDC_CHK_ROT_SPEED)->SetWindowText( _T("Угловая скорость"));
}

void CDlgExport::OnRad100s() 
{
	//GetDlgItem( IDC_CHK_ROT_SPEED)->SetWindowText( _T("Угловая скорость"));
}

void CDlgExport::OnRad10s() 
{
	//GetDlgItem( IDC_CHK_ROT_SPEED)->SetWindowText( _T("Угловая скорость"));
}

void CDlgExport::OnRad1s() 
{
	//GetDlgItem( IDC_CHK_ROT_SPEED)->SetWindowText( _T("Угловая скорость"));
}
