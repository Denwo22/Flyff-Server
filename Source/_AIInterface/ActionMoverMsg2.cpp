#include "stdafx.h"
#include "ActionMover.h"

#ifdef __CLIENT
#include "DPClient.h"
extern	CDPClient	g_DPlay;
#endif

#ifdef __WORLDSERVER
#include "user.h"
#include "dpcoreclient.h"
#include "dpdatabaseclient.h"
#include "..\_AIInterface\AIInterface.h"

extern	CUserMng			g_UserMng;
extern	CDPDatabaseClient	g_dpDBClient;
extern	CDPCoreClient		g_DPCoreClient;
#endif	// __WORLDSERVER




//
//	Action Message Process
//	�׼� �޽����� �޾� ó���Ѵ�.
//	� ������ �߻��ϴ� ������ ���� ó���� ���.
//	����ȭ�� ���ؼ� �̹� �����Ǿ� �ִ� ���¸� �ߺ� ó�� ���� ����
//	���ڷ� �����
//
int		CActionMover::ProcessActMsg2(  CMover* pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4, int nParam5 )
{
	ItemProp* pItemProp = prj.GetItemProp( m_pMover->m_dwRideItemIdx );

	switch( dwMsg )
	{
	// ��ȭ��� ���ڷ� ���ڸ� ���
	case OBJMSG_STAND:
		{
		#ifdef __Y_INTERFACE_VER3
			if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) || (GetMoveState() == OBJSTA_LMOVE) || (GetMoveState() == OBJSTA_RMOVE) )	// ��/�������϶� ���ڸ��� �����.
		#else //__Y_INTERFACE_VER3
			if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) )	// ��/�������϶� ���ڸ��� �����.
		#endif //__Y_INTERFACE_VER3
			{
				ResetState( OBJSTA_MOVE_ALL );		
			}
			if( GetMoveState() == OBJSTA_STAND )	return 0;	// �̹� �������̸� ���
			if( IsActAttack() )		return -2;	// ���� ���ۻ��¸� ���
			if( IsActTurn() )		return 0;
			if( !pItemProp )
				Error( "ItemProp is NULL in ProcessActMsg2() %d", m_pMover->m_dwRideItemIdx );
			SetMoveState( OBJSTA_STAND );	// ���¼���
			BOOL ret;
			if( pItemProp )
				ret = pMover->SetMotion( pItemProp->dwUseMotion + MTA_FSTAND1, ANILOOP_LOOP, MOP_FIXED );		// �ִϸ��̼� ����
			if( ret == TRUE )
				if( pMover->m_pRide )	pMover->m_pRide->m_fFrameCurrent	= 0;
			break;
		}
	case OBJMSG_STOP:		
	case OBJMSG_ASTOP:
	#ifdef __Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) || (GetMoveState() == OBJSTA_LMOVE) || (GetMoveState() == OBJSTA_RMOVE) )	// ��/�������϶� ���ڸ��� �����.
	#else //__Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) )	// ��/�������϶� ���ڸ��� �����.
	#endif //__Y_INTERFACE_VER3
		{
			pMover->ClearDest();				// ��ǥ ��ǥ Ŭ����.
			SendActMsg( OBJMSG_ACC_STOP );		// ���� ����
			ResetState( OBJSTA_MOVE_ALL );		
		}
		break;
		// �����ض�!
	case OBJMSG_FORWARD:
		if( GetMoveState() == OBJSTA_FMOVE )	return 0;// �̹� �������¸� �߰��� ó�� ���� ����
		SetMoveState( OBJSTA_FMOVE );		// 
		if( IsActTurn() )	break;
		if( GetState() & OBJSTA_ATK_ALL )	
			return 1;
		if( !pItemProp )
			Error( "ItemProp is NULL in ProcessActMsg2()\n" );
		pMover->SetMotion( pItemProp->dwUseMotion + MTA_FRUNNING1, ANILOOP_LOOP, MOP_FIXED );	// �ϴ��� �� ��� �������� ����
		break;
	// ��/�� �� �ض�! 
	case OBJMSG_LTURN:
		{
		FLOAT fTurnAngle = (FLOAT) nParam1 / 100.0f;
		if( GetTurnState() == OBJSTA_LTURN && m_fTurnAngle == fTurnAngle )	return 0;
		m_fTurnAngle = fTurnAngle;
		SetTurnState( OBJSTA_LTURN );
		if( GetState() & OBJSTA_ATK_ALL )	return 0;
		if( !pItemProp )
			Error( "ItemProp is NULL in ProcessActMsg2()\n" );
		if( fTurnAngle > 1.0f )
			pMover->SetMotion( pItemProp->dwUseMotion + MTA_FLTURN1, ANILOOP_LOOP, MOP_FIXED );
		}
		break;
	case OBJMSG_RTURN:
		{
		FLOAT fTurnAngle = (FLOAT) nParam1 / 100.0f;
		if( GetTurnState() == OBJSTA_RTURN && m_fTurnAngle == fTurnAngle )	return 0;
		m_fTurnAngle = fTurnAngle;
		SetTurnState( OBJSTA_RTURN );
		if( GetState() & OBJSTA_ATK_ALL )	return 0;	// �����߿� ȸ����� ���� ����.
		if( !pItemProp )
			Error( "ItemProp is NULL in ProcessActMsg2()\n" );
		if( fTurnAngle > 1.0f )
			pMover->SetMotion( pItemProp->dwUseMotion + MTA_FRTURN1, ANILOOP_LOOP, MOP_FIXED );
		}
		break;
	case OBJMSG_STOP_TURN:
		if( GetTurnState() == 0 )		return 0;		// �̹� �� ���°� ���ٸ� ó�� ����
		m_fTurnAngle = 0.0f;
		SetTurnState( 0 );		// ���� ����
		if( GetState() & OBJSTA_ATK_ALL )	return 0;
		if( !pItemProp )
			Error( "ItemProp is NULL in ProcessActMsg2()\n" );
		if( GetMoveState() == OBJSTA_FMOVE )
			pMover->SetMotion( pItemProp->dwUseMotion + MTA_FRUNNING1, ANILOOP_LOOP, MOP_FIXED );
		else
			pMover->SetMotion( pItemProp->dwUseMotion + MTA_FSTAND1, ANILOOP_LOOP, MOP_FIXED );
		break;
	// ��� ���Ʒ��� ���
	case OBJMSG_LOOKUP:
		if( GetLookState() == OBJSTA_LOOKUP )	return 0;
		SetTurnState( OBJSTA_LOOKUP );
		break;
	case OBJMSG_LOOKDOWN:
		if( GetLookState() == OBJSTA_LOOKDOWN )	return 0;
		SetLookState( OBJSTA_LOOKDOWN );
		break;
	case OBJMSG_STOP_LOOK:
		if( GetLookState() == 0 )		return 0;
		SetLookState( 0 );		// �� ����
		break;

	// ����
	case OBJMSG_ATK1:	// ���� 1
		{
			if( (GetState() & OBJSTA_ATK_ALL) == OBJSTA_ATK1 )	// �̹� �������̸� ����
				return 0;
			m_objidHit	= (OBJID)nParam1;	// Ÿ���� ���̵�.
			CMover* pHitObj	= prj.GetMover( m_objidHit );
			if( IsInvalidObj( pHitObj ) || pHitObj->IsDie() )	// Ÿ���� �Žñ��ϰų� �׾����� ���
				return -2;

#ifdef __CLIENT				
			if( pMover->IsActiveMover() && pMover->IsRangeObj( pHitObj, 3.0f ) == FALSE )	return 0;	// x���� �̳��� ���;� �ֵθ�.
#endif
#ifdef __WORLDSERVER
			if( !g_eLocal.GetState( EVE_STEAL ) )
			{
				if( pMover->IsSteal( pHitObj ) )	// pHitObj�� ��ƿ�Ϸ��ϸ� �̰����� ����.
					return -2;
			}
#endif
			
			AddStateFlag( OBJSTAF_COMBAT );	// �������� ��ȯ.
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK1 );
			if( pItemProp )
				pMover->SetMotion( pItemProp->dwUseMotion + MTA_FATK1, ANILOOP_1PLAY, MOP_NO_TRANS | MOP_SWDFORCE | MOP_FIXED );		// ���� �޽��� �Է½� ���� ����
			else
				Error( "ItemProp is NULL in ProcessActMsg2()\n" );
			pHitObj->SetJJim( pMover );
			pMover->RemoveInvisible();
			
