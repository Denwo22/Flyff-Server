#include "stdafx.h"

#include "AIGuard.h"
#include "lang.h"
#include "defineObj.h"
#include "dialogMsg.h"
#include "User.h"
#include "dpcoreclient.h"
#include "Party.h"

extern	CUserMng		g_UserMng;
extern	CDPCoreClient	g_DPCoreClient;
extern	CPartyMng		g_PartyMng;


#define RANGE_MOVE   15.0f

enum
{
	STATE_INIT = 1,
	STATE_IDLE,
	STATE_WANDER,
	STATE_PURSUE,
	STATE_EVADE,
	STATE_RUNAWAY,
	STATE_RAGE
};
BEGIN_AISTATE_MAP( CAIGuard, CAIInterface )

	ON_STATE( STATE_INIT   , &CAIGuard::StateInit   )
	ON_STATE( STATE_IDLE   , &CAIGuard::StateIdle   )
	ON_STATE( STATE_WANDER , &CAIGuard::StateWander )
	ON_STATE( STATE_EVADE  , &CAIGuard::StateEvade  )
	ON_STATE( STATE_RAGE   , &CAIGuard::StateRage   )
	ON_STATE( STATE_RUNAWAY, &CAIGuard::StateRunaway  )

END_AISTATE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __VM_0820
#ifndef __MEM_TRACE
	#ifdef __VM_0819
	MemPooler<CAIGuard>*	CAIGuard::m_pPool		= new MemPooler<CAIGuard>( 16, "CAIGuard" );
	#else	// __VM_0819
	MemPooler<CAIGuard>*	CAIGuard::m_pPool		= new MemPooler<CAIGuard>( 16 );
	#endif	// __VM_0819
#endif	// __MEM_TRACE
#endif	// __VM_0820

static DWORD s_tmAttack = SEC(10);		//  m_tmAttack 10�� 

CAIGuard::CAIGuard()
{
}
CAIGuard::CAIGuard( CObj* pObj ) : CAIMonster( pObj )
{
}

CAIGuard::~CAIGuard()
{
}

void CAIGuard::InitAI()
{
	// ����Ŭ���� InitAI()�θ����� Ȯ���� ��
}


BOOL CAIGuard::StateInit( const AIMSG & msg )
{
	return TRUE;
}

BOOL CAIGuard::MoveProcessIdle( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	MoverProp *pProp = pMover->GetProp();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) )
		return FALSE;

