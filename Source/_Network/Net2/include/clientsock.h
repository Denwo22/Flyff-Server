#ifndef __CLIENTSOCK_H__
#define __CLIENTSOCK_H__

#pragma once

#include "sock.h"
#include "buffer.h"

#include "crc.h"
//using	namespace	CryptoPP;
//using	namespace	std;

template <class T>
class CClientSock : public CSock
{
public:
	T*	m_pRecvBuffer;
	WSAOVERLAPPED	m_ovRecv;
	WSAOVERLAPPED	m_ovSend;
	LONG	m_l;
private:
	CBufferQueue<T>		m_lspSendBuffer;
	WSABUF	m_lpBuffers[MAX_WSABUF];
	CRC32	m_crcRead;
	CRC32	m_crcWrite;
	DWORD	m_dwReadHeaderSize;
public:
//	Constructions
	CClientSock( DWORD dwcrc );
	virtual	~CClientSock();
//	Operations
	BOOL	Connect( char* lpAddr, u_short uPort );
	int		Recv( void );
	int		SendRemnant( DWORD dwBytesSent );
	T*	Fetch( DWORD dwBytes );
	
	virtual	void	Close( void );
	virtual	BOOL	CloseConnection( SOCKET hSocket );
	virtual	BOOL	Shutdown( SOCKET hSocket );
	virtual	void	Send( char* lpData, DWORD dwDataSize, DPID dpidTo );
	virtual	HRESULT		GetPeerAddr( DPID dpid, LPVOID lpAddr, LPDWORD lpdwSize );
};

template <class T>
CClientSock<T>::CClientSock( DWORD dwcrc )
	{
		m_dwcrc		= dwcrc;
		m_l	= 0;
		m_pRecvBuffer	= new T;

		if( IsCrcWrite() )
			m_lspSendBuffer.crc( &m_crcWrite );
	}

template <class T>
CClientSock<T>::~CClientSock()
	{
		Close();
	}

template <class T>
void CClientSock<T>::Close( void )
	{
		CLOSE_SOCKET( m_hSocket );
		SAFE_DELETE( m_pRecvBuffer );
	}

template <class T>
BOOL CClientSock<T>::CloseConnection( SOCKET hSocket )
	{
		ASSERT( hSocket == m_hSocket );
		Close();
		return( TRUE );
	}

template <class T>
BOOL CClientSock<T>::Shutdown( SOCKET hSocket )
	{	return FALSE;	}

template <class T>
BOOL CClientSock<T>::Connect( char* lpAddr, u_short uPort )
	{
		int	err;
		SOCKADDR_IN sin;

		if( m_hSocket == INVALID_SOCKET )
		{
			TRACE( "Creation needed\n" );
			return FALSE;
		}
		sin.sin_family	= PF_INET;
		sin.sin_addr.s_addr		= inet_addr( lpAddr );

		if( sin.sin_addr.s_addr == INADDR_NONE )
		{
			LPHOSTENT lphost;
			lphost	= gethostbyname( lpAddr );
			if( lphost )
				sin.sin_addr.s_addr		= ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				WSASetLastError( WSAEINVAL );
				return FALSE;
			}
		}

		sin.sin_port = htons( uPort );

		if( connect( m_hSocket, (LPSOCKADDR) &sin, sizeof(sin) ) == SOCKET_ERROR )
		{
			if( ( err = WSAGetLastError() ) == WSAEWOULDBLOCK )
				return TRUE;
			TRACE( "Can't connect with error %d\n", err );
			return FALSE;
		}
		return TRUE;
	}

