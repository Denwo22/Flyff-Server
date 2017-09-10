#include "StdAfx.h"
#include "defineText.h"
#include "AppDefine.h"
#ifdef __CERTIFIER_COLLECTING_SYSTEM
#include "DPCollectClient.h"
#include "tools.h"
extern char	g_szVersion[];
#endif // __CERTIFIER_COLLECTING_SYSTEM
#include "DPCertified.h"
#include "dpclient.h"
#include "DPLoginClient.h"
#include "Network.h"
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
#include "Wnd2ndPassword.h"
#endif // __2ND_PASSWORD_SYSTEM

extern	CDPClient		g_DPlay;
extern	CDPLoginClient	g_dpLoginClient;
 
CDPLoginClient::CDPLoginClient()
{
	m_timer.Set( SEC( 10 ) );
	m_fConn	= FALSE;
	m_nSlot	= -1;
	m_bShowDisconnectMsg = TRUE;
	m_lError = 0;
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
	m_idNumberPad = 0;
#endif // __2ND_PASSWORD_SYSTEM

	BEGIN_MSG;
	ON_MSG( PACKETTYPE_ERROR, &CDPLoginClient::OnError );
	ON_MSG( PACKETTYPE_PLAYER_LIST, &CDPLoginClient::OnPlayerList );
	ON_MSG( PACKETTYPE_CACHE_ADDR, &CDPLoginClient::OnCacheAddr );
	ON_MSG( PACKETTYPE_QUERYTICKCOUNT, &CDPLoginClient::OnQueryTickCount );
	ON_MSG( PACKETTYPE_PRE_JOIN, &CDPLoginClient::OnPreJoin );
	ON_MSG( PACKETTYPE_ONE_HOUR_NOTIFY, &CDPLoginClient::OnOneHourNotify );
	
	ON_MSG( PACKETTYPE_PING, &CDPLoginClient::OnPing );
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
	ON_MSG( PACKETTYPE_LOGIN_PROTECT_NUMPAD, &CDPLoginClient::OnLoginNumPad );
	ON_MSG( PACKETTYPE_LOGIN_PROTECT_CERT, &CDPLoginClient::OnLoginProtect );
#endif // __2ND_PASSWORD_SYSTEM
}

CDPLoginClient::~CDPLoginClient()
{

}

LONG CDPLoginClient::GetNetError()
{
	return m_lError;
}


void CDPLoginClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	switch( lpMsg->dwType )
	{
		case DPSYS_CREATEPLAYERORGROUP:
			m_fConn		= TRUE;
			break;

		case DPSYS_DESTROYPLAYERORGROUP:
			{
				#ifdef __INFO_SOCKLIB0516
				{
					LPDPMSG_DESTROYPLAYERORGROUP lpDestroyPlayer	= (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
					m_lError = lpDestroyPlayer->dwFlags;
				}
				#endif

				CNetwork::GetInstance().OnEvent( LOGIN_DISCONNECT );

				m_fConn		= FALSE;
				g_DPlay.DeleteDPObject();

				if( m_bShowDisconnectMsg )
					g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_DIAG_0023), MB_OK, TRUE );	//�����κ��� ������ �����Ǿ����ϴ�.		  
				m_bShowDisconnectMsg = TRUE;
				break;
			}
	}
}

void CDPLoginClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	CAr ar( (LPBYTE)lpMsg, dwMsgSize );
	GETTYPE( ar );
	void ( theClass::*pfn )( theParameters )	=	GetHandler( dw );
	
	if( pfn ) 
		( this->*( pfn ) )( ar );
}

