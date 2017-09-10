#include "stdafx.h"
#include "defineText.h"
#include "defineSkill.h"
#include "party.h"

#ifdef __CORESERVER
#include "dpcoresrvr.h"
extern	CDPCoreSrvr g_dpCoreSrvr;
#include "player.h"
extern	CPlayerMng	g_PlayerMng;
#endif // __CORESERVER

#ifdef __WORLDSERVER
#include "worldmng.h"
#include "User.h"
extern	CUserMng	g_UserMng;
extern CWorldMng   g_WorldMng;
#include "dpcoreclient.h"
extern	CDPCoreClient	g_DPCoreClient;
#include "dpdatabaseclient.h"
extern	CDPDatabaseClient	g_dpDBClient;
#if __VER >= 12 // __LORD
#include "slord.h"
#endif	// __LORD
#endif	// __WORLDSERVER

#if __VER >= 11 // __SYS_PLAYER_DATA
#include "playerdata.h"
#endif	// __SYS_PLAYER_DATA

#ifndef __VM_0820
#ifndef __MEM_TRACE
	#ifdef __VM_0819
	MemPooler<CParty>*	CParty::m_pPool		= new MemPooler<CParty>( 512, "CParty" );
	#else	// __VM_0819
	MemPooler<CParty>*	CParty::m_pPool		= new MemPooler<CParty>( 512 );
	#endif	// __VM_0819
#endif	// __MEM_TRACE
#endif	// __VM_0820

#if __VER >= 14 // __INSTANCE_DUNGEON
#include "InstanceDungeonParty.h"
#endif // __INSTANCE_DUNGEON
/*--------------------------------------------------------------------------------*/

CParty::CParty()
{
	m_uPartyId	= 0;
	m_nSizeofMember	= 0;
	memset( m_sParty, 0, sizeof(m_sParty) );
	m_nLevel = 1;
	m_nExp = m_nPoint = 0;
	m_nTroupsShareExp = m_nTroupeShareItem = 0;
	m_nKindTroup = 0;							// �ܸ��ش�
	m_nReferens = 0;
	m_nGetItemPlayerId = 0;
	for( int i = 0 ; i < MAX_PARTYMODE ; i++ )
	{
		m_nModeTime[i] = 0;
	}
	m_idDuelParty = 0;
#ifdef __WORLDSERVER
	m_dwWorldId = 0;
#endif // __WORLDSERVER
}

CParty::~CParty()
{
}

void CParty::InitParty()
{
	m_uPartyId	= 0;
	m_nSizeofMember	= 0;
	memset( m_sParty, 0, sizeof(m_sParty) );
	m_nLevel = 1;
	m_nExp = m_nPoint = 0;
	m_nTroupsShareExp = m_nTroupeShareItem = 0;
	m_nKindTroup = 0;							// �ܸ��ش�
	m_nReferens = 0;	
	for( int i = 0 ; i < MAX_PTMEMBER_SIZE ; i++ )
	{
		m_aMember[i].m_uPlayerId	= 0;
		m_aMember[i].m_tTime = CTime::GetCurrentTime();
		m_aMember[i].m_bRemove = FALSE;
#if __VER < 11 // __SYS_PLAYER_DATA
		m_aMember[i].m_nLevel = m_aMember[i].m_nJob = 0;
		m_aMember[i].m_szName[0] = '\0';
#endif	// __SYS_PLAYER_DATA
	}
}

#ifndef __CORESERVER
CMover* CParty::GetLeader( void ) 
{ 
#ifdef __WORLDSERVER
	CMover *pLeader = (CMover *)g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
	return pLeader;
#else
	CMover *pLeader = (CMover *)prj.GetUserByID( m_aMember[0].m_uPlayerId );	// ������ ������
	return pLeader;
#endif
}
#endif // __CORESERVER

int CParty::FindMember( u_long uPlayerId )
{
	for( int i = 0; i < m_nSizeofMember; i++ )
		if( m_aMember[i].m_uPlayerId == uPlayerId )
			return i;
	return -1;
}

#if __VER >= 11 // __SYS_PLAYER_DATA
BOOL CParty::NewMember( u_long uPlayerId )
#else	// __SYS_PLAYER_DATA
BOOL CParty::NewMember( u_long uPlayerId, LONG nLevel, LONG nJob, BYTE nSex, LPSTR szName )
#endif	// __SYS_PLAYER_DATA
{
#if __VER < 11 // __SYS_PLAYER_DATA
	if( szName == NULL )
		return FALSE;
#endif	// __SYS_PLAYER_DATA
	
	if( IsMember( uPlayerId ) == FALSE && m_nSizeofMember < MAX_PTMEMBER_SIZE )
	{
		m_aMember[m_nSizeofMember].m_uPlayerId = uPlayerId;
#if __VER < 11 // __SYS_PLAYER_DATA
		m_aMember[m_nSizeofMember].m_nLevel = nLevel;
		m_aMember[m_nSizeofMember].m_nJob = nJob;
		m_aMember[m_nSizeofMember].m_nSex = nSex;
#endif	// __SYS_PLAYER_DATA
		m_aMember[m_nSizeofMember].m_bRemove	= FALSE;
#if __VER < 11 // __SYS_PLAYER_DATA
		strcpy( m_aMember[m_nSizeofMember].m_szName, szName );
#endif	// __SYS_PLAYER_DATA
		m_nSizeofMember++;
		return TRUE;
	}
	return FALSE;
}

BOOL CParty::DeleteMember( u_long uPlayerId )
{
	int Findid = FindMember( uPlayerId );
	if( Findid >= 0)
	{
		for( int i = Findid ; i < m_nSizeofMember-1 ; i++ )
		{
			memcpy( &m_aMember[i], &m_aMember[i+1], sizeof(PartyMember) );
		}
		m_nSizeofMember--;
#ifdef __WORLDSERVER
#if __VER >= 14 // __INSTANCE_DUNGEON
		CInstanceDungeonParty::GetInstance()->SetPartyLeaveTime( uPlayerId );
#endif // __INSTANCE_DUNGEON
#endif // __WORLDSERVER
		return TRUE;
	}
	return FALSE;
}