template <class T>
void CClientSock<T>::Send( char* lpData, DWORD dwDataSize, DPID dpidTo )
	{
	//	ASSERT( m_hSocket != INVALID_SOCKET );
		if( m_hSocket == INVALID_SOCKET )
			return;

		DWORD dwBufferCount, dwBytes;
		CMclAutoLock	Lock( m_lspSendBuffer.m_cs );
		if( m_lspSendBuffer.IsEmpty() )
		{
			m_lspSendBuffer.AddData( (LPBYTE)lpData, dwDataSize );
			m_lspSendBuffer.GetData( m_lpBuffers, &dwBufferCount );

			memset( (void*)&m_ovSend, 0, sizeof(WSAOVERLAPPED) );
			int err;
			InterlockedIncrement( &m_l );
			if( WSASend( GetHandle(), m_lpBuffers, dwBufferCount, &dwBytes, 0, &m_ovSend, NULL ) != 0 && ( err = WSAGetLastError() ) != WSA_IO_PENDING )
			{
				InterlockedDecrement( &m_l );
	//			ASSERT( 0 );
			}
		}
		else
		{
			m_lspSendBuffer.AddData( (LPBYTE)lpData, dwDataSize );
		}
	}

template <class T>
int CClientSock<T>::Recv( void )
	{
		ASSERT( m_hSocket != INVALID_SOCKET );
		DWORD dwBytesRecvd	= 0, dwFlags	= 0;

		WSABUF buffer;
		buffer.buf	= (char*)m_pRecvBuffer->GetWritableBuffer( (int*)&buffer.len );

		memset( &m_ovRecv, 0, sizeof(WSAOVERLAPPED) );

		int err;
		if( WSARecv( GetHandle(), &buffer, (DWORD)1, &dwBytesRecvd, &dwFlags, &m_ovRecv, NULL ) != 0 && ( err = WSAGetLastError() ) != WSA_IO_PENDING )
		{
			TRACE( "I/o error, close socket %d, %x //REF:%d\n", GetHandle(), this, m_l-1 );
			InterlockedDecrement( &m_l );
			return err;
		}
		// TOM!
		//for(DWORD i = 0; i < buffer.len; i++)
		//	buffer.buf[i]--;
		// \TOM!
		return 0;
	}

template <class T>
int CClientSock<T>::SendRemnant( DWORD dwBytes )
	{
		ASSERT( m_hSocket != INVALID_SOCKET );
		DWORD dwBufferCount;
		
		CMclAutoLock	Lock( m_lspSendBuffer.m_cs );

		m_lspSendBuffer.RemoveData( dwBytes );
		if( m_lspSendBuffer.IsEmpty() )
		{
			InterlockedDecrement( &m_l );
			return 0;
		}
		m_lspSendBuffer.GetData( m_lpBuffers, &dwBufferCount );

		memset( (void*)&m_ovSend, 0, sizeof(WSAOVERLAPPED) );
		int err;
		if( WSASend( GetHandle(), m_lpBuffers, dwBufferCount, &dwBytes, 0, &m_ovSend, NULL ) != 0 && ( err = WSAGetLastError() ) != WSA_IO_PENDING )
		{
			TRACE( "i/O error, close socket %d, %x //REF:%d\n", GetHandle(), this, m_l-1 );
			InterlockedDecrement( &m_l );
			return err;
		}
		return 0;
	}

