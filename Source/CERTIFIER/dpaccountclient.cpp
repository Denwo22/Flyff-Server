#include "stdafx.h"
#include "dpcertifier.h"
#include "dpaccountclient.h"
#ifdef __GPAUTH_01
#include "lang.h"
#endif	// __GPAUTH_01

#ifdef __TRACE1027

#endif	// __TRACE1027

#include "user.h"

extern	CDPCertifier	g_dpCertifier;

CDPAccountClient::CDPAccountClient()
{
	BEGIN_MSG;
	ON_MSG( PACKETTYPE_ADD_ACCOUNT, &CDPAccountClient::OnAddAccount );
	ON_MSG( PACKETTYPE_DESTROY_PLAYER, &CDPAccountClient::OnDestroyPlayer );
	ON_MSG( PACKETTYPE_SRVR_LIST, &CDPAccountClient::OnServersetList );
	ON_MSG( PACKETTYPE_PLAYER_COUNT, &CDPAccountClient::OnPlayerCount );
	ON_MSG( PACKETTYPE_ENABLE_SERVER, &CDPAccountClient::OnEnableServer );
}

CDPAccountClient::~CDPAccountClient()
{
}

void CDPAccountClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_DESTROYPLAYERORGROUP:
			break;
	}
}

void CDPAccountClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	static size_t	nSize	= sizeof(DPID);

	LPBYTE lpBuf	= (LPBYTE)lpMsg + nSize;
	ULONG	uBufSize	= dwMsgSize - nSize;
	DPID dpid2	= *(UNALIGNED LPDPID)lpMsg;

	CAr ar( lpBuf, uBufSize );
	GETTYPE( ar );


	void ( theClass::*pfn )( theParameters )	=	GetHandler( dw );
//	ASSERT( pfn );
	if( pfn )
		( this->*( pfn ) )( ar, dpid2 );
}

