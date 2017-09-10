#include "stdafx.h"
#include "dpsock.h"
#include <process.h>

#ifdef	__VERIFY_LOOP041010
	#undef INIT_LOOP
	#undef VERIFY_LOOP

	#define	INIT_LOOP					int __loop = 0;
	#define VERIFY_LOOP(file, line)		if( ++__loop == 1000 ) {	Error( "%s %d", file, line ); }
#else
	#undef INIT_LOOP
	#undef VERIFY_LOOP

	#define	INIT_LOOP					((void)0);
	#define VERIFY_LOOP(file, line)		((void)0);
#endif	


u_int	__stdcall	IoWorkerThread( LPVOID lpvThreadParameter );
//void	__cdecl	EventWorkerThread( LPVOID lpvThreadParameter );
u_int	__stdcall	EventWorkerThread( LPVOID lpvThreadParameter );

CDPSock::CDPSock()
{
	m_fServer	= FALSE;
	m_pSock		= NULL;
	m_phCompletionPort	= NULL;
	m_lActiveIoWorker	= 0;
	m_hRecv		= CreateEvent( NULL, FALSE, FALSE, NULL );
	ASSERT( m_hRecv );
	m_hClose	= WSACreateEvent();
	ASSERT( m_hClose );
	m_uIoWorker		= 0;
	memset( m_lpAddr, 0, sizeof(char)*16 );
	m_pSockThreaded		= NULL;
#ifdef __CRC
	m_dwReadHeaderSize	= 0;
	m_dwDataSizeOffset	= 0;
#endif	// __CRC
#ifdef __PROTOCOL0910
	m_hProtocolId	= (HANDLE)0;
#endif	// __PROTOCOL0910
}

CDPSock::~CDPSock()
{
	CLOSE_HANDLE( m_hRecv );
	CLOSE_WSAEVENT( m_hClose );
	Close();
}

BOOL CDPSock::Close( void )
{
	CloseIoWorker();
#ifdef __PROTOCOL0910
	CLOSE_HANDLE( m_hProtocolId );
#endif	// __PROTOCOL0910
	if( m_pSock )
	{
		m_pSock->Close();
		SAFE_DELETE( m_pSock );
	}
	m_lspRecvBuffer.Clear( TRUE);	// delete memory
	return TRUE;
}

BOOL CDPSock::InitializeConnection( LPVOID lpConnection, DWORD dwFlags )
{
	if( lpConnection )
		strcpy( m_lpAddr, (char*)lpConnection );
	m_uPort		= (u_short)dwFlags;
	return TRUE;
}

BOOL CDPSock::CreateIoWorker( u_long uIoWorker )
{
	HANDLE hThread;
	
	m_uIoWorker	= uIoWorker;
	m_phCompletionPort	= new HANDLE[uIoWorker];
	for( DWORD iIoWorker = 0; iIoWorker < uIoWorker; iIoWorker++ )
	{
		if( ( m_phCompletionPort[iIoWorker] = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 ) ) == (HANDLE)NULL )
			return FALSE;
	}

	DWORD dwThreadId;
	INIT_LOOP;
	for( DWORD i = 0; i < m_uIoWorker; i++ )
	{
		VERIFY_LOOP( __FILE__, __LINE__ );
		hThread		= chBEGINTHREADEX( NULL, 0, IoWorkerThread, this, 0, &dwThreadId );
		ASSERT( hThread );
		SetThreadPriority( hThread, THREAD_PRIORITY_ABOVE_NORMAL );
		m_listthread.push_back( hThread );
	}
	return TRUE;
}

BOOL CDPSock::CloseIoWorker( void )
{
	u_long cbThread;
	LPHANDLE lphThread;
	
	m_locklistthread.Enter();
	cbThread	= m_listthread.size();
	if( cbThread > 0 )
	{
		lphThread	= new HANDLE[cbThread];

		INIT_LOOP;
		for( DWORD i = 0; i < cbThread; i++ )
		{
			VERIFY_LOOP( __FILE__, __LINE__ );

			lphThread[i]	= m_listthread.front();
			m_listthread.pop_front();
			if( !chWindows9x() )
			{
				PostQueuedCompletionStatus( m_phCompletionPort[i], CLOSEIOWORKERMSG, TRUE, NULL );
			}
		}
		if( chWindows9x() )
			WSASetEvent( m_hClose );
		WaitForMultipleObjects( cbThread, lphThread, TRUE, INFINITE );

		for( DWORD i = 0; i < cbThread; i++ ) {
			CloseHandle( (HANDLE)lphThread[i] );
		}
		SAFE_DELETE_ARRAY( lphThread );
	}
	if( m_phCompletionPort )
	{
		for( DWORD iIoWorker = 0; iIoWorker < m_uIoWorker; iIoWorker++ )
			CLOSE_HANDLE( m_phCompletionPort[iIoWorker] );
		SAFE_DELETE_ARRAY( m_phCompletionPort );
	}
	m_uIoWorker	= 0;
	m_locklistthread.Leave();
	return TRUE;
}

