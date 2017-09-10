#ifndef __PET_H__
#define	__PET_H__

#if __VER >= 9	// __PET_0410

#ifdef __CLIENT
#include "..\_AIInterface\AIInterface.h"
#endif	// __CLIENT

#include "defineitem.h"
// �׳� Ŭ���̾�Ʈ���� ���� ���� �������� ��� ���� ������?
// �̷��� �� ���, (���۰� ó���� ���������� ������ �ִ�.)
	// ������ �� �ڷ� ��ü��,
	// Ŭ���̾�Ʈ�� CPet�� �����͸� ������ �ִ� CMover ��ü�� �����ϸ� �ȴ�.

enum	PETLEVEL	{	PL_EGG,	PL_D,	PL_C,	PL_B,	PL_A,	PL_S,	PL_MAX,	};

#define	MAX_PET_AVAIL_LEVEL	9
#define	MAX_ADD_LIFE	5

typedef	struct	_FEEDENERGY
{
	DWORD	dwCostMin;
	DWORD	dwCostMax;
	WORD	wEnergyMin;
	WORD	wEnergyMax;
}	FEEDENERGY,	*PFEEDENERGY;

typedef struct	_PETPENALTY
{
	FLOAT	fExp;
	WORD	wEnergy;
	_PETPENALTY()
	{
		fExp	= 0.0f;
		wEnergy	= 0;
	}
}	PETPENALTY, *PPETPENALTY;

#define	MAX_PET_SHAPE	3
typedef	struct	_PETAVAILPARAM
{
	DWORD	dwDstParam;		// �ɷ�ġ ��� �Ķ����
	DWORD	m_anParam[MAX_PET_AVAIL_LEVEL];		// �ɷ�ġ ��� ��
	DWORD	m_dwItemId;	// �� ������
	DWORD	m_adwIndex[MAX_PET_SHAPE];	// �� ��ü ���
	DWORD	m_dwProbability;	// ��ȭ Ȯ��
}	PETAVAILPARAM, *PPETAVAILPARAM;

class CPetProperty
{
public:
	CPetProperty();
	virtual	~CPetProperty();

	PPETAVAILPARAM	GetAvailParam( BYTE nKind );	// ġ�� �Ӽ� �߰�
	BYTE	GetLevelupAvailLevel( BYTE wLevel );		// ���� �� �� ��µǴ� �ɷ�ġ ����(����)
	WORD	GetFeedEnergy( DWORD dwCost, int nIndex = 0 );		// ���ݿ� ���� ���� ����(����)
	DWORD	GetIncrementExp( BYTE nLevel );	// �д� ���� ����ġ
	WORD	GetMaxEnergy( BYTE nLevel );
	WORD	GetAddLife( void );		// ���� ȸ���׿� ���� ���� �߰�

#ifdef __JEFF_11_3
	LPDWORD		GetLevelupAvailLevelProbabilityPtr( void )	{	return &m_adwLevelupAvailLevelProbability[0][0];	}
	LPBYTE	GetLevelupAvailLevelMaxPtr( void )		{	return m_anLevelupAvailLevelMax;	}
	vector<WORD>*	GetAddLifeProbabilityPtr( void )	{	return &m_awAddLifeProbability;		}
#endif	// __JEFF_11_3

	static	CPetProperty*	GetInstance( void );
	BOOL	LoadScript( LPCTSTR szFile );
	BYTE	Hatch( void );
	PPETPENALTY		GetPenalty( BYTE nLevel );
private:
	PETAVAILPARAM	m_aPetAvailParam[PK_MAX];
	BYTE	m_anLevelupAvailLevelMax[PL_MAX];
	DWORD	m_adwLevelupAvailLevelProbability[PL_MAX][MAX_PET_AVAIL_LEVEL];	// Ȯ��
	vector<FEEDENERGY>	m_aFeedEnergy[2];	// ���� ����� �� ���� ���ݿ� ���� ���� �� 
	DWORD	m_adwIncrementExp[PL_MAX];		// �д� ���� ����ġ
	WORD	m_awMaxEnergy[PL_MAX];
	vector<WORD>	m_awAddLifeProbability;

	PETPENALTY	m_aPenalty[PL_MAX];
};

#define	MAX_PET_LIFE	99
#define	MAX_PET_EGG_EXP		(DWORD)50000
#define	MAX_PET_EXP		(DWORD)100000

#define	PF_PET	0
#define	PF_PET_LEVEL_UP	1
#define	PF_PET_LEVEL_DOWN	2
#define	PF_PET_GET_AVAIL	3

