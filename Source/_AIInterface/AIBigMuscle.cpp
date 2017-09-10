#include "stdafx.h"

#include "AIBigMuscle.h"
#include "User.h"
#include "dpcoreclient.h"
#include "defineobj.h"
#include "Party.h"
#include "defineskill.h"
#include "definetext.h"

extern	CUserMng		g_UserMng;
extern	CDPCoreClient	g_DPCoreClient;
extern	CPartyMng		g_PartyMng;

// Event Pattern Type
#define		EPT_READY			0		// �ƹ��͵� ���ϰ� ����.
#define		EPT_MOVING			1		// ����/�̵�
#define		EPT_ATTACKING		2		// ������.
#define		EPT_TRACKING		3		// ������.

// ClockWorks Attack Pattern
#define		CAT_NONE					0
#define		CAT_NORMAL					1		// �⺻����
#define		CAT_NORMAL2					2
#define		CAT_QUAKE_ONE				3		// �Ѽ����� ���� ����ħ
#define		CAT_QUAKEDOUBLE				4		// �μ����� ���� ����ħ - ����

enum
{
	STATE_INIT = 1,
	STATE_APPEAR,		// �����
	STATE_IDLE,			// ������ - �������� �Ϲ� �ʵ忡 ��������ų�� �̰ͺ��� �����ϸ� �ȴ�.
//	STATE_RUNAWAY,		// ��������.
	STATE_RAGE,			// ���ݻ���.
	STATE_SUPER_RAGE	// ���� ȭ�� ����.
};
BEGIN_AISTATE_MAP( CAIBigMuscle, CAIInterface )

	ON_STATE( STATE_INIT   , &CAIBigMuscle::StateInit   )
	ON_STATE( STATE_IDLE   , &CAIBigMuscle::StateIdle   )
	ON_STATE( STATE_RAGE   , &CAIBigMuscle::StateRage   )
	ON_STATE( STATE_SUPER_RAGE   , &CAIBigMuscle::StateRage   )
//	ON_STATE( STATE_RUNAWAY, StateRunaway  )

END_AISTATE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __VM_0820
#ifndef __MEM_TRACE
	#ifdef __VM_0819
	MemPooler<CAIBigMuscle>*	CAIBigMuscle::m_pPool		= new MemPooler<CAIBigMuscle>( 4, "CAIBigMuscle" );
	#else	// __VM_0819
	MemPooler<CAIBigMuscle>*	CAIBigMuscle::m_pPool		= new MemPooler<CAIBigMuscle>( 4 );
	#endif	// __VM_0819
#endif	// __MEM_TRACE
#endif	// __VM_0820

CAIBigMuscle::CAIBigMuscle()
{
	Init();
}
CAIBigMuscle::CAIBigMuscle( CObj* pObj ) : CAIInterface( pObj )
{
	Init();
}

CAIBigMuscle::~CAIBigMuscle()
{
	Destroy();
}

void CAIBigMuscle::Init( void )
{
	m_vPosBegin.x = m_vPosBegin.y = m_vPosBegin.z = 0;
	m_nEvent = EPT_READY;
	m_nAttackType = 0;
	m_tmReattack = m_tmAddReattack = m_tmTrace = m_tmTimeOver = timeGetTime();
	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;
	m_idLastAttacker = NULL_ID;
	m_bDefenseMode = FALSE;
	m_nAppearCnt = 0;

	memset( m_bEventFlag, 0, sizeof(BOOL)*5 );
}

void CAIBigMuscle::Destroy( void )
{
}

void CAIBigMuscle::InitAI()
{
	m_vPosBegin = GetMover()->GetPos();
	PostAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// ���� �����·� �ʱ�ȭ.

#ifdef __INTERNALSERVER
	DEBUGOUT2( "ClockWorks start" );
#endif
}

BOOL CAIBigMuscle::StateInit( const AIMSG & msg )
{
	return TRUE;
}

BOOL CAIBigMuscle::MoveProcessIdle()
{
#ifdef __MOVER_STATE_EFFECT
	CMover* pMover = GetMover();

	if( IsInvalidObj( pMover ) == TRUE && pMover->m_pActMover->IsStateFlag( OBJSTAF_COMBAT ) == TRUE )
	{
		if( pMover->m_pActMover->SendActMsg( OBJMSG_MODE_PEACE ) == TRUE )
		{
			g_UserMng.AddSendActMsg( pMover, OBJMSG_MODE_PEACE );
		}
	}
#endif // __MOVER_STATE_EFFECT

	return TRUE;
}