void CParty::ChangeLeader( u_long uLeaderId )
{
	int changeIndex = FindMember( uLeaderId );
	SwapPartyMember( 0, changeIndex );
}

void CParty::Serialize( CAr & ar )
{
	if( ar.IsStoring() )
	{
		ar << m_uPartyId << m_nKindTroup << m_nSizeofMember;
		ar << m_nLevel << m_nExp << m_nPoint;
		ar << m_nTroupsShareExp << m_nTroupeShareItem;
		ar << m_idDuelParty;
		for( int i = 0 ; i < MAX_PARTYMODE  ; i++ )
		{
			ar << m_nModeTime[i];
		}
		if( m_nKindTroup )
			ar.WriteString( m_sParty );
		for( int i = 0 ; i < m_nSizeofMember ; i++ )
		{
			ar << m_aMember[i].m_uPlayerId;
#if __VER < 11 // __SYS_PLAYER_DATA
			ar << m_aMember[i].m_nLevel;
			ar << m_aMember[i].m_nJob;
#endif	// __SYS_PLAYER_DATA
			ar << m_aMember[i].m_bRemove;
#if __VER < 11 // __SYS_PLAYER_DATA
			ar << m_aMember[i].m_nSex;
			ar.WriteString( m_aMember[i].m_szName );
#endif	// __SYS_PLAYER_DATA
		}
	}
	else
	{
		ar >> m_uPartyId >> m_nKindTroup >> m_nSizeofMember;
		ar >> m_nLevel >> m_nExp >> m_nPoint;
		ar >> m_nTroupsShareExp >> m_nTroupeShareItem;
		ar >> m_idDuelParty;
		for( int i = 0 ; i < MAX_PARTYMODE  ; i++ )
		{
			ar >> m_nModeTime[i];
		}
		if( m_nKindTroup )
			ar.ReadString( m_sParty, 33 );
		for( int i = 0 ; i < m_nSizeofMember ; i++ )
		{
			ar >> m_aMember[i].m_uPlayerId;
#if __VER < 11 // __SYS_PLAYER_DATA
			ar >> m_aMember[i].m_nLevel;
			ar >> m_aMember[i].m_nJob;
#endif	// __SYS_PLAYER_DATA
			ar >> m_aMember[i].m_bRemove;
#if __VER < 11 // __SYS_PLAYER_DATA
			ar >> m_aMember[i].m_nSex;
			ar.ReadString( m_aMember[i].m_szName, 20 );
#endif	// __SYS_PLAYER_DATA
		}
	}
}

void CParty::SwapPartyMember( int first, int Second )
{
	PartyMember PartyMemberBuf;

	memcpy( &PartyMemberBuf, &m_aMember[first], sizeof(PartyMember) );
	memcpy( &m_aMember[first], &m_aMember[Second], sizeof(PartyMember) );
	memcpy( &m_aMember[Second], &PartyMemberBuf, sizeof(PartyMember) );
}

#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
int CParty::GetPartyModeTime( int nMode )
{
	return m_nModeTime[nMode];	
}
void CParty::SetPartyMode( int nMode, DWORD dwSkillTime ,int nCachMode )
{
	if( nCachMode == 1)
        m_nModeTime[nMode] += (int)dwSkillTime;	
	else
		m_nModeTime[nMode] = (int)dwSkillTime;	
}
#else	//12�� �ش����������
void CParty::SetPartyMode( int nMode, DWORD dwSkillTime )
{
	m_nModeTime[nMode] = (int)dwSkillTime;	
}
#endif // //12�� �ش����������

#ifdef __WORLDSERVER
void CParty::SetPartyLevel( CUser* pUser, DWORD dwLevel, DWORD dwPoint, DWORD dwExp )
{
	g_DPCoreClient.SendPartyLevel( pUser, dwLevel, dwPoint, dwExp );
}
#endif // __WORLDSERVER

void CParty::GetPoint( int nTotalLevel, int nMemberSize, int nDeadLeavel )
{
#ifdef __WORLDSERVER
#ifndef __PARTYDEBUG
	if( (nTotalLevel / nMemberSize) - nDeadLeavel < 5 )
#endif // __PARTYDEBUG
	{
		BOOL bExpResult = TRUE;
		BOOL bSuperLeader = FALSE;
#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
		BOOL bLeaderSMExpUp = FALSE;
#endif // //12�� �ش����������
		if( m_nKindTroup == 0 && m_nLevel >= MAX_PARTYLEVEL )
			bExpResult = FALSE;

		CMover* pMover = GetLeader();
		if( pMover && pMover->HasBuff( BUFF_ITEM, II_SYS_SYS_SCR_SUPERLEADERPARTY ) )
		{
			bSuperLeader = TRUE;
		}
#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
		#define II_SYS_SYS_SCR_PARTYEXPUP01_01 20296
		#define II_SYS_SYS_SCR_PARTYSKILLUP01_01 20297
		if( pMover && ( pMover->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP01 ) || pMover->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP02 )
			 || pMover->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYEXPUP01_01 ) ) )
		{
			bLeaderSMExpUp = TRUE;
		}
#endif // //12�� �ش����������

		if( bExpResult )
#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
			g_DPCoreClient.SendAddPartyExp( m_uPartyId, nDeadLeavel, bSuperLeader , bLeaderSMExpUp );
#else // //12�� �ش����������
			g_DPCoreClient.SendAddPartyExp( m_uPartyId, nDeadLeavel, bSuperLeader );
#endif // //12�� �ش����������
	}
#endif // __WORLDSERVER
}

