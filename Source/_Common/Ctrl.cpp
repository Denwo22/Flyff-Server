// Ctrl.cpp: implementation of the CCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "defineSkill.h"
#include "defineObj.h"
#include "defineText.h"
#include "ResData.h"
#ifdef __WORLDSERVER
#include "CtrlObjs.h"
#endif
#include "Ship.h"

#ifdef __CLIENT
#include "DPClient.h"
extern	CDPClient	g_DPlay;
#endif  

#ifdef __WORLDSERVER
#include "user.h"
#include "dpcoreclient.h"
#include "dpsrvr.h"

#ifdef __WORLDSERVER
 extern	CUserMng	g_UserMng;
 extern	CDPCoreClient	g_DPCoreClient;
 extern	CDPSrvr		g_DPSrvr;
 #include "Party.h"
 extern	CPartyMng	g_PartyMng;
 #include "dpDatabaseClient.h"
 extern	CDPDatabaseClient	g_dpDBClient;
#endif // Worldserver
#endif

#include "commonctrl.h"

extern map< string, DWORD > g_MapStrToObjId;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtrl::CCtrl()
{
	m_dwType = OT_CTRL;
	m_objid	 = NULL_ID;

#ifdef __WORLDSERVER
	m_lRespawn	= -1;
	m_nRespawnType = 0;

	memset( m_nOldCenter, 0, sizeof(int) * MAX_LINKLEVEL );
#endif	// __WORLDSERVER

	m_pIAObjLink = NULL;
	D3DXMatrixIdentity( &m_mInvTM );
}

CCtrl::~CCtrl()
{
#ifdef __WORLDSERVER
	if( IsVirtual() )
		return;

	RemoveItFromView();
#endif	// __WORLDSERVER

	RemoveItFromGlobalId();
}

void CCtrl::Process()
{
	CObj::Process();
#ifdef __WORLDSERVER
	ProcessDeleteRespawn();
#endif
}

void CCtrl::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CObj::Render( pd3dDevice );
}

BOOL CCtrl::Read( CFileIO* pFile )
{
	return CObj::Read( pFile );
}
// this�� pIA�� ��ũ�Ǿ���.
// pIA�� NULL�̸� ��ũ����.
void CCtrl::SetIAObjLink( CShip *pIA )
{
	if( pIA != NULL )	// IA�� ��ũ���Ѿ� �Ҷ�...
	{
		if( m_pIAObjLink != pIA )				// ������ ��ũ�� �ٸ���ũ�� �ɶ���...
			if( pIA->FindCtrl( GetId() ) == NULL )			// pIA�� �̹� this�� ��ϵǾ����� ����...
				pIA->AddCtrl( GetId() );			// IA������Ʈ�� this�� ��Ͻ�Ŵ.
	} else
	{
		if( m_pIAObjLink )
			m_pIAObjLink->RemoveCtrl( GetId() );
	}

	m_pIAObjLink = pIA;
}

void CCtrl::AddItToGlobalId()
{
	prj.m_objmap.Add( this );

	if( OT_MOVER == GetType() && ( (CMover*)this )->IsPlayer() )
	{
		bool bResult = prj.m_idPlayerToUserPtr.insert( make_pair( ( (CMover*)this )->m_idPlayer, (CMover*)this ) ).second;
		if( bResult == false )
			Error("AddItToGlobalId id:%d duplicated.", ((CMover*)this)->m_idPlayer );
	}

#ifdef __WORLDSERVER	
	if( GetType() == OT_CTRL )
	{
		CCommonCtrl* pCommonCtrl = (CCommonCtrl*)this;
		if( pCommonCtrl->m_CtrlElem.m_strCtrlKey[0] != '\0' )
			g_MapStrToObjId.insert( map< string, DWORD >::value_type(pCommonCtrl->m_CtrlElem.m_strCtrlKey, pCommonCtrl->GetId() ) );
	}
#endif // __WORLDSERVER
}

void CCtrl::RemoveItFromGlobalId()
{
	if( m_objid == NULL_ID )	
		return;

	prj.m_objmap.RemoveKey( m_objid );

	if( OT_MOVER == GetType() && ( (CMover*)this )->IsPlayer() && ( (CMover*)this )->m_idPlayer != (u_long)0xffffffff )
		prj.m_idPlayerToUserPtr.erase( ( (CMover*)this )->m_idPlayer );

#ifdef __WORLDSERVER	
	if( GetType() == OT_CTRL )
	{
		CCommonCtrl* pCommonCtrl = (CCommonCtrl*)this;
		if( pCommonCtrl->m_CtrlElem.m_strCtrlKey[0] != '\0' )
			g_MapStrToObjId.erase( pCommonCtrl->m_CtrlElem.m_strCtrlKey );
	}
#endif // __WORLDSERVER
}

#ifdef __WORLDSERVER

BOOL CCtrl::ProcessDeleteRespawn()
{
	// �������� Remove�̸� ���� ��� 
#ifdef __LAYER_1021
	CRespawnInfo* pRespawnInfo = GetWorld()->m_respawner.GetRespawnInfo( GetRespawn(), m_nRespawnType, GetLayer() );
#else	// __LAYER_1021
	CRespawnInfo* pRespawnInfo = GetWorld()->m_respawner.GetRespawnInfo( GetRespawn(), m_nRespawnType );
#endif	// __LAYER_1021
	if( pRespawnInfo && pRespawnInfo->m_bRemove )
	{
		Delete();
		return TRUE;
	}
	return FALSE;
}

void CCtrl::RemoveItFromView( BOOL bRemoveall )
{
	if( !GetWorld() )	
		return;

	CUser* pUser;
	if( GetType() == OT_MOVER && ( (CMover*)this )->IsPlayer() )
	{
		map<DWORD, CUser* >::iterator it = m_2pc.begin();
		while( it != m_2pc.end() )
		{
			pUser = it->second;
			if( pUser != this && pUser->PCRemoveKey( GetId() ) )
				pUser->AddRemoveObj( GetId() );
			++it;
		}

		if( bRemoveall )
			m_2pc.clear();
	}
	else
	{
		map<DWORD, CUser* >::iterator it = m_2pc.begin();
		while( it != m_2pc.end() )
		{
			pUser = it->second;
			if( pUser->NPCRemoveKey( GetId() ) )
				pUser->AddRemoveObj( GetId() );
			++it;
		}

		if( bRemoveall )
			m_2pc.clear();
	}

}

BOOL CCtrl::IsNearPC( CUser* pUser )
{
	return IsNearPC( pUser->GetId() );
}

BOOL CCtrl::IsNearPC( OBJID objid )
{
	map<DWORD, CUser* >::iterator it = m_2pc.find( objid );
	return ( it != m_2pc.end() ) ;
}

#endif	// __WORLDSERVER

BOOL	CCtrl::GetSkillProp( ItemProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr )
{
	ItemProp* pSkillProp = prj.GetSkillProp( nSkill );	// UseSkill���� ����� ��ų�� ������Ƽ ����
	if( pSkillProp == NULL )
	{
		Error( "%s : %d. ��ų(%d)�� ������Ƽ�� ����.", szErr, GetIndex(), nSkill );
		return FALSE;
	}
	
	
	AddSkillProp *pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, dwLevel );
	if( pAddSkillProp == NULL )
	{
		Error( "%s : %d. Add��ų(%d) Lv%d�� ������Ƽ�� ����.", szErr, GetIndex(), nSkill, dwLevel );
		return FALSE;
	}

	if( pAddSkillProp->dwName != pSkillProp->dwID )
		Error( "%s : %d. Add��ų(%d) AddSkill->dwName�� Skill->dwID�� �ٸ���.", szErr, GetIndex(), nSkill );
	
	*ppSkillProp = pSkillProp;
	*ppAddSkillProp = pAddSkillProp;
	
	return TRUE;
}


