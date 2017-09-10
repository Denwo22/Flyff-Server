#include "stdafx.h"

#ifdef __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�
#include "DPCoreClient.h"
#endif // __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�

#include "DPCacheSrvr.h"
#include "DPClient.h"
#include "Player.h"
#include "ServerDesc.h"
#include "MyTrace.h"
#ifdef __TRACE1027

#endif	// __TRACE1027

#ifdef __CRASH_0404
#include "crashstatus.h"
#endif	// __CRASH_0404

#ifdef __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�
extern	CDPCoreClient	g_DPCoreClient;
#endif // __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�

extern	CDPCacheSrvr		g_DPCacheSrvr;
extern	CMyTrace	g_MyTrace;

CDPClientArray	g_DPClientArray;

CDPClient::CDPClient()
{
	BEGIN_MSG;
	ON_MSG( PACKETTYPE_QUERY_DESTROY_PLAYER, &CDPClient::OnQueryDestroyPlayer );

	m_pServer	= NULL;
	pNext		= NULL;
}

CDPClient::~CDPClient()
{

}

void CDPClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
				g_DPClientArray.Remove( this );
				break;
			}
	}
}

void CDPClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	static size_t	nSize	= sizeof(DPID);

#ifdef __CRASH_0404
	CCrashStatus::GetInstance()->SetLastPacket( this, 0xEEEEEEEE );
#endif	// __CRASH_0404

	LPBYTE lpBuf	= (LPBYTE)lpMsg + nSize;
	ULONG	uBufSize	= dwMsgSize - nSize;
	DPID dpidUser	= *(UNALIGNED LPDPID)lpMsg;

	CAr ar( lpBuf, uBufSize );
	GETTYPE( ar );

	void ( theClass::*pfn )( theParameters )	=	GetHandler( dw );
	
#ifdef __CRASH_0404
	CCrashStatus::GetInstance()->SetLastPacket( this, dw );
#endif	// __CRASH_0404

	if( pfn )
		( this->*( pfn ) )( ar, dpidUser, lpMsg, dwMsgSize );
	else
		g_DPCacheSrvr.Send( lpBuf, uBufSize, dpidUser );

#ifdef __CRASH_0404
	CCrashStatus::GetInstance()->SetLastPacket( this, 0xFFFFFFFF );
#endif	// __CRASH_0404
}


void CDPClient::OnReplace( CAr & ar, DPID dpidUser, LPVOID lpBuffer, u_long uBufSize )
{
}

