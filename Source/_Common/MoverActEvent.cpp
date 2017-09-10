#include "stdafx.h"
#include "defineText.h"
#include "defineObj.h"
#include "defineSkill.h"
#include "Ship.h"
#include "ModelObject.h"
#include "CreateObj.h"
//#include "ModelGlobal.h"


#ifdef __CLIENT
	#include "AppDefine.h"
	#include "defineSound.h"
	#include "Sfx.h"
	#include "DPClient.h"
	#include "..\_Common\ParticleMng.h"
#else	// __CLIENT
	#include "User.h"
	#include "WorldMng.h"
	#include "DPSrvr.h"
	#include "DPCoreClient.h"
	#include "dpdatabaseclient.h"
	#include "..\_AIInterface\AIInterface.h"
	#include "Party.h"
#endif	// not __CLIENT

#ifdef __EVENT_MONSTER
	#include "EventMonster.h"
#endif // __EVENT_MONSTER



#ifdef __CLIENT
	extern	CDPClient			g_DPlay;
#else	// __CLIENT
	extern	CUserMng			g_UserMng;
	extern	CWorldMng			g_WorldMng;
	extern	CDPSrvr				g_DPSrvr;
	extern	CDPDatabaseClient	g_dpDBClient;
	extern	CDPCoreClient		g_DPCoreClient;
	extern	CPartyMng			g_PartyMng;
#endif	// not __CLIENT

BOOL IsLootSendText( ItemProp* pItemProp )
{
	if( pItemProp == NULL )
		return FALSE;

	if( pItemProp->dwItemKind1 == IK1_GOLD    ||
		pItemProp->dwItemKind1 == IK1_GENERAL ||
		pItemProp->dwItemKind1 == IK1_SYSTEM  ||
		pItemProp->dwItemKind1 == IK1_RIDE        )
		return FALSE;

	return TRUE;
}

/*--------------------------------------------------------------------------------------------------------


											Action Event Handler
										   ----------------------
						CActionMover������ ���ۿ� ���õ� �̺�Ʈ�� �߻��ϸ� ȣ��Ǵ� �Լ���


----------------------------------------------------------------------------------------------------------*/

//
// ��ų ����� �۵��ϴ� Ÿ�� �������� �Ǹ� �� �Լ��� ȣ���� 
// ������ ��ų.    
//
BOOL CMover::OnMeleeSkill( int nType, int nCount )
{
	if( m_oaAct != OBJACT_USESKILL )	
		return FALSE;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.
	int		nSkill = GetActParam( 0 );
	OBJID	idTarget = GetActParam( 1 );
	DWORD	dwLevel	= GetActParam( 3 );
#if __VER >= 8 // __S8_PK
	BOOL	bControl = GetActParam( 4 );
#endif // __VER >= 8 // __S8_PK

	ItemProp* pSkillProp;
	AddSkillProp* pAddSkillProp;

	// ��ų������Ƽ ������.
	GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "OnMeleeSkill" );

	// .Ÿ�ٿ��� ��ųȿ��(����������..)�� �ش�.
	CMover* pFocusObj = prj.GetMover( idTarget );		// Ÿ�ٹ��������͸� ��.
	if( IsInvalidObj( pFocusObj ) )	
		return FALSE;

#if defined(__CLIENT)
	CModelObject* pModel =(CModelObject*)m_pModel;
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;
		
		D3DXVECTOR3 vDir;
		if( pFocusObj == this )
			vDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		else
			vDir = GetPos() - pFocusObj->GetPos();

		D3DXVec3Normalize( &pModel->m_SparkInfo.m_v3SparkDir, &vDir );
	}
#endif //defined(__CLIENT) 

	int nDamage = 0;

	// ��� ���¸� sfx�߻�.
	if( pSkillProp->dwExeTarget == EXT_MAGICATKSHOT || pSkillProp->dwExeTarget == EXT_MAGICSHOT	)	
	{
#ifdef __WORLDSERVER
		int nMaxDmgCnt	= 1;
		if( pSkillProp->tmContinuousPain != NULL_ID )
			nMaxDmgCnt	= ( pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime ) + 1;
		float fDmgPower = 0;
		if( pSkillProp->dwDmgShift != NULL_ID )
			fDmgPower	= (int)pSkillProp->dwDmgShift * 0.2f;
//		for( int nDmgCnt = 0; nDmgCnt < nMaxDmgCnt; nDmgCnt++ )
#if __VER >= 8 // __S8_PK
			OnAttackSFX( pFocusObj->GetId(), pAddSkillProp->dwSkillLvl, pSkillProp->dwID, 0, GetAngle(), fDmgPower, ( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ), bControl );
#else // __VER >= 8 // __S8_PK
			OnAttackSFX( pFocusObj->GetId(), pAddSkillProp->dwSkillLvl, pSkillProp->dwID, 0, GetAngle(), fDmgPower, ( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ) );
#endif // __VER >= 8 // __S8_PK
#endif	// __WORLDSERVER
		nDamage = ShootSkill( pFocusObj, pSkillProp, pAddSkillProp );		// �������� �̺κ��� ����ȵȴ�.
	}
	else
	{
		// ������� ó��. ���ο��� ApplySkill()�� ȣ��.
#if __VER >= 8 // __S8_PK
		nDamage = DoApplySkill( pFocusObj, pSkillProp, pAddSkillProp, false, 0, FALSE, bControl );
#else // __VER >= 8 // __S8_PK
		nDamage = DoApplySkill( pFocusObj, pSkillProp, pAddSkillProp );
#endif // __VER >= 8 // __S8_PK
	}
	TRACE( "OnMeleeSkill%s, ", pSkillProp->szName );

	if( nDamage && nCount == 0 )		// ����Ÿ���ϰ�� ù��° Ÿ������ ����
	{
		// ��Ÿ�� �ð� ���� - Ŭ�� ���� ���ÿ� ���ư���
#ifdef __CLIENT
		if( IsActiveMover() )
#endif
		{
#if __VER >= 9	// __SKILL_0706
			if( pAddSkillProp->dwCooldown != 0xFFFFFFFF )	// ��Ÿ���� �ִ� ��ų�� ���
				SetCoolTime( pAddSkillProp, "OnMeleeSkill" );
#else	// __SKILL_0706
			if( pSkillProp->dwSkillReady != 0xffffffff )	// ��Ÿ���� �ִ� ��ų�� ���
				SetCoolTime( pSkillProp, "OnMeleeSkill" );
#endif	// __SKILL_0706
		}
	}
	return TRUE;
} 


// RT_TIME�� ���� ��ų���� ��ų �ð��� ���.
void	CMover::SubReferTime( DWORD *pTime1, DWORD *pTime2, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp )
{
	DWORD dwTime1 = 0, dwTime2 = 0;

	if( pSkillProp->dwReferTarget1 == RT_TIME )
	{
		switch( pSkillProp->dwReferStat1 )
		{
		case DST_STA:	dwTime1 = GetSta();		break;
		case DST_DEX:	dwTime1 = GetDex();		break;
		case DST_INT:	dwTime1 = GetInt();		break;
		}
		
		dwTime1 = (DWORD)( ((pSkillProp->dwReferValue1/10.0f)*dwTime1) + (pAddSkillProp->dwSkillLvl * FLOAT(dwTime1/50.0f)) );
	}
	
	if( pSkillProp->dwReferTarget2 == RT_TIME )
	{
		switch( pSkillProp->dwReferStat2 )
		{
		case DST_STA:	dwTime2 = GetSta();		break;
		case DST_DEX:	dwTime2 = GetDex();		break;
		case DST_INT:	dwTime2 = GetInt();		break;
		}
		
		dwTime2 = (DWORD)( ((pSkillProp->dwReferValue2/10.0f)*dwTime2) + (pAddSkillProp->dwSkillLvl * FLOAT(dwTime2/50.0f)) );
	}

	*pTime1 = dwTime1;
	*pTime2 = dwTime2;
}

