#include "stdafx.h"
#include "BillingMgrJP.h"
#include "dpBilling.h"
#include "account.h"
#include "dpsrvr.h"
#include "dpdbsrvr.h"

extern	CAccountMng	g_AccountMng;
extern	CDPSrvr		g_dpSrvr;
extern	CDPDBSrvr	g_dpDbSrvr;

///////////////////////////////////////////////////////////////////////
// CBillingMgrJP
///////////////////////////////////////////////////////////////////////
CBillingMgrJP::CBillingMgrJP()
{
	m_iBillingFreePass	= 0;
	m_pDPBillings		= NULL;
	m_nMaxConnect		= 0;
}

CBillingMgrJP::~CBillingMgrJP()
{
	for( int i=0; i<m_nMaxConnect; ++i )
	{
		m_pDPBillings[i].DeleteDPObject();
	}
	
	if( m_pDPBillings )
	{
		safe_delete_array( m_pDPBillings );
		m_pDPBillings = NULL;
	}
}

BOOL CBillingMgrJP::SetConfig( BILLING_ENUM id, DWORD data )
{
	switch( id )
	{
	case BID_FREEPASS:
		m_iBillingFreePass = (int)data;
		break;
	case BID_IP:
		if( data )
			m_strIPs.push_back( (const char*) data );
		break;
	case BID_PORT:
		m_nPorts.push_back( (u_short)data );
		break;
	}
	return TRUE;
}

// �ʱ�ȭ: ���� �������� ���� ��ü�� �Ҵ��� �д�.
bool CBillingMgrJP::Init( HWND hWnd )
{
	assert( m_pDPBillings == NULL );

	if( m_strIPs.size() == 0 )
	{
		MessageBox( hWnd, "AccountServer.ini - ip port is missing.", "Billing Setting ERROR", MB_OK );
		return false;
	}
	
	if( m_strIPs.size() != m_nPorts.size() )
	{
		MessageBox( hWnd, "AccountServer.ini - ip and port count is different", "Billing Setting ERROR", MB_OK );
		return false;
	}
	
	int nMaxConnect = m_strIPs.size();
	nMaxConnect = min( nMaxConnect, MAX_DPBILLING );	// 32�� �̻� ���� 

	m_nMaxConnect = nMaxConnect;
	m_pDPBillings = new CDPBilling[ nMaxConnect ];

	for( int i=0; i<m_nMaxConnect; ++i )
	{
		m_pDPBillings[i].Init( hWnd );
	}

	::PostMessage( hWnd, WM_CONNENT_BILLING, 0, 0 );
	::SetTimer( hWnd, IDT_KICKOUT, 1000 * 60, NULL );

	return true;
}

// �Ҵ�� �ڿ��� �ݳ��Ѵ�.
void CBillingMgrJP::Release()
{
	safe_delete( this );
}

// ���������� �˻��Ѵ�. (dwKey�� ������� �ʴ´�.)
BYTE CBillingMgrJP::CheckAccount( int nType, DWORD , const char* szAccount, const char* szAddr )
{
	if( nType != m_iBillingFreePass )  // ��������? 
	{ 
		// ���������� �ƴϸ� ���������� �˻� ��û
		if( RequestBillingInfo( szAccount, szAddr ) )
			return ACCOUNT_BILLING_WAIT_ACK;		// �����κ��� ACK�� ���� OnAfterChecking�� ȣ���Ѵ�.
		else
			return ACCOUNT_BILLING_DISCONNECTED;
	}
	else														
	{
		// ���������� TRACE
		char szTrace[1024];
		sprintf( szTrace, "FreePass - Account:%s fCheck:%d", szAccount, nType );
		OutputDebugString( szTrace );

		return ACCOUNT_CHECK_OK;
	}
}

// ���������� �˻� ��û
bool CBillingMgrJP::RequestBillingInfo( LPCTSTR lpszAccount, LPCTSTR lpAddr )
{
	if( m_nMaxConnect == 0 )
		return false;

	CDPBilling* billings[MAX_DPBILLING];

	int iConnected = 0;
	for( int i=0; i<m_nMaxConnect; ++i )
	{
		if( m_pDPBillings[i].m_bConnected )
			billings[iConnected++] = m_pDPBillings + i;
	}

	if( iConnected == 0 )
		return false;

	int iSelect = rand() % iConnected;
	billings[iSelect]->SendGameStart( lpszAccount, lpAddr );
	return ( iConnected > 0 );
}

// ������ ���õ� ������ �޼����� ó���ǰ� �Ѵ�.
BOOL CBillingMgrJP::PreTranslateMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	switch( message ) 
	{
		case WM_CONNENT_BILLING:
			if( Connect() )
				KillTimer( hWnd, IDT_RECONNECT );
			else
				SetTimer( hWnd, IDT_RECONNECT, 2000, NULL );
			return TRUE;

		case WM_TIMER:
			{
				switch( wParam )
				{
				case IDT_RECONNECT:
					::PostMessage( hWnd, WM_CONNENT_BILLING, 0, 0 );
					return TRUE;
				case IDT_KEEPALIVE:
					SendKeepAlive();
					return TRUE;
				case IDT_KICKOUT:
					g_AccountMng.KickOutCheck();
					return TRUE;
				}
			}
			break;
	}

	return FALSE;
}

// ���������� �����Ѵ�.
bool CBillingMgrJP::Connect()
{
	int iFailed = 0;
	for( int i=0; i<m_nMaxConnect; ++i )
	{
		if( FALSE == m_pDPBillings[i].m_bConnected )
		{
			if( FALSE == m_pDPBillings[i].ConnectToServer( m_strIPs[i].c_str(), m_nPorts[i], FALSE ) )  
				++iFailed;
		}			
	}

	return iFailed == 0;
}

// KEEP_ALIVE��ȣ�� ������.
void CBillingMgrJP::SendKeepAlive()
{
	for( int i=0; i<m_nMaxConnect; ++i )
	{
		m_pDPBillings[i].SendKeepAlive();
	}
}


// 
void CBillingMgrJP::OnTimer( CAccount* pAccount )
{
	if( pAccount->m_fCheck == m_iBillingFreePass )	// �������� skip
		return;

	CTime tm = 0;
	if( pAccount->m_TimeOverDays == tm )			// login ��ó���� or timeover skip
		return;		

	CTime cur = CTime::GetCurrentTime();
	if( pAccount->m_TimeOverDays <= cur )			// timerover ó�� 
	{	
		pAccount->m_TimeOverDays = 0;
		g_dpSrvr.CloseExistingConnection( pAccount->m_lpszAccount, ERROR_BILLING_TIME_OVER );
	}
	else
	{
		// ���� �ð� ����
		CTimeSpan ts = pAccount->m_TimeOverDays - cur;
		if ( ts.GetTotalMinutes() >= 59 && ts.GetTotalMinutes() <= 61 && 
			 pAccount->m_nStatus != ACCOUNT_STATUS_NOTIFIED &&
			 pAccount->m_fRoute == TRUE )
		{
			pAccount->m_nStatus = ACCOUNT_STATUS_NOTIFIED;				
			g_dpDbSrvr.SendOneHourNotify( pAccount );	// TRANS�� ������.
		}
	}			
}