template <class T>
T* CClientSock<T>::Fetch( DWORD dwBytes )
	{
		return m_pRecvBuffer->Fetch( &m_pRecvBuffer, dwBytes, ( IsCrcRead()? &m_crcRead: NULL ) );
		/*
		m_pRecvBuffer->m_pTail	+=	dwBytes;
		ASSERT( m_pRecvBuffer->m_pTail <= m_pRecvBuffer->m_lpBufMax );
		
		int nRemnant;
		LPBYTE ptr	= m_pRecvBuffer->GetReadableBuffer( &nRemnant );

		CBuffer* pOld	= NULL;
		u_long uPacketSize;

		DWORD dwCrc, dwDataSize;
		byte digest[4];

		while( 1 )
		{
	#ifdef _DEBUG
			while( !CBuffer::IsHeader( *ptr ) && nRemnant > 0 )
			{
	//			ptr++;
	//			nRemnant--;
				WSASetLastError( ERROR_BAD_NET_NAME );
				return NULL;
	//			ASSERT( 0 );
			}
	#endif
			
			if( nRemnant < m_dwReadHeaderSize )
			{
				if( m_pRecvBuffer->cb > 0 ) {
					pOld	= m_pRecvBuffer;
					pOld->m_pTail	-= nRemnant;	// remove remnant from old buffer
					m_pRecvBuffer	= new CBuffer;
					ASSERT( m_pRecvBuffer->m_pTail + nRemnant <= m_pRecvBuffer->m_lpBufMax ); 
					memcpy( m_pRecvBuffer->m_pTail, ptr, nRemnant );
	//				if( nRemnant > 0 )
	//				{
	//					ASSERT( CBuffer::IsHeader( *ptr ) );
	//				}
					m_pRecvBuffer->m_pTail	+=	nRemnant;
				}
				return pOld;
			}
			else
			{
				if( m_dwReadHeaderSize == HEADERSIZE13 )
				{
					dwCrc	= *(UNALIGNED LPDWORD)( ptr + sizeof(char) );
					dwDataSize	= *(UNALIGNED LPDWORD)( ptr + sizeof(char) + sizeof(DWORD) );
					m_crcRead.Restart();
					m_crcRead.Update( (const byte*)( &dwDataSize ), sizeof(DWORD) );
					m_crcRead.Final( digest );
					if( *(UNALIGNED LPDWORD)digest != dwCrc )
					{
						WSASetLastError( ERROR_BAD_NET_NAME );
						return NULL;
					}
					dwCrc	= *(UNALIGNED LPDWORD)( ptr + sizeof(char) + sizeof(DWORD) + sizeof(DWORD) );
					uPacketSize		= m_dwReadHeaderSize + dwDataSize;
				}
				else
				{
					uPacketSize	= m_dwReadHeaderSize + *(UNALIGNED LPDWORD)&ptr[1];
				}
				if( nRemnant < uPacketSize )
				{
					if( uPacketSize > m_pRecvBuffer->GetSize() )
					{
						pOld	= m_pRecvBuffer;
						m_pRecvBuffer	= new CBuffer( uPacketSize );
					}
					else
					{
						if( m_pRecvBuffer->cb > 0 )
						{
							pOld	= m_pRecvBuffer;
							m_pRecvBuffer	= new CBuffer;
						}
					}

					if( pOld )
					{
						ASSERT( m_pRecvBuffer->m_pTail + nRemnant <= m_pRecvBuffer->m_lpBufMax );
						memcpy( m_pRecvBuffer->m_pTail, ptr, nRemnant );
	//					if( nRemnant > 0 )
	//					{
	//						ASSERT( CBuffer::IsHeader( *ptr ) );
	//					}
						m_pRecvBuffer->m_pTail	+=	nRemnant;
					}

					return pOld;
				}
				else	// completion
				{
					if( m_dwReadHeaderSize == HEADERSIZE13 )
					{
						m_crcRead.Restart();
						m_crcRead.Update( (const byte*)( ptr + m_dwReadHeaderSize ), dwDataSize );
						m_crcRead.Final( digest );
						if( *(UNALIGNED LPDWORD)digest != dwCrc )
						{
							WSASetLastError( ERROR_BAD_NET_NAME );
							return NULL;
						}
					}
					m_pRecvBuffer->cb++;
					nRemnant	-= ( uPacketSize );
					ptr		+= ( uPacketSize );
				}
			}
		}

		return NULL;
		*/
	}

template <class T>
HRESULT CClientSock<T>::GetPeerAddr( DPID dpidPlayer, LPVOID lpAddr, LPDWORD lpdwSize )
{
	ASSERT( lpAddr );

	SOCKADDR_IN sin;
	ZeroMemory( &sin, sizeof( SOCKADDR_IN) );

	int namelen	= sizeof(sin);
	if( getpeername( m_hSocket, (SOCKADDR *)&sin, &namelen ) == SOCKET_ERROR )
		return DPERR_GENERIC;

	sprintf( (char *)lpAddr, "%d.%d.%d.%d", sin.sin_addr.s_net
																	, sin.sin_addr.s_host
																	, sin.sin_addr.s_lh
																	, sin.sin_addr.s_impno );
	return DP_OK;
}

#endif //__CLIENTSOCK_H__