void CParty::DoUsePartySkill( u_long uPartyId, u_long uLeaderid, int nSkill )
{
#ifdef __WORLDSERVER
	CUser* pMember	= NULL;
	int		i;
	
	if( IsLeader( uLeaderid ) && m_nKindTroup == 1 )
	{
		ItemProp* pItemProp =  prj.GetPartySkill( nSkill );
		if( pItemProp )
		{
#ifndef __PARTYDEBUG
			if( int( GetLevel() - pItemProp->dwReqDisLV ) >= 0 )
#endif // __PARTYDEBUG
			{
#ifndef __PARTYDEBUG
	#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
				CUser *pLeadertmp = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
				if( IsValidObj( pLeadertmp ) == FALSE )
					return;
				int nHasCashSkill = 0,nFPoint = 0;
				if( pLeadertmp->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP01 ) 
					|| pLeadertmp->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP02 )
					|| pLeadertmp->HasBuff( BUFF_ITEM2, II_SYS_SYS_SCR_PARTYSKILLUP01_01 ) )
				{
					if( nSkill == ST_LINKATTACK 
						|| nSkill == ST_FORTUNECIRCLE 
						|| nSkill == ST_STRETCHING
						|| nSkill == ST_GIFTBOX )
						nHasCashSkill = 1;
				}

				DWORD dwSkillTime	= pItemProp->dwSkillTime;
				int nRemovePoint	= pItemProp->dwExp;

				nFPoint	= int( GetPoint() - pItemProp->dwExp);
#if __VER >= 12 // __LORD
				// ������ �ش�
				// ���ְ� �ش��� ���ν� �شܽ�ų ��� ��,
				// ���ӽð� 4�� ����(�Ҹ� ����Ʈ�� ����)
				if( CSLord::Instance()->IsLord( uLeaderid ) )
					dwSkillTime		*= 4;
#endif	// __LORD

				if( nFPoint >= 0 )
	#else
				if( int( GetPoint() - pItemProp->dwExp) >= 0 )
	#endif // //12�� �ش����������
#endif // __PARTYDEBUG
				{
					switch( nSkill )
					{
					case ST_CALL:
						{
							g_DPCoreClient.SendRemovePartyPoint( uPartyId, pItemProp->dwExp );
							CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
							if( IsValidObj( pLeader ) == FALSE )
								break;

							// ���� �߽����� �𿩴޶�� ǥ��
							for( i = 0; i < m_nSizeofMember; i ++ )		// ����(0)���Դ� �����ʿ� ����.
							{
								pMember		= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
								// ���� ����� ������ �ִ� ������Ը� ������
								if( IsValidObj( (CObj*)pMember ) )
									pMember->AddPartySkillCall( pLeader->GetPos() );		// �� ����鿡�� ������ ��ǥ�� ������.
							}
						}
						break;
					case ST_BLITZ:
						{
							CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
							if( IsValidObj( pLeader ) == FALSE )
								break;

							if( pLeader->m_idSetTarget != NULL_ID )
							{
								CMover *pT = prj.GetMover( pLeader->m_idSetTarget );
								if( pT && !pT->IsPlayer() )
								{
									g_DPCoreClient.SendRemovePartyPoint( uPartyId, pItemProp->dwExp );
									// ������ Ÿ������ ���߰��� ǥ��
									for( i = 0; i < m_nSizeofMember; i ++ )
									{
										pMember		= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
										if( IsValidObj( (CObj*)pMember ) )
										{
											if( m_nModeTime[PARTY_GIFTBOX_MODE] || m_nModeTime[PARTY_FORTUNECIRCLE_MODE] )
											{
												pMember->AddPartySkillBlitz( pLeader->m_idSetTarget );		// �� ����鿡�� ����Ÿ������ ���� ������ ���̵� ����.
											}
											else
											{
												if( pLeader->IsNearPC( pMember ) )
													pMember->AddPartySkillBlitz( pLeader->m_idSetTarget );		// �� ����鿡�� ����Ÿ������ ���� ������ ���̵� ����.
											}
										}
									}
								}
								else
								{
									pLeader->AddSendErrorParty( ERROR_NOTTARGET, ST_BLITZ );
								}
							}
							else
							{
								pLeader->AddSendErrorParty( ERROR_NOTTARGET, ST_BLITZ );
								// Ÿ���� ����Ҵ�.
							}
						}
						break;
					case ST_RETREAT:
						{
							g_DPCoreClient.SendRemovePartyPoint( uPartyId, pItemProp->dwExp );
							// ���� ǥ��
							CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
							if( IsValidObj( pLeader ) == FALSE )
								break;

							for( i = 0; i < m_nSizeofMember; i ++ )
							{
								pMember		= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
								if( IsValidObj( (CObj*)pMember ) )
								{
									if( m_nModeTime[PARTY_GIFTBOX_MODE] || m_nModeTime[PARTY_FORTUNECIRCLE_MODE] )
									{
										pMember->AddHdr( pMember->GetId(), SNAPSHOTTYPE_PARTYSKILL_RETREAT );
									}
									else
									{
										if( pLeader->IsNearPC( pMember ) )
											pMember->AddHdr( pMember->GetId(), SNAPSHOTTYPE_PARTYSKILL_RETREAT );
									}
								}
							}
						}
						break;
					case ST_SPHERECIRCLE:
						{
							// ũ��Ƽ�� Ȯ��
							CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
							if( IsValidObj( pLeader ) == FALSE )
								break;
							
							if( pLeader->m_idSetTarget != NULL_ID )
							{
								CMover * pT = prj.GetMover( pLeader->m_idSetTarget );
								if( pT && !pT->IsPlayer() )
								{
									g_DPCoreClient.SendRemovePartyPoint( uPartyId, pItemProp->dwExp );
									for( i = 0; i < m_nSizeofMember; i ++ )
									{
										pMember		= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
										if( IsValidObj( (CObj*)pMember ) )
										{
											if( m_nModeTime[PARTY_GIFTBOX_MODE] || m_nModeTime[PARTY_FORTUNECIRCLE_MODE] )
											{
												pMember->AddHdr( pLeader->m_idSetTarget, SNAPSHOTTYPE_PARTYSKILL_SPHERECIRCLE );
												pMember->m_dwFlag |= MVRF_CRITICAL;
											}
											else
											{
												if( pLeader->IsNearPC( pMember ) )
												{
													pMember->AddHdr( pLeader->m_idSetTarget, SNAPSHOTTYPE_PARTYSKILL_SPHERECIRCLE );
													pMember->m_dwFlag |= MVRF_CRITICAL;
												}
											}
										}
									}
								}
								else
								{
									pLeader->AddSendErrorParty( ERROR_NOTTARGET, ST_SPHERECIRCLE );
									// Ÿ���� ���Ͱ� �ƴϴ�
								}
							}
							else
							{
								pLeader->AddSendErrorParty( ERROR_NOTTARGET, ST_SPHERECIRCLE );
								// Ÿ���� ����Ҵ�.
							}
//							g_DPCoreClient.SendSetPartyExp( uLeaderid, m_nPoint );
						}
						break;

#if __VER >= 12 // __JHMA_VER12_1	//12�� �ش����������
					case ST_LINKATTACK:
						{
							// ������ ����
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_LINKATTACK_MODE, dwSkillTime, nRemovePoint, nHasCashSkill );
						}
						break;
					case ST_FORTUNECIRCLE:
						{
							// ����ũ ������ �߻�Ȯ�� ����
								g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_FORTUNECIRCLE_MODE, dwSkillTime, nRemovePoint, nHasCashSkill );
						}
						break;
					case ST_STRETCHING:
						{
							// ���°�� ȸ���ӵ� ������
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_STRETCHING_MODE, dwSkillTime, nRemovePoint, nHasCashSkill );
						}
						break;
					case ST_GIFTBOX:
						{
							// ������ ���� �ι�
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_GIFTBOX_MODE, dwSkillTime, nRemovePoint, nHasCashSkill );
						}
						break;
