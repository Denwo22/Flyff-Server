#pragma once

#if __VER >= 13 // __HOUSING

#include "Object3D.h"

/////////////////////////////////////////////////////////////////////
//// CHousing ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
struct HOUSINGINFO
{
	DWORD		dwItemId;		// ������ ID
	time_t		tKeepTime;		// �Ⱓ(���ӽð�)
	BOOL		bSetup;			// ��ġ����
	D3DXVECTOR3	vPos;			// ��ġ��ġ
	float		fAngle;			// ��ġ����
#ifdef __WORLDSERVER
	OBJID		objId;			// ��ġ�� ������ ��Ʈ��ID(���Ž� �ʿ�)
#endif // __WORLDSERVER
	HOUSINGINFO() : dwItemId(NULL_ID), tKeepTime(0), vPos(0.0f,0.0f,0.0f), bSetup(FALSE), fAngle(0.0f)
#ifdef __WORLDSERVER
	, objId( NULL_ID )
#endif // __WORLDSERVER
	{} // �ʱ�ȭ
#ifdef __DBSERVER
	HOUSINGINFO( DWORD dwII ) : dwItemId(dwII), tKeepTime(0), vPos(0.0f,0.0f,0.0f), bSetup(FALSE), fAngle(0.0f)
	{
		ItemProp* pItemProp = prj.GetItemProp( dwItemId );   
		if( pItemProp )
		{
			CTime time	= CTime::GetCurrentTime() + CTimeSpan( 0, 0, pItemProp->dwAbilityMin, 0 );
			tKeepTime	= (time_t)( time.GetTime() );
		}
	}
#endif // __DBSERVER
	void Serialize( CAr & ar )
	{
		if( ar.IsStoring() )
			ar << dwItemId << static_cast<time_t>(tKeepTime - time_null()) << bSetup << vPos << fAngle;
		else
		{
			ar >> dwItemId >> tKeepTime;
			tKeepTime += time_null();
			ar >> bSetup >> vPos >> fAngle;
		}
	}
};

class CHousing
{
public:
	CHousing( DWORD dwPlayerId );
	~CHousing(void);

#ifdef __CLIENT
	vector<DWORD>	m_vecVisitable;
	static CHousing* GetInstance();
	void	GetHousingList( vector<HOUSINGINFO> & vHousingList );	// ���� ��� ������ �����ؼ� �ѱ�.
	void	GetVisitAllow( vector<DWORD> & vVisitAllow );		// �湮����� ����� �ѱ�
#endif // __CLIENT
		
	void Serialize( CAr & ar );
	void SetFurnitureList( HOUSINGINFO& housingInfo, BOOL bAdd );
	void SetupFurniture( HOUSINGINFO housingInfo );
	void SetVisitAllow( DWORD dwTargetId, BOOL bAllow );

#ifdef __WORLDSERVER
	BOOL IsListUpAble( CUser* pUser, DWORD dwItemId );	// ���� ��� �߰��� �����Ѱ�?
	BOOL IsSetupAble( CUser* pUser, HOUSINGINFO housingInfo );		// ���� ��ġ�� �����Ѱ�?
	BOOL IsAllowVisit( DWORD dwPlayerId );	// ���� ����� �湮�� �ΰ�?
	BOOL AddFurnitureControl( int nIndex );		// ���� ��Ʈ���� �����Ѵ�.
	void RemoveFurnitureControl( int nIndex );	// ���� ��Ʈ���� �����Ѵ�.
	void AddAllFurnitureControl();				// ��ġ�� ��� ���� ��Ʈ���� �����Ѵ�.
	void SetBuff( DWORD dwItemId, BOOL bSet );	// ���� ��ġ�� ���Ž� ��������..
	vector<DWORD> GetAllPaperingInfo();			// ���� �� ���� ����..
	
