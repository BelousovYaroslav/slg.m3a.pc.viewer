// DispParam.cpp: implementation of the CDispParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "DispParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDispParam::CDispParam()
{
  m_adbl_tact_x = NULL;   m_adbl_tact_y = NULL;
  m_adbl_100ms_x = NULL;  m_adbl_100ms_y = NULL;
  m_adbl_1s_x = NULL;     m_adbl_1s_y = NULL;
  m_adbl_10s_x = NULL;    m_adbl_10s_y = NULL;
  m_adbl_100s_x = NULL;   m_adbl_100s_y = NULL;

  m_dbl_tact_min = m_dbl_tact_mean = m_dbl_tact_max = m_dbl_tact_rms = 0;
  m_dbl_100m_min = m_dbl_100m_mean = m_dbl_100m_max = m_dbl_100m_rms = 0;
  m_dbl_1s_min   = m_dbl_1s_mean   = m_dbl_1s_max   = m_dbl_1s_rms   = 0;
  m_dbl_10s_min  = m_dbl_10s_mean  = m_dbl_10s_max  = m_dbl_10s_rms  = 0;
  m_dbl_100s_min = m_dbl_100s_mean = m_dbl_100s_max = m_dbl_100s_rms = 0;

  m_lCntTact = m_lCnt100m = m_lCnt1s = m_lCnt10s = m_lCnt100s = 0;

  m_bInitialized = FALSE;
}

CDispParam::~CDispParam()
{
  if( m_adbl_tact_x != NULL)   { free( m_adbl_tact_x);  m_adbl_tact_x = NULL; }
  if( m_adbl_tact_y != NULL)   { free( m_adbl_tact_y);  m_adbl_tact_y = NULL; }
  if( m_adbl_100ms_x != NULL)  { free( m_adbl_100ms_x); m_adbl_100ms_x = NULL; }
  if( m_adbl_100ms_y != NULL)  { free( m_adbl_100ms_y); m_adbl_100ms_y = NULL; }
  if( m_adbl_1s_x != NULL)     { free( m_adbl_1s_x);    m_adbl_1s_x = NULL; }
  if( m_adbl_1s_y != NULL)     { free( m_adbl_1s_y);    m_adbl_1s_y = NULL; }
  if( m_adbl_10s_x != NULL)    { free( m_adbl_10s_x);   m_adbl_10s_x = NULL; }
  if( m_adbl_10s_y != NULL)    { free( m_adbl_10s_y);   m_adbl_10s_y = NULL; }
  if( m_adbl_100s_x != NULL)   { free( m_adbl_100s_x);  m_adbl_100s_x = NULL; }
  if( m_adbl_100s_y != NULL)   { free( m_adbl_100s_y);  m_adbl_100s_y = NULL; }
  m_bInitialized = FALSE;
}

int CDispParam::AllocMem( long lTactPoints, long l100msPoints)
{
  if( m_adbl_tact_x != NULL)   { free( m_adbl_tact_x);  m_adbl_tact_x = NULL; }
  if( m_adbl_tact_y != NULL)   { free( m_adbl_tact_y);  m_adbl_tact_y = NULL; }
  if( m_adbl_100ms_x != NULL)  { free( m_adbl_100ms_x); m_adbl_100ms_x = NULL; }
  if( m_adbl_100ms_y != NULL)  { free( m_adbl_100ms_y); m_adbl_100ms_y = NULL; }
  if( m_adbl_1s_x != NULL)     { free( m_adbl_1s_x);    m_adbl_1s_x = NULL; }
  if( m_adbl_1s_y != NULL)     { free( m_adbl_1s_y);    m_adbl_1s_y = NULL; }
  if( m_adbl_10s_x != NULL)    { free( m_adbl_10s_x);   m_adbl_10s_x = NULL; }
  if( m_adbl_10s_y != NULL)    { free( m_adbl_10s_y);   m_adbl_10s_y = NULL; }
  if( m_adbl_100s_x != NULL)   { free( m_adbl_100s_x);  m_adbl_100s_x = NULL; }
  if( m_adbl_100s_y != NULL)   { free( m_adbl_100s_y);  m_adbl_100s_y = NULL; }
  m_bInitialized = FALSE;

  m_adbl_tact_x =  ( double *) malloc( lTactPoints         * sizeof( double));
  m_adbl_tact_y =  ( double *) malloc( lTactPoints         * sizeof( double));
  m_adbl_100ms_x = ( double *) malloc( l100msPoints        * sizeof( double));
  m_adbl_100ms_y = ( double *) malloc( l100msPoints        * sizeof( double));
  m_adbl_1s_x =    ( double *) malloc( l100msPoints / 10   * sizeof( double));
  m_adbl_1s_y =    ( double *) malloc( l100msPoints / 10   * sizeof( double));
  m_adbl_10s_x =   ( double *) malloc( l100msPoints / 100  * sizeof( double));
  m_adbl_10s_y =   ( double *) malloc( l100msPoints / 100  * sizeof( double));
  m_adbl_100s_x =  ( double *) malloc( l100msPoints / 1000 * sizeof( double));
  m_adbl_100s_y =  ( double *) malloc( l100msPoints / 1000 * sizeof( double));

  m_dbl_tact_min = m_dbl_tact_mean = m_dbl_tact_max = m_dbl_tact_rms = 0;
  m_dbl_100m_min = m_dbl_100m_mean = m_dbl_100m_max = m_dbl_100m_rms = 0;
  m_dbl_1s_min   = m_dbl_1s_mean   = m_dbl_1s_max   = m_dbl_1s_rms   = 0;
  m_dbl_10s_min  = m_dbl_10s_mean  = m_dbl_10s_max  = m_dbl_10s_rms  = 0;
  m_dbl_100s_min = m_dbl_100s_mean = m_dbl_100s_max = m_dbl_100s_rms = 0;

  m_lAllocTact = lTactPoints;
  m_lAlloc100m = l100msPoints;

  m_lCntTact = m_lCnt100m = m_lCnt1s = m_lCnt10s = m_lCnt100s = 0;
  m_bInitialized = TRUE;
  return 0;
}

