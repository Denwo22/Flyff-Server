#include "stdafx.h"
#include "AIInterface.h"
#include "DefineSkill.h"

#ifndef __CLIENT
#include "User.h"
#endif	// __CLIENT


BEGIN_AISTATE_MAP( CAIInterface, CAIInterface )
END_AISTATE_MAP()

CAIInterface::CAIInterface()
{
	m_pObj = NULL;
	m_pStateFunc = NULL;
#if __VER >= 9	//__AI_0509
	m_uParty	= 0;
#endif	// __AI_0509
}
CAIInterface::CAIInterface( CObj* pObj ) 
{ 
	m_pObj = pObj;
	m_pStateFunc = NULL;
#if __VER >= 9	//__AI_0509
	m_uParty	= 0;
#endif	// __AI_0509
}

CAIInterface::~CAIInterface()
{
}



FUNCAISTATE CAIInterface::GetAIStateFunc( DWORD dwState)
{
	const STATEMAP* lpCurrentStateMap = GetStateMap();
	const STATEMAP_ENTRIES* lpEntries = lpCurrentStateMap->lpStateMapEntries;
	// �Ϻ� ��ü���� ��ġ�ϴ� �Ž����� ã�´�. 
	while( lpCurrentStateMap->lpBaseStateMap->lpStateMapEntries != lpEntries )
	{
		int i = 0;
		while(  lpEntries[ i ].pStateFunc )
		{
			if( lpEntries[ i ].dwAIStateId == dwState )
			{
				return lpEntries[ i ].pStateFunc;
			}
			i++;
		}
		// �߰ߵ��� �ʾҴ�! ���� ��ü�� �����͸� �ٲ۴�.
		lpCurrentStateMap = lpCurrentStateMap->lpBaseStateMap;
		lpEntries = lpCurrentStateMap->lpStateMapEntries;
	}
	return NULL;
}


void CAIInterface::RouteMessage()
{
	AIMSG msg;

	while( !m_MsgQueue.empty() )
	{
		msg	= m_MsgQueue.front();
		m_MsgQueue.pop();
		if( msg.dwMessage == AIMSG_SETSTATE )
		{
			msg.dwMessage	= AIMSG_INIT;
			m_pStateFunc	= GetAIStateFunc( msg.dwParam1 );
		}
		if( m_pStateFunc )
			( this->*( m_pStateFunc ) ) ( msg );
	}
	if( m_pStateFunc )
	{
		msg.dwMessage	= AIMSG_PROCESS;
		msg.dwParam1	= 0;
		msg.dwParam2	= 0;
		( this->*( m_pStateFunc ) ) ( msg );
	}
}

void CAIInterface::SendAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
	AIMSG msg;
	msg.dwMessage	= dwMessage;
	msg.dwParam1	= dwParam1;
	msg.dwParam2	= dwParam2;
	if( dwMessage == AIMSG_SETSTATE )
	{
		msg.dwMessage	= AIMSG_INIT;
		m_pStateFunc	= GetAIStateFunc( dwParam1 );
	}
	if( m_pStateFunc )
		( this->*( m_pStateFunc ) ) ( msg );
}

void CAIInterface::PostAIMsg( DWORD dwMessage, DWORD dwParam1, DWORD dwParam2 )
{
#if __VER >= 9	//__AI_0509
	switch( dwMessage )
	{
		case AIMSG_ARRIVAL:
			{
				AIMSG msg;
				msg.dwMessage	= dwMessage;
				msg.dwParam1	= dwParam1;
				msg.dwParam2	= dwParam2;
				m_MsgQueue.push( msg );
				break;
			}
		default:	SendAIMsg( dwMessage, dwParam1, dwParam2 );	break;
	}
#else	// __AI_0509
	AIMSG msg;
	msg.dwMessage	= dwMessage;
	msg.dwParam1	= dwParam1;
	msg.dwParam2	= dwParam2;
	m_MsgQueue.push( msg );
#endif	// __AI_0509
}

