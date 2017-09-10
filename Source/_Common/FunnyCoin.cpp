// FunnyCoin.cpp: implementation of the CFunnyCoin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef __FUNNY_COIN
#include "FunnyCoin.h"

const int FC_DBSUCCESS = 1;
const int FC_DBFAILED = -1;
const int FC_SAMEUNIQUENO = -2;

#ifdef __WORLDSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoin ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "User.h"
#include "defineText.h"
#include "DPSrvr.h"
extern CDPSrvr g_DPSrvr;
#include "DPDatabaseClient.h"
extern CDPDatabaseClient g_dpDBClient;

CFunnyCoin::CFunnyCoin()
{
	m_vecAckWaitPlayer.clear();
}

CFunnyCoin::~CFunnyCoin()
{
	for( size_t i=0; i<m_vecAckWaitPlayer.size(); i++ )
		Error( "FunnyCoin AckWait Player - %07d", m_vecAckWaitPlayer[i] );
	
	m_vecAckWaitPlayer.clear();
}

CFunnyCoin* CFunnyCoin::GetInstance()
{
	static CFunnyCoin sFunnyCoin;
	return & sFunnyCoin;
}

BOOL CFunnyCoin::DoUseFunnyCoin( CUser* pUser, CItemElem* pItemElem )	// �۴������� ����Ѵ�.
{
	if ( !IsUsable( pUser ) )
		return FALSE;
	
	m_vecAckWaitPlayer.push_back( pUser->m_idPlayer );
	g_dpDBClient.SendFunnyCoinReqUse( pUser->m_idPlayer, pItemElem );	// Trans Server �� ����
	g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_USE", pItemElem );
	
	return TRUE;
}

void CFunnyCoin::OnFunnyCoinAckUse( CAr & ar )	// �۴����� ��뿡 ���� ������ TranServer�� ���� �޾Ҵ�.
{
	DWORD	dwPlayerId, dwItemId;
	SERIALNUMBER	dwSerialNumber;
	int		nResult;

	ar >> dwPlayerId >> dwItemId >> dwSerialNumber >> nResult;

	if( IsAckWaitPlayer( dwPlayerId, TRUE ) )
	{
		CUser* pUser = static_cast<CUser*>( prj.GetUserByID( dwPlayerId ) );	
		switch( nResult )
		{
			case FC_DBSUCCESS:
				{
					ItemProp* pItemProp = prj.GetItemProp( dwItemId );
					if( IsValidObj( pUser ) && pItemProp )
						pUser->AddDefinedText( TID_GAME_FUNNYCOIN_USECOMPLETE, "\"%s\"", pItemProp->szName );
				}
				break;

			case FC_DBFAILED:
				{
					if( IsValidObj( pUser ) )
					{
						CItemElem itemElem;
						itemElem.m_dwItemId = dwItemId;
						itemElem.m_nItemNum = 1;
						itemElem.SetSerialNumber( dwSerialNumber );		// ���� �ø��� �ѹ��� �����Ѵ�.
						if( pUser->CreateItem( &itemElem ) )	
							g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_REPAIR_SUCCESS", &itemElem );
						else	// �κ��丮�� �������� ���� ������ ������ ���� ����!!
						{
							g_DPSrvr.PutItemLog( pUser, "M", "FUNNYCOIN_REPAIR_FAILED", &itemElem );
							Error( "CFunnyCoin::OnAckFunnyCoinAckUse() - FunnyCoin Repair Failed! [PlayerId:%07d], [Item:%d], [SerialNumber:%d]",
									dwPlayerId, dwItemId, dwSerialNumber );
						}
					}
					else	// �ش� �÷��̾ ���� ������ �ʴ�.
						Error( "CFunnyCoin::OnAckFunnyCoinAckUse() - FunnyCoin Repair Failed!(Invalid User) [PlayerId:%07d], [Item:%d], [SerialNumber:%d]",
								dwPlayerId, dwItemId, dwSerialNumber );
				}
				break;

			case FC_SAMEUNIQUENO:
				{
					Error( "CFunnyCoin::OnAckFunnyCoinAckUse() - Same SerialNumber! [PlayerId:%07d], [Item:%d], [SerialNumber:%d]",
							dwPlayerId, dwItemId, dwSerialNumber );
				}
				break;
		}
	}
	else	// �۴����� ��� ���� ��� �÷��̾ �ƴϴ�.
		Error( "CFunnyCoin::OnAckFunnyCoinAckUse() - Is Not AckWait Player [PlayerId:%07d]", dwPlayerId );
}

