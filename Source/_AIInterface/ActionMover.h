#ifndef __ACTIONMOVER_H
#define __ACTIONMOVER_H


#include "Mover.h"
#include "Action.h"
#include "MoverMsg.h"
#ifdef __CLIENT
#include "TailEffectMng.h"
#endif
#include "mempooler.h"

#ifdef __CLIENT
extern CDamageNumMng g_DamageNumMng;
#endif	// __CLIENT

#define MAX_MOVE_HEIGHT		270.0f
#define MIN_MOVE_HEIGHT		85.0f

#define	CTRLMSG_FORWARD		0x00000001
#define	CTRLMSG_BACKWARD	0x00000002
#define	CTRLMSG_LEFT		0x00000004
#define	CTRLMSG_RIGHT		0x00000008
#define	CTRLMSG_LDOWN		0x00000010
#define	CTRLMSG_RDOWN		0x00000020

#define	AF_GENERIC			(DWORD)0x00000001
#define	AF_MISS				(DWORD)0x00000002
#define	AF_RESERVED			(DWORD)0x00000004
#define	AF_MAGIC			(DWORD)0x00000008
#define AF_MELEESKILL		(DWORD)0x00000010
#define AF_MAGICSKILL		(DWORD)0x00000020
#define AF_CRITICAL1		(DWORD)0x00000040	// 2.3�� ũ��Ƽ��
#define AF_CRITICAL2		(DWORD)0x00000080	// 2.6�� ũ��Ƽ��
#define AF_CRITICAL			(DWORD)0x000000c0	// ���� �ΰ� ���Ѱ�.
#define AF_PUSH				(DWORD)0x00000100	// ������ �и�.
#define AF_PARRY			(DWORD)0x00000200	// ȸ��...
#define AF_RESIST			(DWORD)0x00000400	// ȸ��...
#define AF_STUN				(DWORD)0x00000800	// ���� - ����
#define AF_BLOCKING			(DWORD)0x00001000	// ���ŷ 
#define AF_FORCE			(DWORD)0x00002000	// ���� �������� �� ��� ��� (��: �ݻ絥����, ����ű ��) 
#define AF_RANGE			(DWORD)0x00004000	// ��Ÿ� ���� ������ 
#define AF_MONSTER_SP_CLIENT (DWORD)0x00008000	// gmpbigsun: ���� ����Ȱ���ó���� Ŭ������!! ������ AF_GENERIC���� ó���� CMover::OnAttackSP()
#define AF_FLYING			(DWORD)0x10000000	// ���������� ���ư�.

// ��ų���ΰ�?
inline BOOL IsSkillAttack( DWORD dwAtkFlags )
{
	return (dwAtkFlags & (AF_MELEESKILL|AF_MAGICSKILL) );
}


#	ifdef __WORLDSERVER
#		define	MAX_CORR_SIZE_45	5
#		define	MAX_CORR_SIZE_150	9
#	else	// __WORLDSERVER
#		define	MAX_CORR_SIZE_45	22
#		define	MAX_CORR_SIZE_150	37
#	endif	// __WORLDSERVER


/////////////////////////////////////////////////////////////////////
// CActionMover 
/////////////////////////////////////////////////////////////////////
class CActionMover;
typedef	MemPooler<CActionMover>	CActionMoverPool;

class CActionMover : public CAction
{
public:
	CActionMover();
	CActionMover( CMover* pMover );
	virtual	~CActionMover();

public:
	int				m_nCount;
	int				m_bGround;		// �ٴڿ� ����ִ�.
	float			m_fSpeed;		// �ٱ� ���ǵ�

	float			m_fAccPower;	// ������Ʈ�� �������� ��
	D3DXVECTOR3		m_vDelta;
	D3DXVECTOR3		m_vDeltaE;		// �ܺο��� �������� �� m_vDelta�� ���� ���°� ���������� �츮�ý����� ������ �������̶�� ������ ��� ���� �غ��ߴ�.
	D3DXVECTOR3		m_vDeltaAccu;	// delta�� ����
	D3DXVECTOR3		m_vPosLast;		// �̹� ��ǥ��
	float			m_fCurrentHeight;	// ���� ���ִ°��� Height��ǥ
	FLOAT			m_fTurnAngle;
	DWORD			m_idTarget;		// Ÿ���� ���̵�(�ٿ뵵 Ÿ��)
	int				m_nMotionEx;		
	float			m_fDistance;
	DWORD			m_dwAtkFlags;
	int				m_nShootCnt;	// �߻�ü ����.  ??�����ӿ� �ѹ��� �߻�ȴ�.
	int				m_nMotionHitCount;	
#ifdef __CLIENT
	DWORD			m_dwCtrlMsg;	// ��Ʈ�� �޽���
#endif
#ifdef __WORLDSERVER
	int				m_nDeadCnt;		
#endif

private:
	OBJID			m_objidHit;
	int				m_nCastingTime;
#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
	DWORD			m_dwCastingEndTick;
	int				m_nCastingSKillID;
	int				m_nCastingTick;
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans
	int				m_bMove;		// �̹��Ͽ� �������� �ִ°�?

#ifdef __CLIENT
	CTailEffectBelt*	m_pTail;		// ���������� ������ ����Ʈ
#endif

public:
	virtual int		SendActMsg( OBJMSG dwMsg, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0, int nParam4 = 0, int nParam5 = 0 );
	virtual	void	ProcessState( DWORD dwState, float fSpeed );	


#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
	int				GetCastingSKillID() { return m_nCastingSKillID;}
	DWORD			GetCastingEndTick() { return m_dwCastingEndTick;}
	void			SetCastingEndTick(DWORD dwCastingEnd) { m_dwCastingEndTick = dwCastingEnd;}
	int				GetCastingTick() { return m_nCastingTick;}
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans

