// SecretRoom.cpp: implementation of the CSecretRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SecretRoom.h"

#ifdef __WORLDSERVER
#include "User.h"
#include "defineText.h"
#include "DPCoreClient.h"
extern  CDPCoreClient g_DPCoreClient;
#include "DPDatabaseClient.h"
extern	CDPDatabaseClient g_dpDBClient;
extern	CGuildMng	g_GuildMng;
#include "worldmng.h"
extern	CWorldMng	g_WorldMng;
#if __VER >= 12 // __TAX
#include "Tax.h"
#endif // __TAX
#include "DPSrvr.h"
extern CDPSrvr g_DPSrvr;
#endif // __WORLDSERVER


#if __VER >= 12 // __SECRET_ROOM
//////////////////////////////////////////////////////////////////////
// ����� �� ��ü ����
//////////////////////////////////////////////////////////////////////
CSecretRoomMng::CSecretRoomMng()
{
	m_mapSecretRoomContinent.insert( make_pair( CONT_EAST, new CSecretRoomContinent(CONT_EAST) ) );
	m_mapSecretRoomContinent.insert( make_pair( CONT_WEST, new CSecretRoomContinent(CONT_WEST) ) );

	m_nState = SRMNG_CLOSE;
	m_dwRemainTime = 0;

#ifdef __CLIENT
	m_pSRCont = NULL;
#endif // __CLIENT
}

CSecretRoomMng::~CSecretRoomMng()
{
	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		if( it->second )
			safe_delete( it->second );
	}
}

CSecretRoomMng* CSecretRoomMng::GetInstance( void )
{	
	static CSecretRoomMng sSecretRoomMng;
	return &sSecretRoomMng;
}

BYTE CSecretRoomMng::GetContinent( CMover* pMover )
{
	return CContinent::GetInstance()->GetArea( pMover );
}

BOOL CSecretRoomMng::IsInTheSecretRoom( CMover* pMover )
{
	CWorld* pWorld = pMover->GetWorld();
	if( pWorld && pWorld->GetID() >= WI_DUNGEON_SECRET_1 && pWorld->GetID() <= WI_DUNGEON_SECRET_L )
		return TRUE;

	return FALSE;
}

#ifdef __WORLDSERVER
BOOL CSecretRoomMng::LoadScript()
{
	if( m_Lua.RunScript( "SecretRoom.lua" ) != 0 )
	{
		Error( "SecretRoom.lua Run Failed!!!" );
		ASSERT(0);
	}
	
	m_nMinGuildLevel		= static_cast<int>( m_Lua.GetGlobalNumber( "MinGuildLevel" ) );
	m_nMinGuildMemberLevel	= static_cast<int>( m_Lua.GetGlobalNumber( "MinGuildMemberLevel" ) );
	m_nMinPenya				= static_cast<int>( m_Lua.GetGlobalNumber( "MinPenya" ) );
	m_nMinGuildMemberNum	= static_cast<int>( m_Lua.GetGlobalNumber( "MinGuildMemberNum" ) );
	m_nMaxGuildMemberNum	= static_cast<int>( m_Lua.GetGlobalNumber( "MaxGuildMemberNum" ) );
	m_nMinGuildNum			= static_cast<int>( m_Lua.GetGlobalNumber( "MinGuildNum" ) );
	m_nMaxGuildNum			= static_cast<int>( m_Lua.GetGlobalNumber( "MaxGuildNum" ) );
	m_nMonsterGenNum		= static_cast<int>( m_Lua.GetGlobalNumber( "MonsterGenNum" ) );
	m_nCancelReturnRate		= static_cast<int>( m_Lua.GetGlobalNumber( "CancelReturnRate" ) );
	m_nDropoutReturnRate	= static_cast<int>( m_Lua.GetGlobalNumber( "DropoutReturnRate" ) );

	m_mapMonsterNum.insert( make_pair( MONSTER_NORMAL, 0 ) );
	m_mapMonsterNum.insert( make_pair( MONSTER_MIDBOSS, 0 ) );
	m_mapMonsterNum.insert( make_pair( MONSTER_BOSS, 0 ) );

	m_Lua.GetGloabal( "tMonster" );
	m_Lua.PushNil();
	while( m_Lua.TableLoop( -2 ) )
	{
		__SECRETROOM_MONSTER srMonster;
		srMonster.dwId = CScript::GetDefineNum( m_Lua.GetFieldToString( -1, "dwId" ) );
		srMonster.nNum = static_cast<int>( m_Lua.GetFieldToNumber( -1, "nNum" ) );
		srMonster.bRed = static_cast<BOOL>( m_Lua.GetFieldToBool( -1, "bRed" ) );
		srMonster.x1 = static_cast<int>( m_Lua.GetFieldToNumber( -1, "x1" ) );
		srMonster.z1 = static_cast<int>( m_Lua.GetFieldToNumber( -1, "z1" ) );
		srMonster.x2 = static_cast<int>( m_Lua.GetFieldToNumber( -1, "x2" ) );
		srMonster.z2 = static_cast<int>( m_Lua.GetFieldToNumber( -1, "z2" ) );
		srMonster.y = static_cast<int>( m_Lua.GetFieldToNumber( -1, "y" ) );

		int nType = static_cast<int>( m_Lua.GetFieldToNumber(-1, "nType" ) );
		m_mapMonsterNum.find(nType)->second += srMonster.nNum;
		switch( nType )
		{
			case MONSTER_NORMAL : m_vecNormalMonster.push_back( srMonster ); break;
			case MONSTER_MIDBOSS : m_vecMidBossMonster.push_back( srMonster ); break;
			case MONSTER_BOSS : m_vecBossMonster.push_back( srMonster ); break;
		}
		m_Lua.Pop(1);
	}
	m_Lua.Pop(0);

	return TRUE;
}