// vPos�� �߽����� �ֺ��� ȿ���� �����Ѵ�.
// ------------------------------------------
// Ʈ���� ������ �ְ�. ���Ͱ��������ִ�(��), ���ΰ��� ��ų�� ����Ҽ��� �ִ�.
// ȿ���� �ݵ�� SkillProp�� ���� �� ���̵� �Ѱܾ� �Ѵ�.
//
// ��쿡���� pAddSkillProp�� NULL�ϼ��� �ִ�.  �����ۿ��� ����Ҽ��� �ֱ⶧��. �������� �ֵ彺ų������ ����.
// pCenter : ���� ���ӽ�ų�� Ÿ��. NULL�ϼ��� �ִ�
#if __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillRegion( const D3DXVECTOR3 &vPos, int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, BOOL bOnlyDmg, CCtrl* pCenter, BOOL bControl )
#else // __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillRegion( const D3DXVECTOR3 &vPos, int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, BOOL bOnlyDmg, CCtrl* pCenter )
#endif // __VER >= 8 // __S8_PK
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	CCtrl *pSrc = this;
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		Error( "CCtrl::ApplySkillRegion : pSkillProp == NULL A:%d-%d", pSrc->GetType(), pSrc->GetIndex() );

	FLOAT fRange = 4.0f;
	if( pAddSkillProp )										// �ֵ彺ų�� ������ �ű⼭ ����
	{
		fRange = (float)pAddSkillProp->dwSkillRange;
		if( pAddSkillProp->dwSkillRange == NULL_ID )
			Error( "��� : ��ų %d�� dwSkillRange�� �������� ����", pAddSkillProp->dwID );
	}
	else
	{
		fRange = (float)pSkillProp->_dwSkillRange;			// ������ ��ų������Ƽ���� ����.
		if( pSkillProp->_dwSkillRange == NULL_ID )
			Error( "��� : ��ų %d�� dwSkillRange�� �������� ����", pSkillProp->dwID );
	}

	if( fRange <= 4.0f )		nRange = 4;
	else if( fRange <= 8.0f )	nRange = 8;
	else if( fRange <= 16.0f )	nRange = 16;
	else						nRange = 32;

	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		Error( "CCtrl::ApplySkillRegion : D:%d A:%d-%d %d %f", GetIndex(), pSrc->GetType(), pSrc->GetIndex(), pSkillProp->dwID, fRange );

	BOOL	bApply = FALSE;
	BOOL	bTarget = TRUE;

	//------------ �������� �÷��̾��ΰ� 
	if( nApplyType & OBJTYPE_PLAYER )	
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			bApply = FALSE;	// �ϴ� FALSE�� �ʱ�ȭ

			if( pObj->GetType() != OT_MOVER )
				Error( "ApplySkillRegion : pObj�� MOVER�� �ƴϴ� %d", pObj->GetType() );

			if( pSrc->GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply = TRUE;
#if __VER >= 8 // __S8_PK
					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						if( pCenter )
							bTarget = (pCenter == pDefender);
						else
							bTarget = FALSE;

						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
							bApply = FALSE;
//	#ifdef	__JHMA_VER_8_5_2			 // 8.5�� ������� �α����� ���������ʰ� ����  World
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
							bApply = FALSE;
//	#endif // __JHMA_VER_8_5_1	
					}
#endif // __VER >= 8 // __S8_PK
				} else
				{	// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			} else
			{	// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}

			if( bApply )
			{
				if( pObj != pSrc )		// ����Ŀ�� �˻���󿡼� ����.
				{
					if( pObj->IsRangeObj( vPos, fRange ) )	// vCenter + fRange�� pObj�� ���Ǿ �浹�ߴ���
					{
						CMover *pTarget = (CMover *)pObj;		// <- ���� ������ pObj�� �÷��̾�Ƿ� �ص���.
					
						if( pCenter )
							bTarget = (pCenter == pTarget);
						else
							bTarget = FALSE;

						if( IsValidObj( pTarget ) && pTarget->IsLive() )
						{
							pTarget->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg, bTarget );		// ��󿡰� ȿ���� ������.
						}
					}
				}
				
				bApply = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
			} // bApply
		}
		END_LINKMAP
	} // OBJTYPE_PLAYER

	// �������� ����/��Ʈ�� �ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )	// linkDynamic�� ����
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTarget = (CMover *)pObj;
				if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;
#if __VER >= 8 //	#ifdef	__JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;
#endif //	#endif // __JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
			} else
			// ����� ��Ʈ���̸�.
			if( pObj->GetType() == OT_CTRL )
			{
				if( pSrc->GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
					bApply = TRUE;							
			}

			if( bApply )
			{
				if( pObj != pSrc )		// �����ڴ� �˻��󿡼� ����.
				{
					if( pObj->IsRangeObj( vPos, fRange ) )	// vCenter + fRange�� pObj�� ���Ǿ �浹�ߴ���
					{
						if( pObj->GetType() == OT_MOVER )	// Ÿ���� ������?
						{
							if( IsValidObj( pObj ) && ((CMover *)pObj)->IsLive() )		// Ÿ���� ������ ����ִ��� �˻�.
								((CCtrl*)pObj)->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
						} else
						{
							// Ÿ���� ������ �ƴϳ�?
							((CCtrl*)pObj)->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
						}
					}
				}
			} // bApply
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
} // ApplySkillRegion()


// this�� �߽����� �ֺ��� ȿ���� �����Ѵ�.
// ------------------------------------------
// Ʈ���� ������ �ְ�. ���Ͱ��������ִ�(��), ���ΰ��� ��ų�� ����Ҽ��� �ִ�.
// ȿ���� �ݵ�� SkillProp�� ���� �� ���̵� �Ѱܾ� �Ѵ�.
//
// ��쿡���� pAddSkillProp�� NULL�ϼ��� �ִ�.  �����ۿ��� ����Ҽ��� �ֱ⶧��. �������� �ֵ彺ų������ ����.
// fRangeCustom�� ���� ������쿡�� ������Ƽ nRange���� �����ϰ� fRangeCustom���� �ٲ��.
#if __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillAround( CCtrl *pSrc, int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, BOOL bOnlyDmg, FLOAT fRangeCustom, BOOL bControl )
#else // __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillAround( CCtrl *pSrc, int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, BOOL bOnlyDmg, FLOAT fRangeCustom )
#endif // __VER >= 8 // __S8_PK
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		Error( "CCtrl::ApplySkillAround : pSkillProp == NULL D:%d A:%d-%d", GetIndex(), pSrc->GetType(), pSrc->GetIndex() );

	FLOAT fRange = 4.0f;
	if( pAddSkillProp )										// �ֵ彺ų�� ������ �ű⼭ ����
		fRange = (float)pAddSkillProp->dwSkillRange;
	else
		fRange = (float)pSkillProp->_dwSkillRange;			// ������ ��ų������Ƽ���� ����.

	if( fRange <= 4.0f )		nRange = 4;
	else if( fRange <= 8.0f )	nRange = 8;
	else if( fRange <= 16.0f )	nRange = 16;
	else						nRange = 32;

	if( fRangeCustom != 0.0f )
	{
		fRange = fRangeCustom;
	}

	FLOAT fTargetRatio = 0.0f;
	if( pSkillProp->dwSpellRegion == SRO_REGION )	// REGION�Ӽ��϶� Ÿ�ٵ� �������� ����.
		fTargetRatio = 1.0f;

	if( fRange <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		Error( "CCtrl::ApplySkillAround : D:%d A:%d-%d %d %f", GetIndex(), pSrc->GetType(), pSrc->GetIndex(), pSkillProp->dwID, fRange );

	BOOL	bApply = FALSE;
	BOOL	bTarget = FALSE;

	//------------ �������� �÷��̾��ΰ� 
	if( nApplyType & OBJTYPE_PLAYER )	
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			bApply = FALSE;	// �ϴ� FALSE�� �ʱ�ȭ

			if( pObj->GetType() != OT_MOVER )
				Error( "ApplySkillAround : pObj�� MOVER�� �ƴϴ� %d", pObj->GetType() );

			if( pSrc->GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply	= TRUE;
#if __VER >= 8 // __S8_PK
					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						bTarget = (this == pDefender);
						
						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
							bApply = FALSE;
//	#ifdef	__JHMA_VER_8_5_2			 // 8.5�� ������� �α����� ���������ʰ� ����  World
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
							bApply = FALSE;
//	#endif // __JHMA_VER_8_5_1	

					}
#endif // __VER >= 8 // __S8_PK
				} else
				{	// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			} else
			{	// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}

			if( bApply )
			{
				if( pObj->IsRangeObj( this, fRange ) )	// this(Center)�� ���Ǿ�� pObj�� ���Ǿ �浹�ߴ���
				{
					if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					} else
					{
						if( pObj != pSrc )		// ����Ŀ�� �˻���󿡼� ����.
						{
							CMover *pTarget = (CMover *)pObj;		// <- ���� ������ pObj�� �÷��̾�Ƿ� �ص���.
							bTarget = (this == pTarget);
							if( IsValidObj( pTarget ) && pTarget->IsLive() )
								pTarget->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg, bTarget );		// ��󿡰� ȿ���� ������.
						}
					}
				}
				
				bApply = FALSE;	// ���� ������ ���ؼ� �ʱ�ȭ.
			} // bApply
		}
		END_LINKMAP
	} // OBJTYPE_PLAYER

	// �������� ����/��Ʈ�� �ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )	// linkDynamic�� ����
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTarget = (CMover *)pObj;
				if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;