#ifdef __CLIENT
			if( pMover->IsActiveMover() )
				g_DPlay.SendMeleeAttack2( OBJMSG_ATK1, m_objidHit, 0, 0 );
#endif
		}
		
		break;
	case OBJMSG_ATK_MAGIC1:	// ���� 1
		{
			if( IsAction() )	return 0;
			if( GetState() & OBJSTA_ATK_ALL )	return 0;		// �̹� ���ݵ����� �ϰ� ������ ���.
			if( !pItemProp )
				Error( "ItemProp is NULL in ProcessActMsg2()\n" );
			CMover* pHitObj		= prj.GetMover( nParam1 );
			if( IsInvalidObj( pHitObj ) )	return -1;
			
#ifdef __WORLDSERVER
			if( !g_eLocal.GetState( EVE_STEAL ) )
			{
				if( pMover->IsSteal( pHitObj ) )	// pHitObj�� ��ƿ�Ϸ��ϸ� �̰����� ����.
					return -2;
			}
#endif
			if( IsActDamage() )
				ResetState( OBJSTA_DMG_ALL );
			AddStateFlag( OBJSTAF_COMBAT );	// �������� ��ȯ.
			
			SendActMsg( OBJMSG_STOP_TURN );
			SendActMsg( OBJMSG_STAND );
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_MAGIC1 );
			if( pItemProp )
				pMover->SetMotion( pItemProp->dwUseMotion + MTA_FATK1, ANILOOP_1PLAY, MOP_NO_TRANS | MOP_SWDFORCE | MOP_FIXED );		// 
			pHitObj->SetJJim( pMover );
			pMover->RemoveInvisible();
			
		}
		break;
		

	// ����
	case OBJMSG_JUMP:
		break;
	case OBJMSG_DAMAGE:
		{
			CMover* pAttacker = PreProcessDamage( pMover, (OBJID)nParam2, nParam4, nParam5 );
			if( pAttacker == NULL )
				return 0;

			int nDamage = _ProcessMsgDmg( dwMsg, pAttacker, (DWORD)nParam1, nParam3, nParam4, nParam5 );
#if __VER >= 10	// __METEONYKER_0608
			pMover->PostAIMsg( AIMSG_DAMAGE, pAttacker->GetId(), nDamage );
#endif	// __METEONYKER_0608
			pAttacker->RemoveInvisible();
			return nDamage;
		}
		break;
	case OBJMSG_DIE:
	#ifdef __WORLDSERVER
		pMover->PostAIMsg( AIMSG_DIE, nParam2 );
	#endif
		SendActMsg( OBJMSG_STAND );	// ������ ���ڸ��� ����
		ResetState( OBJSTA_TURN_ALL );	// ������ �� ���ֶ�.
		SetState( OBJSTA_DMG_ALL, OBJSTA_DEAD );
		
		m_vDelta.x = m_vDelta.z = 0;		// �׾����� ��Ÿ�� Ŭ����.
		{
			CMover *pAttacker = (CMover *)nParam2;	
			if( pAttacker )
			{
				if( pMover->IsPlayer() && pAttacker->IsPlayer() )	// ������� ���γ��� �÷��̾��
				{
					if( pMover->m_nDuel == 1 && pMover->m_idDuelOther == pAttacker->GetId() )	// ���� ������̾�����
					{
						pMover->m_nDuel		= 0;	// ��������� ������ ����� ����
						pAttacker->m_nDuel	= 0;

						TRACE( "m_nDuel		= 0		// OBJMSG_DIE	// ProcessActMsg2\n" );

						pMover->m_idDuelOther = NULL_ID;
						pAttacker->m_idDuelOther = NULL_ID;
					}
				}
			}
		}
		
		pMover->SetMotion( MTI_DIE1, ANILOOP_CONT );
		if( !pMover->IsPlayer() ) 
		{
		#ifdef __WORLDSERVER
			m_nDeadCnt	= 60*3;	// 5 sec
		#endif // WorldServer
		}
		break;

	case OBJMSG_ACC_START:
