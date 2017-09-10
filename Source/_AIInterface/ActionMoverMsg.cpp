#include "stdafx.h"
#include "defineObj.h"
#include "defineSkill.h"
#include "ActionMover.h"
#include "ErrorCode.h"
#include "party.h"

#ifdef __CLIENT
	#include "DPClient.h"
#else
	#include "user.h"
	#include "dpcoreclient.h"
	#include "dpdatabaseclient.h"
	#include "..\_AIInterface\AIInterface.h"
#endif	

#if __VER >= 12 // __NEW_ITEMCREATEMON_SERVER
#include "CreateMonster.h"
#endif // __NEW_ITEMCREATEMON_SERVER



extern	CPartyMng	g_PartyMng;

#ifdef __CLIENT
	extern	CDPClient	g_DPlay;
#else
	extern	CUserMng			g_UserMng;
	extern	CDPDatabaseClient	g_dpDBClient;
	extern	CDPCoreClient		g_DPCoreClient;
#endif 

// ���� ���¸� �����ϰ� ���ο� �������� ���� ����.
// ������ ����ȭ �Ҷ��ܿ� ������� ����.
void	CActionMover::ForcedSet( D3DXVECTOR3 &vDelta, DWORD dwState, DWORD dwStateFlag, int nMotionEx )
{
	if( ( GetStateFlag() & OBJSTAF_FLY ) == 0 && ( dwStateFlag & OBJSTAF_FLY ) )
		return;
	if( ( GetStateFlag() & OBJSTAF_FLY ) && ( dwStateFlag & OBJSTAF_FLY ) == 0 )
		return;
	if( ( GetStateFlag() & OBJSTAF_SIT ) == 0 && ( dwStateFlag & OBJSTAF_SIT ) )
		return;
	if( ( GetStateFlag() & OBJSTAF_SIT ) && ( dwStateFlag & OBJSTAF_SIT ) == 0 )
		return;

	m_vDelta = vDelta;
	__ForceSetState( dwState );	// ���� �̿ܿ� ���Լ� ������� ����.
			
	ClearStateFlag();
	AddStateFlag( dwStateFlag );

	if( (GetStateFlag() & OBJSTAF_SIT) && ((dwState & OBJSTA_MOVE_ALL) != OBJSTA_SIT) )	// IsSit()�ε�
		Error( "ForcedSet : dwState=0x%08x, dwStateFlag=0x%08x", GetState(), GetStateFlag() );
	else if( (GetStateFlag() & OBJSTAF_SIT) == 0 && ((dwState & OBJSTA_MOVE_ALL) == OBJSTA_SIT) )	// IsSit()�ƴѵ� SIT�̸�
		Error( "ForcedSet : dwState=0x%08x, dwStateFlag=0x%08x", GetState(), GetStateFlag() );

	m_nMotionEx = nMotionEx;
	DefaultSet();
}

void	CActionMover::ForcedSet2( D3DXVECTOR3 &vDelta, float fAccPower, float fTurnAngle, DWORD dwState, DWORD dwStateFlag, int nMotionEx )
{
	if( ( GetStateFlag() & OBJSTAF_FLY ) == 0 && ( dwStateFlag & OBJSTAF_FLY ) )
		return;
	if( ( GetStateFlag() & OBJSTAF_FLY ) && ( dwStateFlag & OBJSTAF_FLY ) == 0 )
		return;

	m_vDelta = vDelta;
	m_fAccPower	= fAccPower;
	m_fTurnAngle	= fTurnAngle;
	__ForceSetState( dwState );
	ClearStateFlag();
	AddStateFlag( dwStateFlag );
	m_nMotionEx = nMotionEx;
	DefaultSet();
	if( (GetStateFlag() & OBJSTAF_SIT) && (dwState != OBJSTA_SIT))	// IsSit()�ε�
		Error( "ForcedSet2 : dwState=0x%08x, dwStateFlag=0x%08x", GetState(), GetStateFlag() );
	else if( (GetStateFlag() & OBJSTAF_SIT) == 0 && (dwState == OBJSTA_SIT) )	// IsSit()�ƴѵ� SIT�̸�
		Error( "ForcedSet2 : dwState=0x%08x, dwStateFlag=0x%08x", GetState(), GetStateFlag() );
}

void CActionMover::HalfForcedSet( D3DXVECTOR3 & vd, float fAccPower, float fTurnAngle )
{
	m_vDelta	 = vd;
	m_fAccPower	 = fAccPower;
	m_fTurnAngle = fTurnAngle;
	DefaultSet();
}

void	CActionMover::DefaultSet( void )
{
	m_pMover->m_fCrrSpd	= 0.0f;
}