#else	//12�� �ش����������
					case ST_LINKATTACK:
						{
							// ������ ����
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_LINKATTACK_MODE, dwSkillTime, nRemovePoint );
						}
						break;
					case ST_FORTUNECIRCLE:
						{
							// ����ũ ������ �߻�Ȯ�� ����
								g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_FORTUNECIRCLE_MODE, dwSkillTime, nRemovePoint );
						}
						break;
					case ST_STRETCHING:
						{
							// ���°�� ȸ���ӵ� ������
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_STRETCHING_MODE, dwSkillTime, nRemovePoint );
						}
						break;
					case ST_GIFTBOX:
						{
							// ������ ���� �ι�
							g_DPCoreClient.SendUserPartySkill( uLeaderid, PARTY_GIFTBOX_MODE, dwSkillTime, nRemovePoint );
						}
						break;
#endif // //12�� �ش����������
					default:
						break;
					}
				}
#ifndef __PARTYDEBUG
				else
				{
					CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
					if( IsValidObj( pLeader ) )
					{
						pLeader->AddSendErrorParty( ERROR_NOTPARTYPOINT );
					}
					//����Ʈ�� ���ڶ� ��ų������Ҽ� �����ϴ�.
					
				}
#endif // __PARTYDEBUG
			}
#ifndef __PARTYDEBUG
			else
			{
				CUser *pLeader = g_UserMng.GetUserByPlayerID( m_aMember[0].m_uPlayerId );	// ������ ������
				if( IsValidObj( pLeader ) )
				{
					pLeader->AddSendErrorParty( ERROR_NOTPARTYSKILL );
				}
				//�� ��ų�� ������� ���߽��ϴ�.
			}
#endif // __PARTYDEBUG
		}
	}
	else
	{
		// ������ �ƴϰų� ��ȸ�ش��� �ƴѰ��
	}
#endif	// __WORLDSERVER
}

//
//
// idDstParty�� this��Ƽ�� ������ �����ߴٴ°� ��� ����鿡�� �˸�.
#ifdef __WORLDSERVER
void CParty::DoDuelPartyStart( CParty *pDst )
{
	int		i, j;
	OBJID	pDstMember[ MAX_PTMEMBER_SIZE ];
	memset( pDstMember, 0xff, sizeof(pDstMember) );
	
	m_idDuelParty = pDst->m_uPartyId;		// this��Ƽ�� ���
	
	CMover *pMember, *pMember2;

	LPCSTR pszLeader;
	if( pDst->m_nKindTroup )	// ��ȸ�ش� �ش� �̸� ����
	{
		pszLeader = pDst->m_sParty;
	}
	else	// �ܸ��ش� �϶��� �����̸��� ����
	{
#if __VER >= 11 // __SYS_PLAYER_DATA
		pszLeader	= CPlayerDataCenter::GetInstance()->GetPlayerString( pDst->GetPlayerId( 0 ) );
#else	// __SYS_PLAYER_DATA
		pszLeader	= prj.GetPlayerString( pDst->GetPlayerId( 0 ) );
#endif	//__SYS_PLAYER_DATA
	}
	
	if( !pszLeader )
	{
		Error( "Error Party PVP 1" );
		return;
	}

	for( i = 0; i < m_nSizeofMember; i ++ )		// �شܿ��� ����.
	{
		pMember	= (CMover *)g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
		if( IsValidObj( pMember ) )
		{
			pMember->m_nDuel = 2;		// 2�� ��Ƽ�����.
			pMember->m_idDuelParty = m_idDuelParty;
			pMember->m_nDuelState = 104;
			// ���� ��Ƽ�� ������̵� �� ����.
			for( j = 0; j < pDst->m_nSizeofMember; j ++ )
			{
				pMember2 = (CMover *)g_UserMng.GetUserByPlayerID( pDst->m_aMember[j].m_uPlayerId );
				if( IsValidObj(pMember2) )
					pDstMember[j] = pMember2->GetId();
			}
			// ��� ��Ƽ�� ���� ������ �츮����鿡�� ����
			((CUser *)pMember)->AddDuelPartyStart( pszLeader , pDst->m_nSizeofMember, pDstMember, pDst->m_uPartyId );		// ���� ����� ID�� �ٺ�����.
		}
	}
}	