void CDPLoginClient::QueryTickCount( void )
{
	BEFORESENDSOLE( ar, PACKETTYPE_QUERYTICKCOUNT, DPID_UNKNOWN );
	ar << (DWORD)timeGetTime();
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPLoginClient::OnQueryTickCount( CAr & ar )
{
	DWORD dwTime;
	__int64 nTickCount;
	ar >> dwTime >> nTickCount;

	dwTime	= timeGetTime() - dwTime;
	nTickCount	+= ( dwTime * 5000 );	// ( dwTime / 2 ) * 10000

	g_TickCount.Start( nTickCount );

	TRACE( "OnQueryTickCount, %d\n", dwTime );
}

void CDPLoginClient::SendGetPlayerList( DWORD dwID, LPCSTR lpszAccount, LPCSTR lpszpw )
{
	BEFORESENDSOLE( ar, PACKETTYPE_GETPLAYERLIST, DPID_UNKNOWN );
	ar.WriteString( ::GetProtocolVersion() );
	ar << g_Neuz.m_dwAuthKey;
	ar.WriteString( lpszAccount );
	ar.WriteString( lpszpw );
	ar << dwID;
	SEND( ar, this, DPID_SERVERPLAYER );
}
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
void CDPLoginClient::SendCreatePlayer(BYTE nSlot, LPCSTR lpszPlayer/*, LPDWORD adwEquipment*/, BYTE nFace, BYTE nCostume, BYTE nSkinSet, BYTE nHairMesh, DWORD dwHairColor, BYTE nSex, BYTE nJob, BYTE nHeadMesh, int nBankPW )
#else // __2ND_PASSWORD_SYSTEM
void CDPLoginClient::SendCreatePlayer(BYTE nSlot, LPCSTR lpszPlayer/*, LPDWORD adwEquipment*/, BYTE nFace, BYTE nCostume, BYTE nSkinSet, BYTE nHairMesh, DWORD dwHairColor, BYTE nSex, BYTE nJob, BYTE nHeadMesh )
#endif // __2ND_PASSWORD_SYSTEM
{
	BEFORESENDSOLE( ar, PACKETTYPE_CREATE_PLAYER, DPID_UNKNOWN );
#ifdef __GPAUTH_01
	ar.WriteString( g_Neuz.m_bGPotatoAuth?g_Neuz.m_szGPotatoNo: g_Neuz.m_szAccount );
#else	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szAccount );
#endif	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szPassword );
	ar << nSlot;

	if( strlen( lpszPlayer ) > 16 )
		Error( "CDPLoginClient::SendCreatePlayer���� �̻� ĳ���� �� E: %s", lpszPlayer );
	ar.WriteString( lpszPlayer );
	if( strlen( lpszPlayer ) > 16 )
		Error( "CDPLoginClient::SendCreatePlayer���� �̻� ĳ���� �� F: %s", lpszPlayer );
	
	//	ar.Write( adwEquipment, sizeof(DWORD) * MAX_HUMAN_PARTS );

	ar << nFace << nCostume << nSkinSet << nHairMesh;
	ar << dwHairColor;
	ar << nSex << nJob << nHeadMesh;
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
	ar << nBankPW;
#endif // __2ND_PASSWORD_SYSTEM
	ar << g_Neuz.m_dwAuthKey;
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPLoginClient::SendDeletePlayer( BYTE nSlot, LPCTSTR szNo )
{
	BEFORESENDSOLE( ar, PACKETTYPE_DEL_PLAYER, DPID_UNKNOWN );

#ifdef __GPAUTH_01
	ar.WriteString( g_Neuz.m_bGPotatoAuth? g_Neuz.m_szGPotatoNo: g_Neuz.m_szAccount );
#else	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szAccount );
#endif	// __GPAUTH_01
	ar.WriteString( g_Neuz.m_szPassword );
	ar.WriteString( g_Neuz.GetDeleteKey( szNo ) );
	ar << g_Neuz.m_apPlayer[nSlot]->m_idPlayer;
	ar << g_Neuz.m_dwAuthKey;
#ifdef __RT_1025
	ar << g_Neuz.m_aRTMessenger[nSlot].size();
	for( CRTMessenger::iterator it=g_Neuz.m_aRTMessenger[nSlot].begin(); it!=g_Neuz.m_aRTMessenger[nSlot].end(); it++ )
	{
		u_long uTemp = it->first;
		ar << it->first;
	}
#endif // __RT_1025

	SEND( ar, this, DPID_SERVERPLAYER );
}

#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
void CDPLoginClient::SendPreJoin( const TCHAR* lpszAccount, u_long idPlayer, const TCHAR* lpszPlayer, int nSlot, int nSecretNum )
#else // __2ND_PASSWORD_SYSTEM
void CDPLoginClient::SendPreJoin( const TCHAR* lpszAccount, u_long idPlayer, const TCHAR* lpszPlayer, int nSlot )
#endif // __2ND_PASSWORD_SYSTEM
{
	m_nSlot		= nSlot;
	BEFORESENDSOLE( ar, PACKETTYPE_PRE_JOIN, DPID_UNKNOWN );
//	ar << g_Neuz.m_dwAuthKey;
	ar.WriteString( lpszAccount );
	ar << idPlayer;
	ar.WriteString( lpszPlayer );
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
	ar << nSecretNum;
#endif // __2ND_PASSWORD_SYSTEM
	SEND( ar, this, DPID_SERVERPLAYER );
}

void CDPLoginClient::OnPreJoin( CAr & ar )
{
	CNetwork::GetInstance().OnEvent( LOGIN_ACK_PREJOIN );

	// ata2k - (1)�ð� ����
	g_Neuz.m_dwTimeOutDis = 0xffffffff;
	
	// Open world here.	
#ifdef __RT_1025
	g_DPlay.SendJoin( (BYTE)m_nSlot, g_Neuz.m_adwWorldID[m_nSlot], g_Neuz.m_apPlayer[m_nSlot], &g_Neuz.m_aRTMessenger[m_nSlot], g_Neuz.m_uIdofMulti );	
#else	// __RT_1025
	g_DPlay.SendJoin( (BYTE)m_nSlot, g_Neuz.m_adwWorldID[m_nSlot], g_Neuz.m_apPlayer[m_nSlot], &g_Neuz.m_Messenger[m_nSlot], g_Neuz.m_uIdofMulti );	
#endif	// __RT_1025

	// ata2k - (2)�ð� ����
	if( ::GetLanguage() == LANG_ENG && ::GetSubLanguage() == LANG_SUB_USA )
		g_Neuz.m_dwTimeOutDis = GetTickCount() + SEC( 50 );
	else
		g_Neuz.m_dwTimeOutDis = GetTickCount() + SEC( 25 );
	
}

void CDPLoginClient::OnOneHourNotify( CAr & ar )
{
	g_WndMng.OpenMessageBox( prj.GetText(TID_DIAG_EXPIRYTIME) );
}

void CDPLoginClient::OnPing( CAr & ar )
{
	DWORD dwPingTime;
	ar >> dwPingTime;
	g_Neuz.m_dwPingTime = GetCurrentTime() - dwPingTime;
	g_Neuz.m_nPingCount = 0;
}

void CDPLoginClient::OnError( CAr & ar )
{
	ar >> m_lError;

	CNetwork::GetInstance().OnEvent( LOGIN_ERROR );

	switch( m_lError )
	{
		case ERROR_DUPLICATE_ACCOUNT:
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0042) ) );
//				g_WndMng.OpenMessageBox( _T( "�������� �����Դϴ�." ) );
				break;
			}
		case ERROR_USER_EXISTS:
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0034) ) );
//				g_WndMng.OpenMessageBox( _T( "�̹� ������� ĳ���� �̸��Դϴ�" ) );
				CWndCreateChar* pWnd = (CWndCreateChar*)g_WndMng.GetWndBase( APP_CREATE_CHAR );
				if( pWnd )
				{
					pWnd->GetDlgItem( WIDC_OK )->EnableWindow( TRUE );
					pWnd->GetDlgItem( WIDC_CANCEL )->EnableWindow( TRUE );
				}
				break;
			}
		case ERROR_DUPLICATE_SLOT:
			{
				CWndBase* pWndBase	= g_WndMng.GetWndBase( APP_CREATE_CHAR );
				if( pWndBase )
					( (CWndCreateChar*)pWndBase )->Destroy();
				g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_SELECT_CHAR );
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0018) ) );
//				g_WndMng.OpenMessageBox( _T( "������� ���� �Դϴ�." ) );
				break;
			}
		case ERROR_ILLEGAL_VER:
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0035) ) );
//				g_WndMng.OpenMessageBox( _T( "���� ������ Ŭ���̾�Ʈ�Դϴ�" ) );
				break;
			}
		case ERROR_INVALID_NAME_CHARACTER:
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0020) ) );
//				g_WndMng.OpenMessageBox( _T( "����Ҽ� ���� �̸��Դϴ�" ) );
				CWndCreateChar* pWnd = (CWndCreateChar*)g_WndMng.GetWndBase( APP_CREATE_CHAR );
				if( pWnd )
					pWnd->GetDlgItem( WIDC_OK )->EnableWindow( TRUE );
				break;
			}
		case ERROR_NO_SUCH_GROUP:
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0039) ) );
//				g_WndMng.OpenMessageBox( _T( "�߸��Է��Ͽ����ϴ�. �ٽ� �Է��Ͽ� �ֽʽÿ�" ) );
				CWndDeleteChar* pWnd = (CWndDeleteChar*)g_WndMng.GetWndBase( APP_DELETE_CHAR );
				if( pWnd )
				{
					CWndEdit* pWndEdit = (CWndEdit*)pWnd->GetDlgItem( WIDC_EDIT1 );
					pWndEdit->SetString( "" );
					CWndButton* pWndButton = (CWndButton*)pWnd->GetDlgItem( WIDC_OK );
					pWndButton->EnableWindow( TRUE );
					pWndButton	= (CWndButton*)pWnd->GetDlgItem( WIDC_CANCEL );
					pWndButton->EnableWindow( TRUE );	
				}
				break;
			}

		case ERROR_WARTIME:
			{
				CWndDeleteChar* pWnd = (CWndDeleteChar*)g_WndMng.GetWndBase( APP_DELETE_CHAR );
				if( pWnd )
				{
					CWndEdit* pWndEdit = (CWndEdit*)pWnd->GetDlgItem( WIDC_EDIT1 );
					pWndEdit->SetString( "" );
					CWndButton* pWndButton = (CWndButton*)pWnd->GetDlgItem( WIDC_OK );
					pWndButton->EnableWindow( TRUE );
					pWndButton	= (CWndButton*)pWnd->GetDlgItem( WIDC_CANCEL );
					pWndButton->EnableWindow( TRUE );	
				}
				break;
			}