#if __VER >= 8 //	#ifdef	__JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)pSrc;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;
#endif //	#endif // __JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
			} else
			// ����� ��Ʈ���̸�.
			if( pObj->GetType() == OT_CTRL )
			{
				if( pSrc->GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
					bApply = TRUE;							
			}

			if( bApply )
			{
				if( pObj->IsRangeObj( this, fRange ) )	// this(Center)�� ���Ǿ�� pObj�� ���Ǿ �浹�ߴ���
				{
					if( pObj == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
					{
					} else
					{
						if( pObj != pSrc )		// �����ڴ� �˻��󿡼� ����.
						{
							if( pObj->GetType() == OT_MOVER )	// Ÿ���� ������?
							{
								if( IsValidObj( pObj ) && ((CMover *)pObj)->IsLive() )		// Ÿ���� ������ ����ִ��� �˻�.
									((CCtrl*)pObj)->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
							} else
							{
								// Ÿ���� ������ �ƴϳ�?
								((CCtrl*)pObj)->ApplySkill( pSrc, pSkillProp, pAddSkillProp, bIgnoreProb, 0, bOnlyDmg );		// ��󿡰� ȿ���� ������.
							}
						}
					}
				}
			} // bApply
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
} // ApplySkillAround()

// 
// ��, ����, ���̸� �����Ͽ� ����ü ������ ������ ������ ����.
// nDmgType : AF_???
// nApplyType : OBJTYPE_PLAYER , MONSTER
// nAttackID : ��ų�̳� ������ ID
// fWidth, fHeight, fDepth : ��, ����, ����.
// this == Attacker
#if __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillLine( int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, float fWidth, float fHeight, float fDepth, bool bIgnoreProb, BOOL bControl )
#else // __VER >= 8 // __S8_PK
void	CCtrl::ApplySkillLine( int nApplyType, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, float fWidth, float fHeight, float fDepth, bool bIgnoreProb )
#endif // __VER >= 8 // __S8_PK
{
#ifdef __WORLDSERVER
	int nRange	= 4;	// 4m
	CObj* pObj;
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDist;

	if( pSkillProp == NULL )
		Error( "CCtrl::ApplySkillLine : pSkillProp == NULL %d", GetIndex() );
	
	// ������ ���� Local AABB.
	D3DXVECTOR3 vMin1 = D3DXVECTOR3( -fWidth / 2.0f, -fHeight / 2.0f, -fDepth );
	D3DXVECTOR3 vMax1 = D3DXVECTOR3(  fWidth / 2.0f,  fHeight / 2.0f, 0 );

	D3DXMATRIX mInv;		// �������� ���忪���
	D3DXMATRIX m1, m2, m3;
	D3DXMatrixRotationY( &m1, D3DXToRadian( -GetAngle() ) );
	D3DXMatrixTranslation( &m2, GetPos().x, GetPos().y, GetPos().z );
	D3DXMatrixMultiply( &m3, &m1, &m2 );	// world = rot * trans
	
	D3DXMatrixInverse( &mInv, NULL, &m3 );		// �������� ����� ����.

	// Ÿ���� ��ǥ�� �������� �������� ����ȯ �Ѱ�.
	D3DXVECTOR3 vDestLocal;

	// �Ϲ������� fDepth�� ���� ��⶧���� �˻� ������ fDepth�� �ߴ�. 
	if( fDepth <= 4.0f )		nRange = 4;
	else if( fDepth <= 8.0f )	nRange = 8;
	else if( fDepth <= 16.0f )	nRange = 16;
	else						nRange = 32;
	
	if( fDepth <= 0 )	// ������ 0�̰ų� �����ϼ��� ����.
		Error( "CCtrl::ApplySkillLine : %d %d", GetIndex(), fDepth );

	
	BOOL	bApply = FALSE;
	BOOL	bTarget = FALSE;

	if( nApplyType & OBJTYPE_PLAYER )	// �������� �÷��̾��ΰ� 
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkPlayer, GetLayer() )
		{
			bApply = FALSE;	// ������ FALSE

			if( pObj->GetType() != OT_MOVER )
				Error( "ApplySkillLine : pObj�� MOVER�� �ƴϴ� %d", pObj->GetType() );

			if( GetType() == OT_MOVER )		// �����ڰ� ������.
			{
				CMover *pAttacker = (CMover *)this;
				if( pAttacker->IsPlayer() )			// �����ڰ� �÷��̾��
				{
					bApply = TRUE;
#if __VER >= 8 // __S8_PK
					CMover *pDefender = (CMover *)pObj;
					if( pDefender->IsPlayer() )
					{
						if( bControl == FALSE && pAttacker->GetHitType( pDefender, bTarget, FALSE ) == HITTYPE_PK )
							bApply = FALSE;
//	#ifdef	__JHMA_VER_8_5_2			 // 8.5�� ������� �α����� ���������ʰ� ����  World
						else if( pAttacker->m_nDuel == 1 && pDefender->m_nDuel == 1 && pAttacker->GetHitType( pDefender, bTarget, FALSE ) != HITTYPE_PVP )
							bApply = FALSE;
//	#endif // __JHMA_VER_8_5_1	
					}
#endif // __VER >= 8 // __S8_PK
				} else
				{
					// �����ڰ� ���͸�
					bApply = TRUE;		// 
				}
			} else
			{
				// �����ڰ� ������ �ƴϴ�.
				bApply = TRUE;
			}
			
			if( bApply && pObj != this )		// this�� ���ݴ�󿡼� ����.
			{
				CMover *pTarget = (CMover *)pObj;
				if( IsValidObj( pTarget ) && pTarget->IsLive() )
				{
					// Ÿ���� ��ǥ�� ����ȯ.
					D3DXVec3TransformCoord( &vDestLocal, &pTarget->GetPos(), &mInv );
					// Ÿ���� AABB
					D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
					D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
					if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
					{
						pTarget->ApplySkill( this, pSkillProp, pAddSkillProp, bIgnoreProb, 0, FALSE, bTarget );		// bTarget Ÿ���� �����Ƿ� FALSE�� ����. PK�� ������ ĥ������
					}
				}
			} // bApply
		}
		END_LINKMAP
	}

	// �������� �����ΰ�.
	if( nApplyType & (OBJTYPE_MONSTER | OBJTYPE_CTRL) )
	{
		FOR_LINKMAP( GetWorld(), vPos, pObj, nRange, CObj::linkDynamic, GetLayer() )
		{
			bApply = FALSE;
			if( pObj->GetType() == OT_MOVER )				// ����� ������.
			{
				CMover *pTarget = (CMover *)pObj;
				if( pTarget->IsPeaceful() == FALSE )		// NPC�� �ƴѰ�츸 ����
					bApply = TRUE;
#if __VER >= 8 //	#ifdef	__JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
				CMover *pAttacker = (CMover *)this;
				if( pAttacker->IsPlayer() && pAttacker->IsChaotic() == FALSE && pTarget->GetProp()->dwClass == RANK_GUARD )
					bApply = FALSE;
#endif //	#endif // __JHMA_VER_8_5_1			 // 8.5�� ��� ������ų ����ȿ�� �Ұ��� ���� World
			} else
			// ����� ��Ʈ���̸�
			if( pObj->GetType() == OT_CTRL )
			{
				if( GetType() == OT_MOVER )			// �����ڰ� �����϶��� ����. ��Ʈ���� ��Ʈ�ѿ��� ������ �ټ� ����.
					bApply = TRUE;							
			}
			
			
			if( bApply )
			{
				if( pObj->GetType() == OT_MOVER )
				{
					CMover *pTarget = (CMover *)pObj;

					if( IsValidObj( pTarget ) && pTarget->IsLive() )
					{
						// Ÿ���� ��ǥ�� ����ȯ.
						D3DXVec3TransformCoord( &vDestLocal, &pTarget->GetPos(), &mInv );
						// Ÿ���� AABB
						D3DXVECTOR3 vMin2 = vDestLocal + pTarget->m_pModel->m_vMin;
						D3DXVECTOR3 vMax2 = vDestLocal + pTarget->m_pModel->m_vMax;
						if( ::IsTouchAABB( vMin1, vMax1, vMin2, vMax2 ) )		// AABB�浹�˻�.
						{
							((CCtrl*)pObj)->ApplySkill( this, pSkillProp, pAddSkillProp, bIgnoreProb );		// ��󿡰� ȿ���� ������.
						}
					}
				} else
					((CCtrl*)pObj)->ApplySkill( this, pSkillProp, pAddSkillProp, bIgnoreProb );		// ��󿡰� ȿ���� ������.
			}
		}
		END_LINKMAP
	}
		
#endif // WORLDSERVER		
} // ApplySkillLine()