BOOL CDPSock::CreateEventWorker( CSock* pSock )
{
	HANDLE hThread;
	m_uIoWorker++;
	m_pSockThreaded	= pSock;
	CBuffer* pBuffer = ( (CClientSockE*)pSock )->CreateBuffer();
	AddCreatePlayerOrGroupMsg( (DPID)pSock->GetHandle(), pBuffer );
//	hThread		= (HANDLE)_beginthread( EventWorkerThread, 0, this );
	DWORD dwThreadId;
	hThread		= chBEGINTHREADEX( NULL, 0, EventWorkerThread, this, 0, &dwThreadId );
	ASSERT( hThread );
	SetThreadPriority( hThread, THREAD_PRIORITY_NORMAL );
	( (CClientSockE*)pSock )->m_hWorker	= hThread;
	m_locklistthread.Enter();
	m_listthread.push_back( hThread );
	m_locklistthread.Leave();
	return TRUE;
}

void CDPSock::RemoveThread( HANDLE hThread )
{
	HANDLE h;
	m_locklistthread.Enter();

	if( m_listthread.size() > 0 )
	{
		INIT_LOOP;
		for( list<HANDLE>::iterator i = m_listthread.begin(); i != m_listthread.end(); ++i )
		{
			VERIFY_LOOP( __FILE__, __LINE__ );
			h	= *i;
			if( h == hThread )
			{
				m_listthread.erase( i );
				m_uIoWorker--;
				break;
			}
		}
	}
	m_locklistthread.Leave();
}

#ifdef __CRC
BOOL CDPSock::CreateServer( DWORD dwcrc )
#else	// __CRC
BOOL CDPSock::CreateServer( BUFFER_TYPE nBufferType )
#endif	// __CRC
{
	Close();

#ifdef __CRC
	if( CSock::crcRead & dwcrc )
	{
		m_dwReadHeaderSize	= HEADERSIZE13;
#ifdef __PACKET_REPLAY_R
		m_dwDataSizeOffset	= sizeof(char) + sizeof(int) + sizeof(DWORD);
#else	// __PACKET_REPLAY_R
		m_dwDataSizeOffset	= sizeof(char) + sizeof(DWORD);
#endif	// __PACKET_REPLAY_R
	}
	else
	{
		m_dwReadHeaderSize	= HEADERSIZE5;
		m_dwDataSizeOffset	= sizeof(char);
	}

	CServerSock* pSock	= new CServerSock( dwcrc );
#else	//__CRC
	CServerSock* pSock	= new CServerSock( nBufferType );
#endif	// __CRC
	if( !pSock->Create( m_uPort ) )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	CSystemInfo si;

	CreateIoWorker( si.dwNumberOfProcessors*2 );

	pSock->StartServer( m_phCompletionPort, m_uIoWorker );

	if( !pSock->Listen() )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	pSock->SetID( DPID_SERVERPLAYER );
	m_pSock		= pSock;
	m_fServer	= TRUE;
	return TRUE;
}

#ifdef __CRC
BOOL CDPSock::CreateServerE( DWORD dwcrc )
#else	// __CRC
BOOL CDPSock::CreateServerE( BUFFER_TYPE nBufferType )
#endif	// __CRC
{
	Close();

#ifdef __CRC
	if( CSock::crcRead & dwcrc )
	{
		m_dwReadHeaderSize	= HEADERSIZE13;
		m_dwDataSizeOffset	= sizeof(char) + sizeof(DWORD);	
	}
	else
	{
		m_dwReadHeaderSize	= HEADERSIZE5;
		m_dwDataSizeOffset	= sizeof(char);
	}

	CServerSockE* pSock		= new CServerSockE( dwcrc );
#else	// __CRC
	CServerSockE* pSock		= new CServerSockE( nBufferType );
#endif	// __CRC
	if( !pSock->Create( m_uPort ) )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	pSock->StartServer( this );
	if( !pSock->Listen() )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	pSock->SetID( DPID_SERVERPLAYER );
	m_pSock		= pSock;
	m_fServer	= TRUE;

	return TRUE;
}