//
// ��Ƽ���鿡�� bWin�� ���� ��/�� ����� �˸��� ����� �����Ŵ.
//
void CParty::DoDuelResult( CParty *pDuelOther, BOOL bWin, int nAddFame, float fSubFameRatio )
{
	CUser *pMember;
	int		i;
	
	for( i = 0; i < m_nSizeofMember; i ++ )
	{
		pMember = g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
		if( IsValidObj( pMember ) )
		{
			pMember->AddDuelPartyResult( pDuelOther, bWin );		// �� ����鿡�� ��/�� ����� �˸�. / �����Ƽ�� ����Ʈ�� ����.
			pMember->ClearDuelParty();

			if( bWin )
			{
				pMember->m_nFame += nAddFame;	// �̱����̸� �� ����
				pMember->AddDefinedText( TID_GAME_GETFAMEPOINT, "%d", nAddFame );	// xxx �� ����Ʈ�� ����
#ifdef __WORLDSERVER
				g_dpDBClient.SendLogPkPvp( pMember, NULL, nAddFame, 'E' );
#endif // __WORLDSERVER
			} 
			else
			{
				int nDecVal;
				fSubFameRatio = fSubFameRatio / 100.0f;	// ������ ȯ��.
				nDecVal = (int)(pMember->m_nFame * fSubFameRatio);
				if( nDecVal == 0 )	nDecVal = 1;
				pMember->m_nFame -= nDecVal;
				if( pMember->m_nFame < 0 )	pMember->m_nFame = 0;
				pMember->AddDefinedText( TID_GAME_DECFAMEPOINT, "%d", nDecVal );	// xxx �� ����Ʈ�� ����.
#ifdef __WORLDSERVER
				g_dpDBClient.SendLogPkPvp( NULL, pMember, nAddFame, 'E' );
#endif // __WORLDSERVER
			}
			
			// �ٲ� ��ġ�� pMember�� ������ ������.
			g_UserMng.AddSetFame( pMember, pMember->m_nFame );
		}
	}
	m_idDuelParty = 0;
}


void CParty::DoUsePartyReCall( u_long uPartyId, u_long uLeaderid, int nSkill )
{
	CUser* pMember	= NULL;
	int		i;

	CUser* pUser    = NULL;
	pUser = g_UserMng.GetUserByPlayerID( uLeaderid );
	if( !IsValidObj( (CObj*)pUser ) )
		return;
	
	for( i = 0; i < m_nSizeofMember; i ++ )		// ����(0)���Դ� �����ʿ� ����.
	{
		pMember		= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
		if( IsValidObj( (CObj*)pMember ) )
		{
			if(pMember->m_idPlayer != pUser->m_idPlayer)
#ifdef __LAYER_1015
				g_DPCoreClient.SendSummonPlayer( pUser->m_idPlayer, pUser->GetWorld()->GetID(), pUser->GetPos(), pMember->m_idPlayer, pUser->GetLayer() );
#else	// __LAYER_1015
				g_DPCoreClient.SendSummonPlayer( pUser->m_idPlayer, pUser->GetWorld()->GetID(), pUser->GetPos(), pMember->m_idPlayer );
#endif	// __LAYER_1015
		}
	}
}

#endif // worldserver

//
// �ش� ��� ����
// 
void CParty::DoDuelPartyCancel( CParty* pDuelParty )
{
#ifndef __CORESERVER
	int		i;
	
	CMover *pMember;
	for( i = 0; i < m_nSizeofMember; i ++ )		// �شܿ� ��ο��� ��� ������ �����ϰ� Ŭ�󿡵� �˸�.
	{
#ifdef __WORLDSERVER
		pMember	= (CMover *)g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
#else
#ifdef __CLIENT
		pMember	= prj.GetUserByID( m_aMember[i].m_uPlayerId );
#endif
#endif // worldserver
		if( IsValidObj( pMember ) )
		{
#ifdef __WORLDSERVER
			if( pDuelParty )
				((CUser *)pMember)->AddDuelPartyCancel( pDuelParty );		// �� ����鿡�� ����� ��ҵǾ��ٰ� �˸�.
			else
				((CUser *)pMember)->AddDuelPartyCancel( NULL );		// �� ����鿡�� ����� ��ҵǾ��ٰ� �˸�.
			if( pMember->m_idDuelParty != m_idDuelParty )
				Error( "CParty::DoDuelPartyCancel : ��Ƽ��� %s�� �����̻�. %d %d", pMember->GetName(), pMember->m_idDuelParty, m_idDuelParty );
			pMember->ClearDuelParty();
#endif // worldserver
		}
	}
	m_idDuelParty = 0;		// ��Ƽ ����
#endif	// __CORESERVER
}	

#ifdef __WORLDSERVER
void CParty::ReplaceLodestar( const CRect &rect )
{
	// locked
	CUser* pUser;
	for( int i = 0; i < m_nSizeofMember; i ++ )		// �شܿ� ��ο��� ��� ������ �����ϰ� Ŭ�󿡵� �˸�.
	{
		pUser	= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );

		if( IsValidObj( pUser ) )
		{
			CWorld* pWorld	= pUser->GetWorld();
			if( pWorld )
			{
				POINT point	= { (int)pUser->GetPos().x, (int)pUser->GetPos().z	};
				if( rect.PtInRect( point ) )
				{
					PRegionElem pRgnElem	= NULL;
					if( pWorld->GetID() != pWorld->m_dwIdWorldRevival && pWorld->m_dwIdWorldRevival != 0 )
						pRgnElem	= g_WorldMng.GetRevivalPos( pWorld->m_dwIdWorldRevival, pWorld->m_szKeyRevival );
					if( !pRgnElem )	// Find near revival pos
						pRgnElem	= g_WorldMng.GetNearRevivalPos( pWorld->GetID(), pUser->GetPos() );
					if( pRgnElem )
						pUser->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
				}
			}
		}
	}
}

void CParty::Replace( DWORD dwWorldId, D3DXVECTOR3 & vPos, BOOL bMasterAround )
{
	CUser *pMember;
	for( int i = 0; i < m_nSizeofMember; i ++ )		// �شܿ� ��ο��� ��� ������ �����ϰ� Ŭ�󿡵� �˸�.
	{
		pMember	= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );

		if( IsValidObj( pMember ) )
		{
			/*
			if( GetQuest( QUEST_BOSS_LV1 ) != NULL )
				pMember->UnequipRide();
			
			if( GetQuest( QUEST_BOSS_LV2 ) != NULL )
				pMember->UnequipRide();
			
			if( GetQuest( QUEST_BOSS_LV3 ) != NULL )
				pMember->UnequipRide();
			*/			
			pMember->REPLACE( g_uIdofMulti, dwWorldId, vPos, REPLACE_NORMAL, nTempLayer );
#ifdef __BUFF_1107
			pMember->m_buffs.RemoveBuffs( RBF_COMMON, 0 );
#else	// __BUFF_1107
			pMember->m_SkillState.RemoveAllSkillInfluence();
#endif	// __BUFF_1107
		}
	}
}