void CSecretRoomMng::Process()
{
	DWORD dwTick = GetTickCount();
	
	switch( m_nState )		// ���� State���� ���� State�� �Ѿ�� ó��, m_nState�� ���� State
	{
		case SRMNG_CLOSE:
			{
				if( CheckOpenTime() )
					SecretRoomOpen();
			}
			break;
	
		case SRMNG_OPEN :
			{
				m_nState = SRMNG_LINEUP;
				m_dwRemainTime = dwTick + (DWORD)m_Lua.GetGlobalNumber( "LineUpTime" );
				
				map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
				for( ; it!=m_mapSecretRoomContinent.end(); it++ )
				{
					CSecretRoomContinent* pSRCont = it->second;
					if( pSRCont && pSRCont->m_nState == SRCONT_OPEN )
					{
						for( DWORD i=0; i<pSRCont->m_vecSecretRoomTender.size(); i++ )
						{
							CGuild* pGuild  = g_GuildMng.GetGuild( pSRCont->m_vecSecretRoomTender[i].dwGuildId );
							if( pGuild )
							{
								// �渶�� ��ϵǾ� ���� ������ �ڵ����
								/*
								BOOL bLineUpMaster = FALSE;
								for( int j=0; j<pSRCont->m_vecSecretRoomTender[i].vecLineUpMember.size(); j++ )
								{
									if( pSRCont->m_vecSecretRoomTender[i].vecLineUpMember[j] == pGuild->m_idMaster )
									{
										bLineUpMaster = TRUE;
										break;
									}
								}
								if( !bLineUpMaster )
								{
									pSRCont->m_vecSecretRoomTender[i].vecLineUpMember.push_back( pGuild->m_idMaster );
									// DB�� Update
									g_dpDBClient.SendSecretRoomInsertLineUpToDB( pSRCont->m_nContinent, pSRCont->m_vecSecretRoomTender[i] );
								}
								*/
								CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
								if( IsValidObj( pUserTemp ) )
									SendNowState( pUserTemp, m_nState, m_dwRemainTime - GetTickCount() );
							}
						}
					}
				}
			}
			break;
		
		case SRMNG_LINEUP :
			{
				if( m_dwRemainTime < dwTick )
				{
					m_nState = SRMNG_ENTRANCE;
					m_dwRemainTime = dwTick + (DWORD)m_Lua.GetGlobalNumber( "EntranceTime" );
					
					map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
					for( int nContCount=0; it!=m_mapSecretRoomContinent.end(); it++, nContCount++ )
					{
						if( it->second )
						{
							it->second->SetSecretRoomWorld( nContCount );
						}	
					}
					SendNowStateAllMember();
				}
			}
			break;

		case SRMNG_ENTRANCE :
			{
				if( m_dwRemainTime < dwTick )
				{
					m_nState = SRMNG_WARWAIT;
					m_dwRemainTime = dwTick + (DWORD)m_Lua.GetGlobalNumber( "WarWaitTime" );
					SendNowStateAllMember();
				}
			}
			break;

		case SRMNG_WARWAIT :
			{
				if( m_dwRemainTime < dwTick )
				{
					m_nState = SRMNG_WAR;
					m_dwRemainTime = 0;
					SendNowStateAllMember();
				}
			}
			break;
		
		case SRMNG_WAR :
			{
				int nCloseConut = 0;
				map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
				for( ; it != m_mapSecretRoomContinent.end(); it++ )
				{
					if( it->second && it->second->m_nState == SRCONT_CLOSE )
					{
						nCloseConut++;
					}
					else
						it->second->Process( dwTick );			
				}

				if( (DWORD)( nCloseConut ) >= m_mapSecretRoomContinent.size() )
				{
					m_nState = SRMNG_CLOSE;
					m_dwRemainTime = 0;
					SendNowStateAllMember();
					g_DPCoreClient.SendCaption( prj.GetText( TID_GAME_SECRETROOM_ALLCLOSE ) );
					g_dpDBClient.SendSecretRoomClosed();
				}
			}
			break;		
	}
}

// ���� ������ ��� �ΰ�?
BOOL CSecretRoomMng::IsPossibleGuild( CUser* pUser )
{
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild && pGuild->m_nLevel >= m_nMinGuildLevel )
		return TRUE;
	
	return FALSE;
}

// ��� ������ �ΰ�?
BOOL CSecretRoomMng::IsGuildMaster( CUser* pUser )
{
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild && pGuild->IsMaster( pUser->m_idPlayer ) )
		return TRUE;
	
	return FALSE;
}

// �ٸ� ����� ������ ����ΰ�?
BOOL CSecretRoomMng::IsOtherTenderGuild( CUser* pUser )
{
	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		if( it->first != GetContinent( (CMover*)pUser ) )
			if( it->second && it->second->GetTenderGuild( pUser->m_idGuild ) != NULL_ID )
				return TRUE;
	}
	
	return FALSE;
}

// ���� ����� �� ���� ���¸� �ش� �������� �����Ѵ�.
void CSecretRoomMng::SendNowState( CUser* pUser, int nState, DWORD dwRemainTime )
{
	pUser->AddSecretRoomMngState( nState, dwRemainTime );
}

// ���� ����� �� ���� ���¸� ������û�� ��� �������� �����Ѵ�.
void CSecretRoomMng::SendNowStateAllMember( BYTE nCont, int nState, DWORD dwRemainTime )
{
	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		if( nCont != CONT_NODATA && it->first != nCont )
			continue;

		CSecretRoomContinent* pSRCont = it->second;
		if( pSRCont )
		{
			for( DWORD i=0; i<pSRCont->m_vecSecretRoomTender.size(); i++ )
			{
				for( DWORD j=0; j<pSRCont->m_vecSecretRoomTender[i].vecLineUpMember.size(); j++ )
				{
					if( nState && pSRCont->m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
						continue;

					CUser* pUserTemp = (CUser*)prj.GetUserByID( pSRCont->m_vecSecretRoomTender[i].vecLineUpMember[j] );
					if( IsValidObj( pUserTemp ) )
					{
						if( nState )
							SendNowState( pUserTemp, nState, dwRemainTime );
						else
							SendNowState( pUserTemp, m_nState, m_dwRemainTime - GetTickCount() );
					}
				}
			}
		}
	}	
}

// ���� ������ ������ �α��ν� ���� ���� ���¸� �����Ѵ�.
void CSecretRoomMng::GetAllInfo( CUser* pUser )
{
	if( m_nState == SRMNG_CLOSE )
	{
		SendNowState( pUser, m_nState, 0 );
		return;
	}

	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		CSecretRoomContinent* pSRCont = it->second;
		if( pSRCont )
		{
			int nIndex = pSRCont->GetTenderGuild( pUser->m_idGuild );
			if( m_nState == SRMNG_LINEUP )
			{
				if( nIndex != NULL_ID && IsGuildMaster( pUser ) )
					SendNowState( pUser, m_nState, m_dwRemainTime - GetTickCount() );
			}
			else if( pSRCont->IsLineUpMember( pUser ) )
			{
				if( m_nState != SRMNG_WAR )
					SendNowState( pUser, m_nState, m_dwRemainTime - GetTickCount() );
				else if( pSRCont->m_vecSecretRoomTender[nIndex].nWarState == MONSTER_FAILED )
					SendNowState( pUser, SRCONT_CLOSE, 0 );
				else
				{
					SendNowState( pUser, pSRCont->m_nState, pSRCont->m_dwRemainTime - GetTickCount() );
					pSRCont->UpdateInfoToMember( pUser, SECRETROOM_GUILDLIST );
					return;
				}
			}
		}
	}
}

