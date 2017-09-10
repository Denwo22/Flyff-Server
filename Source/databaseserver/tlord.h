#ifndef __TLORD_H__
#define	__TLORD_H__

#if __VER >= 12 // __LORD

#include "lord.h"
#include "dbcontroller.h"
#include "post.h"

////////////////////////////////////////////////////////////////////////////////
class CLController;
// Ʈ���� ������ ���� Ŭ����
class CTLord
	: public CLord
{
public:
	// ó�� ��û
	enum	{	eBeginCandidacy, eBeginVote, eEndVote, eAddDeposit, eSetPledge, eIncVote, eInit, eLEventCreate, eLEventInitialize, eLordSkillUse	};
public:
	CTLord();
	virtual	~CTLord();
	static	CTLord*	Instance( void );
	// ���� ��ü ����
	virtual	void	CreateColleagues( void );
	// ���� ��ü ����
	virtual void	DestroyColleagues( void );
	// ��� ���� �ý��� ���� ��ü�� ����
	BOOL	RestoreAll( void );
	// ���� ���� ����
	BOOL	Restore( CQuery* pQuery );
	// ���� ���� ��û�� ���� �ڵ鷯
	void	Handler( LPDB_OVERLAPPED_PLUS pov, DWORD dwCompletionKey );
	// ƽ
	void	OnTimer( void );
	// ó�� ��û
	BOOL	PostRequest( int nQuery, BYTE* lpBuf = NULL, int nBufSize = 0, DWORD dwCompletionKey = 0 );
	virtual	CLController*	GetController( void )	{	return m_pController;	}
private:
	CLController*	m_pController;
};

////////////////////////////////////////////////////////////////////////////////
typedef	struct	_PostItemStruct
{
	int	nItem;
	DWORD dwKeepTime;
	_PostItemStruct() : nItem( 0 ), dwKeepTime( 0 )
	{
	}
}	PostItemStruct;

class CQuery;
// Ʈ���� ������ ���� Ŭ����
class CTElection :
	public IElection
{
public:
	CTElection( CLord* pLord );
	virtual	~CTElection();
	// �ʱ�ȭ �� ��ũ��Ʈ �ε�
	BOOL	Initialize( const char* szFile );
	// �ش� ������� ������ ���� ������ ��ȯ��
	float	GetRetRate( u_long idPlayer );
	// ���μ��� ���� ����
	void	SetRun( BOOL bRun )		{	m_bRun	= bRun;		}
	// ���� ���� ���μ����� �������ΰ�?
	BOOL	IsRunable( void )	{	return m_bRun;	}
	// ƽ
	void	OnTimer( void );
	// ����
	BOOL	Restore( CQuery* pQuery );

public:
	// �ð�
	static	CString	ttos( time_t t );
	static	time_t	stot( const char* sz );

protected:
	// ���ø� �޼ҵ� �Լ���
	// ���ĺ� ���� ������ ���� �� ����
	virtual	BOOL	DoTestBeginCandidacy( void );
	// �������� ������ ���� ��ȯ
	virtual	void	DoReturnDeposit( void );
	// ��ǥ ���� ������ ���� �� ����
	virtual	BOOL	DoTestBeginVote( int & nRequirement );
	// ��ǥ ���� ������ ���� �� ����(���� ����)
	virtual	BOOL	DoTestEndVote( u_long idPlayer );
	// ���� ���� ���� �� ��� ����
	virtual	BOOL	DoTestAddDeposit( u_long idPlayer, __int64 iDeposit, time_t tCreate );
	virtual	void	DoAddDepositComplete( u_long idPlayer, __int64 iDeposit, time_t tCreate );
	// ���� ���� ���� �� ��� ����
	virtual	BOOL	DoTestSetPledge( u_long idPlayer, const char* szPledge );
	// ��ǥ ���� �� ��� ����
	virtual	BOOL	DoTestIncVote( u_long idPlayer, u_long idElector );

private:
	// ��ũ��Ʈ �ε�
	BOOL	ReadIni( const char* szFile );
	// ������ ���� ����	- ���� �Լ��� ���� �ʿ�
	void	PostItem( u_long idPlayer, PostItemStruct & pi );
	// �ĺ��ڵ鿡�� ������ ��ȯ
	void	PostDepositToAllCandidate( void );
	// ����� ���ֿ��� ���� ������ ���� ����
	void	PostLordItems( u_long idLord );

private:
	// ���μ��� ���� ����
	BOOL	m_bRun;
};

