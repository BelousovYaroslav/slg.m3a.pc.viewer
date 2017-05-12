// slg2Doc.cpp : implementation of the CSlg2Doc class
//

#include "stdafx.h"
#include "slg2.h"

#include "slg2Doc.h"
#include "MainView.h"
#include "OpenMeasDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSlg2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CSlg2Doc

IMPLEMENT_DYNCREATE(CSlg2Doc, CDocument)

BEGIN_MESSAGE_MAP(CSlg2Doc, CDocument)
	//{{AFX_MSG_MAP(CSlg2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlg2Doc construction/destruction

CSlg2Doc::CSlg2Doc()
{
  m_nMcVersionMajor = 0;
  m_nMcVersionMiddle = 0;
  m_nMcVersionMinor = 0;

	m_strCurrentFileName.Empty();

	m_nDataFileLines = 0;
	m_nStatFileVersion = 0;

	m_bShowCursors = false;
	m_bZoomed = false;
}

CSlg2Doc::~CSlg2Doc()
{
}

BOOL CSlg2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CString tmps;
	tmps.LoadString( IDS_SOFTWARE_VERSION);
	this->SetTitle( _T("Viewer ") + tmps);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSlg2Doc serialization

void CSlg2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSlg2Doc diagnostics

#ifdef _DEBUG
void CSlg2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSlg2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSlg2Doc commands

BOOL CSlg2Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_strCurrentFileName = lpszPathName;

  if (!CDocument::OnOpenDocument(lpszPathName)) {
    //delete dlg;
    return FALSE;
  }

  COpenMeasDlg dlg;
  dlg.m_strOpenFilePathName = lpszPathName;
  dlg.DoModal();
	
  /*
  COpenMeasDlg *dlg;
  dlg = new COpenMeasDlg();
	dlg->Create( IDD_OPEN_MEAS_DLG, NULL);
	dlg->ShowWindow( SW_SHOW);
	dlg->CenterWindow();
  */

  /*
  
	
	m_nDataFileLines = 0;
	//ReadDataFile( m_strCurrentFileName, dlg);
	*/

	POSITION pos = GetFirstViewPosition();
	(( CMainView* ) GetNextView( pos))->SetRefreshTimer();

  /*  
	dlg->EndDialog( 0);
	delete dlg;
  */

	return TRUE;
}

void CSlg2Doc::OnCloseDocument() 
{
	//память под линии измерений автоматически должна удалиться тут
  //(деструктор документа вызовет деструкторы объектов линий данный)

	CDocument::OnCloseDocument();
}

void CSlg2Doc::RecalculateStatistics()
{
	m_dpW.CalcMeanAndRMS();
  m_dpI1.CalcMeanAndRMS();
  m_dpI2.CalcMeanAndRMS();
  m_dpVpc.CalcMeanAndRMS();
  m_dpAAa.CalcMeanAndRMS();
  m_dpAAd.CalcMeanAndRMS();
  m_dpAAr.CalcMeanAndRMS();
  m_dpT1.CalcMeanAndRMS();
  m_dpT2.CalcMeanAndRMS();
  m_dpT3.CalcMeanAndRMS();
  m_dpTsa.CalcMeanAndRMS();
  m_dpDecCoeff.CalcMeanAndRMS();
}