// ���� �ð��ΰ�?
BOOL CSecretRoomMng::CheckOpenTime()
{
	BOOL bOpen = FALSE;
	if( m_Lua.GetLuaFunction( "CheckOpenTime" ) )
	{
		m_Lua.CallLuaFunction( 0, 1 );
		bOpen = static_cast<BOOL>( m_Lua.ToBool( -1 ) );
	}
	m_Lua.Pop(0);

	return bOpen;
}

// ��� ����� ����� ���� �����Ѵ�.
void CSecretRoomMng::SecretRoomOpen()
{
	if( !g_eLocal.GetState( EVE_SECRETROOM ) )
		return;

	if( m_nState != SRMNG_CLOSE )
		return;
	
	BOOL bOpen = FALSE;
	
	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		CSecretRoomContinent* pSRCont = it->second;
		if( !pSRCont || pSRCont->m_vecSecretRoomTender.size() < (DWORD)( m_nMinGuildNum ) )
			continue;
		
		CString strTemp;
		strTemp.Format( prj.GetText( TID_GAME_SECRETROOM_CONTOPEN ), CContinent::GetInstance()->GetContinentName( it->first ) );
		g_DPCoreClient.SendCaption( strTemp );
		
		bOpen = TRUE;
		pSRCont->SecretRoomContOpen();
	}

	if( !bOpen )
	{
		g_DPCoreClient.SendCaption( prj.GetText( TID_GAME_SECRETROOM_NOTOPEN ) );
		return;
	}

	m_nState = SRMNG_OPEN;
}

void CSecretRoomMng::SetTenderOpenWnd( CUser* pUser )
{
	if( m_nState != SRMNG_CLOSE )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDER );
		return;
	}

	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISNOTGUILDMASTER );
		return;
	}

	if( !IsPossibleGuild( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_LOWGUILDLEVEL );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;

	int nTenderPenya = 0;
	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont )
	{
		int nIndex = pSRCont->GetTenderGuild( pUser->m_idGuild );
		if( nIndex != NULL_ID )
			nTenderPenya = pSRCont->m_vecSecretRoomTender[nIndex].nPenya;
	}

	pUser->AddSecretRoomTenderOpenWnd( nTenderPenya );
}

// ���� ��û
void CSecretRoomMng::SetTender( CUser* pUser, int nPenya )
{
	if( m_nState != SRMNG_CLOSE )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDER );
		return;
	}
	
	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISNOTGUILDMASTER );
		return;
	}

	if( !IsPossibleGuild( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_LOWGUILDLEVEL );
		return;
	}
	
	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;
	
	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;

	if( pSRCont )
		pSRCont->SetContTender( pUser, nPenya );
}

// ���� ���
void CSecretRoomMng::SetTenderCancelReturn( CUser* pUser )
{
	if( m_nState != SRMNG_CLOSE )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDERCANCEL );
		return;
	}

	if( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISNOTGUILDMASTER );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;
	
	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont )
		pSRCont->SetContTenderCancelReturn( pUser );
}

// ������ ����â�� �����Ҷ� ������ �����Ѵ�.
void CSecretRoomMng::SetLineUpOpenWnd( CUser* pUser )
{
	if( m_nState != SRMNG_LINEUP )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOLINEUP );
		return;
	}

	if ( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISNOTGUILDMASTER );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;

	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont && pSRCont->m_nState == SRCONT_OPEN )
	{
		int nIndex = pSRCont->GetTenderGuild( pUser->m_idGuild );
		if( nIndex != NULL_ID )
			pUser->AddSecretRoomLineUpOpenWnd( pSRCont->m_vecSecretRoomTender[nIndex].vecLineUpMember );
		else
			pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDERGUILD, "\"%s\"", CContinent::GetInstance()->GetContinentName( nCont ) );
	}
	else
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTOPENCONT, "\"%s\"", CContinent::GetInstance()->GetContinentName( nCont ) );
}

// ������ ���� ��û
void CSecretRoomMng::SetLineUp( CUser* pUser, vector<DWORD> vecLineUpMemeber )
{
	if( m_nState != SRMNG_LINEUP )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOLINEUP );
		return;
	}

	if ( !IsGuildMaster( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISNOTGUILDMASTER );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;

	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont && pSRCont->m_nState == SRCONT_OPEN )
		pSRCont->SetContLineUp( pUser, vecLineUpMemeber );
	else
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTOPENCONT, "\"%s\"", CContinent::GetInstance()->GetContinentName( nCont ) );

}

// ���� ��Ȳ ����
void CSecretRoomMng::GetTenderView( CUser* pUser )
{
	if( m_nState != SRMNG_CLOSE )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDERVIEW );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;

	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont )
	{
		int nTenderPenya = 0;
		int nRanking = 0;
		int nIndex = pSRCont->GetTenderGuild( pUser->m_idGuild );
		if( nIndex != NULL_ID )
		{
			nTenderPenya = pSRCont->m_vecSecretRoomTender[nIndex].nPenya;
			nRanking = nIndex + 1;
		}

		time_t t = 0;
		if( m_Lua.GetLuaFunction( "GetNextOpenTime" ) )
		{
			m_Lua.CallLuaFunction( 0, 1 );
			t = static_cast<time_t>(m_Lua.ToNumber( -1 ) );
		}
		m_Lua.Pop(0);
		
		pUser->AddSecretRoomTenderView( nTenderPenya, nRanking, t, pSRCont->m_vecSecretRoomTender );
	}
}

// �ش� ����� ����� ������ �ڷ���Ʈ ��Ų��.
void CSecretRoomMng::SetTeleportSecretRoom( CUser* pUser )
{
	if( m_nState != SRMNG_ENTRANCE && m_nState != SRMNG_WARWAIT && m_nState != SRMNG_WAR )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOENTRANCE );
		return;
	}

	BYTE nCont = GetContinent( (CMover*)pUser );
	if( nCont == CONT_NODATA )
		return;

	CSecretRoomContinent* pSRCont = m_mapSecretRoomContinent.find(nCont)->second;
	if( pSRCont && pSRCont->m_nState != SRCONT_CLOSE )
		pSRCont->SetTeleportSecretRoomCont( pUser );
	else
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTOPENCONT, "\"%s\"", CContinent::GetInstance()->GetContinentName( nCont ) );
}

