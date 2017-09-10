// GuildHouse.h: interface for the CGuildHouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDHOUSE_H__7A401B90_A88E_4AB9_A5A3_CBFE71AB03F6__INCLUDED_)
#define AFX_GUILDHOUSE_H__7A401B90_A88E_4AB9_A5A3_CBFE71AB03F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if __VER >= 15 // __GUILD_HOUSE
enum
{
	GUILDHOUSE_PCKTTYPE_LISTUP = 0,
	GUILDHOUSE_PCKTTYPE_LISTDROP,
	GUILDHOUSE_PCKTTYPE_SETUP,
	GUILDHOUSE_PCKTTYPE_REMOVE,
	GUILDHOUSE_PCKTTYPE_RESET,
	GUILDHOUSE_PCKTTYPE_REFRESH,
	GUILDHOUSE_PCKTTYPE_UPKEEP,
	GUILDHOUSE_PCKTTYPE_EXPIRED,
};

static const int	BUY_MIN_MEMBERSIZE		= 12;			// ����Ͽ콺 ���� �ּ� ���� �� 
static const int	BUY_MIN_LEVEL			= 3;			// ����Ͽ콺 ���� �ּ� ��� ����
static const int	BUY_PENYA				= 100000000;	// ����Ͽ콺 ���� ���
static const int	MAX_LISTUP_NUM			= 200;			// �ִ� ListUp ����

static const int	UPKEEP_DAY_TIME			= 60 * 60 * 24;	// 1�� ���� �ð�
static const int	MAX_UPKEEP_DAY			= 35;			// �ִ� ���� �Ⱓ
static const int	UPKEEP_DAY_PENYA_SMALL	= 2400000;		// ��������Ͽ콺 1�� ������
static const int	UPKEEP_DAY_PENYA_MIDDLE	= 24000000;		// ��������Ͽ콺 1�� ������

static const int	MAX_REST_POINT			= 1000000;		// �޽İ������� �ִ밪
static const int	REST_POINT_INC			= 10000;		// �޽İ������� Tick �� ������(�α��� ����)
static const int	REST_POINT_LOGOUT_INC	= 2500;			// �޽İ������� Tick �� ������(�α׾ƿ� ����)
static const int	REST_POINT_TICK			= 60 * 4;		// �޽İ����� ���� Tick(sec)


#ifdef __GUILD_HOUSE_MIDDLE
static const int	MAX_GH_NAME				= 64;
enum { GH_AREA_FLARIS = 1, GH_AREA_SAINTMORNING, GH_AREA_DARKON, GH_AREA_MAX };
enum { GH_TENDER_CLOSE = 0, GH_TENDER_CANCEL, GH_TENDER_OPEN, GH_TENDER_READY, GH_TENDER_TENDER, GH_TENDER_RESULT };
#endif // __GUILD_HOUSE_MIDDLE

//////////////////////////////////////////////////////////////////////
// CGuildHouseBase
//////////////////////////////////////////////////////////////////////

#ifdef __CLIENT
#define GuildHouse CGuildHouseBase::GetInstance()
#endif // __CLIENT
#define GH_Fntr_Info CGuildHouseBase::GuildHouse_Furniture_Info 

struct HOUSING_ITEM;

class CGuildHouseBase
{
public:
	struct GuildHouse_Furniture_Info
	{
		DWORD		dwItemId;		// ������ ID
		BOOL		bSetup;			// ��ġ����
		D3DXVECTOR3	vPos;			// ��ġ��ġ
		float		fAngle;			// ��ġ����
		OBJID		objId;			// ��ġ�� ������ ��Ʈ��ID(���Ž� �ʿ�)
		time_t		tKeepTime;		// �Ⱓ(���ӽð�)
#ifdef __DBSERVER
		int			nSeqNum;
#endif // __DBSERVER

		GuildHouse_Furniture_Info( DWORD dwII = NULL_ID, BOOL bS = FALSE, D3DXVECTOR3 v = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), float fA = 0.0f, time_t tK = 0 );
		void		Serialize( CAr & ar );
	};
	typedef vector<GuildHouse_Furniture_Info>	VECFurnituretInfo;
	typedef VECFurnituretInfo::iterator			VECFurnitureIter;
	