//
//	Action Message Process
//	�׼� �޽����� �޾� ó���Ѵ�.
//	� ������ �߻��ϴ� ������ ���� ó���� ���.
//	����ȭ�� ���ؼ� �̹� �����Ǿ� �ִ� ���¸� �ߺ� ó�� ���� ����
//
int		CActionMover::ProcessActMsg1( CMover* pMover,  OBJMSG dwMsg, int nParam1, int nParam2, int nParam3, int nParam4, int nParam5 )
{
	CModelObject* pModel = (CModelObject*)pMover->m_pModel;

	switch( dwMsg )
	{
	// ��ȭ��� ���ڸ��� ���־��!
	case OBJMSG_STAND:
#ifdef __Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) || (GetMoveState() == OBJSTA_LMOVE) || (GetMoveState() == OBJSTA_RMOVE) )	// ��/�������϶� ���ڸ��� �����.
#else //__Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) )	// ��/�������϶� ���ڸ��� �����.
#endif //__Y_INTERFACE_VER3
		{
			ResetState( OBJSTA_MOVE_ALL );		
			m_vDelta.x = m_vDelta.z = 0;
			if( pMover->IsFlyingNPC() )
			{
				m_vDelta.y = 0;
				pMover->SetAngleX(0);
			}
		}

		if( GetMoveState() == OBJSTA_STAND )	return 0;
		if( GetMoveState() == OBJSTA_PICKUP )	return 0;
		if( IsActJump() )		return -1;
		if( IsActAttack() )		return -2;
		if( IsActDamage() )		return -3;
		if( IsDie() )			return -4;
		if( IsAction() )		return 0;
		SetMoveState( OBJSTA_STAND );
		pMover->SetMotion( MTI_STAND );
		RemoveStateFlag( OBJSTAF_ETC );
		break;
	case OBJMSG_STOP:		
	case OBJMSG_ASTOP:
	#ifdef __Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) || (GetMoveState() == OBJSTA_LMOVE) || (GetMoveState() == OBJSTA_RMOVE) )	// ��/�������϶� ���ڸ��� �����.
	#else //__Y_INTERFACE_VER3
		if( (GetMoveState() == OBJSTA_FMOVE) || (GetMoveState() == OBJSTA_BMOVE) )	// ��/�������϶� ���ڸ��� �����.
	#endif //__Y_INTERFACE_VER3
		{
			ResetState( OBJSTA_MOVE_ALL );		
			m_vDelta.x = m_vDelta.z = 0;
			if( pMover->IsFlyingNPC() )
			{
				m_vDelta.y = 0;
				pMover->SetAngleX(0);
			}
		}
		pMover->ClearDest();
		return 0;	// ControlGround���� Ű�Է¾����� STOP�� �θ��µ� �ű⼭ ���ϰ��� ���߱� ���� �̷��� ����. 
		break;
	case OBJMSG_SITDOWN:
		if( GetStateFlag() & OBJSTAF_SIT )	return 0;		// �̹� �������� ���� - �÷��׷� �˻����� ���� state�� �����˻�����.

		if( nParam3 == 0 )
		{
			if( IsActJump() )	return 0;
			if( IsActAttack() )	return 0;
			if( IsActDamage() )	return 0;
			if( IsAction() )	return 0;
		#ifdef __CLIENT
			if( pMover->IsActiveMover() && (pMover->m_dwReqFlag & REQ_USESKILL) )	return 0;		// �����κ��� useskill������ ���������� �׼��ؼ� �ȵ�.
		#endif	// __CLIENT
		}
		AddStateFlag( OBJSTAF_SIT );		
		SendActMsg( OBJMSG_STOP );
		pMover->SetMotion( MTI_SIT, ANILOOP_CONT );		// �ɱ� ��ǽ���
		SetMoveState( OBJSTA_SIT );
		break;
	case OBJMSG_STANDUP:
		if( (GetStateFlag() & OBJSTAF_SIT) == 0 )	return 0;	

		if( nParam3 == 0 )
		{
			if( IsActJump() )	return 0;
			if( IsActAttack() )	return 0;
			if( IsActDamage() )	return 0;
			if( IsAction() )	return 0;
#ifdef __CLIENT
			if( pMover->IsActiveMover() && (pMover->m_dwReqFlag & REQ_USESKILL) )	return 0;		// �����κ��� useskill������ ���������� �׼��ؼ� �ȵ�.
#endif	// __CLIENT
		}
		SetMoveState( OBJSTA_SIT );
		AddStateFlag( OBJSTAF_SIT );		 
		pMover->SetMotion( MTI_GETUP, ANILOOP_CONT );
		break;
	case OBJMSG_PICKUP:
		if( IsSit() )	return 0;
		if( IsAction() )	return 0;
		if( IsActJump() )	return 0;
		if( IsActAttack() )	return 0;
		if( IsActDamage() )	return 0;
#ifdef __CLIENT
		if( pMover->IsActiveMover() )
		{
			if( pMover->m_dwReqFlag & REQ_USESKILL )	return 0;		// �����κ��� useskill������ ���������� �׼��ؼ� �ȵ�.
			if( pMover->m_dwReqFlag & REQ_MOTION )		return 0;	// �����κ��� ��� ������ ���������� �׼� ����.
		}
#endif	// __CLIENT
		if( GetMoveState() == OBJSTA_PICKUP )	return 0;	// �̹� ���� ������ �߰��� ������� �ʰ� .
		SetMoveState( OBJSTA_PICKUP );
		pMover->SetMotion( MTI_PICKUP, ANILOOP_1PLAY, MOP_FIXED );
		break;

	case OBJMSG_COLLECT:
		
		//if( IsSit() )		return 0;
		//if( IsAction() )	return 0;
		//if( IsActJump() )	return 0;
		//if( IsActAttack() )	return 0;
		//if( IsActDamage() )	return 0;
		//SetState( OBJSTA_ACTION_ALL, OBJSTA_COLLECT );
		//pMover->SetMotion( MTI_COLLECT, ANILOOP_LOOP, MOP_FIXED );
		//pMover->RemoveInvisible();
		
		// ˬ	// ä��
		ClearState();
		ResetState( OBJSTA_ACTION_ALL );
		SetState( OBJSTA_ACTION_ALL, OBJSTA_COLLECT );
		pMover->m_dwFlag |= MVRF_NOACTION;
		pMover->SetMotion( MTI_COLLECT, ANILOOP_LOOP, MOP_FIXED );
		pMover->RemoveInvisible();
		break;

	case OBJMSG_STUN:
		SetState( OBJSTA_ACTION_ALL, OBJSTA_STUN );		// ������ � ���¿����� �ɸ� �� ������ ���´� ������ ��������.
		pMover->m_dwFlag |= MVRF_NOACTION;				// �׼� ���� ���·� ��ȯ.
		pMover->m_wStunCnt = (short)nParam1;			// �󸶵����̳� ���ϻ��°� �Ǵ���.

		if( GetState() == OBJSTA_STAND )
			pMover->SetMotion( MTI_GROGGY, ANILOOP_LOOP, MOP_FIXED );		// �ƹ��͵� ���ϴ� ���ĵ� ���¸� ���� ��� ����.
		break;
		
	case OBJMSG_FALL:
		if( IsActJump() )	return 0;

		// �����̸� ���⼭ ����.
		if( pMover->m_dwMode & MATCHLESS_MODE )		
			return 0;
		
		pMover->m_dwFlag |= MVRF_NOACTION;				// �׼� ���� ���·� ��ȯ.
		
		if( GetState() != OBJSTA_DMG_FLY )
		{
			SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_FLY );	
			pMover->SetMotion( MTI_DMGFLY, ANILOOP_LOOP, MOP_FIXED );
		}
		break;
		
	// �����ض�!
	case OBJMSG_TURNMOVE:
		if( GetMoveState() == OBJSTA_FMOVE && (int)pMover->GetAngle() == nParam1 )	return 0;	// ���� �����̸� ������ ������ �޽��� ��ȿ 
		if( pMover->m_dwFlag & MVRF_NOMOVE )	
			return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		
		if( GetState() & OBJSTA_DMG_FLY_ALL )	// ���󰡴� �������̸� ���
			return 0;
		
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		pMover->SetAngle( (float)nParam1 );
		if( pMover->IsFlyingNPC() )
			pMover->SetAngleX( (float)nParam2 );
		SendActMsg( OBJMSG_STOP_TURN );
		SendActMsg( OBJMSG_FORWARD );
		break;
	case OBJMSG_TURNMOVE2:
		if( GetMoveState() == OBJSTA_FMOVE && (int)pMover->GetAngle() == nParam1 )	return 0;	// ���� �����̸� ������ ������ �޽��� ��ȿ 
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		if( IsAction() )	return 0;

		if( GetState() & OBJSTA_DMG_FLY_ALL )	// ���󰡴� �������̸� ���
			return 0;

		if( IsActJump() )	return -1;	// �����ų� ���� ���¸� �޽��� ��ȿ ó��
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		pMover->SetAngle( (float)nParam1 );
		if( pMover->IsFlyingNPC() )
			pMover->SetAngleX( (float)nParam2 );
		SendActMsg( OBJMSG_STOP_TURN );
		SendActMsg( OBJMSG_BACKWARD );
		break;

#ifdef __Y_INTERFACE_VER3
	case OBJMSG_LFORWARD:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else
			if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
		if( GetMoveState() == OBJSTA_LMOVE )	return 0;	// �̹� �������¸� �߰��� ó�� ���� ����
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetMoveState( OBJSTA_LMOVE );
		if( IsStateFlag( OBJSTAF_WALK ) )	// �ȴ¸��
		{
			pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
		} 
		else
		{
			if( pMover->SetMotion( MTI_RUN ) )
				pModel->SetMotionBlending( TRUE );
		}
		break;
	case OBJMSG_RFORWARD:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else
			if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
		if( GetMoveState() == OBJSTA_RMOVE )	return 0;	// �̹� �������¸� �߰��� ó�� ���� ����
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetMoveState( OBJSTA_RMOVE );
		if( IsStateFlag( OBJSTAF_WALK ) )	// �ȴ¸��
		{
			pMover->SetMotion( MTI_WALK );		// �������� �ȱ�
		} 
		else
		{
			if( pMover->SetMotion( MTI_RUN ) )
				pModel->SetMotionBlending( TRUE );
		}
		break;
#endif //__Y_INTERFACE_VER3

	case OBJMSG_FORWARD:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
		if( GetMoveState() == OBJSTA_FMOVE )	return 0;	// �̹� �������¸� �߰��� ó�� ���� ����
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetMoveState( OBJSTA_FMOVE );
		if( IsStateFlag( OBJSTAF_WALK ) )	// �ȴ¸��
		{
			pMover->SetMotion( MTI_WALK );	
		} 
		else
		{
			if( pMover->SetMotion( MTI_RUN ) )
				pModel->SetMotionBlending( TRUE );
		}
		pMover->OnActFMove();
		break;
		
	case OBJMSG_STOP_RUN:
	#ifdef __CLIENT
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		if( GetMoveState() == OBJSTA_STOP_RUN )	return 0;	// �̹� �������¸� �߰��� ó�� ���� ����
		if( IsSit() )		return 0;						// ���� 0���� �ص� �ǳ�?.
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;	// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetMoveState( OBJSTA_STOP_RUN );
	#endif
		break;

	
	// �����ض�!
	case OBJMSG_BACKWARD:
		if( IsActJump() )	return -1;	// if jump, x
		if( IsActAttack() )		return -2;	// ���ݵ������̸� ��ҵ�.
		if( IsActDamage() )		return -3;
		if( GetMoveState() == OBJSTA_BMOVE )	return 0;
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )	return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetMoveState( OBJSTA_BMOVE );
		pMover->SetMotion( MTI_WALK );		
		break;
	// ��/�� �� �ض�!
	case OBJMSG_LTURN:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
		if( GetTurnState() == OBJSTA_LTURN )	return 0;
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )	return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetTurnState( OBJSTA_LTURN );
		break;
	case OBJMSG_RTURN:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		if( GetTurnState() == OBJSTA_RTURN )	return 0;
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )	return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetTurnState( OBJSTA_RTURN );
		break;
	case OBJMSG_STOP_TURN:
		if( GetTurnState() == 0 )		return 0;		// �̹� �� ���°� ���ٸ� ó�� ����
		SetTurnState( 0 );		// ���� ����
		break;

	// ��ġ�� ���ø�. ������� ��츸 ���.
	case OBJMSG_LOOKUP:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		if( GetTurnState() == OBJSTA_LOOKUP )	return 0;
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )	return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetLookState( OBJSTA_LOOKUP );
		break;
	case OBJMSG_LOOKDOWN:
		if( IsActJump() )	return -1;
		if( IsActAttack() )		return -2;
		if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ����̸� �ǰ� ���¸� Ŭ���� ��Ų��.
			ResetState( OBJSTA_DMG_ALL );
		else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
			return -3;
		if( GetTurnState() == OBJSTA_LOOKDOWN )	return 0;
		if( IsSit() )		return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )	return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

		SetLookState( OBJSTA_LOOKDOWN );
		break;
	case OBJMSG_STOP_LOOK:
		if( GetLookState() == 0 )		return 0;
		SetLookState( 0 );		// �� ����
		break;
		// ����
	case OBJMSG_JUMP:
		if( IsActJump() )		return 0;
		if( IsActAttack() )		return -2;
		if( IsActDamage() )		return -3;
		if( IsSit() )			return 0;		// ���� 0���� �ص� �ǳ�?.
		if( IsAction() )		return 0;
		if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

