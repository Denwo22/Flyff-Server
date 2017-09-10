#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndParty.h"
#include "WndManager.h"
#include "Party.h"
#include "dpclient.h"
extern CDPClient	g_DPlay;

#if __VER >= 11 // __SYS_PLAYER_DATA
#include "playerdata.h"
#endif	// __SYS_PLAYER_DATA


/****************************************************
  WndId : APP_PARTY - �ش�(Party)
  CtrlId : WIDC_TABCTRL1 - TabCtrl
  CtrlId : WIDC_LEAVE - Ż��
  CtrlId : WIDC_CHANGE - �ش���ȯ
****************************************************/

CWndParty::CWndParty() 
{ 
	m_WndPartyChangeTroup = NULL;
#if __VER >= 8 //__CSC_VER8_2
	m_pWndPartyQuick = NULL;
#endif //__CSC_VER8_2
	Error( "CWndParty::CWndParty()" );
} 
CWndParty::~CWndParty() 
{ 
#ifdef __FIX_WND_1109
	SAFE_DELETE( m_WndPartyChangeTroup );
#endif	// __FIX_WND_1109
	DeleteDeviceObjects();
} 
void CWndParty::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	Error( "CWndParty::SerializeRegInfo() - IN" );

	CWndNeuz::SerializeRegInfo( ar, dwVersion );
	CWndTabCtrl* lpTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	if( ar.IsLoading() )
	{
		if( dwVersion == 0 )
		{
		}
		else
		if( dwVersion == 1 )
		{
			int nCurSel;
			ar >> nCurSel;
			lpTabCtrl->SetCurSel( nCurSel );
		}
	}
	else
	{
		dwVersion = 1;
		ar << lpTabCtrl->GetCurSel();
	}

	Error( "CWndParty::SerializeRegInfo() - OUT" );
}
void CWndParty::OnDraw( C2DRender* p2DRender ) 
{ 
	if( g_Party.GetSizeofMember() < 2 )
	{
		m_pWndLeave->EnableWindow( FALSE );
		m_pWndTransfer->EnableWindow( FALSE );
#if __VER >= 8 //__CSC_VER8_2
		m_pBtnPartyQuick->EnableWindow( FALSE );
#endif //__CSC_VER8_2
	}
	else
	{
		m_pWndLeave->EnableWindow( TRUE );
		m_pWndTransfer->EnableWindow( TRUE );
#if __VER >= 8 //__CSC_VER8_2
		m_pBtnPartyQuick->EnableWindow( TRUE );
#endif //__CSC_VER8_2
	}

#ifdef __PARTY_DEBUG_0129		// �ش��� ƨ��� ���� ����� neuz
	if( g_Party.GetSizeofMember() < 2 && g_Party.m_nLevel > 1 )
	{
		m_pWndLeave->EnableWindow( TRUE );
	}
#endif	// __PARTY_DEBUG_0129		// �ش��� ƨ��� ���� ����� neuz

	if( g_Party.GetLevel() >= 10 && g_Party.IsLeader( g_pPlayer->m_idPlayer ) )
		m_pWndChange->EnableWindow( TRUE );
	else
		m_pWndChange->EnableWindow( FALSE );

	// ��Ƽ ���� ���
	CWndStatic* pWndStatic;
	CString strTemp;
	if(g_Party.m_nKindTroup==0)
	{
		strTemp.Format("%s",prj.GetText(TID_GAME_PARTY1));
	}
	else 
	{
		if( 0 == strlen( g_Party.m_sParty ) )
			strTemp.Format("%s",prj.GetText(TID_GAME_PARTY2));
		else
			strTemp.Format("%s",g_Party.m_sParty);
	}
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_NAME );
	pWndStatic->SetTitle(strTemp);
	strTemp.Format("%d",g_Party.m_nLevel);
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_LEVEL );
	pWndStatic->SetTitle(strTemp);
	
	if( g_Party.m_nLevel >= 10 )
	{
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)( ( ( 50 + g_Party.GetLevel() ) * g_Party.GetLevel() / 13 ) * 10 ));
	}
	else
	{
		strTemp.Format("%.2f %%", (float)g_Party.m_nExp * 100 / (float)prj.m_aExpParty[g_Party.m_nLevel].Exp );
	}
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_EXP );
	pWndStatic->SetTitle(strTemp);
	strTemp.Format("%d",g_Party.m_nPoint);
	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_POINT );
	pWndStatic->SetTitle(strTemp);
	// ������ ����ġ �й��� ���� ����
	CWndButton* pWndButton;
	switch( g_Party.m_nTroupsShareExp) 
	{
	case 0:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(FALSE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(FALSE);
		}
		break;
	case 1:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(TRUE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(FALSE);
		}
		break;
	case 2:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_EXP_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO6 );
			pWndButton->SetCheck(FALSE);