//
// this���� �Ķ���͸� �����Ѵ�.
// SetDestParam�� �ϰ������� �ϴ� �κ�.
// m_SkillState.Serialize()�Ŀ��� �̰� ����ȴ�.
// pSrc�� ������.
BOOL	CMover::ApplyParam( CCtrl *pSrc, ItemProp *pSkillProp, AddSkillProp *pAddSkillProp, BOOL bSend, int nDamage )
{
	int		i;
	CMover *pTarget = (CMover *)this;

	DWORD	dwDestParam, dwSkillTime, dwPainTime, dwAbilityMin;
	int		nAdjParam, nChgParam;
	int		nDestData[3];

#if __VER >= 9	// __SKILL_0706
	BOOL bPVP	= pSrc->GetType() == OT_MOVER && ( (CMover*)pSrc )->IsPlayer() && IsPlayer();
#endif	// __SKILL_0706

	for( i = 0; i < 2; i ++ )
	{
		if( pAddSkillProp )
		{
			dwDestParam = pAddSkillProp->dwDestParam[i];
			dwSkillTime = pAddSkillProp->dwSkillTime;
			dwPainTime = pAddSkillProp->dwPainTime;
#if __VER >= 9	// __SKILL_0706
			dwAbilityMin	= bPVP? pAddSkillProp->dwAbilityMinPVP: pAddSkillProp->dwAbilityMin;
#else	// __SKILL_0706
			dwAbilityMin	= pAddSkillProp->dwAbilityMin;
#endif	// __SKILL_0706
			nAdjParam	= pAddSkillProp->nAdjParamVal[i];
			nChgParam	= (int)pAddSkillProp->dwChgParamVal[i];
			nDestData[0] = pAddSkillProp->nDestData1[0];
			nDestData[1] = pAddSkillProp->nDestData1[1];
			nDestData[2] = pAddSkillProp->nDestData1[2];
		}
		else
		{	
			dwDestParam = pSkillProp->dwDestParam[i];
			dwSkillTime = pSkillProp->dwSkillTime;
			dwPainTime  = pSkillProp->tmContinuousPain;
			dwAbilityMin = pSkillProp->dwAbilityMin;
			nAdjParam	= pSkillProp->nAdjParamVal[i];
			nChgParam	= (int)pSkillProp->dwChgParamVal[i];
			nDestData[0] = pSkillProp->nDestData1[0];
			nDestData[1] = pSkillProp->nDestData1[1];
			nDestData[2] = pSkillProp->nDestData1[2];
		}
		// destParam 1
		switch( dwDestParam )
		{
		case NULL_ID:
			break;
		case DST_CHRSTATE:		// ���� ����
			if( nAdjParam & CHS_INVISIBILITY )	// ��ũ�Ϸ�ǻ��·�...
			{
				if( pTarget->SetDarkCover( TRUE, dwSkillTime ) == FALSE )
					return FALSE;
				pTarget->m_wDarkCover = 0;		
			}
			else
			if( nAdjParam & CHS_STUN )	// ���ϻ��·�...
			{
				if( pTarget->SetStun( TRUE, dwSkillTime ) == FALSE )
					return FALSE;
				// �ϴ� ����. 
				// ������ �ɸ��°� �ΰ��� ��Ʈ�� �ִµ�
				// 1. ��ų�� �¾�����.
				// 2. ���Ͼ����ۿ� �¾�����
				// 3. ��Ÿ ��Ȳ(Ʈ������..)
				// ��ų�� ����ؼ� ������ �ɸ��� SkillInflu���� ó���ؼ�
				// �������� �����ֱⶫ�� m_wStunCnt�� ���� ���� �α����� ������ ��Ű�� �ȴ�.
				// �����ۿ� ���� ������ �ɸ��� SkillInflu���� ó���� ���ϹǷ� m_wStunCnt�� �ʿ��ϴ�.
				// �̰��� ��ų�� ���ؼ� ������ ���̱� ����.
				// ������ �߸�����ƴ�.  ��ų�߽��� �ƴϰ� �����߽����� ó���� �ƾ�� �ߴ�.
				pTarget->m_wStunCnt = 0;		
			} else
			if( nAdjParam & CHS_POISON )	// �����·�..
			{
				if( pTarget->SetPoison( TRUE, pSrc->GetId(), dwSkillTime, dwPainTime, nDestData[0] ) == FALSE )	// nDestData[0] ƽ�� ���� ������
					return FALSE;
				pTarget->m_wPoisonCnt = 0;		// �����̶� ���� ����
			} else
			if( nAdjParam & CHS_DARK )	// �������
			{
				if( pTarget->SetDark( TRUE, dwSkillTime, nDestData[1] ) == FALSE )		// nDestData1[1] : ���߷� ����ġ.
					return FALSE;
				pTarget->m_wDarkCnt = 0;		// �����̶� ���� ����
			} else

			if( nAdjParam & CHS_BLEEDING )	// �������·�..
			{
				if( nDestData[0] < 0 )
					nDestData[0]	= nDamage / 10;
				if( pTarget->SetBleeding( TRUE, pSrc->GetId(), dwSkillTime, dwPainTime, nDestData[0] ) == FALSE )
					return FALSE;
				pTarget->m_wBleedingCnt = 0;		// �����̶� ���� ����
			} else
			{
				// �ٸ� ���µ��� �� ������.
				BOOL bAble = TRUE;
				if( (nAdjParam & CHS_SLOW) && pTarget->GetProp()->dwClass == RANK_MIDBOSS )		// ���ο���¸� �߰��������� �ɷ��� �ߴٸ�.
					bAble = FALSE;		// �鿪.
				if( bAble )
					pTarget->SetDestParam( dwDestParam, nAdjParam, NULL_CHGPARAM, TRUE );
			}
			break;

		case DST_CHR_STEALHP: 			// HP���.  ������ DST_�� �ϱⰡ �� ������.
			{
				int nStealHP = (int)( nDamage * (nAdjParam / 100.0f) );	// ���������� ���������� ����Ҿ��� ����.
				if( pSrc->GetType() == OT_MOVER )
					((CMover *)pSrc)->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM );		// ������ HP�� ���.
			}
			break;
		case DST_CHR_STEALHP_IMM: 			// �����Ⱦ��� HP���.  
			{
				if( GetType() == OT_MOVER )
				{
					int nHP = GetHitPoint();		// ������� hp
					int nStealHP = nAdjParam;	// ����ؾ��� hp
					if( nHP < nStealHP )	// �Ǹ� �̾ƿ;��ϴµ� �����ǰ� �� ������.
						nStealHP = nHP;		// �����Ǹ�ŭ �̾ƿ�
					//SetDestParam( DST_HP, -nStealHP, 0 );		// �Ǹ� ����.
					if( pSrc->GetType() == OT_MOVER )
					{
//   						((CMover*)this)->m_pActMover->SendDamage( AF_FORCE,		// Ÿ���� �������԰�
//   													      GetId(), 
//  														  nStealHP );
						// chipi_080303 ���� ���� - ���Ͱ� �ڱ� �ڽ��� Ÿ������ �����ϴ� ����
 						((CMover*)this)->m_pActMover->SendDamage( AF_FORCE,		// Ÿ���� �������԰�
  														  ((CMover*)pSrc)->GetId(), 
 														  nStealHP );


						((CMover *)pSrc)->SetDestParam( DST_HP, nStealHP, NULL_CHGPARAM, TRUE  );		// ������ HP�� ���.
					}
				}
			}
			break;

		case DST_CURECHR:		// ���� ����
			pTarget->RemoveDebuff( nAdjParam );
			break;

		case DST_CLEARBUFF:		// ����� ������ �ı�.
			if( nAdjParam == NULL_ID )
			{
				pTarget->RemoveBuffAll();
			}
			else
			{
				for( int i = 0; i < nAdjParam; i ++ )
					pTarget->RemoveBuffOne();
			}
			break;
		case DST_CHR_CHANCESTUN:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEPOISON:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEDARK:	// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCEBLEEDING:// adj:Ȯ��		data1:��ų
		case DST_CHR_CHANCESTEALHP:	// adj:Ȯ��		data1:��ų
			pTarget->SetDestParam( dwDestParam, nAdjParam, nChgParam, TRUE );
			break;
			
#ifdef __WORLDSERVER
		case DST_HP:	// ����.
			if( pAddSkillProp )
			{
				if( pSkillProp->dwReferTarget1 == RT_HEAL || pSkillProp->dwReferTarget2 == RT_HEAL )
				{
					DWORD dwHp1 = 0, dwHp2 = 0;
					
					if( pSrc->GetType() == OT_MOVER )	// �����ڰ� ����϶��� ���ȿ� ���� ����
					{
						if( pSkillProp->dwReferTarget1 == RT_HEAL )
						{
							switch( pSkillProp->dwReferStat1 )
							{
							case DST_STA:		dwHp1 = ((CMover *)pSrc)->GetSta();			break;
							case DST_DEX:		dwHp1 = ((CMover *)pSrc)->GetDex();			break;
							case DST_INT:		dwHp1 = ((CMover *)pSrc)->GetInt();			break;
							}
							dwHp1 = (DWORD)( ((pSkillProp->dwReferValue1 / 10.0f) * dwHp1) + (pAddSkillProp->dwSkillLvl * FLOAT(dwHp1 / 50.0f)) );
						}
						
						if( pSkillProp->dwReferTarget2 == RT_HEAL )
						{
							switch( pSkillProp->dwReferStat2 )
							{
							case DST_STA:		dwHp2 = ((CMover *)pSrc)->GetSta();			break;
							case DST_DEX:		dwHp2 = ((CMover *)pSrc)->GetDex();			break;
							case DST_INT:		dwHp2 = ((CMover *)pSrc)->GetInt();			break;
							}
							dwHp2 = (DWORD)( ((pSkillProp->dwReferValue2 / 10.0f) * dwHp2) + (pAddSkillProp->dwSkillLvl * FLOAT(dwHp2 / 50.0f)) );
						}
					}
					
					// ������ HP��.
					int nIncHP = pAddSkillProp->nAdjParamVal1 + dwHp1 + dwHp2;
					// �����迭 ��ų�� ���?
					if( (pAddSkillProp->dwDestParam1 == DST_HP || pAddSkillProp->dwDestParam2 == DST_HP) && nIncHP > 0 )
					{
						if( pSrc->GetType() == OT_MOVER )
							if( ((CMover *)pSrc)->IsPlayer() )	// �����ڰ� �÷��̾��϶�
								if( pTarget->GetMaxHitPoint() > pTarget->GetHitPoint() + nIncHP )		// Ÿ���� ������ �Ƴ�?
									((CUser *)pSrc)->m_nOverHeal = PROCESS_COUNT * 30;				// �����ڴ� 30�ʰ� ������ ����. 
					}
					
					// ������.
					pTarget->SetDestParam( pAddSkillProp->dwDestParam1, nIncHP, pAddSkillProp->dwChgParamVal1, TRUE );
					
				}
			}
			break;
#endif // WorldServer
		case DST_SPEED:
			{
				MoverProp *pMoverProp = pTarget->GetProp();
				if( pMoverProp )
				{
					if( pMoverProp->dwClass != RANK_MIDBOSS		// MIDBOSS�� �ƴϸ� �� �ɸ�.
#if __VER >= 12 // __SECRET_ROOM
						&& pMoverProp->dwClass != RANK_SUPER	// 12�� ���ʹ� SUPER�� �Ȱɸ���.
#endif // __SECRET_ROOM
						)		
						pTarget->SetDestParam( dwDestParam, nAdjParam, nChgParam, bSend );
				}
			}
			break;
		default: // �׿� DST_�� �Ϲ����� ��ƾ�� ź��.
			// �Ϲ� DST_ó��
			pTarget->SetDestParam( dwDestParam, nAdjParam, nChgParam, bSend );
			break;
		}
	}

	return TRUE;
}


BOOL	CMover::GetSkillProp( ItemProp **ppSkillProp, AddSkillProp **ppAddSkillProp, int nSkill, DWORD dwLevel, LPCTSTR szErr )
{
	char szStr[256] = {0, };
	sprintf( szStr, "%s-%s", m_szName, szErr );

	return CCtrl::GetSkillProp( ppSkillProp, ppAddSkillProp, nSkill, dwLevel, szStr );
}