/*
#ifdef __S0114_RELOADPRO
		case ERROR_FLYFF_DB_JOB_ING:	// 133L
			{
				m_bShowDisconnectMsg = FALSE;       // ������ ���� ������ ����Ǿ����ϴٸ� ǥ������ ���� 
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DB_INSPECTION) ) );
				break;
			}
#endif // __S0114_RELOADPRO
*/
		case ERROR_BILLING_TIME_OVER:			// �÷��� ���� ���Ⱓ ����
			m_bShowDisconnectMsg = FALSE;       // ������ ���� ������ ����Ǿ����ϴٸ� ǥ������ ���� 
			g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_DIAG_PLAYNOCHARGING), MB_OK, TRUE );  // TRUE - �޼��� ǥ���� �α���ȭ������ 
			break;
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
		case ERROR_15MIN_PREVENT:	// 2�� ��й�ȣ 3ȸ�̻� Ʋ���� ���
			{
				g_WndMng.CloseMessageBox();
				g_WndMng.OpenMessageBox( _T( prj.GetText( TID_15MIN_PREVENT ) ) );
				g_Neuz.m_dwTimeOutDis = 0xffffffff;
				break;
			}
#endif // __2ND_PASSWORD_SYSTEM
	}
}

void CDPLoginClient::OnPlayerList( CAr & ar )
{
	g_Neuz.m_dwTimeOutDis = 0xffffffff;

#ifdef __CERTIFIER_COLLECTING_SYSTEM
	TCHAR szEncryptedCertifierIP[ MAX_PATH ] = { 0, };
	md5( szEncryptedCertifierIP, g_Neuz.m_lpCertifierAddr );

	BOOL bCertifiedIP = FALSE;
	for( vector< CString >::iterator Iterator = g_vecEncryptedValidCertifierIP.begin(); Iterator != g_vecEncryptedValidCertifierIP.end(); ++Iterator )
	{
		if( strcmp( szEncryptedCertifierIP, *Iterator ) == 0 )
		{
			bCertifiedIP = TRUE;
			break;
		}
	}

	if( bCertifiedIP == FALSE )
	{
#ifdef __CRC
		if( DPCollectClient->ConnectToServer( "127.0.0.1", PN_COLLECTION, TRUE, CSock::crcWrite, 500 ) )	// "log.flyff.com"
#else // __CRC
		if( DPCollectClient->ConnectToServer( "127.0.0.1", PN_COLLECTION, TRUE ) )	// "log.flyff.com"
#endif // __CRC
		{
			DPCollectClient->SendCollectionCertify( g_Neuz.m_lpCertifierAddr, g_szVersion, __VER, ::GetLanguage() );
		}
	}
#endif // __CERTIFIER_COLLECTING_SYSTEM

	CWndBase* pWndBase = g_WndMng.GetWndBase( APP_SELECT_SERVER );
	if( pWndBase )
		((CWndSelectServer*)pWndBase)->Connected();
	
	if( ( pWndBase = g_WndMng.GetWndBase( APP_CREATE_CHAR ) ) )
	{
		( (CWndCreateChar*)pWndBase )->Destroy();
		g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_SELECT_CHAR );
	}
	if( ( pWndBase = g_WndMng.GetWndBase( APP_DELETE_CHAR ) ) )
	{
		( (CWndDeleteChar*)pWndBase )->Destroy();
	}