#ifdef __CRC
BOOL CDPSock::JoinToServer( DWORD dwcrc, u_long uWaitingTime )
#else	// __CRC
BOOL CDPSock::JoinToServer( BUFFER_TYPE nBufferType )
#endif	// __CRC
{
	Close();

	CClientSock* pSock;
#ifdef __CRC
	if( CSock::crcRead & dwcrc )
	{
		m_dwReadHeaderSize	= HEADERSIZE13;
		m_dwDataSizeOffset	= sizeof(char) + sizeof(DWORD);
	}
	else
	{
		m_dwReadHeaderSize	= HEADERSIZE5;
		m_dwDataSizeOffset	= sizeof(char);
	}

	pSock	= new CClientSock( dwcrc );
#else	// __CRC
	pSock	= new CClientSock( nBufferType );
#endif	// __CRC
	if( !pSock->Create() )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	m_fServer	= FALSE;	
	m_pSock		= pSock;
	pSock->SetID( pSock->GetHandle() );
	pSock->SetPeerID( DPID_SERVERPLAYER );

	if( !pSock->Connect( m_lpAddr, m_uPort ) )
		return FALSE;

	CreateIoWorker( (u_long)0x01 );

	int iIoWorker	= ( pSock->GetHandle() / 4 ) % GetIoWorkerCount();
	if( !CreateIoCompletionPort( (HANDLE)pSock->GetHandle(), m_phCompletionPort[iIoWorker], (DWORD)( pSock->GetHandle() ),  0 ) )
	{
		TRACE( "Can't create completion port with error %d\n", WSAGetLastError() );
		SAFE_DELETE( pSock );
		return FALSE;
	}

#ifdef __PROTOCOL0910
	m_hProtocolId	= CreateEvent( NULL, FALSE, FALSE, NULL );
#endif	// __PROTOCOL0910

	int zero	= 0;
	setsockopt( pSock->GetHandle(), SOL_SOCKET, SO_SNDBUF, (char *)&zero, sizeof(zero) );
	setsockopt( pSock->GetHandle(), SOL_SOCKET, SO_RCVBUF, (char *)&zero, sizeof(zero) );

	PostQueuedCompletionStatus( m_phCompletionPort[iIoWorker], NEWSOCKETMSG, (DWORD)pSock->GetHandle(), NULL );

#ifdef __PROTOCOL0910
	if( CSock::crcWrite & dwcrc )
	{
		if( WaitForSingleObject( m_hProtocolId, uWaitingTime ) == WAIT_TIMEOUT )
		{
			TRACE( "WAIT_TIMEOUT\n" );
			PostQueuedCompletionStatus( m_phCompletionPort[iIoWorker], (DWORD)-4, (DWORD)pSock->GetHandle(), NULL );
			return FALSE;
		}
	}
#endif	// __PROTOCOL0910

	return TRUE;
}