BOOL CFunnyCoin::IsAckWaitPlayer( DWORD dwPlayerId, BOOL bRemove )	// �۴������� ����� �� DB������ ��ٸ��� �ִ� �÷��̾� �ΰ�?
{																	// bRemove�� TRUE�̸� �ش� ����ڸ� �����Ѵ�.
	for( vector<DWORD>::iterator it=m_vecAckWaitPlayer.begin(); it!=m_vecAckWaitPlayer.end(); it++ )
	{
		if( (*it) == dwPlayerId )
		{
			if( bRemove )	
				m_vecAckWaitPlayer.erase( it );

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFunnyCoin::IsUsable( CUser* pUser )
{
	switch( ::GetLanguage() )
	{
		case LANG_KOR :	
			{
				CString strTemp;
				strTemp.Format( "%s", pUser->m_playAccount.lpszAccount );
				if( strTemp.Right( 4 ) == "__bu" )
				{
					pUser->AddDefinedText( TID_GAME_FUNNYCOIN_NOUSEBUDDYPLAYER );
					return FALSE;
				}

				if( IsAckWaitPlayer( pUser->m_idPlayer ) )
				{
					//pUser->AddText( "����Ŀ� �ٽ� ����ϰԳ�..." );
					return FALSE;
				}

				return TRUE;
			}
	}
	
	return FALSE;
}
#endif // __WORLDSERVER

#ifdef __DBSERVER
//////////////////////////////////////////////////////////////////////
// CFunnyCoinDbCtrl //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "dptrans.h"
extern AppInfo g_appInfo;

CFunnyCoinDbCtrl::CFunnyCoinDbCtrl()
{
}

CFunnyCoinDbCtrl::~CFunnyCoinDbCtrl()
{
}

void CFunnyCoinDbCtrl::Handler( LPDB_OVERLAPPED_PLUS pov, DWORD dwCompletionKey )
{
	switch( pov->nQueryMode )
	{
		case FC_USE:
			{
				DWORD dwPlayerId, dwItemId;
				SERIALNUMBER dwSerialNumber;

				CAr ar( pov->lpBuf, pov->uBufSize );
				ar >> dwPlayerId;
				ar >> dwItemId >> dwSerialNumber;

				int nResult = InsertFunnyCoin( dwPlayerId, dwItemId, dwSerialNumber );
				CDPTrans::GetInstance()->SendFunnyCoinAckUse( dwPlayerId, dwItemId, dwSerialNumber, nResult, dwCompletionKey );
			}
			break;
	
		default:
			break;
	}
}

int CFunnyCoinDbCtrl::InsertFunnyCoin( DWORD dwPlayerId, DWORD dwItemId, SERIALNUMBER dwSerialNumber )	// ����� �۴������� DB�� �����Ѵ�.
{
	ItemProp* pItemProp = prj.GetItemProp( dwItemId );
	if( !pItemProp )
		return FC_DBFAILED;
	
	CQuery* pQuery = GetQueryObject();
	if( !pQuery->Execute( "usp_FunnyCoin_input '%02d', '%07d', %d, %d, %d", g_appInfo.dwSys, dwPlayerId, dwItemId, pItemProp->dwCost, dwSerialNumber ) )
	{
		Error( "Call \"usp_FunnyCoin_input\" FAILED! - %07d, %d", dwPlayerId, dwItemId );
		return FC_DBFAILED;
	}

	if( pQuery->Fetch() )
	{
		int nResult = pQuery->GetInt( "result" );
		if( nResult != FC_DBSUCCESS )	// ����chipi	
		{
			Error( "usp_FunnyCoin_input Result is Fail - ErrorCode : %d, %07d, %d, %d", nResult, dwPlayerId, dwItemId, dwSerialNumber );
			return nResult;
		}
	}
	else
		return FC_DBFAILED;

	// ���� �ߴ�!!
	return FC_DBSUCCESS;
}

//////////////////////////////////////////////////////////////////////
// CFunnyCoin ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
CFunnyCoinDbMng::CFunnyCoinDbMng()
{
	if( !m_FCDbCtrl.CreateDbHandler() )
		Error( "CFunnyCoinDbMng - m_FCDbCtrl.CreateDbHandler()" );
}

CFunnyCoinDbMng::~CFunnyCoinDbMng()
{
	m_FCDbCtrl.CloseDbHandler();
}

CFunnyCoinDbMng* CFunnyCoinDbMng::GetInstance()
{
	static CFunnyCoinDbMng sFunnyCoinDbMng;
	return &sFunnyCoinDbMng;
}
#endif // __DBSERVER

#endif // __FUNNY_COIN