//
// idParty �شܴ�� ���� ȿ��.
//
int		CCtrl::ApplySkillAroundTroupe( u_long idParty, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb )
{
	BOOL bRet = FALSE;
#ifdef __WORLDSERVER
	if( GetType() != OT_MOVER )		return bRet;	// ������ �ƴѰ� �شܽ�ų�� ������ ����.
	if( ((CMover *)this)->IsPlayer() == FALSE )		return bRet;	// �÷��̾ �ƴѰ� �شܽ�ų�� ������ ����.

	CParty *pParty	= g_PartyMng.GetParty( idParty );
	if( pParty )
	{
		int i;
		CUser *pMember;
		D3DXVECTOR3	vDist;
		float	fDist;
		float	fMaxDistSq = (float)pAddSkillProp->dwSkillRange;
		fMaxDistSq *= fMaxDistSq;
		FLOAT fTargetRatio = 0.0f;
		if( pSkillProp->dwSpellRegion == SRO_REGION )
			fTargetRatio = 1.0f;
		for( i = 0; i < pParty->m_nSizeofMember; i++ )
		{
			pMember	= g_UserMng.GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
			if( IsValidObj( pMember ) && !pMember->IsDie()
				&& GetWorld() && pMember->GetWorld() && GetWorld()->GetID() == pMember->GetWorld()->GetID()
				&& GetLayer() == pMember->GetLayer() )	// mirchang_100217 ���� �˻� ���� ����
			{
				if( pMember == this && fTargetRatio == 0.0f )	// Ÿ�ٵ����� ������ 0�̸� �ڱ��ڽ�(Ÿ��)�� �������� ��������.
				{
				} else
				{
					vDist = pMember->GetPos() - GetPos();
					fDist = D3DXVec3LengthSq( &vDist );			// ��Ƽ ����� �����ڿ��� �Ÿ�.
					if( fDist < fMaxDistSq )					// xx���ͺ��� ������
					{
						pMember->ApplySkill( this, pSkillProp, pAddSkillProp, bIgnoreProb );	// ��� �������� ȿ�� ����.
						bRet = TRUE;
					}
				}
			}
		}
	}
#endif // WorldServer
	return bRet;
} // ApplySkillAroundTroupe

//
// ApplySkill ���ο��� ��ų�� �ϵ��ڵ� ó����.
// this : Target
// pSrc : ������.
//
BOOL CCtrl::ApplySkillHardCoding( CCtrl *pSrc, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
#ifdef __WORLDSERVER
	switch( pSkillProp->dwID )
	{
	case SI_ASS_HEAL_RESURRECTION:		// ��Ȱ
		if( GetType() == OT_MOVER )
		{
			CMover *pTarget = (CMover *)this;
			if( pTarget->IsPlayer() && pTarget->IsDie() )	// �÷��̾��̰� Ÿ���� �׾�������
			{
				if( pTarget->m_pActMover->GetDmgState() == OBJSTA_RESURRECTION )
				{
					return FALSE;		// �̹� ��Ȱ���� ������� ���Ŷ�� ���.
				} else
				{
					g_dpDBClient.SendLogLevelUp( pTarget, 10 );	// ��Ȱ ��ų �α�

					if( ((CUser *)this)->m_Resurrection_Data.bUseing != TRUE )
					{
						((CUser *)this)->AddResurrectionMessage();
					
						((CUser *)this)->m_Resurrection_Data.bUseing        = TRUE;
						((CUser *)this)->m_Resurrection_Data.dwPlayerID     = ((CMover*)pSrc)->m_idPlayer;
						((CUser *)this)->m_Resurrection_Data.pSkillProp     = pSkillProp;
						((CUser *)this)->m_Resurrection_Data.pAddSkillProp  = pAddSkillProp;
					}

					break;
				}

				// ��Ȱ�� DST_RECOVERY_EXP�� ������ ��ġ�� ���� ����.
				if( pAddSkillProp->dwDestParam2 == DST_RECOVERY_EXP )
					pTarget->SubDieDecExp(TRUE, pAddSkillProp->nAdjParamVal2 );	// ��Ȱ�� �Ǹ鼭 ��ġ�� ���� ����.
			}
		}
		break;
	} // switch
#endif // WorldServer
	return TRUE;
}

#ifdef __CLIENT
void CCtrl::CreateYoyoSkill( CSfx* pSfx, CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	ItemProp* pItemProp = ((CMover *)this)->GetActiveHandItemProp();
	
	if(pItemProp)
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLocal;
		D3DXVECTOR3 vPosSrc   = GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 
		D3DXVECTOR3 vPosDest  = pTarget->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // ��ǥ ������ ���Ƿ� �÷��ش�. ���� 
		
		CModelObject *pModel = (CModelObject *)m_pModel;
		
		int nSkill = pSkillProp->dwID;
		
		switch( nSkill )
		{
		case SI_ACR_SUP_SLOWSTEP:
		case SI_JST_YOYO_HITOFPENYA:
		case SI_JST_YOYO_VATALSTAB:
			{
				pModel->GetHandPos( &vPos, PARTS_RWEAPON, GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_RWEAPON);
			}
			break;
		case SI_ACR_YOYO_DEADLYSWING:
		case SI_ACR_YOYO_CROSSLINE:
			{
				pModel->GetHandPos( &vPos, PARTS_RWEAPON, GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_RWEAPON);

				pModel->GetHandPos( &vPos, PARTS_LWEAPON, GetMatrixWorld() );
				pSfx = CreateSfxYoYo( D3DDEVICE, pItemProp->dwSfxObj2, vPos, GetId(), vPosDest );	
				((CSfxItemYoyoAtk*)pSfx)->MakePath(PARTS_LWEAPON);
			}
			break;							
		}
	}

}
#endif //__CLIENT

//
// ��ų�� ������ Ŭ�󿡼� �����ؾ��� sfx ó��
//
void CCtrl::CreateSkillSfx( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
#ifdef __CLIENT
		
	{
		// �߻�ü ���°� �ƴϴ�.
		CSfx *pSfx = NULL;
		if( (int)pAddSkillProp->dwSkillTime > 0 )	// ���ӽð��� �ִ� ��ų��
		{
			if( pSkillProp->dwSfxObj4 == NULL_ID )		// ������ �����Ǵ� ��ų�� �ƴҰ�쿡��.
			{
				if( pSkillProp->dwExeTarget == EXT_MELEEATK )
				{
					D3DXVECTOR3 vLocal;
					
					if( pSkillProp->dwLinkKind == IK3_YOYO )
					{
						vLocal = pTarget->GetPos();
						vLocal.y += 1.0f;
					}
					else
					{
						AngleToVectorXZ( &vLocal, GetAngle(), 1.3f );	// �����¹��� 1���;�.
						vLocal += GetPos();
						vLocal.y += 1.0f;
					}

					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, GetPos(), GetId(), vLocal, pTarget->GetId(), 0 );
					
					if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, GetPos(), GetId(), this->GetPos(), this->GetId(), 0 );  // 5�� ����Ʈ�� �����.
				} else
				{
					if( pSkillProp->dwSfxObj3 != NULL_ID )		// Ÿ�ٿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, GetPos(), GetId(), pTarget->GetPos(), pTarget->GetId(), 0 );  // 3�� ����Ʈ�� �����.
					if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
						pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, GetPos(), GetId(), this->GetPos(), this->GetId(), 0 );  // 5�� ����Ʈ�� �����.
				}

				if( pSkillProp->dwLinkKind == IK3_YOYO )  //���� ������������Ƽ�����Ͽ� ����Ʈ ����(����ó��)
				{
				#ifdef __CLIENT
					CreateYoyoSkill( pSfx, pTarget, pSkillProp, pAddSkillProp );
				#endif
				}
			} else
			{
				if( GetType() == OT_MOVER )
				{
					if( pSkillProp->dwSkillType == NULL_ID )	// NULL_ID�� �����۵�
					{
						if( ((CMover *)this)->HasBuff( BUFF_ITEM, (WORD)( pSkillProp->dwID ) ) == FALSE )	// �̹� ����ǰ� ���� �������� ���� ���
							CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj4, this->GetPos(), this->GetId(), pTarget->GetPos(), pTarget->GetId(), (int)(pAddSkillProp->dwSkillTime / 1000.0f) );
					} else
					{
						// �̹� ����ǰ� ���� �������� ���� ���
						if( ((CMover *)this)->HasBuff( BUFF_SKILL, (WORD)( pSkillProp->dwID ) ) == FALSE
							&& pSkillProp->dwID != SI_MER_SHIELD_PROTECTION && pSkillProp->dwID != SI_MER_SHIELD_PANBARRIER )	// 091022 mirchang - ���⼭ ���� ���� �� IBuff.Process���� sfx�� �ٽ� �����ϴ� ���� �߻�. ����.
							CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj4, this->GetPos(), this->GetId(), pTarget->GetPos(), pTarget->GetId(), (int)(pAddSkillProp->dwSkillTime / 1000.0f) );
					}
				}
			}
		} else
		{	// ���ӽð� ���� ��ų
			if( pSkillProp->dwExeTarget == EXT_MELEEATK )	// �������� ��ų�� Ÿ�� ����Ʈ.
			{
				if( pSkillProp->dwSfxObj3 != NULL_ID )	
				{
					D3DXVECTOR3 vLocal;

					if( pSkillProp->dwLinkKind == IK3_YOYO )
					{
						vLocal = pTarget->GetPos();
						vLocal.y += 1.0f;
					}
					else
					{
						AngleToVectorXZ( &vLocal, GetAngle(), 1.3f );	// �����¹��� 1���;�.
						vLocal += GetPos();		// GetPos()�� this���� pTarget���� �ٲ��. 2006/6/20 xuzhu.
						vLocal.y += 1.0f;
					}

					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, vLocal, GetId(), vLocal, pTarget->GetId(), 0 );	// 2006/6/20 xuzhu �� �ٲ�.
				}
			} else
			{
				if( pSkillProp->dwSfxObj3 != NULL_ID )
				{
					D3DXVECTOR3 vPos = pTarget->GetPos();
					vPos.y += 1.0f;
					pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj3, GetPos(), GetId(), vPos, pTarget->GetId(), 0 );
				}
			}

			if( pSkillProp->dwLinkKind == IK3_YOYO )  //���� ������������Ƽ�����Ͽ� ����Ʈ ����(����ó��)
			{
			#ifdef __CLIENT
				CreateYoyoSkill( pSfx, pTarget, pSkillProp, pAddSkillProp );
			#endif
			}
			else
			if( pSkillProp->dwSfxObj5 != NULL_ID )		// �����ڿ��� �߻�
				pSfx = CreateSfx( D3DDEVICE, pSkillProp->dwSfxObj5, GetPos(), GetId(), this->GetPos(), this->GetId(), 0 );  // 5�� ����Ʈ�� ������ �ڽſ��� ���
		}
		if( pSfx )
		{
			D3DXVECTOR3 vPos = GetPos();
			PLAYSND( pSkillProp->dwSndAttack1, &vPos );		// �߻� ȿ����.
		}
			
		//dwSkillRange
		if( pSkillProp->dwID == SI_GEN_FLASH && pSfx )
		{
			FLOAT fRange = 1.5f;
			pSfx->SetScale(  D3DXVECTOR3(fRange, fRange, fRange) );
		}
	}
	