void CParty::Replace( DWORD dwWorldId, LPCTSTR sKey )
{
	CUser *pMember;
	for( int i = 0; i < m_nSizeofMember; i ++ )		// �شܿ� ��ο��� ��� ������ �����ϰ� Ŭ�󿡵� �˸�.
	{
		pMember	= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
		
		if( IsValidObj( pMember ) )
		{
			PRegionElem pRgnElem = g_WorldMng.GetRevivalPos( dwWorldId, sKey );
			if( NULL != pRgnElem )
				pMember->REPLACE( g_uIdofMulti, pRgnElem->m_dwWorldId, pRgnElem->m_vPos, REPLACE_NORMAL, nRevivalLayer );
		}
	}
}

BOOL CParty::ReplaceChkLv( int Lv )
{
	CUser *pMember;
	for( int i = 0; i < m_nSizeofMember; i ++ )		// �شܿ� ��ο��� ��� ������ �����ϰ� Ŭ�󿡵� �˸�.
	{
		pMember	= g_UserMng.GetUserByPlayerID( m_aMember[i].m_uPlayerId );
		
		if( IsValidObj( pMember ) )
		{
			if( pMember->GetLevel() > Lv )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
#endif //__WORLDSERVER



CParty g_Party;
/*--------------------------------------------------------------------------------*/
#ifndef __CLIENT

CPartyMng::CPartyMng()
{
	m_id	= 0;
#ifdef __WORLDSERVER
	m_nSecCount = 0;
#endif // __WORLDSERVER
//	m_2Party.SetSize( 1024, 4096, 1024 );
#ifdef __CORESERVER
	m_hWorker	= m_hCloseWorker	= NULL;
#endif // __CORESERVER
}

CPartyMng::~CPartyMng()
{
//	CPartyBucket* pBucket	= m_2Party.GetFirstActive();
//	while( pBucket )
//	{
//		delete pBucket->m_value;
//		pBucket		= pBucket->pNext;
//	}
//	m_2Party.RemoveAll();
	Clear();
}

void CPartyMng::Clear( void )
{
#ifndef __WORLDSERVER
	m_AddRemoveLock.Enter( theLineFile );	// lock1
#endif	// __WORLDSERVER

	for( C2PartyPtr::iterator i	= m_2PartyPtr.begin(); i != m_2PartyPtr.end(); ++i )
		safe_delete( i->second );

	m_2PartyPtr.clear();
	
#ifdef __CORESERVER
	CloseWorkers();
#endif // __CORESERVER

#ifndef __WORLDSERVER
	m_AddRemoveLock.Leave( theLineFile );	// unlock1
#endif	// __WORLDSERVER
}

// �ش� ����
// uLeaderPlayerId : ����, uPartyId : 1��° �ܿ�
// �شܸ� �����Ҷ��� 2������ ������( ȥ�ڼ��� �شܸ� �����Ҽ� ���� )
u_long	CPartyMng::NewParty( u_long uLeaderId, LONG nLeaderLevel, LONG nLeaderJob, BYTE nLeaderSex, LPSTR szLeaderName, u_long uMemberId, LONG nMemberLevel, LONG nMemberJob, BYTE nMemberSex, LPSTR szMembername, u_long uPartyId )
{
//	locked
	if( 0 == uPartyId )
	{
		m_id++;
	}
	else
	{
		m_id = uPartyId;
	}

	
	if( NULL == GetParty( m_id ) ) // NULL �̸� �ش��� �����Ƿ� ������ �Ѵ�
	{
		CParty* pParty	= new CParty;
		
//		pParty->Lock();
		pParty->SetPartyId( m_id );
#if __VER >= 11 // __SYS_PLAYER_DATA
		if( TRUE == pParty->NewMember( uLeaderId ) && TRUE == pParty->NewMember( uMemberId ) )
#else	// __SYS_PLAYER_DATA
		if( TRUE == pParty->NewMember( uLeaderId, nLeaderLevel, nLeaderJob, nLeaderSex, szLeaderName ) && TRUE == pParty->NewMember( uMemberId, nMemberLevel, nMemberJob, nMemberSex, szMembername ) )
#endif	// __SYS_PLAYER_DATA
		{
//			m_2Party.SetAt( m_id, pParty );
			m_2PartyPtr.insert( C2PartyPtr::value_type( m_id, pParty ) );
			pParty->m_nGetItemPlayerId = pParty->m_aMember[0].m_uPlayerId;
//			pParty->Unlock();
			return m_id;
		}
		else // �شܿ� �ҼӵǾ� �ִ�
		{
//			pParty->Unlock();
			safe_delete( pParty );
		}
	}
	return 0;
}

// �ش� ��ü
// ��ü�� ����� �����Ͽ� �ڵ����� ��ü
// �ܵ������� ��ü�Ҽ� ����
BOOL CPartyMng::DeleteParty( u_long uPartyId )
{
	CParty* pParty = GetParty( uPartyId );
	if( pParty )
	{
		if( pParty->m_idDuelParty )		// �شܵ�����϶� �ش��� ��ü�Ǿ�����
		{
			CParty *pDstParty = GetParty( pParty->m_idDuelParty );		// ��� ��Ƽ���鿡�Ե� this��Ƽ�� ��ü�Ǿ� ����������ƴٴ°� �˸�.
			if( pDstParty )
			{
				pDstParty->DoDuelPartyCancel( pParty );
			} 
			else
			{
				Error( "CPartyMng::DeleteParty : �����Ƽ�� ã�� �� ���� %d", pParty->m_idDuelParty );
			}

	#ifdef __WORLDSERVER
			g_DPCoreClient.SendSetPartyDuel( pParty->m_uPartyId, pParty->m_idDuelParty, FALSE );
	#endif // __WORLDSERVER
			pParty->DoDuelPartyCancel( pDstParty );	// this��Ƽ���鿡�Ե� ��������ƴٴ°� �˸�.
		}
#ifdef __WORLDSERVER
#if __VER >= 14 // __INSTANCE_DUNGEON
		CInstanceDungeonParty::GetInstance()->DestroyAllDungeonByDungeonID( uPartyId );
#endif // __INSTANCE_DUNGEON
#endif // __WORLDSERVER

		safe_delete( pParty );
		m_2PartyPtr.erase( uPartyId );
		return TRUE;
	}
	return FALSE;
}

CParty* CPartyMng::GetParty( u_long uPartyId )
{
//	CParty* pParty;
//	if( m_2Party.Lookup( uPartyId, pParty ) )
//	{
//		return pParty;
//	}
	C2PartyPtr::iterator i		= m_2PartyPtr.find( uPartyId );
	if( i != m_2PartyPtr.end() )
		return i->second;
	return NULL;
}

void CPartyMng::Serialize( CAr & ar )
{
#ifndef __WORLDSERVER
	m_AddRemoveLock.Enter( theLineFile );	// lock1
#endif	// __WORLDSERVER

	if( ar.IsStoring() )
	{
		ar << m_id;
		int nCount	= 0;
		u_long uOffset	= ar.GetOffset();
		ar << nCount;
		for( C2PartyPtr::iterator i	= m_2PartyPtr.begin(); i != m_2PartyPtr.end(); ++i )
		{
			CParty* pParty = (CParty*)i->second;
			pParty->Serialize( ar );
			nCount++;
		}
		int nBufSize1;
		LPBYTE lpBuf1	= ar.GetBuffer( &nBufSize1 );
		*(UNALIGNED int*)( lpBuf1 + uOffset )	= nCount;
	}
	else
	{
		int nCount;
		ar >> m_id;
		ar >> nCount;		
		for( int i = 0 ; i < nCount ; i++ )
		{
			CParty* pParty	= new CParty;
//			pParty->Lock();
			pParty->Serialize( ar );
			m_2PartyPtr.insert( C2PartyPtr::value_type( pParty->m_uPartyId, pParty ) );
//			pParty->Unlock();
		}
	}
#ifndef __WORLDSERVER
	m_AddRemoveLock.Leave( theLineFile );	// unlock1
#endif	// __WORLDSERVER
}

#ifdef __CORESERVER
BOOL CPartyMng::IsPartyNameId( u_long uidPlayer )
{
	if( m_2PartyNameLongPtr.end() != m_2PartyNameLongPtr.find( uidPlayer ) )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CPartyMng::IsPartyName( const char* szPartyName )
{
	if( m_2PartyNameStringPtr.end() != m_2PartyNameStringPtr.find( szPartyName ) )
	{
		return TRUE;
	}
	return FALSE;
}

LPCSTR CPartyMng::GetPartyString( u_long uidPlayer )
{
	ULONG2STRING::iterator i	= m_2PartyNameLongPtr.find( uidPlayer );
	if( i != m_2PartyNameLongPtr.end() )
		return( i->second.data() );
	return NULL;
}

u_long CPartyMng::GetPartyID( const char* szPartyName )
{
	STRING2ULONG::iterator i	= m_2PartyNameStringPtr.find( szPartyName );
	if( i != m_2PartyNameStringPtr.end() )
		return( i->second );
	return 0;
}



void CPartyMng::AddPartyName( u_long uidPlayer, const char* szPartyName )
{
	m_2PartyNameLongPtr.insert(  map<u_long, string>::value_type( uidPlayer, szPartyName ) );
	m_2PartyNameStringPtr.insert(  map<string, u_long>::value_type(szPartyName, uidPlayer ) );
}

void CPartyMng::RemovePartyName( u_long uidPlayer, const char* szPartyName )
{
	ULONG2STRING::iterator iter = m_2PartyNameLongPtr.find( uidPlayer );
	STRING2ULONG::iterator iter1 = m_2PartyNameStringPtr.find( szPartyName );

	if( iter != m_2PartyNameLongPtr.end() )
		m_2PartyNameLongPtr.erase( iter );

	if( iter1 != m_2PartyNameStringPtr.end() )
		m_2PartyNameStringPtr.erase( iter1 );
//	m_2PartyNameLongPtr.insert(  map<u_long, string>::value_type( uidPlayer, szPartyName ) );
//	m_2PartyNameStringPtr.insert(  map<string, u_long>::value_type(szPartyName, uidPlayer ) );
}

BOOL CPartyMng::CreateWorkers( void )
{
	DWORD dwThreadId;
	m_hCloseWorker	= CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hWorker	= chBEGINTHREADEX( NULL, 0, _Worker, this, 0, &dwThreadId );
	if( m_hWorker == NULL )
		return FALSE;
	return TRUE;
}

void CPartyMng::CloseWorkers( void )
{
	CLOSE_THREAD( m_hWorker, m_hCloseWorker );
}

UINT CPartyMng::_Worker( LPVOID pParam )
{
	CPartyMng* pPartyMng	= (CPartyMng*)pParam;
	pPartyMng->Worker();
	return 0;
}

void CPartyMng::Worker( void )
{
	CPlayer* pMember;

	HANDLE hHandle	= m_hCloseWorker;
	while( WaitForSingleObject( hHandle, 1000 ) == WAIT_TIMEOUT )
	{
		CTime timeCurr	= CTime::GetCurrentTime();

		CMclAutoLock	Lock( g_PlayerMng.m_AddRemoveLock );
		CMclAutoLock	Lock2( m_AddRemoveLock );

		for( C2PartyPtr::iterator i	= m_2PartyPtr.begin(); i != m_2PartyPtr.end(); )
		{
			CParty* pParty = (CParty*)i->second;
			++i;
			if( pParty->m_nReferens > 0 )
			{
				for( int j = 1; j < pParty->m_nSizeofMember; j++ )
				{
					if( pParty->m_aMember[j].m_uPlayerId != 0 && pParty->m_aMember[j].m_bRemove )
					{
						if( timeCurr.GetTime() - pParty->m_aMember[j].m_tTime.GetTime() > 60 * 10 )	// 10�� : 60 * 10 // ������ 1��
						{
							u_long idMember		= pParty->GetPlayerId( j );
							if( pParty->DeleteMember( idMember ) )
							{
								pParty->m_nReferens--;
								
								g_dpCoreSrvr.SendRemoveParty( pParty->m_uPartyId, pParty->m_aMember[0].m_uPlayerId, idMember );
								
								if( pParty->GetSizeofMember() < 2 )
								{
									pMember		= g_PlayerMng.GetPlayer( pParty->GetPlayerId( 0 ) );
									if( pMember )
										pMember->m_uPartyId	= 0;
									DeleteParty( pParty->m_uPartyId );
									break;
								}

								if( pParty->m_nReferens == 0 )
									break;
							}
						}
					}
				}	// for
			}	// if

			// ��Ƽ��带 üũ
			for( int j = 0 ; j < MAX_PARTYMODE ; j++ )
			{
				if( pParty->m_nModeTime[j] )
				{
#if __VER >= 12 // __PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz
					if( j == PARTY_PARSKILL_MODE)
						continue;
#endif //__PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz
					pParty->m_nModeTime[j] -= 1000;
					if( pParty->m_nModeTime[j] <= 0 )
					{
						pParty->m_nModeTime[j] = 0;
						g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, j, FALSE );
					}
				}
			}
		}	// for
//		TRACE( "Worker Party\n" );
	}
}

void CPartyMng::AddConnection( CPlayer* pPlayer )
{
	CParty* pParty;
	CMclAutoLock	Lock( m_AddRemoveLock );
	
	pParty	= GetParty( pPlayer->m_uPartyId );
	if( pParty )
	{
		int i	= pParty->FindMember( pPlayer->uKey );
		if( i < 0 )
		{
			pPlayer->m_uPartyId		= 0;
			return;
		}

		pParty->m_aMember[i].m_bRemove	= FALSE;
		pParty->m_nReferens--;

		BEFORESENDDUAL( ar, PACKETTYPE_ADDPLAYERPARTY, DPID_UNKNOWN, DPID_UNKNOWN );
		ar << pPlayer->m_uPartyId << pPlayer->uKey;
		SEND( ar, &g_dpCoreSrvr, DPID_ALLPLAYERS );
	}
	else
		pPlayer->m_uPartyId		= 0;
}

void CPartyMng::RemoveConnection( CPlayer* pPlayer )
{
	if( pPlayer->m_uPartyId == 0 )
		return;

	CParty* pParty;
	CMclAutoLock	Lock( m_AddRemoveLock );
	
	pParty	= GetParty( pPlayer->m_uPartyId );
	if( pParty )
	{
		int i	= pParty->FindMember( pPlayer->uKey );
		if( i < 0 )
			return;
		pParty->m_aMember[i].m_tTime	= CTime::GetCurrentTime();
		pParty->m_aMember[i].m_bRemove	= TRUE;
		pParty->m_nReferens++;

		BEFORESENDDUAL( ar, PACKETTYPE_REMOVEPLAYERPARTY, DPID_UNKNOWN, DPID_UNKNOWN );
		ar << pPlayer->m_uPartyId << pPlayer->uKey;
		SEND( ar, &g_dpCoreSrvr, DPID_ALLPLAYERS );

#if __VER >= 12 // __PARSKILL1001 090917 mirchang - �Ľ�ų ������ ����
		if( pParty->m_nModeTime[PARTY_PARSKILL_MODE] )
			g_dpCoreSrvr.SendSetPartyMode( pParty->m_uPartyId, PARTY_PARSKILL_MODE, FALSE );
#endif // __PARSKILL1001 090917 mirchang - �Ľ�ų ������ ����

		if( i == 0 )
		{
			bool fRemoveParty	= true;
			for( int j = 1; j < pParty->m_nSizeofMember; j++ )
			{
				if( pParty->m_aMember[j].m_bRemove == FALSE )
				{
					fRemoveParty	= false;
					pParty->SwapPartyMember( 0, j );
					break;
				}
			}
#if __VER >= 12 // __PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz
			for( int k = 0 ; k < MAX_PARTYMODE ; k++ )
			{
				if( pParty->m_nModeTime[k] )
				{
					if( k == PARTY_PARSKILL_MODE)
						continue;
					pParty->m_nModeTime[k] = 0;
				}
			}
#endif //__PARSKILL1001	//12�� �Ľ�ų ������ ����  world,core,neuz

			if( fRemoveParty )
			{
				CPlayer* pPlayer;
				for( int j = 0; j < pParty->m_nSizeofMember; j++ )
				{
					pPlayer		= g_PlayerMng.GetPlayer( pParty->GetPlayerId( j ) );
					if( pPlayer )
						pPlayer->m_uPartyId		= 0;
				}
				DeleteParty( pParty->m_uPartyId );
			}
		}
	}
}
#endif // __CORESERVER

#ifdef __WORLDSERVER

void CPartyMng::PartyMapInfo( )
{
	const float PARTY_MAP_AROUND = 32.0f * 32.0f;	// m_nVisibilityRange�� ������ �޴´�.

	if( ++m_nSecCount < PARTY_MAP_SEC )
		return;

	m_nSecCount = 0;

	D3DXVECTOR3 vPosBuf;
	float fDist;
	for( C2PartyPtr::iterator i	= m_2PartyPtr.begin(); i != m_2PartyPtr.end(); ++i )
	{
		CParty* pParty = (CParty*)i->second;
		for( int j = 0 ; j < pParty->GetSizeofMember() ; ++j )
		{
			CMover* pMover = prj.GetUserByID( pParty->GetPlayerId( j ) );
			if( !IsValidObj( pMover ) )
				continue;
			
			vPosBuf = pMover->GetPos() - pParty->GetPos( j );
			fDist = D3DXVec3LengthSq( &vPosBuf );
			if( 0.0f < fDist )
			{
				pParty->SetPos( j, pMover->GetPos() );
				
				CMover* pSendMover;
				for( int k = 0 ; k < pParty->GetSizeofMember() ; ++k )
				{
					if( k == j )
						continue;
					pSendMover = prj.GetUserByID( pParty->GetPlayerId( k ) );
					if( !IsValidObj( pSendMover ) )
						continue;

					vPosBuf = pSendMover->GetPos() - pMover->GetPos();
					fDist = D3DXVec3LengthSq( &vPosBuf );
					if( fDist > PARTY_MAP_AROUND )
						((CUser*)pSendMover)->AddPartyMapInfo( j, pMover->GetPos() );		
				}
			}
		}
	}
}

#endif // __WORLDSERVER

CPartyMng	g_PartyMng;
#endif // not client