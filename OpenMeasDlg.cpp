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
#include "SlgGroupNewAverager.h"


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

  //char *meas = ( char *) malloc( lLen);
  //if( meas != NULL) free( meas);

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
  gl_InfoStep1.dblDecCoeffFirst = 0.;
  gl_InfoStep1.dblDecCoeffLast = 0.;
  gl_InfoStep1.nDecCoeffData = 0;


  int nVer = (pParent->m_nVersionMajor << 16) + (pParent->m_nVersionMiddle << 8) + pParent->m_nVersionMinor;

  //поехали
  do {

    unsigned char bts[14];
    long lPos;

    if( gl_bStopLoadThread1 == TRUE)
      break;

    if( fread( bts, 14, 1, fh) != 1) break;

    //ПРИРАЩЕНИЕ УГЛА: 4 байта
    //НОМЕР ЧЕРЕДУЮЩЕГОСЯ (ТЕХНОЛОГИЧЕСКОГО, АНАЛОГОВОГО) ПАРАМЕТРА. 1 байт
    //ЗНАЧЕНИЕ ТЕХНОЛОГИЧЕСКОГО (АНАЛОГОВОГО) ПАРАМЕТРА. 2 Байта
    //SA TIMING.
    //ИНТЕРВАЛ ВРЕМЕНИ МЕЖДУ ТЕКУЩИМ И ПРЕДЫДУЩИМ МОМЕНТАМИ ФИКСАЦИИ ПАРАМЕТРОВ. 2 БАЙТА
    //ПОРЯДКОВЫЙ НОМЕР СООБЩЕНИЯ. 1 БАЙТ
    //EMERGENCY CODE
    //КОД ОШИБКИ. 1 БАЙТ
    //CHEKSUMM
    //КОНТРОЛЬНАЯ СУММА, CS. 1 байт
    
    if( feof( fh) != 0)
      break;

    lPos = ftell( fh);
    pParent->SendMessage( WM_SET_FILEPOS1, 0, lPos);
    pParent->SendMessage( WM_SET_STEP1INFO, 0, reinterpret_cast <LPARAM> ( &gl_InfoStep1));

    //РАЗБОР ПАЧКИ
    CPackProcessing pack;
        
    pack.bt3 = bts[2];
    pack.bt4 = bts[3];
    pack.bt5 = bts[4];
    pack.bt6 = bts[5];

    pack.bt7 = bts[6];

    pack.bt8 = bts[7];
    pack.bt9 = bts[8];

    pack.bt10 = bts[9];
    pack.bt11 = bts[10];

    pack.bt12 = bts[11];

    pack.bt13 = bts[12];

    //Обсчитаем пачку
    switch( nVer) {
      case 0x030202: pack.ProcessPackTime_3_2_2(); break;  // << там формат bin... глупость в общем
      case 0x030204: pack.ProcessPackTime_3_2_4(); break;
      case 0x030205: pack.ProcessPackTime_3_2_5(); break;

      case 0x040107: pack.ProcessPackTime_4_1_7(); break;
      case 0x040200: pack.ProcessPackTime_4_2_0(); break;
      default:       pack.ProcessPackTime_3_2_3();
    }
    
    //ЕСЛИ С ПАЧКОЙ ВСЁ ХОРОШО
     
    //плюсуем количество пачек
    gl_InfoStep1.lPacks++;
      
    //суммируем время такта
    gl_InfoStep1.dblTime += pack.m_dblTime;


    //отслеживаем коэфициенты вычета
    if( bts[6] == DECCOEFF) {
      unsigned short shCur1 = ( bts[8] << 8) + bts[7];
      double dblDecCoeff = ( ( double) shCur1) / 65535.;

      if( gl_InfoStep1.dblDecCoeffFirst == 0.) gl_InfoStep1.dblDecCoeffFirst = dblDecCoeff;
      gl_InfoStep1.dblDecCoeffLast = dblDecCoeff;

      if( dblDecCoeff < gl_InfoStep1.dblDecCoeffMin) gl_InfoStep1.dblDecCoeffMin = dblDecCoeff;
      if( dblDecCoeff > gl_InfoStep1.dblDecCoeffMax) gl_InfoStep1.dblDecCoeffMax = dblDecCoeff;
      gl_InfoStep1.dblDecCoeffMean += dblDecCoeff;
      gl_InfoStep1.nDecCoeffData++;
    }
      
    //отслеживаем номер прибора
    if( bts[6] == DEVNUM) {
      gl_InfoStep1.lDeviceId = ( bts[8] << 8) + bts[7];
    }

    if( bts[6] == SIGNCOEFF) {
      if( pack.m_shSignCoeff == 0) gl_InfoStep1.bSignCoeff = 2;
      if( pack.m_shSignCoeff == 2) gl_InfoStep1.bSignCoeff = 1;
    }

    //отслеживаем режимы работы и тип выдаваемого параметра
    switch( nVer) {
      case 0x030202:
        gl_InfoStep1.cSyncAsyncUnknown = 0;
        gl_InfoStep1.cHaveRegimedNdU = 0;
      break;

      case 0x030204:
      case 0x030205:
      case 0x040200:          
        if( bts[12] & 0x20)
          gl_InfoStep1.cSyncAsyncUnknown = 2;
        else
          gl_InfoStep1.cSyncAsyncUnknown = 1;

        if( bts[12] & 0x10) 
          gl_InfoStep1.cHaveRegimedNdU = 1; //dnDu
        else
          gl_InfoStep1.cHaveRegimedNdU = 2; //phi
      break;
    }


    pParent->SendMessage( WM_SET_STEP1INFO, 0, reinterpret_cast <LPARAM> ( &gl_InfoStep1));


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
int gl_nSkipPacks, gl_nSkipMsecs;

BOOL gl_bLineTact, gl_bLine100ms, gl_bLine1s, gl_bLine10s, gl_bLine100s, gl_bLine10m;

DWORD WINAPI LoadFile2Thread(LPVOID lparam)
{
  double dblRunningI1 = 0.;
  double dblRunningI2 = 0.;
  double dblRunningVpc = 0.;
  double dblRunningAAa = 0.;
  double dblRunningAAd = 0.;
  double dblRunningAAr = 0.;
  double dblRunningT1 = 0.;
  double dblRunningT2 = 0.;
  double dblRunningT3 = 0.;
  double dblRunningDecCoeff = 0.;
  double dblRunningSignCoeff = 0.;

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

  CSlgGroupNewAverager sgav_phi;
  CSlgGroupNewAverager sgav_i1;
  CSlgGroupNewAverager sgav_i2;
  CSlgGroupNewAverager sgav_vpc;
  CSlgGroupNewAverager sgav_aaa;
  CSlgGroupNewAverager sgav_aad;
  CSlgGroupNewAverager sgav_aar;
  CSlgGroupNewAverager sgav_t1;
  CSlgGroupNewAverager sgav_t2;
  CSlgGroupNewAverager sgav_t3;
  CSlgGroupNewAverager sgav_tsa;
  CSlgGroupNewAverager sgav_dc;
  CSlgGroupNewAverager sgav_deccoeff;

  double d_global_time = 0.;

  //стартовый коэффициент вычета
  CString strEdtDecCoeffStart;
  pParent->GetDlgItem( IDC_EDT_DECCOEFF_START)->GetWindowText( strEdtDecCoeffStart);
  strEdtDecCoeffStart.Replace( ',', '.');
  dblRunningDecCoeff = atof( strEdtDecCoeffStart);

  //стартовый знаковый коэффициент
  dblRunningSignCoeff = gl_dblStartSignCoeff;
  
  //поехали
  do {
    char bts[14];
    long lPos; 
    lPos = ftell( fh);
    pParent->SendMessage( WM_SET_FILEPOS2, 0, lPos);

    if( feof( fh) != 0)
      break;

    if( gl_bStopLoadThread1 == TRUE)
      break;

    if( fread( bts, 14, 1, fh) != 1)
      break;

    //РАЗБОР ПАЧКИ
    CPackProcessing pack;
        
    pack.bt3 = bts[2];
    pack.bt4 = bts[3];
    pack.bt5 = bts[4];
    pack.bt6 = bts[5];

    pack.bt7 = bts[6];

    pack.bt8 = bts[7];
    pack.bt9 = bts[8];

    pack.bt10 = bts[9];
    pack.bt11 = bts[10];

    pack.bt12 = bts[11];

    pack.bt13 = bts[12];

    pack.m_dblDecCoeff = dblRunningDecCoeff;
    pack.m_shSignCoeff = dblRunningSignCoeff;

    //Обсчитаем пачку (время мы получим в любом случае, поэтому нефиг смотреть на возвратное значение)
    int nVer = pParent->m_nVersionMajor * 65536 + pParent->m_nVersionMiddle * 256 + pParent->m_nVersionMinor;
    switch( nVer) {
      case 0x030202: pack.ProcessPack_3_2_2(); break;
      case 0x030205: pack.ProcessPack_3_2_5(); break;
      case 0x040107: pack.ProcessPack_4_1_7(); break;
      case 0x040200: pack.ProcessPack_4_2_0(); break;
      default:    pack.ProcessPack_3_2_3();
    }

    
    lPacks++;
    pParent->SendMessage( WM_SET_PACKS, 0, lPacks);

    gl_dblTime += pack.m_dblTime;
    

    //общее время данных
    d_global_time += pack.m_dblTime;

    if( pParent->m_nRadSkip == 0) {
      if( lPacks <= gl_nSkipPacks)
        continue;
    }
    else if( pParent->m_nRadSkip == 1) {
      if( gl_dblTime <= gl_nSkipMsecs / 1000.)
        continue;
    }

    //обновим (если обработчик пачки их сменил) к.вычета и зн.коэф.
    dblRunningDecCoeff = pack.m_dblDecCoeff;
    dblRunningSignCoeff = pack.m_shSignCoeff;


    //ОБСЧЁТ ПАРАМЕТРОВ ИЗ ПОСЫЛКИ
    sgav_phi.CommonAddPoint( pack.m_dblPhi);
    sgav_tsa.CommonAddPoint( pack.m_dblTime);
    switch( pack.m_nAnParam) {
      case UTD1: sgav_t1.CommonAddPoint( pack.m_dblAnParamValue); break;       //термодатчик 1
      case UTD2: sgav_t2.CommonAddPoint( pack.m_dblAnParamValue); break;       //термодатчик 2
      case UTD3: sgav_t3.CommonAddPoint( pack.m_dblAnParamValue); break;       //термодатчик 3
      case I1:   sgav_i1.CommonAddPoint( pack.m_dblAnParamValue); break;       //разрядный ток i1
      case I2:   sgav_i2.CommonAddPoint( pack.m_dblAnParamValue); break;       //разрядный ток i2
      case CNTRPC: sgav_vpc.CommonAddPoint( pack.m_dblAnParamValue); break;    //напряжение на пьезокорректорах
      case AMPLANG_ALTERA: sgav_aaa.CommonAddPoint( pack.m_dblAnParamValue); break; //амплитуда получаемая от alter'ы
      case AMPLANG_DUS: sgav_aad.CommonAddPoint( pack.m_dblAnParamValue); break;    //амплитуда получаемая с ДУСа
      case RULA: sgav_aar.CommonAddPoint( pack.m_dblAnParamValue); break;       //напряжение RULA
      case DECCOEFF: sgav_deccoeff.CommonAddPoint( pack.m_dblAnParamValue); break;       //коэффициент вычета
    }

    // *** *** *** *** *** *** ***
    // TACTS POINTS
    // *** *** *** *** *** *** ***
    if( gl_bLineTact) {
      //угловая скорость (угол поворота)
      pDoc->m_dpW.AddPoint_Tact( d_global_time, pack.m_dblPhi / pack.m_dblTime);
      
      //время такта
      pDoc->m_dpTsa.AddPoint_Tact( d_global_time, pack.m_dblTime);
      
      //в зависимости от того, что пришло - аналоговый параметр
      switch( pack.m_nAnParam) {

        case UTD1: pDoc->m_dpT1.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;       //термодатчик 1
        case UTD2: pDoc->m_dpT2.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;       //термодатчик 2
        case UTD3: pDoc->m_dpT3.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;       //термодатчик 3
        case I1:   pDoc->m_dpI1.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;       //разрядный ток i1
        case I2:   pDoc->m_dpI2.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;       //разрядный ток i2
        case CNTRPC: pDoc->m_dpVpc.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break;           //напряжение на пьезокорректорах
        case AMPLANG_ALTERA: pDoc->m_dpAAa.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue);   break; //амплитуда получаемая от alter'ы
        case AMPLANG_DUS: pDoc->m_dpAAd.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue);      break; //амплитуда получаемая с ДУСа
        case RULA:        pDoc->m_dpAAr.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue);      break; //напряжение RULA
        case DECCOEFF:    pDoc->m_dpDecCoeff.AddPoint_Tact( d_global_time, pack.m_dblAnParamValue); break; //коэффициент вычета
      }
    }
  

    // *** *** *** *** *** *** ***
    // 100MSEC POINTS
    // *** *** *** *** *** *** ***
    if( gl_bLine100ms && sgav_tsa.Get_100ms()->GetSumm() >= 0.1) {
      //100ms happens

      double dblW = sgav_phi.Get_100ms()->GetSumm() / sgav_tsa.Get_100ms()->GetSumm();
      sgav_phi.Get_100ms()->Reset();
      pDoc->m_dpW.AddPoint_100m( d_global_time, dblW);

      pDoc->m_dpI1.AddPoint_100m(   d_global_time, sgav_i1.Get_100ms()->GetMean());
      pDoc->m_dpI2.AddPoint_100m(   d_global_time, sgav_i2.Get_100ms()->GetMean());
      pDoc->m_dpVpc.AddPoint_100m(  d_global_time, sgav_vpc.Get_100ms()->GetMean());
      pDoc->m_dpAAa.AddPoint_100m(  d_global_time, sgav_aaa.Get_100ms()->GetMean());
      pDoc->m_dpAAd.AddPoint_100m(  d_global_time, sgav_aad.Get_100ms()->GetMean());
      pDoc->m_dpAAr.AddPoint_100m(  d_global_time, sgav_aar.Get_100ms()->GetMean());
      pDoc->m_dpT1.AddPoint_100m(   d_global_time, sgav_t1.Get_100ms()->GetMean());
      pDoc->m_dpT2.AddPoint_100m(   d_global_time, sgav_t2.Get_100ms()->GetMean());
      pDoc->m_dpT3.AddPoint_100m(   d_global_time, sgav_t3.Get_100ms()->GetMean());
      pDoc->m_dpTsa.AddPoint_100m(  d_global_time, sgav_tsa.Get_100ms()->GetMean());
      pDoc->m_dpDecCoeff.AddPoint_100m( d_global_time, sgav_dc.Get_100ms()->GetMean());
    }
      
    // *** *** *** *** *** *** ***
    // 1 SEC POINTS
    // *** *** *** *** *** *** ***
    if( gl_bLine1s && sgav_tsa.Get_1s()->GetSumm() >= 1.0) {
      //10s happens

      double dblW = sgav_phi.Get_1s()->GetSumm() / sgav_tsa.Get_1s()->GetSumm();
      sgav_phi.Get_1s()->Reset();
      pDoc->m_dpW.AddPoint_1s( d_global_time, dblW);

      pDoc->m_dpI1.AddPoint_1s(   d_global_time, sgav_i1.Get_1s()->GetMean());
      pDoc->m_dpI2.AddPoint_1s(   d_global_time, sgav_i2.Get_1s()->GetMean());
      pDoc->m_dpVpc.AddPoint_1s(  d_global_time, sgav_vpc.Get_1s()->GetMean());
      pDoc->m_dpAAa.AddPoint_1s(  d_global_time, sgav_aaa.Get_1s()->GetMean());
      pDoc->m_dpAAd.AddPoint_1s(  d_global_time, sgav_aad.Get_1s()->GetMean());
      pDoc->m_dpAAr.AddPoint_1s(  d_global_time, sgav_aar.Get_1s()->GetMean());
      pDoc->m_dpT1.AddPoint_1s(   d_global_time, sgav_t1.Get_1s()->GetMean());
      pDoc->m_dpT2.AddPoint_1s(   d_global_time, sgav_t2.Get_1s()->GetMean());
      pDoc->m_dpT3.AddPoint_1s(   d_global_time, sgav_t3.Get_1s()->GetMean());
      pDoc->m_dpTsa.AddPoint_1s(  d_global_time, sgav_tsa.Get_1s()->GetMean());
      pDoc->m_dpDecCoeff.AddPoint_1s( d_global_time, sgav_dc.Get_1s()->GetMean());
    }

		// *** *** *** *** *** *** ***
    // 10 SEC POINTS
    // *** *** *** *** *** *** ***
		if( gl_bLine10s && sgav_tsa.Get_10s()->GetSumm() >= 10.0) {
      //10s happens

      double dblW = sgav_phi.Get_10s()->GetSumm() / sgav_tsa.Get_10s()->GetSumm();
      sgav_phi.Get_10s()->Reset();
      pDoc->m_dpW.AddPoint_10s( d_global_time, dblW);

      pDoc->m_dpI1.AddPoint_10s(   d_global_time, sgav_i1.Get_10s()->GetMean());
      pDoc->m_dpI2.AddPoint_10s(   d_global_time, sgav_i2.Get_10s()->GetMean());
      pDoc->m_dpVpc.AddPoint_10s(  d_global_time, sgav_vpc.Get_10s()->GetMean());
      pDoc->m_dpAAa.AddPoint_10s(  d_global_time, sgav_aaa.Get_10s()->GetMean());
      pDoc->m_dpAAd.AddPoint_10s(  d_global_time, sgav_aad.Get_10s()->GetMean());
      pDoc->m_dpAAr.AddPoint_10s(  d_global_time, sgav_aar.Get_10s()->GetMean());
      pDoc->m_dpT1.AddPoint_10s(   d_global_time, sgav_t1.Get_10s()->GetMean());
      pDoc->m_dpT2.AddPoint_10s(   d_global_time, sgav_t2.Get_10s()->GetMean());
      pDoc->m_dpT3.AddPoint_10s(   d_global_time, sgav_t3.Get_10s()->GetMean());
      pDoc->m_dpTsa.AddPoint_10s(  d_global_time, sgav_tsa.Get_10s()->GetMean());
      pDoc->m_dpDecCoeff.AddPoint_10s( d_global_time, sgav_dc.Get_10s()->GetMean());
    }

    // *** *** *** *** *** *** ***
    // 100 SEC POINTS
    // *** *** *** *** *** *** ***
		if( gl_bLine100s && sgav_tsa.Get_100s()->GetSumm() >= 100.0) {
      //100s happens

      double dblW = sgav_phi.Get_100s()->GetSumm() / sgav_tsa.Get_100s()->GetSumm();
      sgav_phi.Get_100s()->Reset();
      pDoc->m_dpW.AddPoint_100s( d_global_time, dblW);

      pDoc->m_dpI1.AddPoint_100s(   d_global_time, sgav_i1.Get_100s()->GetMean());
      pDoc->m_dpI2.AddPoint_100s(   d_global_time, sgav_i2.Get_100s()->GetMean());
      pDoc->m_dpVpc.AddPoint_100s(  d_global_time, sgav_vpc.Get_100s()->GetMean());
      pDoc->m_dpAAa.AddPoint_100s(  d_global_time, sgav_aaa.Get_100s()->GetMean());
      pDoc->m_dpAAd.AddPoint_100s(  d_global_time, sgav_aad.Get_100s()->GetMean());
      pDoc->m_dpAAr.AddPoint_100s(  d_global_time, sgav_aar.Get_100s()->GetMean());
      pDoc->m_dpT1.AddPoint_100s(   d_global_time, sgav_t1.Get_100s()->GetMean());
      pDoc->m_dpT2.AddPoint_100s(   d_global_time, sgav_t2.Get_100s()->GetMean());
      pDoc->m_dpT3.AddPoint_100s(   d_global_time, sgav_t3.Get_100s()->GetMean());
      pDoc->m_dpTsa.AddPoint_100s(  d_global_time, sgav_tsa.Get_100s()->GetMean());
      pDoc->m_dpDecCoeff.AddPoint_100s( d_global_time, sgav_dc.Get_100s()->GetMean());
    }

    // *** *** *** *** *** *** ***
    // 10 MIN POINTS
    // *** *** *** *** *** *** ***
		if( gl_bLine10m && sgav_tsa.Get_10m()->GetSumm() >= 600.0) {
      //10m happens

      double dblW = sgav_phi.Get_10m()->GetSumm() / sgav_tsa.Get_10m()->GetSumm();
      sgav_phi.Get_10m()->Reset();
      pDoc->m_dpW.AddPoint_10m( d_global_time, dblW);

      pDoc->m_dpI1.AddPoint_10m(   d_global_time, sgav_i1.Get_10m()->GetMean());
      pDoc->m_dpI2.AddPoint_10m(   d_global_time, sgav_i2.Get_10m()->GetMean());
      pDoc->m_dpVpc.AddPoint_10m(  d_global_time, sgav_vpc.Get_10m()->GetMean());
      pDoc->m_dpAAa.AddPoint_10m(  d_global_time, sgav_aaa.Get_10m()->GetMean());
      pDoc->m_dpAAd.AddPoint_10m(  d_global_time, sgav_aad.Get_10m()->GetMean());
      pDoc->m_dpAAr.AddPoint_10m(  d_global_time, sgav_aar.Get_10m()->GetMean());
      pDoc->m_dpT1.AddPoint_10m(   d_global_time, sgav_t1.Get_10m()->GetMean());
      pDoc->m_dpT2.AddPoint_10m(   d_global_time, sgav_t2.Get_10m()->GetMean());
      pDoc->m_dpT3.AddPoint_10m(   d_global_time, sgav_t3.Get_10m()->GetMean());
      pDoc->m_dpTsa.AddPoint_10m(  d_global_time, sgav_tsa.Get_10m()->GetMean());
      pDoc->m_dpDecCoeff.AddPoint_10m( d_global_time, sgav_dc.Get_10m()->GetMean());
    }

  } while( feof( fh) == 0 && gl_bStopLoadThread1 == FALSE);

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
	m_nRadSkip = 0;
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
	DDX_Control(pDX, IDC_CWN_SKIP_PACKS, m_ctlSkipPacks);
	DDX_Radio(pDX, IDC_RAD_SKIP_PACKS, m_nRadSkip);
	DDX_Control(pDX, IDC_CWN_SKIP_MSECS, m_ctlSkipMsecs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenMeasDlg, CDialog)
	//{{AFX_MSG_MAP(COpenMeasDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP_LOAD, OnBtnStopLoad)
	ON_BN_CLICKED(IDC_BTN_GOON, OnBtnGoon)
	ON_BN_CLICKED(IDC_RAD_SKIP_PACKS, OnRadSkipPacks)
	ON_BN_CLICKED(IDC_RAD_SKIP_TIME, OnRadSkipTime)
	ON_BN_CLICKED(IDC_BTN_STEP1_DECCOEFF_FIRST, OnBtnStep1DeccoeffFirst)
	ON_BN_CLICKED(IDC_BTN_STEP1_DECCOEFF_LAST, OnBtnStep1DeccoeffLast)
	ON_BN_CLICKED(IDC_BTN_STEP1_DECCOEFF_MIN, OnBtnStep1DeccoeffMin)
	ON_BN_CLICKED(IDC_BTN_STEP1_DECCOEFF_AVG, OnBtnStep1DeccoeffAvg)
	ON_BN_CLICKED(IDC_BTN_STEP1_DECCOEFF_MAX, OnBtnStep1DeccoeffMax)
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
  SetThreadPriority( hthread2, THREAD_PRIORITY_HIGHEST);
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

  m_lPrevPacks = 0;

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
      m_pStep1.dblDecCoeffFirst   = pStep1Info->dblDecCoeffFirst;
      m_pStep1.dblDecCoeffLast    = pStep1Info->dblDecCoeffLast;
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

            strTmp.Format( "Версия прошивки прибора определена вручную: %x.%x.%x", m_nVersionMajor, m_nVersionMiddle, m_nVersionMinor);
            GetDlgItem( IDC_LBL_MC_VERSION)->SetWindowText( strTmp);
          }
          //версия ПО микроконтроллера известна

          
          //начинаем пробежку для определения временной длительности файла
          DWORD id2;
          HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile1Thread, this, 0, &id2);
          SetThreadPriority( hthread2, THREAD_PRIORITY_HIGHEST);

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

      /*
      strTmp.Format( "MF: %d     CF: %d     CSF: %d ",
          m_pStep1.nMarkerFails, m_pStep1.nCounterFails, m_pStep1.nCheckSummFails);
      GetDlgItem( IDC_LBL_STEP1_QUALITY)->SetWindowText( strTmp);
      */
      strTmp.Format( "%d", m_pStep1.lPacks - m_lPrevPacks);
      GetDlgItem( IDC_LBL_STEP1_QUALITY)->SetWindowText( strTmp);
      m_lPrevPacks = m_pStep1.lPacks;
      

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
        strTmp.Format( "Коэфф. вычета, первый: %.5f", m_pStep1.dblDecCoeffFirst);
      else
        strTmp.Format( "Коэфф. вычета, первый: -");
      GetDlgItem( IDC_BTN_STEP1_DECCOEFF_FIRST)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, последний: %.5f", m_pStep1.dblDecCoeffLast);
      else
        strTmp.Format( "Коэфф. вычета, последний: -");
      GetDlgItem( IDC_BTN_STEP1_DECCOEFF_LAST)->SetWindowText( strTmp);


      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, мин: %.5f", m_pStep1.dblDecCoeffMin);
      else
        strTmp.Format( "Коэфф. вычета, мин: -");
      GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MIN)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, ср.: %.5f", m_pStep1.dblDecCoeffMean / m_pStep1.nDecCoeffData);
      else
        strTmp.Format( "Коэфф. вычета, ср: -");
      GetDlgItem( IDC_BTN_STEP1_DECCOEFF_AVG)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "%.5f", m_pStep1.dblDecCoeffMean / m_pStep1.nDecCoeffData);
      else
        strTmp.Format( "");
      GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);

      if( m_pStep1.nDecCoeffData > 0)
        strTmp.Format( "Коэфф. вычета, макс: %.5f", m_pStep1.dblDecCoeffMax);
      else
        strTmp.Format( "Коэфф. вычета, макс: -");
      GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MAX)->SetWindowText( strTmp);
      

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
          
          pDoc->m_dblMeasDuration  = m_pStep1.dblTime;

          pDoc->m_nMcVersionMajor  = m_nVersionMajor;
          pDoc->m_nMcVersionMiddle = m_nVersionMiddle;
          pDoc->m_nMcVersionMinor  = m_nVersionMinor;

          //высвобождаем если что либо заказано (что-то было открыто)
		      //АВТОМАТИЧЕСКИ
	 
          m_dn100m = ( long) ( ceil( m_pStep1.dblTime * 10));
          long dn100m = ( long) ( ceil( m_pStep1.dblTime * 10));

          if( m_pStep1.dblTime < 36000)
            ( ( CButton *) GetDlgItem( IDC_CHK_LINE_TACT))->SetCheck( TRUE);
          ( ( CButton *) GetDlgItem( IDC_CHK_LINE_100MS))->SetCheck( TRUE);
          ( ( CButton *) GetDlgItem( IDC_CHK_LINE_1S))->SetCheck( TRUE);
          ( ( CButton *) GetDlgItem( IDC_CHK_LINE_10S))->SetCheck( TRUE);
          if( m_pStep1.dblTime > 200)
            ( ( CButton *) GetDlgItem( IDC_CHK_LINE_100S))->SetCheck( TRUE);
          if( m_pStep1.dblTime > 36000)
            ( ( CButton *) GetDlgItem( IDC_CHK_LINE_10M))->SetCheck( TRUE);

          //0 - unknown; 1-only dndu;   2-only phi;   3-both
          //if( m_pStep1.cHaveRegimedNdU == 1 || m_pStep1.cHaveRegimedNdU == 3) {
            GetDlgItem( IDC_CWN_SIGNCOEFF)->EnableWindow( TRUE);
            GetDlgItem( IDC_CWN_SKIP_PACKS)->EnableWindow( TRUE);
            GetDlgItem( IDC_EDT_DECCOEFF_START)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_GOON)->EnableWindow( TRUE);

            GetDlgItem( IDC_CHK_LINE_TACT)->EnableWindow( TRUE);
            GetDlgItem( IDC_CHK_LINE_100MS)->EnableWindow( TRUE);
            GetDlgItem( IDC_CHK_LINE_1S)->EnableWindow( TRUE);
            GetDlgItem( IDC_CHK_LINE_10S)->EnableWindow( TRUE);
            GetDlgItem( IDC_CHK_LINE_100S)->EnableWindow( TRUE);
            GetDlgItem( IDC_CHK_LINE_10M)->EnableWindow( TRUE);

            GetDlgItem( IDC_BTN_STEP1_DECCOEFF_FIRST)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_STEP1_DECCOEFF_LAST)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MIN)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_STEP1_DECCOEFF_AVG)->EnableWindow( TRUE);
            GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MAX)->EnableWindow( TRUE);
          //}
          //else {
          //  gl_dblStartSignCoeff = m_ctlSignCoeff.GetValue();
          //  gl_nSkipPacks = m_ctlSkipPacks.GetValue();
          //
          //  //начинаем третью пробежку
          //  DWORD id2;
          //  HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile2Thread, this, 0, &id2);          
          //  SetTimer( TIMER_REFRESH_VALUES2, 500, NULL);
          //}
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


      /*
      strTmp.Format( "PACKS: %ld   TIME: %02d:%02d:%02d.%03d   MF: %d   CF: %d   CSF: %d",
                m_lPacks,
                nHou, nMin, nSec, nMsec,
                m_nMarkerFails, m_nCounterFails, m_nCheckSummFails);
      */
      strTmp.Format( "TIME: %02d:%02d:%02d.%03d   PACKS: %ld    %ld",
                nHou, nMin, nSec, nMsec,
                m_lPacks, m_lPacks - m_lPrevPacks);
      GetDlgItem( IDC_LBL_SLIDER2)->SetWindowText( strTmp);
      m_lPrevPacks = m_lPacks;

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
  UpdateData( TRUE);
  
  gl_bLineTact =  ( ( CButton *) GetDlgItem( IDC_CHK_LINE_TACT))->GetCheck();
  gl_bLine100ms = ( ( CButton *) GetDlgItem( IDC_CHK_LINE_100MS))->GetCheck();
  gl_bLine1s =    ( ( CButton *) GetDlgItem( IDC_CHK_LINE_1S))->GetCheck();
  gl_bLine10s =   ( ( CButton *) GetDlgItem( IDC_CHK_LINE_10S))->GetCheck();
  gl_bLine100s =  ( ( CButton *) GetDlgItem( IDC_CHK_LINE_100S))->GetCheck();
  gl_bLine10m =   ( ( CButton *) GetDlgItem( IDC_CHK_LINE_10M))->GetCheck();

  if( !(gl_bLineTact || gl_bLine100ms || gl_bLine1s || gl_bLine10s || gl_bLine100s || gl_bLine10m)) {
    AfxMessageBox( _T("Выберите хотя бы один интервал осреденения для вычисления."));
    return;
  }

  GetDlgItem( IDC_BTN_GOON)->EnableWindow( FALSE);
  gl_dblStartSignCoeff = m_ctlSignCoeff.GetValue();
  gl_nSkipPacks = m_ctlSkipPacks.GetValue();
  gl_nSkipMsecs = m_ctlSkipMsecs.GetValue();

  //заказываем память под измерения
  CMainFrame *pFrm = ( CMainFrame *) AfxGetApp()->GetMainWnd();
  CMainView *pView = ( CMainView *) pFrm->GetActiveView();
  CSlg2Doc *pDoc =   ( CSlg2Doc *) pView->GetDocument();

  long dn100m = ( long) ( ceil( pDoc->m_dblMeasDuration * 10));
  pDoc->AllocMem( m_pStep1.lPacks, dn100m, gl_bLineTact, gl_bLine100ms, gl_bLine1s, gl_bLine10s, gl_bLine100s, gl_bLine10m);

  m_lPrevPacks = 0;

  //начинаем третью пробежку
  DWORD id2;
  HANDLE hthread2 = ::CreateThread( 0, 0, &LoadFile2Thread, this, 0, &id2);          
  SetTimer( TIMER_REFRESH_VALUES2, 500, NULL);	
  SetThreadPriority( hthread2, THREAD_PRIORITY_HIGHEST);
}

