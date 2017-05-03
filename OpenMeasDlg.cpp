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

#include "AnalogueParamsConstList.h"

extern CSlg2App theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SET_FILELEN      ( WM_USER + 1)
#define WM_SET_FILEPOS1     ( WM_USER + 2)
#define WM_SET_MF           ( WM_USER + 3)
#define WM_SET_CF           ( WM_USER + 4)
#define WM_SET_CSF          ( WM_USER + 5)
#define WM_SET_PACKS        ( WM_USER + 6)
#define WM_SET_MC_VERSION   ( WM_USER + 7)
//#define WM_SET_TIME         ( WM_USER + 8)
#define WM_SET_FILEPOS2     ( WM_USER + 9)
#define WM_SET_FILEPOS0     ( WM_USER + 10)
#define WM_SET_STEP1INFO    ( WM_USER + 11)

#define TIMER_REFRESH_VALUES0 1001
#define TIMER_REFRESH_VALUES1 1002
#define TIMER_REFRESH_VALUES2 1003

/////////////////////////////////////////////////////////////////////////////
// ПОТОК 0
// Поток определения версии прошивки прибора
BOOL gl_bLoadFile0ThreadWork;
BOOL gl_bStopLoadThread0;

DWORD WINAPI LoadFile0Thread(LPVOID lparam)
{
  gl_bStopLoadThread0 = FALSE;
  gl_bLoadFile0ThreadWork = TRUE;

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

      if( gl_bStopLoadThread0 == TRUE)
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
      pParent->SendMessage( WM_SET_FILEPOS0, 0, lPos);

    } while( feof( fh) == 0 && nMarkerCounter != 2);

    if( feof( fh) != 0)
      break;

    if( gl_bStopLoadThread0 == TRUE)
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


      if( bt7 == 43) {//VERSION) {
        pParent->SendMessage( WM_SET_MC_VERSION, bt8, bt9);
        break;
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

  } while( feof( fh) == 0 && gl_bStopLoadThread0 == FALSE);

  fclose( fh);
  gl_bLoadFile0ThreadWork = FALSE;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ПОТОК 1
// Поток определения числа посылок (временной длительности) файла измерения
BOOL gl_bLoadFile1ThreadWork;
BOOL gl_bStopLoadThread1;
Step1Info gl_InfoStep1;