#define	PETLOGTYPE_CALL	0
#define	PETLOGTYPE_LEVELUP	1
#define	PETLOGTYPE_RELEASE	2
#define	PETLOGTYPE_DEATH		4
#define	PETLOGTYPE_FEED	5
#define	PETLOGTYPE_MIRACLE	6
#define	PETLOGTYPE_MISTAKE	7
#define	PETLOGTYPE_LIFE	8

#ifdef __PET_1024
#define	MAX_PET_NAME_FMT	33	// 16 * 2 + 1
#define	MAX_PET_NAME	17	// �ѱ� 8��
#endif	// __PET_1024

class CPet
{
public:
//	Constructions
	CPet();
	virtual	~CPet();

//	Operations
	void	Serialize( CAr & ar );

//	Attributions
	void	SetAvailLevel( BYTE nLevel, BYTE nAvailLevel );

	DWORD	GetIndex( void );
	BYTE	GetAvailLevel( BYTE nLevel );
	LPBYTE	GetAvailLevel( void )	{	return m_anAvailLevel;	}

	void	GetAvailDestParam( DWORD & dwDestParam, int & nParam );
	BYTE	GetKind( void )	{	return m_nKind;	}
	void	SetKind( BYTE nKind )	{	m_nKind	= nKind;	}
	BYTE	GetLevel( void )	{	return m_nLevel;	}
	void	IncLevel( void )		{	m_nLevel++;	}
	void	SetLevel( BYTE nLevel )	{	m_nLevel	= nLevel;	}
	DWORD	GetExp( void )	{	return m_dwExp;		}
	void	SetExp( DWORD dwExp );
	WORD	GetLife( void );
	void	SetLife( WORD wLife )	{	m_wLife	= wLife;	}
	WORD	GetEnergy( void )	{	return m_wEnergy;	}
	void	SetEnergy( WORD wEnergy );
	WORD	GetMaxEnergy( void )	{	return CPetProperty::GetInstance()->GetMaxEnergy( m_nLevel );	}
	int		GetEnergyPercent( void )	{	return GetEnergy() * 100 / GetMaxEnergy();	}
	int		GetExpPercent( void )	{	return( GetExp() * 100 / GetMaxExp() );		}
	int		GetMaxExp( void )	{	return ( m_nLevel == PL_EGG? MAX_PET_EGG_EXP: MAX_PET_EXP );	}
#ifdef __PET_1024
	void	SetName( const char* szName );
	const char*	GetName()	{	return m_szName;	}
	BOOL	HasName()	{	return strlen( m_szName ) > 0;	}
#endif	// __PET_1024
	void	InitEgg( void );
	DWORD	GetItemId( void )
		{
			if( m_nLevel == PL_EGG )
				return II_PET_EGG;
			return( CPetProperty::GetInstance()->GetAvailParam( m_nKind )->m_dwItemId );
		}
	CPet&	operator=( CPet & pet );
private:
	BYTE	m_nKind;	// ���� : 0~6
	BYTE	m_nLevel;	// ���� : e		// e = 0, d = 1, c = 2, b = 3, a = 4, s = 5
	DWORD	m_dwExp;	// ����ġ : 0
	WORD	m_wEnergy;		// ��� : 0
	WORD	m_wLife;	// ���� :  0 ~ 99	// ����Ʈ : 1	// ������ 0�� ���¿��� ��� �� ��ü ����
	BYTE	m_anAvailLevel[PL_MAX];	// �ɷ�ġ
#ifdef __PET_1024
	char	m_szName[MAX_PET_NAME];
#endif	// __PET_1024
};

#ifdef __CLIENT

class CAIEgg: public CAIInterface
{
private:
	OBJID m_idOwner;	// ���δ�
	int		m_nState;	// ���� ����.
	void MoveToDst(	OBJID idTarget );
	void MoveToDst(	D3DXVECTOR3 vDst );
	BOOL  MoveProcessIdle( const AIMSG & msg );
	BOOL  MoveProcessRage( const AIMSG & msg );
	void Init( void );
	void Destroy( void );
public:
	CAIEgg( CObj* pObj, OBJID idOwner );
	virtual ~CAIEgg();

	virtual void InitAI();

	int		GetState( void )	{	return m_nState;	}
	OBJID	GetOwnerId( void )	{	return m_idOwner;	}

	BOOL	StateInit( const AIMSG & msg );
	BOOL	StateIdle( const AIMSG & msg );
	BOOL	StateRage( const AIMSG & msg );

	DECLARE_AISTATE()
};
#endif	// __CLIENT