	void			ForcedSet( D3DXVECTOR3 &vDelta, DWORD dwState, DWORD dwStateFlag, int nMotionEx );
	void			ForcedSet2( D3DXVECTOR3 &vDelta, float fAccPower, float fTurnAngle, DWORD dwState, DWORD dwStateFlag, int nMotionEx );
	void			DefaultSet();
	void			HalfForcedSet( D3DXVECTOR3 & vd, float fAccPower, float fTurnAngle );

	void			DoDamageFly( float fAngleXZ, float fAngleY, float fPower );		
	void			SetExternalForce( const D3DXVECTOR3 &vForce ) { m_vDeltaE = vForce; }		// �ܺο��� �������� ��.
	int				MakeParamDamage( int nMagicPower, int nSkill, int nDmgCnt = 0 );
	int				SendDamage( DWORD dwAtkFlags, OBJID idSender, int nParam = 0, BOOL bTarget = TRUE, int nReflect = 0 )	{	return SendActMsg( OBJMSG_DAMAGE, dwAtkFlags, idSender, nParam, bTarget, nReflect );	}
	int				SendDamageForce( DWORD dwAtkFlags, OBJID idSender, int nParam = 0, BOOL bTarget = TRUE );// { return SendActMsg( OBJMSG_DAMAGE_FORCE, dwAtkFlags, idSender, nParam, bTarget ); }	// ������

	// �������(_) �����Լ��� XuZhu�̿ܿ� ���� Call���� ����
	DWORD			_ProcessMsgDmg( DWORD dwMsg, CMover* pSender, DWORD dwAtkFlags, int nParam = 0, BOOL bTarget = TRUE, int nReflect = 0 );
	void			_ProcessStateAttack( DWORD dwState, int nParam = 0 );	// ���ݺκ��� �ڵ尡 �� ���� ����
	void			_ProcessStateAttack2( DWORD dwState, int nParam = 0 );

	void			ProcessStateAttackSP( DWORD dwState, int nParam = 0 );
	void			ProcessStateAttackMagic( DWORD dwState, int nParam = 0 );		// �ϵ����
	void			ProcessStateAttackRange( DWORD dwState, int nParam = 0 );		// ����������
	void			ProcessStateMeleeSkill( DWORD dwState, int nParam );	// ��ų���� ó��
	void			ProcessStateMagicSkill( DWORD dwState, int nParam );	// ������ų���� ó��
	void			ProcessStateMagicCasting( DWORD dwState, int nParam );
	void			ProcessPreAction();
	int				ProcessAction( const D3DXVECTOR3 *vPos );
	int				ProcessActMsg1( CMover* pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4, int nParam5 );
	int				ProcessActMsg2( CMover* pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nparam3, int nParam4, int nParam5 );
	void			ProcessState1( CMover* pMover, DWORD dwState, float fSpeed );
	void			ProcessState2( CMover* pMover, DWORD dwState, float fSpeed );
	void			ProcessFlyMove();
	BOOL			ProcessCollisionFly( D3DXVECTOR3 *vPos );
	BOOL			ProcessCollisionGround( D3DXVECTOR3 *vPos );
	BOOL			ProcessCollision( D3DXVECTOR3 *vPos );


	void			SetObjHit( OBJID idHit ) {	m_objidHit = idHit;	}
	OBJID			GetObjHit() {	return m_objidHit; }	
	CMover*			PreProcessDamage( CMover* pMover, OBJID idAttacker, BOOL bTarget, int nReflect );

#ifdef __CLIENT
	void			PresupposePos( D3DXVECTOR3* pv, D3DXVECTOR3* pvd, float* pf, u_long uTickCount );
	void			PresupposePos2( D3DXVECTOR3* pv, D3DXVECTOR3* pvd, float* pf, float* pfAngleX, float* pfAccPower, u_long uTickCount );
	DWORD			OnDamageMsgC( DWORD dwMsg, CMover* pAttacker, DWORD dwAtkFlags, int nParam );
#endif	// __CLIENT

#ifndef __VM_0820
#ifndef __MEM_TRACE
	static			CActionMoverPool*	m_pPool;
	void*			operator new( size_t nSize )	{	return CActionMover::m_pPool->Alloc();	}
	void*			operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CActionMover::m_pPool->Alloc();	}
	void			operator delete( void* lpMem )	{	CActionMover::m_pPool->Free( (CActionMover*)lpMem );	}
	void			operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CActionMover::m_pPool->Free( (CActionMover*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820

protected:
	void			Init();
	void			Destroy();
	void			ProcessFlyParticle( float fLenSq );
	void			ProcessFlyTracking();
};

inline int CActionMover::MakeParamDamage( int nMagicPower, int nSkill, int nDmgCnt)
{
	int nParam = nMagicPower;	// D0~D7
	nParam |= (nDmgCnt << 8);	// D7~D15
	nParam |= (nSkill << 16);	// D16~D31
	return nParam;
}



#endif	// __ACTIONMOVER_H