// SecretRoom.h: interface for the CSecretRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECRETROOM_H__E848C037_E4D5_44ED_BC88_2CDBE643B64F__INCLUDED_)
#define AFX_SECRETROOM_H__E848C037_E4D5_44ED_BC88_2CDBE643B64F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if __VER >= 12 // __SECRET_ROOM
#include "Continent.h"
////////////////////////////////////////////////////////////////

enum { SRMNG_CLOSE = 1, SRMNG_OPEN, SRMNG_LINEUP, SRMNG_ENTRANCE, SRMNG_WARWAIT, SRMNG_WAR };
enum { SRCONT_CLOSE = 101, SRCONT_OPEN, SRCONT_WAR, SRCONT_CLOSEWAIT };

#define MONSTER_FAILED	0
#define MONSTER_NORMAL	1
#define MONSTER_MIDBOSS	2
#define MONSTER_BOSS	3
#define MONSTER_WIN		4



struct __SECRETROOM_MONSTER
{
	DWORD	dwId;
	int		nNum;
	BOOL	bRed;
	int		x1, z1, x2, z2, y;
	__SECRETROOM_MONSTER()
	{
		dwId = NULL_ID;
		bRed = FALSE;
		nNum = x1 = z1 = x2 = z2 = y = 0;
	}
};


struct __SECRETROOM_TENDER
{
	DWORD	dwGuildId;
	int		nPenya;
	int		nKillCount;
	DWORD	dwWorldId;
	int		nWarState;
	vector<DWORD>	vecLineUpMember;
	vector<__SECRETROOM_MONSTER> vecMonster;
	__SECRETROOM_TENDER()
	{
		dwGuildId = NULL_ID;
		nPenya = 0;
		nKillCount = 0;
		dwWorldId = NULL_ID;
		nWarState = MONSTER_NORMAL;
	}

	bool operator ==( DWORD dwGuildId )
	{
		return this->dwGuildId == dwGuildId;
	}
};

class CSecretRoomContinent;
class CSecretRoom;

class CSecretRoomMng
{
public:	
	CSecretRoomMng();
	virtual ~CSecretRoomMng();

	static CSecretRoomMng*	GetInstance( void );
	
	BYTE GetContinent( CMover* pMover );
	BOOL IsInTheSecretRoom( CMover* pMover );

	map<BYTE, CSecretRoomContinent*>	m_mapSecretRoomContinent;
	
	int	m_nMinGuildLevel;			// ���� ���� �ּ� ��� ����
	int m_nMinGuildMemberLevel;		// ���� ������ �ּ� ����
	int m_nMinPenya;				// �ּ� ���� ���
	int m_nMinGuildNum;				// ���� ���� ��� ��
	int m_nMaxGuildNum;				// ���� ���� ��� ��
	int m_nMinGuildMemberNum;		// �ּ� ���� ���� ��
	int m_nMaxGuildMemberNum;		// �ִ� ���� ���� ��
	int m_nCancelReturnRate;		// ���� ��� ��� ��ȯ��
	int m_nDropoutReturnRate;		// ���� ���� ��� ��ȯ��

	int m_nMonsterGenNum;			// �߰� ���� ���� ���� ���� ��

	int		m_nState;				// ���� state
	DWORD	m_dwRemainTime;			// ���� state�� ���� �ð�

	map<int, int>	m_mapMonsterNum;	// �� ������ �� ��ü��(������ ��ü���� �����ϱ� ������ �ʿ��ϴ�.)
#ifdef __CLIENT
	CSecretRoomContinent* m_pSRCont;
#endif // __CLIENT
#ifdef __WORLDSERVER	
	BOOL LoadScript();
	void Process();			//  ���� ������ ���ǵǾ� �����ϱ� �ڵ� �����Ҷ� ������ �ʿ��ϸ� ������� Ȯ������!!!
	BOOL CheckOpenTime();	// ����� �� ���½ð����� �˻�
	void SendNowState( CUser* pUser, int nState, DWORD dwRemainTime );
	void SendNowStateAllMember( BYTE nCont = CONT_NODATA, int nState = 0, DWORD dwRemainTime = 0 );
	void GetAllInfo( CUser* pUser );				// ���ӽ� ���� �������� �������̸� ����� �� ���� ������ ����
	BOOL IsPossibleGuild( CUser* pUser );			// ���� ������ ����ΰ�?
	BOOL IsGuildMaster( CUser* pUser );				// ��� ������ �ΰ�?
	BOOL IsOtherTenderGuild( CUser* pUser );		// �ٸ� ����� ������ ����ΰ�?
	void SetTenderOpenWnd( CUser* pUser );			// ����� �� ��ûâ�� ���� �ʿ��� ���� ����
	void SetTender( CUser* pUser, int nPenya );		// ����
	void SetTenderCancelReturn( CUser* pUser );		// ���� ��ҽ� �����Ḧ ������ ��û���� ��ȯ�Ѵ�.
	void SetLineUpOpenWnd( CUser* pUser );			// ������ ���� â�� ���� �ʿ��� ���� ����
	void SetLineUp( CUser* pUser, vector<DWORD> vecLineUpMember );	// ������ ����
	void GetTenderView( CUser* pUser );				// ������Ȳ â�� ���� 
	void SecretRoomOpen();							// ���� ����� ���� �����Ѵ�.
	void SetTeleportSecretRoom( CUser* pUser );		// ����� ������ �ڷ���Ʈ...
	void SetSecretRoomKill( DWORD dwWorldId, DWORD dwMonsterId );	// ����� �濡�� ���͸� kill������ ó��
	void SetFailGuild( CUser* pUser );				// �渶�� �װų� �α� �ƿ��� �й� ó��
	D3DXVECTOR3 GetRevivalPos( CUser* pUser );		// ĳ���� �����, �����, �α� �ƿ��� �� ĳ���� ��Ȱ��ġ

