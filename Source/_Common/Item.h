#if !defined(AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_)
#define AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(__DBSERVER) || defined(__CORESERVER)
#include "..\_Network\Objects\Obj.h"
#else
#include "Ctrl.h"
#endif
#include "ProjectCmn.h"
#include "lang.h"

#if __VER >= 9	// __PET_0410
#include "pet.h"
#endif	// __PET_0410

#if __VER >= 11 // __SYS_COLLECTING
#include "accessory.h"
#endif	// __SYS_COLLECTING

#include "Piercing.h"

class CItemBase
{
public:
	OBJID       m_dwObjId;		// ������ �����̳� �ȿ����� ID (�������� ID�� ���)
	DWORD		m_dwObjIndex;	// ������ �����̳� �ȿ����� Index
	DWORD		m_dwItemId;		// ������ �ĺ� �ε���( Property���� ��� )
	int 		m_nExtra;		// �ŷ��� ��ǰ���� or ���λ����� ����� ���� 
	TCHAR       m_szItemText[ 32 ];
	int			m_nCost;

#ifdef __CLIENT
	CTexture*   m_pTexture;
#endif

private:
	SERIALNUMBER		m_liSerialNumber;

public:
	CItemBase();
	virtual ~CItemBase()	{}
	virtual	CItemBase&	operator = ( CItemBase & ib );
	virtual void	Serialize( CAr & ar );
	BOOL			IsEmpty() { return m_dwItemId ? FALSE : TRUE; }
	BOOL			IsQuest();

	ItemProp*		GetProp(); 
	virtual	void			Empty();
	void			SetTexture();
	void			SetTexture( CTexture* pTexture );
	CTexture*		GetTexture();						
	int				GetCost();							// ������ ��´�.
	void			SetExtra( int nExtra );			// Ȯ�� ����Ÿ�� ���� 
	int				GetExtra();							// Ȯ�� ����Ÿ�� ��� 
#if __VER >= 11 // __GUILDCOMBATCHIP
	DWORD			GetChipCost();
#endif // __GUILDCOMBATCHIP

	void	SetSerialNumber( void );
	SERIALNUMBER	GetSerialNumber( void )		{	return m_liSerialNumber;	}
	void	SetSerialNumber( SERIALNUMBER liSerialNumber )	{	m_liSerialNumber	= liSerialNumber;	}
};

inline void CItemBase::SetExtra( int nExtra )
{
	m_nExtra = nExtra;
}

inline int CItemBase::GetExtra()
{
	return m_nExtra;
}


inline void CItemBase::SetTexture( CTexture* pTexture )
{
#ifdef __CLIENT
	m_pTexture = pTexture;
#endif
}

inline CTexture* CItemBase::GetTexture() 
{ 
#ifdef __CLIENT
	return m_pTexture;
#else
	return NULL;
#endif
}

class CItemElem : public CItemBase
{
private:
	CPiercing	m_piercing;
public:
	void	SetPiercingSize( int nSize )	{	m_piercing.SetPiercingSize( nSize );	}
	int		GetPiercingSize()	{	return m_piercing.GetPiercingSize();		}
	void	SetPiercingItem( int nth, DWORD dwItem )	{	m_piercing.SetPiercingItem( nth, dwItem );	}
	DWORD	GetPiercingItem( int nth )	{	return m_piercing.GetPiercingItem( nth );	}
	void	CopyPiercing( const CPiercing & piercing )		{	m_piercing	= piercing;		}
	void	SerializePiercing( CAr & ar )	{	m_piercing.Serialize( ar );		}
	BOOL	IsPiercedItem()	{ return m_piercing.IsPiercedItem(); }
#if __VER >= 12 // __EXT_PIERCING
	void	SetUltimatePiercingSize( int nSize )	{	m_piercing.SetUltimatePiercingSize( nSize );	}
	int		GetUltimatePiercingSize()	{	return m_piercing.GetUltimatePiercingSize();		}
	void	SetUltimatePiercingItem( int nth, DWORD dwItem )	{	m_piercing.SetUltimatePiercingItem( nth, dwItem );	}
	DWORD	GetUltimatePiercingItem( int nth )	{	return m_piercing.GetUltimatePiercingItem( nth );	}
	
	BOOL	IsPierceAble( DWORD dwTargetItemKind3 = NULL_ID, BOOL bSize = FALSE );
#endif // __EXT_PIERCING

	int			m_nAbilityOption;	// �߰� �ɷ�ġ ���� �ɼ�

public:
	enum	{	expired	= 0x01,	binds	= 0x02,	isusing	= 0x04,	};

	int			m_nRepair;			// ���� Ƚ��
	BYTE		m_nRepairNumber;
	int			m_nHitPoint;
	short		m_nItemNum; 
	BYTE		m_byFlag;
	DWORD		m_idGuild;			// �����ۿ� ��� ��ȣ�� �ٴ� ���(����)
	
#ifdef __CLIENT
	BOOL		m_bRepair;
#endif	// __CLIENT