BOOL CAIBigMuscle::StateIdle( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	
	BeginAIHandler( )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 
////		SendAIMsg( AIMSG_SETPROCESS, TRUE );
		SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );	// ������ IDLE������ ����. �ٷ� ���ݿ� ����.
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessIdle();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		SendAIMsg( AIMSG_SETSTATE, STATE_RAGE, NULL_ID );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );
		
	EndAIHandler( )
			return TRUE;
}

BOOL	CAIBigMuscle::SelectTarget( void )
{
	CMover	*pMover = GetMover();
	CWorld	*pWorld = GetWorld();
	CModelObject *pModel = (CModelObject *)pMover->GetModel();

	FLOAT fRadius = pMover->GetRadiusXZ();		// this�� ������
	FLOAT fRadiusSq = fRadius * fRadius;		// ������Sq����.
	
	CMover *pLastAttacker = prj.GetMover( m_idLastAttacker );
	
	if( pLastAttacker && pLastAttacker->IsDie() )
	{
		m_idLastAttacker = NULL_ID;
		pLastAttacker = NULL;
	}
	
	if( pLastAttacker == NULL )			// LastAttacker�� ���������� Ÿ�� �ٽ� ���� �� �ֵ��� ����.
	{
		m_idLastAttacker = NULL_ID;
	} else
	{		
		D3DXVECTOR3 vDist = pLastAttacker->GetPos() - pMover->GetPos();
		FLOAT fDistSq = D3DXVec3LengthSq( &vDist );		// ��ǥ���������� �Ÿ�.
		if( fDistSq >= fRadiusSq * 10.0f )				// ��Ʈ����Ŀ�� �� �������� 10���̻� ������������
		{
			// Ÿ�� ����
#ifdef __INTERNALSERVER
			CString str;
			str.Format( "LastAtker(%s)�� �ʹ� �־(%5.2f) Ÿ��������", pLastAttacker->GetName(), sqrt(fDistSq) );
			DEBUGOUT2( str, "boss(BigMuscle).txt" );
#endif
			
			m_idLastAttacker = NULL_ID;
			pLastAttacker = NULL;
		}
	}


	m_idTarget = NULL_ID;
	m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;	// �ϴ� �̰� �Ⱦ��°ɷ� ����.

	if( m_idLastAttacker == NULL_ID )		// ���� �� ���������� ����.
	{
		CMover* pTarget = NULL;
		pTarget = ScanTarget( pMover, 40, JOB_ALL );		// 40���� �ݰ泻���� �ƹ��� ����.
		if( pTarget )
		{
			// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
			if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
			{
				m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
				CString str;
				str.Format( "scanTarget:%s", pTarget->GetName() );
				DEBUGOUT2( str, "boss(BigMuscle).txt" );
#endif
				
			}
			else
				return FALSE;		// �� ã������ �� ����. - ������ �ٸ�Ÿ���� ã�ƾ߰���.
		} else
			return FALSE;
	} else
	// �� ���� ������ �ִ�.
	{
		DWORD dwNum = xRandom( 100 );		// 0 ~ 99������ ����.
		DWORD dwAggroRate = 50;
		
		if( IsValidObj( pLastAttacker ) )
		{
			if( pLastAttacker->GetJob() == JOB_MERCENARY )		// ���������� ������ ������ �Ӽ��� ��׷� ���� ����.
				dwAggroRate = 70;
		}
		if( dwNum < dwAggroRate )		
		{
			// dwAggroRate% Ȯ���� ���������� �� ������ ����.
			m_idTarget = m_idLastAttacker;		// �� ������ ������ Ÿ������ ��������.
		} else
		if( dwNum < 75 )
		{
			// 50���� �ݰ泻���� ���� ����� ����.
			CMover *pTarget = ScanTargetStrong( pMover, 50 );		
			if( pTarget )
			{
				// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					CString str;
					str.Format( "Leader:%s %d", pTarget->GetName(), pTarget->GetLevel() );
					DEBUGOUT2( str, "boss(BigMuscle).txt" );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
			} else
				m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
		} else
		if( dwNum < 100 )
		{
			// �������ϴ� ��ø� ������.
			CMover *pTarget = ScanTargetOverHealer( pMover, 50 );		
			if( pTarget )
			{
				// this�� ������ ���Ͱų� || Ÿ���� �������� �ƴҶ��� ����.
				if( pMover->IsFlyingNPC() || pTarget->m_pActMover->IsFly() == FALSE )	
				{
					m_idTarget = pTarget->GetId();
#ifdef __INTERNALSERVER
					CString str;
					str.Format( "OverHealer:%s", pTarget->GetName() );
					DEBUGOUT2( str, "boss(BigMuscle).txt" );
#endif
				}
				else
					m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
			} else
				m_idTarget = m_idLastAttacker;		// Ÿ���� �����ϱⰡ ����ġ ������ ���������� �������� ��������.
		}
#ifdef __INTERNALSERVER
		{
			CMover *pTarget = prj.GetMover( m_idTarget );
			CMover *pMover = prj.GetMover( m_idLastAttacker );
			CString str;
			if( pMover )
			{
				if( pTarget )
					str.Format( "rand:%d LastAttacker:%s SelectTarget:%s", dwNum, pMover->GetName(), pTarget->GetName() );
				else
					str.Format( "rand:%d LastAttacker:%s SelectTarget:%s", dwNum, pMover->GetName(), "����" );
				DEBUGOUT2( str, "boss(BigMuscle).txt" );
			} else
			{
				if( pTarget )
					str.Format( "rand:%d LastAttacker:%s SelectTarget:%s", dwNum, "����.", pTarget->GetName() );
				else
					str.Format( "rand:%d LastAttacker:%s SelectTarget:%s", dwNum, "����.", "����" );
				
				DEBUGOUT2( str, "boss(BigMuscle).txt" );
			}

		}
#endif
	}

	// ������ ���ݴ���� ������.
	// .���� ���������� �� ������ - 50%. - ���� �����Ѵٴ� ������ ��.
	// .������ ���� �������γ�(�������� ���� �ִ³� Ȥ�� ���� ���� ��) - 25%
	// .������ �ϴ� ��� - 25%.
	// .���Ÿ��� ���� �����ִ� ������ �߾ӿ� �ִ³�. - 10% - �ϴ� �̰� ���߿�

	return TRUE;
}