protected:
	DWORD	m_dwGuildId;
	DWORD	m_dwWorldId;
	VECFurnituretInfo	m_vecFntInfo;
#ifdef __GUILD_HOUSE_MIDDLE
	OBJID	m_objGHId;
	int		m_nAreaIndex;
	int		m_nGHLevel;
	char	m_szGHName[MAX_GH_NAME];
#endif // __GUILD_HOUSE_MIDDLE

private:
	time_t	m_tUpkeepTime;

public:
	CGuildHouseBase( DWORD dwGuildId, DWORD dwWorldId );
	virtual ~CGuildHouseBase();

	void	Clear();
	DWORD	GetType()	{ return m_dwWorldId; }
	BOOL	OnGuildHousePacket( int nPacketType, GuildHouse_Furniture_Info & gfi, int nIndex );
	void	SerializeAllInfo( CAr & ar );
	GuildHouse_Furniture_Info GetFurnitureInfo( int nIndex ) { return nIndex < (int)( m_vecFntInfo.size() ) ? m_vecFntInfo[nIndex] : GuildHouse_Furniture_Info(); }
	time_t	GetUpkeepTime()	{ return m_tUpkeepTime; }
	void	SetUpkeeptime( time_t tUpkeepTime ) { m_tUpkeepTime = tUpkeepTime; }
	int		GetFurnitureListSize()	{ return m_vecFntInfo.size(); }
	BOOL	ExpiredGuildHouse();
#ifdef __GUILD_HOUSE_MIDDLE
	OBJID	GetGuildHouseId()	{	return m_objGHId;	}
	int		GetAreaIndex()			{	return m_nAreaIndex;	}
	int		GetGuildHouseLevel()	{	return m_nGHLevel;	}
#ifndef __CLIENT
	void	SetGuildHouseLevel( int nLevel )			{	m_nGHLevel = nLevel;	}
	void	SetGuildHouseData( OBJID objGHId, int nAreaIndex, int nGHLevel, time_t tUpkeepTime, const char* szGHName );
#endif // __nCLIENT
#endif // __GUILD_HOUSE_MIDDLE
	
#ifdef __CLIENT
	static CGuildHouseBase* GetInstance();
	void	SendClientToWorld( int nPacketType, GuildHouse_Furniture_Info & gfi, int nIndex );
	GuildHouse_Furniture_Info* GetFurnitureInfoPtr( int nIndex ) { return nIndex < (int)( m_vecFntInfo.size() ) ? &m_vecFntInfo[nIndex] : NULL; }
	BOOL	IsMyGuildHouse( DWORD dwWorldId ) { return m_dwWorldId == dwWorldId; }
	GuildHouse_Furniture_Info* Find( OBJID objID );
	int		FindIndex( OBJID objID_ );
	void	ApplyEFTexture( );

	//gmpbigsun: �Ʒ��Լ����� ��Ʈ���� �����Կ� �־ �˾��޴��� ���� ���� ( �ڵ����� �������� Ÿ���� Ŭ�����Ŵ ) ������ ������ 
	void	Setup( const HOUSING_ITEM& kInfo );
	void	Reset( const HOUSING_ITEM& kInfo );
	void	Remove( );
	void	Reset( );	// ������ ��û -> ������� -> ���� ��Ŷ���� Resetȣ�� 
	int		GetMode( )	{ return m_iMode; }				// ���õ� Obj���� ����� �ൿ ( ��ġ, �缳ġ, ���� )

	DWORD	m_dwSelectedObjID;		//���õ� CCtrl ObjID
	int		m_iMode;
	int		m_nExtraExp;			//�޽İ���ġ ( Ŭ�� ������ )
	int		m_nWndDeployingIndex;	//Ŭ�� ���� ��ġ, �缳ġ���� �༮�� Index

public :
	void	SetFurnitureChannel( BOOL bSetFurnitureChannel )	{ m_bSetFurnitureChannel = bSetFurnitureChannel; }
	BOOL	IsSetFurnitureChannel()								{ return m_bSetFurnitureChannel; }
	void	ResetValues( );

private:
	BOOL	m_bSetFurnitureChannel;
#endif // __CLIENT