#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
	if( ( pWndBase = g_WndMng.GetWndBase( APP_2ND_PASSWORD_NUMBERPAD ) ) )
	{
		( (CWnd2ndPassword*)pWndBase )->Destroy();
	}
#endif // __2ND_PASSWORD_SYSTEM

	DWORD dwAuthKey;
	ar >> dwAuthKey;
	if( dwAuthKey != g_Neuz.m_dwAuthKey )
	{
		CNetwork::GetInstance().OnEvent( LOGIN_AUTHKEY_ERROR );
		return;
	}

	DWORD dwIndex;

	CObj::SetMethod( METHOD_NONE );

	for( int i = 0 ; i < MAX_CHARACTER_LIST ; i++ )
	{
		g_Neuz.m_nCharacterBlock[i] = 2; // ��� �󽽷����� ��
		SAFE_DELETE( g_Neuz.m_apPlayer[i] );
	}
	
	int countPlayer = 0;
	ar >> countPlayer;

	for( int i = 0 ; i < countPlayer ; i++ )
	{
//		CMover* pMover;
		BYTE bySex;
		int slot;	
		DWORD dwJobLv;

		ar >> slot;

		ar >> g_Neuz.m_nCharacterBlock[slot];
		
		ar >> g_Neuz.m_adwWorldID[slot];
		ar >> dwIndex;


		CMover* pMover = new CMover;
		pMover->m_dwType = OT_MOVER;
		pMover->m_dwIndex = dwIndex;
		g_Neuz.m_apPlayer[slot]	= pMover;

		//g_Neuz.m_apPlayer[slot]	= (CMover*)CreateObj( g_Neuz.m_pd3dDevice, OT_MOVER, dwIndex, FALSE );
		g_Neuz.m_apPlayer[slot]->m_bPlayer	= TRUE;
		g_Neuz.m_apPlayer[slot]->InitProp();

		ar.ReadString(  (char*)g_Neuz.m_apPlayer[slot]->GetName(), MAX_NAME );
		D3DXVECTOR3 Pos;
		ar >> Pos;
		g_Neuz.m_apPlayer[slot]->SetPos(Pos);
		ar >> g_Neuz.m_apPlayer[slot]->m_idPlayer;
		ar >> g_Neuz.m_apPlayer[slot]->m_idparty;
		ar >> g_Neuz.m_apPlayer[slot]->m_idGuild;
		ar >> g_Neuz.m_apPlayer[slot]->m_idWar;
		ar >> g_Neuz.m_apPlayer[slot]->m_dwSkinSet;
		ar >> g_Neuz.m_apPlayer[slot]->m_dwHairMesh;
		ar >> g_Neuz.m_apPlayer[slot]->m_dwHairColor;

		ar >> g_Neuz.m_apPlayer[slot]->m_dwHeadMesh;
		ar >> bySex;
		g_Neuz.m_apPlayer[slot]->SetSex( bySex );
		ar >> g_Neuz.m_apPlayer[slot]->m_nJob;
		ar >> g_Neuz.m_apPlayer[slot]->m_nLevel;
		ar >> dwJobLv;
		ar >> g_Neuz.m_apPlayer[slot]->m_nStr;
		ar >> g_Neuz.m_apPlayer[slot]->m_nSta;
		ar >> g_Neuz.m_apPlayer[slot]->m_nDex;
		ar >> g_Neuz.m_apPlayer[slot]->m_nInt;
		g_Neuz.m_apPlayer[slot]->SetHairColor( g_Neuz.m_apPlayer[slot]->m_dwHairColor );
#if __VER < 8 // __S8_PK
		ar >> g_Neuz.m_apPlayer[slot]->m_nSlaughter;
#endif // __VER < 8 __S8_PK
		ar >> g_Neuz.m_apPlayer[slot]->m_dwMode;

		int CountEquip = 0;
		ar >> CountEquip;
		
		ItemProp* pItemProp;
		DWORD dwItemId;
		int nParts = 0;
		for( int i = 0; i < CountEquip ; i++ )
		{
			ar >> dwItemId;
			pItemProp	= prj.GetItemProp( dwItemId );
			if( pItemProp == NULL)
				Error( "CDPLoginClient::OnPlayerList : %d������ ������Ƽ ����", dwItemId );

			nParts = (int)pItemProp->dwParts;
			// ������ ���������� �����ε� �̹� �����տ� ���� �����Ǿ� ������ 
			if( nParts == PARTS_RWEAPON && g_Neuz.m_apPlayer[slot]->m_aEquipInfo[nParts].dwId != NULL_ID )
			{
				// �����տ��� �޼����� �ű�. - ������ �Ųٷ� ���־ �̷��� �ߴ�.
				g_Neuz.m_apPlayer[slot]->m_aEquipInfo[PARTS_LWEAPON].dwId = g_Neuz.m_apPlayer[slot]->m_aEquipInfo[PARTS_RWEAPON].dwId;
				// ���� ���� ����� �����տ� ��.
			}
			g_Neuz.m_apPlayer[slot]->m_aEquipInfo[nParts].dwId	= dwItemId;
		}
	}
	int CountMessenger = 0;
	ar >> CountMessenger;
	BYTE nSlot = 0;
	for( int i = 0 ; i < CountMessenger ; i++ )
	{		
		ar >> nSlot;
#ifdef __RT_1025
		g_Neuz.m_aRTMessenger[nSlot].Serialize( ar );
#else	// __RT_1025
		g_Neuz.m_Messenger[nSlot].Serialize( ar );
#endif	// __RT_1025
	}

	CNetwork::GetInstance().OnEvent( LOGIN_PLAYER_LIST );

	CWndSelectChar* pWndSelectChar = (CWndSelectChar*)g_WndMng.GetWndBase( APP_SELECT_CHAR );
	if( pWndSelectChar )
	{
		pWndSelectChar->DeleteCharacter();
		pWndSelectChar->UpdateCharacter();
		if( g_Neuz.m_nCharacterBlock[ pWndSelectChar->m_nSelectCharacter ] == 0 )
		{
			for( int i = 0 ; i < MAX_CHARACTER_LIST ; i++ )
			{
				if( g_Neuz.m_nCharacterBlock[i] == 1 )
				{
					pWndSelectChar->m_nSelectCharacter = i;
					break;
				}
			}
		}
		pWndSelectChar->SelectCharacter( pWndSelectChar->m_nSelectCharacter );
//		g_WndMng.OpenCustomBox( _T( "�����Ҽ� ���� �����Դϴ�"), new CWndAllCharBlockBox );
		// ��� ĳ���Ͱ� ���Ǿ� �ֽ��ϴ�.		
	}
}

