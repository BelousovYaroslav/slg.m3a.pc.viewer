// OpenMeasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "OpenMeasDlg.h"
#include "DlgSelectMcVer.h"
#include "PackProcessing.h"

#include "MainFrm.h"
#include "MainView.h"
#include "Slg2Doc.h"

extern CSlg2App theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SET_FILELEN      ( WM_USER + 1)
#define WM_SET_FILEPOS      ( WM_USER + 2)
#define WM_SET_MF           ( WM_USER + 3)
#define WM_SET_CF           ( WM_USER + 4)
#define WM_SET_CSF          ( WM_USER + 5)
#define WM_SET_PACKS        ( WM_USER + 6)
#define WM_SET_MC_VERSION   ( WM_USER + 7)
#define WM_SET_TIME         ( WM_USER + 8)
#define WM_SET_FILEPOS2     ( WM_USER + 9)

#define TIMER_REFRESH_VALUES1 1001
#define TIMER_REFRESH_VALUES2 1002

/////////////////////////////////////////////////////////////////////////////
// Поток определения числа посылок в файле
BOOL gl_bLoadFile1ThreadWork;
BOOL gl_bStopLoadThread1;

DWORD WINAPI LoadFile1Thread(LPVOID lparam)
{
  gl_bStopLoadThread1 = FALSE;
  gl_bLoadFile1ThreadWork = TRUE;
  COpenMeasDlg *pParent = ( COpenMeasDlg *) lparam;
  //открываем файл измрения
  FILE *fh = fopen( pParent->m_strOpenFilePathName, _T("rb"));
  
  //перематываем файл измерения в его конец
  fseek( fh, 0, SEEK_END);
  
  //узнаём длину, и передаём её в окно прогресса
  long lLen = ftell( fh);
  SendMessage( pParent->m_hWnd, WM_SET_FILELEN, 0, lLen);

  //перематываем файл в начало
  fseek( fh, 0, SEEK_SET);

  long lPacks = 0;
  int nMarkerFails = 0;
  int nCheckSummFails = 0;
  int nCounterFails = 0;
  int btPrevPackCounter = 500;
  long lTimeMillis = 0.;

  //поехали
  do {

    unsigned char bt1, bt2, bt3, bt4, bt5, bt6, bt7, bt8, bt9, bt10, bt11, bt12, bt13, bt14;
    unsigned char btCheckSumm;

    long lPos;

    //выискиваем последовательность маркера 0x55, 0xAA
    int nMarkerCounter = 0;
    bool bMarkerFailOnce = true;
    do {

      if( gl_bStopLoadThread1 == TRUE)
        break;

      switch( nMarkerCounter) {
        case 0:
          bt1 = fgetc( fh);
          if( bt1 == 0x55)
            nMarkerCounter = 1;
          else {
            if( feof( fh) == 0) {
              if( bMarkerFailOnce) {
              
                nMarkerFails++;
                pParent->SendMessage( WM_SET_MF, nMarkerFails, 0);
              
                bMarkerFailOnce = false;
              }
            }
          }
        break;

        case 1:
          bt2 = fgetc( fh);
          if( bt2 == 0xAA) {
            nMarkerCounter = 2;
          }
          else {
            nMarkerCounter = 0;

            if( feof( fh) == 0) {
              if( bMarkerFailOnce) {
                nMarkerFails++;
                pParent->SendMessage( WM_SET_MF, nMarkerFails, 0);

                bMarkerFailOnce = false;
              }
            }
          }
        break;
      }
      
      lPos = ftell( fh);
      pParent->SendMessage( WM_SET_FILEPOS, 0, lPos);

    } while( feof( fh) == 0 && nMarkerCounter != 2);

    if( feof( fh) != 0)
      break;

    if( gl_bStopLoadThread1 == TRUE)
      break;

    //ПРИРАЩЕНИЕ УГЛА: 4 байта
    bt3 = fgetc( fh);
    bt4 = fgetc( fh);
    bt5 = fgetc( fh);
    bt6 = fgetc( fh);

    //НОМЕР ЧЕРЕДУЮЩЕГОСЯ (ТЕХНОЛОГИЧЕСКОГО, АНАЛОГОВОГО) ПАРАМЕТРА. 1 байт
    bt7 = fgetc( fh);

    //ЗНАЧЕНИЕ ТЕХНОЛОГИЧЕСКОГО (АНАЛОГОВОГО) ПАРАМЕТРА. 2 Байта
    bt8 = fgetc( fh);
    bt9 = fgetc( fh);

    //SA TIMING.
    //ИНТЕРВАЛ ВРЕМЕНИ МЕЖДУ ТЕКУЩИМ И ПРЕДЫДУЩИМ МОМЕНТАМИ ФИКСАЦИИ ПАРАМЕТРОВ. 2 БАЙТА
    bt10 = fgetc( fh);
    bt11 = fgetc( fh);

    //ПОРЯДКОВЫЙ НОМЕР СООБЩЕНИЯ. 1 БАЙТ
    bt12 = fgetc( fh);

    //EMERGENCY CODE
    //КОД ОШИБКИ. 1 БАЙТ
    bt13 = fgetc( fh);

    //CHEKSUMM
    //КОНТРОЛЬНАЯ СУММА, CS. 1 байт
    bt14 = fgetc( fh);

    if( feof( fh) != 0)
      break;

    btCheckSumm  = bt3;
    btCheckSumm += bt4;
    btCheckSumm += bt5;
    btCheckSumm += bt6;
    btCheckSumm += bt7;
    btCheckSumm += bt8;
    btCheckSumm += bt9;
    btCheckSumm += bt10;
    btCheckSumm += bt11;
    btCheckSumm += bt12;
    btCheckSumm += bt13;

    if( ( btCheckSumm % 256) != bt14) {
      if( feof( fh) == 0) {
        nCheckSummFails++;
        pParent->SendMessage( WM_SET_CSF, nCheckSummFails, 0);
      }
    }
    else {
      lPacks++;
      pParent->SendMessage( WM_SET_PACKS, 0, lPacks);


      lTimeMillis += ( long) (
                    ( ( double) ( bt11 * 16 + bt10))
                                  / 32768. * 1000.);      
      pParent->SendMessage( WM_SET_TIME, 0, lTimeMillis);
      
      if( bt7 == 43) {//VERSION) {
        pParent->SendMessage( WM_SET_MC_VERSION, bt8, bt9);
      }

      if( btPrevPackCounter == 500) {
        btPrevPackCounter = bt12;
      }
      else {
        if( ( ( btPrevPackCounter + 1) % 256) != bt12) {
          if( feof( fh) == 0) {
            nCounterFails++;
            pParent->SendMessage( WM_SET_CF, nCounterFails, 0);
          }
        }
        btPrevPackCounter = bt12;
      }
    }

  } while( feof( fh) == 0 && gl_bStopLoadThread1 == FALSE);

  fclose( fh);
  gl_bLoadFile1ThreadWork = FALSE;

  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Поток загрузки файла
BOOL gl_bLoadFile2ThreadWork;
BOOL gl_bStopLoadThread2;

DWORD WINAPI LoadFile2Thread(LPVOID lparam)
{
  gl_bStopLoadThread2 = FALSE;
  gl_bLoadFile2ThreadWork = TRUE;

  CMainFrame *pFrm =  ( CMainFrame *) AfxGetApp()->GetMainWnd();
  CMainView *pView =  ( CMainView *) pFrm->GetActiveView();
  CSlg2Doc *pDoc =    ( CSlg2Doc *) pView->GetDocument();

  COpenMeasDlg *pParent = ( COpenMeasDlg *) lparam;

  //открываем файл измерения
  FILE *fh = fopen( pParent->m_strOpenFilePathName, _T("rb"));
  
  long lPacks = 0;
  int nMarkerFails = 0;
  int nCheckSummFails = 0;
  int nCounterFails = 0;
  int btPrevPackCounter = 500;
  long lTimeMillis = 0.;


  double w100m = 0., t100m = 0.;
	int n100m = 0;

	double w1s = 0., t1s = 0.;
	int n1s = 0;
	pDoc->m_dn1s = 0;

	double w10s = 0., t10s = 0.;
	int n10s = 0;
	pDoc->m_dn10s = 0;

	double w100s = 0., t100s = 0.;
	int n100s = 0;
	pDoc->m_dn100s = 0;

	double i1 = 0.;
	int n_i1_s = 0, n_i1_n = 0;

	double i2 = 0.;
	int n_i2_s = 0, n_i2_n = 0;

	double vpc = 0.;
	int n_vpc_s = 0, n_vpc_n = 0;

	double aa = 0.;
	int n_aa_s = 0, n_aa_n = 0;

	double temp1 = 0.;
	int n_temp1_s = 0, n_temp1_n = 0;

	double temp2 = 0.;
	int n_temp2_s = 0, n_temp2_n = 0;

  double temp3 = 0.;
	int n_temp3_s = 0, n_temp3_n = 0;

  double tsa = 0.;
  int n_tsa_s = 0, n_tsa_n = 0;

  double d_global_time = 0.;

  //поехали
  do {

    unsigned char bt1, bt2, bt3, bt4, bt5, bt6, bt7, bt8, bt9, bt10, bt11, bt12, bt13, bt14;
    unsigned char btCheckSumm;

    long lPos;

    //выискиваем последовательность маркера 0x55, 0xAA
    int nMarkerCounter = 0;
    bool bMarkerFailOnce = true;
    do {

      if( gl_bStopLoadThread1 == TRUE)
        break;

      switch( nMarkerCounter) {
        case 0:
          bt1 = fgetc( fh);
          if( bt1 == 0x55)
            nMarkerCounter = 1;
          else {
            if( feof( fh) == 0) {
              if( bMarkerFailOnce) {
              
                nMarkerFails++;
                pParent->SendMessage( WM_SET_MF, nMarkerFails, 0);
              
                bMarkerFailOnce = false;
              }
            }
          }
        break;

        case 1:
          bt2 = fgetc( fh);
          if( bt2 == 0xAA) {
            nMarkerCounter = 2;
          }
          else {
            nMarkerCounter = 0;

            if( feof( fh) == 0) {
              if( bMarkerFailOnce) {
                nMarkerFails++;
                pParent->SendMessage( WM_SET_MF, nMarkerFails, 0);

                bMarkerFailOnce = false;
              }
            }
          }
        break;
      }
      
      lPos = ftell( fh);
      pParent->SendMessage( WM_SET_FILEPOS2, 0, lPos);

    } while( feof( fh) == 0 && nMarkerCounter != 2);

    if( feof( fh) != 0)
      break;

    if( gl_bStopLoadThread1 == TRUE)
      break;

    //ПРИРАЩЕНИЕ УГЛА: 4 байта
    bt3 = fgetc( fh);
    bt4 = fgetc( fh);
    bt5 = fgetc( fh);
    bt6 = fgetc( fh);

    //НОМЕР ЧЕРЕДУЮЩЕГОСЯ (ТЕХНОЛОГИЧЕСКОГО, АНАЛОГОВОГО) ПАРАМЕТРА. 1 байт
    bt7 = fgetc( fh);

    //ЗНАЧЕНИЕ ТЕХНОЛОГИЧЕСКОГО (АНАЛОГОВОГО) ПАРАМЕТРА. 2 Байта
    bt8 = fgetc( fh);
    bt9 = fgetc( fh);

    //SA TIMING.
    //ИНТЕРВАЛ ВРЕМЕНИ МЕЖДУ ТЕКУЩИМ И ПРЕДЫДУЩИМ МОМЕНТАМИ ФИКСАЦИИ ПАРАМЕТРОВ. 2 БАЙТА
    bt10 = fgetc( fh);
    bt11 = fgetc( fh);

    //ПОРЯДКОВЫЙ НОМЕР СООБЩЕНИЯ. 1 БАЙТ
    bt12 = fgetc( fh);

    //EMERGENCY CODE
    //КОД ОШИБКИ. 1 БАЙТ
    bt13 = fgetc( fh);

    //CHEKSUMM
    //КОНТРОЛЬНАЯ СУММА, CS. 1 байт
    bt14 = fgetc( fh);

    if( feof( fh) != 0)
      break;

    btCheckSumm  = bt3;
    btCheckSumm += bt4;
    btCheckSumm += bt5;
    btCheckSumm += bt6;
    btCheckSumm += bt7;
    btCheckSumm += bt8;
    btCheckSumm += bt9;
    btCheckSumm += bt10;
    btCheckSumm += bt11;
    btCheckSumm += bt12;
    btCheckSumm += bt13;

    if( ( btCheckSumm % 256) != bt14) {
      if( feof( fh) == 0) {
        nCheckSummFails++;
        pParent->SendMessage( WM_SET_CSF, nCheckSummFails, 0);
      }
    }
    else {
      lPacks++;
      pParent->SendMessage( WM_SET_PACKS, 0, lPacks);


      lTimeMillis += ( long) (
                    ( ( double) ( bt11 * 16 + bt10))
                                  / 32768. * 1000.);      
      pParent->SendMessage( WM_SET_TIME, 0, lTimeMillis);
      
      if( bt7 == 43) {//VERSION) {
        pParent->SendMessage( WM_SET_MC_VERSION, bt8, bt9);
      }

      if( btPrevPackCounter == 500) {
        btPrevPackCounter = bt12;
      }
      else {
        if( ( ( btPrevPackCounter + 1) % 256) != bt12) {
          if( feof( fh) == 0) {
            nCounterFails++;
            pParent->SendMessage( WM_SET_CF, nCounterFails, 0);
          }
        }
        btPrevPackCounter = bt12;
      }

      //РАЗБОР ПАЧКИ
      CPackProcessing pack;
        
      pack.bt3 = bt3;
      pack.bt4 = bt4;
      pack.bt5 = bt5;
      pack.bt6 = bt6;

      pack.bt7 = bt7;

      pack.bt8 = bt8;
      pack.bt9 = bt9;

      pack.bt10 = bt10;
      pack.bt11 = bt11;

      pack.bt12 = bt12;

      pack.bt13 = bt13;
        
      //pack.m_dblDecCoeff = ;

      //if( pParent->m_nVersionMajor == 3 && pParent->m_nVersionMiddle == 2 && pParent->m_nVersionMinor == 3) {
        pack.ProcessPack_323();
      //}

      tsa += pack.m_dblTime;    n_tsa_s++;

      double dblPhi_ang = pack.m_dblPhi;
			w100m +=  dblPhi_ang;		t100m += pack.m_dblTime;
			w1s +=    dblPhi_ang;		t1s += pack.m_dblTime;
			w10s +=   dblPhi_ang;		t10s += pack.m_dblTime;
			w100s +=  dblPhi_ang;		t100s += pack.m_dblTime;

      d_global_time += pack.m_dblTime;

      switch( bt7) {
        case 0: temp1 += pack.m_dblAnParamValue; n_temp1_s++; break;	//Temp1
				case 1: temp2 += pack.m_dblAnParamValue; n_temp2_s++; break;	//Temp2
        case 2: temp3 += pack.m_dblAnParamValue; n_temp3_s++; break;	//Temp3

				case 3: i1 += pack.m_dblAnParamValue;	  n_i1_s++;   break;			//i1
				case 4: i2 += pack.m_dblAnParamValue;	  n_i2_s++;   break;			//i2
				case 5: vpc += pack.m_dblAnParamValue;	n_vpc_s++;  break;		  //vpc
				case 6: aa += pack.m_dblAnParamValue;	  n_aa_s++;   break;			//aa
      }

      /*
      if( n100m >= pParent->m_dn100m) {
        OutputDebugString( "ops");
      }
      */

      if( t100m > 0.1) {
				//100 ms points
				pDoc->m_dx100m[ n100m] = d_global_time;
				pDoc->m_dy100m[ n100m] = w100m / t100m;
        pDoc->m_dn100m++;
				n100m++;
				w100m = t100m = 0.;

				//i1 points
				pDoc->m_dx_i1[ n_i1_n] = d_global_time;
				pDoc->m_dy_i1[ n_i1_n] = i1 / n_i1_s;
				n_i1_n++;
				i1 = 0.; n_i1_s = 0;

				//i2 points
				pDoc->m_dx_i2[ n_i2_n] = d_global_time;
				pDoc->m_dy_i2[ n_i2_n] = i2 / n_i2_s;
				n_i2_n++;
				i2 = 0.; n_i2_s = 0;
			
				//vpc points
				pDoc->m_dx_vpc[ n_vpc_n] = d_global_time;
				pDoc->m_dy_vpc[ n_vpc_n] = vpc / n_vpc_s;
				n_vpc_n++;
				vpc = 0.; n_vpc_s = 0;

				//aa points
				pDoc->m_dx_aa[ n_aa_n] = d_global_time;
				pDoc->m_dy_aa[ n_aa_n] = aa / n_aa_s;
				n_aa_n++;
				aa = 0.; n_aa_s = 0;

				//t1 points
				pDoc->m_dx_t1[ n_temp1_n] = d_global_time;
				pDoc->m_dy_t1[ n_temp1_n] = temp1 / n_temp1_s;
				n_temp1_n++;
				temp1 = 0.; n_temp1_s = 0;

				//t2 points
				pDoc->m_dx_t2[ n_temp2_n] = d_global_time;
				pDoc->m_dy_t2[ n_temp2_n] = temp2 / n_temp2_s;
				//m_dy_t2[ n_temp2_n] = 70.24586 * ( temp2 / n_temp2_s / 4096. * 3.) - 128.209;
				//m_dy_t2[ n_temp2_n] = ( temp2 / n_temp2_s / 4096. * 3.);
				n_temp2_n++;
				temp2 = 0.; n_temp2_s = 0;

        //t3 points
				pDoc->m_dx_t3[ n_temp3_n] = d_global_time;
				pDoc->m_dy_t3[ n_temp3_n] = temp3 / n_temp3_s;
				n_temp3_n++;
				temp3 = 0.; n_temp3_s = 0;

				//tsa points
				pDoc->m_dx_tsa[ n_tsa_n] = d_global_time;
				pDoc->m_dy_tsa[ n_tsa_n] = tsa / n_tsa_s;
				n_tsa_n++;
				tsa = 0.; n_tsa_s = 0;
			}

			//1-секундные точки
			if( t1s > 1.) {
				pDoc->m_dx1s[n1s] = d_global_time;
				pDoc->m_dy1s[n1s] = w1s / t1s;
				n1s++;
				pDoc->m_dn1s += 1.;
				w1s = 0.; t1s = 0.;
			}

			//10-секундные точки
			if( t10s > 10.) {
				pDoc->m_dx10s[n10s] = d_global_time;
				pDoc->m_dy10s[n10s] = w10s / t10s;
				n10s++;
				pDoc->m_dn10s += 1.;
				w10s = 0.; t10s = 0.;
			}

			//100-секундные точки
			if( t100s > 100.) {
				pDoc->m_dx100s[ n100s] = d_global_time;
				pDoc->m_dy100s[ n100s] = w100s / t100s;
				n100s++;
				pDoc->m_dn100s += 1.;
				w100s = 0.; t100s = 0.;
			}
    }

  } while( feof( fh) == 0 && gl_bStopLoadThread1 == FALSE);

  pDoc->m_dn_i1 = pDoc->m_dn_i2 = pDoc->m_dn_vpc = pDoc->m_dn_aa = pDoc->m_dn_t1 = pDoc->m_dn_t2 = pDoc->m_dn_t3 = pDoc->m_dn_tsa = pDoc->m_dn100m;
  pDoc->RecalculateStatistics();

  fclose( fh);

  gl_bLoadFile2ThreadWork = FALSE;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// COpenMeasDlg dialog


COpenMeasDlg::COpenMeasDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenMeasDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenMeasDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COpenMeasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenMeasDlg)
	DDX_Control(pDX, IDC_PRGR2, m_ctlProgress2);
	DDX_Control(pDX, IDC_PRGR, m_ctlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenMeasDlg, CDialog)
	//{{AFX_MSG_MAP(COpenMeasDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP_LOAD, OnBtnStopLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenMeasDlg message handlers

BOOL COpenMeasDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  //ЗАПУСК ПОТОКА ДЛЯ ОБРАБОТКИ ДАННЫХ
  DWORD id2;
  HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile1Thread, this, 0, &id2);

  m_lPacks = 0;
  m_nMarkerFails = 0;
  m_nCounterFails = 0;
  m_nCheckSummFails = 0;

  m_nVersionMajor = -1;
  m_nVersionMiddle = -1;
  m_nVersionMinor = -1;

  SetTimer( TIMER_REFRESH_VALUES1, 1000, NULL);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT COpenMeasDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  switch( message) {
    case WM_SET_FILELEN:
      m_lFileLen = lParam;
      m_ctlProgress.SetRange32( 0, m_lFileLen);
      m_ctlProgress2.SetRange32( 0, m_lFileLen);
    break;
    case WM_SET_FILEPOS:
      m_lFilePos = lParam;
      m_ctlProgress.SetPos( m_lFilePos);
    break;
    case WM_SET_FILEPOS2:
      m_lFilePos = lParam;
      m_ctlProgress2.SetPos( m_lFilePos);
    break;
    case WM_SET_MF:
      m_nMarkerFails = wParam;
    break;
    case WM_SET_CF:
      m_nCounterFails = wParam;
    break;
    case WM_SET_CSF:
      m_nCheckSummFails = wParam;
    break;
    case WM_SET_PACKS:
      m_lPacks = lParam;
    break;
    case WM_SET_TIME:
      m_lTimeMillis = lParam;
    break;
    case WM_SET_MC_VERSION:
      m_nVersionMajor =  wParam / 16;  //major
      m_nVersionMiddle = wParam % 16;	//middle
      m_nVersionMinor =  lParam / 16;	//minor
      theApp.m_strSoftwareVer.Format( "%d.%d.%d", m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);
    break;
  }

  return CDialog::WindowProc(message, wParam, lParam);
}

