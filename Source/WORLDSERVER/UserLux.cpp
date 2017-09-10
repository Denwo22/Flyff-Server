#include "stdafx.h"
#include "defineText.h"
#include "User.h"
#include "DPDatabaseClient.h"
#include "DPSrvr.h"
#include "WorldMng.h"

extern	CUserMng	g_UserMng;

#ifndef __MEM_TRACE
#ifdef _DEBUG
#define new new( __FILE__, __LINE__ )
#endif
#endif	// __MEM_TRACE

#include "UserMacro.h"

extern	CDPSrvr		g_DPSrvr;
extern	CWorldMng	g_WorldMng;


/*  
//
// �ڿ� ä�� ����
//
BOOL CUser::DoCollect( CMover *pTarget )
{
	BOOL bAble = TRUE;

	if( pTarget->m_idCollecter != NULL_ID )	// pTarget�� ������ �ִ� ���̰� 
	{
		CMover *pCollecter = prj.GetMover( pTarget->m_idCollecter );
		if( IsValidObj(pCollecter) )
		{
			if( m_idparty )		// ä���Ϸ��� ����� ��Ƽ�� �ִ°�?
			{
				if( m_idparty != pCollecter->m_idparty )		// �� ���ΰ� ���� ��Ƽ�� �ƴϴ�.
					if( pTarget->m_nCollectOwnCnt > 0 )			// ���� �ð��� ��Ǯ�ȴ°�?
						bAble = FALSE;		// ������.
			} else
			// ��Ƽ�� ������
			{
				if( pTarget->m_idCollecter != GetId() )			// �� ������ ���� �ƴϳ�?
					if( pTarget->m_nCollectOwnCnt > 0 )			// ���� �ð��� ��Ǯ�ȴ°�?
						bAble = FALSE;		// ������.
			}
		}
//		else
			// ������ ��������� �� ���� �� ����.
//
//		if( pTarget->m_idCollecter != GetId() )	// �� ������ ���� �ƴϰ�
//		{
//			CMover *pCollecter = prj.GetMover( pTarget->m_idCollecter );
//			if( IsValidObj( pCollecter ) )
//			{
//				if( m_idparty && m_idparty != pCollecter->m_idparty )	// ���� ��Ƽ���� �ƴϰ�
//				{
//					if( pTarget->m_nCollectOwnCnt > 0 ) // ���� �ð��� �� Ǯ������
//					{
//						AddDefinedText( TID_COLLECT_ERROROTHER );
//						return FALSE;				// pTarget�� this�� �� �Դ´�.
//					}
//				}
//			}
//		}

	}

	// ä���� ������ �ʴ� ��Ȳ�̸�
	if( bAble == FALSE )
	{
		AddDefinedText( TID_COLLECT_ERROROTHER );		// �����޽��� ����ϰ�
		return FALSE;				// pTarget�� this�� �� �Դ´�.
	}
	

	ItemProp *pHandProp = GetActiveHandItemProp();
	if( pHandProp->dwItemKind3 != IK3_COLLECTER )
		return FALSE;
				
	if( CMover::DoCollect( pTarget ) == TRUE )
	{
		m_idTargetCollect = pTarget->GetId();	// pTarget�� ä�����̶�°��� ǥ����.
//		pTarget->m_idCollecter = GetId();		// Ÿ�ٿ��� ������� ǥ����.
		m_nCollect = 0;
		m_tmCollect = g_tmCurrent;
	} else
		return FALSE;
	
	return TRUE;
}
*/

#ifdef __XUZHU
int g_nCC = 0;
#endif

