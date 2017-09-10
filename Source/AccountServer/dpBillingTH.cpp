#include "stdafx.h"
#include "account.h"
#include "dpsrvr.h"
#include "DPBillingTH.h"
#include "BillingMgr.h"

extern	CDPSrvr			g_dpSrvr;
extern	CAccountMng		g_AccountMng;

const DWORD	MAX_ID_LENGTH	= 30;
const DWORD	MAX_IP_LENGTH	= 16;

const DWORD TH_REQ_BILLINGINFO	= 0x00000001;
const DWORD	TH_ACK_BILLINGINFO	= 0x00000101;
const DWORD TH_ACK_ISALIVE		= 0x00000002;
const DWORD TH_REQ_ISALIVE		= 0x00000102;
const DWORD TH_REQ_KEEPALIVE    = 0x00000003;
const DWORD TH_ACK_KEEPALIVE    = 0x00000103;
const DWORD TH_LOGOUT		    = 0x00000004;

////////////////////////////////////////////////////////////////////////////////////////////
// CDPBillingTH
////////////////////////////////////////////////////////////////////////////////////////////
CDPBillingTH::CDPBillingTH()
{
	m_hWnd			= NULL;
	m_bConnected	= FALSE;
	m_nKeepAlive    = 0;

	BEGIN_MSG;
	ON_MSG( TH_ACK_KEEPALIVE, &CDPBillingTH::AckKeepAlive );
	ON_MSG( TH_ACK_BILLINGINFO, &CDPBillingTH::AckBillingInfo );
	ON_MSG( TH_REQ_ISALIVE, &CDPBillingTH::ReqIsAlive );
}

CDPBillingTH::~CDPBillingTH()
{
}

void CDPBillingTH::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			{
				OutputDebugString("billing server connected.\n");
				SetTimer( m_hWnd, IDT_KEEPALIVE, 1000 * 60 * 5, NULL );
				
				m_bConnected = TRUE;
				break;
			}
		case DPSYS_DESTROYPLAYERORGROUP:
			{
				m_bConnected = FALSE;

				OutputDebugString("billing server disconnected.\n");
				KillTimer( m_hWnd, IDT_KEEPALIVE );
				::PostMessage( m_hWnd, WM_CONNENT_BILLING, 0, 0 );
				break;
			}
	}
}

void CDPBillingTH::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	CAr ar( (LPBYTE)lpMsg, dwMsgSize );
	DWORD dw;
	ar >> dw;
	
	void ( theClass::*pfn )( theParameters ) =	GetHandler( ntohl( dw ) );
	
	if( pfn )
		( this->*( pfn ) )( ar, idFrom );	
	else
	{
		char szBuffer[1024];
		sprintf( szBuffer, "abnormal billing protocol: %x\n", ntohl( dw ) ); 
		OutputDebugString( szBuffer );
	}
}

// �������� REQ
void CDPBillingTH::ReqBillingInfo( LPCTSTR lpszUserID, LPCTSTR lpszUserIP, DWORD dwSession )
{
	OutputDebugString("REQ_BILLINGINFO\n");

	char cbUserID[MAX_ID_LENGTH];	// NULL�� ������ �ʴ´�.
	char cbUserIP[MAX_IP_LENGTH];

//	char* p;
//	p = StrCpyExcludeNull( cbUserID, lpszUserID );
//	AppendSpace( p, cbUserID + MAX_ID_LENGTH );
	strncpy( cbUserID, lpszUserID, MAX_ID_LENGTH );
	cbUserID[ MAX_ID_LENGTH-1 ] = '\0';

//	p = StrCpyExcludeNull( cbUserIP, lpszUserIP );
//	AppendSpace( p, cbUserIP + MAX_IP_LENGTH );
	strncpy( cbUserIP, lpszUserIP, MAX_IP_LENGTH );
	cbUserIP[ MAX_IP_LENGTH-1 ] = '\0';

	CAr	ar;
	int nBufSize;
	ar << htonl( TH_REQ_BILLINGINFO );
	ar.Write( cbUserID, MAX_ID_LENGTH );
	ar << htonl( dwSession );
	ar.Write( cbUserIP, MAX_IP_LENGTH );
	SEND( ar, this, DPID_SERVERPLAYER );
}
/*
0 : ������ ����, 1: ������ ����
100 : ��� ó���� �ý��� �������� ó���Ǿ����� ticket�� ���� ������ �Ǵܵ�
101 : DBó������ error�߻�
102 : DBó������ �⿡���߻�
*/
BILLING_RETURN GetResult( long lData )
{
	BILLING_RETURN result;
	switch( lData )
	{
	case 0:		// ����������
	case 1:		// ���������� 
		result = SUCCESS;
		break;
	case 100:	// ��� ó���� �ý��� �������� ó���Ǿ����� ticket�� ���� ������ �Ǵܵ�
		result = BILLING_INFO_FAILED;
		break;
	case 101:	// ��񿡷� 
		result = DATABASE_ERROR;
		break;
	case 102:	// ��Ÿ���� 
	default:
		result = OTHER_ERROR;
		break;
	}
	return result;
}
/*
// chString�迭�ȿ� 0x20�� �����Ͽ��� NULL�� ������ ���ڿ��� �����.
void TrimSpace( char* chString, int nMax )
{
	for( int i=0; i<nMax; ++i )
	{
		if( chString[i] == 0x20 )
			break;
	}
	chString[i] = '\0';
}
*/

