#ifndef __GUILD_H__
#define	__GUILD_H__

#include "mempooler.h"
#include <map>
#include <string>
#include "ar.h"
#include "script.h"
#if defined(__DBSERVER) || defined(__CORESERVER)
#include "..\DatabaseServer\project.h"
#include "..\_Network\Objects\obj.h"
#endif

#include "guildquest.h"

////////////////////////////////////////////////////////////////////////////////
// ������ ��� 
////////////////////////////////////////////////////////////////////////////////

const int MAX_GM_ALIAS			= 48;
const int MAX_GM_LEVEL			= 5;
const int MAX_GM_SIZE			= 128;
const int MAX_SQL_G_NAME		= 16;
const int MAX_G_NAME			= 48;
const int MAX_BYTE_NOTICE		= 128;		// ���������� ���� �ִ���� 
const int MAX_GUILD_LEVEL		= 50;		// ��� �ִ뷹�� 	
const int MAX_BYTE_VOTETITLE	= 26;		// ��ǥ ���� ���� �ִ� ���� 
const int MAX_BYTE_VOTEQUESTION	= 171;		// ��ǥ ���� ���� �ִ� ���� 
const int MAX_BYTE_VOTESELECT   = 21;		// ��ǥ ���û��� ���� �ִ� ���� 
const int MAX_VOTE_ENTRY		= 20;		// ��尡 ��ǥ�� �ִ� � ������ �ִ°�

#define GM_MASTER 1
#define GM_KINGPIN 5
#define GM_CAPTAIN 10
#define GM_SUPPORTER 20
#define GM_ROOKIE 80

const DWORD PF_MEMBERLEVEL		= 0x00000001;
const DWORD PF_LEVEL			= 0x00000002;
const DWORD PF_INVITATION		= 0x00000004;
const DWORD PF_PENYA			= 0x00000008;
const DWORD PF_ITEM				= 0x00000010;
#if __VER >= 15 // __GUILD_HOUSE
const DWORD PF_GUILDHOUSE_FURNITURE		= 0x00000020;
const DWORD PF_GUILDHOUSE_UPKEEP		= 0x00000040;
#endif // __GUILD_HOUSE

typedef struct _SGuildMsgHeader
{
	enum
	{
		GUILD_BANK		= 0x0001,
		PENYA			= 0x0001,
		ITEM			= 0x0002,
	};

	union
	{
		DWORD		HeadA;			//
		struct 
		{
			WORD	HeadASub;		//	��� ���̵�
			WORD	HeadAMain;		//	������Ʈ�� �� Ÿ�� ����
		};
	};

	union
	{
		DWORD		HeadB;			//	������Ʈ Ÿ��
		struct 
		{
			WORD	HeadBSub;		//	�뵵�� �°� �ɰ� ����.
			WORD	HeadBMain;
		};
	};

}GUILD_MSG_HEADER, *LPGUILD_MSG_HEADER;

////////////////////////////////////////////////////////////////////////////////
//��ǥ ���� 
////////////////////////////////////////////////////////////////////////////////

struct VOTE_INSERTED_INFO
{
	u_long  idGuild;									// ���ID
	u_long	idVote;										// ��ǥID
	char    szTitle[MAX_BYTE_VOTETITLE];				// ���� 
	char	szQuestion[MAX_BYTE_VOTEQUESTION];			// ��ǥ�������� 
	char	szSelections[4][MAX_BYTE_VOTESELECT];		// ���ù��� 
};

extern CAr&  operator<<(CAr& ar, VOTE_INSERTED_INFO& info);
extern CAr&  operator>>(CAr& ar, VOTE_INSERTED_INFO& info);


// ��� ��ǥ�� ����  
struct GUILD_VOTE_SELECT
{
	char szString[MAX_BYTE_VOTESELECT];		// ���ù��� 
	BYTE cbCount;							// ����count
};