#ifdef __WORLDSERVER
public:
	void	AfterFailedGuildHousePacket( int nPacketType, GH_Fntr_Info& gfi, int nIndex );
	void	SetWaitDBAckPlayerId( DWORD dwWaitDBAckPlayerId )	{ m_dwWaitDBAckPlayerId = dwWaitDBAckPlayerId; }
	DWORD	GetWaitDBAckPlayerId()	{ return m_dwWaitDBAckPlayerId; }
	BOOL	IsWaitDBAck()	{ return m_dwWaitDBAckPlayerId != NULL_ID; }
	BOOL	SendWorldToDatabase( CUser* pUser, int nPacketType, GH_Fntr_Info& gfi, int nIndex );
	void	SendWorldToClient( int nPacketType, GH_Fntr_Info& gfi, int nIndex );

	BOOL	IsEnteranceAble( CUser* pUser );
	void	SetApplyDST( CUser* pUser );
	void	ResetApplyDST( CUser* pUser );
	BOOL	EnteranceGuildHouseRoom( CUser* pUser );
	void	GoOutGuildHouseRoom( CUser* pUser );
	void	CheckDestroyRoom( CUser* pUser = NULL );
	
	virtual float		GetRestPointFactor() = 0;
#endif // __WORLDSERVER

private:
	BOOL	SetupFurnitureCtrl( int nIndex, GuildHouse_Furniture_Info & gfi );
	BOOL	RemoveFurnitureCtrl( int nIndex, GuildHouse_Furniture_Info & gfi );
	BOOL	ResetFurnitureCtrl( int nIndex, GuildHouse_Furniture_Info & gfi );
	BOOL	ListUpFurniture( GuildHouse_Furniture_Info & gfi );
	BOOL	ListDropFurniture( int nIndex );
#ifdef __WORLDSERVER
	BOOL	IsAuthority( CUser* pUser, int nPacketType );
	BOOL	PreCheckPacket( CUser* pUser, int nPacketType, GH_Fntr_Info& gfi, int nIndex );
	void	SetDSTFunriture( ItemProp* pItemProp );
	void	ResetDSTFunriture( ItemProp* pItemProp );

	BOOL	CreateGuildHouseRoom();
	BOOL	DestroyGuildHouseRoom();
	BOOL	IsListUpAble( DWORD dwItemId );
	virtual int			GetMaxListUpNum( ItemProp* pItemProp ) = 0;
	virtual int			GetUpkeepPenya() = 0;
	
	virtual D3DXVECTOR3	GetEnterPos() = 0;
	virtual BOOL		IsSetupAble( CUser* pUser, int nIndex ) = 0;
	
	DWORD	m_dwWaitDBAckPlayerId;
#endif // __WORLDSERVER
#ifndef __CLIENT
public:
	virtual D3DXVECTOR3 GetTeleporterPos() = 0;
#endif // n__CLIENT

#ifdef __DBSERVER
public:
	int		GetFirstExpiredFurnitureIndex();
private:
#endif // __DBSERVER
};

#ifndef __CLIENT
//////////////////////////////////////////////////////////////////////
// CGuildHouseSmall : CGuildBase
//////////////////////////////////////////////////////////////////////
class CGuildHouseSmall : public CGuildHouseBase
{
public:
	CGuildHouseSmall( DWORD dwGuildId );
	virtual ~CGuildHouseSmall();

public:
#ifndef __CLIENT
	virtual D3DXVECTOR3 GetTeleporterPos()	{ return D3DXVECTOR3( 80.0f, 100.0f, 75.0f ); }
#endif // n__CLIENT

#ifdef __WORLDSERVER
public:
	virtual float		GetRestPointFactor() { return 1.0f; }
private:
	virtual D3DXVECTOR3 GetEnterPos() { return D3DXVECTOR3( (float)( xRandom( 78, 83 ) ), 100.0f, (float)( xRandom( 61, 66 ) ) ); }
	virtual int			GetMaxListUpNum( ItemProp* pItemProp );
	virtual int			GetUpkeepPenya() { return UPKEEP_DAY_PENYA_SMALL; }
	virtual BOOL		IsSetupAble( CUser* pUser, int nIndex );
#endif // __WORLDSERVER


};