enum {
	ALIVE_TRUE,	// �÷��� ��	 
	ALIVE_FALSE	//�÷��� �� �ƴ�
};

// Ư�������� �÷��� �������� billing agent(���̷��� ��������)�� ���´�.
void CDPBillingTH::ReqIsAlive( CAr & ar, DPID dpid )
{
	char	szAccount[MAX_ID_LENGTH+1];
	long	lRemain;
	DWORD	dwSession;

	ar.Read( szAccount, MAX_ID_LENGTH );
	ar >> dwSession;
	ar >> lRemain;

	dwSession = (DWORD)ntohl( dwSession );	
	lRemain	  = ntohl( lRemain );	
//	TrimSpace( szAccount, MAX_ID_LENGTH );	

	int nResult = ALIVE_FALSE;  
	if( g_AccountMng.GetAccount( szAccount, dwSession ) )
		nResult = ALIVE_TRUE;	

	{
		char cbUserID[MAX_ID_LENGTH];		
//		char* p = StrCpyExcludeNull( cbUserID, szAccount );
//		AppendSpace( p, cbUserID + MAX_ID_LENGTH );
		strncpy( cbUserID, szAccount, MAX_ID_LENGTH );
		cbUserID[ MAX_ID_LENGTH-1 ] = '\0';
	
		{
			char szBuffer[256];
			sprintf(szBuffer, "TH_ACK_ISALIVE - user:%s session:%d nResult:%d\n",		     
							   szAccount, dwSession, nResult );
			OutputDebugString( szBuffer );
		}
		CAr	ar;
		int nBufSize;
		ar << htonl( TH_ACK_ISALIVE );
		ar.Write( cbUserID, MAX_ID_LENGTH );
		ar << htonl( dwSession );
		ar << htonl( nResult );
		SEND( ar, this, DPID_SERVERPLAYER );
	}
}

// ���������� ACK
void CDPBillingTH::AckBillingInfo( CAr & ar, DPID dpid )
{
	char	szAccount[MAX_ID_LENGTH+1];
	long	lData, lRemain;
	DWORD	dwSession;

	ar >> lData;
	ar.Read( szAccount, MAX_ID_LENGTH );
	ar >> dwSession;
	ar >> lRemain;

	lData	  = ntohl( lData );		// ȣ��Ʈ ���� ������ ��ȯ�Ѵ�.
	dwSession = (DWORD)ntohl( dwSession );	
	lRemain	  = ntohl( lRemain );	

//	TrimSpace( szAccount, MAX_ID_LENGTH );

	{
		char szBuffer[256];
		sprintf(szBuffer, "TH_ACK_BILLINGINFO - result:%d user:%s session:%d remain:%d\n",		     
						   lData, szAccount, dwSession, lRemain );
		OutputDebugString( szBuffer );
	}

	BILLING_RETURN result = GetResult( lData );

	CTime tmOverDays( 0 );
	if( result == SUCCESS )
	{
		if( lRemain > 0 )		// TODO: 30�� ���Ϸ� ���� ������ cancel
			tmOverDays = time( NULL ) + lRemain;  // == CTime::GetCurrentTime() + CTimeSpan( lRemain ) ;	
		else
			result = TIME_OVER;
	}
		
	BILLING_INFO info;
	info.szAccount = szAccount;
	info.dwKey = dwSession;
	info.lResult = result;
	info.pTimeOverDays = &tmOverDays;	// tmOverDays�� lResult�� SUCCESS�� ��쿡�� �����Ǹ� �ȴ�.
	
	g_AccountMng.SendBillingResult( &info );
}

void CDPBillingTH::SendKeepAlive()
{
	if( m_bConnected == false )
	{
		OutputDebugString("TH_REQ_KEEPALIVE FAIL because disconnected.\n");
		return;
	}

	OutputDebugString("send TH_REQ_KEEPALIVE\n");

	CAr	ar;
	int nBufSize;
	ar << htonl( TH_REQ_KEEPALIVE );
	ar << htonl( ++m_nKeepAlive );
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPBillingTH::AckKeepAlive( CAr & ar, DPID dpid )
{
	int nCur;
	ar >> nCur;
	nCur = ntohl( nCur );		
	
	if( abs( m_nKeepAlive - nCur ) >= 2 )
	{
		OutputDebugString("TODO: close billing agent.\n");
	}

	OutputDebugString("recv TH_ACK_KEEPALIVE\n");
}

void CDPBillingTH::NotifyLogout( LPCTSTR lpszAccount, DWORD dwSession )
{
	OutputDebugString("send TH_LOGOUT\n");

	char cbUserID[MAX_ID_LENGTH];		
//	char* p = StrCpyExcludeNull( cbUserID, lpszAccount );
//	AppendSpace( p, cbUserID + MAX_ID_LENGTH );
	strncpy( cbUserID, lpszAccount, MAX_ID_LENGTH );
	cbUserID[ MAX_ID_LENGTH-1 ] = '\0';

	CAr	ar;
	int nBufSize;
	ar << htonl( TH_LOGOUT );
	ar.Write( cbUserID, MAX_ID_LENGTH );
	ar << htonl( dwSession );
	SEND( ar, this, DPID_SERVERPLAYER );
}