	void Setting( BOOL bSetting ) { m_bSetting = bSetting; }
	BOOL m_bSetting;			// ���� ���� �������ΰ�?(TransServer�� Ÿ�̹� ������..)
#endif // __WORLDSERVER

#ifdef __DBSERVER
	void ProcessExpiredFurniture( time_t time, BOOL bGMRevmoe = FALSE );
#endif // __DBSERVER

private:
	BOOL	RemoveFurnitureList( DWORD dwItemId );
	int		GetIndexFromList( DWORD dwItemId );

	DWORD	m_dwMasterId;
	vector<HOUSINGINFO> m_vecHousingInfo;		// ���� ���
	vector<DWORD>		m_vecIdVisitAllow;		// ���� �湮 ����� ���
};

#ifndef __CLIENT	// __WORLDSERVER, __DBSERVER
/////////////////////////////////////////////////////////////////////
//// CHousingMng ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
typedef map<DWORD, CHousing*> MAP_HP;
typedef map< DWORD, vector<DWORD> > MAP_VSTABLE;
class CHousingMng
{
public:
	CHousingMng(void);
	~CHousingMng(void);
	void	Clear();
	static CHousingMng* GetInstance();
#ifdef __WORLDSERVER
	void ReqLoadHousingInfo( DWORD dwPlayerId );
	BOOL ReqSetFurnitureList( CUser* pUser, DWORD dwItemId );	// Trans�� ���� ��� �߰� ��û
	BOOL ReqSetupFurniture( CUser* pUser, HOUSINGINFO housingInfo );	// Trans�� ���� ��ġ �� ���� ��û
	BOOL ReqSetAllowVisit( CUser* pUser, DWORD dwPlayerId, BOOL bAllow ); // Trans�� ��� ��� �߰� ��û
	void ReqGMFunrnitureListAll( CUser* pUser );	// GM ���� ��� ��ü ����..

	void CreateRoomLayer( DWORD dwPlayerId );	// ���ӽ� �� ���̾ �����ϰ� ��ġ�� ���� ��Ʈ���� �����Ѵ�.
	BOOL DestroyHousing( DWORD dwPlayerId );	// �α� �ƿ��� �Ͽ�¡ ���� ����

private:
	MAP_VSTABLE	m_mapVisitable;		// �湮 ���� ���
public:
	void SetAddVisitable( DWORD dwPlayerId, DWORD dwTargetId );		// �湮 ���� ��� �߰�
	void SetRemoveVisitable( DWORD dwPlayerId, DWORD dwTargetId );	// �湮 ���� ��� ����
	void OnReqVisitableList( CUser* pUser );	// Ŭ���̾�Ʈ�� �湮 ���� ����� ��û�ߴ�.
	void SetVisitRoom( CUser* pUser, DWORD dwPlayerId ); // �湮...
	void GoOut( CUser* pUser );		// �濡�� ����.
#endif // __WORLDSERVER
	
	CHousing*	CreateHousing( DWORD dwPlayerId );	// ���ӽ� �Ͽ�¡ ���� ����(Trans�� ���� ���ӽ�)
	CHousing*	GetHousing( DWORD dwPlayerId );	// �ش� ID�� �Ͽ�¡ ���� Get
	
	void		SetFurnitureList( DWORD dwPlayerId, HOUSINGINFO& housingInfo, BOOL bAdd );	// ���� ��� �߰� �� ����
	void		SetupFurniture( DWORD dwPlayerId, HOUSINGINFO housingInfo );	// ���� ��ġ �� ����
	void		SetVisitAllow( DWORD dwPlayerId, DWORD dwTargetId, BOOL bAllow );	// ��� ��� �߰�

#ifdef __DBSERVER
	void		ProcessRemoveExpiredFurniture();	// ����ð� �˻�
#endif // __DBSERVER

private:
	MAP_HP		m_mapHousing;
};

#endif // n__CLIENT

#ifdef __CLIENT