#ifdef __CLIENT
		if( pMover->IsActiveMover() )
		{
			if( pMover->m_dwReqFlag & REQ_USESKILL )	return 0;	// �����κ��� useskill������ ���������� �����ؼ� �ȵ�.
			if( pMover->m_dwReqFlag & REQ_MOTION )		return 0;	// �����κ��� ��� ������ ���������� ���� ����.
		}
#endif	// __CLIENT
		SendActMsg( OBJMSG_STOP_TURN );
		SetJumpState( OBJSTA_SJUMP1 );
		if( (GetState() & OBJSTA_COLLECT)== 0 ) pMover->SetMotion( MTI_JUMP1, ANILOOP_1PLAY );
		pModel->SetMotionBlending( FALSE );
		break;
	// �ϵ����
	case OBJMSG_ATK_MAGIC1:	// ���� 1
		{
			if( IsAction() )	return 0;
			if( IsSit() )		// �ɾ��ִ»��¿����� ����
				ResetState( OBJSTA_MOVE_ALL );
			if( GetState() & OBJSTA_ATK_ALL )	return 0;		// �̹� ���ݵ����� �ϰ� ������ ���.
			if( pMover->m_dwFlag & MVRF_NOATTACK )	return -1;		// ���ݱ��� ���¸� �� ����.
			if( pMover->IsMode( NO_ATTACK_MODE ) ) return -1;
			CMover* pHitObj		= prj.GetMover( nParam1 );
			if( IsInvalidObj( pHitObj ) )	return -1;
			if( pHitObj->IsDie() )	return -1;
#ifdef __WORLDSERVER
			if( !g_eLocal.GetState( EVE_STEAL ) )
			{
				if( pMover->IsSteal( pHitObj ) )	// pHitObj�� ��ƿ�Ϸ��ϸ� �̰����� ����.
					return -1;
			}
#endif
			if( IsActDamage() )
				ResetState( OBJSTA_DMG_ALL );
			if( IsStateFlag( OBJSTAF_COMBAT ) == FALSE )
				SendActMsg( OBJMSG_MODE_COMBAT );
		
			SendActMsg( OBJMSG_STOP_TURN );
			SendActMsg( OBJMSG_STAND );
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_MAGIC1 );
			pMover->SetMotion( MTI_ATK1, ANILOOP_1PLAY );		// �ϵ嵿���� �����Ƿ� ���ݵ������� �����.
			pHitObj->SetJJim( pMover );
			pMover->m_nAtkCnt = 1;		// ī��Ʈ ����.
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
		}
		break;
		// ����������
	case OBJMSG_ATK_RANGE1:	// ���� 1
		{
			if( IsAction() )	return 0;
			if( pMover->m_dwFlag & MVRF_NOATTACK )	return -1;		// ���ݱ��� ���¸� �� ����.
			if( pMover->IsMode( NO_ATTACK_MODE ) ) return -1;
			if( IsSit() )		// �ɾ��ִ»��¿����� ����
				ResetState( OBJSTA_MOVE_ALL );
			if( GetState() & OBJSTA_ATK_ALL )	return 0;		// �̹� ���ݵ����� �ϰ� ������ ���.
			CMover* pHitObj		= prj.GetMover( nParam1 );
			if( IsValidObj( (CObj*)pHitObj ) == FALSE )	
				return -1;
			if( pHitObj->IsDie() )
				return -1;

			m_idTarget = (DWORD)nParam1;		// ���� Ÿ��.

#ifdef __WORLDSERVER
			if( !g_eLocal.GetState( EVE_STEAL ) )
			{
				if( pMover->IsSteal( pHitObj ) )	// pHitObj�� ��ƿ�Ϸ��ϸ� �̰����� ����.
					return -1;
			}
#endif
			if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ���ݸ���̸� �ǰ� ���¸� Ŭ���� ��Ų��.
				ResetState( OBJSTA_DMG_ALL );
			else if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -1;
			if( IsStateFlag( OBJSTAF_COMBAT ) == FALSE )
				SendActMsg( OBJMSG_MODE_COMBAT );
			
			int nUseMotion = nParam2;
//			SendActMsg( OBJMSG_STOP_TURN );
//			SendActMsg( OBJMSG_STAND );
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_RANGE1 );
			pMover->SetMotion( nUseMotion, ANILOOP_1PLAY );		// �ϵ嵿���� �����Ƿ� ���ݵ������� �����.
			pHitObj->SetJJim( pMover );			// ������ ���۵Ǹ� Ÿ�ٿ��ٰ� ���� ���ߴٴ°� ǥ��.
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
			pMover->m_nAtkCnt = 1;		// ī��Ʈ ����.
		}
		break;
		// ���� - ���ϰ� -2�� Ŭ��� ���� ���� ����� ���� �����Ѵ�.
	case OBJMSG_ATK1:	// ���� 1
	case OBJMSG_ATK2:	// ���� 2
	case OBJMSG_ATK3:	// ���� 3	
	case OBJMSG_ATK4:	// ���� 4
		{
			if( IsAction() )
				return 0;
			if( pMover->m_dwFlag & MVRF_NOATTACK )
				return -2;		// ���ݱ��� ���¸� �� ����.
			if( pMover->IsMode( NO_ATTACK_MODE ) )
				return -2;

#ifdef __CLIENT
			if( pMover->IsActiveMover() )
#endif
			{
				// �⺻�� ������ �˻�( �ϵ�� )
				CItemElem* pItemElem = pMover->GetWeaponItem();
				if( pItemElem && (pItemElem->GetProp()->dwItemKind3 == IK3_WAND || pItemElem->GetProp()->dwItemKind3 == IK3_BOW ) )
					return -2;
			}
				
			CMover* pHitObj		= prj.GetMover( nParam1 );
			if( IsValidObj( (CObj*)pHitObj ) == FALSE )
				return( -1 );

#ifdef __XUZHU
			if( pMover->IsPlayer() )
			{
				int a = 0;
			}
#endif // xuzhu
			m_idTarget = (DWORD)nParam1;		// ���� Ÿ��.
#ifdef __WORLDSERVER
			if( !g_eLocal.GetState( EVE_STEAL ) )
			{
				if( pMover->IsSteal( pHitObj ) )	// pHitObj�� ��ƿ�Ϸ��ϸ� �̰����� ����.
					return -2;
			}
#endif
			if( pHitObj->m_pActMover->IsDie() )	return( -1 );
			if( GetState() & OBJSTA_ATK_ALL )	return( 0 );	// �̹� �������̸� ����
			if( IsSit() )		// �ɾ��ִ»��¿����� ����
				ResetState( OBJSTA_MOVE_ALL );
			if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ���ݸ���̸� �ǰ� ���¸� Ŭ���� ��Ų��.
				ResetState( OBJSTA_DMG_ALL );
			else
			if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
			if( IsStateFlag( OBJSTAF_COMBAT ) == FALSE )
				SendActMsg( OBJMSG_MODE_COMBAT );
			SendActMsg( OBJMSG_STOP_TURN );	// �����Ҷ� ���� ������Ŵ
			SendActMsg( OBJMSG_STAND );
			

			DWORD dwOption = MOP_SWDFORCE | MOP_NO_TRANS;
			
#ifdef __XSLOW1018
			int nMin, nTemp;
			pMover->GetHitMinMax( &nMin, &nTemp );
			if( dwMsg == OBJMSG_ATK3 || dwMsg == OBJMSG_ATK4 )
				if( pMover->GetLevel() >= 12 && pHitObj->GetHitPoint() < (int)(nMin * 1.5f) )
					dwOption |= MOP_HITSLOW;
#endif // xSlow1018
			if( pMover->IsPlayer() )
				pMover->m_fAniSpeed = pMover->GetAttackSpeed();
			
			switch( dwMsg )
			{
				case OBJMSG_ATK1:
					SetState( OBJSTA_ATK_ALL, OBJSTA_ATK1 );
					pMover->SetMotion( MTI_ATK1, ANILOOP_1PLAY, dwOption );		// ���� �޽��� �Է½� ���� ����
					break;
				case OBJMSG_ATK2:	
					SetState( OBJSTA_ATK_ALL, OBJSTA_ATK2 );
					pMover->SetMotion( MTI_ATK2, ANILOOP_1PLAY, dwOption );		// ���� �޽��� �Է½� ���� ����
					break;
				case OBJMSG_ATK3:	
					SetState( OBJSTA_ATK_ALL, OBJSTA_ATK3 );
					pMover->SetMotion( MTI_ATK3, ANILOOP_1PLAY, dwOption );		// ���� �޽��� �Է½� ���� ����
					break;
				case OBJMSG_ATK4:	
					SetState( OBJSTA_ATK_ALL, OBJSTA_ATK4 );
					pMover->SetMotion( MTI_ATK4, ANILOOP_1PLAY, dwOption );		// ���� �޽��� �Է½� ���� ����
					break;
			}
			pMover->SetAngle( GetDegree( pHitObj->GetPos(), m_pMover->GetPos() ) );
			pHitObj->SetJJim( pMover );

			pMover->m_nAtkCnt = 1;		// ī��Ʈ ����.
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
			m_objidHit	= (OBJID)nParam1;	// Ÿ���� ���̵�.	

			m_dwAtkFlags	= (DWORD)LOWORD( (DWORD)nParam3 );
#ifdef __WORLDSERVER
			if( m_dwAtkFlags == 0 ) 
			{
				if( pMover->GetAttackResult( pHitObj, dwOption ) )		// ���� �������� ��´�.
					m_dwAtkFlags = AF_GENERIC;
				else
					m_dwAtkFlags = AF_MISS;
			}
#else
			m_dwAtkFlags = AF_GENERIC;		// ������ �� �̻��ѵ�. Ŭ�󿡼��� ���������� ���� �����Ƿ� ������ ��Ÿ�������� ����.
#endif
			
			return( (int)m_dwAtkFlags );
			break;
		}	
	// ���͵��� Ư������.
	case OBJMSG_SP_ATK1:
	case OBJMSG_SP_ATK2:
		{
			CMover* pHitObj		= prj.GetMover( nParam1 );
			//nParam2;		// Ư�����ݿ� ����� ����. ������ �����̹Ƿ� ����������� ���´�.
			int		nUseMotion	= nParam3;		// ����� ���.
			if( IsValidObj( (CObj*)pHitObj ) == FALSE )	return( -1 );
			if( pHitObj->m_pActMover->IsDie() )	return( -1 );
			if( GetState() & OBJSTA_ATK_ALL )	return( 0 );	// �̹� �������̸� ����
			if( pMover->m_dwFlag & MVRF_NOATTACK )	return 0;		// ���ݱ��� ���¸� �� ����.
			if( GetDmgState() == OBJSTA_DMG )	// �ǰ� �� ���ݸ���̸� �ǰ� ���¸� Ŭ���� ��Ų��.
				ResetState( OBJSTA_DMG_ALL );
			else
			if( GetState() & OBJSTA_DMG_ALL )	// �׿� �ǰݵ����� �� ���
				return -3;
			if( IsStateFlag( OBJSTAF_COMBAT ) == FALSE )
				SendActMsg( OBJMSG_MODE_COMBAT );
			SendActMsg( OBJMSG_STOP_TURN );	// �����Ҷ� ���� ������Ŵ
			SendActMsg( OBJMSG_STAND );

			DWORD dwOption = MOP_SWDFORCE /*| MOP_NO_TRANS*/;	// ���ʹϱ� ���ݵ��۵� ���Ʈ������ �ǰ� �غô�.
			SetState( OBJSTA_ATK_ALL, OBJSTA_SP_ATK1 );
			pMover->SetMotion( nUseMotion, ANILOOP_1PLAY, dwOption );		// ���� �޽��� �Է½� ���� ����
			pMover->SetAngle( GetDegree( pHitObj->GetPos(), m_pMover->GetPos() ) );
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
			return 1;
		}
		break;

	// ������� ���� ������ų��
	case OBJMSG_MELEESKILL:
		{
			if( IsAction() )	return -2;	
			if( pMover->m_dwFlag & MVRF_NOATTACK )	return -2;		// ���ݱ��� ���¸� �� ����.
			CMover* pHitObj		= prj.GetMover( nParam2 );
			if( IsValidObj( pHitObj ) == FALSE )	return -2;
#ifdef __WORLDSERVER
			if( GetState() & OBJSTA_ATK_RANGE1 )
				ResetState( OBJSTA_ATK_RANGE1 );
			if( GetState() & OBJSTA_ATK_ALL )	return -2;	// �̹� �������̸� ����. �������� ������ �ʰ� ����.
			pMover->m_dwMotion = -1;	// ������ ���� �����ϰ� �����ϱ� ���� ��� Ŭ���� ��Ű�� ��ų�� ��������.
			ClearState();				// ���� Ŭ�����ϰ� �ٽ� ����,.
#else
			// �̸����� �غ��� ��Ƽ�� ������ �ƴϰ� �������� ��ȣ���� �ٷ� �����ϴ°� ������ ���ּ� �̷��� ��.
			pMover->m_dwMotion = -1;	
			ClearState();				// ���� Ŭ�����ϰ� �ٽ� ����,.
#endif
			SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_MELEESKILL );			// ����������ų���� ����.
			DWORD dwMotion = (DWORD)nParam1;
			int	nLoop = ANILOOP_1PLAY;

			int	nSkill = pMover->GetActParam( 0 );
			if( nSkill == SI_BLD_DOUBLESW_SILENTSTRIKE || nSkill == SI_ACR_YOYO_COUNTER )
				nLoop = ANILOOP_CONT;