//
//	Rage Processs
//
BOOL CAIBigMuscle::MoveProcessRage()
{
	CMover	*pMover = GetMover();
	CWorld	*pWorld = GetWorld();
	CModelObject *pModel = (CModelObject *)pMover->GetModel();
/*
	if( m_bDefenseMode == FALSE && pMover->GetHitPointPercent() <= 100 )		// 20%���Ϸ� �ǰ� ������
	{
		m_bDefenseMode = TRUE;
		pMover->SetDestParam( DST_ADJDEF_RATE, 800, NULL_CHGPARAM );			// �� 30%����.
#ifdef __INTERNALSERVER
//		Error( "������ ���� �����Ǿ���. ���͸� ġ�ð� ��� ����� �Ͻÿ�" );
#endif
	}
*/
	
	// �̺�Ʈ �޼���
	// �������Ͱ� �������� ���� �Ѵ�.
	CMover * pTarget = NULL;
	
	if( m_bEventFlag[0] == FALSE )
	{
		pTarget = ScanTarget( pMover, 80, JOB_ALL );		// 80���� �ݰ泻���� �ƹ��� ����.

		if( pTarget )
		{
			m_bEventFlag[0] = TRUE;

			g_DPCoreClient.SendCaption( prj.GetText(TID_GAME_BOSS_BIGMUSCLE_MSG_01), pTarget->GetWorld()->GetID() );	
			
			g_UserMng.AddWorldShout( pMover->GetName(), prj.GetText(TID_GAME_BOSS_BIGMUSCLE_MSG_02),
				pTarget->GetPos(), pTarget->GetWorld() );
			g_UserMng.AddWorldShout( pMover->GetName(), prj.GetText(TID_GAME_BOSS_BIGMUSCLE_MSG_03),
					pTarget->GetPos(), pTarget->GetWorld() );
			
			g_UserMng.AddCreateSfxObj( pTarget, XI_SKILL_DROP_DUST_RAIN );
			g_UserMng.AddCommonPlace( pTarget, COMMONPLACE_QUAKE );

			ItemProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess = pMover->GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_BLEEDING, 10, "CAIBigMuscle::MoveProcessRage" );
			if( bSuccess )
				pMover->ApplySkillAround( pMover, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false, FALSE, 255.0f );										
		}
	}


	if( m_nEvent == EPT_READY )
	{
		if( pMover->m_pActMover->m_bGround == 1 && pMover->m_pActMover->GetActionState() != OBJSTA_APPEAR )		// �ٴڿ� ��������..
			pMover->SendActMsg( OBJMSG_APPEAR );		// ��ȿ. �̰� Ŭ�󿡼� �������� �ȳ��ò���...
		if( ++m_nAppearCnt >= PROCESS_COUNT * 5 )		// 
		{
			PostAIMsg( AIMSG_END_APPEAR );		// ����� �ִϸ��̼��� �������� �˸�.
			pMover->m_pActMover->ResetState( OBJSTA_ACTION_ALL );
			m_nAppearCnt = 0;
		}
		// �ƹ��͵� ����.
	} else
	// ������ �ִ�.
	if( m_nEvent == EPT_ATTACKING )
	{
		BOOL bEnd = FALSE;

		if( pMover->m_dwMotion == MTI_STAND || pModel->IsEndFrame()	)
		{
			bEnd = TRUE;		// ���� ������ �����°�. - ����� �̰� �ȳ־ �Ǵµ� ���� ���ݵ����� ���� ��ġ�� ���� ���涧�� ����ؼ� �־�θ� ��Ÿ.
		}
		if( timeGetTime() >= m_tmTimeOver + SEC(15) )				// ���ݽ������� 10���̻���������� �� ����.
			bEnd = TRUE;
		if( bEnd )
		{
			m_nEvent = EPT_MOVING;				// �̵����·� ��ȯ.
			m_tmReattack = timeGetTime();		// ������ Ÿ�� ����;
			m_tmAddReattack = 2000; // xRandom( 2000 );	// x�� ���������� �������� ����� �� �ִ�.
			m_idTarget = NULL_ID;				// Ÿ���� Ŭ����.
		} 

	} else
	// ������ ���� �߰���.
	if( m_nEvent == EPT_TRACKING )
	{
		if( timeGetTime() >= m_tmTrace + 3000 )		// ������ �������� 3�ʰ� ������ ������ ����
		{
			m_nEvent = EPT_MOVING;
			m_idTarget = NULL;			// ���⼭ �̰� Ŭ���� ����� �Ʒ����� SelectTarget()�� �ٽ� �Ѵ� .
			m_vTarget.x = m_vTarget.y = m_vTarget.z = 0;
		}
	} else	
	//------------- �������� �ƴ�. --------------------
	{

		if( timeGetTime() >= m_tmReattack + (2000 - m_tmAddReattack) )		// ������ �ð��� �ƴ°�?  �⺻������ 5�ʰ��ݿ� AddReattack���� �پ�� �� ����.
		{
			FLOAT fRadius = pMover->GetRadiusXZ();		// this�� ������
			FLOAT fRadiusSq = fRadius * fRadius;		// ������Sq����.
			
			if( m_idTarget == NULL_ID &&										// ���ݴ���� �������� �ʾҰ�
				(m_vTarget.x == 0 && m_vTarget.y == 0 && m_vTarget.z == 0) )	// ������ġ�� �������� �ʾҴ�.
			{
				// � ������ �����ұ�....? �� ������.
				if( SelectTarget() == FALSE )
					return FALSE;
			}

			D3DXVECTOR3	vTarget;
			FLOAT	fDistSq = 0;					// ���������� this�� �Ÿ�.
			CMover *pTarget = NULL;
			if( m_idTarget != NULL_ID )		// Ÿ�ٿ������� ������
			{
				pTarget = prj.GetMover( m_idTarget );
				if( IsValidObj(pTarget) )
					vTarget = pTarget->GetPos();		// ������ǥ�� Ÿ�ٹ����� ��ǥ
				else
				{
					m_idTarget = NULL_ID;
					return FALSE;		// Ÿ���� �Žñ� �ϸ� ������.
				}
			} else
			if( m_vTarget.x && m_vTarget.y && m_vTarget.z )		// ���� ��ǥ�� �����Ǿ� ������.
			{
				vTarget = m_vTarget;
			} else
			{
//				Error( "CAIBigMuscle::MoveProcessRage : Ÿ���� ����" );
				return FALSE;
			}

			D3DXVECTOR3 vDist = vTarget - pMover->GetPos();
			fDistSq = D3DXVec3LengthSq( &vDist );		// ��ǥ���������� �Ÿ�.
			FLOAT fArrivalRange = fRadius;			// �󸶳� �����ؾ��ϴ°�? ����Ʈ�� ������ ����.

			if( fDistSq < fRadiusSq * 4.0f )		// �ٰŸ���.
			{
				DWORD dwNum = xRandom( 100 );		// 0 ~ 99������ ����.
				
				if( dwNum < 85 )
				{
					if( xRandom( 2 ) )
						m_nAttackType = CAT_NORMAL;		// �չ߷� ���.
					else
						m_nAttackType = CAT_NORMAL2;

					fArrivalRange = 5.0f;				// ���پ ��ƾ� �Ѵ�.
				}
				else
				{
					m_nAttackType = CAT_QUAKEDOUBLE;	// �μ����� ����ġ�� - ����
					fArrivalRange = fRadius;			// ������ ���̸�ŭ ��������.
				}
			} else
			if( fDistSq < fRadiusSq * 6.0f )		// �������� x�� ���ϴ� ���.
			{								
				DWORD dwNum = xRandom( 100 );		// 0 ~ 99������ ����.
				// ������ 3��Ÿ� �̻��̸� ���Ÿ�.
				if( dwNum < 60 )
				{						
					m_nAttackType = CAT_QUAKE_ONE;		// �ϴÿ��� �� ����߸���
					fArrivalRange = 15.0f;			// xx���� ���� ��������.
				}
				else
				{
					m_idTarget = NULL_ID;
					return FALSE;
				}
			} else
			{
				m_nAttackType = CAT_NORMAL;		// �չ߷� ���.
				fArrivalRange = 10.0f;

				// ��û �ָ� �������ִ³��� ����.
				//m_idTarget = NULL_ID;
				//return FALSE;
			}
						
			m_tmTrace = timeGetTime();		// ���� ������ �ð�.
			m_nEvent = EPT_TRACKING;		// �����߰ݻ��� ��ȯ.
			pMover->CMD_SetMeleeAttack( m_idTarget, fArrivalRange );		// ���ݹݰ��� this�� ������ ũ�������� �����ϴ�.

#ifdef __INTERNALSERVER
			{
				CString strType;
				switch( m_nAttackType )
				{
				case CAT_NORMAL:		strType = "�⺻";	break;
				case CAT_QUAKE_ONE:		strType = "�μճ���ħ-����";	break;
				case CAT_QUAKEDOUBLE:		strType = "�μճ���ħ";	break;
				case CAT_NORMAL2:		strType = "�Ѽ� ����ħ";	break;
				}
				CString str;
				str.Format( "AtkType:%s Dist:%5.2f", strType, sqrt(fDistSq) );
				DEBUGOUT2( str, "boss(BigMuscle).txt" );
			}
#endif
			
				
		} else	// ������ Ÿ�ֿ̹� ����.
		{
			// �����ϱ� ������ ��ġ�� �̵�����.
		}
	}

	return TRUE;
}