	CLuaBase m_Lua;

	vector<__SECRETROOM_MONSTER> m_vecNormalMonster;	// �Ϲ� ���� ����, ��ġ, ��ü��, ���� �Ӽ�
	vector<__SECRETROOM_MONSTER> m_vecMidBossMonster;	// �߰����� ���� ����, ��ġ, ��ü��, ���� �Ӽ�
	vector<__SECRETROOM_MONSTER> m_vecBossMonster;		// �������� ���� ����, ��ġ, ��ü��, ���� �Ӽ�
#endif // __WORLDSERVER
};


class CSecretRoomContinent  
{
public:
	CSecretRoomContinent( BYTE nCont );
	virtual ~CSecretRoomContinent();

	// CSecretRoomMng�� ���� ������ �Լ��� ���� ��κ��̴�.

	int		m_nState;
	DWORD	m_dwRemainTime;
	BYTE	m_nContinent;	// ���� ����Ǵ� ����� ���� �ش��ϴ� ���(����, ����)
	vector<__SECRETROOM_TENDER> m_vecSecretRoomTender;		// ��û ��� ���

	int		GetTenderGuild( DWORD dwGuildId );			// ��û�� ����̸� m_vecSecretRoomTender�� index�� ��ȯ�Ѵ�.
#ifdef __WORLDSERVER
	void	Process( DWORD dwTick );	
	BOOL	IsLineUpMember( CUser* pUser );				// ������ ������ �ΰ�?
	void	SetContTender( CUser* pUser, int nPenya );	// ���� �ش� ����� ����
	void	SortTenderGuild();							// ������ ������ ����
	void	SetContTenderCancelReturn( CUser* pUser );	// ���� ��ҽ� ��ȯ
	void	SetContLineUp( CUser* pUser, vector<DWORD> vecLineUpMember );	// ������ ����
	void	SecretRoomContOpen();							// ���� ����� ����� �� ����
	void	SetSecretRoomWorld( int nContNum );				// �� ��带 ����� �� 16���߿� ������ �ش�.
	void	SetTeleportSecretRoomCont( CUser* pUser );		// �ش� ����� ������ �ڷ���Ʈ...
	void	SetTeleportNormalRoom( CUser* pUser, int nIndex );		// �Ϲ� ���� ��ġ�� �ڷ���Ʈ...
	void	SetTeleportBossRoom( CUser* pUser, int nIndex );		// ���������� ��� �ڷ���Ʈ ��ġ�� �ٸ���.
	void	CreateMonster();			// ���͸� ������ ����(��ũ��Ʈ)��ŭ�� �ܰ������� �����Ѵ�.
	void	RemoveAllSecretRoomObj( DWORD dwWorldId );		// ����� �濡�� �й��ϰų� �¸���尡 ������ ��� ���� ����
	void	SetSecretRoomContKill( DWORD dwWorldId, DWORD dwMonsterId, BOOL bKill = TRUE );	// ���� kill�� ó��(�¸� ���, �����ܰ� �� ó���Ѵ�.)
	BOOL	IsSecretRoomMonster( DWORD dwMonsterId, int nWarState );	// ���� kill�� �ش� ���Ͱ� ����� �� �����ΰ�?
	void	UpdateInfoToMember( CUser* pUser, BYTE nType, int nIndex = NULL_ID );	// kill count�� client�� �˷��ش�.
	void	UpdateInfoToAllMember( BYTE nType, int nIndex = NULL_ID );		// �� ������ ������ ��� client�� ����(�ش� �����)
	void	ProcessInGuildMaster();	// �渶�� �ش� ����� �濡 �����ϴ� �˻�
	void	SetContFailGuild( DWORD dwGuildId );			// �渶�� �װų� �α� �ƿ��� �й� ó��
	void	SetContCloseWait();								// ����� ���� ����ǰ� ���ð�...
	void	SetContClose();									// ����� �� ���� ����

	int		m_nFailedGuild;	 //�й��� ��� ��(��û��� ���� ������ �¸� ��� ���� ����)
#endif // __WORLDSERVER

};

/////////////////////////////////////////////////////////////////////
#endif // __SECRET_ROOM

#endif // !defined(AFX_SECRETROOM_H__E848C037_E4D5_44ED_BC88_2CDBE643B64F__INCLUDED_)