#ifdef __CRC
BOOL CDPSock::JoinToServerE( DWORD dwcrc, u_long uWaitingTime )
#else	// __CRC
BOOL CDPSock::JoinToServerE( BUFFER_TYPE nBufferType )
#endif	// __CRC
{
	Close();

#ifdef __CRC
	if( CSock::crcRead & dwcrc )
	{
		m_dwReadHeaderSize	= HEADERSIZE13;
		m_dwDataSizeOffset	= sizeof(char) + sizeof(DWORD);
	}
	else
	{
		m_dwReadHeaderSize	= HEADERSIZE5;
		m_dwDataSizeOffset	= sizeof(char);
	}

	CClientSockE* pSock	= new CClientSockE( dwcrc );
#else	// __CRC
	CClientSockE* pSock	= new CClientSockE( nBufferType );
#endif	// __CRC
	if( !pSock->Create() )
	{
		SAFE_DELETE( pSock );
		return FALSE;
	}
	m_fServer	= FALSE;	
	m_pSock		= pSock;
	pSock->SetID( pSock->GetHandle() );
	pSock->SetPeerID( DPID_SERVERPLAYER );


	if( !pSock->Connect( m_lpAddr, m_uPort ) )
		return FALSE;

	CreateEventWorker( (CClientSockE*)pSock );

#ifdef __PROTOCOL0910
	m_hProtocolId	= CreateEvent( NULL, FALSE, FALSE, NULL );
#endif	// __PROTOCOL0910

	int zero	= 0;
	setsockopt( pSock->GetHandle(), SOL_SOCKET, SO_SNDBUF, (char*)&zero, sizeof(zero) );
	setsockopt( pSock->GetHandle(), SOL_SOCKET, SO_RCVBUF, (char*)&zero, sizeof(zero) );
	
	InterlockedIncrement( &pSock->m_l );
	if( pSock->Recv() != 0 )
	{
		TRACE( "I/0 error %d\n", WSAGetLastError() );
//		if( pSock->m_l == 0 ) {
			RemoveThread( pSock->m_hWorker );
			CloseConnection( pSock->GetID() );
			return FALSE;
//		}
	}


#ifdef __PROTOCOL0910
	if( CSock::crcWrite & dwcrc )
	{
		if( WaitForSingleObject( m_hProtocolId, uWaitingTime ) == WAIT_TIMEOUT )
		{
			TRACE( "WAIT_TIMEOUT\n" );
			RemoveThread( pSock->m_hWorker );
			CloseConnection( pSock->GetID() );
			return FALSE;
		}
	}
#endif	// __PROTOCOL0910

	return TRUE;
}

BOOL CDPSock::Send( char* lpData, DWORD dwDataSize, DPID dpidTo )
{
	if( m_pSock )
	{
		m_pSock->Send( lpData, dwDataSize, dpidTo );
		return TRUE;
	}
	return FALSE;
}

CBuffer* CDPSock::Receive( )
{
	CMclAutoLock	Lock( m_lspRecvBuffer.m_cs );

	CBuffer* pBuffer = m_lspRecvBuffer.GetHead();
	if( pBuffer )
	{
		m_lspRecvBuffer.RemoveHead();
		return pBuffer;
	}
	else
		return NULL;
}

void CDPSock::CloseConnection( DPID dpid )
{
	if( m_pSock )
	{
		if( m_pSock->CloseConnection( dpid ) )
		{
			CBuffer* pBuffer = CBufferFactory::GetInstance().CreateBuffer( BUFFER_TYPE_5BYTE );
			AddDestroyPlayerOrGroupMsg( dpid, pBuffer );
		}
		if( !m_fServer )
			SAFE_DELETE( m_pSock );
	}
}

BOOL CDPSock::Shutdown( DPID dpid )
{
	return ( m_pSock? m_pSock->Shutdown( dpid ): FALSE );
}

#ifdef __INFO_SOCKLIB0516
DWORD CDPSock::GetDebugInfo( DPID dpid )
{
	DWORD dwValue = 0;
	if( m_pSock )
	{
		if( dpid == DPID_UNKNOWN )
			dpid = m_pSock->GetID();
		dwValue = m_pSock->GetDebugInfo( dpid );
	}
	return dwValue;
}
#endif	// __INFO_SOCKLIB0516

void CDPSock::AddCreatePlayerOrGroupMsg( DPID dpid, CBuffer* pBuffer )
{
	DPMSG_CREATEPLAYERORGROUP createPlayer;
	
	ZeroMemory( (void*)&createPlayer, sizeof(DPMSG_CREATEPLAYERORGROUP) );
	createPlayer.dwType	= DPSYS_CREATEPLAYERORGROUP;	
	createPlayer.dwPlayerType	= DPPLAYERTYPE_PLAYER;	
	createPlayer.dpId	= dpid;	
	createPlayer.dpIdParent	= DPID_SERVERPLAYER;
	
	pBuffer->dpid	= DPID_SYSMSG;
	pBuffer->cb++;

#ifdef __CRC
	*pBuffer->m_pTail	= SYSHEADERMARK;	
	pBuffer->m_pTail++;

#ifdef __PACKET_REPLAY_R
	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(int) + sizeof(DWORD);	// + 4 + 4
#else	// __PACKET_REPLAY_R
	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(DWORD);	// + 4
#endif	// __PACKET_REPLAY_R

	*(UNALIGNED DWORD*)pBuffer->m_pTail	= sizeof(DPMSG_CREATEPLAYERORGROUP);
	pBuffer->m_pTail	+= sizeof(DWORD);

	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(DWORD);	// + 4
#else	
	pBuffer->SetHeader( sizeof(DPMSG_CREATEPLAYERORGROUP) );
#endif	// __CRC

	memcpy( pBuffer->m_pTail, (LPBYTE)&createPlayer, sizeof(DPMSG_CREATEPLAYERORGROUP) );
	pBuffer->m_pTail	+= sizeof(DPMSG_CREATEPLAYERORGROUP);	
	m_lspRecvBuffer.AddTail( pBuffer );

	SetEvent( m_hRecv );
}