//			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO7 );
//			pWndButton->SetCheck(TRUE);
		}
		break;
	}
	switch(g_Party.m_nTroupeShareItem) 
	{
	case 0:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 1:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 2:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(TRUE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(FALSE);
		}
		break;
	case 3:
		{
			pWndButton = (CWndButton*)GetDlgItem( WIDC_ITEM_SHARE );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO2 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO3 );
			pWndButton->SetCheck(FALSE);
			pWndButton = (CWndButton*)GetDlgItem( WIDC_RADIO4 );
			pWndButton->SetCheck(TRUE);
		}
		break;
	}
} 
void CWndParty::OnInitialUpdate() 
{ 
	Error( "CWndParty::OnInitialUpdate() - IN" );

	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	m_pWndLeave = (CWndButton*)GetDlgItem( WIDC_LEAVE );
	m_pWndChange = (CWndButton*)GetDlgItem( WIDC_CHANGE );
	m_pWndTransfer = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
#if __VER >= 8 //__CSC_VER8_2
	m_pBtnPartyQuick = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
#else
	m_pBtnPartyQuick = (CWndButton*)GetDlgItem( WIDC_BUTTON2 );
	m_pBtnPartyQuick->EnableWindow(FALSE);
	m_pBtnPartyQuick->SetVisible(FALSE);
#endif //__CSC_VER8_2
	if( g_Party.GetSizeofMember() < 2 )
	{
		m_pWndTransfer->EnableWindow( FALSE );
		m_pWndLeave->EnableWindow( FALSE );
#if __VER >= 8 //__CSC_VER8_2
		m_pBtnPartyQuick->EnableWindow( FALSE );
#endif	//__CSC_VER8_2
	}
	m_pWndChange->EnableWindow( FALSE );
	
	CWndTabCtrl* pWndTabCtrl = (CWndTabCtrl*)GetDlgItem( WIDC_TABCTRL1 );
	CRect rect = GetClientRect();
	rect.left = 5;
	rect.top = 15;

	//gmpbigsun( 100416 ) : Error ���� 
	//Error( "CWndParty::OnInitialUpdate() - m_wndPartyInfo.Create - IN" );
	m_wndPartyInfo.Create( WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME, rect, pWndTabCtrl, APP_PARTY_INFO );
	//Error( "CWndParty::OnInitialUpdate() - m_wndPartyInfo.Create - OUT" );

	//Error( "CWndParty::OnInitialUpdate() - m_wndPartySkill.Create - IN" );
	m_wndPartySkill.Create( WBS_CHILD | WBS_NOFRAME | WBS_NODRAWFRAME , rect, pWndTabCtrl, APP_PARTY_SKILL );
	//Error( "CWndParty::OnInitialUpdate() - m_wndPartySkill.Create - OUT" );

	WTCITEM tabTabItem;
	
	tabTabItem.mask = WTCIF_TEXT | WTCIF_PARAM;
	tabTabItem.pszText = prj.GetText(TID_APP_INFOMATION);//"����";
	tabTabItem.pWndBase = &m_wndPartyInfo;
	pWndTabCtrl->InsertItem( 0, &tabTabItem );
	
	tabTabItem.pszText = prj.GetText(TID_APP_SKILL);//"��ų";
	tabTabItem.pWndBase = &m_wndPartySkill;
	pWndTabCtrl->InsertItem( 1, &tabTabItem );

	MoveParentCenter();

	Error( "CWndParty::OnInitialUpdate() - OUT" );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndParty::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	Error( "CWndParty::Initialize() - IN" );
	BOOL bFlag = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY, 0, CPoint( 0, 0 ), pWndParent );
	Error( "CWndParty::Initialize() - OUT" );

	// Daisy���� ������ ���ҽ��� ������ ����.
	return bFlag;
} 
/*
  ���� ������ ���� ��� 
BOOL CWndParty::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndParty::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndParty::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndParty::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndParty::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndParty::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_LEAVE ) 
	{ // Ż�� ���ȴ�...
		if( g_pPlayer->m_nDuel == 2 )		// �شܵ�����̸� Ż�� �ȵ�.
		{
			DWORD dwText = TID_GAME_PPVP_LEAVE;
			g_WndMng.PutString( prj.GetText( dwText ), NULL, prj.GetTextColor( dwText ) );
		} 
		else
		{
			if(m_wndPartyInfo.m_nSelected!=-1) 
			{
				u_long  nLeaveMember = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId; //Ż���ų�� ID
				// ����ٰ� Ż��
				if( g_Party.m_nSizeofMember != 0 )
				{
					CWndPartyLeaveConfirm* pWndPartyLeaveConfirm = (CWndPartyLeaveConfirm*)g_WndMng.GetWndBase( APP_PARTYLEAVE_CONFIRM );
					if( pWndPartyLeaveConfirm == NULL )
					{
						g_WndMng.m_pWndPartyLeaveConfirm = new CWndPartyLeaveConfirm;
						g_WndMng.m_pWndPartyLeaveConfirm->Initialize( NULL, APP_PARTYLEAVE_CONFIRM );
						if( g_Party.m_aMember[0].m_uPlayerId == g_pPlayer->m_idPlayer )
							g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( nLeaveMember );
						else
							g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( g_pPlayer->m_idPlayer );
					}
				}
				else
				{
					// �ش��� ������ �ȵǾ�����
				}
			}
			else
			{
				CWndPartyLeaveConfirm* pWndPartyLeaveConfirm = (CWndPartyLeaveConfirm*)g_WndMng.GetWndBase( APP_PARTYLEAVE_CONFIRM );
				if( pWndPartyLeaveConfirm == NULL )
				{
					g_WndMng.m_pWndPartyLeaveConfirm = new CWndPartyLeaveConfirm;
					g_WndMng.m_pWndPartyLeaveConfirm->Initialize( NULL, APP_PARTYLEAVE_CONFIRM );
					if( g_Party.m_nSizeofMember != 0 )
						g_WndMng.m_pWndPartyLeaveConfirm->SetLeaveId( g_pPlayer->m_idPlayer );
				}
				// �� �ڽ��� �شܿ��� Ż��
			}
			m_wndPartyInfo.m_nSelected=-1;
		}
	}
	// �ش��� �ΰ�.
	else if( nID==WIDC_BUTTON1 )
	{
		if( g_Party.m_nSizeofMember != 0 )
		{
			if( g_Party.IsLeader( g_pPlayer->m_idPlayer ) && m_wndPartyInfo.m_nSelected != -1 )
			{
				if( g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_bRemove == FALSE )
				{
					u_long  nLeadMember = g_Party.m_aMember[m_wndPartyInfo.m_nSelected].m_uPlayerId;
					if( g_Party.IsLeader( nLeadMember ) == FALSE )
					{
						if( g_pPlayer->m_nDuel == 2 )	// �ش� ������϶��� �ش����� �ٲܼ� ����
							g_WndMng.PutString( prj.GetText( TID_PK_NOCHANGE_PARTYLEADER ), NULL, prj.GetTextColor( TID_PK_NOCHANGE_PARTYLEADER ) );
						else
							g_DPlay.SendPartyChangeLeader( g_pPlayer->m_idPlayer, nLeadMember );						
					}
					else
					{
						g_WndMng.PutString( prj.GetText( TID_GAME_PARTY_ALREADYMASTER ), NULL, prj.GetTextColor( TID_GAME_PARTY_ALREADYMASTER ) ); // "�̹� ������ �ϰ� �Դϴ�"
					}
				}
				else
				{
					g_WndMng.PutString( prj.GetText( TID_GAME_MASTER_AWAY ), NULL, prj.GetTextColor( TID_GAME_MASTER_AWAY ) ); // "������ �ƴϰų� �ش�â���� �ܿ��� ������ �ؾ� �մϴ�"					
				}
			}
			else
			{
				g_WndMng.PutString( prj.GetText( TID_GAME_PARTY_ISNOTMASTER ), NULL, prj.GetTextColor( TID_GAME_PARTY_ISNOTMASTER ) ); // "������ �ƴϰų� �ش�â���� �ܿ��� ������ �ؾ� �մϴ�"
			}
		}
	}
	else if( nID==WIDC_CHANGE )
	{
		if( g_Party.m_nKindTroup == 0 /* && g_Party.m_nLevel == 10 */ )
		{
			// ��ȸ�ش����� �ٲٱ�~
			SAFE_DELETE( m_WndPartyChangeTroup );
			m_WndPartyChangeTroup = new CWndPartyChangeTroup;
#ifdef __FIX_WND_1109
			m_WndPartyChangeTroup->Initialize( this );
#else	// __FIX_WND_1109
			m_WndPartyChangeTroup->Initialize();
#endif	// __FIX_WND_1109
		}
	}
