// DlgSelectMcVer.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DlgSelectMcVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectMcVer dialog


CDlgSelectMcVer::CDlgSelectMcVer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectMcVer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectMcVer)
	m_nRadSelect = 3;
	//}}AFX_DATA_INIT
}


void CDlgSelectMcVer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectMcVer)
	DDX_Radio(pDX, IDC_RAD_318, m_nRadSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectMcVer, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectMcVer)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectMcVer message handlers