/*
	if( �̵���� �ִ°�? )
	{
		��Ʈ���� �̵���� ��带 ���� �̵��� ��Ŵ(���� ����)
	} else
		���� �ڸ����� ���;
*/

	{
		// ���ÿɼ��� �ִ� ���̴�.
/*		if( pProp->m_nLoot )
		{
			if( (pMover->GetCount() & 63) == 0 )		// ������ �ֺ��� ��ĵ�ؼ�.(63�̸� �� 4.2��)
			{	
				if( pProp->m_nLootProb == 0 || xRandom(100) < pProp->m_nLootProb  )	// ����Ʈ��(0)�� 100% / �̰ų� ����Ȯ���� ���� ���ýõ�
					if( m_bLootMove == FALSE )		// �����Ϸ� ���� ������ �� üũ�ϸ� �Ȋ�
						SubItemLoot();		// �������� ������.
			}
		}

		if( m_bLootMove == TRUE )
		{
			CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
			if( IsInvalidObj(pCtrl) )		// ������ ������ �̵��߿� ������ ��������
			{
				MoveToDst( pMover->GetPos() );	// ���ڸ��� ��.
			}
		} */

		// �����Ӽ��̸� �ֺ��� �� �����Ѵ�.
		if( GetMover()->m_bActiveAttack )
		{
			int nAttackFirstRange = pMover->GetProp()->m_nAttackFirstRange;

			if( pMover->GetProp()->m_nAttackFirstRange > 10 || pMover->GetProp()->m_nAttackFirstRange <= 0 )
			{
				Error( "m_nAttackFirstRange �̻�:%d %s\n", pMover->GetProp()->m_nAttackFirstRange, pMover->GetName() );
				return TRUE;
			}

			MoverProp *pProp = GetMover()->GetProp();
			CMover* pTarget = ScanTarget( pMover, nAttackFirstRange, pProp->m_nScanJob );		// �ڱ� ī������ �ݴ�Ǵ� ���� �˻������ �߰�.
			if( IsValidObj( (CObj*)pTarget ) )
			{
				// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_dwIdTarget = pTarget->GetId();
					// Ÿ���� �����ϸ� ���� ���� ���� 
					if( m_dwIdTarget != NULL_ID )	
					{
						g_UserMng.AddDlgEmoticon( pMover, DLGEMOT_ATTACK );
						SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CAIGuard::StateIdle( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		float fRangeMove = RANGE_MOVE;
		if( pMover->IsFlyingNPC() )
			fRangeMove *= 3.0f;
		D3DXVECTOR3	vTargetTo = pMover->GetPos() - m_vPosBegin;
		if( IsInRange( vTargetTo, fRangeMove ) == FALSE )
			m_bReturnToBegin = TRUE;
		SetStop( SEC( 0 ) );
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )
		m_dwIdTarget = NULL_ID;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessIdle( msg );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		//�г� ���� ����!
		if( m_bReturnToBegin == FALSE || (m_bReturnToBegin && (int)(g_tmCurrent - m_tmReturnToBegin) > SEC(5)) )
		{
			m_dwIdTarget = msg.dwParam1;
			MoveToDst( m_dwIdTarget );
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
			m_tmAttack = timeGetTime() + s_tmAttack;	// ������϶� ���ݹ����� 10�� Ÿ�̸� ����
		}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		MoveToRandom( STATE_IDLE );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		if( pMover->IsFlyingNPC() )		
		{
			// ������� �����ϰ� ���� ������ �ʴ´�. 8�ں������� �̵������� �ٲ��.
			if( msg.dwParam2 == 1 )
			{
				int nPattern = 2;
				pMover->SetMovePattern( nPattern );
				// Ŭ��� ����
				g_UserMng.AddSetMovePattern( pMover, nPattern, pMover->GetPos(), pMover->GetAngle(), pMover->GetAngleX() );
			}
		} else
		{
			if( m_bReturnToBegin )
				SetStop( xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 
			else
				SetStop( SEC( 5 ) + xRandom( SEC( 1 ) ) ); // 1�� ���� ����� 

			// --------
			// �̺κ��� CAIMonster�� ��ƾ�� �״�� ���� �ְ� ����.
/*			if( m_bLootMove )	// ������ ���ø�忴��.
			{
				CCtrl *pCtrl = prj.GetCtrl( m_idLootItem );		// �׾������� ��������� �����ϱ� �˻���.
				if( IsValidObj(pCtrl) )
				{
					CItem *pItem = (CItem *)pCtrl;
					D3DXVECTOR3 vDist = pCtrl->GetPos() - pMover->GetPos();
					FLOAT fDistSq = D3DXVec3LengthSq( &vDist );
					if( fDistSq < 3.0f * 3.0f )		// �����ؼ� �Ÿ� �˻� �ѹ��� �ؼ� 
					{
						pItem->Lock( theLineFile );	// lock
						if( pItem->IsDelete() )
						{
							pItem->Unlock( theLineFile );	// unlock
							return TRUE;
						}
						CItemElem* pItemElem = (CItemElem *)pItem->m_pItemBase;
						ItemProp *pItemProp = pItem->GetProp();
						BOOL bSuccess = FALSE;
						if( pItemProp->dwItemKind1 == IK1_GOLD )	// ������������ ���̳�?
						{
							//pMover->SetPointParam( DST_GOLD, pMover->GetGold() + pItemElem->m_nItemNum );	// ������.
							pMover->AddGold( pItemElem->m_nItemNum );	// ������.
							bSuccess = TRUE;
						} else
						// not GOLD
						{
							bSuccess = pMover->CreateItem( pItemElem );			// ���� ������ ����.
						}

						if( bSuccess )
						{
							pItem->Delete();		// �κ������� �ִµ� �����ϸ� �ٴ��� �������� ����.
						}
						pItem->Unlock( theLineFile );	// unlock
					}
				}
				m_bLootMove = FALSE;		// this�� ������ �������̶�°� ����.
				m_idLootItem = NULL_ID;
			} */
		}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	return TRUE;
}

BOOL CAIGuard::StateWander( const AIMSG & msg )
	{	return TRUE;	}
BOOL CAIGuard::StateEvade( const AIMSG & msg )
	{	return TRUE;	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////



// ������忡���� ���μ���.
BOOL CAIGuard::MoveProcessRage( const AIMSG & msg )
{
	CAIMonster::MoveProcessRage( msg );		// ������ ���������� �״�� ���.
	return TRUE;
}

BOOL CAIGuard::StateRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	if( IsInvalidObj(pMover) )	return FALSE;

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		if( msg.dwParam2 != NULL_ID )
			m_dwIdTarget = msg.dwParam2;
		m_tmAttack = timeGetTime() + s_tmAttack;
		m_tmAttackDelay = timeGetTime(); // ���� �����̸� �ʱ�ȭ. �׷��� ������ ó�� ������ ������ �԰� ����.
		SetStop( 0 ); // 0���� �����ϸ� ���� ���°� �ٷ� ������ �Ǿ� ������ �����ϰ� �ȴ�. 
		              // ���� ���¿����� ���� ���¸� ���� �ð����� �������־���ϴµ�, �׷��� �Ǹ� ������ ���� �� �ִ�.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT_TARGETCLEAR )		// Ÿ���� Ŭ�����ϰ� ������ ���ư�.
		m_dwIdTarget = NULL_ID;
		SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage( msg );
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( IsMove() && m_bGoTarget == FALSE )
		{
			if( xRandom( 2 ) == 0 )		// 1/2Ȯ���� �����ڸ� ����.
			{
				m_dwIdTarget = msg.dwParam1;
				// �����ڿ��� ��ǥ�� ���� 
				MoveToDst( m_dwIdTarget );
			}
			m_bGoTarget = TRUE;
		}
		MoverProp* pMoverProp = pMover->GetProp();
		int nHPPercent = pMover->GetHitPointPercent( 100 );
		// ���� HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ��������.
		if( nHPPercent < pMoverProp->m_nRunawayHP )
		{
			// ���� ���� ���� 
			SendAIMsg( AIMSG_SETSTATE, STATE_RUNAWAY );
			g_UserMng.AddDlgEmoticon( pMover, DLGEMOT_EVADE );
		}
		// Call HP �ۼ�Ʈ���� ���� HP�ۼ�Ʈ�� ���� ��� ���Ḧ �θ���.
		if( pMoverProp->m_bHelpWho )	// ����AI�� �������.
		{
			if( timeGetTime() - m_tmHelp > pMoverProp->m_tmUnitHelp )	// ���� m_tmHelp�� 0�̹Ƿ� ������ �Ѵ�.
			{
				if( m_bCallHelper == FALSE )
				{
					CallHelper( pMoverProp );
					g_UserMng.AddDlgEmoticon( pMover, DLGEMOT_HELPER );
					m_tmHelp = timeGetTime();
					if( pMoverProp->m_tmUnitHelp == 0 )		// 0�̸� �ѹ��� �θ��� �Ⱥθ�.
						m_bCallHelper = TRUE;
				}
			}
		}
		else
			m_bCallHelper = FALSE;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		MoveToRandom( STATE_RAGE );
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		if( m_dwIdTarget == msg.dwParam1 )
			m_dwIdTarget = NULL_ID;
		TRACE( "TARGET CLEARED\n" );
		SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );

	OnMessage( AIMSG_BEGINMOVE )
		m_bGoTarget = FALSE; 

	OnMessage( AIMSG_ARRIVAL )

	OnMessage( AIMSG_EXIT )	

	EndAIHandler( )
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CAIGuard::MoveProcessRunaway()
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();

	// ������ ���°ų� �׾��� ��� �̵� ó�� ���� ���� 
	if( pMover->IsDie() || (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL))
		return FALSE;

	CCtrl* pTarget = prj.GetCtrl( m_dwIdTarget ); // ��ǥ�� �缳�����ش�.

	// Ÿ���� ��ȿ�ΰ�?
	if( IsValidObj( pTarget ) == FALSE )
	{
		m_dwIdTarget = NULL_ID;
		SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );
	}
	return TRUE;
}

BOOL CAIGuard::StateRunaway( const AIMSG & msg )
{

	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();

	BeginAIHandler( )

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
		// �̰�� �������� �ɼǿ� ���� ���ְ� �ƴ϶�, ��밡 ������ �� ����, ������ ������ �ִ� ��ҿ�
		// �ֱ� ������ ������� ġ�� ����. 
		m_bTargetNoMovePos	= msg.dwParam2;
		// ������ �ݴ� �������� ���� ���� 
		// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
		CMover* pMoverTarget = (CMover*)prj.GetCtrl( m_dwIdTarget ); // ��ǥ�� �缳�����ش�.
		if( IsValidObj(pMoverTarget) )
		{
			DWORD dwAttackRange = pMoverTarget->GetActiveHandItemProp()->dwAttackRange;
			FLOAT fRange = pMoverTarget->GetAttackRange( dwAttackRange ) * 1.7f;
			FLOAT fAngle = pMover->GetAngle();
			D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, fRange );
			MoveToDst( vDst );
//			SendAIMsg( AIMSG_SETPROCESS, TRUE );
			m_bFirstRunaway = TRUE; // ó�� �����ΰ�?
		}
		else
			SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRunaway();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		if( m_bTargetNoMovePos == TRUE && m_bFirstRunaway == FALSE )
			SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		// ������ �ݴ� �������� ���� ���� 
		// 5�� ���� ������ ���� ����. ���� ũ�� Ŭ���� ������� ���·� �����δ�.
		pMover->AddAngle( (float)( 180 + ( 20 - xRandom( 40 ) ) ) );
		// ������ ���� �Ŀ� ���弱�� �ִ� ��ǥ�� ���Ѵ�.
		FLOAT fAngle = pMover->GetAngle();
		D3DXVECTOR3 vDst = pMover->GetPos() + VelocityToVec( fAngle, 5 );
		MoveToDst( vDst );
		// m_bFirstRunaway = FALSE; <-- 04.10.08 ������� �����ϴ� ������ ������.
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DSTDIE ) 
		if( m_dwIdTarget == msg.dwParam1 )
			m_dwIdTarget = NULL_ID;
		SendAIMsg( AIMSG_SETSTATE, STATE_IDLE );

	OnMessage( AIMSG_ARRIVAL )
		m_bFirstRunaway = FALSE;
		//SetStop( xRandom( SEC( 2 ) ) + SEC( 1 ) ); // 2��~3�� ���� ����� 
		MoverProp* pMoverProp = pMover->GetProp();
		if( pMoverProp->m_dwRunawayDelay )
			SetStop( pMover->GetProp()->m_dwRunawayDelay + xRandom( SEC( 1 ) ) ); // + xRandom�� �ణ�� ���� ���̸� �ֱ� ���� ��.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );

	EndAIHandler( )

	return TRUE;
}