#ifdef __CLIENT			
			if( pMover->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
				dwMotion = MTI_ATK1;
#endif

			pMover->SetMotion( dwMotion, nLoop, MOP_SWDFORCE | MOP_NO_TRANS | MOP_FIXED );		// �ش� ���۾ִ� ����.
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
			
			m_nMotionHitCount = 0;
			
			break;
		}
	// ��������� ������ų��.
	case OBJMSG_MAGICSKILL:
	case OBJMSG_MAGICCASTING:
		{
			if( IsAction() )	return -2;
			if( pMover->m_dwFlag & MVRF_NOATTACK )	return -2;		// ���ݱ��� ���¸� �� ����.
			CMover* pHitObj		= prj.GetMover( nParam2 );
			if( IsValidObj( pHitObj ) == FALSE )	return -2;
		#ifdef __WORLDSERVER
			if( GetState() & OBJSTA_ATK_ALL )	return -2;	// �̹� �������̸� ����
		#endif  

			pMover->m_dwMotion = -1;	
			ClearState();				// ���� Ŭ�����ϰ� �ٽ� ����.

			m_nCastingTime	= nParam3 * 4;	// ProcessAction�� ����/Ŭ�� ȣ�� ȸ�� �����ϹǷ�, SEC1 ����� �������� �ʴ�.

#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
			m_dwCastingEndTick = (DWORD)( GetTickCount() + nParam3 * 66.66F );
			m_nCastingTick	= (int)( nParam3 * 66.66F );
			m_nCastingSKillID = nParam4;
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans
			if( m_nCastingTime > 0 )	// ĳ���� Ÿ���� ��������...
			{
				m_nCount	= 0;	// ��ü ĳ���� Ÿ���� ����1+����2�� ���̾�� �ϹǷ� ���⼭ ī���͸� �ʱ�ȭ �Ѵ�.
				SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_CASTING1 );			// ĳ���ú��� ���� -> �߻���.
			}
			else
				SetState( OBJSTA_ATK_ALL, OBJSTA_ATK_MAGICSKILL );			// �߻��Ǹ�.

		#ifdef __CLIENT
			if( dwMsg == OBJMSG_MAGICSKILL && pMover->HasBuffByIk3( IK3_TEXT_DISGUISE ) )
				nParam1 = MTI_ATK1;
		#endif

			pMover->SetMotion( nParam1, ANILOOP_1PLAY, MOP_SWDFORCE | MOP_NO_TRANS | MOP_FIXED );		// �ش� ���۾ִ� ����.
			pMover->OnAttackStart( pHitObj, dwMsg );			// ���ݽ��� �ڵ鷯.
			m_nMotionHitCount = 0;
			break;
		}
			

	//-------------------- �¾Ҵ�.
	case OBJMSG_DAMAGE:
	case OBJMSG_DAMAGE_FORCE:	// �����ݿ� �¾Ҵ�. ���ư�.
		{
			CMover* pAttacker = PreProcessDamage( pMover, (OBJID)nParam2, nParam4, nParam5 );
			if( pAttacker == NULL )
				return 0;

			if( dwMsg == OBJMSG_DAMAGE_FORCE )			// ���������� ��������
				if( pMover->CanFlyByAttack() == FALSE )	// ���ư��� ���ϴ� ���¸�.
					dwMsg = OBJMSG_DAMAGE;				// �Ϲ� �������� �ٲ�.

			// ���� ������ �������� ������.
			int nDamage = _ProcessMsgDmg( dwMsg, pAttacker, (DWORD)nParam1, nParam3, nParam4, nParam5 );	
#if __VER >= 10	// __METEONYKER_0608
#ifdef __WORLDSERVER
#if __VER >= 12 // __NEW_ITEMCREATEMON_SERVER
			if( pAttacker->IsPlayer() && CCreateMonster::GetInstance()->IsAttackAble( static_cast<CUser*>(pAttacker), pMover ) )
#endif // __NEW_ITEMCREATEMON_SERVER
#endif // __WORLDSERVER
				pMover->PostAIMsg( AIMSG_DAMAGE, pAttacker->GetId(), nDamage );
#endif	// __METEONYKER_0608
			return nDamage;
		}
		break;

	// HP�� 0�� �� ���� ȣ��
	case OBJMSG_DIE:
	{
		if( IsSit() )						// �ɾ��ִ»��¿����� ����
			ResetState( OBJSTA_MOVE_ALL );
		
		ClearState();						// ������ �ٸ� ���� �ϰ� �ִ��� �� Ŭ���� ���ѹ�����.
		m_vDelta.x = m_vDelta.z = 0;		// �׾����� ��Ÿ�� Ŭ����.

		pMover->RemoveInvisible();
#ifdef __WORLDSERVER
		pMover->PostAIMsg( AIMSG_DIE, nParam2 );
#endif
		CMover *pAttacker = (CMover *)nParam2;	
		if( pAttacker )
		{
			if( pMover->IsPlayer() && pAttacker->IsPlayer() )	// ������� ���γ��� �÷��̾��
			{
#ifdef __WORLDSERVER
				if( pMover->m_nDuel == 1 && pMover->m_idDuelOther == pAttacker->GetId() )	// ���� ������̾�����
				{
					pMover->ClearDuel();
					pAttacker->ClearDuel();
					((CUser*)pMover)->AddSetDuel( pMover );
					((CUser*)pAttacker)->AddSetDuel( pAttacker );
				}
				else if( pMover->m_nDuel == 2 && pMover->m_idDuelParty == pAttacker->m_idparty )
				{
					pMover->m_nDuelState	= 300;		
					CMover *pMember;
					CParty* pParty	= g_PartyMng.GetParty( pMover->m_idparty );

					if( pParty )
					{
						for( int i = 0; i < pParty->m_nSizeofMember; i ++ )
						{
							pMember = (CMover *)g_UserMng.GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
							if( IsValidObj( pMember ) )
								((CUser*)pMember)->AddSetDuel( pMover );
						}
					}

					CParty* pParty2		= g_PartyMng.GetParty( pMover->m_idDuelParty );
					if( pParty2 )
					{
						for( int i = 0; i < pParty2->m_nSizeofMember; i ++ )
						{
							pMember = (CMover *)g_UserMng.GetUserByPlayerID( pParty2->m_aMember[i].m_uPlayerId );
							if( IsValidObj( pMember ) )
								((CUser*)pMember)->AddSetDuel( pMover );
						}
					}
				}
#endif	// __WORLDSERVER
			}

			if( pAttacker->IsNPC() )	// �����ڰ� ��.
			{	// ���������� ����Ŀ�� ���ϰ��� �ױ����� Ŭ��������ش�.
				if( pAttacker->m_idAttacker == pMover->GetId() )	// �����Ѹ��� ģ����� ���ϰ��� ��ƿüũ ����.
				{
					pAttacker->m_idAttacker = NULL_ID;
					pAttacker->m_idTargeter = NULL_ID;
				}
			}
		}
		
		switch( nParam1 )
		{
#ifdef __BS_DEATH_ACTION
		case OBJMSG_DAMAGE:
		case OBJMSG_DAMAGE_FORCE:
		{
			int rAngle = 0;
			int rPower = 0;
			int nCase = rand() % 6;
			if( nCase < 3 )
			{
				rAngle = -2;
				rPower = 10 + rand() % 50;
			}
			else if( nCase < 4 )
			{
				rAngle = 10;
				rPower = 10 + rand() % 50;
			}
			else if( nCase < 5 )
			{
				rAngle = 50;
				rPower = 10 + rand() % 50;
			}
			else
			{
				rAngle = rand() % 30;
				rPower = rand() % 200;
			}
			
			if( OBJMSG_DAMAGE_FORCE == nParam1 )
				rPower += 50;
		
			CMover *pAttacker = (CMover *)nParam2;
			SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_FLY );
			pMover->SetMotion( MTI_DMGFLY, ANILOOP_CONT );
			if( pAttacker )
			{
				float faddPower = rPower * 0.01f;
				float faddAngle = (float)( rAngle );

				DoDamageFly( pAttacker->GetAngle(), 100.0f + faddAngle, 0.05f + faddPower );	// ����Ŀ�� ���������� ��������.
			}
		}
		break;
#else
		case OBJMSG_DAMAGE_FORCE:	// �� �������� �׾��ٸ� ���ư��� �ױ�
			{
			CMover *pAttacker = (CMover *)nParam2;
			SetState( OBJSTA_DMG_ALL, OBJSTA_DMG_FLY );
			pMover->SetMotion( MTI_DMGFLY, ANILOOP_CONT );
			if( pAttacker )
				DoDamageFly( pAttacker->GetAngle(), 145.0f, 0.25f );	// ����Ŀ�� ���������� ��������.
			}
			break;
		case OBJMSG_DAMAGE:		// �� �Ϲ� �������� �׾��ٸ� �׳� ������ �ױ�
#endif
		default:	
			SetState( OBJSTA_DMG_ALL, OBJSTA_DEAD );
			pMover->SetMotion( MTI_DIE1, ANILOOP_CONT );
			break;

		}

		if( !pMover->IsPlayer() )	// ������ ���.
		{
		#ifdef __WORLDSERVER			
			m_nDeadCnt	= 60*3;	// ����Ʈ - 3�� �Ŀ� ������.
			MoverProp *pProp = pMover->GetProp();
			if( pProp )
			{
				/*
				if( pProp->dwSourceMaterial != NULL_ID || pProp->dwClass == RANK_MATERIAL )	// �ڿ� ���̸�..
				{
					if( pProp->dwHoldingTime == NULL_ID )
						Error( "%s �� MoverProp::dwHoldingTime ���� -1", pMover->GetName() );
					m_nDeadCnt = (int)(60.0f * (pProp->dwHoldingTime / 1000.0f));
					if( pProp->dwHoldingTime < 10000 )		// 10�� �����϶�
						Error( "dwHoldingTime : %d %s", pProp->dwHoldingTime, pMover->GetName() );
					pMover->m_nCollectOwnCnt = PROCESS_COUNT * 40;
					CMover *pAttacker = (CMover *)nParam2;	
					if( pAttacker )
						pMover->m_idCollecter = pAttacker->GetId();		// �����ڰ� �����ڰ� �ȴ�.
					
				}
				{
					if( pProp->dwSourceMaterial == NULL_ID && pProp->dwClass == RANK_MATERIAL )	// �ڿ����ε� �ڿ����� ���°��
						Error( "OBJMSG_DIE : %s %d", pMover->GetName(), pProp->dwSourceMaterial );
					if( pProp->dwSourceMaterial != NULL_ID && pProp->dwClass != RANK_MATERIAL )	// �ڿ����� �ƴѵ� �ڿ����� �ִ°��.
						Error( "OBJMSG_DIE 2 : %s %d", pMover->GetName(), pProp->dwSourceMaterial );
				}
				*/
			}
		#endif // WorldServer
		}
		break;
	}
		
	case OBJMSG_APPEAR:
		SetState( OBJSTA_ACTION_ALL, OBJSTA_APPEAR );
		pMover->SetMotion( MTI_APPEAR2, ANILOOP_1PLAY, MOP_FIXED | MOP_NO_TRANS );
		break;
	//---- �����ȯ ------------------------
	case OBJMSG_MODE_COMBAT:
		if( GetStateFlag() & OBJSTAF_COMBAT )	return 0;		// �̹� �������� ����
		if( IsActJump() )	return 0;
		if( IsActAttack() )	return 0;
		if( IsActDamage() )	return 0;
		if( IsSit() )		return 0;
		if( IsAction() )	return 0;
		AddStateFlag( OBJSTAF_COMBAT );	// �������� ���� �ƴϰ� �������� �ٲ��ⶫ�� ���� ����� �߻���Ű�� �ʴ´�.
		pMover->m_dwMotion = -1;	// ���� ����� �϶�� �ѰŶ� ����� �ȹٲ�Ƿ� �̷���...
		pMover->SetMotion( MTI_STAND );
		break;
	case OBJMSG_MODE_PEACE:
		if( (GetStateFlag() & OBJSTAF_COMBAT) == 0 )	return 0;
		if( IsActJump() )	return 0;
		if( IsActAttack() )	return 0;
		if( IsActDamage() )	return 0;
		if( IsSit() )		return 0;
		if( IsAction() )	return 0;
		RemoveStateFlag( OBJSTAF_COMBAT );
		pMover->m_dwMotion = -1;
		pMover->SetMotion( MTI_STAND );
		break;
	case OBJMSG_MODE_WALK:
		if( IsSit() )		return 0;
		if( IsAction() )	return 0;
		if( GetStateFlag() & OBJSTAF_WALK )	return 0;		// �̹� �ȱ���� ����
		AddStateFlag( OBJSTAF_WALK );
		break;
	case OBJMSG_MODE_RUN:
		if( IsSit() )		return 0;
		if( IsAction() )	return 0;
		if( (GetStateFlag() & OBJSTAF_WALK) == 0 )	return 0;		// 
		RemoveStateFlag( OBJSTAF_WALK );
		break;
	case OBJMSG_MODE_FLY:
	{
		if( nParam3 == 0 )
		{
			if( IsSit() )		return 0;
			if( IsAction() )	return 0;
			if( IsActAttack() )	return 0;
			if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
			if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

			{
				CWorld* pWorld	= pMover->GetWorld();
				if( pWorld )
				{
					int nAttr = pMover->GetWorld()->GetHeightAttribute( pMover->GetPos().x, pMover->GetPos().z );		// �̵��� ��ġ�� �Ӽ� ����.
					if( nAttr == HATTR_NOFLY )		// ���� ���� �������� Ÿ���� �ϸ� ��ź��.
						return 0;
				}
			}
		}
		AddStateFlag( OBJSTAF_FLY );
		ItemProp *pItemProp = prj.GetItemProp( nParam1 );	// ���ڷ� ������Ƽ.
		if( pItemProp )
		{
			if( pMover->m_nFuel == -1 )		// �ʱⰪ�̸�
				pMover->m_nFuel = (int)pItemProp->dwFFuelReMax;	// ���ڷ� �ִ� ���ᷮ�� ����.
			// -1�϶��� �����ؾ��� ���Ḧ ���� ���ٰ� ���ڷ縦 �ٲ㵵 ���� ���� ���� �ʴ´�.
		}
#ifndef __JEFF_9_20
#ifdef __WORLDSERVER
		if( pMover->IsPlayer() )
		{
			((CUser*)pMover)->SetFlyTimeOn();
		}
#endif // __WORLDSERVER
#endif	// __JEFF_9_20
		ClearState();
#ifdef __CLIENT
		if( m_pMover == CMover::GetActiveMover() )
		{
			m_pMover->GetWorld()->SetObjFocus( NULL );	// ���������� ����Ÿ������ Ŭ�������ش�.
			g_Neuz.m_camera.Unlock();
			g_WndMng.m_pWndTaskBar->OnCancelSkill();	// ���������� ��ų�� ��� ���.
		}
#endif

		CModel* pModel = prj.m_modelMng.LoadModel( D3DDEVICE, OT_ITEM, (DWORD) nParam1 );
#if __VER >= 14 // __WING_ITEM
		CModelObject* pModelObject = (CModelObject*)pModel;
		if( pModelObject->m_pBone )
		{
			CString strMotion = pModelObject->GetMotionFileName( _T("stand") );
			assert( strMotion != _T("") );
			pModelObject->LoadMotion( strMotion );
		}
#endif // __WING_ITEM
		m_pMover->SetRide( pModel, (DWORD) nParam1 );
		break;
	}
	case OBJMSG_MOTION:		// �ܼ� ��� �÷���
		{
			if( IsSit() )		return 0;
			if( IsAction() )	return 0;
			if( IsActJump() )	return 0;
			if( IsActAttack() )	return 0;
			if( IsActDamage() )	return 0;
			if( pMover->m_dwFlag & MVRF_NOMOVE )	return 0;		// �̵����� ���¸� �� ����.
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_LOOT)			return 0;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

			AddStateFlag( OBJSTAF_ETC );		// �ܼ� ��� �÷��� ����
			m_nMotionEx = nParam1;				// ��� �ε���
			int nPlayMode = nParam2;			// ���� ���.
			if( m_nMotionEx != -1 )
				pMover->SetMotion( m_nMotionEx, nPlayMode, MOP_FIXED/* | MOP_SWDFORCE*/ );

			pMover->RemoveInvisible();
		}
		break;
	// �Ϲ� �׼�.
	case OBJMSG_RESURRECTION:		// ��Ȱ.
		ClearState();
		SetState( OBJSTA_DMG_ALL, OBJSTA_RESURRECTION );
		pMover->SetMotion( MTI_ASS_RESURRECTION, ANILOOP_1PLAY, MOP_FIXED );
		break;