void CDPClient::SendJoin( CPlayer* pPlayer )
{
	BEFORESENDSOLE( ar, PACKETTYPE_JOIN, pPlayer->GetSerial() );
	ar << pPlayer->GetAuthKey();
	ar << pPlayer->GetPlayerId();
	ar << pPlayer->GetSlot();
	ar << pPlayer->GetNetworkId();
	ar.WriteString( pPlayer->GetAccount() );
	ar.WriteString( pPlayer->GetPass() );
	ar.WriteString( pPlayer->GetAddr() );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPClient::OnQueryDestroyPlayer( CAr & ar, DPID dpid, LPVOID lpBuffer, u_long uBufSize )
{
	DWORD dwSerial;
	ar >> dwSerial;
	u_long idPlayer;
	ar >> idPlayer;

	CMclAutoLock	Lock( CPlayerMng::Instance()->m_AddRemoveLock );

	CPlayer* pPlayer	= CPlayerMng::Instance()->GetPlayerBySerial( dwSerial );
	if( pPlayer )
	{
		// ���������� ã�����ϱ� ������ ���´�. 
		g_DPCacheSrvr.DestroyPlayer( pPlayer->GetNetworkId() );	// ���Ͽ����� �����, PACKETTYPE_LEAVE�� ���弭���� ����.
#ifdef __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�
		g_DPCoreClient.SendDestroyPlayer( pPlayer );// coresvr�� ���� loginsvr ���� ������ �˷��ش�
#endif // __MA_VER11_07	// cache �������� loginserver�� �������Ӳ����� �˷��ִ� �κ�
		return;	
	}

	WriteError( "OnQueryDestroyPlayer - idPlayer:%d GetPlayer return NULL.", idPlayer );

	// ã�� ���ص�, �����ٰ� �˷��ش�.
	BEFORESENDSOLE( out, PACKETTYPE_LEAVE, DPID_UNKNOWN );
	out << idPlayer;
	LPBYTE lpBuf = out.GetBuffer( &nBufSize );
	g_DPClientArray.SendToServer( dpid, lpBuf, nBufSize );
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CDPClientArray::CDPClientArray()
{
	m_pFirstActive	= NULL;
	m_pFirstFree	= NULL;
}

CDPClientArray::~CDPClientArray()
{
	Free();	
}

void CDPClientArray::Free( void )
{
	CDPClient* pClient, *pRemove;
	CMclAutoLock Lock( m_AddRemoveLock );

	pClient		= m_pFirstActive;
	while( ( pRemove = pClient ) )
	{
		pClient->DeleteDPObject();
		pClient		= pClient->pNext;
		safe_delete( pRemove );
	}

	m_pFirstActive	= NULL;

	pClient		= m_pFirstFree;
	while( ( pRemove = pClient ) )
	{
		pClient->DeleteDPObject();
		pClient		= pClient->pNext;
		safe_delete( pRemove );
	}

	m_pFirstFree	= NULL;
}

int CDPClientArray::Connect( CServerDesc *pServer )
{
	CMclAutoLock Lock( m_AddRemoveLock );
	CServerDescArray::iterator i = m_apServer.find( pServer->GetKey() );
	if( i == m_apServer.end() )
	{
		CDPClient* pClient;
		if( m_pFirstFree == NULL ) {
			pClient		= new CDPClient;
		}
		else {
			pClient		= m_pFirstFree;
			m_pFirstFree	= m_pFirstFree->pNext;
		}
		pClient->m_pServer	= pServer;
		g_MyTrace.Add( pServer->GetKey(), TRUE, "%04d", pServer->GetKey() );
		if( pClient->CreateDPObject( FALSE ) && pClient->InitConnection( (LPVOID)pServer->m_szAddr, PN_WORLDSRVR + (u_short)pServer->GetKey() ) && pClient->JoinSession( NULL ) )
		{
			bool bResult	= m_apServer.insert( CServerDescArray::value_type( pServer->GetKey(), pServer ) ).second;
			ASSERT( bResult );
			Add( pClient );
			g_MyTrace.Add( pServer->GetKey(), FALSE, "%04d", pServer->GetKey() );
			return TRUE;
		}
		SAFE_DELETE( pClient );
	}
	return FALSE;
}


void CDPClientArray::Add( CDPClient* pClient )
{
	pClient->pNext	= m_pFirstActive;
	m_pFirstActive	= pClient;
}

BOOL CDPClientArray::Remove( CDPClient* pClient )
{
	CPlayerMng::Instance()->DestroyPlayer( pClient );
	CDPClient *pCur, *pPrev		= NULL;
	CMclAutoLock Lock( m_AddRemoveLock );

	pCur	= m_pFirstActive;
	while( pCur )
		{
		if( pCur == pClient )
		{
			if( pPrev ) {
				pPrev->pNext	= pCur->pNext;
			}
			else {
				m_pFirstActive	= m_pFirstActive->pNext;
			}

			CServerDescArray::iterator i	= m_apServer.find( pClient->m_pServer->GetKey() );
			if( i != m_apServer.end() )
			{
				m_apServer.erase( pClient->m_pServer->GetKey() );
				g_MyTrace.Add( pClient->m_pServer->GetKey(), TRUE, "%04d", pClient->m_pServer->GetKey() );
				SAFE_DELETE( pClient->m_pServer );
			}

			pClient->pNext	= m_pFirstFree;
			m_pFirstFree	= pClient;

			return TRUE;
		}
		pPrev	= pCur;
		pCur		= pCur->pNext;
	}
	return FALSE;
}

CDPClient* CDPClientArray::GetClient( u_long uIdofMulti, DWORD dwWorldID, const D3DXVECTOR3 & vPos )
{
	if( uIdofMulti == NULL_ID )
		return NULL;

	CDPClient* pClient;
	CMclAutoLock Lock( m_AddRemoveLock );

	pClient		= m_pFirstActive;
	while( pClient )
	{
		if( pClient->m_pServer->GetIdofMulti() == uIdofMulti &&
			pClient->m_pServer->IsUnderJurisdiction( dwWorldID, vPos ) == TRUE )
			return pClient;
		pClient		= pClient->pNext;
	}
	return NULL;
}

void CDPClientArray::SendToServer( DPID dpidUser, LPVOID lpMsg, DWORD dwMsgSize )
{
	CMclAutoLock Lock( m_AddRemoveLock );

	CPlayer* pPlayer = CPlayerMng::Instance()->GetPlayer( dpidUser );
	if( pPlayer )
	{
		if( pPlayer->GetClient() )
			pPlayer->GetClient()->SendToServer( pPlayer->GetSerial(), lpMsg, dwMsgSize );
	}
}