// Ʈ���� ������ ���� ��ų
class CTLordSkill
	: public CLordSkill
{
public:
	CTLordSkill( CLord* pLord );
	virtual	~CTLordSkill();
	virtual	CLordSkillComponentExecutable*	CreateSkillComponent( int nType );	// template method
	// ����
	BOOL	Restore( CQuery* pQuery );
	// ƽ
	void	OnTimer( void );
};

////////////////////////////////////////////////////////////////////////////////
class CQuery;
// Ʈ���� ������ ���� �̺�Ʈ
class CLEvent : public ILordEvent
{
public:
	CLEvent( CLord* pLord );
	virtual	~CLEvent();
	// ����
	BOOL	Restore( CQuery * pQuery );
	// ƽ
	void	OnTimer( void );
protected:
	// ���ø� �޼ҵ� �Լ�
	// ���� �̺�Ʈ �߰��� ���� ���� �� ��� ����
	virtual	BOOL	DoTestAddComponent( CLEComponent* pComponent );
	// ���� �̺�Ʈ �ʱ�ȭ ���� �� ��� ����
	virtual	BOOL	DoTestInitialize( void );
	// ��� ���� �̺�Ʈ�� ���� �ð� ����
	BOOL	DecrementAllComponentTick( void );
};

// ������ ��ȯ�� ó���ϱ� ���� �Լ��� Ŭ����
struct	PostDeposit:
	public unary_function< SPC, void >	{
	void operator() ( const SPC & lhs )	const;
	PostDeposit( CQuery* pQuery )	{	m_pQuery	= pQuery;	}
private:
	CQuery* m_pQuery;
};

////////////////////////////////////////////////////////////////////////////////
class CLordSkillComponentODBC;
// ���ֿ� ���õ� ��� ó���� ����ȭ�� ���� Ŭ����
// ������ ������ ����
// �����ͺ��̽� ���� ��ü ����
class CLController : public CDbController
{
public:
	CLController( CTLord* pLord );
	virtual	~CLController();

	// ����
	BOOL	Restore( void );
	// ���ĺ� ���� ���� ����
	BOOL	BeginCandidacy( void );
	// ��ǥ ���� ���� ����
	int		BeginVote( void );
	// ��ǥ ���� ���� ����
	BOOL	EndVote( u_long idPlayer );
	// ���� ���� ����
	BOOL	AddDeposit( u_long idPlayer, __int64 iDeposit, time_t tCreate );
	// ���� ���� ����
	BOOL	SetPledge( u_long idPlayer, const char* szPledge );
	// ��ǥ ����
	BOOL	IncVote( u_long idPlayer, u_long idElector );
	// ���� �̺�Ʈ ���� ����
	BOOL	AddLEComponent( CLEComponent* pComponent );
	// ���� �̺�Ʈ �ʱ�ȭ ����
	BOOL	InitializeLEvent( void );
	// ���� ��ų ���� ��� �ð� ����
	BOOL	UpdateLordSkillTick( CLordSkillComponent* pSkill, int nTick );
	// ���� �̺�Ʈ ���� �ð� ����
	BOOL	UpdateLordEventTick( CLEComponent* pComponent );
	// ó���� m_pLord�� ����
	virtual	void	Handler( LPDB_OVERLAPPED_PLUS pov, DWORD dwCompletionKey );
	// ó���� m_pLord�� ����
	virtual	void	OnTimer( void ); 

private:
	CTLord*	m_pLord;	// ���� ��ü
};

#endif	// __LORD

#endif	// __TLORD_H__