void COpenMeasDlg::OnTimer(UINT nIDEvent)
{
  CString strTmp;

  switch( nIDEvent) {
    case TIMER_REFRESH_VALUES1: {
      int nHou = ( int) ( m_lTimeMillis / 1000. / 60. / 60.);
      int nMin = (( int) ( m_lTimeMillis / 1000. / 60) % 60);
      int nSec = (( int) (( m_lTimeMillis / 1000) % 60) % 60);
      int nMsec = ( int) ( m_lTimeMillis % 1000);

      if( m_nVersionMajor == -1 || m_nVersionMiddle == -1 || m_nVersionMinor == -1) {
        strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d   MC_VERSION: unknown",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails);
      }
      else {
        strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d   MC_VERSION: %x.%x.%x",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails,
                m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);
      }
      GetDlgItem( IDC_LBL_SLIDER1)->SetWindowText( strTmp);

      if( gl_bLoadFile1ThreadWork == FALSE) {
        //закончил работу поток первого прохода

        KillTimer( TIMER_REFRESH_VALUES1);

        if( gl_bStopLoadThread1 == TRUE) {
          //его прервали - выходим по CANCEL
          OnCancel();
        }
        else {
          
          //он закончил работу сам!
          if( m_nVersionMajor == -1 || m_nVersionMiddle == -1 || m_nVersionMinor == -1) {
            
            //если мы не смогли автоматичсеки определить версию ПО микроконтроллера - попросим указать её вручную
            CDlgSelectMcVer dlg;
            if( dlg.DoModal() != IDOK) {
              SetTimer( TIMER_REFRESH_VALUES2, 1000, NULL);
              break;
            }

            switch( dlg.m_nRadSelect) {
              case 0: //3.1.8
                m_nVersionMajor = 3; m_nVersionMiddle = 1; m_nVersionMinor = 8;
              break;
              case 1: //3.1.9
                m_nVersionMajor = 3; m_nVersionMiddle = 1; m_nVersionMinor = 9;
              break;
              case 2: //3.2.2
                m_nVersionMajor = 3; m_nVersionMiddle = 2; m_nVersionMinor = 2;
              break;

              default: //3.2.3
                m_nVersionMajor = 3; m_nVersionMiddle = 2; m_nVersionMinor = 3;
              break;
            }
          }
          //версия ПО микроконтроллера известна

          //заказываем массивы под точки
          CMainFrame *pFrm = ( CMainFrame *) AfxGetApp()->GetMainWnd();
          CMainView *pView = ( CMainView *) pFrm->GetActiveView();
          CSlg2Doc *pDoc = (CSlg2Doc *) pView->GetDocument();
          
          //высвобождаем если что либо заказано (что-то было открыто)
		      if( pDoc->m_dx100m != NULL) { delete pDoc->m_dx100m;  pDoc->m_dx100m = NULL;}
		      if( pDoc->m_dy100m != NULL) { delete pDoc->m_dy100m;  pDoc->m_dy100m = NULL;}
		      if( pDoc->m_dx1s != NULL)   { delete pDoc->m_dx1s;    pDoc->m_dx1s = NULL;}
		      if( pDoc->m_dy1s != NULL)   { delete pDoc->m_dy1s;    pDoc->m_dy1s = NULL;}
		      if( pDoc->m_dx10s != NULL)  { delete pDoc->m_dx10s;   pDoc->m_dx10s = NULL;}
		      if( pDoc->m_dy10s != NULL)  { delete pDoc->m_dy10s;   pDoc->m_dy10s = NULL;}
		      if( pDoc->m_dx100s != NULL) { delete pDoc->m_dx100s;  pDoc->m_dx100s = NULL;}
		      if( pDoc->m_dy100s != NULL) { delete pDoc->m_dy100s;  pDoc->m_dy100s = NULL;}
		      if( pDoc->m_dx_i1 != NULL)  { delete pDoc->m_dx_i1;   pDoc->m_dx_i1 = NULL;}
		      if( pDoc->m_dy_i1 != NULL)  { delete pDoc->m_dy_i1;   pDoc->m_dy_i1 = NULL;}
		      if( pDoc->m_dx_i2 != NULL)  { delete pDoc->m_dx_i2;   pDoc->m_dx_i2 = NULL;}
		      if( pDoc->m_dy_i2 != NULL)  { delete pDoc->m_dy_i2;   pDoc->m_dy_i2 = NULL;}
		      if( pDoc->m_dx_vpc != NULL) { delete pDoc->m_dx_vpc;  pDoc->m_dx_vpc = NULL;}
		      if( pDoc->m_dy_vpc != NULL) { delete pDoc->m_dy_vpc;  pDoc->m_dy_vpc = NULL;}
		      if( pDoc->m_dx_aa != NULL)  { delete pDoc->m_dx_aa;   pDoc->m_dx_aa = NULL;}
		      if( pDoc->m_dy_aa != NULL)  { delete pDoc->m_dy_aa;   pDoc->m_dy_aa = NULL;}
		      if( pDoc->m_dx_t1 != NULL)  { delete pDoc->m_dx_t1;   pDoc->m_dx_t1 = NULL;}
		      if( pDoc->m_dy_t1 != NULL)  { delete pDoc->m_dy_t1;   pDoc->m_dy_t1 = NULL;}
		      if( pDoc->m_dx_t2 != NULL)  { delete pDoc->m_dx_t2;   pDoc->m_dx_t2 = NULL;}
		      if( pDoc->m_dy_t2 != NULL)  { delete pDoc->m_dy_t2;   pDoc->m_dy_t2 = NULL;}
          if( pDoc->m_dx_t3 != NULL)  { delete pDoc->m_dx_t3;   pDoc->m_dx_t3 = NULL;}
          if( pDoc->m_dy_t3 != NULL)  { delete pDoc->m_dy_t3;   pDoc->m_dy_t3 = NULL;}
		      if( pDoc->m_dx_tsa != NULL) { delete pDoc->m_dx_tsa;  pDoc->m_dx_tsa = NULL;}
		      if( pDoc->m_dy_tsa != NULL) { delete pDoc->m_dy_tsa;  pDoc->m_dy_tsa = NULL;}
	 
          m_dn100m = m_lTimeMillis / 10;
          long dn100m = m_lTimeMillis / 10;

		      //заказываем память под измерения
		      pDoc->m_dx100m =  new double[ dn100m];
		      pDoc->m_dy100m =  new double[ dn100m];
		      pDoc->m_dx1s =    new double[ dn100m];
		      pDoc->m_dy1s =    new double[ dn100m];
		      pDoc->m_dx10s =   new double[ dn100m];
		      pDoc->m_dy10s =   new double[ dn100m];
		      pDoc->m_dx100s =  new double[ dn100m];
		      pDoc->m_dy100s =  new double[ dn100m];
		      pDoc->m_dx_i1 =   new double[ dn100m];
		      pDoc->m_dy_i1 =   new double[ dn100m];
		      pDoc->m_dx_i2	=   new double[ dn100m];
		      pDoc->m_dy_i2 =   new double[ dn100m];
		      pDoc->m_dx_vpc =  new double[ dn100m];
		      pDoc->m_dy_vpc =  new double[ dn100m];
		      pDoc->m_dx_aa	=   new double[ dn100m];
		      pDoc->m_dy_aa =   new double[ dn100m];
		      pDoc->m_dx_t1	=   new double[ dn100m];
		      pDoc->m_dy_t1 =   new double[ dn100m];
		      pDoc->m_dx_t2	=   new double[ dn100m];
		      pDoc->m_dy_t2 =   new double[ dn100m];
          pDoc->m_dx_t3	=   new double[ dn100m];
		      pDoc->m_dy_t3 =   new double[ dn100m];
		      pDoc->m_dx_tsa =  new double[ dn100m];
		      pDoc->m_dy_tsa =  new double[ dn100m];

          //начинаем вторую пробежку
          DWORD id2;
          HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile2Thread, this, 0, &id2);
          
          SetTimer( TIMER_REFRESH_VALUES2, 1000, NULL);
        }
      }
    }
    break;

    case TIMER_REFRESH_VALUES2:
      int nHou = ( int) ( m_lTimeMillis / 1000. / 60. / 60.);
      int nMin = (( int) ( m_lTimeMillis / 1000. / 60) % 60);
      int nSec = (( int) (( m_lTimeMillis / 1000) % 60) % 60);
      int nMsec = ( int) ( m_lTimeMillis % 1000);

      if( m_nVersionMajor == -1 || m_nVersionMiddle == -1 || m_nVersionMinor == -1) {
        strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d   MC_VERSION: unknown",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails);
      }
      else {
        strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d   MC_VERSION: %x.%x.%x",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails,
                m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);
      }
      GetDlgItem( IDC_LBL_SLIDER2)->SetWindowText( strTmp);

      if( gl_bLoadFile2ThreadWork == FALSE) {
        //закончил работу поток второго прохода
        KillTimer( TIMER_REFRESH_VALUES2);
        if( gl_bStopLoadThread1 == TRUE) {
          //его прервали - выходим по CANCEL
          OnCancel();
        }
        else {
          //он закончил работу сам!
          OnOK();
        }
      }
    break;

  }
  CDialog::OnTimer(nIDEvent);
}

void COpenMeasDlg::OnBtnStopLoad()
{
  gl_bStopLoadThread1 = TRUE;
  gl_bStopLoadThread2 = TRUE;
}