void CSecretRoomMng::SetSecretRoomKill( DWORD dwWorldId, DWORD dwMonsterId )
{
	if( m_nState != SRMNG_WAR )
		return;

	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		CSecretRoomContinent* pSRCont = it->second;
		if( pSRCont )
			pSRCont->SetSecretRoomContKill( dwWorldId, dwMonsterId );
	}
}

void CSecretRoomMng::SetFailGuild( CUser* pUser )
{
	if( m_nState != SRMNG_WAR )
		return;

	if( !IsValidObj( pUser ) )
		return;

	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		CSecretRoomContinent* pSRCont = it->second;
		if( pSRCont && pSRCont->m_nState == SRCONT_WAR )
			pSRCont->SetContFailGuild( pUser->m_idGuild );
	}
}

D3DXVECTOR3 CSecretRoomMng::GetRevivalPos( CUser* pUser )
{
	if( !IsValidObj( pUser ) )
		return CContinent::GetInstance()->GetRevivalPos( CONT_NODATA );

	map<BYTE, CSecretRoomContinent*>::iterator it = m_mapSecretRoomContinent.begin();
	for( ; it!=m_mapSecretRoomContinent.end(); it++ )
	{
		CSecretRoomContinent* pSRCont = it->second;
		if( pSRCont && pSRCont->IsLineUpMember( pUser ) )
			return CContinent::GetInstance()->GetRevivalPos( pSRCont->m_nContinent );
	}

	return CContinent::GetInstance()->GetRevivalPos( CONT_NODATA );
}
#endif // __WORLDSERVER


//////////////////////////////////////////////////////////////////////
// ����� �� ����� ����
//////////////////////////////////////////////////////////////////////

CSecretRoomContinent::CSecretRoomContinent( BYTE nCont )
{
	m_nState = SRCONT_CLOSE;
	m_dwRemainTime = 0;
	m_nContinent = nCont;
#ifdef __WORLDSERVER
	m_nFailedGuild = 0;
#endif // __WORLDSERVER
}

CSecretRoomContinent::~CSecretRoomContinent()
{

}

int CSecretRoomContinent::GetTenderGuild( DWORD dwGuildId )
{
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].dwGuildId == dwGuildId )
			return i;
	}

	return NULL_ID;
}

#ifdef __WORLDSERVER
void CSecretRoomContinent::Process( DWORD dwTick )
{
	switch( m_nState )
	{
		case SRCONT_CLOSE :
			break;

		case SRCONT_OPEN :
			{
				UpdateInfoToAllMember( SECRETROOM_GUILDLIST );
				m_nState = SRCONT_WAR;
				m_dwRemainTime = dwTick + (DWORD)CSecretRoomMng::GetInstance()->m_Lua.GetGlobalNumber( "WarTime" );
				CSecretRoomMng::GetInstance()->SendNowStateAllMember( m_nContinent, m_nState, m_dwRemainTime - dwTick );
				
				// ��帶���Ͱ� �������� ���� ���� Ż����Ų��.
				/*
				for( int i=0; i<m_vecSecretRoomTender.size(); i++ )
				{
					CGuild* pGuild = g_GuildMng.GetGuild( m_vecSecretRoomTender[i].dwGuildId );
					if( pGuild )
					{
						CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
						if( IsValidObj( pUserTemp ) && pUserTemp->GetWorld() &&
							pUserTemp->GetWorld()->GetID() == m_vecSecretRoomTender[i].dwWorldId )
							continue;
						
						SetContFailGuild( pGuild->GetGuildId() );
					}
				}
				*/
			}
			break;

		case SRCONT_WAR :
			{
				if( m_dwRemainTime < dwTick || (DWORD)( m_nFailedGuild ) >= m_vecSecretRoomTender.size() )
				{
					CString strTemp;
					strTemp.Format( prj.GetText( TID_GAME_SECRETROOM_NOWINNER ), CContinent::GetInstance()->GetContinentName( m_nContinent ) );
					g_DPCoreClient.SendCaption( strTemp );
					SetContCloseWait();
				}
				else 
				{
					ProcessInGuildMaster();		// �渶�� �ش� ����� �濡 �����ϴ��� �˻��ؼ� ������ Ż��!!!
					CreateMonster();
				}
			}
			break;

		case SRCONT_CLOSEWAIT :
			{
				if( m_dwRemainTime < dwTick )
					SetContClose();
			}
			break;
	}
}

BOOL CSecretRoomContinent::IsLineUpMember( CUser* pUser )
{
	int nIndex = GetTenderGuild( pUser->m_idGuild );
	if( nIndex == NULL_ID )
		return FALSE;

	for( DWORD i=0; i<m_vecSecretRoomTender[nIndex].vecLineUpMember.size(); i++ )
	{
		if( m_vecSecretRoomTender[nIndex].vecLineUpMember[i] == pUser->m_idPlayer )
			return TRUE;
	}
	
	return FALSE;
}

