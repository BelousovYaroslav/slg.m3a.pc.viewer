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
  m_dbl_tact = NULL;
  m_dbl_100ms = NULL;
  m_dbl_1s = NULL;
  m_dbl_10s = NULL;
  m_dbl_100s = NULL;

  m_dbl_tact_min = m_dbl_tact_mean = m_dbl_tact_max = m_dbl_tact_rms = 0;
  m_dbl_100m_min = m_dbl_100m_mean = m_dbl_100m_max = m_dbl_100m_rms = 0;
  m_dbl_1s_min   = m_dbl_1s_mean   = m_dbl_1s_max   = m_dbl_1s_rms   = 0;
  m_dbl_10s_min  = m_dbl_10s_mean  = m_dbl_10s_max  = m_dbl_10s_rms  = 0;
  m_dbl_100s_min = m_dbl_100s_mean = m_dbl_100s_max = m_dbl_100s_rms = 0;
}

CDispParam::~CDispParam()
{
  if( m_dbl_tact != NULL)   { free( m_dbl_tact);  m_dbl_tact = NULL; }
  if( m_dbl_100ms != NULL)  { free( m_dbl_100ms); m_dbl_100ms = NULL; }
  if( m_dbl_1s != NULL)     { free( m_dbl_1s);    m_dbl_1s = NULL; }
  if( m_dbl_10s != NULL)    { free( m_dbl_10s);   m_dbl_10s = NULL; }
  if( m_dbl_100s != NULL)   { free( m_dbl_100s);  m_dbl_100s = NULL; }
}

int CDispParam::AllocMem( long lTactPoints, long l100msPoints)
{
  if( m_dbl_tact != NULL)   { free( m_dbl_tact);  m_dbl_tact = NULL; }
  if( m_dbl_100ms != NULL)  { free( m_dbl_100ms); m_dbl_100ms = NULL; }
  if( m_dbl_1s != NULL)     { free( m_dbl_1s);    m_dbl_1s = NULL; }
  if( m_dbl_10s != NULL)    { free( m_dbl_10s);   m_dbl_10s = NULL; }
  if( m_dbl_100s != NULL)   { free( m_dbl_100s);  m_dbl_100s = NULL; }

  m_dbl_tact =  ( double *) malloc( lTactPoints         * sizeof( double));
  m_dbl_100ms = ( double *) malloc( l100msPoints        * sizeof( double));
  m_dbl_1s =    ( double *) malloc( l100msPoints / 10   * sizeof( double));
  m_dbl_10s =   ( double *) malloc( l100msPoints / 100  * sizeof( double));
  m_dbl_100s =  ( double *) malloc( l100msPoints / 1000 * sizeof( double));

  m_dbl_tact_min = m_dbl_tact_mean = m_dbl_tact_max = m_dbl_tact_rms = 0;
  m_dbl_100m_min = m_dbl_100m_mean = m_dbl_100m_max = m_dbl_100m_rms = 0;
  m_dbl_1s_min   = m_dbl_1s_mean   = m_dbl_1s_max   = m_dbl_1s_rms   = 0;
  m_dbl_10s_min  = m_dbl_10s_mean  = m_dbl_10s_max  = m_dbl_10s_rms  = 0;
  m_dbl_100s_min = m_dbl_100s_mean = m_dbl_100s_max = m_dbl_100s_rms = 0;

  return 0;
}
