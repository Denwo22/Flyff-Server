#ifndef __RANGDA_H__
#define	__RANGDA_H__

// ���� �̺�Ʈ ���Ͱ� ��Ÿ���� ������ ��Ÿ���� ����ü
typedef	struct	_RANGDA_POS
{
	DWORD	dwWorldId;	// ��Ÿ�� �� �ִ� ���� ���� �ĺ���
	D3DXVECTOR3 vPos;	// ��Ÿ�� �� �ִ� ���� ��ǥ
}	RANGDA_POS,	*PRANGDA_POS;

typedef	vector<RANGDA_POS>	VRP;
// ���� ���� �̺�Ʈ ���͸� �����ϴ� Ŭ����
class CRangda
{
public:
	CRangda( DWORD dwMonster );
	CRangda( DWORD dwMonster, int nInterval, int nReplace, BOOL bActiveAttack );
	virtual	~CRangda();
	void	AddPos( const RANGDA_POS & vPos );	// ��ǥ �߰�
	void	OnTimer();	// ƽ ó��
	void	SetInterval( int nInterval )	{	m_nInterval	= nInterval;	}	// ��� ���� ����
	void	SetReplace( int nReplace )	{	m_nReplace	= nReplace;		}	// �̵� ���� ����
	void	SetActiveAttack( BOOL bActiveAttack )	{	m_bActiveAttack		= bActiveAttack;	}	// ���� ���� ����

private:
	RANGDA_POS	GetRandomPos( void );	// ���� ��ǥ ��ȯ
	void	SetObj( OBJID objid );		// �� ��ü ����
	BOOL	HavetoGenerate( void );		// ��� ������ �����ϴ°�?
	BOOL	HavetoReplace( void );		// �̵� ������ �����ϴ°�?
	void	CreateMonster( void );	// �� ��ü ����
	BOOL	IsDamaged( CMover* pMonster );	// ��ó �Ծ���?
	void	ProcessReplace( CMover* pMonster );		// �̵� ó��
	void	ProcessGenerate( void );	// ��� ó��
	CMover*		GetMonster( void );		// �� ��ü ��ȯ
private:
	const DWORD	m_dwMonster;	// ������ ���� ���� �ĺ���
	int		m_nInterval;	// ��� ����
	int		m_nReplace;		// �̵� ����
	BOOL	m_bActiveAttack;	// ���� ����
	VRP	m_vvPos;	// ��Ÿ�� �� �ִ� ��ǥ ����
	int		m_nGenerateCountdown;	// ��� ��� �ð� ƽ�� 1 ����
	int		m_nReplaceCountdown;	// �̵� ��� �ð�, ƽ�� 1 ����
	BOOL	m_bReplaceable;		// �̵� ���� �÷���
	int		m_nOldPos;		// ���� ��ǥ
	OBJID	m_objid;	// �� ��ü �ĺ���
};

typedef	vector<CRangda*>	VR;
// ��� ���� �̺�Ʈ ���Ϳ� ���� ���� Ŭ����
class CRangdaController
{
public:
	virtual	~CRangdaController();
	static	CRangdaController*	Instance( void );
	void	AddRangda( CRangda* pRangda );	// ���� �̺�Ʈ ���� �߰�
	void	OnTimer( void );	// ƽ
	BOOL	LoadScript( const char* szFile );
private:
	CRangdaController();
private:
	VR	m_vRangda;
};
#endif	// __RANGDA_H__