#if __VER >= 8 //__CSC_VER8_2
	else if(nID == WIDC_BUTTON2)
	{
		if(g_Party.GetSizeofMember() >= 2)
		{
			m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.GetApplet( APP_PARTY_QUICK );
			if(m_pWndPartyQuick == NULL)
			{
				m_pWndPartyQuick = (CWndPartyQuick*)g_WndMng.CreateApplet( APP_PARTY_QUICK );
				m_pWndPartyQuick->SetActiveMember(g_Party.GetSizeofMember());
			}
			else
			{
				m_pWndPartyQuick->Destroy();
				m_pWndPartyQuick = NULL;
			}
		}
	}
#endif //__CSC_VER8_2
	// ��ȸ�ش��϶��� ������ ����ġ �й��� ����
	else
	if( g_Party.IsLeader(g_pPlayer->m_idPlayer) )
	{
		if( g_Party.m_nKindTroup != 0 )
		{
			// ����ġ �й� ����� ��ȸ�ش� ��
			int nTroupsShareExp = g_Party.m_nTroupsShareExp;
			
			if(nID==WIDC_EXP_SHARE) nTroupsShareExp   = 0;
			else if(nID==WIDC_RADIO6) nTroupsShareExp = 1;
	//		else if(nID==WIDC_RADIO7) nTroupsShareExp = 2;
			
			if( nTroupsShareExp == 2 )
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0004) ) );
				//			g_WndMng.OpenMessageBox( _T("����ġ �й��� ���Ϻй�� ���� ������ �ȵ˴ϴ�.") );
			}
			else
			{
				if( nTroupsShareExp != g_Party.m_nTroupsShareExp )
				{
					// Send
					g_DPlay.SendChangeShareExp( nTroupsShareExp );
				}
			}
		}
		// ������ �й� ����� �ܸ��ش� ��
		int nTroupeShareItem = g_Party.m_nTroupeShareItem;

		if(nID==WIDC_ITEM_SHARE) nTroupeShareItem  = 0;
		else if(nID==WIDC_RADIO2) nTroupeShareItem = 1;
		else if(nID==WIDC_RADIO3) nTroupeShareItem = 2;
		else if(nID==WIDC_RADIO4) nTroupeShareItem = 3;		

		if( nTroupeShareItem != g_Party.m_nTroupeShareItem )
		{
			// Send
			g_DPlay.SendChangeShareItem( nTroupeShareItem );
			
		}
	}
	else
	{
		if( nID != WTBID_CLOSE && nID != WTBID_HELP )
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0008) ) );
//			g_WndMng.OpenMessageBox( "������ �ƴϰų� ��ȸ�ش��� �ƴϹǷ� �����Ҽ� �����ϴ�." );
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