void CDispParam::AddPoint_Tact( double dblT, double dblValue)
{
  if( m_lCntTact < m_lAllocTact) {
    m_adbl_tact_x[ m_lCntTact]    = dblT;
    m_adbl_tact_y[ m_lCntTact++]  = dblValue;

    if( m_dbl_tact_min > dblValue) m_dbl_tact_min = dblValue;
    if( m_dbl_tact_max < dblValue) m_dbl_tact_max = dblValue;
  }
}

void CDispParam::AddPoint_100m( double dblT, double dblValue)
{
  if( m_lCnt100m < m_lAllocTact) {
    m_adbl_100ms_x[ m_lCnt100m]   = dblT;
    m_adbl_100ms_y[ m_lCnt100m++] = dblValue;

    if( m_dbl_100m_min > dblValue) m_dbl_100m_min = dblValue;
    if( m_dbl_100m_max < dblValue) m_dbl_100m_max = dblValue;
  }
}

void CDispParam::AddPoint_1s( double dblT, double dblValue)
{
  if( m_lCnt1s < m_lAlloc100m / 10.) {
    m_adbl_1s_x[ m_lCnt1s]   = dblT;
    m_adbl_1s_y[ m_lCnt1s++] = dblValue;

    if( m_dbl_1s_min > dblValue) m_dbl_1s_min = dblValue;
    if( m_dbl_1s_max < dblValue) m_dbl_1s_max = dblValue;
  }
}

void CDispParam::AddPoint_10s( double dblT, double dblValue)
{
  if( m_lCnt10s < m_lAlloc100m / 100.) {
    m_adbl_10s_x[ m_lCnt10s]   = dblT;
    m_adbl_10s_y[ m_lCnt10s++] = dblValue;

    if( m_dbl_10s_min > dblValue) m_dbl_10s_min = dblValue;
    if( m_dbl_10s_max < dblValue) m_dbl_10s_max = dblValue;
  }
}

void CDispParam::AddPoint_100s( double dblT, double dblValue)
{
  if( m_lCnt100s < m_lAlloc100m / 1000.) {
    m_adbl_100s_x[ m_lCnt100s]   = dblT;
    m_adbl_100s_y[ m_lCnt100s++] = dblValue;

    if( m_dbl_100s_min > dblValue) m_dbl_100s_min = dblValue;
    if( m_dbl_100s_max < dblValue) m_dbl_100s_max = dblValue;
  }
}

void CDispParam::CalcMeanAndRMS()
{
  //средние
  double dblSumm = 0.;
  for( long l=0; l<m_lCntTact; dblSumm += m_adbl_tact_y[ l++]);
  m_dbl_tact_mean = dblSumm / ( double) m_lCntTact;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCntTact; l++) {
    dblSumm += ( m_adbl_tact_y[ l] - m_dbl_tact_mean) * ( m_adbl_tact_y[ l] - m_dbl_tact_mean);
  }
  m_dbl_tact_rms = sqrt( dblSumm / m_lCntTact); 
}

double * CDispParam::GetY( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_adbl_100ms_y;
    case 2: return m_adbl_1s_y;
    case 3: return m_adbl_10s_y;
    case 4: return m_adbl_100s_y;
    default: return m_adbl_tact_y;
  }
}

double * CDispParam::GetX( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_adbl_100ms_x;
    case 2: return m_adbl_1s_x;
    case 3: return m_adbl_10s_x;
    case 4: return m_adbl_100s_x;
    default: return m_adbl_tact_x;
  }
}

long CDispParam::GetDataLen( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_lCnt100m;
    case 2: return m_lCnt1s;
    case 3: return m_lCnt10s;
    case 4: return m_lCnt100s;
    default: return m_lCntTact;
  }
}