void CSecretRoomContinent::SetContTender( CUser* pUser, int nPenya )
{
	if( CSecretRoomMng::GetInstance()->IsOtherTenderGuild( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISOTHERCONT );
		return;
	}
	
	if( nPenya < CSecretRoomMng::GetInstance()->m_nMinPenya )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_TRYMOREPENYA );
		return;
	}
	
	// �α� ���� �غ�
	LogItemInfo aLogItem;
	aLogItem.Action = "h";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "SECRETROOM";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	//aLogItem.ItemName = "SEED";
	_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
	aLogItem.Gold = pUser->GetGold();

	int nIndex = GetTenderGuild( pUser->m_idGuild );
	// ó�� ������ ���
	
	if( nIndex == NULL_ID )
	{
		if( pUser->GetGold() < nPenya )
		{
			pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENOUGHPENYA );
			return;
		}
		
		pUser->AddGold( -nPenya );
		__SECRETROOM_TENDER srTender;
		srTender.dwGuildId = pUser->m_idGuild;
		srTender.nPenya = nPenya;
		
		m_vecSecretRoomTender.push_back( srTender );
		// DB�� Insert
		g_dpDBClient.SendSecretRoomInsertToDB( m_nContinent, srTender );
		pUser->AddDefinedText( TID_GAME_SECRETROOM_TENDERCOMPLETE );
		// �α�
		aLogItem.Gold2 = pUser->GetGold();
		aLogItem.Gold_1 = -nPenya;
	}
	else	// �̹� ������ ���
	{
		if( m_vecSecretRoomTender[nIndex].nPenya < nPenya )
		{
			// ������ ��İ� �����ϸ�
			if( pUser->GetGold() < (nPenya - m_vecSecretRoomTender[nIndex].nPenya) )
			{
				pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENOUGHPENYA );
				return;
			}
			else
			{
				pUser->AddGold( m_vecSecretRoomTender[nIndex].nPenya - nPenya );
				m_vecSecretRoomTender[nIndex].nPenya = nPenya;
				// DB�� Update
				g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, m_vecSecretRoomTender[nIndex], 'T' );
				pUser->AddDefinedText( TID_GAME_SECRETROOM_TENDERCOMPLETE );
				// �α�
				aLogItem.RecvName = "SECRETROOM_ADD";
				aLogItem.Gold2 = pUser->GetGold();
				aLogItem.Gold_1 = m_vecSecretRoomTender[nIndex].nPenya - nPenya;
			}
		}
		else
		{
			pUser->AddDefinedText( TID_GAME_SECRETROOM_TRYMOREPENYA );
			return;
		}
	}

	g_DPSrvr.OnLogItem( aLogItem );

	SortTenderGuild();
}

struct TenderCompare :	public binary_function< __SECRETROOM_TENDER, __SECRETROOM_TENDER, bool>
{
	bool operator() ( const __SECRETROOM_TENDER & lhs, const __SECRETROOM_TENDER & rhs )	const
	{
		return ( lhs.nPenya > rhs.nPenya );
	}
};

void CSecretRoomContinent::SortTenderGuild()
{
	// ������ ������ ����
	//std::sort( m_vecSecretRoomTender.begin(), m_vecSecretRoomTender.end(), TenderCompare() );
	
	if( m_vecSecretRoomTender.size() < 2 )
		return;
	for( DWORD i=0; i<m_vecSecretRoomTender.size()-1; i++ )
	{
		for( DWORD j=0; j<m_vecSecretRoomTender.size()-i-1; j++ )
		{
			if( m_vecSecretRoomTender[j].nPenya < m_vecSecretRoomTender[j+1].nPenya )
			{
				__SECRETROOM_TENDER temp = m_vecSecretRoomTender[j];
				m_vecSecretRoomTender[j] = m_vecSecretRoomTender[j+1];
				m_vecSecretRoomTender[j+1] = temp;
			}
		}
	}
}

void CSecretRoomContinent::SetContTenderCancelReturn( CUser* pUser )
{
	int nIndex = GetTenderGuild( pUser->m_idGuild );
	if( nIndex == NULL_ID )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOTENDERGUILD, "\"%s\"", CContinent::GetInstance()->GetContinentName( m_nContinent ) );
		return;
	}
	
	int nPenya = m_vecSecretRoomTender[nIndex].nPenya;
	nPenya	= (int)( nPenya - ( nPenya * static_cast<float>(CSecretRoomMng::GetInstance()->m_nCancelReturnRate/100.0f) ) );
	
	if( (pUser->GetGold() + nPenya) < 0 )
	{
		pUser->AddDefinedText( TID_GAME_LACKSPACE );
		return;
	}

	// �α� ���� �غ�
	LogItemInfo aLogItem;
	aLogItem.Action = "h";
	aLogItem.SendName = pUser->GetName();
	aLogItem.RecvName = "SECRETROOM_CANCEL";
	aLogItem.WorldId = pUser->GetWorld()->GetID();
	//aLogItem.ItemName = "SEED";
	_stprintf( aLogItem.szItemName, "%d", II_GOLD_SEED1 );
	aLogItem.Gold = pUser->GetGold();

	// DB�� Update - 'C'
	g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, m_vecSecretRoomTender[nIndex], 'C' );	
	m_vecSecretRoomTender.erase( remove( m_vecSecretRoomTender.begin(), m_vecSecretRoomTender.end(), pUser->m_idGuild ) );
	pUser->AddGold( nPenya );
	pUser->AddDefinedText( TID_GAME_SECRETROOM_CANCELRETURN, "%d", nPenya );

	//�α�
	aLogItem.Gold2 = pUser->GetGold();
	aLogItem.Gold_1 = nPenya;
	g_DPSrvr.OnLogItem( aLogItem );
}

void CSecretRoomContinent::SetContLineUp( CUser* pUser, vector<DWORD> vecLineUpMember )
{
	int nIndex = GetTenderGuild( pUser->m_idGuild );
	if( nIndex == NULL_ID )
		return;

	if( ( (int)( vecLineUpMember.size() ) < CSecretRoomMng::GetInstance()->m_nMinGuildMemberNum)
		|| ( (int)( vecLineUpMember.size() ) > CSecretRoomMng::GetInstance()->m_nMaxGuildMemberNum) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_LINEUPMINMAX );
		return;
	}

	BOOL bMaster = FALSE;
	for( DWORD i=0; i<vecLineUpMember.size(); i++ )
	{
		CUser* pUserTemp = (CUser*)prj.GetUserByID( vecLineUpMember[i] );
		if( !IsValidObj( pUserTemp ) || pUserTemp->m_nLevel < CSecretRoomMng::GetInstance()->m_nMinGuildMemberLevel )
		{
			pUser->AddDefinedText( TID_GAME_SECRETROOM_LOWMEMBERLEVEL, "%d", CSecretRoomMng::GetInstance()->m_nMinGuildMemberLevel );
			return;
		}

		if( CSecretRoomMng::GetInstance()->IsGuildMaster( pUserTemp ) )
			bMaster = TRUE;
	}

	if( !bMaster )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOGUILDMASTER );
		return;
	}

	m_vecSecretRoomTender[nIndex].vecLineUpMember = vecLineUpMember;
	pUser->AddDefinedText( TID_GAME_SECRETROOM_LINEUPCOMPLETE );
	// DB�� ������ ��� ���� - ������ ����� ����� delete�ϰ� ���� Insert�Ѵ�.
	g_dpDBClient.SendSecretRoomInsertLineUpToDB( m_nContinent, m_vecSecretRoomTender[nIndex] );
}