//////////////////////////////////////////////////////////////////////
// CGuildHouseMiddle : CGuildBase
//////////////////////////////////////////////////////////////////////
class CGuildHouseMiddle : public CGuildHouseBase
{
public:
	CGuildHouseMiddle( DWORD dwGuildId );
	virtual ~CGuildHouseMiddle();

public:
#ifndef __CLIENT
	virtual D3DXVECTOR3 GetTeleporterPos()	{ return D3DXVECTOR3( 250.0f, 100.0f, 250.0f ); }
#endif // n__CLIENT

#ifdef __WORLDSERVER
public:
	virtual float		GetRestPointFactor() { return 0.75f; }
private:
	virtual D3DXVECTOR3 GetEnterPos() { return D3DXVECTOR3( (float)( xRandom( 248, 252 ) ), 100.0f, (float)( xRandom( 248, 252 ) ) ); }
	virtual int			GetMaxListUpNum( ItemProp* pItemProp )	{ return MAX_LISTUP_NUM; }
	virtual int			GetUpkeepPenya() { return UPKEEP_DAY_PENYA_MIDDLE; }
	virtual BOOL		IsSetupAble( CUser* pUser, int nIndex )	{ return TRUE; }
#endif // __WORLDSERVER
};



/*	15�� ���� - ��ȹ�� ���� ��� ���� �۾��� ���Ѵ�.
				���� �۾��ڴ� CGuildHouseMiddle�� �����ϱ� �ٶ���. 
				CGuildHouseMng::MakeGuildHouse() �Լ��� ���� �ٶ�.
				- chipi -
//////////////////////////////////////////////////////////////////////
// CGuildHouseBig : CGuildBase
//////////////////////////////////////////////////////////////////////
class CGuildHouseLarge : public CGuildHouseBase
{
public:
	CGuildHouseLarge( DWORD dwGuildId );
	virtual ~CGuildHouseLarge();

 public:
#ifndef __CLIENT
	virtual D3DXVECTOR3 GetTeleporterPos()	{ return D3DXVECTOR3( 270.0f, 0.0f, 300.0f ); }
#endif // n__CLIENT

#ifdef __WORLDSERVER
public:
	virtual float		GetRestPointFactor() { return 0.5f; }
private:
	virtual D3DXVECTOR3 GetEnterPos() { return D3DXVECTOR3( xRandom( 268, 273 ), 0.0f, xRandom( 298, 303 ) ); }
	virtual int			GetMaxListUpNum( ItemProp* pItemProp )	{ return MAX_LISTUP_NUM; }
	virtual int			GetUpkeepPenya() { return 24000000; }
	virtual BOOL		IsSetupAble( CUser* pUser, int nIndex )	{ return TRUE; }
#endif // __WORLDSERVER
};
*/



//////////////////////////////////////////////////////////////////////
// CGuildHouseMng : CGuildBase
//////////////////////////////////////////////////////////////////////
#define	GuildHouseMng	CGuildHouseMng::GetInstance()
class CGuildHouseMng
{
public:
	CGuildHouseMng();
	~CGuildHouseMng();
	static CGuildHouseMng* GetInstance();

	CGuildHouseBase*	MakeGuildHouse( DWORD dwGuildId, DWORD dwWorldId );
	BOOL				AddGuildHouse( DWORD dwGuildId, CGuildHouseBase* pGuildHouse )	{ return m_mapGuildHouse.insert( make_pair( dwGuildId, pGuildHouse ) ).second; }
	BOOL				RemoveGuildHouse( DWORD dwGuildId );
	CGuildHouseBase* GetGuildHouse( DWORD dwGuildId );
	BOOL	IsEmpty()	{ return m_mapGuildHouse.empty(); }
	void	Serialize( CAr & ar );

#ifdef __WORLDSERVER
public:
	BOOL	IsGuildHouse( DWORD dwWorldId );
	BOOL	IsLoginAble( CUser* pUser, DWORD dwWorldId, int nLayer );
	BOOL	EnteranceGuildHouse( CUser* pUser, DWORD dwComebackItemId = NULL_ID );
	void	GoOutGuildHouse( CUser* pUser );
	void	CheckDestroyGuildHouse( CUser* pUser );