void CDPLoginClient::OnCacheAddr( CAr & ar )
{	
	ar.ReadString( g_Neuz.m_lpCacheAddr, 16 );
	
	// ���� �׽�Ʈ ������ ���� �����̸�, �缳 �ּҷ� ��ü
	CString strAddr	= g_Neuz.m_lpCertifierAddr;
	if( ::GetLanguage() == LANG_GER && strAddr.Find( "192.168", 0 ) == 0 )
		lstrcpy( g_Neuz.m_lpCacheAddr, g_Neuz.m_lpCertifierAddr );

	CNetwork::GetInstance().OnEvent( LOGIN_CACHE_ADDR );
}

#if __VER >= 15 // __2ND_PASSWORD_SYSTEM
void CDPLoginClient::OnLoginNumPad( CAr & ar )
{
	u_long idNumPad;
	ar >> idNumPad;
	m_idNumberPad = idNumPad;
}

void CDPLoginClient::OnLoginProtect( CAr & ar )
{
	g_Neuz.m_dwTimeOutDis = 0xffffffff;
	BOOL bLogin;
	u_long idNumPad;
	ar >> bLogin >> idNumPad;

	if( ::IsUse2ndPassWord() == TRUE )
	{
		CWndSelectChar* pWndSelectChar = (CWndSelectChar*)g_WndMng.GetWndBase( APP_SELECT_CHAR );
		if( pWndSelectChar == NULL )
			return;

		if( bLogin == FALSE )
		{
			g_WndMng.CloseMessageBox();
			g_WndMng.OpenMessageBox( prj.GetText( TID_2ND_PASSWORD_CONNECTING_FAIL_ERROR ) );	// 2�� ��й�ȣ�� Ʋ�Ƚ��ϴ�.
			m_idNumberPad = idNumPad;
			pWndSelectChar->m_pWnd2ndPassword->ResetNumberpad( idNumPad );
			pWndSelectChar->m_pWnd2ndPassword->DeletePassword();
		}
	}
}

u_long CDPLoginClient::GetNumberPad( void ) const
{
	return m_idNumberPad;
}
#endif // __2ND_PASSWORD_SYSTEM

CDPLoginClient	g_dpLoginClient;