void CSecretRoomContinent::SecretRoomContOpen()
{
	m_nState = SRCONT_OPEN;

	vector<__SECRETROOM_TENDER>::iterator it = m_vecSecretRoomTender.begin();
	for( int nCount=1; it!=m_vecSecretRoomTender.end(); nCount++ )
	{
		// �������� �з��� ��� ó��
		if( nCount > CSecretRoomMng::GetInstance()->m_nMaxGuildNum )
		{
			CGuild* pGuild = g_GuildMng.GetGuild( (*it).dwGuildId );
			if( pGuild )
			{
				// �������� �߼�
				int nPenya = (*it).nPenya;
				nPenya	= (int)( nPenya - ( nPenya * static_cast<float>(CSecretRoomMng::GetInstance()->m_nDropoutReturnRate/100.0f) ) );
				CItemElem itemElem;
				char szTitle[256] = {0,};
				char szSub[1024] = {0,};
				sprintf( szTitle, prj.GetText( TID_GAME_SECRETROOM_RETURNPENYA_POST_TITLE ) );
				sprintf( szSub, prj.GetText( TID_GAME_SECRETROOM_RETURNPENYA_POST_SUBSTANCE ) );
				g_dpDBClient.SendQueryPostMail( pGuild->m_idMaster, 0, itemElem, nPenya, szTitle, szSub );
				
				// ���� �渶�� �������̸� �ý��� �޼��� ���
				CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
				if( IsValidObj( pUserTemp ) )
					pUserTemp->AddDefinedText( TID_GAME_SECRETROOM_TENDERFAILED );
			}
			// DB�� Update - 'F'
			g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, (*it), 'F' );	
			it	= m_vecSecretRoomTender.erase( it );
		}
		else // �����ȿ� ������� �� ����� �濡 ������ų ���� ����� �غ��Ѵ�.
		{
			(*it).nWarState = MONSTER_NORMAL;
			(*it).vecMonster.assign( CSecretRoomMng::GetInstance()->m_vecNormalMonster.begin(), CSecretRoomMng::GetInstance()->m_vecNormalMonster.end() );
			it++;
		}
	}
}

void CSecretRoomContinent::SetSecretRoomWorld( int nContCount )
{
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		DWORD dwWorldId = WI_DUNGEON_SECRET_1 + i + (CSecretRoomMng::GetInstance()->m_nMaxGuildNum*nContCount);
		if( dwWorldId <= WI_DUNGEON_SECRET_L )
		{
			m_vecSecretRoomTender[i].dwWorldId = dwWorldId;
			
			CGuild* pGuild = g_GuildMng.GetGuild( m_vecSecretRoomTender[i].dwGuildId );
			if( pGuild )
			{
				CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
				if( IsValidObj( pUserTemp ) )
					CSecretRoomMng::GetInstance()->SendNowState( pUserTemp, SRCONT_CLOSE, 0 );
			}
		}
	}
}

void CSecretRoomContinent::SetTeleportSecretRoomCont( CUser* pUser )
{
	int nIndex = GetTenderGuild( pUser->m_idGuild );
	if( nIndex == NULL_ID )
		return;
	
	if( !IsLineUpMember( pUser ) )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_NOLINEUPMEMBER );
		return;
	}

	// �й��� ���
	if( m_vecSecretRoomTender[nIndex].nWarState == MONSTER_FAILED )
	{
		pUser->AddDefinedText( TID_GAME_SECRETROOM_ISLOSTGUILD );
		return;
	}
	
	// ���� ���϶�
	if( m_vecSecretRoomTender[nIndex].nWarState == MONSTER_BOSS )
       SetTeleportBossRoom( pUser, nIndex );
	else
		SetTeleportNormalRoom( pUser, nIndex );
	
	// ���� ���ֱ�
	pUser->RemoveCommonBuffs();
	
	// ��Ƽ Ż��
	if( pUser->GetPartyId() > 0 )
		g_DPCoreClient.SendGCRemoveParty( pUser->GetPartyId(), pUser->m_idPlayer );
}

// �ش� ��尡 ���� ���Ϳ� �������� �ʴ� ��� ����� �� �Ա��� �̵�
// ������ ��ǥ���� Ư�� �����ȿ� �����ϰ� ��ġ�Ѵ�.
void CSecretRoomContinent::SetTeleportNormalRoom( CUser* pUser, int nIndex )
{
	int nRandx = xRandom(4) - 2;
	int nRandz = xRandom(4) - 2;

	pUser->SetAngle( 180.0f );
	pUser->REPLACE( g_uIdofMulti, m_vecSecretRoomTender[nIndex].dwWorldId, D3DXVECTOR3( (float)( 295 + nRandx ), (float)( 102 ), (float)( 530 + nRandz ) ), REPLACE_NORMAL, nDefaultLayer );
}

// �ش� ��尡 ���� ���Ϳ� �������� ��� ���������� �̵�
// ������ ��ǥ���� Ư�� �����ȿ� �����ϰ� ��ġ�Ѵ�.
void CSecretRoomContinent::SetTeleportBossRoom( CUser* pUser, int nIndex )
{
	int nRandx = xRandom(4) - 2;
	int nRandz = xRandom(4) - 2;

	pUser->SetAngle( 180.0f );
	pUser->REPLACE( g_uIdofMulti, m_vecSecretRoomTender[nIndex].dwWorldId, D3DXVECTOR3( (float)( 600 + nRandx ), (float)( 102 ), (float)( 310 + nRandz ) ), REPLACE_NORMAL, nDefaultLayer );
}

