#include "stdafx.h"
#include "defineText.h"
#include "defineItem.h"
#include "defineSkill.h"
#include "defineObj.h"
#include "dpcoreclient.h"
#include "dpdatabaseclient.h"
#include "dpsrvr.h"
#include "user.h"
#include "worldmng.h"
#include "misc.h"

#include "guild.h"
extern	CGuildMng	g_GuildMng;


#include "..\_Common\Ship.h"


#include "..\_aiinterface\AIPet.h"

#include "Party.h"
extern	CPartyMng			g_PartyMng;
extern	CUserMng			g_UserMng;
extern	CDPDatabaseClient	g_dpDBClient;
extern	CDPCoreClient		g_DPCoreClient;
extern	CWorldMng			g_WorldMng;

#include "..\_Network\ErrorCode.h"


void CDPSrvr::OnUseSkill( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	WORD wType;			// ������ų�̳�, ���̼�����ų�̳� �����ϴ� ���� - 2005.10.04 �ǹ̾��� 
	WORD wId;
	OBJID objid;
	int	 nUseType = 0;

	ar >> wType >> wId >> objid >> nUseType;
#if __VER >= 8 // __S8_PK
	BOOL bControl;
	ar >> bControl;
#endif // __VER >= 8 // __S8_PK
	
	int nIdx = wId;
	if( nIdx < 0 || nIdx >= MAX_SKILL_JOB )
		return;

	wType = 0;
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( (CObj*)pUser ) )
	{
		if( pUser->m_vtInfo.VendorIsVendor() )
			return;

#ifdef __S_SERVER_UNIFY
		if( pUser->m_bAllAction == FALSE )
			return;
#endif // __S_SERVER_UNIFY
		
		LPSKILL pSkill	= pUser->GetSkill( wType, nIdx );
		if( !pSkill )
			return;
		if( pSkill->dwSkill == DWORD(-1) )
			return;
		ItemProp* pSkillProp	= prj.GetSkillProp( pSkill->dwSkill );
		if( !pSkillProp )
			return;
	#ifdef __SKILL0517
		DWORD dwLevel		= pUser->GetSkillLevel( pSkill );
	#else	// __SKILL0517
		DWORD dwLevel		= pSkill->dwLevel;
	#endif	// __SKILL0517
		if( dwLevel == 0 || dwLevel > pSkillProp->dwExpertMax )
			return;

#if __VER >= 8 // __S8_PK
		BOOL fSuccess	= pUser->DoUseSkill( wType, nIdx, objid, (SKILLUSETYPE)nUseType, bControl );
#else // __VER >= 8 // __S8_PK
		BOOL fSuccess	= pUser->DoUseSkill( wType, nIdx, objid, (SKILLUSETYPE)nUseType );
#endif // __VER >= 8 // __S8_PK
		if( fSuccess == TRUE )	// ��ų��뿡 �����߰�
		{
			if( nUseType == SUT_QUEUESTART )	// ��ųť�� �����϶�� �Ѱſ���.
			{
				pUser->m_playTaskBar.m_nUsedSkillQueue = 0;		// ��ųť �������� ǥ�� ����.
			}
		}

		if( TRUE == fSuccess )
		{
		}
		else	// �������� UseSkill�� �����ϸ� �װ��� �� Ŭ������ �˷���� �Ѵ�.
		{
			TRACE( "Fail %d, ", nIdx );
			pUser->AddHdr( GETID( pUser ), SNAPSHOTTYPE_CLEAR_USESKILL );
		}
	}
}
/*
void CDPSrvr::OnDoCollect( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
}
*/
// Ŭ���̾�Ʈ�� ���� ���� �����ڵ带 �α׷� �����.
void CDPSrvr::OnError( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	int		nCode, nData;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		ar >> nCode >> nData;
		switch( nCode )
		{
		case FE_GENERAL:
			break;
		case FE_INVALIDATTACKER:	// Ŭ���̾�Ʈ���� ����Ŀ�� invalid�Ѱ��(���ɸ� ����)
			{
#ifndef _DEBUG
				OBJID idAttacker = (OBJID)nData;
				CMover *pAttacker = prj.GetMover( idAttacker );
				if( IsValidObj(pAttacker) )
				{
					Error( "2OnError : FE_INVALIDATTACKER ������:%s(%f,%f,%f), ������:%s(%f,%f,%f)", pUser->GetName(), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z,
						pAttacker->GetName(), pAttacker->GetPos().x, pAttacker->GetPos().y, pAttacker->GetPos().z );
					pUser->AddCorrReq( pAttacker );	// ��û�� Ŭ�󿡰� �ι븮���� ����Ŀ�� �ٽ� ������.
				}
				else
					Error( "2OnError : FE_INVALIDATTACKER ������:%s(%f,%f,%f) �̷����� ���������� pAttacker�� Invalid��. 0x%08x", 
					pUser->GetName(), pUser->GetPos().x, pUser->GetPos().y, pUser->GetPos().z,
					nData );
				
#endif // not debug
				//		case NEXT:
				
			}
			break;
		}
	} else
		Error( "CDPSrvr::OnError pUser - Invalid %d %d", dpidCache, dpidUser );
}

void CDPSrvr::OnShipActMsg( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	DWORD dwMsg;
	OBJID idShip;
	int nParam1, nParam2;
	ar >> dwMsg >> nParam1 >> nParam2 >> idShip;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		if( IsInvalidObj( pUser->GetIAObjLink() ) )		return;
		if( idShip != pUser->GetIAObjLink()->GetId() )
		{
			Error( "OnShipActMsg : Ŭ�󿡼� ������ ���̵�(%d)�� ���������� ���̵�(%d)�� �ٸ���", idShip, pUser->GetIAObjLink()->GetId() );
			return;
		}
		
		CShip *pShip = prj.GetShip( idShip );
		if( IsValidObj( pShip ) )
		{
			pShip->SendActMsg( (OBJMSG)dwMsg, nParam1, nParam2, 0 );
			//			g_DPCoreClient.SendShipActMsg( pUser, dwMsg, nParam1, nParam2 );
			g_UserMng.AddShipActMsg( pUser, pShip, dwMsg, nParam1, nParam2 );
		}
	}
}

void CDPSrvr::OnLocalPosFromIA( CAr & ar, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize )
{
	D3DXVECTOR3 vLocal;
	OBJID		idIA;
	ar >> vLocal >> idIA;
	
	CUser* pUser	= g_UserMng.GetUser( dpidCache, dpidUser );
	if( IsValidObj( pUser ) )
	{
		// Ŭ���̾�Ʈ�κ��� ������������ �����ǥ�� �޾Ҵ�.
		// �� ��ǥ�� �������� ����ȭ ����
		CShip *pIA = prj.GetShip( idIA );
		if( IsInvalidObj( pIA ) )	return;

		
		D3DXVECTOR3 vPos = pIA->GetPos() + vLocal;		// �����󿡼��� IA������Ʈ�� Ŭ�󿡼� ���� ������ǥ�� ���ļ� ���ο� ��ǥ����
		pUser->SetPos( vPos );
		pUser->SetIAObjLink( pIA );
	}
}