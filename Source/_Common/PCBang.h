// PCBang.h: interface for the CPCBang class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBANG_H__DD46F01A_2342_4D10_9A65_C6F56352353C__INCLUDED_)
#define AFX_PCBANG_H__DD46F01A_2342_4D10_9A65_C6F56352353C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if __VER >= 14 // __PCBANG
#ifdef __WORLDSERVER
#include "User.h"
#endif // __WORLDSERVER

class CPCBangInfo
{
#ifdef __WORLDSERVER
public:
	CPCBangInfo( DWORD dwPCBangClass, DWORD dwConnectTime );
	~CPCBangInfo();

	void	SetDisconnect( DWORD dwTime )	{ m_dwDisconnectTime = dwTime; }
	BOOL	ProcessRemove( DWORD dwPlayerId );
	void	UpdateInfo();
private:
	DWORD	m_dwUnitTime;
	DWORD	m_dwDisconnectTime;
#endif // __WORLDSERVER
public:
	DWORD	GetPCBangClass()			{ return m_dwPCBangClass; }
	float	GetExpFactor()				{ return m_fExpFactor; }
	float	GetPieceItemDropFactor()	{ return m_fPieceItemDropFactor; }
	DWORD	GetUnitTime()				{ return (g_tmCurrent - m_dwConnectTime)/MIN(60); }
	void	Serialize( CAr & ar );
#ifdef __CLIENT
	CPCBangInfo();
	~CPCBangInfo();
	static CPCBangInfo* GetInstance();
	DWORD	GetConnectTime()			{ return (g_tmCurrent - m_dwConnectTime)/SEC(1); }
#endif // __CLIENT
	
private:
	DWORD	m_dwPCBangClass;
	DWORD	m_dwConnectTime;
	float	m_fExpFactor;
	float	m_fPieceItemDropFactor;
};


#ifdef __WORLDSERVER
typedef map<DWORD, CPCBangInfo> MAPPBI;

class CPCBang  
{
public:
	CPCBang();
	~CPCBang();
	static CPCBang*	GetInstance();
	
	BOOL	LoadScript();		

	void	SetPCBangPlayer( CUser* pUser, DWORD dwPCBangClass );	// PC�� ������ ��Ͽ� �߰��Ѵ�.
	void	DestroyPCBangPlayer( DWORD dwPlayerId );	// �������� �ð��� �����Ѵ�.
	DWORD	GetPCBangClass( DWORD dwPlayerId );
	void	ProcessPCBang();							// ���� ���� �� 10���� ����� ������ �ʱ�ȭ �Ѵ�.
	float	GetExpInfo( DWORD dwHour );
	float	GetExpFactor( CUser* pUser );				// ������ ����ġ�� ���´�.
	float	GetPartyExpFactor( CUser* apUser[], int nMemberSize );
	float	GetPieceItemDropInfo( DWORD dwHour );
	float	GetPieceItemDropFactor( CUser* pUser );		// ������ ������ ��ӷ��� �����´�.
	void	SetApply( BOOL bApply );
	BOOL	IsApply()				{ return m_bApply; }
	
private:
	CPCBangInfo*	GetPCBangInfo( DWORD dwPlayerId );			// PC�� �����ΰ�?
	int		GetPlayTime( DWORD dwConnectTime ) { return ( g_tmCurrent - dwConnectTime ) / MIN(60); }	// �������� �� �÷��� �ð�
	
	MAPPBI m_mapPCBang;	// PC�� ����� ����
	vector<float>	m_vecfExp;			// �ð��뺰 ����ġ
	vector<float>	m_vecfDropRate;		// �ð��뺰 ������ ��ӷ�
	BOOL	m_bApply;
};
#endif // __WORLDSERVER

#endif // __PCBANG

#endif // !defined(AFX_PCBANG_H__DD46F01A_2342_4D10_9A65_C6F56352353C__INCLUDED_)