// ��� ��ǥ 
class CGuildVote
{
public:
	CGuildVote();
	virtual ~CGuildVote();
	void	Serialize( CAr & ar );
	bool	IsCompleted() const { return m_bCompleted; }
	void	SetComplete() { m_bCompleted = true; }
	void    Init( const VOTE_INSERTED_INFO& info, bool bCompleted, BYTE* cbCounts );
	void	Cast( BYTE cbSelect );

	u_long  GetID() const { return m_idVote; }
	LPCTSTR	GetTitle() const { return m_szTitle; }
	LPCTSTR	GetQuestion() const { return m_szQuestion; }
	GUILD_VOTE_SELECT& GetVoteSelect(int i) { return m_selects[i]; }

private:
	u_long	m_idVote;								// ID
	bool	m_bCompleted;							// �Ϸ���� : true, ������: false
	char    m_szTitle[MAX_BYTE_VOTETITLE];			// ���� 
	char	m_szQuestion[MAX_BYTE_VOTEQUESTION];	// ��ǥ�������� 
	GUILD_VOTE_SELECT	m_selects[4];				// ���� 
};

////////////////////////////////////////////////////////////////////////////////
//CONTRIBUTION_CHANGED_INFO ���� 
////////////////////////////////////////////////////////////////////////////////
struct CONTRIBUTION_CHANGED_INFO
{
	u_long idGuild;					// ���������� ����� ����� ���id
	u_long idPlayer;				// ������ �÷��̾� 
	DWORD dwPxpCount, dwPenya;		// �÷��̾ ������ pxpȽ��, ��� 
	DWORD dwGuildPxpCount, dwGuildPenya;	// ���忡 ���� ����� ��� pxpȽ��, ��� 
	WORD nGuildLevel;						// ��巹�� 
};

extern CAr&  operator<<(CAr& ar, CONTRIBUTION_CHANGED_INFO& info);
extern CAr&  operator>>(CAr& ar, CONTRIBUTION_CHANGED_INFO& info);

////////////////////////////////////////////////////////////////////////////////
//CGuildTable ���� 
////////////////////////////////////////////////////////////////////////////////

struct GUILD_TABLE_ENTRY
{
	DWORD	dwPxpCount;		// �ʿ���嵵 
	DWORD	dwPenya;		// �ʿ���� 
	WORD	nMaxMember;		// �ִ��ο� 
};

class CGuildTable
{
public:
	virtual ~CGuildTable();

	BOOL	Load(LPCTSTR szFileName);
	DWORD	GetPxpCount(WORD nLevel) const;
	DWORD	GetPenya(WORD nLevel) const;
	WORD	GetMaxMemeber(WORD nLevel) const;
	void	Serialize( CAr & ar );
	BOOL	ReadBlock( CScript & script );
	int		GetMaxLevel() const { return m_nCount; } 

	static CGuildTable& GetInstance();
	
private:
	CGuildTable();
	GUILD_TABLE_ENTRY	m_table[MAX_GUILD_LEVEL];
	int					m_nCount;					// m_table�� ���� ����Ÿ�� 
};

////////////////////////////////////////////////////////////////////////////////
//CGuildMember ���� 
////////////////////////////////////////////////////////////////////////////////

class CGuildMember
{
public:
	u_long	m_idPlayer;						// Player ID
	int		m_nPay;							// ����
	int		m_nGiveGold;					// ��忡 ����� ���
	DWORD	m_dwGivePxpCount;				// ��忡 ����� PXPȽ��( ��ų ����ġ )
	short	m_nWin;							// ������ �̰���?
	short	m_nLose;						// ������ �����?
	char	m_szAlias[MAX_GM_ALIAS];	
	BYTE	m_nMemberLv;					// ���� ����
#if __VER < 11 // __SYS_PLAYER_DATA
	DWORD	m_dwSex;						// ����
	LONG	m_nJob;							// ����
	LONG	m_nLevel;						// ����
	BYTE	m_nLogin;						// �α��� ����( 1 ) / �α� �ƿ�( 0 )
	BYTE	m_nMultiNo;						// ��Ƽ ��ȣ
#endif	// __SYS_PLAYER_DATA
	u_long  m_idSelectedVote;				// ������ ��ǥID
	int		m_nSurrender;	// �׺� ȸ��
	int		m_nClass;						// ��� A, B, C

public:
//	Constructions
	CGuildMember();
	virtual ~CGuildMember();
//	Operations
	void	Serialize( CAr & ar );

