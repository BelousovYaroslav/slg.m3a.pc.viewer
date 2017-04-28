// SettingsGraph.h: interface for the CSettingsGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_)
#define AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSettingsGraph  
{
public:
	CSettingsGraph();
	virtual ~CSettingsGraph();

  //������������ �������� (�������� �� ��� Y)
  int  GetAxisY() { return m_nAxisY;}
  void SetAxisY( int nAxY) { m_nAxisY = nAxY;}

  //�����������
  int  GetMeaning() { return m_nMeaning;}
  void SetMeaning( int nNewVal) { m_nMeaning = nNewVal;}

  //�������� ��� �
  int  GetAxisX() { return m_nAxisX;}
  void SetAxisX( int nAxX) { m_nAxisX = nAxX;}

  //Min-max ��������
  BOOL Get_bMinY()            {  return m_bMinY; }
  void Set_bMinY( BOOL bMinY) {  m_bMinY = bMinY; }
  BOOL Get_bMaxY()            {  return m_bMaxY; }
  void Set_bMaxY( BOOL bMaxY) {  m_bMaxY = bMaxY; } 
  double Get_dblMinY()                {  return m_dblMinY; }
  void   Set_dblMinY( double dblMinY) {  m_dblMinY = dblMinY; }
  double Get_dblMaxY()                {  return m_dblMaxY; }
  void   Set_dblMaxY( double dblMaxY) {  m_dblMaxY = dblMaxY; }

  //��������� ����� � ����� ��������
  COLORREF GetLineColor() { return m_clrLineColor;}
  void SetLineColor( COLORREF clr) { m_clrLineColor = clr;}
  
private:
  int m_nAxisY;                   //������������ �� ��� Y ��������
  int m_nAxisX;                   //��� X (�����, �����������)
  int m_nMeaning;                 //���������� ������� 1
  BOOL m_bMinY;                   //���������� ����� minY
  double m_dblMinY;               //minY
  BOOL m_bMaxY;                   //���������� ����� maxY
  double m_dblMaxY;               //maxY
  COLORREF m_clrLineColor;        //���� �����
  //BOOL m_bLine;                   //��������� �������
  //int m_nLineWidth;         //������� �����
};

#endif // !defined(AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_)