#if __VER >= 9	//__AI_0509
CMover* CAIInterface::ScanTargetNext( CObj* pObjCenter, int nRange, OBJID dwIdTarget, u_long uParty )
{
	// Ÿ���� �������� ��, ���� ������ �����ϴ� �ֺ��� ���� ��Ƽ���� �˻��Ѵ�.
	// ���� Ÿ���� ��Ƽ�� �����ų�, ����� ����
	if( uParty == 0 || nRange <= 0 || GetMover()->GetProp()->dwClass == RANK_GUARD )
		return NULL;

	TRACE( "ScanTargetNext\n" );
	CObj* pObj;
	D3DXVECTOR3 vPos	= pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	CMover *pTarget	= NULL;

	BOOL bFlyMob	= GetMover()->IsFlyingNPC();
	float fRadius	= (float)( nRange * nRange );

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetMover()->GetLayer() )
	{
		pTarget	= (CMover *)pObj;
		if( pTarget->IsLive() && pTarget->GetId() != dwIdTarget ) 
		{
			if( bFlyMob == pTarget->IsFly() || bFlyMob == FALSE  )
			{
				vDist	= pTarget->GetPos() - vPos;				
				if( D3DXVec3LengthSq( &vDist ) < fRadius && pTarget->m_idparty == uParty )
				{
					if( pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )
						if( pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == FALSE )
							return pTarget;
				}
			}
		}
	}
	END_LINKMAP
	return NULL;
}
#endif	// __AI_0509

// pObjCenter�� �߽����� nRangeMeter�ݰ泻�� �����³ѵ��� ������� �ɸ��� �� ���/
// nJobCond : �ش� ������ ������. �����˻� ����.
CMover* CAIInterface::ScanTarget( CObj* pObjCenter, int nRangeMeter, int nJobCond, DWORD dwQuest, DWORD dwItem, int nChao )
{
	CObj* pObj;
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	CMover *pTarget = NULL;
	int nAble = 0;
	BOOL bCondScan = FALSE;
	if( nJobCond || dwQuest || dwItem || nChao )
		bCondScan = TRUE;

	BOOL bGuard = ( GetMover()->GetProp()->dwClass == RANK_GUARD );
	BOOL bFlyMob = GetMover()->IsFlyingNPC();

	float fRadius = (float)( nRangeMeter * nRangeMeter );	// �Ÿ� 

	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, nRangeMeter, CObj::linkPlayer, GetMover()->GetLayer() )
	{
		ASSERT( pObj->GetType() == OT_MOVER && ((CMover *)pObj)->IsPlayer() );
		pTarget = (CMover *)pObj;

		// ���� ������ �ƴ϶�� �� �����ϴ�.
		nAble = 0;
		// ���� �����̶�� ������ ��ġ�ϴ� �� �����ϴ�.
		if( bCondScan == TRUE && pTarget->IsLive() ) 
		{
			// 1. ���� ���� üũ    
			if( nJobCond == 0 )
				nAble++;
			else if( nJobCond == JOB_ALL || pTarget->GetJob() == nJobCond )		// JOB_ALL : �������
				nAble++;
			// 2. ������ ���� üũ  
			if( dwItem == 0 )
				nAble++;
			else if( pTarget->GetItemNum( dwItem ) != 0 )	
				nAble++;
			// 3. ����Ʈ ���� üũ 
			if( dwQuest == 0 )
				nAble++;
			else if( pTarget->GetQuest( dwQuest ) != NULL )
				nAble++;	
			//4. ī�� ���� üũ  
			if( nChao == 0 )
				nAble++;
			else
			{
				// 100 = ī������, 101 = ��ī��
				if( nChao == 100 )
				{
#if __VER >= 8 // __S8_PK
					if( pTarget->IsChaotic() )
#else // __VER >= 8 // __S8_PK
					if( pTarget->IsGuardReactionChao() )
#endif // __VER >= 8 // __S8_PK
						nAble++;
				}
				else if( nChao == 101 )
				{
#if __VER >= 8 // __S8_PK
					if( !pTarget->IsChaotic() )
#else // __VER >= 8 // __S8_PK
					if( pTarget->IsGuardReactionNormal() )
#endif // __VER >= 8 // __S8_PK
						nAble++;
				}
			}
		}
		if( nAble == 4 )
		{
			// ����� ������ ����			
			// ������� �����÷��̾ ����, ������� �����÷��̾�� ���� - 1. true true  2. false false

#ifdef __JHMA_VER_8_6     // 8�� ������Ͱ� �������������� ���ݰ����ϰ���   World
			if( bGuard || bFlyMob == pTarget->IsFly() || bFlyMob == FALSE  )		
#else	// __VER >= 8  
			if( bGuard || bFlyMob == pTarget->IsFly() )		
#endif	// __VER >= 8  
			{
				vDist = pTarget->GetPos() - vPos;				
				if( D3DXVec3LengthSq( &vDist ) < fRadius )	// �� ��ü���� �Ÿ��� ���� �̳��̸� 
				{
					if( pTarget->IsMode( TRANSPARENT_MODE ) == FALSE )
						if( pTarget->HasBuffByIk3( IK3_TEXT_DISGUISE ) == FALSE )
							return pTarget;
				}
			}
		}
	}
	END_LINKMAP
	return NULL;
}