void CDPSock::AddDestroyPlayerOrGroupMsg( DPID dpid, CBuffer* pBuffer )
{
	DPMSG_DESTROYPLAYERORGROUP destroyPlayer;

	ZeroMemory( (void*)&destroyPlayer, sizeof(DPMSG_DESTROYPLAYERORGROUP) );
	destroyPlayer.dwType	= DPSYS_DESTROYPLAYERORGROUP;
	destroyPlayer.dwPlayerType	= DPPLAYERTYPE_PLAYER;	
	destroyPlayer.dpId	= dpid;		
	destroyPlayer.dpIdParent	= DPID_SERVERPLAYER;	
#ifdef __INFO_SOCKLIB0516
	destroyPlayer.dwFlags = GetDebugInfo( dpid );
#endif
	pBuffer->dpid	= DPID_SYSMSG;
	pBuffer->cb++;

#ifdef __CRC
	*pBuffer->m_pTail	= SYSHEADERMARK;	
	pBuffer->m_pTail++;

#ifdef __PACKET_REPLAY_R
	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(int) + sizeof(DWORD);	// + 4
#else	// __PACKET_REPLAY_R
	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(DWORD);	// + 4
#endif	// __PACKET_REPLAY_R

	*(UNALIGNED DWORD*)pBuffer->m_pTail	= sizeof(DPMSG_DESTROYPLAYERORGROUP);
	pBuffer->m_pTail	+= sizeof(DWORD);

	if( m_dwDataSizeOffset != sizeof(char) )
		pBuffer->m_pTail	+= sizeof(DWORD);	// + 4
#else
	pBuffer->SetHeader( sizeof(DPMSG_DESTROYPLAYERORGROUP) );
#endif	// __CRC

	memcpy( pBuffer->m_pTail, (LPBYTE)&destroyPlayer, sizeof(DPMSG_DESTROYPLAYERORGROUP) );
	pBuffer->m_pTail	+= sizeof(DPMSG_DESTROYPLAYERORGROUP);	
	m_lspRecvBuffer.AddTail( pBuffer );

	SetEvent( m_hRecv );
}

#ifdef __PROTOCOL0910
void CDPSock::SetProtocolId( DWORD dwProtocolId )
{
	if( m_pSock )
	{
		m_pSock->SetProtocolId( dwProtocolId );
		SetEvent( m_hProtocolId );
	}
}
#endif	// __PROTOCOL0910