// ��Ÿ�� ����!
#if __VER >= 9	// __SKILL_0706
void	CMover::SetCoolTime( AddSkillProp* pAddSkillProp, LPCTSTR szCall )
{
	int nIdx	= GetSkillIdx( pAddSkillProp->dwName );		// ��ų����Ʈ �ε����� ã��.
	
	if( nIdx < 0 || nIdx >= MAX_SKILL_JOB )
		Error( "szCall SetCoolTime : %d %d��ų�� ã�� �� ���� %s", nIdx, pAddSkillProp->dwName, GetName() );
	else
		m_tmReUseDelay[ nIdx ] = pAddSkillProp->dwCooldown + timeGetTime();		// 1/1000����
}
#else	// __SKILL_0706
void	CMover::SetCoolTime( ItemProp *pSkillProp, LPCTSTR szCall )
{
	int nIdx = GetSkillIdx( pSkillProp->dwID );		// ��ų����Ʈ �ε����� ã��.
	
	if( nIdx < 0 || nIdx >= MAX_SKILL_JOB )
		Error( "szCall SetCoolTime : %d %d��ų�� ã�� �� ���� %s", nIdx, pSkillProp->dwID, GetName() );
	else
		m_tmReUseDelay[ nIdx ] = pSkillProp->dwSkillReady + timeGetTime();	// 1/1000����
}
#endif	// __SKILL_0706
//
//	���� ��ų�� Ÿ���� ȣ��.
//  nCount ���ͺ��� ��� ������ ȣ���� �Ǳ⶧���� nCount==0 �� ��츸 ����ġ�� �ö󰣴�.
//
BOOL CMover::OnMagicSkill( int nType, int nCount )
{
	if( m_oaAct != OBJACT_USESKILL )	
		return FALSE;	// ��ų����� Ÿ���� �Ǳ����� �ٸ� ������� �ٲ���. �̷���Ȳ�� �ͼ� �ȵȴ�.

	int			nSkill = GetActParam(0);
	OBJID		idTarget = GetActParam(1);
	DWORD		dwLevel = GetActParam( 3 );
#if __VER >= 8 // __S8_PK
	BOOL	bControl = GetActParam( 4 );
#endif // __VER >= 8 // __S8_PK

	ItemProp*	pSkillProp = NULL;
	AddSkillProp* pAddSkillProp = NULL;
	LPSKILL		pSkill	= NULL;
	
	BOOL bRet = GetSkillProp( &pSkillProp, &pAddSkillProp, nSkill, dwLevel, "CMover::OnMagicSkill" );
	if( bRet == FALSE )
	{
		Error( "OnMagicSkill : ��ų�����б� ������. ������������ Motion=%d %d", m_dwMotion, dwLevel );
		return FALSE;
	}
	
	CMover* pFocusObj = prj.GetMover( idTarget );		// Ÿ�ٹ��������͸� ��.
	if( IsInvalidObj( pFocusObj ) )		
		return FALSE;

	if( pSkillProp->dwID == SI_ASS_HEAL_HEALING )	// ���� ��밡 �׾����� ���
	{
		if( pFocusObj->IsDie() )
			return FALSE;
	}

	BOOL bSuccess = FALSE;
	BOOL bShoot	= pSkillProp->dwExeTarget == EXT_MAGICATKSHOT  || pSkillProp->dwExeTarget == EXT_MAGICSHOT;
	if( bShoot )	// ��� ���¸� sfx�߻�.
	{
#ifdef __WORLDSERVER
		int nMaxDmgCnt	= 1;
		if( pSkillProp->tmContinuousPain != NULL_ID )
			nMaxDmgCnt	= ( pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime ) + 1;
		float fDmgPower = 0;
		if( pSkillProp->dwDmgShift != NULL_ID )
			fDmgPower	= (int)pSkillProp->dwDmgShift * 0.2f;
//		for( int nDmgCnt = 0; nDmgCnt < nMaxDmgCnt; nDmgCnt++ )
#if __VER >= 8 // __S8_PK
			OnAttackSFX( pFocusObj->GetId(), pAddSkillProp->dwSkillLvl, pSkillProp->dwID, 0, GetAngle(), fDmgPower, ( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ), bControl );
#else // __VER >= 8 // __S8_PK
			OnAttackSFX( pFocusObj->GetId(), pAddSkillProp->dwSkillLvl, pSkillProp->dwID, 0, GetAngle(), fDmgPower, ( pSkillProp->dwSkillType == KT_SKILL? AF_MELEESKILL: AF_MAGICSKILL ) );
#endif // __VER >= 8 // __S8_PK
#endif	// __WORLDSERVER
		bSuccess = ShootSkill( pFocusObj, pSkillProp, pAddSkillProp );		// �������� �̺κ��� ����ȵȴ�.
	}
	else
	{
		// ������� ó��. ���ο��� ApplySkill()�� ȣ��.
#if __VER >= 8 // __S8_PK
		int nDamage = DoApplySkill( pFocusObj, pSkillProp, pAddSkillProp, false, 0, FALSE, bControl );
#else // __VER >= 8 // __S8_PK
		int nDamage = DoApplySkill( pFocusObj, pSkillProp, pAddSkillProp );
#endif // __VER >= 8 // __S8_PK
		if( nDamage == 0 )
			return FALSE;
		bSuccess = (BOOL)nDamage;

	#ifdef __CLIENT	
		if( pSkillProp->dwID == SI_MAG_MAG_BLINKPOOL )
		{
			CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
			
			if( IsActiveMover() )
			{
				D3DXVECTOR3 vStart;
				D3DXVECTOR3 vEnd;

				vStart = GetPos();		vStart.y += 1.0f;
				vEnd = pWndWorld->m_vTelePos;
				
				CWorld *pWorld = GetWorld();
				
				if( pWorld->IntersectObjLine( NULL, vStart, vEnd, FALSE, FALSE ) )
				{
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
					g_WndMng.PutString( prj.GetText( TID_GAME_BLOCKTARGETING ), NULL, prj.GetTextColor( TID_GAME_BLOCKTARGETING ) );
					return FALSE;
				}
			
				g_DPlay.SendTeleSkill( GetId(), pWndWorld->m_vTelePos );
			}
		}
	#endif // Client		
	}
	
	if( bSuccess )	// ��ų��� �����ϸ� �����ȱ���. / ����ġ �ȿ���.
	{
	#ifdef __WORLDSERVER
		// �߻��� ������ �������� ��´�.
		int nReqMp = GetReqMp( pAddSkillProp->nReqMp );

		#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		if(nReqMp != 9999999)
		#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans
		{
			if( pAddSkillProp->nSkillCount > 1 )
				nReqMp /= pAddSkillProp->nSkillCount;
			IncManaPoint( -nReqMp );
			if( pAddSkillProp->nReqFp != NULL_ID )
				IncFatiguePoint( -GetReqFp( pAddSkillProp->nReqFp ) );
		}
		
		#if __VER >= 10 // __LEGEND	//	10�� ���½ý���	Neuz, World, Trans
			if( pSkillProp->dwID == SI_RIG_HERO_RETURN )
			{
				REPLACE_TYPE type = REPLACE_NORMAL;

				PRegionElem pRgnElem	= NULL;
				CWorld* pWorld	= GetWorld();
				if( !pWorld )
					return FALSE;

				if( IsChaotic() )
				{
		#if __VER >= 8 // __S8_PK
					if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
						pRgnElem	= g_WorldMng.GetRevivalPosChao( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
					if( NULL == pRgnElem )	// Find near revival pos
						pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), GetPos() );
		#else // __VER >= 8 // __S8_PK
					pRgnElem	= g_WorldMng.GetNearRevivalPosChao( pWorld->GetID(), GetPos() );
		#endif // __VER >= 8 // __S8_PK
				}
				else

				{
					if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
						pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
					if( NULL == pRgnElem )	// Find near revival pos
						pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), GetPos() );
				}
				if( NULL != pRgnElem )
					REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, type, nRevivalLayer );
				else
					REPLACE( g_uIdofMulti, pWorld->GetID(), GetPos(), type, nDefaultLayer );
			}
			if( pSkillProp->dwID == SI_KNT_HERO_DRAWING )
			{
				D3DXVECTOR3 vSrc	= GetPos();
				FLOAT		fAngle	= GetDegree( pFocusObj->GetPos(), GetPos() );	// �����ڿ� Ÿ���� ������ ����
				//vSrc.x -= 1.0f;
				//vSrc.z -= 1.0f;

				pFocusObj->SetPos(vSrc);
				g_UserMng.AddSetPos( pFocusObj, vSrc );
				g_UserMng.AddPushPower( pFocusObj, pFocusObj->GetPos(), pFocusObj->GetAngle(), fAngle, 0.2f );
				pFocusObj->ClearDestObj();
//				pFocusObj->SetPosChanged( TRUE );
			}
		#endif	//__LEGEND	//	10�� ���½ý���	Neuz, World, Trans


	#endif // WORLDSERVER

		// ��Ÿ�� �ð� ���� - Ŭ�� ���� ���ÿ� ���ư���
	#ifdef __CLIENT
		if( IsActiveMover() && nCount == 0 )		// ����Ÿ���ϰ�� ù��° Ÿ������ ����
	#endif // __CLIENT
#if __VER >= 9	// __SKILL_0706
		if( pAddSkillProp->dwCooldown != 0xFFFFFFFF )
			SetCoolTime( pAddSkillProp, "OnMagicSkill" );
#else	// __SKILL_0706
		if( pSkillProp->dwSkillReady != 0xffffffff )	// ��Ÿ���� �ִ� ��ų�� ���
			SetCoolTime( pSkillProp, "OnMagicSkill" );
#endif	// __SKILL_0706
	} // bSuccess

#if defined(__CLIENT)
	CModelObject* pModel = (CModelObject*)m_pModel;
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;
		
		AngleToVectorXZ( &pModel->m_SparkInfo.m_v3SparkDir, GetAngle(), -1.0f );
	}
#endif //defined(__CLIENT)

	return TRUE;
} // OnMagicSkill


//
//	��Ÿ����Ⱑ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ
//
BOOL	CMover::OnAttackRange()
{
	int		idTarget = GetActParam( 0 );
	DWORD	dwItemID = GetActParam( 1 );		// ����� ������ ���̵� (player�̸� ��������)
	int		idSfxHit = GetActParam( 2 );		// ���Ͱ� ����Ѱ��̸� ���⿡ �����κ��� �������� ����ִ�.
#if __VER >= 8 // __S8_PK
	BOOL	bControl = FALSE;
#endif // __VER >= 8 // __S8_PK

	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj(pHit) )	
		return FALSE;							// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.
	
	ItemProp *pItemProp = NULL;
	if( IsPlayer() )
		pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else 
	{
		if( dwItemID == NULL_ID || dwItemID == 0 )	
			pItemProp = GetActiveHandItemProp();		// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
		else
			pItemProp = prj.GetItemProp( dwItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)
	}

	if( pItemProp == NULL )
		return FALSE;

#ifdef __WORLDSERVER
#if __VER >= 12 // __MONSTER_SKILL
	if( IsNPC() )
		CMonsterSkill::GetInstance()->ApplySkill( this, pHit, ATK_RANGE );
#endif // __MONSTER_SKILL
#endif // __WORLDSERVER

	DWORD dwSfxObj = pItemProp->dwSfxObj2;
	
	D3DXVECTOR3 vPosDest = pHit->GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 
	D3DXVECTOR3 vPosSrc  = GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 

#ifdef __CLIENT		
	CSfx* pSfx = NULL;
#endif
	// ��ũ�ι��� ����Ʈ ����� �׻� �޼տ�...
	if( IsPlayer() )		
	{
		ItemProp *pItemElem = GetActiveHandItemProp( PARTS_BULLET );  // ȭ��������� ������ ȭ������Ʈ ����
		if( pItemElem && pItemElem->dwSfxObj2 != NULL_ID )
		{
		#ifdef __CLIENT
			if( IsActiveMover() )
				pSfx = CreateSfxArrow( pItemElem->dwSfxObj2, pItemProp->dwSfxObj3, vPosDest, idTarget );
		#endif //__CLIENT

		#ifdef __WORLDSERVER
			g_UserMng.AddCreateSfxAllow( this, pItemElem->dwSfxObj2, pItemProp->dwSfxObj3, vPosDest, idTarget );
		#endif //__WORLDSERVER
		}
	}
	else
	{
		if( dwSfxObj == NULL_ID )	
			return FALSE;
	#ifdef __CLIENT		
		pSfx = CreateSfx( D3DDEVICE, dwSfxObj, vPosSrc,GetId(), vPosDest ,idTarget );
	#endif
	}

	DWORD dwAtkFlags = AF_GENERIC;
	if( IsPlayer() )
		dwAtkFlags |= AF_RANGE;

#ifdef __WORLDSERVER
#if __VER >= 8 // __S8_PK
	OnAttackSFX( pHit->GetId(), ( IsPlayer()? dwItemID: 0 ), NULL_ID, 0, 0, 0, dwAtkFlags, bControl );
#else // __VER >= 8 // __S8_PK
	OnAttackSFX( pHit->GetId(), ( IsPlayer()? dwItemID: 0 ), NULL_ID, 0, 0, 0, dwAtkFlags );
#endif // __VER >= 8 // __S8_PK
#endif	// __WORLDSERVER

#ifdef __CLIENT
	if( pSfx )
	{
		if( IsActiveMover() )		
		{
			idSfxHit = ++m_idSfxHit; 	// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
			g_DPlay.SendSfxID( idTarget, idSfxHit, dwAtkFlags );
			pSfx->m_idSfxHit	= idSfxHit;
			pSfx->m_nMagicPower = IsPlayer() ? dwItemID : 0;		// ���������� �־��ش�.
		}
		// ����� g_pPlayer�ų� / ����� ���̰� �´³��� g_pPlayer��
		if( IsActiveMover() || (IsNPC() && pHit->IsActiveMover()) )
			pSfx->m_idSfxHit	= idSfxHit;		// id�� �����
	}
#endif //__CLIENT
	return TRUE;
}