//DEL void CSlg2Doc::ReadDataFile(CString filename, COpenMeasDlg *dlg)
//DEL {
//DEL 	/*FILE *fh_test_needs;
//DEL 	fh_test_needs = fopen( _T("d:\\test_needs.txt"), _T("w"));*/
//DEL 
//DEL 	FILE *fh;
//DEL 	fh = fopen( filename, _T("r"));
//DEL 	if( fh == NULL) {
//DEL 		AfxMessageBox( _T("Ошибка при открытии файла измерения."));
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//определяем тип файла статистики (старый(ascii) или новый(bin))
//DEL 	BOOL bNewOld;
//DEL 	char buf_check[2];
//DEL 	fread( buf_check, 2, 1, fh);
//DEL 	if( buf_check[1] == '.')	{		// <---- фаршно, но терпимо
//DEL 		bNewOld = false;	//ascii
//DEL 		m_nStatFileVersion = 1;
//DEL 	}
//DEL 	else {
//DEL 		bNewOld = true;		//bin
//DEL 		m_nStatFileVersion = 2;
//DEL 		fclose( fh);
//DEL 		fh = fopen( filename, _T("rb"));
//DEL 	}
//DEL 	if( dlg != NULL) {
//DEL 		fseek( fh, 0, SEEK_END);
//DEL 		dlg->m_ctlProgress1.SetRange32( 0, ftell( fh));
//DEL 		dlg->m_ctlProgress1.SetPos( 0);
//DEL 		dlg->m_ctlProgress2.SetRange32( 0, ftell( fh));
//DEL 		dlg->m_ctlProgress2.SetPos( 0);
//DEL 	}
//DEL 	fseek( fh, 0, SEEK_SET);
//DEL 
//DEL 	if( bNewOld) {
//DEL 		// ********************************************************************
//DEL 		// ** NEW BIN STAT FILE
//DEL 		// ********************************************************************
//DEL 		//определяем количество линий
//DEL 		m_dn100m = 0;
//DEL 		double tsasumm = 0.;
//DEL 		while( 1) {
//DEL 			float f1;
//DEL 			short s1, s2, s3;
//DEL 			if( fread( &f1, sizeof( float), 1, fh) < 1)
//DEL 				break;
//DEL 			if( fread( &s1, sizeof( short), 1, fh) < 1)
//DEL 				break;
//DEL 			if( fread( &s2, sizeof( short), 1, fh) < 1) {
//DEL 				break;
//DEL 			}
//DEL 			if( fread( &s3, sizeof( short), 1, fh) < 1)
//DEL 				break;
//DEL 
//DEL 			m_nDataFileLines++;
//DEL 			tsasumm += ( double) s3 / 32768.;
//DEL 			if( tsasumm > 0.1) {
//DEL 				m_dn100m++;
//DEL 				tsasumm = 0.;
//DEL 			}
//DEL 
//DEL 			if( dlg != NULL)
//DEL 				if( !( m_nDataFileLines % 100))
//DEL 					dlg->m_ctlProgress2.SetPos( ftell( fh));
//DEL 
//DEL 			if( feof( fh))
//DEL 				break;
//DEL 		}
//DEL 	
//DEL 		//высвобождаем если что либо заказано (что-то было открыто)
//DEL 		if( m_dx100m != NULL) { delete m_dx100m; m_dx100m = NULL;}
//DEL 		if( m_dy100m != NULL) { delete m_dy100m; m_dy100m = NULL;}
//DEL 		if( m_dx1s != NULL) { delete m_dx1s; m_dx1s = NULL;}
//DEL 		if( m_dy1s != NULL) { delete m_dy1s; m_dy1s = NULL;}
//DEL 		if( m_dx10s != NULL) { delete m_dx10s; m_dx10s = NULL;}
//DEL 		if( m_dy10s != NULL) { delete m_dy10s; m_dy10s = NULL;}
//DEL 		if( m_dx100s != NULL) { delete m_dx100s; m_dx100s = NULL;}
//DEL 		if( m_dy100s != NULL) { delete m_dy100s; m_dy100s = NULL;}
//DEL 		if( m_dx_i1 != NULL) { delete m_dx_i1; m_dx_i1 = NULL;}
//DEL 		if( m_dy_i1 != NULL) { delete m_dy_i1; m_dy_i1 = NULL;}
//DEL 		if( m_dx_i2 != NULL) { delete m_dx_i2; m_dx_i2 = NULL;}
//DEL 		if( m_dy_i2 != NULL) { delete m_dy_i2; m_dy_i2 = NULL;}
//DEL 		if( m_dx_vpc != NULL) { delete m_dx_vpc; m_dx_vpc = NULL;}
//DEL 		if( m_dy_vpc != NULL) { delete m_dy_vpc; m_dy_vpc = NULL;}
//DEL 		if( m_dx_aa != NULL) { delete m_dx_aa; m_dx_aa = NULL;}
//DEL 		if( m_dy_aa != NULL) { delete m_dy_aa; m_dy_aa = NULL;}
//DEL 		if( m_dx_t1 != NULL) { delete m_dx_t1; m_dx_t1 = NULL;}
//DEL 		if( m_dy_t1 != NULL) { delete m_dy_t1; m_dy_t1 = NULL;}
//DEL 		if( m_dx_t2 != NULL) { delete m_dx_t2; m_dx_t2 = NULL;}
//DEL 		if( m_dy_t2 != NULL) { delete m_dy_t2; m_dy_t2 = NULL;}
//DEL     if( m_dx_t3 != NULL) { delete m_dx_t3; m_dx_t3 = NULL;}
//DEL     if( m_dy_t3 != NULL) { delete m_dy_t3; m_dy_t3 = NULL;}
//DEL 		if( m_dx_tsa != NULL) { delete m_dx_tsa; m_dx_tsa = NULL;}
//DEL 		if( m_dy_tsa != NULL) { delete m_dy_tsa; m_dy_tsa = NULL;}
//DEL 	 
//DEL 		//заказываем память под измерения
//DEL 		m_dx100m = new double[m_dn100m];
//DEL 		m_dy100m = new double[m_dn100m];
//DEL 		m_dx1s = new double[m_dn100m];
//DEL 		m_dy1s = new double[m_dn100m];
//DEL 		m_dx10s = new double[m_dn100m];
//DEL 		m_dy10s = new double[m_dn100m];
//DEL 		m_dx100s = new double[m_dn100m];
//DEL 		m_dy100s = new double[m_dn100m];
//DEL 		m_dx_i1 = new double[m_dn100m];
//DEL 		m_dy_i1 = new double[m_dn100m];
//DEL 		m_dx_i2	= new double[m_dn100m];
//DEL 		m_dy_i2 = new double[m_dn100m];
//DEL 		m_dx_vpc = new double[m_dn100m];
//DEL 		m_dy_vpc = new double[m_dn100m];
//DEL 		m_dx_aa	= new double[m_dn100m];
//DEL 		m_dy_aa = new double[m_dn100m];
//DEL 		m_dx_t1	= new double[m_dn100m];
//DEL 		m_dy_t1 = new double[m_dn100m];
//DEL 		m_dx_t2	= new double[m_dn100m];
//DEL 		m_dy_t2 = new double[m_dn100m];
//DEL     m_dx_t3	= new double[m_dn100m];
//DEL 		m_dy_t3 = new double[m_dn100m];
//DEL 		m_dx_tsa = new double[m_dn100m];
//DEL 		m_dy_tsa = new double[m_dn100m];
//DEL 
//DEL 		m_dn_i1 = m_dn_i2 = m_dn_vpc = m_dn_aa =
//DEL 		m_dn_t1 = m_dn_t2 = m_dn_t3 = m_dn_tsa = m_dn100m;
//DEL 
//DEL 		double d_global_time = 0.;
//DEL 	
//DEL 		double w100m = 0., t100m = 0.;
//DEL 		int n100m = 0;
//DEL 
//DEL 		double w1s = 0., t1s = 0.;
//DEL 		int n1s = 0;
//DEL 		m_dn1s = 0;
//DEL 
//DEL 		double w10s = 0., t10s = 0.;
//DEL 		int n10s = 0;
//DEL 		m_dn10s = 0;
//DEL 
//DEL 		double w100s = 0., t100s = 0.;
//DEL 		int n100s = 0;
//DEL 		m_dn100s = 0;
//DEL 
//DEL 		double i1 = 0.;
//DEL 		int n_i1_s = 0, n_i1_n = 0;
//DEL 
//DEL 		double i2 = 0.;
//DEL 		int n_i2_s = 0, n_i2_n = 0;
//DEL 
//DEL 		double vpc = 0.;
//DEL 		int n_vpc_s = 0, n_vpc_n = 0;
//DEL 
//DEL 		double aa = 0.;
//DEL 		int n_aa_s = 0, n_aa_n = 0;
//DEL 
//DEL 		double temp1 = 0.;
//DEL 		int n_temp1_s = 0, n_temp1_n = 0;
//DEL 
//DEL 		double temp2 = 0.;
//DEL 		int n_temp2_s = 0, n_temp2_n = 0;
//DEL 
//DEL     double temp3 = 0.;
//DEL 		int n_temp3_s = 0, n_temp3_n = 0;
//DEL 
//DEL 		double tsa = 0.;
//DEL 		int n_tsa_s = 0, n_tsa_n = 0;
//DEL 
//DEL 		fseek( fh, 0, SEEK_SET);
//DEL 
//DEL 		while( 1) {
//DEL 			float f1;
//DEL 			short s1, s2, s3;
//DEL       int d1;
//DEL 			
//DEL 			fread( &d1, sizeof( int), 1, fh);
//DEL 			fread( &s1, sizeof( short), 1, fh);
//DEL 			fread( &s2, sizeof( short), 1, fh);
//DEL 			fread( &s3, sizeof( short), 1, fh);
//DEL 		
//DEL 			/*fprintf( fh_test_needs, _T("%.4f %.4f\n"), d_global_time, f1);*/
//DEL 
//DEL 			if( s2 == -1) {
//DEL 				f1 = 0;
//DEL 			}
//DEL 
//DEL       CString strTmp;
//DEL 
//DEL 			double p_phi = ( d1 / 2147483647. * 99310.) * theApp.GetSettings()->GetScaleCoeff() / 4.;
//DEL 			double p_tsa = s3 / 32768.;
//DEL 			switch( s1 & 0x7F) {
//DEL 				case 0: temp1 += ( unsigned short) s2; n_temp1_s++; break;	//Temp1
//DEL 				case 1: temp2 += ( unsigned short) s2; n_temp2_s++; break;	//Temp2
//DEL         case 2: temp3 += ( unsigned short) s2; n_temp3_s++; break;	//Temp3
//DEL 
//DEL 				case 3: i1 += s2;	n_i1_s++; break;			//i1
//DEL 				case 4: i2 += s2;	n_i2_s++; break;			//i2
//DEL 				case 5: vpc += s2;	n_vpc_s++; break;		//vpc
//DEL 				case 6:
//DEL 					aa += s2;	n_aa_s++;
//DEL 				break;			//aa
//DEL           
//DEL 				case 16: //software version
//DEL 					( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer.Format( _T("%d.%d.%d.%d"),
//DEL 							( s2 & 0x00F0) >> 4,
//DEL 							( s2 & 0x000F),
//DEL 							( s2 & 0xF000) >> 12,
//DEL 							( s2 & 0x0F00) >> 8);
//DEL           
//DEL           
//DEL           /*
//DEL           strTmp.Format( "0x%x\n%d.%d.%d.%d", s2,
//DEL               ( s2 & 0x00F0) >> 4,
//DEL 							( s2 & 0x000F),
//DEL 							( s2 & 0xF000) >> 12,
//DEL 							( s2 & 0x0F00) >> 8);
//DEL           
//DEL           AfxMessageBox(strTmp);
//DEL           */
//DEL 
//DEL 				break;
//DEL 
//DEL         /*
//DEL         case 25:
//DEL 					 ( ( CSlg2App *) AfxGetApp())->m_shPhaseShift = nCur1;
//DEL 			  break; //фазовый сдвиг
//DEL         */
//DEL 			}
//DEL 
//DEL 			tsa += p_tsa;			n_tsa_s++;
//DEL 
//DEL 			w100m += p_phi;		t100m += p_tsa;
//DEL 			w1s += p_phi;			t1s += p_tsa;
//DEL 			w10s += p_phi;		t10s += p_tsa;
//DEL 			w100s += p_phi;		t100s += p_tsa;
//DEL 		
//DEL 		
//DEL 
//DEL 			d_global_time += p_tsa;
//DEL 		
//DEL 			if( t100m > 0.1) {
//DEL 				double w100 = w100m / t100m;
//DEL 				//100 ms points
//DEL 				m_dx100m[n100m] = d_global_time;
//DEL 				m_dy100m[n100m] = w100;
//DEL 				n100m++;
//DEL 				w100m = t100m = 0.;
//DEL 
//DEL 				//i1 points
//DEL 				m_dx_i1[ n_i1_n] = d_global_time;
//DEL 				m_dy_i1[ n_i1_n] = ( 2.5 - i1 / n_i1_s / 4096. * 3.) / 2.5;
//DEL 				if( m_dy_i1[ n_i1_n] < 0.57) {
//DEL 					i1 = 0;
//DEL 				}
//DEL 				n_i1_n++;
//DEL 				i1 = 0.; n_i1_s = 0;
//DEL 
//DEL 				//i2 points
//DEL 				m_dx_i2[ n_i2_n] = d_global_time;
//DEL 				m_dy_i2[ n_i2_n] = ( 2.5 - i2 / n_i2_s / 4096. * 3.) / 2.5;
//DEL 				n_i2_n++;
//DEL 				i2 = 0.; n_i2_s = 0;
//DEL 			
//DEL 				//vpc points
//DEL 				m_dx_vpc[ n_vpc_n] = d_global_time;
//DEL 				m_dy_vpc[ n_vpc_n] = (( vpc / n_vpc_s / 4096. * 3.) - 2.048) * 100.;
//DEL 				n_vpc_n++;
//DEL 				vpc = 0.; n_vpc_s = 0;
//DEL 
//DEL 				//aa points
//DEL 				m_dx_aa[ n_aa_n] = d_global_time;
//DEL 				m_dy_aa[ n_aa_n] = aa / n_aa_s / 4. * theApp.GetSettings()->GetScaleCoeff();
//DEL 				n_aa_n++;
//DEL 				aa = 0.; n_aa_s = 0;
//DEL 
//DEL 				//t1 points
//DEL 				m_dx_t1[ n_temp1_n] = d_global_time;
//DEL 				m_dy_t1[ n_temp1_n] = temp1 / n_temp1_s / 65535. * 200. - 100.;
//DEL 				//m_dy_t1[ n_temp1_n] = 70.24586 * ( temp1 / n_temp1_s / 4096. * 3.) - 128.209;
//DEL 				//m_dy_t1[ n_temp1_n] = ( temp1 / n_temp1_s / 4096. * 3.);
//DEL 				n_temp1_n++;
//DEL 				temp1 = 0.; n_temp1_s = 0;
//DEL 
//DEL 				//t2 points
//DEL 				m_dx_t2[ n_temp2_n] = d_global_time;
//DEL 				m_dy_t2[ n_temp2_n] = temp2 / n_temp2_s / 65535. * 200. - 100.;
//DEL 				//m_dy_t2[ n_temp2_n] = 70.24586 * ( temp2 / n_temp2_s / 4096. * 3.) - 128.209;
//DEL 				//m_dy_t2[ n_temp2_n] = ( temp2 / n_temp2_s / 4096. * 3.);
//DEL 				n_temp2_n++;
//DEL 				temp2 = 0.; n_temp2_s = 0;
//DEL 
//DEL         //t3 points
//DEL 				m_dx_t3[ n_temp3_n] = d_global_time;
//DEL 				m_dy_t3[ n_temp3_n] = temp3 / n_temp3_s / 65535. * 200. - 100.;
//DEL 				n_temp3_n++;
//DEL 				temp3 = 0.; n_temp3_s = 0;
//DEL 
//DEL 				//tsa points
//DEL 				m_dx_tsa[ n_tsa_n] = d_global_time;
//DEL 				m_dy_tsa[ n_tsa_n] = tsa / n_tsa_s;
//DEL 				n_tsa_n++;
//DEL 				tsa = 0.; n_tsa_s = 0;
//DEL 			}
//DEL 
//DEL 			//1-секундные точки
//DEL 			if( t1s > 1.) {
//DEL 				m_dx1s[n1s] = d_global_time;
//DEL 				m_dy1s[n1s] = w1s / t1s;
//DEL 				n1s++;
//DEL 				m_dn1s += 1.;
//DEL 				w1s = 0.; t1s = 0.;
//DEL 			}
//DEL 
//DEL 			//10-секундные точки
//DEL 			if( t10s > 10.) {
//DEL 				m_dx10s[n10s] = d_global_time;
//DEL 				m_dy10s[n10s] = w10s / t10s;
//DEL 				n10s++;
//DEL 				m_dn10s += 1.;
//DEL 				w10s = 0.; t10s = 0.;
//DEL 			}
//DEL 
//DEL 			//100-секундные точки
//DEL 			if( t100s > 100.) {
//DEL 				m_dx100s[n100s] = d_global_time;
//DEL 				m_dy100s[n100s] = w100s / t100s;
//DEL 				n100s++;
//DEL 				m_dn100s += 1.;
//DEL 				w100s = 0.; t100s = 0.;
//DEL 			}
//DEL 
//DEL 			if( dlg != NULL)
//DEL 				if( !( n100m % 50))
//DEL 					dlg->m_ctlProgress1.SetPos( ftell( fh));
//DEL 
//DEL 			if( feof( fh))
//DEL 				break;
//DEL 		}
//DEL 	}
//DEL 	else {
//DEL 		// ********************************************************************
//DEL 		// ** OLD ASCII STAT FILE
//DEL 		// ********************************************************************
//DEL 		//определяем количество линий
//DEL 		m_dn100m = 0;
//DEL 		double tsasumm = 0.;
//DEL 		while( 1) {
//DEL 			float f1, f2;
//DEL 			int i1, i2, i3;
//DEL 			fscanf(fh, "%f\t%f\t%d\t%d\t%d\n", &f1, &f2, &i1, &i2, &i3);
//DEL 			m_nDataFileLines++;
//DEL 			tsasumm += ( double) i3 / 32768.;
//DEL 			if( tsasumm > 0.1) {
//DEL 				m_dn100m++;
//DEL 				tsasumm = 0.;
//DEL 			}
//DEL 
//DEL 			if( dlg != NULL)
//DEL 				if( !( m_nDataFileLines % 100))
//DEL 					dlg->m_ctlProgress2.SetPos( ftell( fh));
//DEL 
//DEL 			if( feof( fh))
//DEL 				break;
//DEL 		}
//DEL 	
//DEL 		//высвобождаем если что либо заказано (что-то было открыто)
//DEL 		if( m_dx100m != NULL) { delete m_dx100m; m_dx100m = NULL;}
//DEL 		if( m_dy100m != NULL) { delete m_dy100m; m_dy100m = NULL;}
//DEL 		if( m_dx1s != NULL) { delete m_dx1s; m_dx1s = NULL;}
//DEL 		if( m_dy1s != NULL) { delete m_dy1s; m_dy1s = NULL;}
//DEL 		if( m_dx10s != NULL) { delete m_dx10s; m_dx10s = NULL;}
//DEL 		if( m_dy10s != NULL) { delete m_dy10s; m_dy10s = NULL;}
//DEL 		if( m_dx100s != NULL) { delete m_dx100s; m_dx100s = NULL;}
//DEL 		if( m_dy100s != NULL) { delete m_dy100s; m_dy100s = NULL;}
//DEL 		if( m_dx_i1 != NULL) { delete m_dx_i1; m_dx_i1 = NULL;}
//DEL 		if( m_dy_i1 != NULL) { delete m_dy_i1; m_dy_i1 = NULL;}
//DEL 		if( m_dx_i2 != NULL) { delete m_dx_i2; m_dx_i2 = NULL;}
//DEL 		if( m_dy_i2 != NULL) { delete m_dy_i2; m_dy_i2 = NULL;}
//DEL 		if( m_dx_vpc != NULL) { delete m_dx_vpc; m_dx_vpc = NULL;}
//DEL 		if( m_dy_vpc != NULL) { delete m_dy_vpc; m_dy_vpc = NULL;}
//DEL 		if( m_dx_aa != NULL) { delete m_dx_aa; m_dx_aa = NULL;}
//DEL 		if( m_dy_aa != NULL) { delete m_dy_aa; m_dy_aa = NULL;}
//DEL 		if( m_dx_t1 != NULL) { delete m_dx_t1; m_dx_t1 = NULL;}
//DEL 		if( m_dy_t1 != NULL) { delete m_dy_t1; m_dy_t1 = NULL;}
//DEL 		if( m_dx_t2 != NULL) { delete m_dx_t2; m_dx_t2 = NULL;}
//DEL 		if( m_dy_t2 != NULL) { delete m_dy_t2; m_dy_t2 = NULL;}
//DEL     if( m_dx_t3 != NULL) { delete m_dx_t3; m_dx_t3 = NULL;}
//DEL 		if( m_dy_t3 != NULL) { delete m_dy_t3; m_dy_t3 = NULL;}
//DEL 		if( m_dx_tsa != NULL) { delete m_dx_tsa; m_dx_tsa = NULL;}
//DEL 		if( m_dy_tsa != NULL) { delete m_dy_tsa; m_dy_tsa = NULL;}
//DEL 	 
//DEL 		//заказываем память под измерения
//DEL 		m_dx100m = new double[m_dn100m];
//DEL 		m_dy100m = new double[m_dn100m];
//DEL 		m_dx1s = new double[m_dn100m];
//DEL 		m_dy1s = new double[m_dn100m];
//DEL 		m_dx10s = new double[m_dn100m];
//DEL 		m_dy10s = new double[m_dn100m];
//DEL 		m_dx100s = new double[m_dn100m];
//DEL 		m_dy100s = new double[m_dn100m];
//DEL 		m_dx_i1 = new double[m_dn100m];
//DEL 		m_dy_i1 = new double[m_dn100m];
//DEL 		m_dx_i2	= new double[m_dn100m];
//DEL 		m_dy_i2 = new double[m_dn100m];
//DEL 		m_dx_vpc = new double[m_dn100m];
//DEL 		m_dy_vpc = new double[m_dn100m];
//DEL 		m_dx_aa	= new double[m_dn100m];
//DEL 		m_dy_aa = new double[m_dn100m];
//DEL 		m_dx_t1	= new double[m_dn100m];
//DEL 		m_dy_t1 = new double[m_dn100m];
//DEL 		m_dx_t2	= new double[m_dn100m];
//DEL 		m_dy_t2 = new double[m_dn100m];
//DEL     m_dx_t3	= new double[m_dn100m];
//DEL 		m_dy_t3 = new double[m_dn100m];
//DEL 		m_dx_tsa = new double[m_dn100m];
//DEL 		m_dy_tsa = new double[m_dn100m];
//DEL 
//DEL 		m_dn_i1 = m_dn_i2 = m_dn_vpc = m_dn_aa =
//DEL 		m_dn_t1 = m_dn_t2 = m_dn_t3 = m_dn_tsa = m_dn100m;
//DEL 
//DEL 		double d_global_time = 0.;
//DEL 	
//DEL 		double w100m = 0., t100m = 0.;
//DEL 		int n100m = 0;
//DEL 
//DEL 		double w1s = 0., t1s = 0.;
//DEL 		int n1s = 0;
//DEL 		m_dn1s = 0;
//DEL 
//DEL 		double w10s = 0., t10s = 0.;
//DEL 		int n10s = 0;
//DEL 		m_dn10s = 0;
//DEL 
//DEL 		double w100s = 0., t100s = 0.;
//DEL 		int n100s = 0;
//DEL 		m_dn100s = 0;
//DEL 
//DEL 		double i1 = 0.;
//DEL 		int n_i1_s = 0, n_i1_n = 0;
//DEL 
//DEL 		double i2 = 0.;
//DEL 		int n_i2_s = 0, n_i2_n = 0;
//DEL 
//DEL 		double vpc = 0.;
//DEL 		int n_vpc_s = 0, n_vpc_n = 0;
//DEL 
//DEL 		double aa = 0.;
//DEL 		int n_aa_s = 0, n_aa_n = 0;
//DEL 
//DEL 		double temp1 = 0.;
//DEL 		int n_temp1_s = 0, n_temp1_n = 0;
//DEL 
//DEL 		double temp2 = 0.;
//DEL 		int n_temp2_s = 0, n_temp2_n = 0;
//DEL 
//DEL     double temp3 = 0.;
//DEL 		int n_temp3_s = 0, n_temp3_n = 0;
//DEL 
//DEL 		double tsa = 0.;
//DEL 		int n_tsa_s = 0, n_tsa_n = 0;
//DEL 
//DEL 		fseek( fh, 0, SEEK_SET);
//DEL 
//DEL 		while( 1) {
//DEL 			float f1, f2;
//DEL 			int int1, int2, int3;
//DEL 			fscanf(fh, "%f\t%f\t%d\t%d\t%d\n", &f1, &f2, &int1, &int2, &int3);
//DEL 		
//DEL 			double p_phi = f1 * theApp.GetSettings()->GetScaleCoeff() / 4.;
//DEL 			double p_tsa = int3 / 32768.;
//DEL 			switch( int1 & 0x0F) {
//DEL 				case 0: temp1 += int2; n_temp1_s++; break;	//Temp1
//DEL 				case 1: temp2 += int2; n_temp2_s++; break;	//Temp2
//DEL         case 2: temp3 += int3; n_temp3_s++; break;	//Temp3
//DEL 
//DEL 				case 3: i1 += int2;	n_i1_s++; break;			//i1
//DEL 				case 4: i2 += int2;	n_i2_s++; break;			//i2
//DEL 				case 5: vpc += int2;	n_vpc_s++; break;		//vpc
//DEL 				case 6: aa += int2;	n_aa_s++; break;			//aa
//DEL 
//DEL 				case 16: //software version
//DEL 					/*( ( CSlg2App *) AfxGetApp())->m_btMajVer = ( int2 & 0xFF);
//DEL 					( ( CSlg2App *) AfxGetApp())->m_btMinVer = (( int2 & 0xFF00) >> 8);*/
//DEL 					( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer.Format( _T("%d.%d.%d.%d"),
//DEL 							( int2 & 0xF000) >> 12,
//DEL 							( int2 & 0x0F00) >> 8,
//DEL 							( int2 & 0x00F0) >> 4,
//DEL 							( int2 & 0x000F));	
//DEL 				break;
//DEL 			}
//DEL 
//DEL 			tsa += p_tsa;			n_tsa_s++;
//DEL 
//DEL 			w100m += p_phi;		t100m += p_tsa;
//DEL 			w1s += p_phi;			t1s += p_tsa;
//DEL 			w10s += p_phi;		t10s += p_tsa;
//DEL 			w100s += p_phi;		t100s += p_tsa;
//DEL 		
//DEL 		
//DEL 
//DEL 			d_global_time += p_tsa;
//DEL 		
//DEL 			if( t100m > 0.1) {
//DEL 				double w100 = w100m / t100m;
//DEL 				//100 ms points
//DEL 				m_dx100m[n100m] = d_global_time;
//DEL 				m_dy100m[n100m] = w100;
//DEL 				n100m++;
//DEL 				w100m = t100m = 0.;
//DEL 
//DEL 				//i1 points
//DEL 				m_dx_i1[ n_i1_n] = d_global_time;
//DEL 				m_dy_i1[ n_i1_n] = i1 / n_i1_s / 4096. * 3. / 3.973;
//DEL 				n_i1_n++;
//DEL 				i1 = 0.; n_i1_s = 0;
//DEL 
//DEL 				//i2 points
//DEL 				m_dx_i2[ n_i2_n] = d_global_time;
//DEL 				m_dy_i2[ n_i2_n] = i2 / n_i2_s / 4096. * 3. / 3.973;
//DEL 				n_i2_n++;
//DEL 				i2 = 0.; n_i2_s = 0;
//DEL 			
//DEL 				//vpc points
//DEL 				m_dx_vpc[ n_vpc_n] = d_global_time;
//DEL 				m_dy_vpc[ n_vpc_n] = (( vpc / n_vpc_s / 4096. * 3.) - 2.048) * 100.;
//DEL 				n_vpc_n++;
//DEL 				vpc = 0.; n_vpc_s = 0;
//DEL 
//DEL 				//aa points
//DEL 				m_dx_aa[ n_aa_n] = d_global_time;
//DEL 				m_dy_aa[ n_aa_n] = ( aa / n_aa_s / 4096. * 3.) / 0.5;
//DEL 				n_aa_n++;
//DEL 				aa = 0.; n_aa_s = 0;
//DEL 
//DEL 				//t1 points
//DEL 				m_dx_t1[ n_temp1_n] = d_global_time;
//DEL 				m_dy_t1[ n_temp1_n] = 70.24586 * ( temp1 / n_temp1_s / 4096. * 3.) - 128.209;
//DEL 				//m_dy_t1[ n_temp1_n] = ( temp1 / n_temp1_s / 4096. * 3.);
//DEL 				n_temp1_n++;
//DEL 				temp1 = 0.; n_temp1_s = 0;
//DEL 
//DEL 				//t2 points
//DEL 				m_dx_t2[ n_temp2_n] = d_global_time;
//DEL 				m_dy_t2[ n_temp2_n] = 70.24586 * ( temp2 / n_temp2_s / 4096. * 3.) - 128.209;
//DEL 				//m_dy_t2[ n_temp2_n] = ( temp2 / n_temp2_s / 4096. * 3.);
//DEL 				n_temp2_n++;
//DEL 				temp2 = 0.; n_temp2_s = 0;
//DEL 
//DEL         //t3 points
//DEL 				m_dx_t3[ n_temp3_n] = d_global_time;
//DEL 				m_dy_t3[ n_temp3_n] = 70.24586 * ( temp3 / n_temp3_s / 4096. * 3.) - 128.209;
//DEL 				n_temp3_n++;
//DEL 				temp3 = 0.; n_temp3_s = 0;
//DEL 
//DEL 				//tsa points
//DEL 				m_dx_tsa[ n_tsa_n] = d_global_time;
//DEL 				m_dy_tsa[ n_tsa_n] = tsa / n_tsa_s;
//DEL 				n_tsa_n++;
//DEL 				tsa = 0.; n_tsa_s = 0;
//DEL 			}
//DEL 
//DEL 			//1-секундные точки
//DEL 			if( t1s > 1.) {
//DEL 				m_dx1s[n1s] = d_global_time;
//DEL 				m_dy1s[n1s] = w1s / t1s;
//DEL 				n1s++;
//DEL 				m_dn1s += 1.;
//DEL 				w1s = 0.; t1s = 0.;
//DEL 			}
//DEL 
//DEL 			//10-секундные точки
//DEL 			if( t10s > 10.) {
//DEL 				m_dx10s[n10s] = d_global_time;
//DEL 				m_dy10s[n10s] = w10s / t10s;
//DEL 				n10s++;
//DEL 				m_dn10s += 1.;
//DEL 				w10s = 0.; t10s = 0.;
//DEL 			}
//DEL 
//DEL 			//100-секундные точки
//DEL 			if( t100s > 100.) {
//DEL 				m_dx100s[n100s] = d_global_time;
//DEL 				m_dy100s[n100s] = w100s / t100s;
//DEL 				n100s++;
//DEL 				m_dn100s += 1.;
//DEL 
//DEL 
//DEL 				w100s = 0.; t100s = 0.;
//DEL 			}
//DEL 
//DEL 			if( dlg != NULL)
//DEL 				if( !( n100m % 50))
//DEL 					dlg->m_ctlProgress1.SetPos( ftell( fh));
//DEL 
//DEL 			if( feof( fh))
//DEL 				break;
//DEL 		}
//DEL 	} //endif (it's old ascii stat file)
//DEL 
//DEL 	fclose( fh);
//DEL 	/*fclose( fh_test_needs);*/
//DEL 
//DEL 	RecalculateStatistics();
//DEL }

void CSlg2Doc::AllocMem(long lPacks, long l100ms)
{
  
  m_dpW.AllocMem( lPacks, l100ms);
  m_dpI1.AllocMem( lPacks, l100ms);
  m_dpI2.AllocMem( lPacks, l100ms);
  m_dpVpc.AllocMem( lPacks, l100ms);
  m_dpAAa.AllocMem( lPacks, l100ms);
  m_dpAAd.AllocMem( lPacks, l100ms);
  m_dpAAr.AllocMem( lPacks, l100ms);
  m_dpT1.AllocMem( lPacks, l100ms);
  m_dpT2.AllocMem( lPacks, l100ms);
  m_dpT3.AllocMem( lPacks, l100ms);
  m_dpTsa.AllocMem( lPacks, l100ms);
}