u_int	__stdcall IoWorkerThread( LPVOID lpvThreadParameter )
{
	CDPSock* pDPSock	= (CDPSock*)lpvThreadParameter;
	SOCKET hSocket;
	DWORD dwBytes;
	CClientSock* pClientSock;
	LPOVERLAPPED lpov;
	BOOL fOk;
	long lIoWorker	= InterlockedIncrement( &pDPSock->m_lActiveIoWorker );
	while( 1 )
	{
		fOk		= GetQueuedCompletionStatus( pDPSock->GetCompletionPort( lIoWorker - 1 ), &dwBytes, (LPDWORD)&hSocket, &lpov, INFINITE );
		if( dwBytes == CLOSEIOWORKERMSG )
		{
			InterlockedDecrement( &pDPSock->m_lActiveIoWorker );
			return( 0 );
		}

		if( dwBytes == NEWSOCKETMSG )
		{
			DWORD dwAddr	= (DWORD)lpov;
			CServerSock* pServerSock	= (CServerSock*)pDPSock->GetServerSock();
			if( pServerSock )
				pServerSock->AllocAndAssoc( hSocket, dwAddr );
			pClientSock		= pDPSock->Get( hSocket );
#ifdef __VERIFYNETLIB
			if( pClientSock == NULL )
			{
				WriteError( "NEWSOCKETMSG" );
				continue;
			}
#endif	// __VERIFYNETLIB
			CBuffer* pBuffer = pClientSock->CreateBuffer();
			pDPSock->AddCreatePlayerOrGroupMsg( hSocket, pBuffer );

			pClientSock->m_AccessLock.Enter();	// lock
			pClientSock->m_nReference++;
			if( pClientSock->Recv() != 0 )
			{
				pClientSock->m_bDoNotWrite	= TRUE;
				int nReference	= --pClientSock->m_nReference;
//				TRACE( "I/o error %d // REF:%d\n", WSAGetLastError(), lRef );
#ifdef __INFO_SOCKLIB0516
				pClientSock->m_dwDbgInfo	= WSAGetLastError(); // * 1000 + 1 * 100 + nReference * 10;
#endif	// __INFO_SOCKLIB0516
				pClientSock->m_AccessLock.Leave();	// unlock
				if( nReference == 0 )
					pDPSock->CloseConnection( hSocket );
			}
			else
			{
				pClientSock->m_AccessLock.Leave();	// unlock
			}
			continue;
		}
#ifdef __PROTOCOL0910
		else if( dwBytes == NEWSOCKETMSG2 )
		{
			DWORD dwAddr	= (DWORD)lpov;
			CServerSock* pServerSock	= (CServerSock*)pDPSock->GetServerSock();
			if( pServerSock )
				pServerSock->AllocAndAssoc( hSocket, dwAddr );
			pClientSock		= pDPSock->Get( hSocket );
#ifdef __VERIFYNETLIB
			if( pClientSock == NULL )
			{
				WriteError( "NEWSOCKETMSG2" );
				continue;
			}
#endif	// __VERIFYNETLIB
			CBuffer* pBuffer = pClientSock->CreateBuffer();
			pDPSock->AddCreatePlayerOrGroupMsg( hSocket, pBuffer );

			pClientSock->m_AccessLock.Enter();	// lock
			pClientSock->m_nReference++;
			if( pClientSock->Recv() != 0 )
			{
				pClientSock->m_bDoNotWrite	= TRUE;
				int nReference	= --pClientSock->m_nReference;
//				TRACE( "I/o error %d // REF:%d\n", WSAGetLastError(), lRef );
#ifdef __INFO_SOCKLIB0516
				pClientSock->m_dwDbgInfo	= WSAGetLastError(); // * 1000 + 2 * 100 + nReference * 10;
#endif	// __INFO_SOCKLIB0516
				pClientSock->m_AccessLock.Leave();	// unlock
				if( nReference == 0 )
					pDPSock->CloseConnection( hSocket );
			}
			else
			{
				pClientSock->m_AccessLock.Leave();	// unlock
				pClientSock->SendProtocolId();
			}
			continue;
		}
		else if( dwBytes == INVALIDSOCKMSG )
		{
			pDPSock->CloseConnection( hSocket );
			continue;
		}
#endif	// __PROTOCOL0910

//		TRACE( "GetQueuedCompletionStatus - fOK:%d, dwBytes:%d\n", fOk, dwBytes );
		
		if( !( pClientSock = pDPSock->Get( hSocket ) ) )
			continue;

		if( !fOk || dwBytes == 0 )	// When Connection Closed by peer gracefully or becoz of error
		{

			int err		=  GetLastError();
			//TRACE( "GetLastError():%d\n", err );

			switch( err )
			{
				case ERROR_NETNAME_DELETED:		//64
				case ERROR_OPERATION_ABORTED:
				case 0:
				case WSAENOTSOCK:	//10038
				default:
					{
//						if( FALSE == fOk && NULL == lpov )	{}
						pClientSock->m_AccessLock.Enter();
						pClientSock->m_bDoNotWrite	= TRUE;
						int nReference	= --pClientSock->m_nReference;
#ifdef __INFO_SOCKLIB0516
						pClientSock->m_dwDbgInfo	= err; // * 1000 + 3 * 100 + nReference * 10;
#endif	// __INFO_SOCKLIB0516
						pClientSock->m_AccessLock.Leave();
						if( nReference == 0 )
							pDPSock->CloseConnection( hSocket );
						break;
					}
			}
			continue;
		}

		if( &pClientSock->m_ovRecv == lpov )	// receive i/o completed
		{
#ifdef __TRAFIC_1218
/*
			if( pClientSock->IsCrcRead() )	// public
				pClientSock->AddTraficLog( dwBytes, CClientSock::read );
*/
#endif	// __TRAFIC_1218
			CBuffer* pBuffer	= pClientSock->Fetch( dwBytes );
			if( pBuffer )
			{
				if( pBuffer->cb > 0 ) {
#ifdef __PROTOCOL0910
					if( pClientSock->IsCrcWrite()
						&& pClientSock->m_dwProtocolId == 0 )
					{
						LPBYTE ptr	= pBuffer->m_lpBufStart;
						if( *(UNALIGNED LPDWORD)( ptr + pDPSock->m_dwDataSizeOffset ) == 8 )
						{
							if( *(UNALIGNED LPDWORD)( &ptr[pDPSock->m_dwReadHeaderSize] )
								== 0x00000000 )
								pDPSock->SetProtocolId( *(UNALIGNED LPDWORD)( &ptr[pDPSock->m_dwReadHeaderSize + 4] ) );
						}
					}
#endif	// __PROTOCOL0910
					pBuffer->dpid	= hSocket;
					pDPSock->m_lspRecvBuffer.AddTail( pBuffer );
					SetEvent( pDPSock->GetRecvHandle() );
				}
				else {
					SAFE_DELETE( pBuffer );
				}
			}
			else if( WSAGetLastError()
				==  ERROR_BAD_NET_NAME )
			{
				pDPSock->CloseConnection( hSocket );
				continue;
			}


			if( pClientSock->Recv() != 0 )	// i/o error
			{
				pClientSock->m_AccessLock.Enter();
				pClientSock->m_bDoNotWrite	= TRUE;
				int nReference	= --pClientSock->m_nReference;
//				TRACE( "I/o error %d, %d // REF:%d\n", pClientSock->GetHandle(), WSAGetLastError(), lRef );
#ifdef __INFO_SOCKLIB0516
				pClientSock->m_dwDbgInfo	= WSAGetLastError(); // * 1000 + 4 * 100 + nReference * 10;
#endif	// __INFO_SOCKLIB0516
				pClientSock->m_AccessLock.Leave();
				if( nReference == 0 )
					pDPSock->CloseConnection( hSocket );
			}
		}
		else if( &pClientSock->m_ovSend == lpov )	// send i/o completed
		{
#ifdef __TRAFIC_1218
/*
			if( pClientSock->IsCrcRead() )	// public
				pClientSock->AddTraficLog( dwBytes, CClientSock::write );
*/
#endif	// __TRAFIC_1218
			pClientSock->m_AccessLock.Enter();	// lock
			if( pClientSock->SendRemnant( dwBytes ) != 0 )
			{
				pClientSock->m_bDoNotWrite	= TRUE;
				int nReference	= --pClientSock->m_nReference;
#ifdef __INFO_SOCKLIB0516
				pClientSock->m_dwDbgInfo	= WSAGetLastError(); // * 1000 + 5 * 100 + nReference * 10;
#endif	// __INFO_SOCKLIB0516
				pClientSock->m_AccessLock.Leave();	// unlock
				if( nReference == 0 )
					pDPSock->CloseConnection( hSocket );
			}
			else
			{
				pClientSock->m_AccessLock.Leave();	// unlock
			}
		}
//		Sleep( 0 );
	}
	return( 0 );
}