#ifdef __GPAUTH_02
#ifdef __EUROPE_0514
#if __VER >= 14 // __PCBANG
void CDPAccountClient::SendAddAccount( char* lpAddr, LPCTSTR lpszAccount, BYTE cbAccountFlag, DPID idFrom, int fCheck, const char* szCheck, const char* szBak, DWORD dwPCBangClass )
#else // __PCBANG
void CDPAccountClient::SendAddAccount( char* lpAddr, LPCTSTR lpszAccount, BYTE cbAccountFlag, DPID idFrom, int fCheck, const char* szCheck, const char* szBak )
#endif // __PCBANG
#else	// __EUROPE_0514
void CDPAccountClient::SendAddAccount( char* lpAddr, LPCTSTR lpszAccount, BYTE cbAccountFlag, DPID idFrom, int fCheck, const char* szCheck )
#endif	// __EUROPE_0514
#else	// __GPAUTH_02
void CDPAccountClient::SendAddAccount( char* lpAddr, LPCTSTR lpszAccount, BYTE cbAccountFlag, DPID idFrom, int fCheck )
#endif	// __GPAUTH_02
{
	BEFORESENDSOLE( ar, PACKETTYPE_ADD_ACCOUNT, idFrom );
	ar.WriteString( lpAddr );
	ar.WriteString( lpszAccount );
	ar << cbAccountFlag;

#ifdef __BILLING0712
	ar << fCheck;
#endif//__BILLING0712

#ifdef __GPAUTH_02
	BOOL bGPotatoAuth	= ::GetLanguage() == LANG_GER || ::GetLanguage() == LANG_FRE;
	if( bGPotatoAuth )
		ar.WriteString( szCheck );
#endif	// __GPAUTH_02
#ifdef __EUROPE_0514
	ar.WriteString( szBak );
#endif	// __EUROPE_0514
#if __VER >= 14 // __PCBANG
	ar << dwPCBangClass;
#endif // __PCBANG

	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPAccountClient::SendRemoveAccount( DPID idFrom )
{
	BEFORESENDSOLE( ar, PACKETTYPE_REMOVE_ACCOUNT, idFrom );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPAccountClient::SendRoute( DWORD dwIdofServer, u_long uIdofMulti, DPID idFrom )
{
	BEFORESENDSOLE( ar, PACKETTYPE_ROUTE, idFrom );
	ar << dwIdofServer;
	ar << uIdofMulti;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPAccountClient::SendPing( DPID idFrom )
{
	BEFORESENDSOLE( ar, PACKETTYPE_PING, idFrom );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPAccountClient::SendCloseExistingConnection( const char* lpszAccount )
{
	BEFORESENDSOLE( ar, PACKETTYPE_CLOSE_EXISTING_CONNECTION, DPID_UNKNOWN );
	ar.WriteString( lpszAccount );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPAccountClient::OnAddAccount( CAr & ar, DPID dpid )
{
	BYTE	cbResult;
	DWORD	dwAuthKey = 0;
	BYTE	cbAccountFlag = 0;
	long	lTimeSpan = 0;

	ar >> cbResult;
	ar >> dwAuthKey;
	ar >> cbAccountFlag;

#ifdef __BILLING0712
	ar >> lTimeSpan;
#endif

#ifdef __GPAUTH_01
	char szGPotatoNo[10]	= { 0,};
#ifdef __GPAUTH_02
	char szCheck[255]	= { 0,};
#endif	// __GPAUTH_02
#ifdef __EUROPE_0514
	char szBak[MAX_ACCOUNT]	= { 0,};
#endif	// __EUROPE_0514
	BOOL bGPotatoAuth	= ::GetLanguage() == LANG_GER || ::GetLanguage() == LANG_FRE;
	if( bGPotatoAuth )
	{
		ar.ReadString( szGPotatoNo, 10 );
#ifdef __GPAUTH_02
		ar.ReadString( szCheck, 255 );
#endif	// __GPAUTH_02
	}
#ifdef __EUROPE_0514
	ar.ReadString( szBak, MAX_ACCOUNT );
#endif	// __EUROPE_0514

#endif	// __GPAUTH_01

	switch( cbResult )
	{
		case ACCOUNT_DUPLIACTE:					// �ߺ� ����
			g_dpCertifier.SendError( ERROR_DUPLICATE_ACCOUNT, dpid );			
			break;
		case ACCOUNT_CHECK_OK:					// ����
			{
				{
					CMclAutoLock Lock( CUserMng::GetInstance()->m_AddRemoveLock );
					CUser* pUser	= CUserMng::GetInstance()->GetUser( dpid );
					if( !pUser )
						return;
					pUser->m_dwTick		= 0xffffffff;
#ifdef __EUROPE_0514
					if( lstrcmp( pUser->GetAccount(), szBak ) )
					{
						Error( "CDPAccountClient.OnAddAccount: %s, %s", pUser->GetAccount(), szBak );
						return;
					}
#endif	// __EUROPE_0514
				}

			#ifdef __GPAUTH_01
				#ifdef __GPAUTH_02
#ifdef __EUROPE_0514
				g_dpCertifier.SendServerList( dpid, dwAuthKey, cbAccountFlag, lTimeSpan, szGPotatoNo, szCheck, szBak );
#else	// __EUROPE_0514
				g_dpCertifier.SendServerList( dpid, dwAuthKey, cbAccountFlag, lTimeSpan, szGPotatoNo, szCheck );
#endif	// __EUROPE_0514
				#else	// __GPAUTH_02
				g_dpCertifier.SendServerList( dpid, dwAuthKey, cbAccountFlag, lTimeSpan, szGPotatoNo );
				#endif	// __GPAUTH_02
			#else	// __GPAUTH_01
				g_dpCertifier.SendServerList( dpid, dwAuthKey, cbAccountFlag, lTimeSpan );
			#endif	// __GPAUTH_01
				break;
			}
		case ACCOUNT_EXTERNAL_ADDR:				// ������ �ʴ� �ּ� 
			g_dpCertifier.SendError( ERROR_EXTERNAL_ADDR, dpid );
			break;
		case ACCOUNT_OVERFLOW:					// �ο��ʰ�
			g_dpCertifier.SendError( ERROR_OVERFLOW, dpid );
			break;
		case ACCOUNT_BILLING_INFO_FAILED:		// ���� ���� ����
			g_dpCertifier.SendError( ERROR_BILLING_INFO_FAILED, dpid );
			break;
		case ACCOUNT_BILLING_DATABASE_ERROR:	// ���� DB ���� 
			g_dpCertifier.SendError( ERROR_BILLING_DATABASE_ERROR, dpid );
			break;
		case ACCOUNT_BILLING_TIME_OVER:			// ���� ���ð� ���� 
			g_dpCertifier.SendError( ERROR_BILLING_TIME_OVER, dpid );
			break;
		case ACCOUNT_BILLING_OTHER_ERROR:		// ���� ��Ÿ �ٸ� ���� 
			g_dpCertifier.SendError( ERROR_BILLING_OTHER_ERROR, dpid );
			break;
		case ACCOUNT_BILLING_DISCONNECTED:
			g_dpCertifier.SendError( ERROR_BILLING_DISCONNECTED, dpid );
			break;
		case ACCOUNT_IPCUT_ADDR:
			g_dpCertifier.SendError( ERROR_IP_CUT, dpid );
			break;
		default:
			{
				Error("CDPAccountClient::OnAddAccount result:%d ", cbResult );
				CMclAutoLock Lock( CUserMng::GetInstance()->m_AddRemoveLock );
				CUser* pUser	= CUserMng::GetInstance()->GetUser( dpid );
				if( pUser )
					Error("account: %s", pUser->GetAccount() );
				break;
			}
	}
}

void CDPAccountClient::OnDestroyPlayer( CAr & ar, DPID dpid )
{
	g_dpCertifier.DestroyPlayer( dpid );
}

void CDPAccountClient::OnServersetList( CAr & ar, DPID dpid )
{
	ar.ReadString( g_dpCertifier.m_szVer );
#ifdef __SECURITY_0628
	ar.ReadString( g_dpCertifier.m_szResVer );
#endif	// __SECURITY_0628
	ar >> g_dpCertifier.m_dwSizeofServerset;
	LPSERVER_DESC pServer;

	for( DWORD i = 0; i < g_dpCertifier.m_dwSizeofServerset; i++ )
	{
		pServer		= g_dpCertifier.m_aServerset + i;
		ar >> pServer->dwParent;
		ar >> pServer->dwID;
		ar.ReadString( pServer->lpName, 36 );
		ar.ReadString( pServer->lpAddr, 16 );
		ar >> pServer->b18;
		ar >> pServer->lCount;
		ar >> pServer->lEnable;
		ar >> pServer->lMax;

//		if( pServer->dwParent != NULL_ID )
		{
			u_long uId	= pServer->dwParent * 100 + pServer->dwID;
			g_dpCertifier.m_2ServersetPtr.insert( map<u_long, LPSERVER_DESC>::value_type( uId, pServer ) );
		}
	}
}

void CDPAccountClient::OnPlayerCount( CAr & ar, DPID dpid )
{
	u_long uId;
	long lCount;
	ar >> uId >> lCount;

	map<u_long, LPSERVER_DESC>::iterator i2	= g_dpCertifier.m_2ServersetPtr.find( uId );
	if( i2 != g_dpCertifier.m_2ServersetPtr.end() )
		InterlockedExchange( &i2->second->lCount, lCount );
}

void CDPAccountClient::OnEnableServer( CAr & ar, DPID dpid )
{
	u_long uId;
	long lEnable;
	ar >> uId >> lEnable;

	map<u_long, LPSERVER_DESC>::iterator i2	= g_dpCertifier.m_2ServersetPtr.find( uId );
	if( i2 != g_dpCertifier.m_2ServersetPtr.end() )
		InterlockedExchange( &i2->second->lEnable, lEnable );
}

CDPAccountClient	g_dpAccountClient;