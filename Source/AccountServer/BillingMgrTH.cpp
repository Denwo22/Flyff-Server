#include "stdafx.h"
#include "BillingMgrTH.h"
#include "dpBillingTH.h"
#include "account.h"
#include "dpsrvr.h"
#include "dpdbsrvr.h"

extern	CAccountMng	g_AccountMng;
extern	CDPSrvr		g_dpSrvr;
extern	CDPDBSrvr	g_dpDbSrvr;

///////////////////////////////////////////////////////////////////////
// CBillingMgrTH
///////////////////////////////////////////////////////////////////////
CBillingMgrTH::CBillingMgrTH()
{
	m_pDPBillings		= NULL;
	m_nMaxConnect		= 0;
}

CBillingMgrTH::~CBillingMgrTH()
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

BOOL CBillingMgrTH::SetConfig( BILLING_ENUM id, DWORD data )
{
	switch( id )
	{
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
bool CBillingMgrTH::Init( HWND hWnd )
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
	m_pDPBillings = new CDPBillingTH[ nMaxConnect ];

	for( int i=0; i<m_nMaxConnect; ++i )
	{
		m_pDPBillings[i].Init( hWnd );
	}

	::PostMessage( hWnd, WM_CONNENT_BILLING, 0, 0 );
	::SetTimer( hWnd, IDT_KICKOUT, 1000 * 60, NULL );

	return true;
}

// �Ҵ�� �ڿ��� �ݳ��Ѵ�.
void CBillingMgrTH::Release()
{
	safe_delete( this );
}

// ���������� �˻��Ѵ�.
BYTE CBillingMgrTH::CheckAccount( int , DWORD dwSession, const char* szAccount, const char* szAddr )
{
	// ���������� �������� ��û 
	if( RequestBillingInfo( szAccount, szAddr, dwSession ) )
		return ACCOUNT_BILLING_WAIT_ACK;		// �����κ��� ACK�� ���� OnAfterChecking�� ȣ���Ѵ�.
	else
		return ACCOUNT_BILLING_DISCONNECTED;
}

// dwSession�� ���� ������ ����ϴ� ���������� index�� ���Ѵ�.
CDPBillingTH* CBillingMgrTH::GetBilling( DWORD dwSession )
{
	if( m_nMaxConnect == 0 )
		return NULL;

	int iSelect = dwSession % m_nMaxConnect;
	if( !m_pDPBillings[iSelect].m_bConnected )
		return NULL;

	return &m_pDPBillings[iSelect];
}

// ���������� �������� ��û
bool CBillingMgrTH::RequestBillingInfo( LPCTSTR lpszAccount, LPCTSTR lpAddr, DWORD dwSession )
{
	CDPBillingTH* pBilling = GetBilling( dwSession );	
	if( pBilling )
	{
		pBilling->ReqBillingInfo( lpszAccount, lpAddr, dwSession );
		return true;
	}
	else
		return false;
}

// ���������� ���� �α׾ƿ��� �˸���.
void CBillingMgrTH::NotifyLogout( LPCTSTR lpszAccount, DWORD dwSession )
{
	CDPBillingTH* pBilling = GetBilling( dwSession );	
	if( pBilling )
		pBilling->NotifyLogout( lpszAccount, dwSession );
}

// ������ ���õ� ������ �޼����� ó���ǰ� �Ѵ�.
BOOL CBillingMgrTH::PreTranslateMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
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
bool CBillingMgrTH::Connect()
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
void CBillingMgrTH::SendKeepAlive()
{
	for( int i=0; i<m_nMaxConnect; ++i )
	{
		m_pDPBillings[i].SendKeepAlive();
	}
}


// 
void CBillingMgrTH::OnTimer( CAccount* pAccount )
{
	static CTime tm( 0 );
	if( pAccount->m_TimeOverDays == tm )			// login ��ó���� or timeover skip
		return;		
		
	if( pAccount->m_nStatus == ACCOUNT_STATUS_SECONDQUERY )
		return;

	CTime cur = CTime::GetCurrentTime();
	if( pAccount->m_TimeOverDays <= cur )			// timerover ó�� 
	{	
		pAccount->m_nStatus = ACCOUNT_STATUS_SECONDQUERY;	
		CheckAccount( 0, pAccount->m_dwAuthKey, pAccount->m_lpszAccount, "" );
	}
	else
	{
		// ���� �ð� ����
		CTimeSpan ts = pAccount->m_TimeOverDays - cur;
		if ( ts.GetTotalMinutes() >= 29 && ts.GetTotalMinutes() <= 31 && 
			 pAccount->m_nStatus != ACCOUNT_STATUS_NOTIFIED &&
			 pAccount->m_fRoute == TRUE )
		{
			pAccount->m_nStatus = ACCOUNT_STATUS_NOTIFIED;				
			g_dpDbSrvr.SendOneHourNotify( pAccount );	// TRANS�� ������.
		}
	}			
}