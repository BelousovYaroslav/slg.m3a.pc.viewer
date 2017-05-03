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
	int AllocMem( long lTacts, long l100ms);
	CDispParam();
	virtual ~CDispParam();
private:
  double *m_dbl_tact;
  double m_dbl_tact_min, m_dbl_tact_mean, m_dbl_tact_max, m_dbl_tact_rms;
  double *m_dbl_100ms;
  double m_dbl_100m_min, m_dbl_100m_mean, m_dbl_100m_max, m_dbl_100m_rms;
  double *m_dbl_1s;
  double m_dbl_1s_min,	 m_dbl_1s_mean,		m_dbl_1s_max,		m_dbl_1s_rms;
  double *m_dbl_10s;
  double m_dbl_10s_min,  m_dbl_10s_mean,	m_dbl_10s_max,	m_dbl_10s_rms;
  double *m_dbl_100s;
  double m_dbl_100s_min, m_dbl_100s_mean, m_dbl_100s_max, m_dbl_100s_rms;
};

#endif // !defined(AFX_DISPPARAM_H__CE669996_6B21_4A85_860A_117025DF6A4C__INCLUDED_)