// state�� ���� ���� ������Ŵ(�ʴ� nCount��ŭ ���������� ����)
// ��ũ��Ʈ���� ������ �����ȿ� ������ ��ġ�� ����
void CSecretRoomContinent::CreateMonster()
{
	int nCount = CSecretRoomMng::GetInstance()->m_nMonsterGenNum;

	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
			continue;

		if( m_vecSecretRoomTender[i].vecMonster.empty() )
			continue;

		vector<__SECRETROOM_MONSTER>::iterator it = m_vecSecretRoomTender[i].vecMonster.begin();
		if( (*it).nNum < nCount )
				nCount = (*it).nNum;
	
		CWorld* pWorld = g_WorldMng.GetWorld( m_vecSecretRoomTender[i].dwWorldId );
		if( !pWorld )
			continue;
				
		MoverProp* pMoverProp = prj.GetMoverPropEx( (*it).dwId );
		if( pMoverProp && pMoverProp->dwID != 0 )
		{
			for( int j=0; j<nCount; j++ )
			{
				CObj* pObj	= CreateObj( D3DDEVICE, OT_MOVER, pMoverProp->dwID );
				if( NULL == pObj )	
					return;	
				D3DXVECTOR3 vPos;
				vPos.x = (float)( (*it).x1 < (*it).x2 ? xRandom( (*it).x1, (*it).x2 ) : xRandom( (*it).x2, (*it).x1 ) );
				vPos.y = (float)( (*it).y );
				vPos.z = (float)( (*it).z1 < (*it).z2 ? xRandom( (*it).z1, (*it).z2 ) : xRandom( (*it).z2, (*it).z1 ) );

				pObj->SetPos( vPos );
				pObj->InitMotion( MTI_STAND );
				pObj->UpdateLocalMatrix();

				((CMover*)pObj)->m_bActiveAttack =(*it).bRed;
				pWorld->ADDOBJ( pObj, TRUE, nDefaultLayer );
			}
		}
		(*it).nNum -= nCount;
		if( (*it).nNum <= 0 )
			m_vecSecretRoomTender[i].vecMonster.erase(it);
	}
}

// ���峻 ��� ���� ����
void CSecretRoomContinent::RemoveAllSecretRoomObj( DWORD dwWorldId )
{
	CWorld* pWorld = g_WorldMng.GetWorld( dwWorldId );
	if( !pWorld )
		return;

	CObj* pObj;
	for( DWORD i=0; i<pWorld->m_dwObjNum; i++ )
	{
		pObj	= pWorld->m_apObject[i];
		if( pObj && pObj->GetType() == OT_MOVER && !((CMover*)pObj)->IsPlayer() && !pObj->IsDelete() )
			pObj->Delete();
	}
}

// ����� �濡�� ���Ͱ� �׾��� �� ó��
// ���� state�� ���͸� ��� ������ ���� state�� �ѱ��.
// ���� ���͸� kill�� ��� �¸��ϰ� �ش� ��� ����� ���� ����ȴ�.
void CSecretRoomContinent::SetSecretRoomContKill( DWORD dwWorldId, DWORD dwMonsterId, BOOL bKill )
{
	if( m_nState != SRCONT_WAR )
		return;

	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].dwWorldId != dwWorldId )
			continue;

		if( bKill )	// ���� ���Ͱ� �׾��� �� ����� �� ���� ���� ���� Ȯ���ؼ� ųī��Ʈ ����
		{
			if( !IsSecretRoomMonster( dwMonsterId, m_vecSecretRoomTender[i].nWarState ) )
				return;

			m_vecSecretRoomTender[i].nKillCount++;
			UpdateInfoToAllMember( SECRETROOM_KILLCOUNT, i );
		}

		// ���� monster state�� ��� ���͸� kill �� ���...
		if( m_vecSecretRoomTender[i].nKillCount >= CSecretRoomMng::GetInstance()->m_mapMonsterNum.find(m_vecSecretRoomTender[i].nWarState)->second )
		{
			m_vecSecretRoomTender[i].nKillCount = 0;
			switch( m_vecSecretRoomTender[i].nWarState )
			{
				case MONSTER_NORMAL :
					{
						m_vecSecretRoomTender[i].nWarState = MONSTER_MIDBOSS;
						m_vecSecretRoomTender[i].vecMonster.assign( CSecretRoomMng::GetInstance()->m_vecMidBossMonster.begin(), CSecretRoomMng::GetInstance()->m_vecMidBossMonster.end() );
						UpdateInfoToAllMember( SECRETROOM_WARSTATE, i );
						SetSecretRoomContKill( dwWorldId, NULL_ID, FALSE );
					}
					break;

				case MONSTER_MIDBOSS :
					{
						m_vecSecretRoomTender[i].nWarState = MONSTER_BOSS;
						m_vecSecretRoomTender[i].vecMonster.assign( CSecretRoomMng::GetInstance()->m_vecBossMonster.begin(), CSecretRoomMng::GetInstance()->m_vecBossMonster.end() );
						UpdateInfoToAllMember( SECRETROOM_WARSTATE, i );
						// ��� �������� ���� ������ �ڷ���Ʈ
						for( DWORD j=0; j<m_vecSecretRoomTender[i].vecLineUpMember.size(); j++ )
						{
							CUser* pUserTemp = (CUser*)prj.GetUserByID( m_vecSecretRoomTender[i].vecLineUpMember[j] );
							if( IsValidObj( pUserTemp ) && CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUserTemp ) )
								SetTeleportBossRoom( pUserTemp, i );
						}
						SetSecretRoomContKill( dwWorldId, NULL_ID, FALSE );
					}
					break;

				case MONSTER_BOSS :
					{
						//����ó��
						m_vecSecretRoomTender[i].nWarState = MONSTER_WIN;
						UpdateInfoToAllMember( SECRETROOM_WARSTATE, i );
						CGuild* pGuild = g_GuildMng.GetGuild( m_vecSecretRoomTender[i].dwGuildId );
						CString strTemp;
						if( pGuild )
							strTemp.Format( prj.GetText( TID_GAME_SECRETROOM_WINNER ),
								CContinent::GetInstance()->GetContinentName( m_nContinent ), pGuild->m_szGuild );
						
						g_DPCoreClient.SendCaption( strTemp );
						SetContCloseWait();
					}
					break;
			}
		}
	}
}

// ����� ���� ����ǰ� ������ ���� ���� ����Ѵ�.
void CSecretRoomContinent::SetContCloseWait()
{
	m_nState = SRCONT_CLOSEWAIT;
	m_dwRemainTime = (DWORD)CSecretRoomMng::GetInstance()->m_Lua.GetGlobalNumber( "CloseWaitTime" ) + GetTickCount();
	m_nFailedGuild = 0;
	CSecretRoomMng::GetInstance()->SendNowStateAllMember( m_nContinent, m_nState, m_dwRemainTime - GetTickCount() );
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		// ���⼭ ���� ���Ͱ� ���� ���ŵȴ�.
//		if( m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
//			continue;
//		else
			RemoveAllSecretRoomObj( m_vecSecretRoomTender[i].dwWorldId );

		if( m_vecSecretRoomTender[i].nWarState == MONSTER_WIN )
		{
	#if __VER >= 12 // __TAX
			CTax::GetInstance()->SetNextSecretRoomGuild( m_nContinent, m_vecSecretRoomTender[i].dwGuildId );
			CTax::GetInstance()->SendSetTaxRateOpenWnd( m_nContinent, m_vecSecretRoomTender[i].dwGuildId );
	#endif // __TAX
			// DB�� Update - 'W' : ��� ���
			g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, m_vecSecretRoomTender[i], 'W' );
		}
		else
			// DB�� Update - 'L' : �������� ���� �й� ���� ����
			g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, m_vecSecretRoomTender[i], 'L' );
	}
}