	BYTE   	    m_bItemResist;				// ��� �Ӽ� �ΰ�? / TEXT���·� �˽�Ʈ�� �۵���Ű�� �����ۿ��� TRUE�̸� ���̻� ����Ʈ�� �۵��� �� ���� �ȴ�.
	int			m_nResistAbilityOption;		// �Ӽ� �߰� �ɷ�ġ ���� �ɼ�
	int			m_nResistSMItemId;	

#ifdef __WORLDSERVER
	BOOL	m_bQuery;
#endif	// __WORLDSERVER

	/*
#ifdef __GIFTBOX0213
#ifdef __WORLDSERVER
	int		m_nQueryGiftbox;
#endif	// __WORLDSERVER
#endif	// __GIFTBOX0213
	*/
#if __VER >= 12 // __LORD
	BOOL	IsPerin( void )		{	return m_dwItemId == II_SYS_SYS_SCR_PERIN;	}
#endif	// __LORD
#if __VER >= 11 // __SYS_COLLECTING
	BOOL	IsCollector( BOOL bRefinable = FALSE )	{	return( m_dwItemId == II_GEN_TOO_COL_NORMAL || ( !bRefinable && m_dwItemId == II_GEN_TOO_COL_EVENT ) );		}
	BOOL	IsAccessory( void )		{	return CAccessoryProperty::GetInstance()->IsAccessory( m_dwItemId );	}
	BOOL	IsActiveTicket( DWORD dwItemId );
#endif	// __SYS_COLLECTING
#if __VER >= 11 // __SYS_IDENTIFY
	// ��Ʈ�� ����
	// 8|10|8|10|8|10|8	= 64
private:
	__int64		m_iRandomOptItemId;

	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	__int64		m_n64NewRandomOption;
	//	mulcom	END100405	���� ��ȣ�� �η縶��

public:
	__int64		GetRandomOptItemId( void )	{	return m_iRandomOptItemId;	}
	void	SetRandomOptItemId( __int64 iRandomOptItemId )	{	m_iRandomOptItemId	= iRandomOptItemId;	}
	int		GetRandomOpt( void )	{	return static_cast<int>( m_iRandomOptItemId & 0x00000000000000FF );	}
	void	SetRandomOpt( int nRandomOpt )	{	m_iRandomOptItemId	= ( m_iRandomOptItemId & 0xFFFFFFFFFFFFFF00 ) | static_cast<__int64>( nRandomOpt & 0x00000000000000FF );	}
	__int64*	GetRandomOptItemIdPtr( void )	{	return &m_iRandomOptItemId;	}


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	__int64		GetNewRandomOption();
	__int64*	GetNewRandomOptionPtr();
	void		ResetNewRandomOption();
	void		SelectNewRandomOption();
	bool		SelectRandomOption( BYTE bySelectFlag );
	//	mulcom	END100405	���� ��ȣ�� �η縶��


	//	0x8000000000000000
	//	0x4000000000000000
	enum	{	eNoLevelDown, e5LevelDown, e10LevelDown,	};
	int		GetLevelDown( void );	// 64|63
	void	SetLevelDown( int i = eNoLevelDown );
	DWORD	GetLimitLevel( void );
#if __VER >= 14 // __NEW_ITEM_LIMIT_LEVEL
	BOOL	IsLimitLevel( CMover* pMover );	// ������ �Ұ����� �����ΰ�?
#endif // __NEW_ITEM_LIMIT_LEVEL
#else	// __SYS_IDENTIFY
private:
	int			m_nRandomOptItemId;
public:
	int		GetRandomOptItemId( void )	{	return m_nRandomOptItemId;	}
	void	SetRandomOptItemId( int nRandomOptItemId )	{	m_nRandomOptItemId	= nRandomOptItemId;	}
	int		GetRandomOpt( void )	{	return m_nRandomOptItemId;	}
	void	SetRandomOpt( int nRandomOpt )	{ m_nRandomOptItemId	= nRandomOpt;	}
#endif	// __SYS_IDENTIFY
/*
#ifdef __XPET2
	int	m_nMaxLevel;	// ����
	int m_nLevel;		// ����(����)
	int m_nStr, m_nDex, m_nInt, m_nSta;		// ����
	int m_nHungry;		// ��������� -10 ~ 10		-10���ϴ� ��������� / -10 ~ -5 ����� / -5 ~ 5 ���� / 5�̻� ��θ�
	int m_nFeeling;		// ���. -10 ~ 10			
	int m_nHealth;		// �ǰ� -10 ~ 10
	int m_nConstitution;	// ü��(�������) -10 ~ 10
#endif
*/
	
#if __VER >= 9	// __PET_0410
	CPet*	m_pPet;		// �� ��ü, ù ��ȯ �� ����
	BOOL	IsPet( void )	{	return GetProp()->dwItemKind3 == IK3_EGG;	}
#endif	// __PET_0410
	BOOL	IsEatPet( void )	{	return GetProp()->dwItemKind3 == IK3_PET;	}
	BOOL	IsFeed( void )		{	return GetProp()->dwItemKind3 == IK3_FEED;	}
	BOOL	IsEgg( void );
	CString		GetName( void );

public:
//	Constructions
	CItemElem();
	virtual	~CItemElem();

//	Attributes
	virtual	void			Empty();
	ItemProp*	GetProp()	{	return prj.GetItemProp( m_dwItemId );	}
	int			GetAttrOption();		// �������� +�ɼǰ��� �Ӽ�/�Ӽ��������� ���ļ� ����.
	int			GetAbilityOption() { return m_nAbilityOption; }
#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
	int			GetItemResist( void ) const;
	int			GetResistAbilityOption( void ) const;
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY
	int*		GetAbilityOptionPtr() { return &m_nAbilityOption; }
	void		SetAbilityOption( int nAbilityOption ) { m_nAbilityOption = nAbilityOption; }
	int			GetGold();

//	PIERCINGINFO	m_piercingInfo;
	void	GetPiercingAvail( PPIERCINGAVAIL pPiercingAvail/*input, output*/ );
	BOOL	m_bCharged;			// ���ȭ ���������� Ȯ��
	DWORD	m_dwKeepTime;		// ���ӽð�

