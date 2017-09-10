// WantedListSnapshot.h: interface for the CWantedListSnapshot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTEDLISTSNAPSHOT_H__09BA82BC_185D_45B2_9245_C2F16E117E62__INCLUDED_)
#define AFX_WANTEDLISTSNAPSHOT_H__09BA82BC_185D_45B2_9245_C2F16E117E62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "misc.h"		// WANTED_ENTRY

typedef std::vector<WANTED_ENTRY>	WANTED_ENTRY_VECTOR;

// ����Ʈ ǥ�ÿ� ��ü ( �ǽð� ������Ʈ ���� �ʱ� ������ ���� �� �� )
class CWantedListSnapshot  
{
public:
	CWantedListSnapshot();
	virtual ~CWantedListSnapshot();

protected:
	long							m_lRecvTime;		// ����Ʈ�� ���� �ð� 
	WANTED_ENTRY_VECTOR				m_wantedList;

public:
	static CWantedListSnapshot&		GetInstance();
	void							Read( CAr& ar );
	void							Write( CAr& ar );
	int								GetPlayerIndex( LPCTSTR lpszPlayer );
};

#endif // !defined(AFX_WANTEDLISTSNAPSHOT_H__09BA82BC_185D_45B2_9245_C2F16E117E62__INCLUDED_)
