// Billboard.cpp : implementation of the CBeastView class
//

#include "stdafx.h"
#include "Action.h"


CAction::CAction()
{
	Init();
}
CAction::CAction( CMover* pMover ) 
{ 
	Init();
	m_pMover = pMover;
}

CAction::~CAction()
{
}

void CAction::Init( void )
{
	m_pMover		= NULL;
	m_dwState		= 0;
	m_dwStateFlag	= 0;
}

void	CAction::AddStateFlag( DWORD dwFlag ) 
{
#ifdef __BS_EFFECT_LUA
	//���°� �߰��� ��� �����ΰ�쿡 ȿ���� �߻���Ų��.
	if( dwFlag & OBJSTAF_COMBAT && !IsStateFlag( OBJSTAF_COMBAT ) )	
	{
		// ���� �������·� ó�� �����ϴ°Ŷ�� ���º� ȿ���� ȣ���Ѵ�. �ϴ� npc���� 
		if( m_pMover->IsNPC( ) )
			run_lua_sfx( OBJSTAF_COMBAT, m_pMover->GetId(), m_pMover->GetNameO3D() );
	}
#endif //__BS_EFFECT_LUA
	m_dwStateFlag |= dwFlag; 
}

void	CAction::RemoveStateFlag( DWORD dwFlag )
{
#ifdef __BS_EFFECT_LUA
	// ���°� �����ɰ�� lua���� ȣ���� ��� ����ȿ���� �����Ѵ�.
	if( dwFlag & OBJSTAF_COMBAT )
	{
		//���� ����ȿ�� ����
		CSfxModelMng::GetThis()->SubData( m_pMover->GetId(), OBJSTAF_COMBAT );
	}
#endif //__BS_EFFECT_LUA
	m_dwStateFlag &= (~dwFlag);
}

void CAction::ProcessAtkMsg( void )
{
	if( !m_qMeleeAtkMsg.Process( this ) )
		m_qMagicAtkMsg.Process( GetMover() );
}


CWorld* CAction::GetWorld( ) 
{ 
	return m_pMover->GetWorld(); 
}

// OBJSTA_MOVE_ALL ������Ʈ�� Ŭ����Ǵ� ���� �߻�.
void	CAction::OnEndMoveState( void )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
	switch( m_dwState & OBJSTA_MOVE_ALL )
	{
	case OBJSTA_SIT:		// ���� ���� ��.
		m_dwStateFlag &= (~OBJSTAF_SIT);
		break;
	}	
}

void	CAction::OnEndTurnState( void )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
}

void	CAction::OnEndLookState( void )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
}

void	CAction::OnEndJumpState( DWORD dwState )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
	switch( dwState & OBJSTA_JUMP_ALL )
	{
	case OBJSTA_SJUMP4:		// ���� ���� ��.
		m_pMover->OnActEndJump4();
		break;
	}
}

void	CAction::OnEndAttackState( DWORD dwState )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
	switch( dwState & OBJSTA_ATK_ALL )
	{
	case OBJSTA_ATK1:
	case OBJSTA_ATK2:
	case OBJSTA_ATK3:
	case OBJSTA_ATK4:
	case OBJSTA_ATK_MAGIC1:
	case OBJSTA_ATK_RANGE1:
	case OBJSTA_RANGE3:
	case OBJSTA_RANGE4:
#ifdef __WORLDSERVER
		if( m_pMover->IsPlayer() && m_pMover->IsFly() )	
			return;

		if( !IsAtkMsgEmpty() )
			ProcessAtkMsg();
#else // worldserver
		if( m_pMover->IsActiveMover() == FALSE )
		{
			if( !IsAtkMsgEmpty() )
				ProcessAtkMsg();
		}
#endif // not worldserver
		break;
	case OBJSTA_ATK_MELEESKILL:
		m_pMover->OnActEndMeleeSkill();		// ������ų ���� ����.
		break;
	case OBJSTA_ATK_MAGICSKILL:
		m_pMover->OnActEndMagicSkill();		// ������ų ���� ����.
		break;
	}
}