	BOOL	IsExpiring( void )
		{	
			if( m_dwKeepTime )
			{
				if( time_null() > (time_t)m_dwKeepTime )
					return TRUE;
			}
			return FALSE;
		}
	void	ResetFlag( BYTE byFlag )	{	m_byFlag	&= ~byFlag;		}
	void	SetFlag( BYTE byFlag )		{	m_byFlag	|= byFlag;		}
	BOOL	IsFlag( BYTE byFlag )	{	return ( m_byFlag & byFlag ) ? TRUE: FALSE;		}
	BOOL	IsBinds();
	BOOL	IsUndestructable( void );
	BOOL	IsCharged();
	BOOL	IsLogable();

	static BOOL	IsDiceRefineryAble( ItemProp* pProp );
	static BOOL	IsEleRefineryAble( ItemProp* pProp );
#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
	static BOOL	IsElementalCard( const DWORD dwItemID );
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY
	
//	Operations
	void		UseItem();
	virtual	CItemElem&	operator = ( CItemElem & ie );
	virtual	void		Serialize( CAr & ar );

#if __VER >= 15 // __PETVIS
	BOOL	IsVisPet()	{ return ( GetProp() && GetProp()->IsVisPet() ) || IsTransformVisPet() ; }
	void	SetSwapVisItem( int nPos1, int nPos2 );
	void	SetVisKeepTimeSize( int nSize )				{ m_piercing.SetVisKeepTimeSize( nSize ); }
	void	SetVisKeepTime( int nth, time_t tmKeep )	{ m_piercing.SetVisKeepTime( nth, tmKeep ); }
	time_t	GetVisKeepTime( int nth )					{ return m_piercing.GetVisKeepTime( nth ); }
	DWORD	GetVisPetSfxId();
	BOOL	IsTransformVisPet() { return GetProp() && GetProp()->dwItemKind3 == IK3_PET && m_bTranformVisPet; }

	BOOL	m_bTranformVisPet;
#endif // __PETVIS
};

inline int CItemElem::GetAttrOption()		// �������� +�ɼǰ��� �Ӽ�/�Ӽ��������� ���ļ� ����.
{
	int nAttr = (int)m_bItemResist;
	int nOption = m_nResistAbilityOption;
	int nRet = m_nAbilityOption;
	
	nRet |= (nAttr << 16);		// D16~D23 : �Ӽ�
	nRet |= (nOption << 24);	// D24~D31 : �Ӽ�����
	return nRet;
}

#if __VER >= 15 // __15_5TH_ELEMENTAL_SMELT_SAFETY
inline int CItemElem::GetItemResist( void ) const
{
	return m_bItemResist;
}

inline int CItemElem::GetResistAbilityOption( void ) const
{
	return m_nResistAbilityOption;
}
#endif // __15_5TH_ELEMENTAL_SMELT_SAFETY

typedef struct tagSkill
{
	DWORD dwSkill;
	DWORD dwLevel;
	ItemProp* GetProp()	{ return prj.GetSkillProp( dwSkill ); }
	
} SKILL,* LPSKILL;

/*
#ifdef __XPET2
class CPetElem : public CItemBase
{
	int m_nMaxLevel;	// ����
	int m_nLevel;		// ����(����)
	int m_dwHungry;		// ��������� -10 ~ 10		-10���ϴ� ��������� / -10 ~ -5 ����� / -5 ~ 5 ���� / 5�̻� ��θ�
	int m_dwFeeling;	// ���. -10 ~ 10			

	void Init();
	void Destroy();
public:
	CPetElem();
	~CPetElem();
	ItemProp* GetProp()	{	return prj.GetItemProp( m_dwItemId );	}
	virtual	CPetElem&	operator = ( CPetElem & pe );
	virtual void	Serialize( CAr & ar );
};
#endif // xPet2
*/

