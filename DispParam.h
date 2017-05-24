// DispParam.h: interface for the CDispParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPPARAM_H__CE669996_6B21_4A85_860A_117025DF6A4C__INCLUDED_)
#define AFX_DISPPARAM_H__CE669996_6B21_4A85_860A_117025DF6A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDispParam  
{
public:
  BOOL IsInitialized() { return m_bInitialized; }
	void CalcMeanAndRMS( void);
  void AddPoint_Tact( double dblT, double dblValue);
	void AddPoint_100m( double dblT, double dblValue);
  void AddPoint_1s(   double dblT, double dblValue);
  void AddPoint_10s(  double dblT, double dblValue);
  void AddPoint_100s( double dblT, double dblValue);
	int AllocMem( long lTacts, long l100ms);
	CDispParam();
	virtual ~CDispParam();
  double * GetY( int nMeaning);
  double * GetX( int nMeaning);
  long GetDataLen( int nMeaning);
  double GetMin( int nMeaning);
  double GetMax( int nMeaning);
  double GetAvg( int nMeaning);
  double GetRms( int nMeaning);
private:
  double *m_adbl_tact_y,  *m_adbl_tact_x;
  double m_dbl_tact_min,  m_dbl_tact_mean,  m_dbl_tact_max, m_dbl_tact_rms;

  double *m_adbl_100ms_y, *m_adbl_100ms_x;
  double m_dbl_100m_min,  m_dbl_100m_mean,  m_dbl_100m_max, m_dbl_100m_rms;

  double *m_adbl_1s_y,    *m_adbl_1s_x;
  double m_dbl_1s_min,	  m_dbl_1s_mean,    m_dbl_1s_max,		m_dbl_1s_rms;

  double *m_adbl_10s_y,   *m_adbl_10s_x;
  double m_dbl_10s_min,   m_dbl_10s_mean,	m_dbl_10s_max,	m_dbl_10s_rms;

  double *m_adbl_100s_y,  *m_adbl_100s_x;
  double m_dbl_100s_min, m_dbl_100s_mean, m_dbl_100s_max, m_dbl_100s_rms;

  long m_lCntTact, m_lCnt100m, m_lCnt1s, m_lCnt10s, m_lCnt100s;
  long m_lAllocTact, m_lAlloc100m;
  BOOL m_bInitialized;
};

#endif // !defined(AFX_DISPPARAM_H__CE669996_6B21_4A85_860A_117025DF6A4C__INCLUDED_)