#endif	// __PET_0410

#if __VER >= 12 // __PET_0519
// ��ȯ �� �ʿ��� ���� ������ ���
typedef	struct	_TransformStuffComponent
{
	int		nItem;
	short	nNum;
	_TransformStuffComponent()
	{
		nItem	= 0;
		nNum	= 0;
	}
	_TransformStuffComponent( int nItem, short nNum )
	{
		this->nItem	= nItem;
		this->nNum	= nNum;
	}
}	TransformStuffComponent, *PTransformStuffComponent;
typedef	vector<TransformStuffComponent>	VTSC;

// ��ȯ �� �ʿ��� ������ ����
class CTransformStuff
{
public:
	CTransformStuff();
	CTransformStuff( int nTransform );
	virtual	~CTransformStuff();
	void	AddComponent( int nItem, short nNum );		// �ʿ� ������ ��� �߰�
	virtual	void	Serialize( CAr & ar );
	int		GetTransform( void )		{	return m_nTransform;	}	// ��ȯ ������ ��ȯ
	size_t	GetSize( void )		{	return m_vComponents.size();		}
	TransformStuffComponent*	GetComponent( int i )	{	return &m_vComponents[i];	}
private:
	int	m_nTransform;	// ��ȯ ����
	VTSC	m_vComponents;	// �ʿ� ������ ����
};

#ifdef __WORLDSERVER
class ITransformer
{
protected:
	ITransformer()	{}
public:
	virtual	~ITransformer()	= 0;
	static	ITransformer*	Transformer( int nTransform );
	virtual	BOOL	IsValidStuff( CUser* pUser, CTransformStuff & stuff );
	// ������� �������� ��ȯ�Ѵ�
	void	Transform( CUser* pUser, CTransformStuff& stuff );
	// ����ڷκ��� ��ȯ ��Ḧ �����Ѵ�
	void	RemoveItem( CUser* pUser, CTransformStuff& stuff );
	// ����ڿ��� ��ȯ ��� �������� ������ش�
	void	CreateItem( CUser* pUser, CTransformStuff& stuff );
};

// �˺�ȯ Ŭ����
class CTransformerEgg
	: public ITransformer
{
public:
	CTransformerEgg();
	virtual	~CTransformerEgg();
	virtual	BOOL	IsValidStuff( CUser* pUser, CTransformStuff & stuff );
	static	CTransformerEgg*	Instance( void );
};

// ��ȯ ��� ������ ���
// ������ ���� ���� ���
typedef	struct _TransformItemElement
{
	CItemElem*	pItem;	// ��ȯ ��� ������
	int		nProb;	// Ȯ��
	_TransformItemElement( CItemElem* pItem, int nProb )
	{
		this->pItem	= pItem;
		this->nProb	= nProb;
	}
}	TransformItemElement;

typedef	vector<TransformItemElement>	VTIE;

// �ϳ��� ��ü���� ��ȯ�� �ǹ��ϴ� ����̴�
class CTransformItemComponent
{
private:
	enum	{	eMaxProb	= 1000000	};
public:
	CTransformItemComponent( int nTransform );
	virtual	~CTransformItemComponent();
	void	Clear( void );
	void	AddElement( TransformItemElement element );
	void	AdjustmentProbability( TransformItemElement & element );
	CItemElem*	GetItem( void );
	void	SetStuffSize( u_int nStuffSize )	{	m_nStuffSize	= nStuffSize;	}
	u_int	GetStuffSize( void )	{	return m_nStuffSize;	}
	int		GetTransform( void )	{	return m_nTransform;	}
private:
	const	int		m_nTransform;
	u_int	m_nStuffSize;
	int		m_nTotalProb;
	VTIE	m_vTransformItemElements;
};

typedef	map<int, CTransformItemComponent*>	MPTIC;
// ��� ��ȯ�� �����Ѵ�
class CTransformItemProperty
{
public:
	CTransformItemProperty();
	virtual	~CTransformItemProperty();
	static	CTransformItemProperty*	Instance( void );
	void	AddComponent( CTransformItemComponent* pComponent );
	u_int	GetStuffSize( int nTransform );
	CItemElem*	GetItem( int nTransform );
	BOOL	LoadScript( const char* szFile );
	CItemElem*	CreateItemGeneric( CScript& s );
	CItemElem*	CreateItemPet( CScript& s );
private:
	CTransformItemComponent* GetComponent( int nTransform );
private:
	MPTIC	m_mapComponents;
};

#endif	// __WORLDSERVER

#endif	// __PET_0519

#endif	// __PET_H__