//////////////////////////////////////////////////////////////////////
// �̰��� ������ �ϳ��ϳ��� ��Ҹ� �迭�� ����� �߰�, ���� ���� 
// ������ ������ �����̳� Ŭ������.
//////////////////////////////////////////////////////////////////////
template <class T> class CItemContainer  
{
public:
	LPDWORD    m_apIndex;
	DWORD      m_dwIndexNum; // equp�� ������ ���� �����̳� ������ 
	DWORD      m_dwItemMax;  // equp�� ������ ��ü ������ 
	T*         m_apItem;

	void SetItemContainer( DWORD dwItemType, DWORD dwItemMax, DWORD dwExtra = NULL_ID );
	CItemContainer();
//	CItemContainer( DWORD dwItemType, DWORD dwItemMax, DWORD dwExtra = NULL_ID );
	virtual ~CItemContainer();

	void Clear();

	T* GetAt( DWORD dwIndex );
#ifdef __CLIENT
	void SetAt( DWORD dwIndex, DWORD dwItemId );
	void SetAt( DWORD dwIndex, T* pItemElem );
	void InsertAt( DWORD dwIndex, DWORD dwItemId );
	void InsertAt( DWORD dwIndex, T* pItemElem );
	void RemoveAt( DWORD dwIndex );
#if __VER >= 8	// __JEFF_VER_8
	DWORD	Find( DWORD dwItemId )
		{
			if( dwItemId == 0 )
				return NULL_ID;
			for( int i = 0; i < (int)( m_dwIndexNum ); i++ )
			{
				DWORD nId	= m_apIndex[i];
				if( nId < 0 || nId >= m_dwItemMax )
					continue;
				if( m_apItem[nId].m_dwItemId == dwItemId )
					return nId;
			}
			return NULL_ID;
		}
	DWORD	GetItemCount( DWORD dwItemId )
		{
			if( dwItemId == 0 )
				return 0;
			int nCount	= 0;
			for( int i = 0; i < (int)( m_dwItemMax ); i++ )
			{
				CItemElem* pItemElem	= (CItemElem*)&m_apItem[i];
				if( pItemElem->m_dwItemId == dwItemId )
					nCount	+= pItemElem->m_nItemNum;
			}
			return( nCount );
		}
#endif	// __JEFF_VER_8
#endif	// __CLIENT

	// Equip ���� 
	BOOL DoEquip( DWORD dwSrcIndex, DWORD dwDstIndex );
	BOOL UnEquip( DWORD dwIndex );
	BOOL IsEquip( DWORD dwObjId );
	T* GetEquip( DWORD dwIndex );

	void	Swap( DWORD dwSrcIndex, DWORD dwDstIndex );
	BOOL	Add( T* pElem, BYTE* pnId = NULL, short* pnNum = NULL, BYTE* pnCount = NULL );
	BOOL	IsFull( T* pElem, ItemProp* pItemProp, short nNum );
	void	SetAtId( OBJID dwObjId, T* pItemElem );

	T*	GetAtId( OBJID dwObjId );
	T* GetAtItemId( DWORD dwItemId );
	int		GetAtItemNum( DWORD dwItemId );
	void	RemoveAtId( OBJID dwObjId );

	int		GetSize() { return int( m_dwIndexNum ); }
	int		GetMax() { return int( m_dwItemMax ); }
	void	Copy( CItemContainer<T> & rItemContainer );
	void	Serialize( CAr & ar );

	int		GetEmptyCount()
		{
			int nCount	= 0;
			for( DWORD i = 0; i < m_dwItemMax; i++ )
			{
				if( m_apItem[i].IsEmpty() && m_apItem[i].m_dwObjIndex < m_dwIndexNum )
					nCount++;
			}
			return( nCount );
		}
	int		GetCount()
		{
			int nCount	= 0;
			for( DWORD i = 0; i < m_dwItemMax; i++ )
			{
				if( m_apItem[i].IsEmpty() == FALSE && m_apItem[i].m_dwObjIndex < m_dwIndexNum )
					nCount++;
			}
			return( nCount );
		}
	int		GetCountByIK3( DWORD dwItemKind3 )
		{
			int nCount	= 0;
			for( int i = 0; i < m_dwItemMax; i++ )
			{
				if( m_apItem[i].IsEmpty() == FALSE && m_apItem[i].GetProp()->dwItemKind3 == dwItemKind3 )
					nCount++;
			}
			return( nCount );
		}
	int		GetCount( DWORD dwItemId )
		{
			int nCount	= 0;
			for( DWORD i = 0; i < m_dwItemMax; i++ )
			{
				if( m_apItem[i].IsEmpty() == FALSE && m_apItem[i].m_dwItemId == dwItemId )
					nCount++;
			}
			return( nCount );
		}
private:
	void	Swap2( DWORD dwSrcIndex, DWORD dwDstIndex );
};

template <class T> CItemContainer<T>::CItemContainer()
{
	m_dwIndexNum = 0;
	m_dwItemMax = 0;

	m_apItem = NULL;
	m_apIndex = NULL;
}


//template <class T> CItemContainer<T>::CItemContainer( DWORD dwItemType, DWORD dwItemMax, DWORD dwExtra )
//{
//	SetItemContainer( dwItemType, dwItemMax, dwExtra );
//}

template <class T> CItemContainer<T>::~CItemContainer()
{
	SAFE_DELETE_ARRAY( m_apItem );
	SAFE_DELETE_ARRAY( m_apIndex );
}