//
// 
//
BOOL CAIBigMuscle::StateRage( const AIMSG & msg )
{
	CMover* pMover = GetMover();
	CWorld* pWorld = GetWorld();
	
	BeginAIHandler( )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_INIT ) 

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_PROCESS ) 
		MoveProcessRage();
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DAMAGE ) 
		// ������ ������ ���� �س���� ����ϰ� ����. ���߿� ��¡����.
		m_idLastAttacker = msg.dwParam1;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_DIE ) 
		SendAIMsg( AIMSG_EXIT );
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_COLLISION )
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ARRIVAL )
	{
		OBJMSG	dwMsg = OBJMSG_NONE;
		DWORD	dwItemID = 0;
		MoverProp	*pProp = pMover->GetProp();
		// �߰��Ͽ� �����ϸ� ���õǾ��� ���ݹ���� �����Ų��.
		switch( m_nAttackType )
		{
		case CAT_NORMAL:			dwMsg = OBJMSG_ATK1;	dwItemID = pProp->dwAtk1;	break;
		case CAT_NORMAL2:			dwMsg = OBJMSG_ATK2;	dwItemID = pProp->dwAtk1;	break;
		case CAT_QUAKEDOUBLE:		dwMsg = OBJMSG_ATK3;	dwItemID = pProp->dwAtk3;	break;
		case CAT_QUAKE_ONE:			dwMsg = OBJMSG_ATK4;	dwItemID = pProp->dwAtk2;	break;
		default:
			Error( "CAIBigMuscle::StateRage - AIMSG_ARRIVAL m_nAttackType=%d", m_nAttackType );
		}

		if( dwMsg )
		{
			if( m_idTarget != NULL_ID )
			{
				if( pMover->DoAttackMelee( m_idTarget, dwMsg, dwItemID ) == 1 )
				{
					m_nEvent = EPT_ATTACKING;		// ���ݻ��� ��ȯ.
					m_tmTimeOver = timeGetTime();	// ���ݽ����� Ÿ�ӿ��� ����

					CMover *pTarget = NULL;
					pTarget = prj.GetMover( m_idTarget );

					// �̺�Ʈ �޼���
					// �������Ͱ� �������� ���� �Ѵ�.
					switch( m_nAttackType )
					{
					case CAT_NORMAL:	
						{
						}
						break;
					case CAT_QUAKEDOUBLE:
						{
							// �̺�Ʈ �޼���
							// �������Ͱ� �������� ���� �Ѵ�.
							if( pTarget )
							{
								g_UserMng.AddWorldShout( pMover->GetName(), prj.GetText(TID_GAME_BOSS_BIGMUSCLE_MSG_04),
									pTarget->GetPos(), pTarget->GetWorld() );
							}
						}
						break;
					case CAT_QUAKE_ONE:
						{
							if( pTarget )
							{
								TCHAR szChar[128] = { 0 };
								sprintf( szChar, prj.GetText(TID_GAME_BOSS_BIGMUSCLE_MSG_05), pTarget->GetName() );
								g_UserMng.AddWorldShout( pMover->GetName(), szChar,
									pTarget->GetPos(), pTarget->GetWorld() );
							}
						}
						break;
					}					
				} else
					m_nEvent = EPT_MOVING;			// ������ �����ؼ� �ٽ� �����·� ���ư�.
			}
		} else
			m_nEvent = EPT_MOVING;		// ���������� ����. ������� ����.
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_ATTACK_MELEE )
		switch( m_nAttackType )			// ���õ� ���� ����.
		{
		case CAT_NORMAL:
			// ���� ����Ʈ �������� ���.
			// ���� ����Ʈ ����;
			// ���� ����;
			break;
		case CAT_QUAKEDOUBLE:
			// �̻��� ������Ʈ ����;
			break;
		case CAT_QUAKE_ONE:		// ��� ���� Ÿ��.
			// ���� ������;
			break;
		case CAT_NORMAL2:
			break;			
		default:
			Error( "CAIBigMuscle::StateRage : AIMSG_ATTACK_MELEE %d", m_nAttackType );
			break;
		}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_APPEAR )	
		m_nEvent = EPT_MOVING;		// ������°� ������ ����/�̵����·� ��ȯ.

	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_END_MELEEATTACK )	
		m_nEvent = EPT_MOVING;				// �̵����·� ��ȯ.
		m_tmReattack = timeGetTime();		// ������ Ÿ�� ����;
		m_tmAddReattack = xRandom( 2000 );	// x�� ���������� �������� ����� �� �ִ�.
		m_idTarget = NULL_ID;				// Ÿ���� Ŭ����.
				
		
	///////////////////////////////////////////////////////////////////////////////////////////////////
	OnMessage( AIMSG_EXIT )	
//		SendAIMsg( AIMSG_SETPROCESS, FALSE );
	
	EndAIHandler( )

	return TRUE;
}