#endif // Client
} // CreateSkillSfx


// ApplySkill ���ο��� ������ �ִºκи� ����.
int		CCtrl::ApplyDamage( CCtrl *pSrc, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, int nParam, BOOL bTarget ) 
{
	int nDamage = 0;
	// �������� ��ų�� ������ ����.
	switch( pSkillProp->dwExeTarget )
	{
	case EXT_MELEEATK:		// ������ų����������
	case EXT_MAGICATK:			// ��ø�������������
	case EXT_MAGICATKSHOT:
		{
			if( pSkillProp->dwID == SI_MAG_FIRE_HOTAIR )	// �ֿ��� ����ó��.
			{
				nDamage = SendDamage( AF_MAGICSKILL, pSrc->GetId(), nParam, bTarget );
			} else
			{
				int nAtkFlag = 0;
				if( pSkillProp->dwSkillType == KT_MAGIC )
					nAtkFlag = AF_MAGICSKILL;
				else
					nAtkFlag = AF_MELEESKILL;
				
				if( pSkillProp->dwComboStyle == CT_FINISH )		nDamage = SendDamageForce( nAtkFlag, pSrc->GetId(), (pSkillProp->dwID << 16) | pAddSkillProp->dwSkillLvl, bTarget );
				else											nDamage = SendDamage( nAtkFlag, pSrc->GetId(), (pSkillProp->dwID << 16) | pAddSkillProp->dwSkillLvl, bTarget );
			}
		}
		break;
	}

	return nDamage;
}

// ���밡���� ��ų�̳�?
BOOL CCtrl::IsPossibleApplySkill( CCtrl *pSrc, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	return TRUE;
}