/*
//
//
// this�� pTarget�� ä���Ѵ�.
int		CUser::OnActCollecting( void )
{
	CMover *pTarget = prj.GetMover( m_idTargetCollect );		// ä������� ������ ����.
	if( IsInvalidObj( pTarget ) )	// ä���� �ð��� �ٵǼ� ������Ʈ�� ������ų� �Žñ��� ������Ʈ�� �� ����.
		return TRUE;		// ä�� ���� �����϶�.

	// ä�� ����� ����Ǵµ��� ��� ���´�.
	if( g_tmCurrent > m_tmCollect + 1000.0f )
	{
		m_tmCollect = g_tmCurrent;
		ItemProp *pHandProp = GetActiveHandItemProp();
		if( pHandProp == NULL )
		{
			Error( "CUser::OnActCollecting : pHandProp==NULL" );
			return TRUE;
		}
		if( pHandProp->dwItemKind3 != IK3_COLLECTER )		// ä���� ������ ���ָ� ä�� ���.
			return TRUE;
		
		m_nCollect += (int)pHandProp->dwLoadingTime;				// �������� ä���ɷ¿� ���� ä������ ����.
		pTarget->m_nResource -= (int)pHandProp->dwLoadingTime;		// ä���� ��ŭ Ÿ���� ���ҽ����� ��.
		if( pTarget->m_nResource < 0 )
			pTarget->m_nResource = 0;
		if( m_nCollect >= pHandProp->nShellQuantity )	// ä������ �ƽ�ġ�� �Ѿ��.
		{
			//������ 1�� ����;
			BYTE bID;
			CItemElem itemElem;
			itemElem.m_dwItemId	= pTarget->GetProp()->dwSourceMaterial;		// �ڿ� ����.
			itemElem.m_nItemNum		= 1;
			if( itemElem.m_dwItemId != NULL_ID )
			{
				BOOL bRet = CreateItem( &itemElem, &bID );
				if( bRet == TRUE )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "C";
					aLogItem.SendName = pTarget->GetName();
					aLogItem.RecvName = GetName();
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = GetGold();
					aLogItem.Gold2 = GetGold();
					aLogItem.ItemNo = itemElem.GetSerialNumber();
					aLogItem.ItemName = itemElem.GetProp()->szName;
					aLogItem.itemNumber = 1;
					g_DPSrvr.OnLogItem( aLogItem );
				}
			} else
				Error( "CUser::OnActCollecting : %s�� dwSourceMaterial�� -1", pTarget->GetName() );

			FLOAT fResRatio = (float)pTarget->m_nResource / pTarget->GetProp()->dwMaterialAmount;		// ���ۼ�Ʈ ���Ҵ��� ...
			FLOAT fScale = pTarget->m_pModel->m_pModelElem->m_fScale;		// mdlDyna�� �ִ� scale���� 100%
			if( fResRatio > 1.0f )
			{
				Error( "OnActCollecting : %f %d %d", fResRatio, pTarget->m_nResource, (int)pTarget->GetProp()->dwMaterialAmount );
				fResRatio = 1.0f;
			}
			fScale *= fResRatio;
			D3DXVECTOR3 vScale = D3DXVECTOR3( fScale, fScale, fScale );
			pTarget->SetScale( vScale );
			if( fScale > 10.0f )
				Error( "OnActCollecting %s %f %f %d %d", GetName(), fScale, 
														pTarget->m_pModel->m_pModelElem->m_fScale, pTarget->m_nResource, pTarget->GetProp()->dwMaterialAmount );
			
			g_UserMng.AddSetScale( pTarget, fResRatio );
			
#ifdef __XUZHU
			g_nCC ++;
			TRACE( "item Create: %d\n", g_nCC );
#endif

			m_nCollect -= pHandProp->nShellQuantity;	// �����Ȱ� ������.
		}
		if( pTarget->m_nResource <= 0 )							// Ÿ���� ���ҽ��� �ٶ���������.
		{
			// ä����
#ifdef __XUZHU
			g_nCC = 0;
#endif
			pTarget->m_nResource = 0;							// 0���θ� �صΰ� Ÿ���� �������°� ���ʿ��� �˾Ƽ� �Ѵ�.
			AddDefinedText( TID_COLLECT_EMPTY );	// ä���� �������ϴ�
			return TRUE;	// ä������ �����϶�.
		}
#ifdef __XUZHU
		TRACE( "%d %d %d\n", m_tmCollect, pTarget->m_nResource, m_nCollect );
#endif
			
	}
	return 0;
}
*/

// tmMaxEscape : Ż���� ��Ÿ��
void CUser::AddEscape( DWORD tmMaxEscape )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_DO_ESCAPE;
	m_Snapshot.ar << tmMaxEscape;
}

// �׼�����Ʈ ����.
void CUser::AddSetActionPoint( int nAP )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_SETACTIONPOINT;
	m_Snapshot.ar << nAP;
}