template <class T> void CItemContainer<T>::Clear()
{
	for( DWORD i = 0; i < m_dwItemMax; i++ )
	{
		m_apItem[ i ].Empty();
		m_apItem[ i ].m_dwObjId = i;
		if( i < m_dwIndexNum )
		{
			m_apItem[ i ].m_dwObjIndex = i;
			m_apIndex[ i ] = i;
		}
		else 
		{
			m_apIndex[ i ] = NULL_ID;
		}
	}
}

template <class T> void CItemContainer<T>::SetItemContainer( DWORD dwItemType, DWORD dwItemMax, DWORD dwExtra )
{
	m_dwIndexNum = dwItemMax;
	m_dwItemMax = dwItemMax;
	if( dwExtra != NULL_ID )
		m_dwItemMax += dwExtra;
	
	SAFE_DELETE_ARRAY( m_apItem );
	SAFE_DELETE_ARRAY( m_apIndex );

	m_apItem = new T[ m_dwItemMax ];
	m_apIndex = new DWORD[ m_dwItemMax ];
	Clear();
}

// �μ��� ������Ʈ ���̵� ����ؼ� ��´�.
// ������Ʈ ���̵�� �迭�� �ѹ� �߰��� �� �����Ǹ� �������� �ʴ� �� ���� �ٲ��� �ʴ´�.
template <class T> T* CItemContainer<T>::GetAtId( DWORD dwId )
{
	if( dwId >= (DWORD)( GetMax() ) ) return NULL;
	T* pItemElem = &m_apItem[ dwId ];
	if( pItemElem->IsEmpty() ) 
		return NULL;
	return pItemElem;
}

template <class T> T* CItemContainer<T>::GetAtItemId( DWORD dwItemId )
{
	for( DWORD i = 0; i < m_dwItemMax; i++ )
	{
		if( m_apItem[i].m_dwItemId == dwItemId )
			return &m_apItem[i];
	}
	return NULL;
}

template <class T> int CItemContainer<T>::GetAtItemNum( DWORD dwItemId )
{
	int nResult = 0;
	for( DWORD i = 0; i < m_dwItemMax; i++ )
	{
		if( m_apItem[i].m_dwItemId == dwItemId )
			nResult += m_apItem[i].m_nItemNum;
	}
	return nResult;
}

template <class T> BOOL CItemContainer<T>::DoEquip( DWORD dwSrcIndex, DWORD dwDstIndex )
{ 
	dwDstIndex += m_dwIndexNum;		// dwDstIndex = ����ũ��(42) + PARTS_???;

	if( dwSrcIndex == dwDstIndex )
		return FALSE;

	if( dwSrcIndex >= m_dwItemMax || dwDstIndex >= m_dwItemMax )
		return FALSE;

	for( DWORD i = 0; i < m_dwItemMax; i++ )
	{
		if( m_apItem[ i ].IsEmpty() && m_apItem[ i ].m_dwObjIndex == NULL_ID )
		{
			if( m_apIndex[dwSrcIndex] >= m_dwItemMax )
				return FALSE;
			m_apIndex[ dwDstIndex ] = m_apIndex[ dwSrcIndex ];		// ���濡 �ִ� index���� ��������(dwDstIndex)�� �ű�.
			m_apIndex[ dwSrcIndex ] = i;							// ���� ������ �ִ���(dwSrcIndex)�� ����ְ�(i) �����.
			m_apItem[ m_apIndex[ dwSrcIndex ] ].m_dwObjIndex = dwSrcIndex;	// �Ű����� ��ԵȰ��� m_dwObjIndex���� �� dwSrcIndex�� �־��ִ°�?
			m_apItem[ m_apIndex[ dwDstIndex ] ].m_dwObjIndex = dwDstIndex;	// dwObjIndex�� apItem���� ������ apIndex�� index�� �˱������ΰ�?
			return TRUE;
		}
	}
	return FALSE;
}

template <class T> BOOL CItemContainer<T>::UnEquip( DWORD dwIndex )
{ 
	dwIndex += m_dwIndexNum;
	if( dwIndex >= m_dwItemMax )
		return FALSE;

	DWORD dwId = m_apIndex[ dwIndex ];

	if( dwId >= m_dwItemMax )
		return FALSE;

	for( DWORD i = 0; i < m_dwIndexNum; i++ )
	{
		if( m_apIndex[i] >= m_dwItemMax )
			continue;

		if( m_apItem[ m_apIndex[ i ] ].IsEmpty() )
		{
			m_apItem[ m_apIndex[ i ] ].m_dwObjIndex = NULL_ID;
			m_apIndex[ dwIndex ] = NULL_ID;
			m_apItem[ dwId ].m_dwObjIndex = i;
			m_apIndex[ i ] = dwId;
			return TRUE;
		}
	}
	return FALSE;
}

template <class T> BOOL CItemContainer<T>::IsEquip( DWORD dwObjId )
{
	if( !m_apItem )
	{
		WriteError( "CONTAINER//0" );
		return FALSE;
	}

	if( dwObjId >= m_dwItemMax )
		return FALSE;

	if( m_apItem[ dwObjId ].m_dwObjIndex >= m_dwIndexNum )
		return TRUE;
	return FALSE;
}

