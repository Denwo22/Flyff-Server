#ifndef __WNDHOUSING__H
#define __WNDHOUSING__H
#pragma once

#if __VER >= 13 // __HOUSING
#include "Housing.h"

struct HOUSING_ITEM
{
	CString		m_strName;
	CString		m_strDesc;
	time_t		m_dwTime;
	BOOL		m_bDeploy;
	int			m_nType;
	int			m_nIndex;
	DWORD		dwItemId;		// ������ ID

#if __VER >= 15 // __GUILD_HOUSE
	int			m_nSlotIndex;		//�������� �����ϴ� �������� �ε��� 
	D3DXVECTOR3 m_vPos;
	float		m_fAngle;
	int			m_nTeleIndex;
#endif

	HOUSING_ITEM() {Init();};
	~HOUSING_ITEM() {};

	void Init() {m_strName.Empty(); m_strDesc.Empty(); m_dwTime = m_bDeploy = m_nType = m_nIndex = dwItemId = 0;};
};


class CWndHousing : public CWndNeuz 
{
private:
	// �������� �ִ� ����
	vector<HOUSINGINFO>		m_vecItem;
	// ���� â���� ���� ������
	vector<HOUSING_ITEM>	m_mapItem;
	// ���� Ÿ��
	int						m_nSortType;
	BOOL					m_bIsGreater;
	int						m_nSelected;
public: 
	CWndHousing(); 
	virtual ~CWndHousing(); 
	
	virtual void SerializeRegInfo( CAr& ar, DWORD& dwVersion );
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	
	void RefreshItemList( );
	void Sort();
	
	
}; 

#endif // __HOUSING

#if __VER >= 15 // __GUILD_HOUSE

class CWndGHouseShowOneUnit : public CWndNeuz			
{
	// ��ġ �缳ġ�� ��������� ������ִ� ...
public:
	CWndGHouseShowOneUnit( );
	virtual ~CWndGHouseShowOneUnit( );

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual	void OnInitialUpdate(); 
	virtual void OnDraw( C2DRender* p2DRender ); 

	void SetItem( const HOUSING_ITEM& kItem )	{ m_kItem = kItem; }

protected:
	HOUSING_ITEM m_kItem;
};


static const int GH_MAX_VIEW_CAPACITY = 7;	//����Ʈ���� ������ �ִ� ��뷮

class CWndGuildHousing : public CWndNeuz
{
public:
	enum GH_SECTION		// ���� ( �ش� �Ӽ��� ����Ʈ���� �����ش�. )
	{
		GS_ALL, 
		GS_FURNITURE,
		GS_TELEPORTER,
	};
		
	CWndGuildHousing( );
	virtual ~CWndGuildHousing( );

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual	void OnInitialUpdate(); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point );
	virtual	BOOL Process();

	void RefreshItemList( );
	void Sort();
	BOOL SetSelectedByObjID( OBJID objID );
	void SetSection( GH_SECTION eSection )		{ m_eSection = eSection; }
	BOOL IsSection( GH_SECTION eSection )		{ return ( eSection == m_eSection ); }
	BOOL InitBySection( GH_SECTION eSection );
	int  GetWndItemIndexByObjID( OBJID objID );

protected:
	void UpdateIRButton( );			//IR: Install/Recall
	void UpdateSortTextColor( int oldType, int newType );
	void SetEnableInstallBtns( BOOL bEnable );
	void CheckChannel( );	// ��ġ ��ü���� ������ ä������ �˻� & ��� 
	void FixScrollBar( const int nSelected );
	void AutoAddingComboItems( );

protected:
	vector<HOUSING_ITEM>	m_cWndItems;

	CWndButton	m_wndButton[ GH_MAX_VIEW_CAPACITY ];	

	int			m_nSortType;
	BOOL		m_bIsGreater;
	int			m_nSelectedSort;

	CTexture  m_texUp;
	CTexture  m_texDown;
	
	int			m_nSelected;
	
	GH_SECTION	m_eSection;
	BOOL		m_bDeploying;

	BOOL		m_bOnFocusEffect;

	CWndGHouseShowOneUnit* m_pGHShowOne;
	DWORD m_dwComboCurrIK3;
};

#ifdef __GUILD_HOUSE_MIDDLE

struct GHBidData	//����Ͽ콺 ���� ������
{
	GHBidData( ) { Init(); }
	void Init() { _id = 0, _name.Empty(), _cGuildList.clear(), _nBidMinPenya = 0; }

	OBJID _id;
	CString _name;

	vector< DWORD > _cGuildList;	//������ �����
	int _nBidMinPenya;				//�ּ� �����ݾ�
};

typedef vector<GHBidData>				GHBidDataContainer;
typedef GHBidDataContainer::iterator	GHBidDataIter;

class CWndGuildHouseBid : public CWndNeuz			
{
	//����Ͽ콺 ���� 
public:
	CWndGuildHouseBid( );
	virtual ~CWndGuildHouseBid( );

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual	void OnInitialUpdate(); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 

	void SetEnableWindow_Apply( BOOL bEnable, BOOL bWait = FALSE );
	void ResetInputMoneyWindows( );				//�Է�â "0"���� �ʱ�ȭ
	void RequestCurrHouseInfo( );				//���� �����ִ� �Ͽ콺 �������� ��û

	void RefreshWnd_HouseList( );	//�������� ���� �� �� ���� ������ �ʱ�ȭ
	void RefreshWnd_HouseInfo( );	//�������� �̿��� ���� ����
	void UpdateData_HouseList( OBJID houseID, const char* szName );
	void UpdateData_HouseInfo( OBJID houseID, const int nMinPenya, __int64 nTnederPenya, vector< DWORD >& guildIDs );

	GHBidData* FindData( OBJID houseID );

	void MakeMoneyStyle( CString& str );

protected:
	GHBidDataContainer _cBidDatas;
	__int64 m_n64TenderPenya;	//�� ����� ������ 
	BOOL m_bMaster;

	BOOL m_bWaitResult;
	DWORD m_dwWaitTime;
};

#endif //__GUILD_HOUSE_MIDDLE


#endif //__GUILD_HOUSE

#endif