void CUser::AddSetTarget( OBJID idTarget )
{
	if( IsDelete() )	return;
	
	m_Snapshot.cb++;
	m_Snapshot.ar << NULL_ID;
	m_Snapshot.ar << SNAPSHOTTYPE_SETTARGET;
	m_Snapshot.ar << idTarget;
}

// ----------------------------------------------------------------------------------
//
//   CUserMng Lux
//
// ----------------------------------------------------------------------------------
/*
void	CUserMng::AddDoCollect( CUser* pUser, OBJID idTarget )
{
	CAr ar;
	ar << GETID( pUser ) << SNAPSHOTTYPE_DO_COLLECT;
	ar << idTarget;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pUser )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pUser )
}
*/

void	CUserMng::AddCreateSkillEffect( CMover *pAttacker, OBJID idTarget, DWORD dwSkill, DWORD dwLevel )
{
	CAr ar;
	ar << GETID( pAttacker ) << SNAPSHOTTYPE_ACTIVESKILL;
	ar << idTarget << dwSkill << dwLevel;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pAttacker )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pAttacker )
}

// pMover�� ���� �Ȱ��� ���� ����Ŭ�� �˸�.
void	CUserMng::AddSetStun( CMover *pMover, int nMaxTime )
{
	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_SETSTUN;
	ar << nMaxTime;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pMover )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pMover )
}

void	CUserMng::AddDoApplySkill( CCtrl *pCtrl, OBJID idTarget, DWORD dwSkill, DWORD dwLevel )
{
	CAr ar;
	ar << GETID( pCtrl ) << SNAPSHOTTYPE_DOAPPLYUSESKILL;
	ar << idTarget << dwSkill << dwLevel;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pCtrl )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pCtrl )
}

// SendActMsg�� ���� �����鿡�� ���� .
void	CUserMng::AddSendActMsg( CMover *pMover, OBJMSG dwMsg, int nParam1, int nParam2, int nParam3 )
{
	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_SENDACTMSG;
	ar << (int)dwMsg;
	ar << pMover->IsFly();
	ar << nParam1 << nParam2 << nParam3;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pMover )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pMover )
}

// Mover�� ����������� �и��� �ϴ�.
// pMover : �и��� ����
// vPos : �и��� ���������� vPos - ��Ȯ�� ����ȭ������ ��ǥ�� �������� �ʿ��ϴ�.
// fAngle : �и��� ���������� Angle - 
// fPushAngle : �̴� ����.
// fPower : �̴� ��.
void	CUserMng::AddPushPower( CMover *pMover, D3DXVECTOR3 vPos, FLOAT fAngle, FLOAT fPushAngle, FLOAT fPower )
{
	if( (pMover->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) || pMover->m_pActMover->GetState() & OBJSTA_STUN ) // ������ �ö������̸� ����
		return;

	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_PUSHPOWER;
	ar << vPos;
	ar << fAngle;
	ar << fPushAngle << fPower;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pMover )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pMover )
}

void	CUserMng::AddRemoveSkillInfluence( CMover *pMover, WORD wType, WORD wID )
{
	CAr ar;
	ar << GETID( pMover ) << SNAPSHOTTYPE_REMOVESKILLINFULENCE;
	ar << wType;
	ar << wID;
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pMover )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pMover )
}

void CUserMng::AddSetPosAngle( CCtrl* pCtrl, const D3DXVECTOR3 &vPos, FLOAT fAngle )
{
	CAr ar;
	
	ar << GETID( pCtrl ) << SNAPSHOTTYPE_SETPOSANGLE;
	ar << vPos << fAngle;
	
	GETBLOCK( ar, lpBuf, nBufSize );
	
	FOR_VISIBILITYRANGE( pCtrl )
		USERPTR->AddBlock( lpBuf, nBufSize );
	NEXT_VISIBILITYRANGE( pCtrl )
}

/*
void CUser::AddRemoveSkillInfluence( WORD wType, WORD wID )
{
	if( IsDelete() )	return;
	
	m_Snapshot.Lock( theLineFile );
	m_Snapshot.cb++;
	m_Snapshot.ar << GetId();
	m_Snapshot.ar << SNAPSHOTTYPE_REMOVESKILLINFULENCE;
	m_Snapshot.ar << wType;
	m_Snapshot.ar << wID;
	
	m_Snapshot.Unlock( theLineFile );
}
*/