void COpenMeasDlg::OnRadSkipPacks() 
{
	m_ctlSkipPacks.SetEnabled( TRUE);	
  m_ctlSkipMsecs.SetEnabled( FALSE);
}

void COpenMeasDlg::OnRadSkipTime() 
{
  m_ctlSkipPacks.SetEnabled( FALSE);	
  m_ctlSkipMsecs.SetEnabled( TRUE);
}

void COpenMeasDlg::OnBtnStep1DeccoeffFirst() 
{
  CString strTmp;
  GetDlgItem( IDC_BTN_STEP1_DECCOEFF_FIRST)->GetWindowText( strTmp);
  strTmp = strTmp.Right( 8);
  double dblValue = atof( strTmp);
  strTmp.Format( "%.5f", dblValue);
  GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);
}

void COpenMeasDlg::OnBtnStep1DeccoeffLast() 
{
	CString strTmp;
  GetDlgItem( IDC_BTN_STEP1_DECCOEFF_LAST)->GetWindowText( strTmp);
  strTmp = strTmp.Right( 8);
  double dblValue = atof( strTmp);
  strTmp.Format( "%.5f", dblValue);
  GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);
}

void COpenMeasDlg::OnBtnStep1DeccoeffMin() 
{
	CString strTmp;
  GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MIN)->GetWindowText( strTmp);
  strTmp = strTmp.Right( 8);
  double dblValue = atof( strTmp);
  strTmp.Format( "%.5f", dblValue);
  GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);
}

void COpenMeasDlg::OnBtnStep1DeccoeffAvg() 
{
	CString strTmp;
  GetDlgItem( IDC_BTN_STEP1_DECCOEFF_AVG)->GetWindowText( strTmp);
  strTmp = strTmp.Right( 8);
  double dblValue = atof( strTmp);
  strTmp.Format( "%.5f", dblValue);
  GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);
}

void COpenMeasDlg::OnBtnStep1DeccoeffMax() 
{
	CString strTmp;
  GetDlgItem( IDC_BTN_STEP1_DECCOEFF_MAX)->GetWindowText( strTmp);
  strTmp = strTmp.Right( 8);
  double dblValue = atof( strTmp);
  strTmp.Format( "%.5f", dblValue);
  GetDlgItem( IDC_EDT_DECCOEFF_START)->SetWindowText( strTmp);
}