	CGuildMember &	operator = ( CGuildMember & source );
	
#ifdef __CLIENT
	CMover *GetMover( void ) { return prj.GetUserByID( m_idPlayer ); }
#endif // client
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CGuildMember>*	sm_pPool;
	void*	operator new( size_t nSize )	{	return CGuildMember::sm_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CGuildMember::sm_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CGuildMember::sm_pPool->Free( (CGuildMember*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CGuildMember::sm_pPool->Free( (CGuildMember*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

////////////////////////////////////////////////////////////////////////////////
//CGuild ���� 
////////////////////////////////////////////////////////////////////////////////

// �����Ǵܰ�� 
enum CONTRIBUTION_RESULT						
{
	CONTRIBUTION_OK,							// ���尡�� 
	CONTRIBUTION_FAIL_MAXLEVEL,					// ����Ұ� - �ְ� ���� 
	CONTRIBUTION_FAIL_GUILD_OVERFLOW_PXP,		// ����Ұ� - ��� PXP overflow
	CONTRIBUTION_FAIL_GUILD_OVERFLOW_PENYA,		// ����Ұ� - ��� Penya overflow
	CONTRIBUTION_FAIL_INVALID_CONDITION,		// ����Ұ� - 
	CONTRIBUTION_FAIL_OVERFLOW_PXP,				// ����Ұ� - ������ PXP overflow
	CONTRIBUTION_FAIL_OVERFLOW_PENYA			// ����Ұ� - ������ Penya overflow
};

#define	GF_WARTIME		(DWORD)0x00000001

typedef	struct	_GUILD_MEMBER_INFO
{
	u_long	idPlayer;
#if __VER < 11 // __SYS_PLAYER_DATA
	LONG	nLevel;
	LONG	nJob;
	DWORD	dwSex;
#endif	// __SYS_PLAYER_DATA
	BYTE	nMultiNo;
	_GUILD_MEMBER_INFO() 
	{	idPlayer	= 0;
#if __VER < 11 // __SYS_PLAYER_DATA
		nLevel	= nJob	= dwSex	= 0;
#endif	// __SYS_PLAYER_DATA
		nMultiNo	= 100;
	}
}
GUILD_MEMBER_INFO, *PGUILD_MEMBER_INFO;

class	CGuildWar;

class CGuild
{
public:
	u_long	m_idGuild;							// Guild ID
	char	m_szGuild[MAX_G_NAME];				// Guild Name
	map<u_long, CGuildMember*>	m_mapPMember;	// ȸ����
	u_long	m_idMaster;							// ����� ID
	int		m_nLevel;							// Guild Level
	DWORD	m_adwPower[MAX_GM_LEVEL];			// ���� ������
	DWORD	m_adwPenya[MAX_GM_LEVEL];			// ��� ������
	BOOL	m_bActive;							// Ȱ����? ������? �˻��ϴ°ǰ�?
	DWORD   m_dwLogo;							// �ΰ� �̹��� ��ȣ
	DWORD   m_dwContributionPxp;				// ����� PXP
	char	m_szNotice[MAX_BYTE_NOTICE];		// ��������
	DWORD	m_dwFlag;							// ��� ���
	BOOL	m_bSendPay;							// ��� ������ ����� Ȯ��
	
#if !defined( __CORESERVER)
	CItemContainer<CItemElem>	m_GuildBank;	// ��� â��
#endif
	DWORD						m_nGoldGuild;	// ��� â�� ��� / ����� ���

	list <CGuildVote*>			m_votes;		// ��ǥ����Ÿ�� 
	u_long	m_idEnemyGuild;						// �������� ������
	int		m_nWinPoint;
	int		m_nWin;
	int		m_nLose;
	int		m_nSurrender;
	int		m_nDead;

#ifdef __CORESERVER
	BOOL	m_bLog;
#endif	// __CORESERVER

public:
//	Constructions
	CGuild();
	virtual ~CGuild();
	void	Clear( void );
//	Operations
//	If an error occurs, return nonzero.
	u_long	GetGuildId( void )		{ return m_idGuild; };
	BOOL	AddMember( CGuildMember* pMember );
	BOOL	RemoveMember( u_long idPlayer );
	BOOL	IsMember( u_long idPlayer )		{	return( GetMember( idPlayer ) != NULL );	}
	BOOL	IsMaster( u_long idPlayer )	{	return( m_idMaster == idPlayer );	}
	void	Serialize( CAr & ar, BOOL bDesc );
	BOOL	SetLogo( DWORD dwLogo );
	void	SetNotice( const char* szNotice );

	void	MeritResultMsg( CONTRIBUTION_RESULT cbResult );
	void	SetContribution( CONTRIBUTION_CHANGED_INFO & info );
	BOOL	AddContribution( DWORD dwPxp, DWORD dwPenya, u_long idPlayer );
	CONTRIBUTION_RESULT CanContribute( DWORD dwPxp, DWORD dwPenya, u_long idPlayer );
	void	DecrementMemberContribution( u_long idPlayer, DWORD dwPenya, DWORD dwPxpCount );

	void	AddVote( const VOTE_INSERTED_INFO& info, bool bCompleted = false, BYTE* cbCounts = NULL );
	CGuildVote* FindVote( u_long idVote );
	bool	ModifyVote( u_long idVote, BYTE cbOperation, BYTE cbExtra );
		
	//	��� â���� ��ĸ� �����ü� �ִ��� Ȯ��
	BOOL	IsGetPenya( u_long idPlayer )	{	return m_adwPower[GetMember(idPlayer)->m_nMemberLv] & PF_PENYA; }
	//	��� â���� �������� �����ü� �ִ��� Ȯ��
	BOOL	IsGetItem( u_long idPlayer )	{	return m_adwPower[GetMember(idPlayer)->m_nMemberLv] & PF_ITEM; }
	
	// ���� �˻�
	BOOL	IsCmdCap( int nMemberLv, DWORD dwPower )	{	return( ( m_adwPower[nMemberLv] & dwPower )? TRUE: FALSE );	}

#if __VER >= 15 // __GUILD_HOUSE
	BOOL	IsAuthority( DWORD dwPlayerId, int nAuthority )	{ return GetMember(dwPlayerId) ? ( m_adwPower[GetMember(dwPlayerId)->m_nMemberLv] & nAuthority ) : FALSE; }
#endif // __GUILD_HOUSE
	//	Attributes
	int		GetSize( void )	{	return m_mapPMember.size();	}
	CGuildMember*	GetMember( u_long idPlayer );
	u_long	m_idWar;
	CGuildWar* GetWar( void );
	int		GetMaxMemberSize( void );

	int		GetMemberLvSize( int nMemberLv );
	int		GetMaxMemberLvSize( int nMemberLv );

	CGuild &	operator = ( CGuild & source );

	GUILDQUEST	m_aQuest[MAX_GUILD_QUEST];
	BYTE	m_nQuestSize;
	PGUILDQUEST		m_pQuest;
	PGUILDQUEST		FindQuest( int nQuestId );
	PGUILDQUEST		GetQuest( int nQuestId )	{	return FindQuest( nQuestId );	}
	BOOL	RemoveQuest( int nQuestId );
	void	SetQuest( int nQuestId, int nState );
#ifdef __WORLDSERVER
	void	ReplaceLodestar( const CRect & rect );
	void	Replace( DWORD dwWorldId, D3DXVECTOR3 & vPos, BOOL bMasterAround = FALSE );
#endif	// __WORLDSERVER

#ifndef __CORESERVER
	CItemElem*	GetItem( DWORD dwItemId, SERIALNUMBER iSerialNumber );
	short	RemoveItem( DWORD dwId, short nItemNum );
#endif	// __CORESERVER

	static	int	sm_anMaxMemberSize[MAX_GUILD_LEVEL];
	static	int	sm_anMaxMemberLvSize[MAX_GM_LEVEL];
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CGuild>*	sm_pPool;
	void*	operator new( size_t nSize )	{	return CGuild::sm_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CGuild::sm_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CGuild::sm_pPool->Free( (CGuild*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CGuild::sm_pPool->Free( (CGuild*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

////////////////////////////////////////////////////////////////////////////////
//CGuildMng ���� 
////////////////////////////////////////////////////////////////////////////////

class CPlayer;

class CGuildMng
{
public:
	u_long	m_id;								// load
	map<u_long, CGuild*>	m_mapPGuild;		// Guild ID�� �̿��Ͽ� ��� ����
	map<string, CGuild*>	m_mapPGuild2;		// Guild Name�� �̿��Ͽ� ��� ����
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	CRIT_SEC	m_AddRemoveLock;
#endif

public:
//	Constructions
	CGuildMng();
	virtual ~CGuildMng();
	void	Clear( void );
	u_long	AddGuild( CGuild* pGuild );
	BOOL	RemoveGuild( u_long idGuild );
	CGuild*		GetGuild( u_long idGuild );
	CGuild*		GetGuild( const char* szGuild );
	void	Serialize( CAr & ar, BOOL bDesc );
	BOOL	SetName( CGuild* pGuild, const char* szName );
public:
#ifdef __WORLDSERVER
	BOOL	m_bSendPay;
	void	Process( void );
#endif // __WORLDSERVER
#ifdef __CORESERVER	
	void	AddConnection( CPlayer* pPlayer );
	void	RemoveConnection( CPlayer* pPlayer );
#endif	// __CORESERVER
//	Attributes
	int		GetSize( void )	{	return m_mapPGuild.size();	}
};



enum {	GUILD_PENYA_MINE_UPDATE, GUILD_ITEM_MINE_UPDATE, GUILD_PENYA_ALL_UPDATE, GUILD_ITEM_ALL_UPDATE };
enum { GUILD_PUT_ITEM, GUILD_GET_ITEM, GUILD_PUT_PENYA, GUILD_GET_PENYA, GUILD_CLOAK, GUILD_QUERY_REMOVE_GUILD_BANK };


#define MAX_RANK_LIST	20

#ifdef __DBSERVER
extern	APP_INFO	g_appInfo;
#endif

////////////////////////////////////////////////////////////////////////////////
//	CGuildRank 
////////////////////////////////////////////////////////////////////////////////
class CGuildRank
{
public:
	/*
	 *	R1 : �ְ����
	 *	R2 : �ִٽ�
	 *	R3 : �ִ���
	 *	R4 : �ִ��׺���
	 *	R5 : �ְ��ӷ�
	 *	R6 : �ְ��ڱ�
	 *	R7 : ��հ�
	 *	R8 : �ִ��÷���
	 */
	enum RANKING
	{
		R1, R2, R3, R4, R5, R6, R7, R8, RANK_END
	};

	// ��ũ ���� ����ü
	typedef struct _SGuildRanking
	{
		int			m_dwLogo;
		char		m_szGuild[17];
		char		m_szName[33];
		int			m_nWin;
		int			m_nLose;
		int			m_nSurrender;
		float		m_AvgLevel;
		int			m_nWinPoint;
	}GUILD_RANKING, *LPGUILD_RANKING;
	
	GUILD_RANKING	m_Ranking[RANK_END][MAX_RANK_LIST];
	int				m_Total[RANK_END];
	DWORD			m_Version;

#if !defined(__WORLDSERVER) && !defined(__CLIENT)
	CRIT_SEC		m_Lock;
#endif
	CTime			m_UpdateTime;


	GUILD_RANKING*	operator[](int i)		{	return m_Ranking[i]; }

public:
	CGuildRank()									{	m_Version = 0; m_UpdateTime = CTime::GetCurrentTime();}
	~CGuildRank()									{	;}

	static CGuildRank* Instance();
#ifdef __DBSERVER
	/*	
	 *	TRANS �����϶����� Rank�Լ��� Call �Ҽ� �ִ�.
	 */
	BOOL GetRanking(CQuery* pQuery, LPCTSTR p_strQuery)
	{
		m_Lock.Enter( theLineFile );

		m_UpdateTime = CTime::GetCurrentTime();

		m_Version++;
		ZeroMemory(m_Ranking, sizeof(GUILD_RANKING)*MAX_RANK_LIST*RANK_END);
		ZeroMemory(m_Total, sizeof(int)*RANK_END);

		pQuery->Clear();

		for ( int i=R1; i<RANK_END; ++i )
		{
			sprintf(const_cast<char*>(p_strQuery), "RANKING.RANKING_DBF.dbo.RANKING_STR 'R%d','%02d'", i+1, g_appInfo.dwSys );
			if( FALSE == pQuery->Exec( p_strQuery ) )
			{
				Error( "CDbManager::UpdateGuildRanking���� (%s) ����", p_strQuery );
				m_Lock.Leave( theLineFile );
				return FALSE;
			}

			int& j = m_Total[i];
			for ( ; pQuery->Fetch() == TRUE ; ++j )
			{
				m_Ranking[i][j].m_dwLogo		= pQuery->GetInt("m_dwLogo");
				
				pQuery->GetStr( "m_szGuild"		, m_Ranking[i][j].m_szGuild );
				pQuery->GetStr( "m_szName"		, m_Ranking[i][j].m_szName );

				m_Ranking[i][j].m_nWin			= pQuery->GetInt("m_nWin");
				m_Ranking[i][j].m_nLose			= pQuery->GetInt("m_nLose");
				m_Ranking[i][j].m_nSurrender	= pQuery->GetInt("m_nSurrender");			
				m_Ranking[i][j].m_AvgLevel		= pQuery->GetFloat("m_AvgLevel");				
				m_Ranking[i][j].m_nWinPoint		= pQuery->GetInt("m_nWinPoint");
			}
			
			pQuery->Clear();
		}

		m_Lock.Leave( theLineFile );

		return TRUE;
	}

	
	BOOL RankingDBUpdate(CQuery* pQuery, LPCTSTR p_strQuery)
	{
		m_Lock.Enter( theLineFile );

		pQuery->Clear();

		sprintf(const_cast<char*>(p_strQuery), "MAKE_RANKING_STR '%d'", g_appInfo.dwSys );
		if( FALSE == pQuery->Exec( p_strQuery ) )
		{
			Error( "CDbManager::RankingDBUpdate (%s) ����", p_strQuery );
			m_Lock.Leave( theLineFile );
			return FALSE;
		}

		m_Lock.Leave( theLineFile );

		return TRUE;
	}
#endif//__DBSERVER

	void Serialize( CAr & ar )
	{
#if !defined(__WORLDSERVER) && !defined(__CLIENT)
		m_Lock.Enter( theLineFile );
#endif

		if( ar.IsStoring() )
		{
			ar << m_Version;
			ar.Write( m_Total, sizeof(int)*RANK_END );
			for ( int i=R1; i<RANK_END; i++ )
			{
				if ( m_Total[i] )
				{
					ar.Write( m_Ranking[i], sizeof(GUILD_RANKING)*m_Total[i] );
				}
			}
		}
		else
		{
			ar >> m_Version;
			ar.Read( m_Total, sizeof(int)*RANK_END );
			for ( int i=R1; i<RANK_END; i++ )
			{
				if ( m_Total[i] )
				{
					ar.Read( m_Ranking[i], sizeof(GUILD_RANKING)*m_Total[i] );
				}
			}
		}

#if !defined(__WORLDSERVER) && !defined(__CLIENT)
		m_Lock.Leave( theLineFile );
#endif	// __WORLDSERVER
	}
};

#endif	// __GUILD_H__