// PackProcessing.cpp: implementation of the CPackProcessing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "PackProcessing.h"
#include "AnalogueParamsConstList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackProcessing::CPackProcessing()
{
  m_dblDecCoeff = -1.;
  m_shSignCoeff = 0;
}

CPackProcessing::~CPackProcessing()
{

}

int CPackProcessing::ProcessPack_323( void) {
  //время такта
  short nSaTime = ( bt11 << 8) + bt10;
	m_dblTime = nSaTime / 32768.;

  m_bVeracity  = ( bt13 & 0x80) ? TRUE : FALSE;
  m_bLockBit   = ( bt13 & 0x40) ? TRUE : FALSE;
  m_bSyncAsync = ( bt13 & 0x20) ? TRUE : FALSE;
  m_bdWdNdU    = ( bt13 & 0x10) ? TRUE : FALSE;
  m_shErrorCode =  bt13 & 0x0F;

  //индикатор аналогового параметра
  m_nAnParam = bt7;

  //аналоговый параметр
  unsigned short shCur1 = ( bt9 << 8) + bt8;
  double dblCur1 = ( double) shCur1;
  switch( m_nAnParam) {
    case UTD1: 
		case UTD2: 
    case UTD3:
      m_dblAnParamValue = shCur1 / 65535. * 200. - 100.;
    break;

		case I1:
		case I2:
      m_dblAnParamValue = ( 2.5 - shCur1 / 4096. * 3.) / 2.5;
    break;

		case CNTRPC:
      m_dblAnParamValue = ( ( shCur1 / 4096. * 3.) - 2.048) * 100.;
    break;

		case AMPLANG_ALTERA:
      m_dblAnParamValue = shCur1 / 4.                              //imp
                    * ( ( CSlg2App *) AfxGetApp())->m_dKimpSec;	  //''
    break;
    
    case AMPLANG_DUS:
      m_dblAnParamValue = shCur1 / 4096. * 3.;            //V
    break;

    /*
		case AMPLITUDE: theApp.m_btParam1 = nCur1;        break;        //Амплитуда колебаний виброподвеса
		case TACT_CODE: theApp.m_btParam2 = nCur1;        break;        //Код такта подставки
		case M_COEFF:   theApp.m_btParam3 = nCur1;        break;        //коэффициент ошумления
		case STARTMODE: theApp.m_btParam4 = nCur1;        break;        //Начальная мода
    case DECCOEFF:    theApp.m_shFlashDecCoeff = nCur1; break;      //коэффициент вычета

    case CONTROL_I1:  theApp.m_shFlashI1min = nCur1;  break;        //Контрольный ток I1
		case CONTROL_I2:  theApp.m_shFlashI2min = nCur1;  break;        //Контрольный ток I2
		case CONTROL_AA:  theApp.m_shFlashAmplAng1min = nCur1; break;   //Контрольный сигнал раскачки AmplAng1
					
    case HV_APPLY_COUNT_TR: theApp.m_nHvAppliesThisRun = nCur1; break;  //число 3kV-импульсов поджига

		case SIGNCOEFF:
		  ( ( CSlg2App *) AfxGetApp())->m_shSignCoeff = (( signed short) nCur1) - 1;
    break;

    case VERSION:
          ( ( CSlg2App *) AfxGetApp())->m_strSoftwareVer.Format(_T("v%d.%d.%d.0"), 
				    	( byte6 / 16),		//major
							  byte6 % 16,			//middle
							  byte7 / 16);		//minor
    break; 

		case CALIB_T1:
		  ( ( CSlg2App *) AfxGetApp())->m_shFlashT1 = nCur1;
			( ( CSlg2App *) AfxGetApp())->m_shFlashT1 -= THERMO_CALIB_PARAMS_BASE;
			
    break;		//калибровка термодатчиков: min T

		case T1_TD1: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD1_1 = nCur1; break; //калибровка термодатчиков: TD1_minT
		case T1_TD2: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD2_1 = nCur1; break; //калибровка термодатчиков: TD2_minT
    case T1_TD3: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD3_1 = nCur1; break; //калибровка термодатчиков: TD3_minT

    case CALIB_T2:
					( ( CSlg2App *) AfxGetApp())->m_shFlashT2 = nCur1;
					( ( CSlg2App *) AfxGetApp())->m_shFlashT2 -= THERMO_CALIB_PARAMS_BASE;
    break;		//калибровка термодатчиков: max T

		case T2_TD1: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD1_2 = nCur1; break; //калибровка термодатчиков: TD1_maxT
		case T2_TD2: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD2_2 = nCur1; break; //калибровка термодатчиков: TD2_maxT
    case T2_TD3: ( ( CSlg2App *) AfxGetApp())->m_shFlashTD3_2 = nCur1; break; //калибровка термодатчиков: TD3_maxT
    */
  }

  short dN, dU;
  if( m_bdWdNdU == TRUE) {
    //режим рассчёта коэффициента вычета
    //От гироскопа нам передаются dN (short) и dU(short)
        
        
    //И нам нужен коэффициент вычета, а то без него мы не сможем вычислить Phi
    if( m_dblDecCoeff == -1.) {
      if( m_nAnParam == DECCOEFF)
        m_dblDecCoeff = m_dblAnParamValue;
      else
        return 1;
    }

    //и нам нужен знаковый коэффициент
    if( m_shSignCoeff == 0) {
      if( m_nAnParam == SIGNCOEFF)
         m_shSignCoeff = m_dblAnParamValue;
      else
        return 2;
    }
        

    char *ptr;
    ptr = ( char *) &dN;
		ptr[0] = bt3;
		ptr[1] = bt4;

    ptr = ( char *) &dU;
		ptr[0] = bt5;
		ptr[1] = bt6;

    m_dblPhi = ( double) dN - ( ( double) dU * m_dblDecCoeff * ( double) m_shSignCoeff);

    if( fabs( m_dblPhi) > 100.) {
      CString strTmp;
      strTmp.Format( _T("OOPS! dN:%d dU:%d\n"), dN, dU);
      OutputDebugString( strTmp);
    }
  }
  else {
    //рабочий режим (передаётся phi)

    //float f_dN;
    int n_dN;
    char *ptr;      
    
    ptr = ( char *) &n_dN;
		
    ptr[0] = bt3;
		ptr[1] = bt4;
		ptr[2] = bt5;
		ptr[3] = bt6;

		m_dblPhi = ( ( double) n_dN / 2147483647. * 99310.);
  }

  return 0;
}