// pSkillProp�� this�� �����Ѵ�.  �������� �ɼ��� �ְ�. ���º�ȭ�� �ɼ����ִ�.
// pSrc�� ������(Ȥ�� ����Ŀ)
// pAddSkillProp�� NULL�ϼ��� ������ ����.
void	CCtrl::ApplySkill( CCtrl *pSrc, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, int nParam, BOOL bOnlyDmg, BOOL bTarget )
{
#ifdef __PK_PVP_SKILL_REGION
	if( pSrc->GetType() == OT_MOVER && GetType() == OT_MOVER )
	{
		if( ((CMover *)pSrc)->IsPlayer() && ((CMover *)this)->IsPlayer() && pSkillProp->dwSkillType != NULL_ID )
		{
			DWORD dwPKRegionAttr = ((CMover *)this)->GetPKPVPRegionAttr();
			int nAttackerPK = ((CMover *)pSrc)->IsPKPVPInspectionBase( dwPKRegionAttr, FALSE );
			int nDefenDerPK = ((CMover *)this)->IsPKPVPInspectionBase( dwPKRegionAttr, FALSE );
			if( nAttackerPK == 1 || nDefenDerPK == 1 )
			{
				if( pSkillProp->nEvildoing < 0 )
				{
					CMover* pAttacker = (CMover*)pSrc;
					CMover* pDefender = (CMover*)this;
					if( !(pAttacker->IsPVPTarget( pDefender ) || 
						pAttacker->IsWarTarget( pDefender ) ||
						pAttacker->IsGuildCombatTarget( pDefender )
#ifdef __JEFF_11_4
						|| pAttacker->IsArenaTarget( pDefender )
#endif	// __JEFF_11_4
						) )
							return;
					
					//if( !((CMover *)pSrc)->IsPVPTarget((CMover *)this) )
					//	return;
				}
			}
		}
	}
#endif // __PK_PVP_SKILL_REGION
	// ���밡���� ��ų�̳�?
	if( IsPossibleApplySkill( pSrc, pSkillProp, pAddSkillProp ) == FALSE )
		return;

	// �������Ӽ��� ������� �װ��� ó��.
	int nDamage = ApplyDamage( pSrc, pSkillProp, pAddSkillProp, nParam, bTarget );
	if( bOnlyDmg == TRUE )		
		return;

#if __VER >= 10	// __AI_0711
	// �غ��� ���ʹ� ��� ����� �鿪
	if( GetType() == OT_MOVER )
	{
		if( ( (CMover*)this )->IsRank( RANK_MIDBOSS ) )
		{
			// �鿪 ����Ʈ
			return;
		}
	}
#endif	// __AI_0711

#if defined(__WORLDSERVER)
	if( this != pSrc && pSrc->GetType() == OT_MOVER && GetType() == OT_MOVER )		// ���� ����� �� Ÿ���̾ƴϸ� skip
	{
		if( ((CMover *)this)->IsDie() == FALSE )
		{
#if __VER >= 8 // __S8_PK
			if( pSkillProp->nEvildoing < 0 )													// ���۸�����
				if( ((CMover *)pSrc)->GetHitType2(((CMover *)this), bTarget, FALSE ) == HITTYPE_FAIL )  	// ��ȭ���λ�뿡��
					return;																			// ����ȵ�
#ifdef __JEFF_11_4
			CWorld* pWorld	= pSrc->GetWorld();
			if(  pWorld && pWorld->IsArena() == FALSE )		// �Ʒ����� �ƴϸ�,
#endif	// __JEFF_11_4
			{
				if( pSkillProp->nEvildoing > 0 )													// ���� ������
					if( ((CMover *)pSrc)->GetHitType2(((CMover *)this), bTarget, TRUE ) != HITTYPE_FAIL )  	// �������λ�뿡��
						return;																			// ����ȵ�
			}
#else // __VER >= 8 // __S8_PK
			if( pSkillProp->nEvildoing < 0 )													// ���۸�����
				if( ((CMover *)pSrc)->GetHitType2(((CMover *)this), bTarget ) == HITTYPE_FAIL )  	// ��ȭ���λ�뿡��
					return;																			// ����ȵ�
			if( pSkillProp->nEvildoing > 0 )													// ���� ������
				if( ((CMover *)pSrc)->GetHitType2(((CMover *)this), bTarget ) != HITTYPE_FAIL )  	// �������λ�뿡��
					return;																			// ����ȵ�
#endif // __VER >= 8 // __S8_PK
		}
		else
		{
			if( pSkillProp->dwExeTarget != EXT_MAGICATKSHOT )
				if( pSkillProp->dwSfxObj2 != NULL_ID )
					g_UserMng.AddCreateSfxObj( this, pSkillProp->dwSfxObj2 );
			
			if( pSkillProp->nEvildoing < 0 )
				return;
		}
	}
#endif //defined(__WORLDSERVER)

	int nProb	= pSkillProp->nProbability;
	if( pAddSkillProp )
	{
		nProb	= pAddSkillProp->nProbability;
#if __VER >= 9	// __SKILL_0706
		BOOL bPVP	= ( pSrc->GetType() == OT_MOVER && ( (CMover *)pSrc )->IsPlayer() && GetType() == OT_MOVER && ( (CMover *)this )->IsPlayer() );
		if( bPVP )
			nProb	= pAddSkillProp->nProbabilityPVP;
#endif	// __SKILL_0706
	}

	if( bIgnoreProb )	// Ȯ�����ÿ��� ������ ������ 100%
		nProb = NULL_ID;
	
	if( nProb == NULL_ID || (int)( xRandom(100) ) < nProb  )		// ���� Ȯ��, =�� �Ǿ� ������ 100% ����
	{
		if( GetType() == OT_MOVER )		// Ÿ���� �����϶��� ���ӽð� ó��
		{
			int nSkillTime = 0;
			if( pAddSkillProp )		
				nSkillTime = (int)pAddSkillProp->dwSkillTime;
			else					
				nSkillTime = (int)pSkillProp->dwSkillTime;

			BOOL bNew = TRUE;	// ��ų�� ÷ ����ƴ°�.
#if __VER >= 8 //__Y_FLAG_SKILL_BUFF
			if( nSkillTime >= 0 )	// ���ӽð��� �ִ� ��ų(�����ؼ� ��)
#else //__Y_FLAG_SKILL_BUFF
			if( nSkillTime > 0 )	// ���ӽð��� �ִ� ��ų(�����ؼ� ��)					
#endif //__Y_FLAG_SKILL_BUFF
			{
				DWORD dwTime1 = 0, dwTime2 = 0;
				if( pSkillProp->dwReferTarget1 == RT_TIME || pSkillProp->dwReferTarget2 == RT_TIME )
				{
					if( pSrc->GetType() == OT_MOVER )		// �����ڰ� �����϶���.
						((CMover *)pSrc)->SubReferTime( &dwTime1, &dwTime2, pSkillProp, pAddSkillProp );
				}

				WORD wType;
				if( pSkillProp->dwSkillType == NULL_ID )	
					wType = BUFF_ITEM;
				else										
					wType = BUFF_SKILL;

				// pTarget�� this�� �ɼ����ְ� Ÿ���� �ɼ��� �ִ�.
			#if defined(__WORLDSERVER)
				int nLevel = 0;
				if( pAddSkillProp )		
					nLevel = pAddSkillProp->dwSkillLvl;
				else					
					nLevel = 0;

				// dwSkillTime�� ��ų��ü�� ���ӽð����ε� ���̰� �ΰ�ȿ���� ���ӽð����ε� ���̱⶧���� �̷������� ���� �ʴ�
				// �̴�ζ�� �����ʵ嵵 ����ó�� �ɷ�������.
				// bNew = ((CMover *)this)->m_SkillState.Set( wType, pSkillProp->dwID, nLevel, nSkillTime + (int)dwTime1 + (int)dwTime2 );	
				if( pSkillProp->dwID == SI_ELE_FIRE_BURINGFIELD )
					bNew = FALSE;	//  �����ʵ�� ����ó�� 
				else
#ifdef __PVPDEBUFSKILL
					bNew	= static_cast<CMover*>( this )->AddBuff( wType, (WORD)( pSkillProp->dwID ), nLevel, nSkillTime + (int)dwTime1 + (int)dwTime2, pSrc->GetId() );
#else // __PVPDEBUFSKILL
					bNew = ((CMover *)this)->m_SkillState.Set( wType, pSkillProp->dwID, nLevel, nSkillTime + (int)dwTime1 + (int)dwTime2 );	
#endif // __PVPDEBUFSKILL

				
			#elif defined(__CLIENT)
				bNew = ((CMover *)this)->HasBuff( wType, (WORD)( pSkillProp->dwID ) ) ? FALSE : TRUE;
			#endif // Client
			}
			
			// ���� ����� ��ų����
			if( bNew )	// �̹� �ɷ��ִ� ���¿��� �� ���� bNew�� FALSE�� �ȴ�.
			{
			#ifdef __WORLDSERVER
				// ��ų�� ����ó�� �ؾ��Ұ��� ó����.
				if( ApplySkillHardCoding( pSrc, pSkillProp, pAddSkillProp ) == FALSE )
					return;

				// ��Ȱ��ų������ �Ǿ��ִٸ� �Ʒ��͵� ����
				if( ((CMover *)this)->m_Resurrection_Data.bUseing )
					return;

				// �������� �ָ鼭 �Ķ���͵� ���ϴ� ��ų�� �ֱⶫ�� �������� ���� ó��.
				switch( pSkillProp->dwExeTarget )
				{
				case EXT_SELFCHGPARAMET:		// ������ �ڽ�.
					if( pSrc->GetType() == OT_MOVER )	// Ÿ��(���⼱ Ÿ���� ������)�� �����϶���...
						((CMover *)pSrc)->ApplyParam( (CMover *)pSrc, pSkillProp, pAddSkillProp, TRUE, nDamage );
					break;
				case EXT_OBJCHGPARAMET:
				case EXT_MAGIC:
				case EXT_ANOTHER:
				case EXT_ANOTHERWITH:
				default:
					if( this->GetType() == OT_MOVER )	// Ÿ���� �����϶���.
						((CMover *)this)->ApplyParam( (CMover *)pSrc, pSkillProp, pAddSkillProp, TRUE , nDamage );
					break;
				}

				if( this->GetType() == OT_MOVER )
				{
					// �и��� ���� �ִ°�?
					CMover *pTarget = (CMover *)this;
					DWORD dwDmgShift;
					if( pAddSkillProp )
						dwDmgShift = pAddSkillProp->dwDmgShift;
					else
						dwDmgShift = pSkillProp->dwDmgShift;

					if( dwDmgShift != NULL_ID && pTarget->GetProp()->dwClass != RANK_SUPER && pTarget->GetProp()->dwClass != RANK_MIDBOSS )
					{
						FLOAT fAngle = GetDegree( pTarget->GetPos(), pSrc->GetPos() );	// �����ڿ� Ÿ���� ������ ����
						AngleToVectorXZ( &pTarget->m_pActMover->m_vDeltaE, fAngle, 0.85f );
						g_UserMng.AddPushPower( pTarget, pTarget->GetPos(), pTarget->GetAngle(), fAngle, 0.85f );
					}
					
					// ������ �������
					if( pSkillProp->dwID == SI_ACR_YOYO_PULLING )
					{
						if( pAddSkillProp && pAddSkillProp->nDestData1[2] != NULL_ID )
						{
							// ����� �Ÿ�
							FLOAT fPullingLen = (FLOAT)pAddSkillProp->nDestData1[2] * 0.001f; 
							
							// Ÿ�ٰ��� �Ÿ�
							D3DXVECTOR3 v3Len = pTarget->GetPos() - pSrc->GetPos();
							FLOAT fTotalLen = D3DXVec3LengthSq( &v3Len ) * 0.1f;
							{
								if( fTotalLen > ((CMover*)this)->GetAttackRange( AR_HRANGE ) )
									fTotalLen = ((CMover*)this)->GetAttackRange( AR_HRANGE );

								// �Ÿ��� ���� ������� ���� �ٸ��� �����
								FLOAT fDest = fTotalLen / ((CMover*)this)->GetAttackRange( AR_HRANGE );
								{
									// ���������� �Ÿ� ���
									FLOAT fAngle = GetDegree( pTarget->GetPos(), pSrc->GetPos() );	// �����ڿ� Ÿ���� ������ ����
									AngleToVectorXZ( &pTarget->m_pActMover->m_vDeltaE, fAngle, fPullingLen * -fDest);
									g_UserMng.AddPushPower( pTarget, pTarget->GetPos(), pTarget->GetAngle(), fAngle, fPullingLen * -fDest );
								}
							}
						}
					}
				}
			#endif // __WORLDSERVER
			} // if( bNew )

		#ifdef __WORLDSERVER
			if( pSkillProp->dwID == SI_GEN_FLASH && GetType() == OT_MOVER && ((CMover *)this)->IsPlayer() )
			{
				((CUser *)this)->AddCommonSkill( pSkillProp->dwID, pAddSkillProp->dwSkillLvl );
			}
		#endif // __WORLDSERVER
		
		}	// this == OT_MOVER
	}	// if( nProb == NULL_ID || xRandom(100) < nProb  )
#ifdef __WORLDSERVER
	#if __VER >= 11 // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
	else
	{
		if(pSkillProp->dwID == SI_PSY_HERO_STONE )
			g_UserMng.AddRemoveSfxObj( this, pSkillProp->dwSfxObj4 );
	}
	#endif // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
#endif	//__WORLDSERVER
	#ifdef __WORLDSERVER
		switch( pSkillProp->dwSpellRegion )
		{
#ifdef __YDEBUG
		case SRO_DIRECT:
#endif //__YDEBUG
		case SRO_AROUND:
		case SRO_REGION:	// ���������϶� ���⼭ Ÿ�ٵ� ������ sfxobj3�� �ѷ���.
			if( pSkillProp->dwExeTarget != EXT_MAGICATKSHOT )
				if( pSkillProp->dwSfxObj2 != NULL_ID )
					g_UserMng.AddCreateSfxObj( this, pSkillProp->dwSfxObj2 );
			break;
		}

		// ����ó��: ��Ż��ų - ��ĸ� ��ġ�鼭 �������� �ش�
		if( pSkillProp->dwID == SI_ACR_YOYO_SNITCH )
		{
			// pSrc - ������ �÷��̾�, this - �ǰ����� ���� 
			// �ǰ����� �ܾ��� 0�� �ƴϸ� ��Ż �Ѵ�.
			CUser* pUser = (CUser*)pSrc;
			CUser* pDest = (CUser*)this;
			if( pDest->GetType() == OT_MOVER && pDest->GetGold() > 0 )
			{
				int nPlus = (pDest->GetLevel()*20) - (pUser->GetLevel()*10);
				
				if( pUser->GetLevel() <= 20 )
				{
				}
				else
				if( pUser->GetLevel() <= 30 )
				{
					nPlus	= (int)( nPlus * 0.7f );
				}
				else
				if( pUser->GetLevel() <= 40 )
				{
					nPlus	= (int)( nPlus * 0.5f );
				}
				else
				{
					nPlus	= (int)( nPlus * 0.3f );
				}
		
				if( nPlus > 0 )
				{				
					nPlus = min( nPlus, pDest->GetGold() );		// �ǰ����� �� ���ٴ� ���� �ʰ� 
					pUser->AddGold( nPlus );
					pDest->AddGold( -nPlus );

					pUser->AddGoldText( nPlus );
				}
			}		
		}
	#endif	//__WORLDSERVER
} // ApplySkill