//
//	�ϵ�(����)������ �߻�Ǵ� ������ �߻��ϴ� �̺�Ʈ.
//	
BOOL	CMover::OnAttackMagic()
{
	int		idTarget = GetActParam( 0 );
	int		nMagicPower = GetActParam( 1 );
#if __VER >= 8 // __S8_PK
	BOOL	bControl = GetActParam( 4 );
#endif // __VER >= 8 // __S8_PK

	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.

	if( IsInvalidObj(pHit) )	
		return FALSE;		// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.

#ifdef __WORLDSERVER
#if __VER >= 8 // __S8_PK
	OnAttackSFX( pHit->GetId(), nMagicPower, NULL_ID, 0, 0, 0, AF_MAGIC, bControl );
#else // __VER >= 8 // __S8_PK
	OnAttackSFX( pHit->GetId(), nMagicPower, NULL_ID, 0, 0, 0, AF_MAGIC );
#endif // __VER >= 8 // __S8_PK
#else	// __WORLDSERVER
	DWORD dwWeaponId;

	if( IsActiveMover() ) 
	{
		CItemElem* pWeapon = GetWeaponItem();
		if( pWeapon == NULL )	return FALSE;
		dwWeaponId	= pWeapon->m_dwItemId;
	}
	else 
	{
		dwWeaponId	= m_aEquipInfo[PARTS_RWEAPON].dwId;
		if( dwWeaponId == NULL )	return FALSE;
	}

#if __VER < 11 // __REMOVE_ENDURANCE
	if( IsActiveMover() )
	{
		CItemElem* pWeapon = GetWeaponItem();
		ItemProp* pItemProp		= pWeapon->GetProp();
		if( pItemProp )
			if( ( ( pWeapon->m_nHitPoint * 100 ) / pItemProp->dwEndurance ) == 0 )
				g_WndMng.PutString( prj.GetText( TID_GAME_WASTEENDURANCE ), NULL, prj.GetTextColor( TID_GAME_WASTEENDURANCE ) );
	}
#endif // __REMOVE_ENDURANCE

	D3DXVECTOR3 vPosDest = pHit->GetPos(); 
	// �߻� ��ǥ������ ����� �߾�.
	float fHeight = fabs(pHit->m_pModel->m_vMax.y) / 2.0f;	// ���̹�Ʈ�������� ���߿� ���ֱⶫ�� ���� ������ Max���� ���ߴ�.
	fHeight *= pHit->GetScale().x;
	vPosDest.y += fHeight;	

	D3DXVECTOR3 vPosSrc = GetPos() + D3DXVECTOR3( 0.0f, 1.0f, 0.0f ); // �߻� ������ ���Ƿ� �÷��ش�. ���� 

	ItemProp *pHandItemProp = GetActiveHandItemProp( PARTS_RWEAPON );
	CSfx* pSfx = NULL;
	if( m_pActMover->IsFly() )
	{
		pSfx = CreateSfx( D3DDEVICE, XI_FIR_WAND02, vPosSrc, GetId(), vPosDest, idTarget );
		if( pSfx )
			pSfx->ShootSfx( GetAngle(), -GetAngleX(), 0.75f );		 
	} 
	else
	{
		DWORD dwSfxObj2 = XI_FIR_WAND01;
		if( pHandItemProp )
			dwSfxObj2 = pHandItemProp->dwSfxObj2;

		pSfx = CreateSfx( D3DDEVICE, dwSfxObj2, vPosSrc,GetId(), vPosDest ,idTarget );
		if( pSfx )
		{
			switch(nMagicPower) 
			{
			case 1:		pSfx->SetScale( D3DXVECTOR3( 1.2f, 1.2f, 1.2f ) );	break;
			case 2:		pSfx->SetScale( D3DXVECTOR3( 1.7f, 1.7f, 1.7f ) );	break;
			case 3:
			case 4:		pSfx->SetScale( D3DXVECTOR3( 2.5f, 2.5f, 2.5f ) );	break;
			}
			((CSfxItemWandAtk1 *)pSfx)->m_fCenter = fHeight;		
		}
	}

	if( pSfx )
	{
		if( IsActiveMover() )		// g_pPlayer�� �Ʒ����� �ϰ� �Ѵ�.
		{
			int idSfxHit = ++m_idSfxHit;			// Ŭ�󿡼� �߻�ü�� ������ ��� ID�� ����� �װ��� ������ ����.
			g_DPlay.SendSfxID( idTarget, idSfxHit, AF_MAGIC );
			pSfx->m_idSfxHit	= idSfxHit;
			pSfx->m_nMagicPower = nMagicPower;		// ���������� �־��ش�.
		}
	}


	CModelObject* pModel = (CModelObject*)m_pModel;
	if( pModel )
	{
		pModel->m_SparkInfo.m_bUsed  = TRUE;
		pModel->m_SparkInfo.m_v3Color.x = 2.0f;
		pModel->m_SparkInfo.m_v3Color.y = 2.0f;
		pModel->m_SparkInfo.m_v3Color.z = 2.0f;
		pModel->m_SparkInfo.m_nCnt = 0;
		pModel->m_SparkInfo.m_fLerp= 1.0f;
		
		AngleToVectorXZ( &pModel->m_SparkInfo.m_v3SparkDir, GetAngle(), -1.0f );
	}
#endif // __WORLDSERVER
	return TRUE;
}

void	CMover::OnAttackMelee_ClockWorks( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	CModelObject *pModel = (CModelObject *)m_pModel;
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:		// Į�� ����ġ��
		{
			D3DXVECTOR3 vPos;
			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 0 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

			OnDamage( nItemID, dwState, NULL, &vPos );		// ����Ʈ�� �߻��ϴ� ��ġ�� �������� �ְ�
#ifdef __CLIENT
			GetWorld()->m_pCamera->SetQuake( 15, 0.5f );
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1DIRFIRESP, vPos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
			if( pSfx )
				pSfx->SetAngle( -GetAngle() );
#endif 
		}
		break;
	case OBJSTA_ATK2:		// �����
		{
			D3DXVECTOR3 vPos;
			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 1 );	// �޼� ���Թ� ��ġ.
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );
#ifdef __CLIENT
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1DIRCANNON, vPos, GetId() );	// �߻�����Ʈ.
			if( pSfx )
			{
				pSfx->SetAngle( -GetAngle() );
				pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
			}
#endif
			CMover *pTarget = pHitObj;
			if( IsValidObj(pTarget) )
			{
				OnDamage( nItemID, dwState, NULL, &pTarget->GetPos() );		// Ÿ���� ��ǥ�� �߽����� �������� ��.
#ifdef __CLIENT
				pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1RANBALL, vPos, GetId(), pTarget->GetPos(), pTarget->GetId(), -1 );		// ��ź
				if( pSfx )
				{
					pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
					{
						FLOAT fAngXZ, fAngH;
						D3DXVECTOR3 vDist = pTarget->GetPos() - vPos;
						xGetDegree( &fAngXZ, &fAngH, vDist );		// ��ǥ���� ��������.
						((CSfxAtkStraight *)pSfx)->ShootSfx( fAngXZ, -fAngH, 2.0f, XI_NPCSP1RANBOOM );		// ���ư��� ���� ����.
					}
				}
#endif
			}
		}
		break;
	case OBJSTA_ATK3:
#ifdef __CLIENT
		GetWorld()->m_pCamera->SetQuake( 30, 0.7f );
#endif
		break;
	case OBJSTA_ATK4:	// �߷� ���.
		{
			D3DXVECTOR3 vPos;

			vPos = GetPos();
			DWORD dwFlag = AF_GENERIC;
			if( xRandom(100) < 15 )
				dwFlag |= AF_FLYING;
			OnDamage( nItemID, dwState, NULL, &vPos, dwFlag );		// �������� ���߽��� �������� �Ѵ�.

			CModelObject *pModel = (CModelObject *)m_pModel;
			pModel->GetEventPos( &vPos, 3 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

#ifdef __CLIENT
			GetWorld()->m_pCamera->SetQuake( 60, 1.5f );

			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1DIRCIRCLE, vPos );
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3(2.0f, 2.0f, 2.0f) );
			pModel->GetEventPos( &vPos, 4 );
			D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

			pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1DIRCIRCLE, vPos );
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3(2.0f, 2.0f, 2.0f) );
#endif
		}
		break;
	}
}

void	CMover::OnAttackMelee_BigMuscle( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�
	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
	case OBJSTA_ATK2:
		{
			if( IsValidObj(pHitObj) )
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
		}
		break;
	case OBJSTA_ATK3:		// ��ġ�� ��ų - �μ�
		{
			D3DXMATRIX mMatrix;
			
			D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(12)), &(GetMatrixWorld()) );
			
			D3DXVECTOR3 v2;
			v2.x = mMatrix._41;
			v2.y = mMatrix._42;
			v2.z = mMatrix._43;
			v2.y -= 5.0f;
			
		#ifdef __CLIENT
			// ��ġ�� ������ ����Ʈ �޼�
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_CIRCLE_DUST, v2 );			
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );

			PLAYSND( SND_PC_SKILLD_HITOFPENYA );//, &v2 );
		#endif //__CLIENT 			
			
			D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(16)), &(GetMatrixWorld()) );
			D3DXVECTOR3 v3;
			v3.x = mMatrix._41;
			v3.y = mMatrix._42;
			v3.z = mMatrix._43;						
			v3.y -= 5.0f;
			
		#ifdef __CLIENT
			// ��ġ�� ������ ����Ʈ ������
			pSfx = CreateSfx( D3DDEVICE, XI_SKILL_CIRCLE_DUST, v3 );			
			if( pSfx )
				pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );
		#endif //__CLIENT 			
		
			if( m_pActMover->m_nMotionHitCount == 1 )
			{				
	#ifdef __WORLDSERVER
				GetWorld()->SendDamageAround( &GetPos(), (AF_GENERIC | AF_CRITICAL) , this, OBJTYPE_PLAYER, II_WEA_MOB_MONSTER4_ATK2
					, 16.0f );					

				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_BLEEDING, 10, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							
	#endif //__WORLDSERVER
				
			}
			else
			{
	#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
	#endif //__CLIENT
	#ifdef __WORLDSERVER				
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_BIGMUSCLE_STUNGROUP, 1, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							
	#endif //__WORLDSERVER
			}
		}
		break;
	case OBJSTA_ATK4:   // ��ġ�� ��ų - �Ѽ�
		{
			static D3DXVECTOR3 v3OldPos;

			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				v3OldPos = pHitObj->GetPos();			
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )
			{
				D3DXMATRIX mMatrix;
				
				D3DXMatrixMultiply( &mMatrix, (pModel->GetMatrixBone(12)), &(GetMatrixWorld()) );
				
				D3DXVECTOR3 v2;
				v2.x = mMatrix._41;
				v2.y = mMatrix._42;
				v2.z = mMatrix._43;
				v2.y -= 5.0f;
				
			#ifdef __CLIENT
				// ��ġ�� ������ ����Ʈ �޼�
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_CIRCLE_DUST, v2 );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 9.0f, 9.0f, 9.0f ) );
				
				pSfx = CreateSfx( D3DDEVICE, XI_SKILL_MAG_EARTH_ROCKCRASH01, v3OldPos );
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
				
				PLAYSND( SND_PC_SKILLD_HITOFPENYA );
			#endif //__CLIENT

			#ifdef __WORLDSERVER
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_BLEEDING, 10, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					DoApplySkill( pHitObj, pSkillProp, pAddSkillProp );	
				
				GetWorld()->SendDamageAround( &(v3OldPos), AF_GENERIC | AF_CRITICAL , this, OBJTYPE_PLAYER, II_WEA_MOB_MONSTER4_ATK2
					, 4.0f );	
			#endif //__WORLDSERVER
				
					
			}
		}
		
		break;
	}
}

void	CMover::OnAttackMelee_Krrr( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
		{
	#ifdef __WORLDSERVER			
			OnDamage( nItemID, dwState, pHitObj );		
	#endif //__WORLDSERVER			
		}
		break;
	case OBJSTA_ATK2:	
		{
	#ifdef __WORLDSERVER
			int nRandom = xRandom(100);
			if( nRandom <= 50 )
			{
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_KRRR_POISON, 1, "OnSfxHit" );
				if( bSuccess )
				{
					pHitObj->ApplySkill( this, pSkillProp, pAddSkillProp );
				}
			}
			else
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK3:		// �μ����� ���� ����ġ��
		{
		#ifdef __CLIENT
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1DIRFIRESP, pHitObj->GetPos() );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
			if( pSfx )
				pSfx->SetAngle( -GetAngle() );

			PLAYSND( SND_PC_BOSS_CRASH, &pHitObj->GetPos() );
		#endif //__CLIENT

		#ifdef __WORLDSERVER
			ItemProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_KNOCK_BACK, 4, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );							

			GetWorld()->SendDamageAround( &(pHitObj->GetPos()), AF_GENERIC, this, OBJTYPE_PLAYER, II_WEA_MOB_MONSTER4_ATK1
				, 7.0f );	
		#endif //__WORLDSERVER
		}
		break;
	case OBJSTA_ATK4:		// �ֹ��� �ܿ�� �÷��̾�� �Ѱ��� ������ ���������� ��
		{
			static D3DXVECTOR3	  vLocal;
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
		#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_SUMMONS );
		#endif //__CLIENT
				
				AngleToVectorXZ( &vLocal, GetAngle(), 5.0f );	// �����¹��� 5���;�.
				vLocal += GetPos();

				FLOAT fHeight = GetWorld()->GetLandHeight( vLocal.x, vLocal.z );
				vLocal.y = fHeight;
				
		#ifdef __WORLDSERVER
				CPartyQuestProcessor* pProc	= CPartyQuestProcessor::GetInstance();
				CWorld* pWorld	= GetWorld();
				if( pWorld )
				{
					int nId	= pProc->PtInQuestRect( pWorld->GetID(), GetPos() );
					if( nId > -1 )
					{
						PPARTYQUESTELEM pElem	= pProc->GetPartyQuest( nId );
						PPARTYQUESTRECT pRect	= pProc->GetPartyQuestRect( nId );
						if( pElem && pRect )
#ifdef __LAYER_1015
							g_UserMng.ReplaceWorldArea( pElem->idParty, pRect->dwWorldId, pRect->dwWorldId, vLocal.x, vLocal.z, 0.5f, TRUE, GetLayer() );
#else	// __LAYER_1015
							g_UserMng.ReplaceWorldArea( pElem->idParty, pRect->dwWorldId, pRect->dwWorldId, vLocal.x, vLocal.z, 0.5f, TRUE );
#endif	// __LAYER_1015
					}
				}
		#endif //__WORLDSERVER
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )		// �� 5���Ŀ� ����
			{	
		#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
		#endif //__CLIENT
#ifdef __WORLDSERVER
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_KRRR_STUNGROUP, 1, "OnAttackMelee_Krrr" );
				if( bSuccess )
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );	
#endif //__WORLDSERVER
				
			}
			else
			if( m_pActMover->m_nMotionHitCount == 2 )
			{	
#ifdef __CLIENT
				D3DXVECTOR3 v3Pos = vLocal;
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_ELE_MULTI_METEOSHOWER02, v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
				v3Pos.x += ( xRandomF( 12.0f ) + -6.0f );
				pSfx = CreateSfx( D3DDEVICE, XI_SKILL_ELE_MULTI_METEOSHOWER02, v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
				v3Pos = vLocal;
				v3Pos.z += ( xRandomF( 12.0f ) + -6.0f );
				pSfx = CreateSfx( D3DDEVICE, XI_SKILL_ELE_MULTI_METEOSHOWER02, v3Pos );	// ����Ʈ�� �߻� - �������� ���� �ȵ�.
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 0.8f, 0.8f, 0.8f ) );
				