void	CAction::OnEndDamageState( void )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
}

void	CAction::OnEndActionState( void )
{
	// �̰��� �׿� �ʿ��� �ڵ带 ������.
}

void	CAction::ClearState( void )
{
	DWORD dwState = m_dwState;
	// 0xFFFFFFFF �� �Ѳ����� ���� ��Ű�� ����.
//	ResetState( OBJSTA_MOVE_ALL | OBJSTA_TURN_ALL | OBJSTA_LOOK_ALL | OBJSTA_JUMP_ALL | OBJSTA_ATK_ALL | OBJSTA_DMG_ALL );
	ResetState( OBJSTA_ALL );
	
	if( m_dwState )
	{
		Error( "CAction::ClearState : ������ ���� Ŭ������� ���� ���� �ִ�. %08x, %08x", dwState, m_dwState );
		m_dwState = 0; 
	}
}

// dwState���� OBJSTA���� ������ �� �� �ִ�.
// �����ϰ��� �ϴ� ������Ʈ�� Ŭ�����Ű�鼭 �ش� �ڵ鷯 �ҷ���.
void	CAction::ResetState( DWORD dwState )
{ 
	DWORD dw	= m_dwState;
	if( dwState & OBJSTA_MOVE_ALL )
	{
		OnEndMoveState();				// �ڵ鷯 ȣ��
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
	}
	
	if( dwState & OBJSTA_TURN_ALL )
	{
		OnEndTurnState();
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
	}
	
	if( dwState & OBJSTA_LOOK_ALL )
	{
		OnEndLookState();
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
	}
	
	if( dwState & OBJSTA_JUMP_ALL )
	{
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
		OnEndJumpState( dw );
	}
	
	if( dwState &	OBJSTA_ATK_ALL )
	{
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
		OnEndAttackState(dw);
	}
	
	if( dwState &	OBJSTA_DMG_ALL )
	{
		OnEndDamageState();
		m_dwState &= (~dwState);		// �ش� ��Ʈ�� Ŭ����.
	}
	
	if( dwState & OBJSTA_ACTION_ALL )
	{
		OnEndActionState();
		m_dwState &= (~dwState);
	}
}

#ifndef __OPT_MEM_0811
//{{AFX
CActMsgq::CActMsgq()
:
m_uHead( 0 ),
m_uTail( 0 )
{
}

CActMsgq::~CActMsgq()
{
}

void CActMsgq::RemoveHead()
{
	if( IsEmpty() )
		return;
	LPACTMSG lpActMsg	= m_aActMsg + m_uHead;
	m_uHead		= ( m_uHead + 1 ) % MAX_ACTMSG;
}

void CActMsgq::AddTail( const ACTMSG & rActMsg )
{
	u_long uTail	= ( m_uTail + 1 ) % MAX_ACTMSG;
	if( uTail == m_uHead )
		return;
	memcpy( ( m_aActMsg + m_uTail ), &rActMsg, sizeof(ACTMSG) );
	m_uTail	= uTail;
}

void CActMsgq::AddTail( OBJMSG dwMsg, int nParam1, int nParam2, int nParam3 )
{
	u_long uTail	= ( m_uTail + 1 ) % MAX_ACTMSG;
	if( uTail == m_uHead )
		return;
		LPACTMSG lpActMsg	= m_aActMsg + m_uTail;
	lpActMsg->dwMsg	= dwMsg;
	lpActMsg->nParam1	= nParam1;
	lpActMsg->nParam2	= nParam2;
	lpActMsg->nParam3	= nParam3;
	m_uTail	= uTail;
}

LPACTMSG CActMsgq::GetHead()
{
	return ( IsEmpty()? NULL: m_aActMsg + m_uHead );
}
//}}AFX
#endif	// __OPT_MEM_0811