// ����� ���� ���� ����ǰ� ��� �÷��̾ ������ �̵���Ų��.
void CSecretRoomContinent::SetContClose()
{
	m_nState = SRCONT_CLOSE;
	m_dwRemainTime = 0;
	CSecretRoomMng::GetInstance()->SendNowStateAllMember( m_nContinent, m_nState );
	// ���⼭ ��� �÷��̾ nWarState != MONSTER_FAILED �� ������ ���� ������ ��������.
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
			continue;

		for( DWORD j=0; j<m_vecSecretRoomTender[i].vecLineUpMember.size(); j++ )
		{
			CUser* pUserTemp = (CUser*)prj.GetUserByID( m_vecSecretRoomTender[i].vecLineUpMember[j] );
			if( IsValidObj( pUserTemp ) && CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUserTemp ) )
				pUserTemp->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, CContinent::GetInstance()->GetRevivalPos( m_nContinent ), REPLACE_NORMAL, nDefaultLayer );
		}
	}
	
	m_vecSecretRoomTender.clear();
}

// ���� ����� ����� �� ���¸� �÷��̾�� �����Ѵ�.
void CSecretRoomContinent::UpdateInfoToMember( CUser* pUser, BYTE nType, int nIndex )
{
	if( IsValidObj( pUser ) )
		pUser->AddSecretRoomContInfo( m_nContinent, nType, m_vecSecretRoomTender, nIndex );
}

// ���� ����� ����� �� ���¸� ������ ��� �÷��̾�� �����Ѵ�.
void CSecretRoomContinent::UpdateInfoToAllMember( BYTE nType, int nIndex )
{
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
			continue;

		for( DWORD j=0; j<m_vecSecretRoomTender[i].vecLineUpMember.size(); j++ )
		{
			CUser* pUserTemp = (CUser*)prj.GetUserByID( m_vecSecretRoomTender[i].vecLineUpMember[j] );
			UpdateInfoToMember( pUserTemp, nType, nIndex );
		}
	}
}

// �渶�� �ش� ����� ��ȿ� �����ϴ��� �˻�
void CSecretRoomContinent::ProcessInGuildMaster()
{
	for( DWORD i=0; i<m_vecSecretRoomTender.size(); i++ )
	{
		if( m_vecSecretRoomTender[i].nWarState == MONSTER_FAILED )
			continue;

		CGuild* pGuild = g_GuildMng.GetGuild( m_vecSecretRoomTender[i].dwGuildId );
		if( pGuild )
		{
			CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
			if( IsValidObj( pUserTemp ) && pUserTemp->GetWorld() &&
				pUserTemp->GetWorld()->GetID() == m_vecSecretRoomTender[i].dwWorldId )
				continue;
		}
		
		SetContFailGuild( m_vecSecretRoomTender[i].dwGuildId );
	}
}

// �渶�� �װų� �α� �ƿ��ϴ� ���� �й� ������ �߻��Ͽ� �ش� ��带 �й� ó���� �� 
// ������ �̵���Ų��. �ش� ����� ���� ��� ���͵� ���ŵȴ�.
void CSecretRoomContinent::SetContFailGuild( DWORD dwGuildId )
{
	int nIndex = GetTenderGuild( dwGuildId );
	if( nIndex == NULL_ID )
		return;

	// DB�� Update - 'L'
	g_dpDBClient.SendSecretRoomUpdateToDB( m_nContinent, m_vecSecretRoomTender[nIndex], 'L' );	
	
	m_vecSecretRoomTender[nIndex].nWarState = MONSTER_FAILED;
	m_nFailedGuild++;
	UpdateInfoToAllMember( SECRETROOM_WARSTATE, nIndex );
	for( DWORD i=0; i<m_vecSecretRoomTender[nIndex].vecLineUpMember.size(); i++ )
	{
		CUser* pUserTemp = (CUser*)prj.GetUserByID( m_vecSecretRoomTender[nIndex].vecLineUpMember[i] );
		if( IsValidObj( pUserTemp ) )
		{
			if( CSecretRoomMng::GetInstance()->IsInTheSecretRoom( pUserTemp ) )
				pUserTemp->REPLACE( g_uIdofMulti, WI_WORLD_MADRIGAL, CContinent::GetInstance()->GetRevivalPos( m_nContinent ), REPLACE_NORMAL, nDefaultLayer );
			UpdateInfoToMember( pUserTemp, SECRETROOM_WARSTATE, nIndex );
			CSecretRoomMng::GetInstance()->SendNowState( pUserTemp, SRCONT_CLOSE, 0 );
		}
	}

	// ��� ����� �� ���� ����
	RemoveAllSecretRoomObj( m_vecSecretRoomTender[nIndex].dwWorldId );
}

// ���Ͱ� kill������ ��� �ش� ���Ͱ� ����� �� ���������� �Ǻ��Ѵ�.
BOOL CSecretRoomContinent::IsSecretRoomMonster( DWORD dwMonsterId, int nWarState )
{
	if( dwMonsterId == NULL_ID )
		return FALSE;

	vector<__SECRETROOM_MONSTER>::iterator it, itEnd;
	switch( nWarState )
	{
		case MONSTER_NORMAL :
			it = CSecretRoomMng::GetInstance()->m_vecNormalMonster.begin();
			itEnd = CSecretRoomMng::GetInstance()->m_vecNormalMonster.end();
			break;
		case MONSTER_MIDBOSS :
			it = CSecretRoomMng::GetInstance()->m_vecMidBossMonster.begin();
			itEnd = CSecretRoomMng::GetInstance()->m_vecMidBossMonster.end();
			break;
		case MONSTER_BOSS :
			it = CSecretRoomMng::GetInstance()->m_vecBossMonster.begin();
			itEnd = CSecretRoomMng::GetInstance()->m_vecBossMonster.end();
			break;
	}
	
	for( ; it!=itEnd; it++ )
	{
        if( (*it).dwId == dwMonsterId )
			return TRUE;
	}

	return FALSE;
}
#endif // __WORLDSERVER

//////////////////////////////////////////////////////////////////////////////
#endif // __SECRET_ROOM