DWORD WINAPI LoadFile1Thread(LPVOID lparam)
{
  gl_bStopLoadThread1 = FALSE;
  gl_bLoadFile1ThreadWork = TRUE;
  COpenMeasDlg *pParent = ( COpenMeasDlg *) lparam;
  //открываем файл измрения
  FILE *fh = fopen( pParent->m_strOpenFilePathName, _T("rb"));
  
  /*
  //перематываем файл измерения в его конец
  fseek( fh, 0, SEEK_END);
  
  //узнаём длину, и передаём её в окно прогресса
  long lLen = ftell( fh);
  //SendMessage( pParent->m_hWnd, WM_SET_FILELEN, 0, lLen);

  //перематываем файл в начало
  fseek( fh, 0, SEEK_SET);
  */

  int btPrevPackCounter = 500;
  
  gl_InfoStep1.nMarkerFails = 0;
  gl_InfoStep1.nCheckSummFails = 0;
  gl_InfoStep1.nCounterFails = 0;
  gl_InfoStep1.lPacks = 0;
  gl_InfoStep1.dblTime = 0.;
  gl_InfoStep1.lDeviceId = 0;
  gl_InfoStep1.cSyncAsyncUnknown = 0;   //0 - unknown; 1-sync;        2-async;      3-both
  gl_InfoStep1.cHaveRegimedNdU = 0;     //0 - unknown; 1-only dndu;   2-only phi;   3-both
  gl_InfoStep1.bSignCoeff = 0;          //0 - unknown; 1- +1;         2- -1;
  gl_InfoStep1.dblDecCoeffMin = 1.e6;
  gl_InfoStep1.dblDecCoeffMax = -1.e6;
  gl_InfoStep1.dblDecCoeffMean = 0.;
  gl_InfoStep1.nDecCoeffData = 0;


  int nVer = (pParent->m_nVersionMajor << 16) + (pParent->m_nVersionMiddle << 8) + pParent->m_nVersionMinor;

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
                gl_InfoStep1.nMarkerFails++;
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
                gl_InfoStep1.nMarkerFails++;
                bMarkerFailOnce = false;
              }
            }
          }
        break;
      }
      
      lPos = ftell( fh);
      pParent->SendMessage( WM_SET_FILEPOS1, 0, lPos);

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
    
    if( bt7 == DECCOEFF) {
      int a =5;
    }

    //Обсчитаем пачку
    switch( nVer) {
      case 0x030205: pack.ProcessPackTime_3_2_5(); break;

      case 0x040107: pack.ProcessPackTime_4_1_7(); break;
      case 0x040200: pack.ProcessPackTime_4_2_0(); break;
      default:       pack.ProcessPackTime_3_2_3();
    }

    //ПРОВЕРКА КОНТРОЛЬНОЙ СУММЫ
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
        gl_InfoStep1.nCheckSummFails++;
      }
    }
    else {
      //ЕСЛИ С ПАЧКОЙ ВСЁ ХОРОШО
      
      //плюсуем количество пачек
      gl_InfoStep1.lPacks++;
      
      //суммируем время такта
      gl_InfoStep1.dblTime += pack.m_dblTime;


      //отслеживаем коэфициенты вычета
      if( bt7 == DECCOEFF) {
        unsigned short shCur1 = ( bt9 << 8) + bt8;
        double dblDecCoeff = ( ( double) shCur1) / 65535.;

        if( dblDecCoeff < gl_InfoStep1.dblDecCoeffMin) gl_InfoStep1.dblDecCoeffMin = dblDecCoeff;
        if( dblDecCoeff > gl_InfoStep1.dblDecCoeffMax) gl_InfoStep1.dblDecCoeffMax = dblDecCoeff;
        gl_InfoStep1.dblDecCoeffMean += dblDecCoeff;
        gl_InfoStep1.nDecCoeffData++;
      }
      
      //отслеживаем номер прибора
      if( bt7 == DEVNUM) {
        gl_InfoStep1.lDeviceId = ( bt9 << 8) + bt8;
      }

      if( bt7 == SIGNCOEFF) {
        if( pack.m_shSignCoeff == 0) gl_InfoStep1.bSignCoeff = 2;
        if( pack.m_shSignCoeff == 2) gl_InfoStep1.bSignCoeff = 1;
      }

      //отслеживаем режимы работы и тип выдаваемого параметра
      switch( nVer) {
        case 0x030204:
        case 0x030205:
        case 0x040200:          
          if( bt13 & 0x20)
            gl_InfoStep1.cSyncAsyncUnknown = 2;
          else
            gl_InfoStep1.cSyncAsyncUnknown = 1;

          if( bt13 & 0x10) 
            gl_InfoStep1.cHaveRegimedNdU = 1; //dnDu
          else
            gl_InfoStep1.cHaveRegimedNdU = 2; //phi
        break;
      }


      //ПРОВЕРКА СЧЁТЧИКА ПОСЫЛОК
      if( btPrevPackCounter == 500) {
        btPrevPackCounter = bt12;
      }
      else {
        if( ( ( btPrevPackCounter + 1) % 256) != bt12) {
          if( feof( fh) == 0) {
            gl_InfoStep1.nCounterFails++;
            
          }
        }
        btPrevPackCounter = bt12;
      }

      pParent->SendMessage( WM_SET_STEP1INFO, 0, reinterpret_cast <LPARAM> ( &gl_InfoStep1));
    }

  } while( feof( fh) == 0 && gl_bStopLoadThread1 == FALSE);

  fclose( fh);
  gl_bLoadFile1ThreadWork = FALSE;

  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// ПОТОК 2
// Поток загрузки данных из файла
BOOL gl_bLoadFile2ThreadWork;
BOOL gl_bStopLoadThread2;
double gl_dblTime;
double gl_dblStartSignCoeff;