#endif //__CLIENT
				
			}
			else
			if( m_pActMover->m_nMotionHitCount == 3 )
			{	
		#ifdef __WORLDSERVER
				GetWorld()->SendDamageAround( &(pHitObj->GetPos()), AF_GENERIC, this, OBJTYPE_PLAYER, II_WEA_MOB_MONSTER4_ATK2
					, 5.0f );	
		#endif //__WORLDSERVER
				
			}
		}
		break;
	}
}

#if __VER >= 10 // __Y_DRAGON_FIRE
void	CMover::OnAttackMelee_Meteonyker( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:
		{
			if( IsValidObj(pHitObj) )
			{
				OnDamage( nItemID, dwState, pHitObj );		
			}
		}
		break;
	case OBJSTA_ATK3:		// ���̾ �߻�
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				CMover *pTarget = pHitObj;

				D3DXVECTOR3 vPos;
				CModelObject *pModel = (CModelObject *)m_pModel;
				pModel->GetEventPos( &vPos, 0 );	// ���� �� ��ǥ.
				D3DXVec3TransformCoord( &vPos, &vPos, GetMatrixWorldPtr() );

				if( IsValidObj(pTarget) )
				{
					OnDamage( nItemID, dwState, pTarget, &pTarget->GetPos() );		// Ÿ���� ��ǥ�� �߽����� �������� ��.
					ItemProp *pSkillProp;
					AddSkillProp *pAddSkillProp;
					BOOL bFind	= GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_STUNSINGLE, 5, "SI_GEN_STUNSINGLE" );
					if( bFind )
					{
						if( xRand() % 2 )
							DoApplySkill( pTarget, pSkillProp, pAddSkillProp , false );
					}

	#ifdef __CLIENT
					CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCSP1RANBALL, vPos, GetId(), pTarget->GetPos(), pTarget->GetId(), -1 );		// ��ź
					if( pSfx )
					{
						pSfx->SetScale( D3DXVECTOR3(4.0f, 4.0f, 4.0f) );
						{
							FLOAT fAngXZ, fAngH;
							D3DXVECTOR3 vDist = pTarget->GetPos() - vPos;
							xGetDegree( &fAngXZ, &fAngH, vDist );		// ��ǥ���� ��������.
							((CSfxAtkStraight *)pSfx)->ShootSfx( fAngXZ, -fAngH, 2.0f, XI_NPCSP1RANBOOM );		// ���ư��� ���� ����.
						}
					}
	#endif	// __CLIENT
				}
			}
		}
		break;
	case OBJSTA_ATK4:   // ���� �����ٰ� ��ġ��
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{
				// ���� ȿ��
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_BURN, 1, "OnAttackMelee_Meteonyker" );
				if( bSuccess )
				{
					D3DXVECTOR3 vPos	= GetPos();
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp, false, FALSE, 10.0F );
					MoverProp* pProp	= GetProp();
					OnDamage( pProp->dwAtk4, OBJSTA_ATK4, NULL, &vPos );
#ifdef __CLIENT
					CSfx *pSfx = CreateSfx( D3DDEVICE, XI_NPCMETEONYKER, vPos );
#endif	// __CLIENT
				}
			}
		}
		break;
	}
}
#endif //__Y_DRAGON_FIRE

void	CMover::OnAttackMelee_Bear( DWORD dwState, CMover *pHitObj )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	CModelObject *pModel = (CModelObject *)m_pModel;
	
#ifdef __WORLDSERVER
	UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
	pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
#endif // WorldServer
	
	switch( dwState )
	{
	case OBJSTA_ATK1:	
		{
	#ifdef __WORLDSERVER
			ItemProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_ATKSPELL_RATEGROUP, 5, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );			
			
			OnDamage( nItemID, dwState, pHitObj );					
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK2:	
		{
	#ifdef __WORLDSERVER
			OnDamage( nItemID, dwState, pHitObj );					
	#endif // WorldServer
		}
		break;
	case OBJSTA_ATK3:		
		{
			if( m_pActMover->m_nMotionHitCount == 0 )
			{				
	#ifdef __CLIENT
				CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_CIRCLE_DUST, GetPos() );			
				if( pSfx )
					pSfx->SetScale( D3DXVECTOR3( 12.0f, 12.0f, 12.0f ) );
				
				PLAYSND( SND_PC_SKILLD_HITOFPENYA );//, &v2 );
	#endif //__CLIENT 			
				
	#ifdef __WORLDSERVER
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_KNOCK_BACK, 4, "OnAttackMelee_BigMuscle" );
				if( bSuccess )
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );
	#endif //__WORLDSERVER			
			}
			else
			if( m_pActMover->m_nMotionHitCount == 1 )
			{
	#ifdef __CLIENT
				PLAYSND( SND_PC_BOSS_STUN );
	#endif //__CLIENT
				
	#ifdef __WORLDSERVER
				ItemProp *pSkillProp;
				AddSkillProp *pAddSkillProp;
				BOOL bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_GEN_STUNGROUP, 5, "OnAttackMelee_Bear" );
				if( bSuccess )
					ApplySkillAround( this, OBJTYPE_PLAYER, pSkillProp, pAddSkillProp , false );				
	#endif //__WORLDSERVER			
			}
		}
		break;
	case OBJSTA_ATK4:	
		{	
#ifdef __WORLDSERVER
			ItemProp *pSkillProp;
			AddSkillProp *pAddSkillProp;
			BOOL bSuccess;
	
			// �ڽ� ����
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_HEAL_HEALING, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//����ũ
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_MUSHMOOT_BERSERK, 1, "OnAttackMelee_Bear" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			// ��罺Ų 
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_MUSHMOOT_WOODENSKIN, 1, "OnAttackMelee_Bear" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );
			
			// ����
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_HEAPUP, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//ĳ��
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_CANNONBALL, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//��Ż����
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_MENTALSIGN, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//������
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_BEEFUP, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );
			
			
			//ĳ���÷���
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_CATSREFLEX, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//��ť����
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_ACCURACY, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );

			//���̽�Ʈ
			bSuccess = GetSkillProp( &pSkillProp, &pAddSkillProp, SI_ASS_CHEER_HASTE, 1, "OnAttackMelee_BigMuscle" );
			if( bSuccess )
				DoApplySkill( this, pSkillProp, pAddSkillProp , false );
			

			// �������Ͱ� ���� Ŀ����.
			D3DXVECTOR3 vScale = GetScale();

			vScale *= 1.3f;

			if( vScale.x < 10.0f )
			{
				SetScale( vScale );
				g_UserMng.AddSetScale( this, (vScale.x / m_pModel->m_pModelElem->m_fScale)  );
			}
			
#endif //__WORLDSERVER			
		}
		break;
	}
}
// �������� �����ϴ� �κ�. - ���ݹ�Ŀ� ���� Ÿ���ѹ��� ��� ȣ��� ���� �ִ�.
// pHitObj : �������� ���� ���. NULL�̸� vDamagePos�� ��ǥ�� �߽����� �������� �ش�.
BOOL	CMover::OnDamage( int nItemID, DWORD dwState, CMover *pHitObj, const D3DXVECTOR3 *pvDamagePos, DWORD dwAtkFlag )
{
	int nParam = 0x01;		//sun

	if( IsPlayer() && IsDualWeapon() )		// ��Į����?
	{
		if( m_pActMover->m_dwAtkFlags & AF_GENERIC )	// ��Ÿ��
		{
			switch( dwState )
			{
			case OBJSTA_ATK1:	nParam |= 0x01;		break; // ������
			case OBJSTA_ATK2:	nParam |= 0x02;		break; // �޼�
			case OBJSTA_ATK3:	nParam |= 0x01;		break; // ������
			case OBJSTA_ATK4:	nParam |= 0x03;		break; // ���
			}
		}
	}


#if defined(__CLIENT)
	if( IsValidObj(pHitObj) )
	{
		CModelObject* pHitModel = (CModelObject*)pHitObj->m_pModel;
		if( pHitModel )
		{
			D3DXVECTOR3 vDir;
			pHitModel->m_SparkInfo.m_bUsed = TRUE;
			pHitModel->m_SparkInfo.m_v3Color.x = 2.0f;
			pHitModel->m_SparkInfo.m_v3Color.y = 2.0f;
			pHitModel->m_SparkInfo.m_v3Color.z = 2.0f;
			pHitModel->m_SparkInfo.m_nCnt = 0;
			pHitModel->m_SparkInfo.m_fLerp= 1.0f;

			vDir = pHitObj->GetPos() - GetPos();
			D3DXVec3Normalize( &pHitModel->m_SparkInfo.m_v3SparkDir, &vDir );
		}
	}
#endif //defined(__CLIENT) 
	
	int	nDamage = 0;
	if( IsPlayer() )
	{
		// �÷��̾�
		nDamage = pHitObj->m_pActMover->SendDamage( m_pActMover->m_dwAtkFlags, GetId(), nParam );
	}
	else
	{
		// ����
		ItemProp *pItemProp = prj.GetItemProp( nItemID );
		if( pItemProp )
		{
			if( pItemProp->dwSpellRegion == SRO_AROUND )
			{
				int nType = OBJTYPE_PLAYER;		// �÷��̾�鸸 ������.
				if( pHitObj )
					pHitObj->SendDamageAround( dwAtkFlag, this, nType, nItemID, (float)pItemProp->_dwSkillRange, 1.0f, 1.0f ); 
				else
					GetWorld()->SendDamageAround( pvDamagePos, dwAtkFlag, this, nType, nItemID, (float)pItemProp->_dwSkillRange ); 
			} else
			{
				if( pHitObj )
					nDamage = pHitObj->m_pActMover->SendDamage( m_pActMover->m_dwAtkFlags, GetId(), 1 );
			}
		} else
		{
			if( pHitObj )
				nDamage = pHitObj->m_pActMover->SendDamage( m_pActMover->m_dwAtkFlags, GetId(), 1 );
		}
	}


 #ifdef __WORLDSERVER
	if( nDamage > 0 )		// THIS�� �����ߴµ� ������ ���̴µ� �����ߴ�
	{
		if( pHitObj->m_pActMover->GetState() & OBJSTA_ATK_MELEESKILL )	// �´³��� ��ų ������̾���.
		{
			DWORD dwSkill = pHitObj->GetActParam(0);		// ���� ����ϰ� �ִ� ��ų. dwSkill
			if( dwSkill == SI_MER_ONE_REFLEXHIT )			// ���÷���Ʈ ��ų�� ���� �ִ� �߿� �¾Ҵ�.
			{
				pHitObj->m_nReflexDmg += nDamage;			// ���÷��������� ����.
			}
		}
	}
 #endif
	return nDamage > 0;
}