/****************************************************
  WndId : APP_PARTY_INFO - ���κй�
  CtrlId : WIDC_GROUPBOX1 - �ش�����
  CtrlId : WIDC_GROUPBOX3 - ����ġ�й�
  CtrlId : WIDC_GROUPBOX4 - �����ۺй�
  CtrlId : WIDC_ITEM_SHARE - ���κй�
  CtrlId : WIDC_RADIO2 - �����й�
  CtrlId : WIDC_RADIO3 - �����й�
  CtrlId : WIDC_RADIO4 - �����й�
  CtrlId : WIDC_EXP_SHARE - ���Ϻй�
  CtrlId : WIDC_RADIO6 - �⿩���й�
  CtrlId : WIDC_RADIO7 - ���غй�
  CtrlId : WIDC_STATIC1 - �شܸ�Ī :
  CtrlId : WIDC_STATIC2 - �شܷ��� :
  CtrlId : WIDC_STATIC3 - �شܰ���ġ :
  CtrlId : WIDC_STATIC4 - �ش�����Ʈ :
  CtrlId : WIDC_NAME - ����Ŀ���̵�
  CtrlId : WIDC_LEVEL - 1
  CtrlId : WIDC_EXP - 90
  CtrlId : WIDC_POINT - 0
****************************************************/