#if	defined(_DEBUG) && defined(__CLIENT)
	case OBJMSG_TEMP:
		{
			for( int i=0; i < 1; i ++ )
			{

				CMover* pObj = new CMover;
				D3DXVECTOR3 vPos = pMover->GetPos();
				pObj->SetPos( vPos );
				pObj->SetIndex( g_Neuz.m_pd3dDevice, MI_AIBATT1, TRUE );
				pObj->SetMotion( 0 );
				g_WorldMng.Get()->AddObj( pObj, TRUE ); 
			}
		}
		break;
		
	case OBJMSG_TEMP2:	// ������ ���ݵ��۸� ����.
		if( CMover::GetActiveMover()->IsAuthHigher( AUTH_GAMEMASTER ) )	// ���� �����϶�
		{
			CObj *pObj = pMover->GetWorld()->GetObjFocus();
			g_DPlay.SendCorrReq( pObj );
			g_DPlay.SendError( FE_GENERAL, 0 );
		}
		break;
	
	case OBJMSG_TEMP3:
		{
		#ifdef __XUZHU			
			g_DPlay.SendError( 0, 0 );
		#endif
		}
		break;
#endif // defined(_DEBUG) && defined(__CLIENT)
		
	}
	return 1;
}


/*

  p0 + tD0 = -e0

  t = -( p0 + e0 ) / d0

void	Test( RECT r, Point p, Point.d )
{
	Point t;	// ������

	tx = (p0 + 
	
}

*/