// �ݰ泻���� ���� ���ѳ�(������)�� ���.
CMover* CAIInterface::ScanTargetStrong( CObj* pObjCenter, FLOAT fRangeMeter  )
{
	CObj *pObj;
	CObj *pObjMax = NULL;	// ������ ������.
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	
	// ���� 
	FLOAT fRadius = fRangeMeter * fRangeMeter;
	
	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, (int)( fRangeMeter / MPU ), CObj::linkPlayer, GetMover()->GetLayer() )
	{
		vDist = pObj->GetPos() - vPos;	// ����ǥ�� ����
		float fDistSq = D3DXVec3LengthSq( &vDist );		// �ο�����Ʈ���� �Ÿ�Sq
		if( fDistSq < fRadius )	
		{
			if(  !( ((CMover*)pObj)->IsMode( TRANSPARENT_MODE ) ) )
			{
				if( pObjMax )
				{
					if( ((CMover *)pObj)->GetLevel() > ((CMover *)pObjMax)->GetLevel() )		// �� ����� ã�Ҵ�.
						pObjMax = pObj;
				} else
					pObjMax = pObj;

			}
		}
	}
	END_LINKMAP

	return (CMover *)pObjMax;
}

// �ݰ泻���� ������ �ϴ� ��� ã�Ƽ� ������.
CMover* CAIInterface::ScanTargetOverHealer( CObj* pObjCenter, FLOAT fRangeMeter  )
{
#ifndef __CLIENT
	CObj *pObj;
	D3DXVECTOR3 vPos = pObjCenter->GetPos();
	D3DXVECTOR3 vDist;
	
	// ���� 
	FLOAT fRadius = fRangeMeter * fRangeMeter;
	
	FOR_LINKMAP( pObjCenter->GetWorld(), vPos, pObj, (int)( fRangeMeter / MPU ), CObj::linkPlayer, GetMover()->GetLayer() )
	{
		vDist = pObj->GetPos() - vPos;	// ����ǥ�� ����
		float fDistSq = D3DXVec3LengthSq( &vDist );		// �ο�����Ʈ���� �Ÿ�Sq
		if( fDistSq < fRadius )	
		{
			if(  !( ((CMover*)pObj)->IsMode( TRANSPARENT_MODE ) ) )
			{
				CMover *pMover = (CMover *)pObj;

				if( pMover->IsPlayer() )
					if( ((CUser *)pMover)->m_nOverHeal > 0 )		// ���������� ã�Ҵ�.
						return pMover;
			}
		}
	}
	END_LINKMAP
#endif	// __CLIENT
	return NULL;
}