CWndPartyInfo::CWndPartyInfo() 
{ 
	m_nSelected=-1;
	m_pVBGauge = NULL;
} 
CWndPartyInfo::~CWndPartyInfo() 
{ 
	DeleteDeviceObjects();
} 
void CWndPartyInfo::OnDraw( C2DRender* p2DRender ) 
{
	CString strTemp;
	
	// ���� ���õ� �� ǥ��
	if( m_nSelected != -1 ) 
	{
		CRect rect( 5, 8 + m_nSelected * 15, 410, 22 + m_nSelected * 15 );
		p2DRender->RenderFillRect( rect, 0x60ffff00 );
	}
	// ��Ƽ�� ���� ���
	if( g_Party.GetSizeofMember() < m_nSelected + 1 ) 
	{
		m_nSelected = -1;
	}
	int y = 10;

#if __VER >= 8 //__CSC_VER8_2
	DWORD dwColor;
	for( int i = 0; i < g_Party.m_nSizeofMember; i++ ) 
	{		
		dwColor = 0xff000000;
#else 
	DWORD dwColor = 0xff000000;
	for( int i = 0; i < g_Party.m_nSizeofMember; i++ ) 
	//for( int i = 0; i < 8; i++ )//g_Party.m_nSizeofMember; i++ ) 
	{
#endif //__CSC_VER8_2
		CMover* pObjMember	= prj.GetUserByID( g_Party.m_aMember[ i ].m_uPlayerId );

		CRect rectTemp,rect;
		CString strClass(prj.GetText(TID_GAME_MEMBER));
		CString strName;
		if( i == 0 ) strClass = prj.GetText(TID_GAME_LEADER); // �� �������� ����
		rectTemp = CRect( 10, y, 20, y + 10 );
		p2DRender->RenderFillRect( rectTemp, dwColor );
		rectTemp = CRect( 11, y + 1, 19, y + 9 );

		// ���¿� ���� �� ����
		DWORD colorStatus = 0xff0000ff; // ����Ʈ�� �̻����
		if( IsValidObj(pObjMember) )
		{
			if( pObjMember->GetHitPoint() == 0 ) 
				colorStatus = 0xffff0000; // ������
			else 
			{
				if( pObjMember->GetMaxHitPoint() > 0 && ( (FLOAT)pObjMember->GetHitPoint() ) / ( (FLOAT)pObjMember->GetMaxHitPoint() ) <.1f ) 
					colorStatus = 0xffffff00; // HP 10% �����γ�
			}
		}
		else
		{
			colorStatus = 0xff878787; // ����Ʈ�� ������ ���³�
			if( g_Party.m_aMember[ i ].m_bRemove ) 
				colorStatus = 0xff000000; // ������ ���³�
		}
		p2DRender->RenderFillRect( rectTemp, colorStatus );

#if __VER >= 8 //__CSC_VER8_2
		u_long  nLeadMember = g_Party.m_aMember[i].m_uPlayerId;
		if(g_Party.IsLeader(nLeadMember)) //Leader Color Set
			dwColor = 0xff1fb72d;
#endif	//__CSC_VER8_2	
		BOOL bSummary	= FALSE;
		int nLang	= ::GetLanguage();
		switch( nLang )
		{
			case LANG_GER:
			case LANG_FRE:
				bSummary	= TRUE;
				break;
		}

		// ���� ��� ����
		if( IsValidObj(pObjMember) )
		{
#if __VER >= 10 // __LEGEND
			if( MAX_PROFESSIONAL <= pObjMember->GetJob() && pObjMember->GetJob() < MAX_MASTER )
				strTemp.Format( "%d%s", pObjMember->GetLevel(), prj.GetText( TID_GAME_TOOLTIP_MARK_MASTER ) );
			else if( MAX_MASTER <= pObjMember->GetJob() )
#if __VER >= 15 // __HERO129_VER15 // 15�� ����� ����Ȯ��
				strTemp.Format( "%d%s", pObjMember->GetLevel(), prj.GetText( TID_GAME_TOOLTIP_MARK_HERO ) );
#else // 15�� ����� ����Ȯ��
				strTemp = prj.GetText( TID_GAME_TOOLTIP_MARK_HERO_BEFORE );
#endif // 15�� ����� ����Ȯ��
			else 
				strTemp.Format( "%d", pObjMember->GetLevel() );
			p2DRender->TextOut( 30, y, 1.0f, 1.0f, strTemp, dwColor );
			if( !bSummary )
			{
				strTemp.Format( "%s", strClass );
				p2DRender->TextOut( 80, y, 1.0f, 1.0f, strTemp, dwColor );
				strTemp.Format( "%s", pObjMember->GetJobString() );
				p2DRender->TextOut( 120, y, 1.0f, 1.0f, strTemp, dwColor );
			}
			else
			{
				strTemp.Format( "%s", pObjMember->GetJobString() );
				p2DRender->TextOut( 80, y, 1.0f, 1.0f, strTemp, dwColor );
			}
			strName		= pObjMember->GetName();
#else //__LEGEND
			strTemp.Format( "%d", pObjMember->GetLevel() );
			p2DRender->TextOut( 30, y, 1.0f, 1.0f, strTemp, dwColor );
			if( !bSummary )
			{
				strTemp.Format( "%s", strClass );
				p2DRender->TextOut( 50, y, 1.0f, 1.0f, strTemp, dwColor );
				strTemp.Format( "%s", pObjMember->GetJobString() );
				p2DRender->TextOut( 80, y, 1.0f, 1.0f, strTemp, dwColor );
			}
			else
			{
				strTemp.Format( "%s", pObjMember->GetJobString() );
				p2DRender->TextOut( 50, y, 1.0f, 1.0f, strTemp, dwColor );
			}
			strName = pObjMember->GetName();
#endif //__LEGEND
		}
		else
		{
#if __VER >= 10 // __LEGEND
#if __VER >= 11 // __SYS_PLAYER_DATA
			PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( g_Party.m_aMember[i].m_uPlayerId );
			int nLevel	= pPlayerData->data.nLevel;
			int nJob	= pPlayerData->data.nJob;
#else	// __SYS_PLAYER_DATA
			int nLevel	= g_Party.m_aMember[i].m_nLevel;
			int nJob	= g_Party.m_aMember[i].m_nJob;
#endif	// __SYS_PLAYER_DATA
			if( MAX_PROFESSIONAL <= nJob && nJob < MAX_MASTER )
				strTemp.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_MASTER ) );
			else if( MAX_MASTER <= nJob )
#if __VER >= 15 // __HERO129_VER15 // 15�� ����� ����Ȯ��
				strTemp.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_HERO ) );