//
// �Ϲ� ������ Ÿ���� ȣ��Ǵ� �ڵ鷯.
BOOL	CMover::OnAttackMelee( DWORD dwState, CMover *pHitObj )
{
	CModelObject	*pModel = (CModelObject *)m_pModel;
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_ATTACK_MELEE );
#endif
	
 	if( m_dwIndex == MI_CLOCKWORK1 )	// Ŭ����ũ�� ��� ������ ����Ʈ.
	{
		OnAttackMelee_ClockWorks( dwState, pHitObj );
		return TRUE;
	}
	
	if( m_dwIndex == MI_BIGMUSCLE )	// ��ӽ��� ��� ������ ����Ʈ.
	{
		OnAttackMelee_BigMuscle( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}

	if( m_dwIndex == MI_KRRR )	// ũ������ ��� ������ ����Ʈ.
	{
		OnAttackMelee_Krrr( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}	

	if( m_dwIndex == MI_MUSHMOOT )	// ũ������ ��� ������ ����Ʈ.
	{
		OnAttackMelee_Bear( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}	

#if __VER >= 10 // __Y_DRAGON_FIRE
	if( m_dwIndex == MI_DU_METEONYKER || m_dwIndex == MI_DU_METEONYKER2 || m_dwIndex == MI_DU_METEONYKER3 ||
		m_dwIndex == MI_DU_METEONYKER4 )
	{
		OnAttackMelee_Meteonyker( dwState, pHitObj );
		m_pActMover->m_nMotionHitCount++;		
		return TRUE;
	}
#endif //__Y_DRAGON_FIRE

#ifdef __WORLDSERVER
#if __VER >= 12 // __MONSTER_SKILL
	if( IsNPC() && CMonsterSkill::GetInstance()->ApplySkill( this, pHitObj, ATK_MELEE ) )
	{
		//m_pActMover->m_nMotionHitCount++;
		return TRUE;
	}
#endif // __MONSTER_SKILL
#endif // __WORLDSERVER

	BOOL bSuccess = FALSE;
	BOOL bDamage = TRUE;

#ifdef __WORLDSERVER
	if( IsNPC() && pHitObj->IsPlayer() && pHitObj->IsRangeObj( this, 10.0f ) == FALSE )		// �ι��״� ��춫�� �־�ô�. ��->���� 10���� �̻� �������������� �ȸ°�.
		bDamage = FALSE;
#endif
	if( bDamage )
	{
		bSuccess = OnDamage( nItemID, dwState, pHitObj );	// �������� �ش�.
	}

#ifdef __WORLDSERVER
	if( bSuccess )
	{
		if( IsPlayer() )
		{
			if( dwState == OBJSTA_ATK1 && ((CUser*)this)->m_playTaskBar.m_nActionPoint < 100 )	// 2Ÿ° ġ�°� �׼�����Ʈ �ö�.
				((CUser*)this)->m_playTaskBar.m_nActionPoint ++;
		}
	}
#endif	// worldserver

#ifdef __CLIENT
	if( bSuccess )		// Ÿ�ݿ� �����ߴٸ� Ÿ������ �÷���.
	{
		if( CMover::GetActiveMover() == this )
		{
			if( dwState == OBJSTA_ATK1 && g_WndMng.m_pWndTaskBar->m_nActionPoint < 100 )	// 2Ÿ° ġ�°� �׼�����Ʈ �ö�.
				g_WndMng.m_pWndTaskBar->m_nActionPoint ++;
		}
		//  �÷��̾�� ��� �ִ� ���⿡ ���� Ÿ�� ���� �ٸ���.
		ItemProp* pItemProp = GetActiveHandItemProp();
		if( pItemProp )
		{
/*		#ifndef __Y_DRAGON_FIRE
			if( pItemProp->dwSfxObj2 != NULL_ID )
			{
				CSfx *pSfx = CreateSfx( D3DDEVICE, pItemProp->dwSfxObj2,  GetPos(), GetId(), this->GetPos(), this->GetId(), 0 ); 
			}
		#endif //__Y_DRAGON_FIRE
*/
			PLAYSND( pItemProp->dwSndAttack1, &GetPos() );
		}
		
		if( IsPlayer() )
		{
			pModel->m_nPause = 5;	// frame ����
		}
		else
		{
			pModel->m_nPause = 0;	// ���ʹ� ������ ����
			pHitObj->m_pModel->m_nPause = 0;
		}

#if defined(__CLIENT)
		{
			if( IsValidObj( pHitObj ) )
			{
				pModel->m_SparkInfo.m_bUsed  = TRUE;
				pModel->m_SparkInfo.m_v3Color.x = 2.0f;
				pModel->m_SparkInfo.m_v3Color.y = 2.0f;
				pModel->m_SparkInfo.m_v3Color.z = 2.0f;
				pModel->m_SparkInfo.m_nCnt = 0;
				pModel->m_SparkInfo.m_fLerp= 1.0f;
				
				D3DXVECTOR3 vDir = GetPos() - pHitObj->GetPos();
				D3DXVec3Normalize( &pModel->m_SparkInfo.m_v3SparkDir, &vDir );
			}
		}
#endif //defined(__CLIENT)
		
		
	}
#endif // client
	
	
	return TRUE;
}

//
//
// Ÿ�� ���ĺ��� ���������� ��� ȣ��.
BOOL	CMover::OnAttackMeleeContinue( DWORD dwState )
{
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�

	if( m_dwIndex == MI_CLOCKWORK1 )	// Ŭ����ũ�� ��� ������ ����Ʈ.
	{
	#ifdef __WORLDSERVER
		UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
		CModelObject *pModel = (CModelObject *)m_pModel;
		pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
	#endif // WorldServer

		switch( dwState )
		{
		case OBJSTA_ATK3:
			{
				D3DXVECTOR3 v1, v2;
				CModelObject *pModel = (CModelObject *)m_pModel;
				pModel->GetEventPos( &v1, 2 );		// ������ ������ �κ�.
				D3DXVec3TransformCoord( &v1, &v1, GetMatrixWorldPtr() );	// ����������� ��ȯ.
				
				v2 = D3DXVECTOR3( 0, 0, 40.0f );
				D3DXVec3TransformCoord( &v2, &v2, pModel->GetMatrixBone(6) );	// 6�� �Ӹ�.  �Ӹ����� v2������ ��ġ�� ���.
				D3DXVec3TransformCoord( &v2, &v2, GetMatrixWorldPtr() );	// ���� �������� ��ȯ.
				
		#ifdef __CLIENT
				extern CPartsLaser g_Laser;
				g_Laser.SetPos( v1, v2, 0.5f );
		#endif

		#ifdef __CLIENT
				if( (m_pActMover->m_nCount & 1) == 0 )
		#else				
				if( (m_pActMover->m_nCount & 3) == 0 )
		#endif
				{
					CWorld *pWorld = GetWorld();
					D3DXVECTOR3 vOut;
					if( pWorld->IntersectObjLine( &vOut, v1, v2, FALSE, TRUE, FALSE ) )
					{
						OnDamage( nItemID, dwState, NULL, &vOut );		// ����Ʈ�� ������ ��ġ�� �������� ��.

					#ifdef __CLIENT
						CSfx *pSfx = CreateSfx( D3DDEVICE,  XI_NPCSP1RANSPARK, vOut );
						if( pSfx )
							pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
					#endif // __CLIENT
					}
						
				}
			}
			break;
		}
	}
#if __VER >= 10 // __Y_DRAGON_FIRE
	else
	if( m_dwIndex == MI_DU_METEONYKER || m_dwIndex == MI_DU_METEONYKER2 || m_dwIndex == MI_DU_METEONYKER3 ||
		m_dwIndex == MI_DU_METEONYKER4 )
	{
	#ifdef __WORLDSERVER
		UpdateMatrix();		// ��ü�� Ư���������� ���� �������� ó���ϱ� ���ؼ� ���������� ��Ʈ������ �����ؾ��Ѵ�.
		CModelObject *pModel = (CModelObject *)m_pModel;
		pModel->UpdateMatrixBone();		// ���������� ���ִϸ��̼��� ���� �����Ƿ� GetEventPos()�� ����� ���� ������ ������Ʈ�� ����� �Ѵ�.
	#endif // WorldServer

		switch( dwState )
		{
			case OBJSTA_ATK2:
			{
				if( m_pActMover->m_nMotionHitCount == 1 )
				{
					D3DXVECTOR3 v1, v2, v3;
					CModelObject *pModel = (CModelObject *)m_pModel;
					pModel->GetEventPos( &v1, 0 );		// ������ ������ �κ�.
					pModel->GetEventPos( &v3, 1 );		// ������ ������ �κ�.
					D3DXVec3TransformCoord( &v1, &v1, GetMatrixWorldPtr() );	// ����������� ��ȯ.
					D3DXVec3TransformCoord( &v3, &v3, GetMatrixWorldPtr() );	// ����������� ��ȯ.

					v2 = D3DXVECTOR3( 0, 0, 40.0f );
					D3DXVec3TransformCoord( &v2, &v2, pModel->GetMatrixBone(6) );	// 6�� �Ӹ�.  �Ӹ����� v2������ ��ġ�� ���.
					D3DXVec3TransformCoord( &v2, &v2, GetMatrixWorldPtr() );	// ���� �������� ��ȯ.
					
			#ifdef __CLIENT
					D3DXVECTOR3 vDir = v3 - v1;
					D3DXVec3Normalize( &vDir, &vDir );
					D3DXVECTOR3 vTemp;
					D3DXVECTOR3 vScal = D3DXVECTOR3( 0.3f, 0.3f, 0.3f );
					D3DXVECTOR3 vModelScal = GetScale();

					vScal.x *= vModelScal.x;
					vScal.y *= vModelScal.y;
					vScal.z *= vModelScal.z;

					for( int i = 0; i < 2; i ++ )
					{
						vTemp = vDir * (xRandomF( 0.2f ) + 0.1f );
						vTemp *= 0.8f;
						vTemp.y -= (xRandomF( 0.05f ) + 0.01f );
						
						extern CPartsFireDragon	g_FireDragon;
						g_FireDragon.Create( D3DDEVICE, v1, XI_NAT_FIRE01_ADV, vScal, vTemp );
					}

			#endif

			#ifdef __CLIENT
					if( (m_pActMover->m_nCount & 1) == 0 )
			#else				
					if( (m_pActMover->m_nCount & 3) == 0 )
			#endif
					{
						CWorld *pWorld = GetWorld();
						D3DXVECTOR3 vOut	= GetPos();
//						if( pWorld->IntersectObjLine( &vOut, v1, v2, FALSE, TRUE, FALSE ) )
						{
							OnDamage( nItemID, dwState, NULL, &vOut );		// ����Ʈ�� ������ ��ġ�� �������� ��.
	/*
						#ifdef __CLIENT
							CSfx *pSfx = CreateSfx( D3DDEVICE,  XI_NPCSP1RANSPARK, vOut );
							if( pSfx )
								pSfx->SetScale( D3DXVECTOR3( 2.0f, 2.0f, 2.0f ) );
						#endif // __CLIENT
	*/
						}
							
					}
				}
				break;
			}
		}
	}
#endif //__Y_DRAGON_FIRE

	return TRUE;
}

// ������ Ư������ Ÿ�ݺκп� ȣ��
BOOL	CMover::OnAttackSP()
{
	int		idTarget = GetActParam( 0 );		
	int		nItemID = GetActParam( 1 );	// ����� ������ ���̵�
	
	CMover* pHit = prj.GetMover( idTarget );	// Ÿ���� �����͸� ��.
	if( IsInvalidObj(pHit) )	return FALSE;		// Ŭ��������ô� �־����� �߻�Ǳ����� �����Ǿ���.

	ItemProp *pItemProp;
	if( nItemID == NULL_ID || nItemID == 0 )	
		pItemProp = GetActiveHandItemProp();	// �̷����� �� �տ� �� ����(Ȥ�� �⺻������)
	else
		pItemProp = prj.GetItemProp( nItemID );	// ���� �������� ������ ���� �װɷ� ������(���Ͱ� �ַ� �����)

	if( pItemProp == NULL )
	{
		Error( "CMover::OnAttackSP : %s�� itemProp�� ����. %d", m_szName, nItemID );
		return FALSE;
	}

	if( pItemProp->dwSpellRegion == SRO_AROUND )
	{	
		SendDamageAround( AF_GENERIC, this, OBJTYPE_PLAYER, nItemID, (float)( pItemProp->_dwSkillRange ), 0.0, 1.0f );	// �������ݽ�����
	} else
	{
#ifdef __CLIENT
		if( pHit->IsLive() )
			pHit->m_pActMover->SendDamage( AF_MONSTER_SP_CLIENT, GetId(), (nItemID << 16) );	// 09_12_21 ���� ����� ����
#else
		if( pHit->IsLive() )
			pHit->m_pActMover->SendDamage( AF_GENERIC, GetId(), (nItemID << 16) );	// ���� ���� ������.
#endif
	}

	return TRUE;
}                                                         

//
//	ĳ���Ͱ� ���������� ������ ������ ȣ��
//
void	CMover::OnActDrop()
{
#ifdef __WORLDSERVER
	if( m_pActMover->GetDmgState() & OBJSTA_DMG_FLY_ALL )		// ������ �ö��̶� �̰� ���ϴ°� ����.
		return;

	if( FALSE == IsPlayer() )
	{
		if( m_nCorr < 0
#if __VER >= 9	//__AI_0509
			&& GetSpeedFactor() < 1.9F
#endif	// __AI_0509
#ifdef __JEFF_11
			&& m_dwAIInterface != AII_PET
#endif	//	__JEFF_11
			)
		{
			ClearDestPos();
			SendActMsg( OBJMSG_STAND );
			if( m_pActMover->IsFly() )
				g_UserMng.AddMoverCorr2( this, GetPos(), m_pActMover->m_vDelta, GetAngle(), GetAngleX(),
					m_pActMover->m_fAccPower, m_pActMover->m_fTurnAngle, m_pActMover->GetState(), m_pActMover->GetStateFlag(),
					m_dwMotion, m_pActMover->m_nMotionEx, m_pModel->m_nLoop, m_dwMotionOption, g_TickCount.GetTickCount() );
			else
				g_UserMng.AddMoverCorr( this, GetPos(), m_pActMover->m_vDelta, GetAngle(),
					m_pActMover->GetState(), m_pActMover->GetStateFlag(), m_dwMotion, m_pActMover->m_nMotionEx,
					m_pModel->m_nLoop, m_dwMotionOption, g_TickCount.GetTickCount() );
			m_nCorr		= 0;
		}
	}
	else
	{
		if( FALSE == IsEmptyDest() && FALSE == m_fWaitQueryGetPos )
		{
			( (CUser*)this )->AddQueryGetPos( NULL_ID );
			m_fWaitQueryGetPos	= TRUE;
		}
	}
#else	// __WORLDSERVER

	if( IsActiveMover() )
	{
		if( m_nCorr != 0 )
		{
			if( m_nCorr < 0 ) {
				ClearDest();
				SendActMsg( OBJMSG_STAND );
			}
			if( m_pActMover->IsFly() )
				g_DPlay.SendPlayerCorr2();
			else
				g_DPlay.SendPlayerCorr();
			m_nCorr		= 0;
		}
	}
	else
	{
#if __VER >= 9	//__AI_0509
		if( IsPlayer() || GetSpeedFactor() < 1.9F )
#endif	// __AI_0509
		{
			if( FALSE == IsEmptyDest() && FALSE == m_fWaitQueryGetPos )
			{
				g_DPlay.SendQueryGetPos( this );
				m_fWaitQueryGetPos	= TRUE;
			}
		}
	}
#endif	// not __WORLDSERVER
}

void	CMover::OnActCollision()
{
#ifdef __WORLDSERVER
	if( FALSE == IsPlayer() )
	{
		if( m_nCorr < 0 
#if __VER >= 9	//__AI_0509
			&& GetSpeedFactor() < 1.9F && m_dwAIInterface != AII_PET
#endif	// __AI_0509
			)
		{

			ClearDest();
			SendActMsg( OBJMSG_STAND );

			if( m_pActMover->IsFly() )
				g_UserMng.AddMoverCorr2( this, GetPos(), m_pActMover->m_vDelta, GetAngle(), GetAngleX(),
					m_pActMover->m_fAccPower, m_pActMover->m_fTurnAngle, m_pActMover->GetState(), m_pActMover->GetStateFlag(),
					m_dwMotion, m_pActMover->m_nMotionEx, m_pModel->m_nLoop, m_dwMotionOption, g_TickCount.GetTickCount() );
			else
				g_UserMng.AddMoverCorr( this, GetPos(), m_pActMover->m_vDelta, GetAngle(),
					m_pActMover->GetState(), m_pActMover->GetStateFlag(), m_dwMotion, m_pActMover->m_nMotionEx,
					m_pModel->m_nLoop, m_dwMotionOption, g_TickCount.GetTickCount() );

			m_nCorr		= 0;

			PostAIMsg( AIMSG_COLLISION );
		}
	}
	else
	{
		if( FALSE == IsEmptyDest() && FALSE == m_fWaitQueryGetPos )
		{
			( (CUser*)this )->AddQueryGetPos( NULL_ID );
			m_fWaitQueryGetPos	= TRUE;
		}
	}

#else	// __WORLDSERVER
	if( TRUE == IsActiveMover() )
	{
		if( m_nCorr != 0 )
		{
			if( m_nCorr < 0 )
			{
				ClearDest();
				SendActMsg( OBJMSG_STAND );
				CWndWorld* pWndWorld	= (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );
				if( pWndWorld )
					pWndWorld->m_bLButtonDown	= FALSE;
			}
			if( m_pActMover->IsFly() )
				g_DPlay.SendPlayerCorr2();
			else
				g_DPlay.SendPlayerCorr();

			m_nCorr	= 0;
		}
	}
	else if( FALSE == IsActiveMover() )
	{
#if __VER >= 9	//__AI_0509
		if( IsPlayer() || GetSpeedFactor() < 1.9F )
#endif	// __AI_0509
		{
			if( FALSE == IsEmptyDest() && FALSE == m_fWaitQueryGetPos )
			{
				g_DPlay.SendQueryGetPos( this );
				m_fWaitQueryGetPos	= TRUE;
			}
		}
	}

#endif	// __WORLDSERVER
}

// ������ų ���� ��������.
void	CMover::OnActEndMeleeSkill()
{
	ClearActParam();			// �ൿ���� Ŭ����
	m_dwFlag &= (~MVRF_SKILL);
#ifdef __WORLDSERVER
	if( IsPlayer() )
	{
		if( ((CUser *)this)->m_playTaskBar.m_nUsedSkillQueue != -1 )
		{
			((CUser *)this)->m_playTaskBar.SetNextSkill( (CUser *)this );
		}
	}
#endif 

}

// ������ų ���� ��������.
void	CMover::OnActEndMagicSkill()
{
	ClearActParam();			// �ൿ���� Ŭ����
	m_dwFlag &= (~MVRF_SKILL);
 #ifdef __WORLDSERVER
	if( IsPlayer() )
	{
		if( ((CUser *)this)->m_playTaskBar.m_nUsedSkillQueue != -1 )
		{
			((CUser *)this)->m_playTaskBar.SetNextSkill( (CUser *)this );
		}
	}
 #endif 
}

// ����(JUMP4)�÷��װ� ������� ����.
void	CMover::OnActEndJump4()
{
	if( FALSE == IsEmptyDestPos() )
		SetDestPos( GetDestPos(), m_bForward, FALSE );

	if( m_dwIndex == MI_CLOCKWORK1 )
	{
		SendActMsg( OBJMSG_APPEAR );
	}
}

// ���� �ϴ� ������ �߻�.
void	CMover::OnActLanding()
{
#ifdef __CLIENT	
	if( m_dwIndex == MI_CLOCKWORK1 )
	{
		GetWorld()->m_pCamera->SetQuake( 60, 1.0f );		// ������ �������� ���� ��鸲.
	}
#endif
}

// ���� �ϴ� ������ �߻�.
void	CMover::OnActFMove()
{
#ifdef __WORLDSERVER
	PostAIMsg( AIMSG_BEGINMOVE );
#endif
}

// Interact ������Ʈ�� ���� ������.
// pIAObj : � IA������Ʈ�� �����ߴ°�.
// vPos : ������ this�� ��ǥ.
void	CMover::OnActIALanding( CObj *pIAObj, const D3DXVECTOR3 &vPos )
{
	if( pIAObj->GetType() != OT_SHIP )	
		return;
	CShip *pIA = (CShip *)pIAObj;

	CObj *pOldIA = GetIAObjLink();
	SetIAObjLink( pIA );
	
	if( IsActiveMover() )
	{
		if( pIA != pOldIA )	// �ɾ�ٴҶ��� ��� ���ŵǱⶫ�� �Ź� �����ʿ�� ����.
		{
#ifdef __CLIENT			
			D3DXVECTOR3 vLocal = GetPos() - pIA->GetPos();		// IA������Ʈ�κ����� ��� ��ǥ�� ����.
			g_DPlay.SendLocalPosFromIA( vLocal, pIA->GetId() );				// �����ǥ ������ ����
			TRACE( "SendLocalPosFromIA\n" );
#endif //__CLIENT
		}
	}

}

/*
int		CMover::OnActCollecting() 
{ 
#ifdef __CLIENT
	if( (m_dwFlag & MVRF_COLLECT) == 0 )
	{
		ItemProp *pHandProp = GetActiveHandItemProp();
		if( pHandProp->dwSfxObj2 != NULL_ID )
		{
			D3DXVECTOR3 vSrc, vLocal = D3DXVECTOR3(0, 0.5f, 0);
			((CModelObject *)m_pModel)->GetForcePos( &vLocal, 0, PARTS_RWEAPON, GetMatrixWorld() );
			vSrc = vLocal;
			CSfx *pSfx = CreateSfx( D3DDEVICE, pHandProp->dwSfxObj2, vSrc, GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );
			if( pSfx )
			{
				pSfx->SetAngle( -GetAngle() + 90.0f );
				m_dwFlag |= MVRF_COLLECT;		// SFX�� ��������.
			}
		}
	}
#endif // Client
	return 0; 
}				
*/

// ������ �� ����������(�Ϲ�/��ų/�ϵ� ��)
void	CMover::OnAttackStart( CMover *pTarget, OBJMSG dwMsg )
{
}

// this�� ����ȭ ���� �ɷ������� ������Ŵ.
void	CMover::RemoveInvisible()
{
#ifdef __WORLDSERVER

	if( GetAdjParam( DST_CHRSTATE ) & CHS_INVISIBILITY )		// ����ȭ ���´�.
	{
		ResetDestParam( DST_CHRSTATE, CHS_INVISIBILITY, TRUE );
	}

	RemoveBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION );		// ����.				

#endif // __WORLDSERVER
}


// pItem�� ���ð����Ѱ��� �Ǻ�.
#ifdef __WORLDSERVER
BOOL CMover::IsLoot( CItem *pItem, BOOL bPet )
{
	BOOL bTake	= FALSE;
	
	if( pItem->IsDelete() || ( pItem->m_idHolder != 0 && pItem->m_idHolder != m_idPlayer ) )
	{
		return bTake;
	}

	if( pItem->m_idOwn != NULL_ID )		// �����ۿ� ������ ������
	{
		// �Ϲ������� ������ ������.
		CMover *pOther = prj.GetMover( pItem->m_idOwn );		// pItem�� ���� ������ �ִ»��.(����...��Ÿ)
		if( IsValidObj(pOther) )
		{
			if( m_idparty && (pOther->m_idparty == m_idparty) )		// ��Ƽ�� �ְ� ���� ���� ��Ƽ���̸� ������ �־��~~~ ������
				bTake = TRUE;
		} else
			bTake = TRUE;		// idOwn�� Invalid���¸� �ƹ��� ���� �� ����.
		
		if( pItem->m_idOwn == GetId() )		// �������� �������ڸ� ���� �� �ִ�.
			bTake = TRUE;
		
	} else
		bTake = TRUE;	// ������ ���°Ÿ� �ƹ��� ������ �ִ�.

	DWORD dwTime	= g_tmCurrent;
#ifdef __EVENT_MONSTER
	DWORD dwMonsterId = pItem->m_IdEventMonster;
	if( dwMonsterId != NULL_ID && CEventMonster::GetInstance()->SetEventMonster( dwMonsterId ) )
	{
		DWORD dwLootTime = CEventMonster::GetInstance()->GetLootTime();
		if( (dwTime - pItem->m_dwDropTime) >= (DWORD)( SEC(dwLootTime) ) )
			bTake = TRUE;
		
		if( bPet && !CEventMonster::GetInstance()->IsPetAble() )
			bTake = FALSE;
	}
	else
#endif // __EVENT_MONSTER
#if __VER >= 9 // __S_9_ADD
	if( (dwTime - pItem->m_dwDropTime) >= SEC(7) )	// 7�ʰ� ������ �ƹ��� ���� �� �ִ�.
#else // __S_9_ADD
	if( (dwTime - pItem->m_dwDropTime) >= SEC(40) )	// 40�ʰ� ������ �ƹ��� ���� �� �ִ�.
#endif // __S_9_ADD
		bTake = TRUE;

	// ���� �������� �������� ������ ����.
	if( bPet && bTake )
	{
		if( 0 < ((CCtrl*)pItem)->m_lRespawn )
			bTake = FALSE;
#ifdef __JEFF_11
		// ����ǰ�� ���� ���� �ش� �������� ȹ���� �� ����
		CItemElem* pItemElem	= (CItemElem*)pItem->m_pItemBase;
		ItemProp* pItemProp		= pItemElem->GetProp();
		if( pItemProp->dwItemKind3 != IK3_GOLD && m_Inventory.IsFull( pItemElem, pItemProp, pItemElem->m_nItemNum ) )
			bTake	= FALSE;
#endif	// __JEFF_11
	}

	return bTake;
}
#endif // WorldServer

// DoLoot�� ���� - ���� ����Ʈ�� �������� �ַ����϶� ������ �Ҷ� ó����
BOOL CMover::SubLootDropMobSingle( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER	
	CItemBase* pItemBase = pItem->m_pItemBase;
	bSuccess	= CreateItem( pItemBase );
	
	if( IsPlayer() && pItemBase->GetProp() != NULL )
	{
		if( bSuccess )
		{
			((CUser*)this)->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );

			if( IsLootSendText( pItemBase->GetProp() ) )
				((CUser*)this)->AddChatText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );									
			
		}
		else
			((CUser*)this)->AddDefinedText( TID_GAME_LACKSPACE );
	}
	
	if( ( (CItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( (CItemElem*)pItemBase )->m_nItemNum > 9 )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "R";
					aLogItem.SendName = "GROUND";
					aLogItem.RecvName = GetName();
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "R";
				aLogItem.SendName = "GROUND";
				aLogItem.RecvName = GetName();
				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}

#endif // WorldServer
	return bSuccess;
} // SubLootDropMobSingle