#define RED_MODE		1
#define NORMAL_MODE		2

class CDeployManager
{

private:
	HOUSINGINFO			m_ItemInfo;

	LPDIRECT3DTEXTURE9	m_pOriginWall;				// �� �⺻�ؽ���
	LPDIRECT3DTEXTURE9	m_pOriginTile;				// �ٴ� �⺻�ؽ���
	vector<LPDIRECT3DTEXTURE9>	m_vecOriginal;		// Ÿ�ٿ�����Ʈ �ؽ��ļ�
	LPDIRECT3DTEXTURE9	m_pRed;
	BOOL				m_bIsColl;
	BOOL				m_bIsMyRoom;				// ���¹��� �����ΰ�?
	int					m_nBlendFactor;
	int					m_nNumTex;
	
public:
	CObj*				m_pTargetObj;
	CObj*				m_pWallObj;
	float				m_fAngle;

	CDeployManager(void);
	~CDeployManager(void);

	static	CDeployManager* GetInstance();
	BOOL	LoadToDeploy(int nItemId, HOUSINGINFO housingInfo);
	BOOL	ChangeObjMode(int nType);
	HOUSINGINFO*	EndDeploy();
	BOOL	IsReady();
	BOOL	IsCollide();
	void	Init();
	void	Process();
	BOOL	CheckCollision();
	BOOL	ChangeWallTex(TCHAR* pTexName = NULL);
	BOOL	ChangeTileTex(TCHAR* pTexName = NULL);
	BOOL	IsMyRoom();
	BOOL	SetMyRoom(BOOL bFlag);
};

#endif // __CLIENT
#endif // __VER >= 13

#if __VER >= 15 // __GUILD_HOUSE

#ifdef __CLIENT

struct HOUSING_ITEM;

class CGuildDeployManager 
{
	// ��� �Ͽ콺 ���� ������ 
public:
	CGuildDeployManager( );
	virtual ~CGuildDeployManager( );

	static CGuildDeployManager* GetInstance( );

	BOOL	LoadToDeploy(int nItemId, const HOUSING_ITEM& housingInfo);
	BOOL	ChangeObjMode(int nType);
	HOUSING_ITEM*	EndDeploy();

	BOOL	IsReady();
	void	Init();
	void	Process();
	BOOL	CheckCollision();
	BOOL	ChangeWallTex(TCHAR* pTexName = NULL); 
	BOOL	ChangeTileTex(TCHAR* pTexName = NULL);
	BOOL	IsManagementPower( )						{ return m_bIsManagement; }	// ���� ���� ������ �ִ°�?
	void	SetManagementPower( BOOL bMng )				{ m_bIsManagement = bMng; }
	BOOL	IsCollide()									{ return m_bIsColl; }

	BOOL	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	std::string GetNameHouseObj( );
	std::string GetNameHouseWallTex( );
	std::string GetNameHouseTileTex( );
	
protected:
	HOUSING_ITEM* m_pItem;

	LPDIRECT3DTEXTURE9	m_pOriginWall;				// �� �⺻�ؽ���
	LPDIRECT3DTEXTURE9	m_pOriginTile;				// �ٴ� �⺻�ؽ���
	vector<LPDIRECT3DTEXTURE9>	m_vecOriginal;		// Ÿ�ٿ�����Ʈ �ؽ��ļ�
	LPDIRECT3DTEXTURE9	m_pRed;
	BOOL				m_bIsColl;
	BOOL				m_bIsManagement;				// �������� ������ �ִ°�?
	int					m_nBlendFactor;
	int					m_nNumTex;

	CObj*				m_pTargetObj;
	CObj*				m_pWallObj;
	float				m_fAngle;

	CModelObject*		m_pClonedModel;

};

inline CGuildDeployManager* GuildDeploy( ) { return CGuildDeployManager::GetInstance( ); }

#endif //__CLIENT

#endif	//__GUILD_HOUSE