//void	__cdecl	EventWorkerThread( LPVOID lpvThreadParameter )
u_int	__stdcall	EventWorkerThread( LPVOID lpvThreadParameter )
{
	CDPSock* pDPSock	= (CDPSock*)lpvThreadParameter;
	CClientSockE* pClientSock	= (CClientSockE*)pDPSock->GetSockThreaded();
	SOCKET hSocket	= pClientSock->GetHandle();

	WSAEVENT aEvent[4];
	aEvent[0]	= pClientSock->GetRecvEvent();
	aEvent[1]	= pClientSock->GetSendEvent();
	aEvent[2]	= pDPSock->GetCloseHandle();
	aEvent[3]	= NULL;

	DWORD dwBytes	= 0, dw;
	BOOL fOk;

	while( 1 )
	{
		dw	= WSAWaitForMultipleEvents( 3, aEvent, FALSE, INFINITE, FALSE );
		switch( dw )
		{
			case WSA_WAIT_EVENT_0+2:
				return 0;
			case WSA_WAIT_FAILED:
				TRACE( "WSA_WAIT_FAILED with error %d\n", WSAGetLastError() );
				continue;
				break;
			case WSA_WAIT_EVENT_0:
				{
					fOk		= GetOverlappedResult( (HANDLE)hSocket, &pClientSock->m_ovRecv, &dwBytes, TRUE );
					if( !fOk || dwBytes == 0 )	// When Connection Closed by peer gracefully or because of error
					{
						int err		= WSAGetLastError();
						switch( err )
						{
							case ERROR_NETNAME_DELETED:
							case ERROR_OPERATION_ABORTED:
							case 0:
							case WSAENOTSOCK:
							default:
								{
									TRACE( "Close socket %d, %x //REF:%d\n", hSocket, pClientSock, pClientSock->m_l-1 );
									InterlockedDecrement( &pClientSock->m_l );
//									if( pClientSock->m_l == 0 ) {
//										pDPSock->RemoveThread( pClientSock->m_hWorker );
										pDPSock->CloseConnection( hSocket );
										return 0;
//									}
									break;
								}
						}
					}

					CBuffer* pBuffer	= pClientSock->Fetch( dwBytes );
					if( pBuffer )
					{
						if( pBuffer->cb > 0 ) {
#ifdef __PROTOCOL0910
							if( pClientSock->IsCrcWrite()
								&& pClientSock->m_dwProtocolId == 0 )
							{
								LPBYTE ptr	= pBuffer->m_lpBufStart;
								if( *(UNALIGNED LPDWORD)( ptr + pDPSock->m_dwDataSizeOffset ) == 8 )
								{
									if( *(UNALIGNED LPDWORD)( &ptr[pDPSock->m_dwReadHeaderSize] )
										== 0x00000000 )
										pDPSock->SetProtocolId( *(UNALIGNED LPDWORD)( &ptr[pDPSock->m_dwReadHeaderSize + 4] ) );
								}
							}
#endif	// __PROTOCOL0910
							pBuffer->dpid	= hSocket;
							pDPSock->m_lspRecvBuffer.AddTail( pBuffer );
							SetEvent( pDPSock->GetRecvHandle() );
						}
						else {
							SAFE_DELETE( pBuffer );
						}
					}
					else if( WSAGetLastError()
						==  ERROR_BAD_NET_NAME )
					{
						pDPSock->CloseConnection( hSocket );
						return 0;
					}

					if( pClientSock->Recv() != 0 )	// i/o error
					{
//						if( pClientSock->m_l == 0 ) {
							pDPSock->RemoveThread( pClientSock->m_hWorker );
							pDPSock->CloseConnection( hSocket );
							return 0;
//						}
					}
					break;
				}
			case WSA_WAIT_EVENT_0+1:	
				{
					fOk		= GetOverlappedResult( (HANDLE)hSocket, &pClientSock->m_ovSend, &dwBytes, TRUE );
					WSAResetEvent( aEvent[1] );
					if( !fOk || dwBytes == 0 )	// When Connection Closed by peer gracefully or because of error
					{
						int err		= WSAGetLastError();
						switch( err )
						{
							case ERROR_NETNAME_DELETED:
							case ERROR_OPERATION_ABORTED:
							case 0:
							case WSAENOTSOCK:
							default:
								{
									TRACE( "Close socket %d, %x //REF:%d\n", hSocket, pClientSock, pClientSock->m_l-1 );
									InterlockedDecrement( &pClientSock->m_l );
//									if( pClientSock->m_l == 0 ) {
										pDPSock->RemoveThread( pClientSock->m_hWorker );
										pDPSock->CloseConnection( hSocket );
										return 0;
//									}
									break;
								}
						}
					}
				
					if( pClientSock->SendRemnant( dwBytes ) != 0 )
					{
//						if( pClientSock->m_l == 0 ) {
							pDPSock->RemoveThread( pClientSock->m_hWorker );
							pDPSock->CloseConnection( hSocket );
							return 0;
//						}
					}
					break;
				}
		}
	}
	return 0;
}