template <class T> T* CItemContainer<T>::GetEquip( DWORD dwIndex )
{
	if( m_dwIndexNum )
	{
		if( dwIndex < 0 || dwIndex >= ( m_dwItemMax - m_dwIndexNum ) )
		{
			WriteError( "GETEQUIP//%d, %d, %d", m_dwIndexNum, m_dwItemMax - m_dwIndexNum, dwIndex );
			return NULL;
		}
		return GetAt( m_dwIndexNum + dwIndex );
	}
	return NULL;
}

template <class T> BOOL CItemContainer<T>::IsFull( T* pElem, ItemProp* pItemProp, short nNum )
{
	int nId;
	short nNumtmp	= nNum;
	CItemElem* pElemtmp;
	for( DWORD i = 0; i < m_dwIndexNum; i++ )
	{
		nId	= m_apIndex[i];
		if( nId < 0 || nId >= (int)( m_dwItemMax ) )
			continue;
		pElemtmp	= (CItemElem*)&m_apItem[nId];
		if( pElemtmp->IsEmpty() )
		{
			if( nNumtmp > (short)pItemProp->dwPackMax )
				nNumtmp		-= (short)pItemProp->dwPackMax;
			else
				return FALSE;
		}
		else if( pElem->m_dwItemId == pElemtmp->m_dwItemId
				&& pElem->m_byFlag == pElemtmp->m_byFlag && pElem->m_bCharged == pElemtmp->m_bCharged )
		{
			if( pElemtmp->m_nItemNum + nNumtmp > (short)pItemProp->dwPackMax )
				nNumtmp	-= ( (short)pItemProp->dwPackMax - pElemtmp->m_nItemNum );
			else
				return FALSE;
		}
	}

	return TRUE;
}

template <class T> BOOL CItemContainer<T>::Add( T* pElem, BYTE* pnId, short* pnNum, BYTE* pnCount )
{
	if( pnId )
		*pnCount	= 0;
	ItemProp* pItemProp		= pElem->GetProp();
	if( !pItemProp )
		return FALSE;
	BOOL bPackItem = TRUE;
	int nId;

	short nNumtmp	= ( (CItemElem*)pElem )->m_nItemNum;
	if( IsFull( pElem, pItemProp, nNumtmp ) )	// �������� ������ �ִ��� �˻�
		return FALSE;

	if( pItemProp->dwPackMax == 1 )
		bPackItem = FALSE;

	CItemElem* pElemtmp;
	if( bPackItem )
	{
		for( DWORD i = 0; i < m_dwIndexNum; i++ )
		{
			nId	= m_apIndex[i];
			if( nId < 0 || nId >= (int)( m_dwItemMax ) )
				continue;
			pElemtmp	= (CItemElem*)&m_apItem[nId];
			
			if( pElemtmp->IsEmpty() == FALSE && pElemtmp->m_dwItemId == pElem->m_dwItemId && pElemtmp->m_nItemNum < (short)pItemProp->dwPackMax 
				&& pElem->m_byFlag == pElemtmp->m_byFlag && pElem->m_bCharged == pElemtmp->m_bCharged )
			{
				if( pElemtmp->m_nItemNum + nNumtmp > (short)pItemProp->dwPackMax )
				{
					nNumtmp	-= ( (short)pItemProp->dwPackMax - pElemtmp->m_nItemNum );
					pElemtmp->m_nItemNum	= (short)pItemProp->dwPackMax;
					if( pnId )
					{
						pnId[*pnCount]	= nId;
						pnNum[*pnCount]	= pElemtmp->m_nItemNum;
						( *pnCount )++;
					}
				}
				else {
					pElemtmp->m_nItemNum	+= nNumtmp;
					if( pnId ) {
						pnId[*pnCount]	= nId;
						pnNum[*pnCount]		= pElemtmp->m_nItemNum;	
						( *pnCount )++;
					}
					nNumtmp = 0;
					break;
				}
			}
			
		}
	}

	if( nNumtmp > 0 )
	{
		for( DWORD i = 0 ; i < m_dwIndexNum ; i++ )
		{
			nId	= m_apIndex[i];
			if( nId < 0 || nId >= (int)( m_dwItemMax ) )
				continue;
			pElemtmp	= (CItemElem*)&m_apItem[nId];
			if( pElemtmp->IsEmpty() )	// ��������� �˻�
			{
				*pElemtmp	= *( (CItemElem*)pElem );
				pElemtmp->m_dwObjId	= nId;
				pElemtmp->m_dwObjIndex		= i;
				pElemtmp->SetTexture();
				
				if( nNumtmp > (short)pItemProp->dwPackMax )
				{
					pElemtmp->m_nItemNum	= (short)pItemProp->dwPackMax;
					nNumtmp		-= (short)pItemProp->dwPackMax;
					if( pnId ) {
						pnId[*pnCount]	= nId;
						pnNum[*pnCount]	= pElemtmp->m_nItemNum;
						( *pnCount )++;
					}
				}
				else
				{
					pElemtmp->m_nItemNum	= nNumtmp;
					nNumtmp	= 0;
					if( pnId )
					{
						pnId[*pnCount]	= nId;
						pnNum[*pnCount]		= pElemtmp->m_nItemNum;
						( *pnCount )++;
					}
					break;
				}
			}
		}
	}
	if( nNumtmp > 0 )
		Error( "template <class T> BOOL CItemContainer<T>::Add( T* pElem... : nNumtmp > 0" );
	return TRUE;
}

