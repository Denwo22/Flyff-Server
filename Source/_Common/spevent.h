#ifndef __SPEVENT_H__
#define	__SPEVENT_H__

class CSPEvent
{
private:

public:
	CSPEvent()	{}
	virtual	~CSPEvent()	{}
};

class CXMasEvent : public CSPEvent
{
public:
	CXMasEvent();
	virtual	~CXMasEvent();
	BOOL	LoadScript( LPCTSTR lpFilename );
	BOOL	IsTimeout( int nHour );
	void	Skip( LONG lSkip );

	static	CXMasEvent*	GetInstance( void );

private:
	DWORD	m_adwInterval[24];
	DWORD	m_dwTimeout;
	LONG		m_lSkip;
};

#define	MAX_EVENT_TITLE		100
typedef	struct	_EVENT_GENERIC
{
	int	nId;	// �̺�Ʈ ��ȣ, 500�� ����, �ִ�� 531
	int	nFlag;		// �̺�Ʈ �÷���, 1���� (�̺�Ʈ ��ȣ - 500) ���� ����Ʈ
	time_t	tStart;		// ���� �ð�
	time_t	tEnd;		// �� �ð�
	FLOAT	fExpFactor;
#ifdef __ITEMDROPRATE
	FLOAT	fItemDropRate;
#endif // __ITEMDROPRATE
	char	pszTitle[MAX_EVENT_TITLE];
	_EVENT_GENERIC()
	{
		nId	= 0;
		nFlag	= 0;
		tStart	= 0;
		tEnd	= 0;
		*pszTitle	= '\0';
		fExpFactor	= 1.0f;
#ifdef __ITEMDROPRATE
		fItemDropRate = 1.0f;
#endif // __ITEMDROPRATE
	}
}	EVENT_GENERIC,	*PEVENT_GENERIC;

class CEventItem
{
private:
	int		m_nMax;		// ���� �ִ� ���
	DWORD	m_adwInterval[24];
	DWORD	m_dwTimeout;
	LONG	m_lSkip;
public:
	DWORD	m_dwItemId;		// ������ ��ȣ
	int		m_nNum;		// ��ȸ �ִ� ���
public:
//
	CEventItem();
	CEventItem( DWORD dwItemId, int nMax, int nNum );
	virtual	~CEventItem()	{}
//	op
	BOOL	IsTimeout( int nHour );		// ��� �ֱ� �ΰ�?
	void	Skip( LONG lSkip );		// ��� �ֱ⿡ ��� ���� ȸ�� ����
	void	Serialize( CAr & ar );
};
// �ھ� �������� ������Ʈ�� �ε����� �����Ƿ�
// Ʈ���� �������� ��ü�� �����Ѵ�.

#ifdef __EVENT_0117
typedef	struct	_REGION_GENERIC		// ���� �̺�Ʈ (������, ����)�� ���� ���� ���� ����
{
	int		nLevel;	// ���� ����
	DWORD	dwWorldId;		// ���� ��ȣ
	CRespawnInfo* pi;	// ���� ����
}	REGION_GENERIC,	*PREGION_GENERIC;

class CSpawn
{
private:
	int		m_nMax;		// ���� �ִ� ����
	float	m_fRatio;	// ���� ���� ������ ��� �ְ����� ���� Ȯ��
	DWORD	m_dwInterval;	// ���� ���� ms
	DWORD	m_dwTimeout;	// ���� �ð� Ÿ�� �ƿ�
public:
	DWORD	m_dwType;	// ���� ��ü Ÿ��	
	DWORD	m_dwIndex;		// ���� ��ü �ε���
public:
// Constructions
	CSpawn();
	CSpawn( DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval );
	virtual	~CSpawn()	{}
//	Operations
	BOOL	IsTimeout( void );	// ���� �ֱ� �ΰ�?
private:
	DWORD	GetMaxSpawnProbability( int nMax )
	{
		ASSERT( m_fRatio > 0 );
		return	( (DWORD)( ( nMax + ( m_fRatio - 1.0F ) * ( nMax - 1 ) / 2.0F ) * 100.0F ) );
	}
public:
	int	GetSpawnIndex( int nMax )
	{
		ASSERT( m_fRatio > 0 );
		DWORD dwProb	= xRand() % GetMaxSpawnProbability( nMax );
		float c	= ( m_fRatio - 1.0F ) / (float)nMax;
		float D	= pow( c + 2, 2 ) - 4 * c * ( 2 - dwProb / 50.0F );
		int nIndex	= 0;
		if( D > 0 )
			nIndex	= (int)( ( - ( 2 + c ) + sqrt( D ) ) / ( 2 * c ) + 1 );
		else
			nIndex	= (int)( ( - ( 2 + c ) - sqrt( D ) ) / ( 2 * c ) + 1 );
		if( nIndex < 0 || nIndex >= nMax )
		{
			// error
			nIndex	= 0;
		}
		return nIndex;
	}
};
#endif	// __EVENT_0117

#ifdef __EVENT_0117
class CRespawnInfo;
#endif	// __EVENT_0117
class CEventGeneric
{
private:
	DWORD	m_dwFlag;
	list<PEVENT_GENERIC>	m_lspEvent;
	map<int, list<CEventItem*>*>	m_mapEventItemList;

#ifdef __EVENT_0117
	vector<REGION_GENERIC>	m_aRegionGeneric;	// ��� ������ ������ ������ ���Ϳ� �����Ѵ�.	// ���� ��ȣ, ����, ����
	map<int, CSpawn*>	m_mapSpawn;		// �̺�Ʈ�� Ȱ��ȭ �Ǿ��� ��, �ش� �̺�Ʈ�� ������ �����Ѵٸ� �������� ���� ���̴�.
#endif	// __EVENT_0117

public:
	CEventGeneric();
	virtual	~CEventGeneric();
//	op
	void Clear( BOOL bDestructor = TRUE );
	// ��ũ��Ʈ �ε�
	BOOL	LoadScript( LPCSTR lpFilename );
	// �̺�Ʈ Ȱ��ȭ �� ��ӵǴ� ������ �߰�
	BOOL	AddItem( int nEvent, DWORD dwItemId, int nMax, int nNum );
#ifdef __WORLDSERVER
	CEventItem*	GetItem( int* pnNum );
	FLOAT	GetExpFactor( void );
#ifdef __ITEMDROPRATE
	FLOAT	GetItemDropRateFactor( void );
#endif // __ITEMDROPRATE
#ifdef __EVENT_1101
	LONG	GetEventElapsed( void );
	void	CallTheRoll( void );
#endif	// __EVENT_1101

#ifdef __EVENT_0117
	void	AddSpawn( int nEvent, DWORD dwType, DWORD dwIndex, int nMax, float fRatio, DWORD dwInterval );		// ���� �̺�Ʈ ���� ���
	void	AddRegionGeneric( int nLevel, DWORD dwWorldId, CRespawnInfo* pi );	// ���� ���� ���
	void	SortRegionGeneric( void );	// ���� ���� ����
	void	Spawn( void );
#endif	// __EVENT_0117

#endif	// __WORLDSERVER
	static	CEventGeneric*	GetInstance( void );
	void	Serialize( CAr & ar );
	DWORD	GetFlag( void )		{	return m_dwFlag;	}
	BOOL	Run( void );
	list<PEVENT_GENERIC>*	GetEventList( void )	{	return &m_lspEvent;		}
	PEVENT_GENERIC	GetEvent( int nEvent );
};

#endif	// __SPEVENT_H__