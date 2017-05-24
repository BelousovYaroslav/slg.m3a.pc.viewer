// slg2Doc.h : interface of the CSlg2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLG2DOC_H__8D5141A6_251C_48D7_AE95_380A460EC4A4__INCLUDED_)
#define AFX_SLG2DOC_H__8D5141A6_251C_48D7_AE95_380A460EC4A4__INCLUDED_

#include "OpenMeasDlg.h"
#include "DispParam.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSlg2Doc : public CDocument
{
protected: // create from serialization only
	CSlg2Doc();
	DECLARE_DYNCREATE(CSlg2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlg2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSlg2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSlg2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void AllocMem( long lPacks, long l100ms);
	BOOL m_bZoomed;
	BOOL m_bShowCursors;
	int m_nStatFileVersion;
	CString m_strCurrentFileName;
  int m_nMcVersionMajor;
  int m_nMcVersionMiddle;
  int m_nMcVersionMinor;
	void RecalculateStatistics( void);
 
  CDispParam m_dpW;
  CDispParam m_dpI1;
  CDispParam m_dpI2;
  CDispParam m_dpVpc;
  CDispParam m_dpAAa;
  CDispParam m_dpAAd;
  CDispParam m_dpAAr;
  CDispParam m_dpT1;
  CDispParam m_dpT2;
  CDispParam m_dpT3;
  CDispParam m_dpTsa;
  CDispParam m_dpDecCoeff;  

	long m_nDataFileLines;
  double m_dblMeasDuration;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLG2DOC_H__8D5141A6_251C_48D7_AE95_380A460EC4A4__INCLUDED_)