// �߻������� ��ų�� �߻�ü�� �����ϴ� �κ�
// �� �߻�ü�� Ÿ�ٿ��� �¾����� DoApplySkill()�� �ߵ��ȴ�.
int		CCtrl::ShootSkill( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
#ifdef __CLIENT
	// �߻����°� �ƴϸ� ����
	if( pSkillProp->dwExeTarget != EXT_MAGICATKSHOT 
		&& pSkillProp->dwExeTarget != EXT_MAGICSHOT 
		)	// �߻�ü sfx ����.
		return 0;

	DWORD dwShootObj;
	{
		dwShootObj = pSkillProp->dwSfxObj2;
	}

	if( dwShootObj != NULL_ID )
	{
		D3DXVECTOR3 vPos = GetPos();
		
		if( ((CMover*)this)->IsPlayer() )		
		{
			// ���쿡�� ������ ȭ���� ��� ������ ������
			if( pSkillProp->dwLinkKind == IK3_BOW )
			{
				// ȭ���� �޼տ� ����...
				CModelObject *pModel = (CModelObject *)m_pModel;
				pModel->GetHandPos( &vPos, PARTS_LWEAPON, GetMatrixWorld() );		// �ָ� ������ǥ ����		
				vPos.y -=	1.0f;
			}
		}
		
		CSfxShoot *pShootSfx = CreateShootSfx( D3DDEVICE, dwShootObj, vPos, GetId(), D3DXVECTOR3(0,0,0), pTarget->GetId() );
		if( pShootSfx )
		{		
			D3DXVECTOR3 vPos = GetPos();
			PLAYSND( pSkillProp->dwSndAttack1, &vPos );		// �߻� ȿ����.
			
			if( GetType() == OT_MOVER )
			{
				if( ((CMover *)this)->IsActiveMover() )		// g_pPlayer�� �򶧸� �����ؼ� ����.
				{
					int idSfxHit = ++(((CMover *)this)->m_idSfxHit);	// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
					int		nMaxDmgCnt = 1;
					if( pSkillProp->tmContinuousPain != NULL_ID )		// ���ӽ�ų�� �ִ� ������ ���¸�
						nMaxDmgCnt = (pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime) + 1;		// ��� �������� �Գ�.
					
					if( pSkillProp->dwSkillType == KT_SKILL )
						g_DPlay.SendSfxID( pTarget->GetId(), idSfxHit, AF_MELEESKILL, NULL_ID, nMaxDmgCnt );
					else
						g_DPlay.SendSfxID( pTarget->GetId(), idSfxHit, AF_MAGICSKILL, NULL_ID, nMaxDmgCnt );
					pShootSfx->m_idSfxHit	= idSfxHit;
				}
			}
			pShootSfx->SetSkill( pSkillProp->dwID );
			pShootSfx->m_nMagicPower	= pAddSkillProp->dwSkillLvl;	// ��ų������ MagicPower������ ���̽ᵵ �ɰ� ����.
			pShootSfx->m_dwSfxHit = pSkillProp->dwSfxObj3;	// ���� ����Ʈ ���.

		}
		// ���ӽð����� ����Ʈ�� �����Ǿ�� �ϴ°��� CSkillInfluence::Process()���� ó���Ѵ�.
	} else
		Error( "ShootSkill MAGICATKSHOT : %s dwSfxObj2�� �������� ����", pSkillProp->szName );
#endif // CLIENT
	return 1;
}


// ��������� Ÿ���� ������ ���� �ֺ� Ÿ���� ������ ������.
int		CCtrl::TargetSelecter( CCtrl *pTarget )
{
	int nApplyType = 0;
	CCtrl *pSrc = this;

	if( pTarget->GetType() == OT_MOVER )	// Ÿ���� �����϶�
	{
		nApplyType = OBJTYPE_PLAYER | OBJTYPE_MONSTER;	// ���� �÷��̾��/���Ϳ��� ����
	}
	else
	{
		// Ÿ���� ��Ʈ���϶�
		if( pSrc->GetType() == OT_MOVER )	// �����ڰ� ������
		{
			if( ((CMover *)pSrc)->IsPlayer() )		// �����ڰ� �÷��̾��
				nApplyType = OBJTYPE_PLAYER | OBJTYPE_CTRL | OBJTYPE_MONSTER;	// ���� ���Ϳ� ��Ʈ�ѿ��� ����.
		}
		else
		{	// �����ڰ� ��Ʈ���̳�
			nApplyType = OBJTYPE_PLAYER;	// �÷��̾�� ����
		}
	}

	return nApplyType;
}

#if __VER >= 8 // __S8_PK
int		CCtrl::DoApplySkill( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, int nParam, BOOL bOnlyDmg, BOOL bControl )
#else // __VER >= 8 // __S8_PK
int		CCtrl::DoApplySkill( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, int nParam, BOOL bOnlyDmg )
#endif // __VER >= 8 // __S8_PK
{
#if __VER >= 8 // __S8_PK
	int nResult = DoApplySkillEx( pTarget, pSkillProp, pAddSkillProp, bIgnoreProb, nParam, bOnlyDmg, bControl );
#else // __VER >= 8 // __S8_PK
	int nResult = DoApplySkillEx( pTarget, pSkillProp, pAddSkillProp, bIgnoreProb, nParam, bOnlyDmg );
#endif // __VER >= 8 // __S8_PK
	if( nResult == 1 )
	{
	#if defined(__WORLDSERVER)	
		if( this != pTarget && GetType() == OT_MOVER && pTarget->GetType() == OT_MOVER )
		{
			if( pSkillProp->nEvildoing < 0 )	// ���۸����� ���� ���ݰ� ���ٰ� �����Ѵ�. 
				((CMover*)pTarget)->OnAttacked( (CMover*)this, 0, TRUE, 0 );	// TRUE�� Ÿ���̴�.
		}
	#endif 
	}

	return nResult;
}