// ���࿬�� 
//		if( pMover->m_nFuel <= 0 )
//			return 0;
		AddStateFlag( OBJSTAF_ACC );
		break;
	case OBJMSG_ACC_STOP:
		RemoveStateFlag( OBJSTAF_ACC );
		break;

	case OBJMSG_MODE_GROUND:
		if( nParam3 == 0 )
		{
			CWorld* pWorld	= pMover->GetWorld();
			if( pWorld )
			{
				int nAttr = pMover->GetWorld()->GetHeightAttribute( pMover->GetPos().x, pMover->GetPos().z );		// �̵��� ��ġ�� �Ӽ� ����.
				if( nAttr == HATTR_NOWALK )		// �ȱ� ���� �������� ���������ϸ� ��������.
					return 0;
			}
		}
		RemoveStateFlag( OBJSTAF_FLY );
		RemoveStateFlag( OBJSTAF_ACC );		// ���� ����.
		ClearState();
#ifdef __CLIENT
		if( pMover->IsActiveMover() && g_WndMng.m_pWndWorld )
		{
			g_WndMng.m_pWndWorld->SetMouseMode( 0 );		// ������ �����Ǹ� FPS�������
			g_WndMng.m_pWndWorld->m_fRollAng = 0;
		}
#endif		

#ifndef __JEFF_9_20
#ifdef __WORLDSERVER
		if( pMover->IsPlayer() )
		{
			((CUser*)pMover)->SetFlyTimeOff();
		}