// DoLoot�� ���� - ���� ����Ʈ�� �������� ��Ƽ�� ������ ������ �Ҷ� ó����
BOOL CMover::SubLootDropMobParty( CItem *pItem, CParty *pParty )
{
	BOOL bSuccess = TRUE;;
#ifdef __WORLDSERVER
	CUser *pListMember[ 8 ];
	int	nMaxListMember = 0;
	CUser* pMember	= NULL;
	CUser* pGetUser = NULL;
//	float fDist;
	D3DXVECTOR3	vDist;
	CItemBase* pItemBase = pItem->m_pItemBase;
	
	// �������ݴ»�� �ݰ�ȿ� ��� ����鸸 �߷�����.
	memset( pListMember, 0, sizeof(pListMember) );
	for( int i = 0; i < pParty->m_nSizeofMember; i++ )
	{
		pMember		= g_UserMng.GetUserByPlayerID( pParty->m_aMember[i].m_uPlayerId );
//		if( IsValidObj( (CObj*)pMember ) )
		// 12-3 ˬ: ������ ȹ���� ���� �� ���� �ݰ濡 ���� ����ڷ� �����ؾ��Ѵ�.
		/*  
		if( IsValidObj( (CObj*)pMember ) && GetWorld() == pMember->GetWorld() )
		{
			vDist = pMember->GetPos() - GetPos();
			fDist = D3DXVec3LengthSq( &vDist );		// ������ �ݴ»���� ������� �Ÿ�.
			if( fDist < 32.0f * 32.0f )		// xx���ͺ��� ������
			{
				pListMember[ nMaxListMember++ ] = pMember;
			}
		}
		*/ //2009.01.21 // �ݰ� �˻翡 ���̾� �߰�
		if( IsValidArea( pMember, 32.0f ) )
			pListMember[ nMaxListMember++ ] = pMember;
	}

	if( nMaxListMember == 0 )
	{
		pGetUser	= (CUser*)this;
	}
	else
	{
		switch( pParty->m_nTroupeShareItem )
		{
		case 1 :	// �����й�

			{
				for( int i = 0 ; i < nMaxListMember ; i++ )
				{
					if( pParty->m_nGetItemPlayerId == pListMember[i]->m_idPlayer )
					{
						if( i + 1 >= nMaxListMember )
						{
							pGetUser = pListMember[0];
							break;												
						}
						else
						{
							pGetUser = pListMember[ i+1 ];
							break;
						}
					}
				}
				
				if( pGetUser == NULL )
				{
					pGetUser = pListMember[0];
				}
			}
			break;
		case 2 :	// �����й� : ���忡�� �� ��
			{
				if( pParty->IsLeader( pListMember[0]->m_idPlayer ) )
				{
					pGetUser = pListMember[0];
				}
				else
				{
					pGetUser = (CUser*)this;
				}
			}
			break;
		case 3 :	// �����й� : �����ϰ� ��
			{
				DWORD dwRand = xRandom( nMaxListMember );
				pGetUser = pListMember[dwRand];
			}
			break;
		default:	// �⺻ ���Ӻй�

			{
				pGetUser = (CUser*)this; 
			}
			break;
		}	// switch
	}	
	pParty->m_nGetItemPlayerId = pGetUser->m_idPlayer;

	bSuccess	= pGetUser->CreateItem( pItemBase );

	for( int i = 0; i < nMaxListMember; i++ )
	{
		pMember = pListMember[i];
		if( TRUE == pMember->IsPlayer() && pItemBase->GetProp() != NULL )
		{
			if( bSuccess )
			{
				if( pMember->m_idPlayer != pGetUser->m_idPlayer )
				{
					if( ::GetLanguage() == LANG_TWN || ::GetLanguage() == LANG_JAP || ::GetLanguage() == LANG_HK )
						( (CUser*)pMember )->AddDefinedText( TID_GAME_TROUPEREAPITEM, "\"%s\" \"%s\"", pGetUser->GetName(), pItemBase->GetProp()->szName );
					else
					{
						if( ::GetLanguage() == LANG_THA )
							( (CUser*)pMember )->AddDefinedText( TID_GAME_REAPITEM_THA, "\"%s\" \"%s\"", pGetUser->GetName(), pItemBase->GetProp()->szName );
						else
							( (CUser*)pMember )->AddDefinedText( TID_GAME_REAPITEM, "\"%s%s %s\"", pGetUser->GetName(), prj.GetText(TID_GAME_FROM2), pItemBase->GetProp()->szName );
					}
				}
				else
				{
					( (CUser*)pGetUser )->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );
				}
			}
			else
			{
				// CreateItem�� ������ Full
				if( pGetUser == pMember )
					( (CUser*)pGetUser )->AddDefinedText( TID_GAME_LACKSPACE );
			}
		}									
	}
	
	if( ( (CItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( ( (CItemElem*)pItemBase )->m_nItemNum ) > 9 )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "R";
					aLogItem.SendName = "GROUND";
					aLogItem.RecvName = pGetUser->GetName();
					aLogItem.WorldId = pGetUser->GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = pGetUser->GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "R";
				aLogItem.SendName = "GROUND";
				aLogItem.RecvName = pGetUser->GetName();
				aLogItem.WorldId = pGetUser->GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = pGetUser->GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}
#endif // __WORLDSERVER
	return bSuccess;
} // SubLootDropMobParty