	void	ReqBuyGuildHouse( CUser* pUser );
	void	OnBuyGuildHouse( CAr & ar );
	void	OnRemoveGuildHouse( CAr & ar );
	BOOL	SendWorldToDatabase( CUser* pUser, int nPacketType, GH_Fntr_Info& gfi, int nIndex = NULL_ID );

	void	SetApplyDST( CUser* pUser );
	void	ResetApplyDST( CUser* pUser );

	float	GetRestPointFactor( CUser* pUser );

	void	AddRestExpFactorTable( int nMinLevel, int nMaxLevel, float fFactor );
	float	GetRestExpFactor( int nLevel );
	
private:
	BOOL	IsBuyAble( CUser* pUser );
	
	typedef map<int, float> MapRestExpFactor;
	MapRestExpFactor	m_mapRestExpFactorTable;
#endif // __WORLDSERVER

#ifdef __DBSERVER
public:
	void	ProcessExpired();
	BOOL	DBLoadGuildHouse( CQuery* pQuery );
	int		AddSeqNum()	{ return ++m_nSeqNum; }
	int		GetSeqNum() { return m_nSeqNum; }

private:
	int		m_nSeqNum;
#endif // __DBSERVER

private:
	typedef map<DWORD, CGuildHouseBase*> MapGuildHouse;
	MapGuildHouse m_mapGuildHouse;

#ifdef __GUILD_HOUSE_MIDDLE
public:
	struct TENDER_DATA
	{
		int		nTenderId;
		int		nTenderArea;
		int		nTenderState;
		DWORD	dwNoticeTick;
		int		nMaxHouse;
		int		nMinPenya;
		int		nChargeRate;
		DWORD	dwPrevNoticeTick;
		DWORD	dwTenderNoticeTick;
		map<OBJID, int>	mapNpc;
		
		TENDER_DATA() : nTenderId( 0 ), nTenderArea( 0 ), nTenderState( 0 ), dwNoticeTick( 0 ),
		nMaxHouse( 0 ), nMinPenya( 0 ), nChargeRate( 0 ), dwPrevNoticeTick( 0 ), dwTenderNoticeTick( 0 )	{}
	};
	typedef map<DWORD, TENDER_DATA>		MAP_TENDER_DATA;

	struct GUILDHOUSE_DATA
	{
		OBJID	objGHId;
		DWORD	dwGHType;
		int		nAreaIndex;
		char	szTitle[128];
		GUILDHOUSE_DATA()
		: objGHId( 0 ), dwGHType( 0 ), nAreaIndex( 0 )	{}
	};
	typedef map<OBJID, GUILDHOUSE_DATA>		MAP_GH_DATA;

	struct GUILDHOUSE_QUEST
	{
		DWORD	dwGHType;
		int		nLevel;
		GUILDHOUSE_QUEST() : dwGHType( 0 ), nLevel( 0 )	{}
	};
	typedef map<int, GUILDHOUSE_QUEST>		MAP_GH_QUEST;

	struct GUILDHOUSE_TENDER
	{
		OBJID		objGHId;
		DWORD		dwGuildId;
		int			nTenderPerin;
		int			nTenderPenya;
		DWORD		dwGHType;
		int			nAreaIndex;
		time_t		tTenderTime;
		char		chState;

		GUILDHOUSE_TENDER()
		: objGHId( 0 ), dwGuildId( 0 ), nTenderPerin( 0 ), nTenderPenya( 0 ), dwGHType( 0 ), nAreaIndex( 0 ), tTenderTime( NULL ), chState( 'T' )	{}
		GUILDHOUSE_TENDER( OBJID _objGHId, DWORD _dwGuildId, int _nTenderPerin, int _nTenderPenya )
		: objGHId( _objGHId ), dwGuildId( _dwGuildId ), nTenderPerin( _nTenderPerin ), nTenderPenya( _nTenderPenya ),
		dwGHType( 0 ), nAreaIndex( 0 ), tTenderTime( NULL ), chState( 'T' )	{}
	
		void Serialize( CAr & ar )
		{
			if( ar.IsStoring() )
			{
				ar << objGHId << dwGuildId << nTenderPerin << nTenderPenya << dwGHType << nAreaIndex << tTenderTime << chState;
			}
			else
			{
				ar >> objGHId >> dwGuildId >> nTenderPerin >> nTenderPenya >> dwGHType >> nAreaIndex >> tTenderTime >> chState;
			}
		}
	};
	typedef map<DWORD, GUILDHOUSE_TENDER>	MAP_TENDER_GUILD;