// return: attacker�� ������ 
CMover* CActionMover::PreProcessDamage( CMover* pMover, OBJID idAttacker, BOOL bTarget, int nReflect )
{
	if( pMover->m_dwMode & MATCHLESS_MODE )		// �����̸� ���⼭ ����.	
		return NULL;
#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
	if( pMover->GetAdjParam( DST_CHRSTATE ) & CHS_SETSTONE)
		return NULL;
#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans

	CCtrl* pCtrl = prj.GetCtrl( idAttacker );	// ������ 
	if( IsValidObj( pCtrl ) == FALSE )
		return NULL;

	CMover* pAttacker = NULL;
	if( pCtrl->GetType() == OT_MOVER )
		pAttacker = (CMover*)pCtrl; 
	else
		return NULL;							

	if( pMover->IsNPC() )						// �´³��� NPC��
	{
		MoverProp *pProp = pMover->GetProp();
		if( pProp && pProp->bKillable != 1 )	// ���̱Ⱑ �Ұ��� �ѳ��̸� ���⼭ ����.
			return NULL;
	}

#ifdef __WORLDSERVER
	HITTYPE ht = pAttacker->GetHitType( pMover, bTarget, nReflect );
	if( ht == HITTYPE_FAIL )
		return 0;
#if __VER < 10	// __METEONYKER_0608
	pMover->PostAIMsg( AIMSG_DAMAGE, idAttacker ); 
#endif	// __METEONYKER_0608

#endif

	pAttacker->RemoveInvisible();
	return pAttacker;
}