DWORD WINAPI LoadFile2Thread(LPVOID lparam)
{
  int nSkip5points = 0;
  double dblDecCoeff, dblSignCoeff;

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
  
  gl_dblTime = 0.;


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

  //стартовый коэффициент вычета
  CString strEdtDecCoeffStart;
  pParent->GetDlgItem( IDC_EDT_DECCOEFF_START)->GetWindowText( strEdtDecCoeffStart);
  strEdtDecCoeffStart.Replace( ',', '.');
  dblDecCoeff = atof( strEdtDecCoeffStart);

  //стартовый знаковый коэффициент
  dblSignCoeff = gl_dblStartSignCoeff;
  
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

    pack.m_dblDecCoeff = dblDecCoeff;
    pack.m_shSignCoeff = dblSignCoeff;

    //Обсчитаем пачку (время мы получим в любом случае, поэтому нефиг смотреть на возвратное значение)
    int nVer = pParent->m_nVersionMajor * 65536 + pParent->m_nVersionMiddle * 256 + pParent->m_nVersionMinor;
    switch( nVer) {
      case 0x030205: pack.ProcessPack_3_2_5(); break;
      case 0x040107: pack.ProcessPack_4_1_7(); break;
      case 0x040200: pack.ProcessPack_4_2_0(); break;
      default:    pack.ProcessPack_3_2_3();
    }

    //ПРОВЕРКА ЧЕКСУММЫ
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


      gl_dblTime += pack.m_dblTime;
      //pParent->SendMessage( WM_SET_TIME, 0, 0);


      //ПРОВЕРКА РАВНОМЕРНОСТИ СЧЁТЧИКА ПОСЫЛОК
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

      
      //ОБСЧЁТ ПАРАМЕТРОВ ИЗ ПОСЫЛКИ
      tsa += pack.m_dblTime;    n_tsa_s++;

      if( lPacks > 10) {
        double dblPhi_ang = pack.m_dblPhi;
			  w100m +=  dblPhi_ang;		t100m += pack.m_dblTime;
			  w1s +=    dblPhi_ang;		t1s += pack.m_dblTime;
			  w10s +=   dblPhi_ang;		t10s += pack.m_dblTime;
			  w100s +=  dblPhi_ang;		t100s += pack.m_dblTime;
      }

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

      dblDecCoeff = pack.m_dblDecCoeff;
      dblSignCoeff = pack.m_shSignCoeff;
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
	//}}AFX_DATA_INIT
}


void COpenMeasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenMeasDlg)
	DDX_Control(pDX, IDC_PRGR0, m_ctlProgress0);
	DDX_Control(pDX, IDC_PRGR1, m_ctlProgress1);
	DDX_Control(pDX, IDC_PRGR2, m_ctlProgress2);
	DDX_Control(pDX, IDC_CWN_SIGNCOEFF, m_ctlSignCoeff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenMeasDlg, CDialog)
	//{{AFX_MSG_MAP(COpenMeasDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP_LOAD, OnBtnStopLoad)
	ON_BN_CLICKED(IDC_BTN_GOON, OnBtnGoon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenMeasDlg message handlers

BOOL COpenMeasDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  //ЗАПУСК ПОТОКА ДЛЯ ОБРАБОТКИ ДАННЫХ
  DWORD id2;
  HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile0Thread, this, 0, &id2);

  m_pStep1.nMarkerFails = 0;
  m_pStep1.nCheckSummFails = 0;
  m_pStep1.nCounterFails = 0;
  m_pStep1.lPacks = 0;
  m_pStep1.dblTime = 0.;
  m_pStep1.lDeviceId = 0;
  m_pStep1.cSyncAsyncUnknown = 0;   //0 - unknown; 1-sync;            2-async;        3-both
  m_pStep1.cHaveRegimedNdU = 0;     //0 - unknown; 1-have such data;  2-dont have it; 3-both
  m_pStep1.bSignCoeff = 0;          //0 - unknown; 1- +1;             2- -1;
  m_pStep1.dblDecCoeffMin = 1.e6;
  m_pStep1.dblDecCoeffMax = -1.e6;
  m_pStep1.dblDecCoeffMean = 0.;
  m_pStep1.nDecCoeffData = 0;

  m_lPacks = 0;
  m_nMarkerFails = 0;
  m_nCounterFails = 0;
  m_nCheckSummFails = 0;

  m_nVersionMajor = -1;
  m_nVersionMiddle = -1;
  m_nVersionMinor = -1;

  SetTimer( TIMER_REFRESH_VALUES0, 100, NULL);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT COpenMeasDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  switch( message) {
    case WM_SET_FILELEN:
      m_lFileLen = lParam;
      m_ctlProgress0.SetRange32( 0, m_lFileLen);
      m_ctlProgress1.SetRange32( 0, m_lFileLen);
      m_ctlProgress2.SetRange32( 0, m_lFileLen);
    break;
    case WM_SET_FILEPOS0:
      m_lFilePos = lParam;
      m_ctlProgress0.SetPos( m_lFilePos);
    break;
    case WM_SET_FILEPOS1:
      m_lFilePos = lParam;
      m_ctlProgress1.SetPos( m_lFilePos);
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
    //case WM_SET_TIME: {
    //  m_dblTime += dblAddTime;
    //}
    //break;
    case WM_SET_MC_VERSION:
      m_nVersionMajor =  wParam / 16;  //major
      m_nVersionMiddle = wParam % 16;	//middle
      m_nVersionMinor =  lParam / 16;	//minor
      theApp.m_strSoftwareVer.Format( "%d.%d.%d", m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);
    break;
    case WM_SET_STEP1INFO: {
      Step1Info *pStep1Info = reinterpret_cast <Step1Info *>(lParam);

      m_pStep1.nMarkerFails       = pStep1Info->nMarkerFails;
      m_pStep1.nCheckSummFails    = pStep1Info->nCheckSummFails;
      m_pStep1.nCounterFails      = pStep1Info->nCounterFails;
      m_pStep1.lPacks             = pStep1Info->lPacks;
      m_pStep1.dblTime            = pStep1Info->dblTime;
      m_pStep1.lDeviceId          = pStep1Info->lDeviceId;

      if( m_pStep1.cSyncAsyncUnknown != 3) {
        if( m_pStep1.cSyncAsyncUnknown == 0) {
          //мы словили (определили) тип запуска
          m_pStep1.cSyncAsyncUnknown = pStep1Info->cSyncAsyncUnknown;    //0 - uknown;   1-sync;  2-async; 3-both;
        }
        else {
          if( m_pStep1.cSyncAsyncUnknown != pStep1Info->cSyncAsyncUnknown)
            //мы словили разные типы запуска в рамках одного измерения
            m_pStep1.cSyncAsyncUnknown = 3;
        }
      }

      if( m_pStep1.cHaveRegimedNdU != 3) {
        if( m_pStep1.cHaveRegimedNdU == 0) {
          //мы словили (определили) выдаваемый параметр
          m_pStep1.cHaveRegimedNdU = pStep1Info->cHaveRegimedNdU;      //0 - unknown;  1-have such data;  2-dont have it; 3-both;
        }
        else {
          if( m_pStep1.cHaveRegimedNdU != pStep1Info->cHaveRegimedNdU)
            //мы словили разные типы выдаваемого параметра в рамках одного измерения
            m_pStep1.cHaveRegimedNdU = 3;
        }
      }
      

      m_pStep1.bSignCoeff         = pStep1Info->bSignCoeff;               //0 - unknown; 1- +1;             2- -1;
      m_pStep1.dblDecCoeffMin     = pStep1Info->dblDecCoeffMin;
      m_pStep1.dblDecCoeffMax     = pStep1Info->dblDecCoeffMax;
      m_pStep1.dblDecCoeffMean    = pStep1Info->dblDecCoeffMean;
      m_pStep1.nDecCoeffData      = pStep1Info->nDecCoeffData;

      /*
      CString str, strToAdd;
      str.Format( "lPacks=%ld\n", pStep1Info->lPacks);
      strToAdd.Format( "Time=");
      GetDlgItem( IDC_LBL_SLIDER1)->SetWindowText( str);
      */
    }
    break;
  }

  return CDialog::WindowProc(message, wParam, lParam);
}