#else // 15�� ����� ����Ȯ��
				strTemp = prj.GetText( TID_GAME_TOOLTIP_MARK_HERO_BEFORE );
#endif // 15�� ����� ����Ȯ��
			else 
				strTemp.Format( "%d", nLevel );
			p2DRender->TextOut(30,y,1.0f,1.0f,strTemp,dwColor);
			if( !bSummary )
			{
				strTemp.Format("%s",strClass);
				p2DRender->TextOut(80,y,1.0f,1.0f,strTemp,dwColor);
				strTemp.Format("%s",prj.m_aJob[nJob].szName);
				p2DRender->TextOut(120,y,1.0f,1.0f,strTemp,dwColor);
			}
			else
			{
				strTemp.Format( "%s", prj.m_aJob[nJob].szName );
				p2DRender->TextOut( 80, y, 1.0f, 1.0f, strTemp, dwColor );
			}
#if __VER >= 11 // __SYS_PLAYER_DATA
			strName		= pPlayerData->szPlayer;
#else	// __SYS_PLAYER_DATA
			strName		= g_Party.m_aMember[i].m_szName;
#endif	// __SYS_PLAYER_DATA
#else //__LEGEND
			strTemp.Format("%d",g_Party.m_aMember[i].m_nLevel);
			p2DRender->TextOut(30,y,1.0f,1.0f,strTemp,dwColor);
			if( !bSummary )
			{
				strTemp.Format("%s",strClass);
				p2DRender->TextOut(50,y,1.0f,1.0f,strTemp,dwColor);
				strTemp.Format("%s",prj.m_aJob[g_Party.m_aMember[i].m_nJob].szName);
				p2DRender->TextOut(80,y,1.0f,1.0f,strTemp,dwColor);
			}
			else
			{
				strTemp.Format( "%s", prj.m_aJob[g_Party.m_aMember[i].m_nJob].szName );
				p2DRender->TextOut( 50,y,1.0f,1.0f,strTemp,dwColor );
			}
			strName		= g_Party.m_aMember[i].m_szName;
#endif //__LEGEND
		}
		// �̸��� 10����Ʈ°���� ¥����
		if( strName.GetLength() > 8 ) 
		{
			int	nReduceCount = 0;

			for( nReduceCount= 0; nReduceCount < 8; )
			{
				if( IsDBCSLeadByte( strName[ nReduceCount ] ) )
					nReduceCount+=2;
				else
					nReduceCount++;
			}

			strName = strName.Left( nReduceCount ); // 10����Ʈ°���� �ѱ��� �ε������Ű����� 9����Ʈ���� ¥����
			strName+="..."; // �̸� ©������ �̰� ���� �ƴ϶�� ǥ������... ��ȹ���� ������ ���ڴ�� �ٿ���
		}
		strTemp.Format("%s",strName);
#if __VER >= 10		// __LEGEND
		p2DRender->TextOut( 180, y, 1.0f, 1.0f, strTemp, dwColor );
#else //__LEGEND
		p2DRender->TextOut( 135, y, 1.0f, 1.0f, strTemp, dwColor );	// 50
