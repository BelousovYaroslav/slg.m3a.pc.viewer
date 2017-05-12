// DlgHisto.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DlgHisto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHisto dialog


CDlgHisto::CDlgHisto(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHisto::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHisto)
	m_nBins = 20;
	//}}AFX_DATA_INIT
	m_doc = NULL;
	
}


void CDlgHisto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHisto)
	DDX_Control(pDX, IDC_CW_HISTO, m_ctlHisto100m);
	DDX_Text(pDX, IDC_EDT_N_BINS, m_nBins);
	DDX_Control(pDX, IDC_CW_HISTO_1S, m_ctlHisto1s);
	DDX_Control(pDX, IDC_CW_HISTO_10S, m_ctlHisto10s);
	DDX_Control(pDX, IDC_CW_HISTO_100S, m_ctlHisto100s);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHisto, CDialog)
	//{{AFX_MSG_MAP(CDlgHisto)
	ON_EN_KILLFOCUS(IDC_EDT_N_BINS, OnKillfocusEdtNBins)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHisto message handlers

void CDlgHisto::OnKillfocusEdtNBins() 
{
	DrawHisto();
}

void CDlgHisto::DrawHisto()
{

//DEL	UpdateData( TRUE);
//DEL
//DEL	if( m_doc == NULL) return;
//DEL	
//DEL	//************* 100 msec
//DEL	CNiReal64Vector data_100m(m_doc->m_dn100m), axis_100m( m_nBins);
//DEL	CNiIntVector histogram_100m( m_nBins);
//DEL
//DEL	for( int i = 0; i < m_doc->m_dn100m; i++) {
//DEL		data_100m[i] = m_doc->m_dy100m[i];
//DEL	}
//DEL
//DEL	// create histogram
//DEL	double min, max;
//DEL	unsigned int minIndex, maxIndex;
//DEL	data_100m.MinMax( min, minIndex, max, maxIndex);
//DEL	CNiMath::Histogram( data_100m, min, max, m_nBins, histogram_100m, axis_100m);

//DEL	//Plot Data and Histogram
//DEL	m_ctlHisto100m.PlotXvsY(axis_100m, histogram_100m);
	

//DEL	//************* 1 sec
//DEL	CNiReal64Vector data_1s(m_doc->m_dn1s), axis_1s( m_nBins);
//DEL	CNiIntVector histogram_1s( m_nBins);

//DEL	for( i = 0; i < m_doc->m_dn1s; i++) {
//DEL		data_1s[i] = m_doc->m_dy1s[i];
//DEL	}

//DEL	// create histogram
//DEL	/*double min, max;
//DEL	unsigned int minIndex, maxIndex;*/
//DEL	data_1s.MinMax( min, minIndex, max, maxIndex);
//DEL	CNiMath::Histogram( data_1s, min, max, m_nBins, histogram_1s, axis_1s);

//DEL	//Plot Data and Histogram
//DEL	m_ctlHisto1s.PlotXvsY(axis_1s, histogram_1s);

//DEL	//************* 10 sec
//DEL	if( m_doc->m_dn10s > 2.) {
//DEL		CNiReal64Vector data_10s(m_doc->m_dn10s), axis_10s( m_nBins);
//DEL		CNiIntVector histogram_10s( m_nBins);

//DEL		for( i = 0; i < m_doc->m_dn10s; i++) {
//DEL			data_10s[i] = m_doc->m_dy10s[i];
//DEL		}

//DEL		// create histogram
//DEL		data_10s.MinMax( min, minIndex, max, maxIndex);
//DEL		CNiMath::Histogram( data_10s, min, max, m_nBins, histogram_10s, axis_10s);

//DEL		//Plot Data and Histogram
//DEL		m_ctlHisto10s.PlotXvsY(axis_10s, histogram_10s);
//DEL	}

//DEL	//************* 100 sec
//DEL	if( m_doc->m_dn100s > 2.) {
//DEL		CNiReal64Vector data_100s(m_doc->m_dn100s), axis_100s( m_nBins);
//DEL		CNiIntVector histogram_100s( m_nBins);

//DEL		for( i = 0; i < m_doc->m_dn100s; i++) {
//DEL			data_100s[i] = m_doc->m_dy100s[i];
//DEL		}

//DEL		// create histogram
//DEL		data_100s.MinMax( min, minIndex, max, maxIndex);
//DEL		CNiMath::Histogram( data_100s, min, max, m_nBins, histogram_100s, axis_100s);

//DEL		//Plot Data and Histogram
//DEL		m_ctlHisto100s.PlotXvsY(axis_100s, histogram_100s);
//DEL	}
}

void CDlgHisto::OnOK() 
{
	CDialog::OnOK();
}

BOOL CDlgHisto::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DrawHisto();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