	int		GetTenderState( DWORD dwGHType );
	void	SetTenderState( DWORD dwGHType, int nState );
	void	LoadScript();
	
	int		GetTenderArea( DWORD dwGHType );
	int		GetTenderId( DWORD dwGHType );


	TENDER_DATA*		GetTenderData( DWORD dwGHType );
	GUILDHOUSE_DATA*	GetGuildHouseData( OBJID objGHId );

	BOOL	IsBeginNoticeTime( DWORD dwGHType );
	BOOL	IsEndNoticeTime( DWORD dwGHType );
	BOOL	IsBeginTenderTime( DWORD dwGHType );
	BOOL	IsEndTenderTime( DWORD dwGHType );

	bool	AddGuildHouseTender( DWORD dwGuildId, GUILDHOUSE_TENDER& GHT )
	{	return m_mapTenderGuild.insert( MAP_TENDER_GUILD::value_type( dwGuildId, GHT ) ).second;	}


	CLuaBase	m_Lua;

private:
	void	SetTenderInfo( DWORD dwGHType, int nTenderArea );
	void	CloseTender( DWORD dwGHType );


	MAP_TENDER_DATA		m_mapTenderData;
	MAP_GH_DATA			m_mapGHData;
	MAP_TENDER_GUILD	m_mapTenderGuild;
	
	typedef map< DWORD, vector<OBJID> >	MAP_TENDERABLE_GH;
	MAP_TENDERABLE_GH	m_mapTenderAbleGH;



#ifdef __DBSERVER
public:
	void	UpdateGuildHouseTender( DWORD dwGHType, CQuery* pQuery );
	void	SendGuildHouseTenderInfo( DPID dpId );
	bool	CheckTenderOpenAble( DWORD dwGHType );

	void	ProcessTender( DWORD dwGHType );
	bool	IsTenderProcess( DWORD dwGHType );

	void	GetHighestTenderGuild( DWORD dwGHType );
	void	ReturnPenyaTenderFailGuild( DWORD dwGHType );

#endif // __DBSERVER
	
#ifdef __WORLDSERVER
public:
	map<OBJID, char*>	GetTenderAbleGuildHouse( DWORD dwGHType );
	bool				ReqTenderGuildHouseList( CUser* pUser, DWORD dwGHType, OBJID objNpcId );
	void				ReqTenderGuildHouseInfo( CUser* pUser, OBJID objGHId );
	bool				IsTenderAbleGuildHouse( DWORD dwGHType, OBJID objGHId );
	bool				IsTenderAble( CUser* pUser, OBJID objGHId, int nTenderPerin, int nTenderPenya );
	void				OnGuildHouseTenderJoin( CUser* pUser, OBJID objGHId, int nTenderPerin, int nTenderPenya );



	GUILDHOUSE_TENDER*	GetGuildHouseTender( DWORD dwGuildId );
	vector<DWORD>		GetTenderView( OBJID objGHId );

	
	void	OnGuildHouseTenderInfo( CAr & ar );
	void	OnGuildHouseTenderJoin( CAr & ar );
	void	OnGuildHouseTenderState( CAr & ar );
	void	OnGuildHouseTenderResult( CAr & ar );
	void	OnGuildHouseGradeUpdate( CAr & ar );
	void	OnGuildHouseLevelUpdate( CAr & ar );
	void	OnGuildHouseExpired( CAr & ar );

	CGuildHouseBase*	ReMakeGuildHouse( DWORD dwGuildId, DWORD dwWorldId );

	GUILDHOUSE_QUEST*	GetGuildHouseQuestData( int nQuestId );
	void	CheckGuildHouseQuest( CUser* pUser, int nQuestId );

private:
	MAP_GH_QUEST	m_mapGHQuest;
#endif // __WORLDSERVER

#endif // __GUILD_HOUSE_MIDDLE
};
#endif // n__CLIENT 

#endif // __GUILD_HOUSE

#endif // !defined(AFX_GUILDHOUSE_H__7A401B90_A88E_4AB9_A5A3_CBFE71AB03F6__INCLUDED_)