#endif //__LEGEND
		
		// HP ������
		int nWidth	= ( IsValidObj( pObjMember ) ? 110 * pObjMember->GetHitPoint() / pObjMember->GetMaxHitPoint() : 0 );
		if( !bSummary )
			rect.SetRect( 280, y, 280 + 110, y + 12 );
		else
			rect.SetRect( 310, y, 310 + 110, y + 12 );	// ���������� 30 �̵�

		rectTemp = rect; 
		rectTemp.right = rectTemp.left + nWidth;
		//rectTemp.DeflateRect( 1, 1 );
		//if( rectTemp.Width() > 5 )
		//	m_pTheme->RenderWndGauge( p2DRender, &rectTemp, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
		//m_pTheme->RenderWndGauge( p2DRender, &rect,  D3DCOLOR_ARGB( 100, 255, 255, 255 )  );
		m_pTheme->RenderGauge( p2DRender, &rect, 0xffffffff, m_pVBGauge, &m_texGauEmptyNormal );
		if( IsValidObj(pObjMember) )
			m_pTheme->RenderGauge( p2DRender, &rectTemp, 0x64ff0000, m_pVBGauge, &m_texGauFillNormal );
	
		y+=15; // ������
	}
} 
HRESULT CWndPartyInfo::RestoreDeviceObjects()
{
	CWndNeuz::RestoreDeviceObjects();
	if( m_pVBGauge == NULL )
		return m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL );
	return S_OK;
}
HRESULT CWndPartyInfo::InvalidateDeviceObjects()
{
	CWndNeuz::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVBGauge );
	return S_OK;
}
HRESULT CWndPartyInfo::DeleteDeviceObjects()
{
	CWndNeuz::DeleteDeviceObjects();
	InvalidateDeviceObjects();
	return S_OK;
}

void CWndPartyInfo::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���

	RestoreDeviceObjects();
	m_texGauEmptyNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "GauEmptyNormal.bmp" ), 0xffff00ff, TRUE );
	m_texGauFillNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "GauEmptyNormal.bmp" ), 0xffff00ff, TRUE );
	
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyInfo::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY_INFO, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndPartyInfo::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndPartyInfo::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPartyInfo::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndPartyInfo::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndPartyInfo::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
#if __VER >= 11 // __CSC_VER11_2
	CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
	if(((CWndWorld*)g_WndMng.m_pWndWorld)->m_bAutoAttack || pTaskBar->m_nExecute != 0)
		return;
#endif //__CSC_VER11_2
	
	// ��Ƽâ���� ��� �������� ����
	CRect rect( 10, 10, 365, 10 + g_Party.GetSizeofMember() * 15 );
	if( rect.PtInRect( point ) ) 
		m_nSelected = ( point.y - 10 ) / 15;
	else 
		m_nSelected = -1;
	if( m_nSelected != -1 ) 
	{ // ���õȳ� ������
		// �׳��� Ÿ������
#if __VER >= 8 //__CSC_VER8_2
		((CWndWorld*)g_WndMng.m_pWndWorld)->m_pSelectRenderObj = NULL; //���콺�� �ɷ� �׷������� ����� Ÿ���� ��ƾ� �Ȱ�ģ��..
#endif //__CSC_VER8_2
		CMover* pObjMember = prj.GetUserByID( g_Party.m_aMember[m_nSelected].m_uPlayerId );
		if(g_pPlayer!=pObjMember) 
		{
			if(IsValidObj(pObjMember)) 
			{ // ȭ�鿡 ���³��� �н�...
				g_WorldMng()->SetObjFocus(pObjMember);
				CWndWorld* pWndWorld = g_WndMng.m_pWndWorld;
				if(pWndWorld)
					pWndWorld->m_pRenderTargetObj = NULL;
			}
		}
#if __VER >= 8 //__CSC_VER8_2
		else
			g_WorldMng()->SetObjFocus( NULL );
#endif //__CSC_VER8_2
	}
	else 
	{
		// ������ Ÿ�� �����
		g_WorldMng()->SetObjFocus(NULL);
	}
} 
BOOL CWndPartyInfo::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/****************************************************
  WndId : APP_PARTY_SKILL - �شܽ�ų
****************************************************/