// 
// ������ ��ų�� ȿ���� pTarget���� �����Ѵ�.
// ���������� ���� pTarget�� �߽����� �ٸ� target���� ����ȴ�.
// �̰��� ����ϴ� ��ü�� ����� �ƴҼ��� �ִ�
// ���⼭�� SpellRegion�� ���� �������� �з��� �ϰ� ���� ȿ��������
// ApplySkill���� ó���Ѵ�.
// bOnlyDmg : ���� �����������Ű�°͸� �Ѵ�.  ���� ���ӵ�������ü���� �����.
#if __VER >= 8 // __S8_PK
int		CCtrl::DoApplySkillEx( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, int nParam, BOOL bOnlyDmg, BOOL bControl )
#else // __VER >= 8 // __S8_PK
int		CCtrl::DoApplySkillEx( CCtrl *pTarget, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProb, int nParam, BOOL bOnlyDmg )
#endif // __VER >= 8 // __S8_PK
{
	int		nRet = 1;
	CCtrl	*pSrc = this;

	switch( pSkillProp->dwSpellRegion )
	{
	case SRO_AROUND:	// ��� 0% ���� 100%
	case SRO_REGION:	// ��� 100% ���� 100%
		{
#if __VER >= 10 //  __LEGEND
			if( pSkillProp->dwExeTarget != EXT_TROUPE && pSkillProp->dwExeTarget != EXT_TROUPEWITH )	// �شܴ���� �ƴҶ�.
#else // __VER >= 10 //  __LEGEND
			if( pSkillProp->dwExeTarget != EXT_TROUPE )	// �شܴ���� �ƴҶ�.
#endif // __VER >= 10 // __LEGEND
			{
				int nApplyType = 0;
				nApplyType = pSrc->TargetSelecter( pTarget );
#if __VER >= 8 // __S8_PK
				pTarget->ApplySkillAround( this, nApplyType, pSkillProp, pAddSkillProp, bIgnoreProb, bOnlyDmg, 0.0f, bControl );
#else // __VER >= 8 // __S8_PK
				pTarget->ApplySkillAround( this, nApplyType, pSkillProp, pAddSkillProp, bIgnoreProb, bOnlyDmg );
#endif // __VER >= 8 // __S8_PK
			}
#if __VER >= 10 //  __LEGEND
			else if( pSkillProp->dwExeTarget == EXT_TROUPEWITH )	// �شܴ���� �ƴҶ�
			{
				// �ش� ��� ���� ȿ��.
				if( GetType() == OT_MOVER )
				{
					if( ((CMover *)this)->IsPlayer() )
					{
						if( pAddSkillProp )
						{
							int ntmpProb	= pAddSkillProp->nProbability;
							if( (int)( xRandom(100) ) < ntmpProb )
							{
								if( ApplySkillAroundTroupe( ((CMover *)this)->m_idparty, pSkillProp, pAddSkillProp, TRUE ) == FALSE )// ��� �������� ȿ�� ����.
								{
									pSrc->ApplySkill( this, pSkillProp, pAddSkillProp, TRUE );	
								}
							}
						}
					}
				}
			}			
#endif // __VER >= 10 // __LEGEND
			else
			{
				// �ش� ��� ���� ȿ��.
				if( GetType() == OT_MOVER )
				{
					if( ((CMover *)this)->IsPlayer() )
					{
						if( ApplySkillAroundTroupe( ((CMover *)this)->m_idparty, pSkillProp, pAddSkillProp, bIgnoreProb ) == FALSE )
							return 0;  // ������.
					}
				}
			}
#ifdef __WORLDSERVER
			// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
			if( pSkillProp->tmContinuousPain != NULL_ID && !(((CMover*)pTarget)->IsPlayer() && ((CMover*)pTarget)->m_nDead) )	// ���ӵ����� ���³�?
			//if( pSkillProp->tmContinuousPain != NULL_ID )
			{
				CContDamageCtrl *pDmgCtrl = new CContDamageCtrl;	// ���ӵ������� �ִ� ���������Ʈ ���� - �޸�Ǯ�� ó�� �Ұ�
				if( pDmgCtrl )
				{
					pDmgCtrl->m_pSkillProp = pSkillProp;
					pDmgCtrl->m_pAddSkillProp = pAddSkillProp;
					pDmgCtrl->m_idSrc = pSrc->GetId();
					pDmgCtrl->m_idTarget = pTarget->GetId();
					pDmgCtrl->SetPos( pTarget->GetPos() );		// �߽��� �Ǵ� Ÿ���� ��ǥ��...
#if __VER >= 8 // __S8_PK
					pDmgCtrl->m_bControl = bControl;
#endif // __VER >= 8 // __S8_PK
					// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
					if( ((CMover*)pSrc)->IsPVPTarget( ((CMover*)pTarget) ) )
						pDmgCtrl->m_bDuelTarget = TRUE;

					GetWorld()->ADDOBJ( pDmgCtrl, FALSE, GetLayer() );
				}
			}
#endif
		}
		break;
	case SRO_LINE:		// this�� �߽����� m_fAngle����.
		{
			int nApplyType = 0;
			nApplyType = pSrc->TargetSelecter( pTarget );
#if __VER >= 8 // __S8_PK
			ApplySkillLine( nApplyType, pSkillProp, pAddSkillProp, 2.5f, 2.5f, (float)pAddSkillProp->dwSkillRange, bIgnoreProb, bControl );
#else // __VER >= 8 // __S8_PK
			ApplySkillLine( nApplyType, pSkillProp, pAddSkillProp, 2.5f, 2.5f, (float)pAddSkillProp->dwSkillRange, bIgnoreProb );
#endif // __VER >= 8 // __S8_PK
		}
		break;
	case SRO_DIRECT:	// ��� �ϳ����� ����
	default:
		if( pSkillProp->dwID != SI_PSY_PSY_PSYCHICWALL )		// ����ű���� ó�� ����.
			pTarget->ApplySkill( this, pSkillProp, pAddSkillProp, bIgnoreProb, nParam );
		break;
	} 

#ifdef __WORLDSERVER		
	// ����ű���� ���� ����.
	if( pSkillProp->dwID == SI_PSY_PSY_PSYCHICWALL )
	{
		BOOL bCreate = FALSE;
		int i = 0;
		if( GetType() == OT_MOVER )
		{
			if( ((CMover *)this)->IsPlayer() )
			{
				CUser *pUser = (CUser *)this;
				for( i = 0; i < 2; i ++ )		// �ѹ��� 2���̻� �������� �ʰ���.
				{
					if( pUser->m_pWall[i] == NULL )
					{
						CObj *pObj = CreateObj( D3DDEVICE, OT_CTRL, CI_PSYCHICWALL );		// �� ��Ʈ�� ����.
						if( pObj )
						{
							pObj->SetPos( this->GetPos() );
							pObj->SetAngle( -GetAngle() );
							((CCommonCtrl *)pObj)->m_pSkillProp = pSkillProp;
							((CCommonCtrl *)pObj)->m_pAddSkillProp = pAddSkillProp;
							((CCommonCtrl *)pObj)->m_idAttacker = GetId();
							((CCommonCtrl *)pObj)->m_bControl = bControl;

							GetWorld()->ADDOBJ( pObj, TRUE, GetLayer() );
							pUser->m_pWall[i] = pObj;
							bCreate = TRUE;
							break;
						}
					}
				}
			}
		}
		if( bCreate == FALSE )	// ������������
			nRet = 0;			// ���� ����

	}


	if( pSrc->GetType() == OT_MOVER )
	{
		// ��� ��ų���� ��ũ�Ϸ��������̸� �����Ѵ�!!~
		if( pSkillProp->dwID != SI_ACR_SUP_DARKILLUSION )
		{
			if( ((CMover *)pSrc)->HasBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION ) )
			{
				((CMover *)pSrc)->SetDarkCover( FALSE );
				((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION );		// ����.
			}
		}

		// ������ ������ Ư�� ������ ������ �����ϰ� �ɾ����
		if( pSkillProp->dwID == SI_JST_SUP_POISON )
		{
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_BLEEDING );
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_ABSORB );
		}
		else
		if( pSkillProp->dwID == SI_JST_SUP_BLEEDING )
		{
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_POISON );
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_ABSORB );
		}
		else		
		if( pSkillProp->dwID == SI_JST_SUP_ABSORB )
		{
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_BLEEDING );
			((CMover *)pSrc)->RemoveBuff( BUFF_SKILL, SI_JST_SUP_POISON );
		}

	}	
#endif // WorldServer
	
	// ��ų�� �ʿ��� sfx�� ����.
	CreateSkillSfx( pTarget, pSkillProp, pAddSkillProp );
	
	// 2�� ��ų �ߵ�. ActiveSkill
	if( pSkillProp->dwActiveSkill != NULL_ID )
	{
		if( pSkillProp->dwActiveSkillRate == NULL_ID || xRandom(100) <= pSkillProp->dwActiveSkillRate )	// �ߵ�Ȯ���� �ɷȴ°�.
#if __VER >= 8 // __S8_PK
			pSrc->DoActiveSkill( pSkillProp->dwActiveSkill, 1, pTarget, false, bControl );		// dwActiveSkill Lv1�� �ߵ���.
#else // __VER >= 8 // __S8_PK
			pSrc->DoActiveSkill( pSkillProp->dwActiveSkill, 1, pTarget );		// dwActiveSkill Lv1�� �ߵ���.
#endif // __VER >= 8 // __S8_PK
			
	}

	return nRet;
}

// nLevel�� dwSkill�� pTarget�� ���Ͽ� �ߵ���.  ����Ŀ�� this
// bIgnoreProb�� TRUE�̸� ���ο��� Ȯ������� �����ϰ� 100% ����ǰ� �Ѵ�.
#if __VER >= 8 // __S8_PK
void CCtrl::DoActiveSkill( DWORD dwSkill, int nLevel, CCtrl *pTarget, bool bIgnoreProb, BOOL bControl )
#else // __VER >= 8 // __S8_PK
void CCtrl::DoActiveSkill( DWORD dwSkill, int nLevel, CCtrl *pTarget, bool bIgnoreProb )
#endif // __VER >= 8 // __S8_PK
{
	ItemProp *pSkillProp;
	AddSkillProp *pAddSkillProp;
	GetSkillProp( &pSkillProp, &pAddSkillProp, dwSkill, nLevel, "DoActiveSkill" );
	
	int nRet;
	if( pSkillProp->dwExeTarget == EXT_MAGICATKSHOT || 
		pSkillProp->dwExeTarget == EXT_MAGICSHOT )		// ��� ���¸� sfx�� �߻�
		nRet = ShootSkill( pTarget, pSkillProp, pAddSkillProp );
	else
	{
#if __VER >= 8 // __S8_PK
		nRet = DoApplySkill( pTarget, pSkillProp, pAddSkillProp, bIgnoreProb, 0, FALSE, bControl );
#else // __VER >= 8 // __S8_PK
		nRet = DoApplySkill( pTarget, pSkillProp, pAddSkillProp, bIgnoreProb );
#endif // __VER >= 8 // __S8_PK
 #ifdef __WORLDSERVER		
		g_UserMng.AddDoApplySkill( this, pTarget->GetId(), dwSkill, nLevel );
 #endif
	}
}