#endif // __WORLDSERVER
#endif	// __JEFF_9_20
		pMover->SetAngleX(0);
		#ifdef __CLIENT
		if( m_pMover == CMover::GetActiveMover() )
			g_Neuz.m_camera.Lock();
		#endif
		m_pMover->SetRide( NULL );
		break;
	case OBJMSG_MODE_TURBO_ON:		// �ͺ���� ����.
		if( GetStateFlag() & OBJSTAF_TURBO )		return 0;		// �̹� ������
		if( pMover->m_tmAccFuel <= 0 )	return 0;				// ���ӿ�������� �ͺ� ����.
		AddStateFlag( OBJSTAF_TURBO );
		break;
	case OBJMSG_MODE_TURBO_OFF:		// �ͺ���� ����.
		if( (GetStateFlag() & OBJSTAF_TURBO) == 0 )		return 0;		// �̹� ����
		RemoveStateFlag( OBJSTAF_TURBO );
		break;


	case OBJMSG_TURNMOVE:
	{
		if( IsActAttack() )	return -2;
		if( IsActDamage() )	return -3;
		SendActMsg( OBJMSG_STOP_TURN );
		SendActMsg( OBJMSG_ACC_START );
		pMover->SetAngle( (float)nParam1 );
		pMover->SetAngleX( (float)nParam2 );

		{
			float fAng	= D3DXToRadian( nParam1 );
			float fAngX		= D3DXToRadian( nParam2 );
			int nRemnant	= pMover->m_uRemnantCorrFrm;
			if( nRemnant == 0 )
			{
				Error( "OBJMSG_TURNMOVE" );
				return 0;
			}
			float f		= 2.0f * m_fDistance / ( 0.10f * nRemnant * nRemnant );
			float fDist	= cosf( fAngX ) * f;
			m_vDelta.x		= sinf( fAng ) * fDist;
			m_vDelta.z		= -cosf( fAng ) * fDist;
			m_vDelta.y		= -sinf( fAngX ) * ( f * cosf( fAngX ) );
		}
		break;
	}
	case OBJMSG_TEMP2:	// ������ ���ݵ��۸� ����.
#ifdef __CLIENT
		if( CMover::GetActiveMover()->IsAuthHigher( AUTH_GAMEMASTER ) )	// ���� �����϶�
		{
			CObj *pObj = pMover->GetWorld()->GetObjFocus();
			g_DPlay.SendCorrReq( pObj );
		}
#endif
		break;
		
	}
	return 1;
}