CWndPartySkill::CWndPartySkill() 
{ 
	ZeroMemory( m_atexSkill, sizeof( m_atexSkill ) );
	m_nSkillSelect = -1;
} 
CWndPartySkill::~CWndPartySkill() 
{ 
} 
void CWndPartySkill::OnDraw( C2DRender* p2DRender ) 
{ 
//	if( g_Party.m_nKindTroup )
	{
		CRect rect = GetClientRect();
		int nWidth = rect.Width() / 3;
		int nHeight = rect.Height() / 3;
		int nCount = 0;
		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 3; j++, nCount++ )
			{
				ItemProp* pItemProp =  prj.GetPartySkill( nCount + 1 );
				if( pItemProp ) //&& g_Party.m_nSizeofMember >= 2 )
				{
					m_atexSkill[ i * 3 + j ] = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pItemProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
					CPoint point1 = CPoint( j * nWidth + 35 , i * nHeight + 6 );
					CPoint point2 = CPoint( j * nWidth + 3, i * nHeight + 3 );
					
					if( g_Party.m_nKindTroup && int(g_Party.GetLevel() - pItemProp->dwReqDisLV) >= 0 )
					{
						if( (g_Party.GetPoint() - pItemProp->dwExp) >= 0 )
						{
							// ��밡���� ���
							p2DRender->TextOut( point1.x, point1.y, pItemProp->szName, 0xff000000 );
							//m_aSlotQueue[nIndex].m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
							p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ] );
							
						}
						else
						{
							// ����� �������� ����Ʈ�� ���ڶ� ����� ����
							p2DRender->TextOut( point1.x, point1.y, pItemProp->szName, 0xff0000ff );
							p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ] );
							
						}
					}
					else
					{
						// ����� ��������
						p2DRender->TextOut( point1.x, point1.y, pItemProp->szName, 0xffff0000 );
						p2DRender->RenderTexture( point2, m_atexSkill[ i * 3 + j ], 54 );		
					}
				}
			}
		}
	}
} 
void CWndPartySkill::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
	CRect rect = GetClientRect();
	int nWidth = rect.Width() / 3;
	int nHeight = rect.Height() / 3;
	point.x /= nWidth; 
	point.y /= nHeight;
	int nSkill = point.y * 3 + point.x;
	
	ItemProp* pItemProp =  prj.GetPartySkill( nSkill + 1 );
	if( pItemProp )
	{
		m_nSkillSelect = nSkill;
		return;
	}
	m_nSkillSelect = -1;
} 
void CWndPartySkill::OnMouseWndSurface( CPoint point )
{
	CRect rect = GetClientRect();
	int nWidth = rect.Width() / 3;
	int nHeight = rect.Height() / 3;
	int nCount = 0;
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++, nCount++ )
		{
			ItemProp* pItemProp =  prj.GetPartySkill( nCount + 1 );
			if( pItemProp ) //&& g_Party.m_nSizeofMember >= 2 )
			{
				CRect rect( j * nWidth + 3, i * nHeight + 3, j * nWidth + 3 + nWidth, i * nHeight + 3 + nHeight);
				if( rect.PtInRect( point ) )
				{
					CPoint point2 = point;
					ClientToScreen( &point2 );
					ClientToScreen( &rect );
					
					g_WndMng.PutToolTip_Troupe( i * 3 + j + 1, point2, &rect );
				}
			}
		}
	}
}	
void CWndPartySkill::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	m_nSkillSelect = -1;
}

void CWndPartySkill::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_nSkillSelect == -1 || IsPush() == FALSE )
		return;
	//	TRACE( " hello \n" );
	CPoint pt( 3, 3 );
	CRect rect;
	//if( m_rect.PtInRect( point ) )
	//{
	//DWORD dwSkill = m_apJobSkill[ m_nCurSelect ].dwSkill;
	ItemProp* pItemProp =  prj.GetPartySkill( m_nSkillSelect + 1 );
	if( pItemProp ) //&& g_Party.m_nSizeofMember >= 2 )
	{
		if( int(g_Party.GetLevel() - pItemProp->dwReqDisLV) >= 0 )
		{
			//CItemElem* pItemElem = &m_pItemContainer->m_apItem[ m_pItemContainer->m_apIndex[ m_nCurSelect ] ] ;
			m_GlobalShortcut.m_dwShortcut  = SHORTCUT_SKILL;
			m_GlobalShortcut.m_dwType  = 2;//m_nJob;//(DWORD)pItemElem;//->m_dwItemId;
			m_GlobalShortcut.m_dwIndex = m_nSkillSelect + 1;//m_nSkillSelect;//m_nCurSelect;//(DWORD)pItemElem;//->m_dwItemId;
			m_GlobalShortcut.m_dwData  = 0;//pItemElem->m_dwObjId;//(DWORD)pItemElem;
			m_GlobalShortcut.m_dwId     = m_nSkillSelect + 1; // �÷�Ʈ ����Ʈ�� �� ID�� ����������.
			m_GlobalShortcut.m_pTexture = m_atexSkill[ m_nSkillSelect ];//L;//pItemElem->m_pTexture;
			_tcscpy( m_GlobalShortcut.m_szString, pItemProp->szName);
			//}
		}
	}
}

void CWndPartySkill::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartySkill::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY_SKILL, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndPartySkill::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndPartySkill::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPartySkill::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndPartySkill::OnLButtonDblClk( UINT nFlags, CPoint point)
{
//	if( g_Party.m_nSizeofMember >= 2 && g_Party.m_nKindTroup == 1 )
	{
		CRect rect = GetClientRect();
		int nWidth = rect.Width() / 3;
		int nHeight = rect.Height() / 3;
		point.x /= nWidth; 
		point.y /= nHeight;
		int nSkill = point.y * 3 + point.x;
		
		ItemProp* pItemProp =  prj.GetPartySkill( nSkill + 1 );
		if( pItemProp )
		{
			g_WndMng.ObjectExecutor( SHORTCUT_SKILL, nSkill + 1, 2 );
		}
	}
}
BOOL CWndPartySkill::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