void COpenMeasDlg::OnTimer(UINT nIDEvent)
{
  CString strTmp;

  switch( nIDEvent) {
    case TIMER_REFRESH_VALUES0: {
      if( m_nVersionMajor == -1 || m_nVersionMiddle == -1 || m_nVersionMinor == -1) {
        CString str;
        GetDlgItem( IDC_LBL_MC_VERSION)->GetWindowText( str);
        if( str.Right( 3) == _T("..."))
          strTmp.Format( "Определение версии прошивки прибора");
        else
          strTmp.Format( "Определение версии прошивки прибора ...");
      }
      else
        strTmp.Format( "Прибор с прошивкой версии: %x.%x.%x", m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);

      GetDlgItem( IDC_LBL_MC_VERSION)->SetWindowText( strTmp);


      if( gl_bLoadFile0ThreadWork == FALSE) {
        //закончил работу поток определения версии МК
        KillTimer( TIMER_REFRESH_VALUES0);

        if( gl_bStopLoadThread0 == TRUE) {
          //его прервали - выходим по CANCEL
          OnCancel();
        }
        else {
          
          //он закончил работу сам!
          if( m_nVersionMajor == -1 || m_nVersionMiddle == -1 || m_nVersionMinor == -1) {
            
            //если мы не смогли автоматичсеки определить версию ПО микроконтроллера - попросим указать её вручную
            CDlgSelectMcVer dlg;
            if( dlg.DoModal() != IDOK) {
              OnCancel();
              //SetTimer( TIMER_REFRESH_VALUES2, 1000, NULL);
              //break;
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
              case 3: //3.2.3
                m_nVersionMajor = 3; m_nVersionMiddle = 2; m_nVersionMinor = 3;
              break;
              case 4: //3.2.4
                m_nVersionMajor = 3; m_nVersionMiddle = 2; m_nVersionMinor = 4;
              break;
              case 5: //3.2.5
                m_nVersionMajor = 3; m_nVersionMiddle = 2; m_nVersionMinor = 5;
              break;

              case 6: //4.1.17
                m_nVersionMajor = 4; m_nVersionMiddle = 1; m_nVersionMinor = 17;
              break;
              case 7: //4.2.0
                m_nVersionMajor = 4; m_nVersionMiddle = 2; m_nVersionMinor = 0;
              break;
            }
          }
          //версия ПО микроконтроллера известна

          //начинаем пробежку для определения временной длительности файла
          DWORD id2;
          HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile1Thread, this, 0, &id2);
          
          SetTimer( TIMER_REFRESH_VALUES1, 500, NULL);
        }
      }

    }
    break;

    case TIMER_REFRESH_VALUES1: {
      double rest = m_pStep1.dblTime;
      
      int nHou = ( int) ( floor( rest / 60. / 60.));
      rest = rest - nHou * 3600;
      
      int nMin = ( int) ( floor( rest / 60));
      rest = rest - nMin * 60;

      int nSec = ( int) ( floor( rest));
      rest = rest - nSec;

      int nMsec = ( int) ( floor( rest * 1000.));

      
      strTmp.Format( "Длтельность: %02d:%02d:%02d.%03d", nHou, nMin, nSec, nMsec);
      GetDlgItem( IDC_LBL_STEP1_DURATION)->SetWindowText( strTmp);

      strTmp.Format( "Кол-во посылок: %ld", m_pStep1.lPacks);
      GetDlgItem( IDC_LBL_STEP1_PACKS)->SetWindowText( strTmp);

      strTmp.Format( "MF: %d     CF: %d     CSF: %d ",
          m_pStep1.nMarkerFails, m_pStep1.nCounterFails, m_pStep1.nCheckSummFails);
      GetDlgItem( IDC_LBL_STEP1_QUALITY)->SetWindowText( strTmp);

      strTmp.Format( "Номер прибора: %ld", m_pStep1.lDeviceId);
      GetDlgItem( IDC_LBL_STEP1_DEVICE_NUM)->SetWindowText( strTmp);

      switch( m_pStep1.cSyncAsyncUnknown) {
        case 0: strTmp.Format( "Режим работы прибора: неизвестно");   break;
        case 1: strTmp.Format( "Режим работы прибора: синхронный");  break;
        case 2: strTmp.Format( "Режим работы прибора: асинхронный");   break;
        case 3: strTmp.Format( "Режим работы прибора: присутствуют данные об обоих режимах работы");   break;
        case 4: strTmp.Format( "Режим работы прибора: default");   break;
      }
      GetDlgItem( IDC_LBL_STEP1_SYNC_ASYNC)->SetWindowText( strTmp);

      switch( m_pStep1.cHaveRegimedNdU) {
        case 0: strTmp.Format( "Выдаваемый параметр: неизвестно");   break;
        case 1: strTmp.Format( "Выдаваемый параметр: данные только в виде dNdU");  break;
        case 2: strTmp.Format( "Выдаваемый параметр: данные только в виде Phi");   break;
        case 3: strTmp.Format( "Выдаваемый параметр: выводимый параметр переключался в процессе запуска");   break;
        case 4: strTmp.Format( "Выдаваемый параметр: default");   break;
      }
      GetDlgItem( IDC_LBL_STEP1_DN_DU_PRESENCE)->SetWindowText( strTmp);
      
      switch( m_pStep1.bSignCoeff) {
        case 1: m_ctlSignCoeff.SetValue( 1.);   break;
        case 2: m_ctlSignCoeff.SetValue( -1.);  break;
      }

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, мин: %.5f", m_pStep1.dblDecCoeffMin);
      else
        strTmp.Format( "Коэфф. вычета, мин: -");
      GetDlgItem( IDC_LBL_STEP1_DECCOEFF_MIN)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, ср.: %.5f", m_pStep1.dblDecCoeffMean / m_pStep1.nDecCoeffData);
      else
        strTmp.Format( "Коэфф. вычета, ср: -");
      GetDlgItem( IDC_LBL_STEP1_DECCOEFF_AVG)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "%.5f", m_pStep1.dblDecCoeffMean / m_pStep1.nDecCoeffData);
      else
        strTmp.Format( "");
      GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, макс: %.5f", m_pStep1.dblDecCoeffMax);
      else
        strTmp.Format( "Коэфф. вычета, макс: -");
      GetDlgItem( IDC_LBL_STEP1_DECCOEFF_MAX)->SetWindowText( strTmp);
      

      if( gl_bLoadFile1ThreadWork == FALSE) {
        //закончил работу поток определение временной длительности файла измерения

        KillTimer( TIMER_REFRESH_VALUES1);

        if( gl_bStopLoadThread1 == TRUE) {
          //его прервали - выходим по CANCEL
          OnCancel();
        }
        else {
          
          //он закончил работу сам!
          
          //заказываем массивы под точки
          CMainFrame *pFrm = ( CMainFrame *) AfxGetApp()->GetMainWnd();
          CMainView *pView = ( CMainView *) pFrm->GetActiveView();
          CSlg2Doc *pDoc =   ( CSlg2Doc *) pView->GetDocument();
          
          pDoc->m_nMcVersionMajor  = m_nVersionMajor;
          pDoc->m_nMcVersionMiddle = m_nVersionMiddle;
          pDoc->m_nMcVersionMinor  = m_nVersionMinor;

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
	 
          m_dn100m = ( long) ( ceil( m_pStep1.dblTime * 10));
          long dn100m = ( long) ( ceil( m_pStep1.dblTime * 10));

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

          //0 - unknown; 1-only dndu;   2-only phi;   3-both
          if( m_pStep1.cHaveRegimedNdU == 1 || m_pStep1.cHaveRegimedNdU == 3) {
            GetDlgItem( IDC_CWN_SIGNCOEFF)->EnableWindow( TRUE);
            GetDlgItem( IDC_EDT_DECCOEFF_START)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_GOON)->EnableWindow( TRUE);
          }
          else {
            gl_dblStartSignCoeff = m_ctlSignCoeff.GetValue();

            //начинаем третью пробежку
            DWORD id2;
            HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile2Thread, this, 0, &id2);          
            SetTimer( TIMER_REFRESH_VALUES2, 500, NULL);
          }

        }
      }
    }
    break;

    case TIMER_REFRESH_VALUES2:
      double rest = gl_dblTime;

      int nHou = ( int) ( floor( rest / 60. / 60.));
      rest = rest - nHou * 3600;
      
      int nMin = ( int) ( floor( rest / 60));
      rest = rest - nMin * 60;

      int nSec = ( int) ( floor( rest));
      rest = rest - nSec;

      int nMsec = ( int) ( floor( rest * 1000.));


      strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails);
      
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
  if( gl_bLoadFile0ThreadWork == FALSE &&
      gl_bLoadFile1ThreadWork == FALSE &&
      gl_bLoadFile2ThreadWork == FALSE)
        OnCancel();

  gl_bStopLoadThread1 = TRUE;
  gl_bStopLoadThread2 = TRUE;
}

void COpenMeasDlg::OnBtnGoon() 
{
  gl_dblStartSignCoeff = m_ctlSignCoeff.GetValue();

  //начинаем третью пробежку
  DWORD id2;
  HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile2Thread, this, 0, &id2);          
  SetTimer( TIMER_REFRESH_VALUES2, 500, NULL);	
  
}