template <class T> void CItemContainer<T>::RemoveAtId( OBJID dwObjId )
{
	if( dwObjId >= m_dwItemMax )
		return;
	if( m_apItem[ dwObjId ].m_dwObjIndex >= m_dwItemMax )
		return;
	m_apItem[ dwObjId ].Empty();
	if( m_apItem[ dwObjId ].m_dwObjIndex >= m_dwIndexNum )
	{
		m_apIndex[ m_apItem[ dwObjId ].m_dwObjIndex ] = NULL_ID;
		m_apItem[ dwObjId ].m_dwObjIndex = NULL_ID;
	}
}


template <class T> void CItemContainer<T>::SetAtId( OBJID dwObjId, T* pItemElem )
{
	if( dwObjId >= m_dwItemMax )
		return;
	m_apItem[dwObjId]	= *pItemElem;
	m_apItem[dwObjId].m_dwObjId	= dwObjId;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// client ���� 
#ifdef __CLIENT

template <class T> void CItemContainer<T>::RemoveAt( DWORD dwIndex )
{
	m_apItem[ m_apIndex[ dwIndex ] ].Empty();
	if( dwIndex >= m_dwIndexNum )
	{
		m_apItem[ m_apIndex[ dwIndex ] ].m_dwObjIndex = NULL_ID;
		m_apIndex[ dwIndex ] = NULL_ID;
	}
}
template <class T> void CItemContainer<T>::SetAt( DWORD dwIndex, DWORD dwItemId )
{
	// �ε����� ����ֳ�?
	T* pItemElem = &m_apItem[ m_apIndex[ dwIndex ] ];
	pItemElem->m_dwItemId = dwItemId;
	pItemElem->m_dwObjId = m_apIndex[ dwIndex ];
	pItemElem->m_dwObjIndex = dwIndex;
}
template <class T> void CItemContainer<T>::SetAt( DWORD dwIndex, T* pItemElem )
{
	CItemElem* pItemElem2 = &m_apItem[ m_apIndex[ dwIndex ] ];
	*pItemElem2	= *pItemElem;
	pItemElem2->m_dwObjId = m_apIndex[ dwIndex ];
	pItemElem2->m_dwObjIndex = dwIndex;
}

#endif // __CLIENT



// �����ϰ� �ε��� �迭���� CItemElem�� ������.
template <class T> T* CItemContainer<T>::GetAt( DWORD dwIndex )
{
	DWORD dwIdx = m_apIndex[ dwIndex ];
	if( dwIdx == NULL_ID )
		return NULL;
	T* pItemElem = &m_apItem[ dwIdx ];
	if( pItemElem->IsEmpty() ) 
		return NULL;
	return pItemElem;
}

template<class T> void CItemContainer<T>::Swap2( DWORD dwSrcIndex, DWORD dwDstIndex )
{
	DWORD dwItem = m_apIndex[ dwSrcIndex ];
	m_apIndex[ dwSrcIndex ] = m_apIndex[ dwDstIndex ];
	m_apIndex[ dwDstIndex ] = dwItem;

	DWORD dwSrcItem	= m_apIndex[dwSrcIndex];
	DWORD dwDstItem	= m_apIndex[dwDstIndex];
	if( dwSrcItem != NULL_ID )
		m_apItem[dwSrcItem].m_dwObjIndex	= dwSrcIndex;
	if( dwDstItem != NULL_ID )
		m_apItem[dwDstItem].m_dwObjIndex	= dwDstIndex;
}

template <class T> void CItemContainer<T>::Swap( DWORD dwSrcIndex, DWORD dwDstIndex )
{
	if( dwSrcIndex == dwDstIndex || dwSrcIndex >= m_dwItemMax || dwDstIndex >= m_dwItemMax )
		return;

	CItemElem* pItemElemSrc, *pItemElemDst;
	pItemElemSrc	= (CItemElem*)GetAtId( m_apIndex[dwSrcIndex] );
	pItemElemDst	= (CItemElem*)GetAtId( m_apIndex[dwDstIndex] );

	if( pItemElemSrc && pItemElemDst && pItemElemSrc->m_dwItemId == pItemElemDst->m_dwItemId && pItemElemSrc->m_byFlag == pItemElemDst->m_byFlag && pItemElemSrc->m_bCharged == pItemElemDst->m_bCharged )
	{
		ItemProp* pItemProp;
		int nPackMax;
		if( ( pItemProp = pItemElemSrc->GetProp() ) && ( nPackMax = pItemProp->dwPackMax ) > 1 )
		{
			short nRemnant	= (short)nPackMax - pItemElemDst->m_nItemNum;
			if( nRemnant >= pItemElemSrc->m_nItemNum )
			{
				pItemElemDst->m_nItemNum	+= pItemElemSrc->m_nItemNum;
				RemoveAtId( m_apIndex[dwSrcIndex] );
			}
			else
			{
				pItemElemDst->m_nItemNum	+= nRemnant;
				pItemElemSrc->m_nItemNum	-= nRemnant;
			}
			return;
		}
	}
	Swap2( dwSrcIndex, dwDstIndex );
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

template <class T> void CItemContainer<T>::Copy( CItemContainer<T> & rItemContainer )
{
	m_dwIndexNum	= rItemContainer.m_dwIndexNum;
	memcpy( (void*)m_apIndex, (void*)rItemContainer.m_apIndex, sizeof(DWORD) * m_dwItemMax );
	for( u_long i = 0; i < m_dwItemMax; i++ )
	{
		m_apItem[i].m_dwObjId	= rItemContainer.m_apItem[i].m_dwObjId;
		m_apItem[i].m_dwObjIndex	= rItemContainer.m_apItem[i].m_dwObjIndex;
		if( rItemContainer.m_apItem[i].IsEmpty() == FALSE )
			m_apItem[i]		= rItemContainer.m_apItem[i];
		else
			m_apItem[i].Empty();
	}
}

template <class T> void CItemContainer<T>::Serialize( CAr & ar )	// 0-673	// 466-631
{
	DWORD	adwObjIndex[128];

	unsigned char chSize	= 0;

	if( ar.IsStoring() )
	{
		
		ar.Write( m_apIndex, sizeof(DWORD) * m_dwItemMax );
		
		u_long uOffset	= ar.GetOffset();
		ar << chSize;

		for( u_char ch = 0; ch < m_dwItemMax; ch++ )	// 0-504
		{
			if( m_apItem[ch].IsEmpty() == FALSE )
			{
				ar << ch;
				m_apItem[ch].Serialize( ar );
				chSize++;
			}
			adwObjIndex[ch]		= m_apItem[ch].m_dwObjIndex;
		}

		ar.Write( adwObjIndex, sizeof(DWORD) * m_dwItemMax );

		int nBufSize;
		LPBYTE lpBuf	= ar.GetBuffer( &nBufSize );
		*( lpBuf + uOffset )	= chSize;
	}
	else
	{
		ar.Read( m_apIndex, sizeof(DWORD) * m_dwItemMax );
		// Clear
		for( u_long i = 0; i < m_dwItemMax; i++ )
			m_apItem[i].Empty();

		ar >> chSize;

		unsigned char ch;
		for( int i = 0; i < chSize; i++ )
		{
			ar >> ch;
			m_apItem[ch].Serialize( ar );
		}

		ar.Read( adwObjIndex, sizeof(DWORD) * m_dwItemMax );
		for( int i = 0; i < m_dwItemMax; i++ )
		{
			m_apItem[i].m_dwObjIndex	= adwObjIndex[i];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////
#include "mempooler.h"
typedef MemPooler<CItem> CItemPool;

/// �������� �����ϴ� ������ 
class CItem  : public CCtrl
{
private:
#ifdef __CLIENT
	float	m_fGroundY;		// �������� �����ɴ�� �� ��ġ�� Y��ǥ�� �̸� ���س���.
	D3DXVECTOR3	 m_vDelta;	// 
#endif // __CLIENT
	
public:
	CItemBase* m_pItemBase;
	u_long	m_idHolder;
	OBJID	m_idOwn;		// ���� �̾������� ���� �Ǹ��� �ִ°�.
	DWORD	m_dwDropTime;	// ����������� �ð�.
	BOOL	m_bDropMob;		// ���Ͱ� �׾ ����Ѱ��̳�?.
#ifdef __EVENT_MONSTER
	BOOL	m_IdEventMonster;	// �̺�Ʈ ���Ͱ� ����� �������ΰ�?
#endif // __EVENT_MONSTER


public:
	CItem();
	virtual ~CItem();

	void SetOwner( OBJID id );
	DWORD GetItemType() const { return 0; }
	void SetItemBase( CItemBase* pItemBase ) { m_pItemBase = pItemBase; }

	CItemBase* GetItemBase() const { return m_pItemBase; }
	ItemProp* GetProp() { return prj.GetItemProp( GetIndex() ); }
	
	virtual void Serialize( CAr & ar );
	virtual void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor = 0xffffffff );
	virtual BOOL Read( CFileIO* pFile );
		
#ifdef __CLIENT
	void SetDelta( float fGroundY, D3DXVECTOR3 &vDelta );
#endif // __CLIENT

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	CItemPool*		m_pPool;
	void*	operator new( size_t nSize )	{	return CItem::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CItem::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CItem::m_pPool->Free( (CItem*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CItem::m_pPool->Free( (CItem*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
};

extern BOOL IsUsableItem( CItemBase* pItem );
extern BOOL IsUsingItem( CItemBase* pItem );

#if __VER >= 11
extern	BOOL	IsNeedTarget( ItemProp* pProp );
#endif	// __VER

#endif // !defined(AFX_ITEM_H__80E88B36_BD6B_449B_BE76_34F2B5B77552__INCLUDED_)