//
//
// DoLoot�� ���� - ���� ����Ʈ�� ������ ���ú�...
BOOL CMover::SubLootDropMob( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER
	CParty *pParty	= g_PartyMng.GetParty( m_idparty );
	if( pParty && pParty->IsMember( m_idPlayer )  )	// ��Ƽ�� �����Ƿ� ����ġ �й��� �ؾ���
	{
		// ��Ƽ�� ������� ���� ó��.
		bSuccess = SubLootDropMobParty( pItem, pParty );
	}
	else // ��Ƽ�� ������~
	{
		bSuccess = SubLootDropMobSingle( pItem );
	}
#endif	// __WORLDSERVER

	return bSuccess;
}

// DoLoot�� ���� - ���� ����Ʈ���� �ƴ�! �����۷��ú�...
BOOL CMover::SubLootDropNotMob( CItem *pItem )
{
	BOOL bSuccess = TRUE;
#ifdef __WORLDSERVER
	CItemBase* pItemBase = pItem->m_pItemBase;
	
	bSuccess	= CreateItem( pItemBase );
	if( TRUE == IsPlayer() && pItemBase->GetProp() != NULL )
	{
		if( bSuccess )
		{
			((CUser*)this)->AddDefinedText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );						

			if( IsLootSendText( pItemBase->GetProp() ) )
				((CUser*)this)->AddChatText( TID_GAME_REAPITEM, "\"%s\"", pItemBase->GetProp()->szName );									
		}
		else
			((CUser*)this)->AddDefinedText( TID_GAME_LACKSPACE );
	}
	
	if( ( (CItemElem*)pItemBase )->IsLogable() && bSuccess )
	{
		switch( pItemBase->GetProp()->dwItemKind2 ) 
		{
		case IK2_FOOD:
		case IK2_MAGIC:
		case IK2_POTION:
		case IK2_REFRESHER:
			{
				if( ( (CItemElem*)pItemBase )->m_nItemNum > 9 )
				{
					LogItemInfo aLogItem;
					aLogItem.Action = "R";
					aLogItem.SendName = "GROUND";
					aLogItem.RecvName = GetName();
					aLogItem.WorldId = GetWorld()->GetID();
					aLogItem.Gold = aLogItem.Gold2 = GetGold();
					g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				}
				break;
			}
		default:
			{
				LogItemInfo aLogItem;
				aLogItem.Action = "R";
				aLogItem.SendName = "GROUND";
				aLogItem.RecvName = GetName();
				aLogItem.WorldId = GetWorld()->GetID();
				aLogItem.Gold = aLogItem.Gold2 = GetGold();
				g_DPSrvr.OnLogItem( aLogItem, ( (CItemElem*)pItemBase ), ( (CItemElem*)pItemBase )->m_nItemNum );
				break;
			}
		}
	}
#endif // __WORLDSERVER
	
	return bSuccess;
} // SubLootDropNotMob

//////////////////////////////////////////////////////////////////////////
//
// �ٴڿ� ������ pItem�� ���´�.
//
//////////////////////////////////////////////////////////////////////////
BOOL CMover::DoLoot( CItem *pItem )
{
	if( IsPlayer() == FALSE )	return FALSE;

	CItemBase* pItemBase = pItem->m_pItemBase;
	BOOL bSuccess = TRUE;

#ifdef __CLIENT	
	PLAYSND( SND_INF_GROUNDPICKUP );
#endif
	
	ItemProp *pItemProp = pItem->GetProp();
	if( pItemProp->dwItemKind1 == IK1_GOLD ) 
	{
		PickupGold( ((CItemElem*)pItemBase)->GetGold(), pItem->m_bDropMob );
	}
	else 
	{
		// <������ �α� - ȹ��>
#ifdef __WORLDSERVER
		if( pItem->m_bDropMob == TRUE ) // ���Ͱ� ����߸��͸�
		{
			// ���� ����Ʈ�� �����ۿ� ���� ���� ó��
			bSuccess = SubLootDropMob( pItem );
		}
		else	// ������� ����߸����� �������� ����~
		{
			// ���� ����Ʈ���� �ƴ� �������� ���ÿ� ���� ó��
			bSuccess = SubLootDropNotMob( pItem );

		}
#endif	// __WORLDSERVER
	}

	if( TRUE == bSuccess )
	{
		pItem->Delete();
#ifdef __WORLDSERVER
//		g_UserMng.AddMotion( this, OBJMSG_PICKUP );
		RemoveInvisible();		// ������ ������ ���� ����.
#endif	// __WORLDSERVER
	}

	return bSuccess;
}

