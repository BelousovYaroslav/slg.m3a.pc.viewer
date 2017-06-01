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
  m_adbl_10m_x = NULL;    m_adbl_10m_y = NULL;

  m_dbl_tact_min = m_dbl_tact_mean = m_dbl_tact_max = m_dbl_tact_rms = 0;
  m_dbl_100m_min = m_dbl_100m_mean = m_dbl_100m_max = m_dbl_100m_rms = 0;
  m_dbl_1s_min   = m_dbl_1s_mean   = m_dbl_1s_max   = m_dbl_1s_rms   = 0;
  m_dbl_10s_min  = m_dbl_10s_mean  = m_dbl_10s_max  = m_dbl_10s_rms  = 0;
  m_dbl_100s_min = m_dbl_100s_mean = m_dbl_100s_max = m_dbl_100s_rms = 0;
  m_dbl_10m_min  = m_dbl_10m_mean  = m_dbl_10m_max  = m_dbl_10m_rms  = 0;

  m_lCntTact = m_lCnt100m = m_lCnt1s = m_lCnt10s = m_lCnt100s = m_lCnt10m = 0;

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
  if( m_adbl_10m_x != NULL)    { free( m_adbl_10m_x);   m_adbl_10m_x = NULL; }
  if( m_adbl_10m_y != NULL)    { free( m_adbl_10m_y);   m_adbl_10m_y = NULL; }
  m_bInitialized = FALSE;
}

int CDispParam::AllocMem( long lTactPoints, long l100msPoints, BOOL bTact, BOOL b100ms, BOOL b1s, BOOL b10s, BOOL b100s, BOOL b10m)
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
  if( m_adbl_10m_x != NULL)    { free( m_adbl_10m_x);   m_adbl_10m_x = NULL; }
  if( m_adbl_10m_y != NULL)    { free( m_adbl_10m_y);   m_adbl_10m_y = NULL; }
  m_bInitialized = FALSE;

  if( bTact)  m_adbl_tact_x =  ( double *) malloc( lTactPoints         * sizeof( double));
  if( bTact)  m_adbl_tact_y =  ( double *) malloc( lTactPoints         * sizeof( double));
  if( b100ms) m_adbl_100ms_x = ( double *) malloc( l100msPoints        * sizeof( double));
  if( b100ms) m_adbl_100ms_y = ( double *) malloc( l100msPoints        * sizeof( double));
  if( b1s   ) m_adbl_1s_x =    ( double *) malloc( l100msPoints / 10   * sizeof( double));
  if( b1s   ) m_adbl_1s_y =    ( double *) malloc( l100msPoints / 10   * sizeof( double));
  if( b10s  ) m_adbl_10s_x =   ( double *) malloc( l100msPoints / 100  * sizeof( double));
  if( b10s  ) m_adbl_10s_y =   ( double *) malloc( l100msPoints / 100  * sizeof( double));
  if( b100s ) m_adbl_100s_x =  ( double *) malloc( l100msPoints / 1000 * sizeof( double));
  if( b100s ) m_adbl_100s_y =  ( double *) malloc( l100msPoints / 1000 * sizeof( double));
  if( b10m  ) m_adbl_10m_x =   ( double *) malloc( l100msPoints / 6000 * sizeof( double));
  if( b10m  ) m_adbl_10m_y =   ( double *) malloc( l100msPoints / 6000 * sizeof( double));

  m_dbl_tact_min  = m_dbl_100m_min  = m_dbl_1s_min  = m_dbl_10s_min  = m_dbl_100s_min  = m_dbl_10m_min  = 1.e+99;
  m_dbl_tact_mean = m_dbl_100m_mean = m_dbl_1s_mean = m_dbl_10s_mean = m_dbl_100s_mean = m_dbl_10m_mean = 0.;
  m_dbl_tact_max  = m_dbl_100m_max  = m_dbl_1s_max  = m_dbl_10s_max  = m_dbl_100s_max  = m_dbl_10m_max  = -1.e+99;
  m_dbl_tact_rms  = m_dbl_100m_rms  = m_dbl_1s_rms  = m_dbl_10s_rms  = m_dbl_100s_rms  = m_dbl_10m_rms  = 0;

  m_lAllocTact = lTactPoints;
  m_lAlloc100m = l100msPoints;

  m_lCntTact = m_lCnt100m = m_lCnt1s = m_lCnt10s = m_lCnt100s = m_lCnt10m = 0;
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

void CDispParam::AddPoint_10m( double dblT, double dblValue)
{
  if( m_lCnt10m < m_lAlloc100m / 6000.) {
    m_adbl_10m_x[ m_lCnt10m]   = dblT;
    m_adbl_10m_y[ m_lCnt10m++] = dblValue;

    if( m_dbl_10m_min > dblValue) m_dbl_10m_min = dblValue;
    if( m_dbl_10m_max < dblValue) m_dbl_10m_max = dblValue;
  }
}

