// PackProcessing.h: interface for the CPackProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKPROCESSING_H__8E1E92F0_812C_4380_ACCF_17EF6347A0C3__INCLUDED_)
#define AFX_PACKPROCESSING_H__8E1E92F0_812C_4380_ACCF_17EF6347A0C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPackProcessing  
{
public:
	CPackProcessing();
	virtual ~CPackProcessing();
  
  //Incoming
  unsigned char bt3; //dW ( в случае 3.2.3 может быть dN,dU)
  unsigned char bt4;
  unsigned char bt5;
  unsigned char bt6;

  unsigned char bt7;    //analog param indicator
  
  unsigned char bt8;    //analog param value
  unsigned char bt9; 
  
  unsigned char bt10;   //tact time
  unsigned char bt11;

  unsigned char bt12;   //counter

  unsigned char bt13;   //emergency code

  double m_dblDecCoeff;  //может понадобиться если версия МК 3,2,3 и приходит dN,dU
  short  m_shSignCoeff;  //может понадобиться если версия МК 3,2,3 и приходит dN,dU

  //Outgoing
  double m_dblPhi;
  double m_dblTime;
  int m_nAnParam;
  double m_dblAnParamValue;

  int ProcessPack_4_1_7( void);
  int ProcessPack_4_2_0( void);
  int ProcessPack_4_2_1( void);
  int ProcessPack_3_2_2( void);
  int ProcessPack_3_2_3( void);
  int ProcessPack_3_2_5( void);

  int ProcessPackTime_3_2_2( void);
  int ProcessPackTime_3_2_3( void);
  int ProcessPackTime_3_2_4( void);
  int ProcessPackTime_3_2_5( void);
  int ProcessPackTime_4_1_7( void);
  int ProcessPackTime_4_2_0( void);
  int ProcessPackTime_4_2_1( void);

  BOOL m_bVeracity;
  BOOL m_bLockBit;
  BOOL m_bSyncAsync;
  BOOL m_bdWdNdU;
  short m_shErrorCode;
};

#endif // !defined(AFX_PACKPROCESSING_H__8E1E92F0_812C_4380_ACCF_17EF6347A0C3__INCLUDED_)
