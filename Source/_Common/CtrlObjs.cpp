#include "stdafx.h"
#include "CtrlObjs.h"
#include "defineSkill.h"
#include "user.h"
#include "dpsrvr.h"
#include "Party.h"
#include "dpDatabaseClient.h"

 
extern	CUserMng	g_UserMng;
extern	CDPSrvr		g_DPSrvr;
extern	CPartyMng	g_PartyMng;
extern	CDPDatabaseClient	g_dpDBClient;


void CContDamageCtrl::Init( void )
{
	m_tmStart = m_tmUpdate = timeGetTime();
	m_pSkillProp = NULL;
	m_pAddSkillProp = NULL;
	m_idTarget = m_idSrc = NULL_ID;
#if __VER >= 8 // __S8_PK
	m_bControl = FALSE;
#endif // __VER >= 8 // __S8_PK
	// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
	m_bDuelTarget = FALSE;
}
 
void CContDamageCtrl::Destroy( void )
{
	Init();
}
 
void CContDamageCtrl::ApplyDamage( void )
{
	ItemProp *pSkillProp = m_pSkillProp;
	AddSkillProp *pAddSkillProp = m_pAddSkillProp;
	
	CCtrl *pSrc = prj.GetCtrl( m_idSrc );			// ������.  �߰��� ���������� �ֽ�.
	if( IsInvalidObj(pSrc) )
	{
		// �̶� ó���� �ؾ��Ѵ�.
		Delete();
		return;
	}

	// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
	if( pSrc->GetType() == OT_MOVER && !((CMover*)pSrc)->m_nDuel && m_bDuelTarget )
		return;
	
	CCtrl *pCenter = prj.GetCtrl( m_idTarget );		// �߽��� �Ǵ� ������Ʈ.  �߰��� ���������� �ֽ�.
	D3DXVECTOR3 vPos = GetPos();					// �߽���ǥ
	int nApplyType;
	nApplyType = OBJTYPE_PLAYER | OBJTYPE_MONSTER;
#if __VER >= 8 // __S8_PK
	pSrc->ApplySkillRegion( vPos, nApplyType, pSkillProp, pAddSkillProp, false, TRUE, pCenter, m_bControl );
#else // __VER >= 8 // __S8_PK
	pSrc->ApplySkillRegion( vPos, nApplyType, pSkillProp, pAddSkillProp, false, TRUE, pCenter );
#endif // __VER >= 8 // __S8_PK
}

 // ���ӵ������� ��.
void CContDamageCtrl::Process()
{
	ItemProp *pSkillProp = m_pSkillProp;
	AddSkillProp *pAddSkillProp = m_pAddSkillProp;
	
	if( pSkillProp->tmContinuousPain == NULL_ID )
		Error( "%s�� SkillProp tmContinuousPain�� ���� -1", pSkillProp->dwID );

	if( (int)(g_tmCurrent - m_tmUpdate) > (int)pSkillProp->tmContinuousPain )		// �����ð��� ������
	{
		m_tmUpdate = g_tmCurrent;		// �ð� �����ϰ�
		// ���ӵ����� ó���� �̰��� ����.
		ApplyDamage();
	}

	if( (int)(g_tmCurrent - m_tmStart) > (int)m_pAddSkillProp->dwSkillTime )		// �ѽð��� ������ ��.
		Delete();
}
 
	