void CDispParam::CalcMeanAndRMS()
{
  //ТАКТОВЫЕ
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


  //100ms
  //средние
  dblSumm = 0.;
  for( l=0; l<m_lCnt100m; dblSumm += m_adbl_100ms_y[ l++]);
  m_dbl_100m_mean = dblSumm / ( double) m_lCnt100m;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCnt100m; l++) {
    dblSumm += ( m_adbl_100ms_y[ l] - m_dbl_100m_mean) * ( m_adbl_100ms_y[ l] - m_dbl_100m_mean);
  }
  m_dbl_100m_rms = sqrt( dblSumm / m_lCnt100m);


  //1s
  //средние
  dblSumm = 0.;
  for( l=0; l<m_lCnt1s; dblSumm += m_adbl_1s_y[ l++]);
  m_dbl_1s_mean = dblSumm / ( double) m_lCnt1s;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCnt1s; l++) {
    dblSumm += ( m_adbl_1s_y[ l] - m_dbl_1s_mean) * ( m_adbl_1s_y[ l] - m_dbl_1s_mean);
  }
  m_dbl_1s_rms = sqrt( dblSumm / m_lCnt1s);


  //10s
  //средние
  dblSumm = 0.;
  for( l=0; l<m_lCnt10s; dblSumm += m_adbl_10s_y[ l++]);
  m_dbl_10s_mean = dblSumm / ( double) m_lCnt10s;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCnt10s; l++) {
    dblSumm += ( m_adbl_10s_y[ l] - m_dbl_10s_mean) * ( m_adbl_10s_y[ l] - m_dbl_10s_mean);
  }
  m_dbl_10s_rms = sqrt( dblSumm / m_lCnt10s);


  //100s
  //средние
  dblSumm = 0.;
  for( l=0; l<m_lCnt100s; dblSumm += m_adbl_100s_y[ l++]);
  m_dbl_100s_mean = dblSumm / ( double) m_lCnt100s;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCnt100s; l++) {
    dblSumm += ( m_adbl_100s_y[ l] - m_dbl_100s_mean) * ( m_adbl_100s_y[ l] - m_dbl_100s_mean);
  }
  m_dbl_100s_rms = sqrt( dblSumm / m_lCnt100s);


  //10m
  //средние
  dblSumm = 0.;
  for( l=0; l<m_lCnt10m; dblSumm += m_adbl_10m_y[ l++]);
  m_dbl_10m_mean = dblSumm / ( double) m_lCnt10m;

  //Rms
  dblSumm = 0.;
  for( l=0; l<m_lCnt10m; l++) {
    dblSumm += ( m_adbl_10m_y[ l] - m_dbl_10m_mean) * ( m_adbl_10m_y[ l] - m_dbl_10m_mean);
  }
  m_dbl_10m_rms = sqrt( dblSumm / m_lCnt10m);
}

double * CDispParam::GetY( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_adbl_100ms_y;
    case 2: return m_adbl_1s_y;
    case 3: return m_adbl_10s_y;
    case 4: return m_adbl_100s_y;
    case 5: return m_adbl_10m_y;
    default: return m_adbl_tact_y;
  }
}

double * CDispParam::GetX( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_adbl_100ms_x;
    case 2: return m_adbl_1s_x;
    case 3: return m_adbl_10s_x;
    case 4: return m_adbl_100s_x;
    case 5: return m_adbl_10m_x;
    default: return m_adbl_tact_x;
  }
}

long CDispParam::GetDataLen( int nMeaning) {
  switch( nMeaning) {
    case 1: return m_lCnt100m;
    case 2: return m_lCnt1s;
    case 3: return m_lCnt10s;
    case 4: return m_lCnt100s;
    case 5: return m_lCnt10m;
    default: return m_lCntTact;
  }
}

double CDispParam::GetMin( int nMeaning) {
  double dblResult = .0;
  switch( nMeaning) {
    case 0: dblResult = m_dbl_tact_min; break;
    case 1: dblResult = m_dbl_100m_min; break;
    case 2: dblResult = m_dbl_1s_min;   break;
    case 3: dblResult = m_dbl_10s_min;  break;
    case 4: dblResult = m_dbl_100s_min; break;
    case 5: dblResult = m_dbl_10m_min;  break;
  }
  return dblResult;
}

double CDispParam::GetMax( int nMeaning) {
  double dblResult = .0;
  switch( nMeaning) {
    case 0: dblResult = m_dbl_tact_max; break;
    case 1: dblResult = m_dbl_100m_max; break;
    case 2: dblResult = m_dbl_1s_max;   break;
    case 3: dblResult = m_dbl_10s_max;  break;
    case 4: dblResult = m_dbl_100s_max; break;
    case 5: dblResult = m_dbl_10m_max;  break;
  }
  return dblResult;
}

double CDispParam::GetAvg( int nMeaning) {
  double dblResult = .0;
  switch( nMeaning) {
    case 0: dblResult = m_dbl_tact_mean; break;
    case 1: dblResult = m_dbl_100m_mean; break;
    case 2: dblResult = m_dbl_1s_mean;   break;
    case 3: dblResult = m_dbl_10s_mean;  break;
    case 4: dblResult = m_dbl_100s_mean; break;
    case 5: dblResult = m_dbl_10m_mean;  break;
  }
  return dblResult;
}

double CDispParam::GetRms( int nMeaning) {
  double dblResult = .0;
  switch( nMeaning) {
    case 0: dblResult = m_dbl_tact_rms; break;
    case 1: dblResult = m_dbl_100m_rms; break;
    case 2: dblResult = m_dbl_1s_rms;   break;
    case 3: dblResult = m_dbl_10s_rms;  break;
    case 4: dblResult = m_dbl_100s_rms; break;
    case 5: dblResult = m_dbl_10m_rms;  break;
  }
  return dblResult;
}