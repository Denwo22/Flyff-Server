#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndDuelConfirm.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;

/****************************************************
  WndId : APP_DUEL_CONFIRM - party confirm
  CtrlId : WIDC_YES - yes
  CtrlId : WIDC_NO - No
  CtrlId : WIDC_STATIC1 - Static
****************************************************/

CWndDuelConfirm::CWndDuelConfirm() 
{ 
	memset( m_szSrc, 0, sizeof(m_szSrc) );
	m_idSrc = NULL_ID;
	m_bDuelParty = FALSE;
} 
CWndDuelConfirm::~CWndDuelConfirm() 
{ 
} 
void CWndDuelConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndDuelConfirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;

	/*
	if( m_bDuelParty == FALSE )
		strTemp.Format( _T("%s ���� ����û�� �Ͽ����ϴ�. ����ϰڽ��ϱ�?"), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	else
		strTemp.Format( _T("%s ���� �شܵ���û�� �Ͽ����ϴ�. ����ϰڽ��ϱ�?"), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	*/
	if( m_bDuelParty == FALSE )
		strTemp.Format( _T( prj.GetText(TID_DIAG_0066) ), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	else
		strTemp.Format( _T( prj.GetText(TID_DIAG_0067) ), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	
	pWndText->SetString( strTemp );

	// ������ �߾����� �ű�� �κ�.
	/*
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
	*/
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( ( rectRoot.right - rectWindow.Width() ) / 2, 70 );
	Move( point );	
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndDuelConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_DUEL_CONFIRM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndDuelConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndDuelConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndDuelConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndDuelConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndDuelConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndDuelConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = FALSE;
	if( m_bDuelParty == FALSE )
	{
		if( nID == WIDC_YES ) 
		{
			// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
			// ������ �¶� �ߴٴ¸޽����� ����
			// �������� ������ �� ĳ���Ϳ��� ��� ����� ���۵ƴٴ°� ����
			// �� Ŭ�󿡼� ������ ����.
			CMover *pSrc = prj.GetMover( m_idSrc );	// ��� ��û��.
			if( IsValidObj( pSrc ) )
			{

#if __VER >= 8 // 8�� ��� 061226 ma
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
				CWndBase* pWndBase = g_WndMng.GetWndBase( APP_DIALOG_EX );
#else // __IMPROVE_QUEST_INTERFACE
				CWndBase* pWndBase = g_WndMng.GetWndBase( APP_DIALOG );
#endif // __IMPROVE_QUEST_INTERFACE
				if( pWndBase ) 
					pWndBase->Destroy();

				if( g_pPlayer->m_vtInfo.GetOther() )
				{
					if( g_pPlayer->m_vtInfo.GetOther()->IsPlayer() )	
						g_DPlay.SendTradeCancel();

						g_DPlay.SendCloseShopWnd();
						g_pPlayer->m_vtInfo.SetOther( NULL );
				}
			
				pWndBase	= g_WndMng.GetWndBase( APP_INVENTORY );
				if( pWndBase )
				{
					((CWndInventory*)pWndBase)->BaseMouseCursor();
				}	

				pWndBase	= g_WndMng.GetWndVendorBase();
				if( pWndBase )
				{
					pWndBase->Destroy();
					pWndBase	= g_WndMng.GetWndBase( APP_INVENTORY );
					if( pWndBase )
						pWndBase->Destroy();
				}

				pWndBase	= g_WndMng.GetWndBase( APP_SHOP_ );
				if( pWndBase )
				{
					pWndBase->Destroy();
					pWndBase = g_WndMng.GetWndBase( APP_TRADE_GOLD );
					if( pWndBase )	
						pWndBase->Destroy();
					pWndBase = g_WndMng.GetWndBase( APP_INVENTORY );
					if( pWndBase ) 
						pWndBase->Destroy();
				}
#if __VER >= 8 //__CSC_VER8_4
				pWndBase	= g_WndMng.GetWndBase( APP_BEAUTY_SHOP_SKIN );
				if( pWndBase )
				{
					pWndBase->Destroy();
				
					pWndBase	= g_WndMng.GetWndBase( APP_INVENTORY );
				
					if( pWndBase )
						pWndBase->Destroy();
				}
				pWndBase	= g_WndMng.GetWndBase( APP_BEAUTY_SHOP_EX );
#else
				pWndBase	= g_WndMng.GetWndBase( APP_BEAUTY_SHOP );
#endif //__CSC_VER8_4
				if( pWndBase )
				{
					pWndBase->Destroy();

					pWndBase	= g_WndMng.GetWndBase( APP_INVENTORY );

					if( pWndBase )
						pWndBase->Destroy();
				}
				pWndBase	= g_WndMng.GetWndBase( APP_COMMON_BANK );
				if( pWndBase )
				{
					g_DPlay.SendCloseBankWnd();
					pWndBase->Destroy();
					pWndBase = g_WndMng.GetWndBase( APP_TRADE_GOLD );
					if( pWndBase )	
						pWndBase->Destroy();
					pWndBase = g_WndMng.GetWndBase( APP_INVENTORY );
					if( pWndBase )	
						pWndBase->Destroy();
				}
				pWndBase	= g_WndMng.GetWndBase( APP_REPAIR );
				if( pWndBase )
				{
					pWndBase->Destroy();
					pWndBase = g_WndMng.GetWndBase( APP_INVENTORY );
					if( pWndBase )	pWndBase->Destroy();
				}
				pWndBase	= g_WndMng.GetWndBase( APP_BANK_PASSWORD );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_CONFIRM_BANK );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_DROP_CONFIRM );
				if( pWndBase )
					pWndBase->Destroy();

				pWndBase	= g_WndMng.GetWndBase( APP_GUILD_BANK );
				if( pWndBase )
				{
					g_DPlay.SendCloseGuildBankWnd();
					pWndBase->Destroy();
				}
				pWndBase	= g_WndMng.GetWndBase( APP_RANK_WAR );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_RANK_INFO );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_RANK_GUILD );
				if( pWndBase )
					pWndBase->Destroy();

				pWndBase	= g_WndMng.GetWndBase( APP_PIERCING );
				if( pWndBase )
					pWndBase->Destroy();
			
				pWndBase	= g_WndMng.GetWndBase( APP_TEST );
				if( pWndBase )
					pWndBase->Destroy();
				
				pWndBase	= g_WndMng.GetWndBase( APP_COMMERCIAL_ELEM );
				if( pWndBase )
					pWndBase->Destroy();
				
				pWndBase	= g_WndMng.GetWndBase( APP_REMOVE_ELEM );
				if( pWndBase )
					pWndBase->Destroy();

				pWndBase	= g_WndMng.GetWndBase( APP_CHANGECLASS_1 );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_CHANGECLASS_2 );
				if( pWndBase )
					pWndBase->Destroy();
				
				pWndBase	= g_WndMng.GetWndBase( APP_QUERYEQUIP );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_POST );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_POST_SEND );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_POST_RECEIVE );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_POST_READ );
				if( pWndBase )
					pWndBase->Destroy();
				
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_RANK_P );
				if( pWndBase )
					pWndBase->Destroy();			
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_RANKING );
				if( pWndBase )
					pWndBase->Destroy();

				pWndBase	= g_WndMng.GetWndBase( APP_GUILD_WAR_STATE );
				if( pWndBase )
					pWndBase->Destroy();

				pWndBase	= g_WndMng.GetWndBase( APP_CLOSE_EXISTING_CONNECTION );
				if( pWndBase )
					pWndBase->Destroy();
#if __VER < 11 // __GUILD_COMBAT_1TO1
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_OFFER );
				if( pWndBase )
					pWndBase->Destroy();
#endif //__GUILD_COMBAT_1TO1
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_SELECTION );
				if( pWndBase )
					pWndBase->Destroy();
#if __VER < 11 // __GUILD_COMBAT_1TO1

				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_OFFERSTATE );
				if( pWndBase )
					pWndBase->Destroy();
#endif //__GUILD_COMBAT_1TO1
#if __VER >= 11 // __GUILD_COMBAT_1TO1
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_1TO1_SELECTION );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_1TO1_OFFERSTATE );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_GUILDCOMBAT_1TO1_OFFER );
				if( pWndBase )
					pWndBase->Destroy();
#endif //__GUILD_COMBAT_1TO1
#if __VER >= 12 // __SECRET_ROOM
				pWndBase	= g_WndMng.GetWndBase( APP_SECRETROOM_SELECTION );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_SECRETROOM_OFFER );
				if( pWndBase )
					pWndBase->Destroy();
				pWndBase	= g_WndMng.GetWndBase( APP_SECRETROOM_OFFERSTATE );
				if( pWndBase )
					pWndBase->Destroy();
#endif //__SECRET_ROOM

#endif // __VER >= 8 // 8�� ��� 061226 ma
				g_DPlay.SendDuelYes( pSrc, g_pPlayer );		// ��� ��û �����.
			}
			bResult = TRUE;
		}
		else if( nID == WIDC_NO || nID == WTBID_CLOSE ) 
		{
			// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
			CMover *pSrc = prj.GetMover( m_idSrc );	// ��� ��û��.
			if( IsValidObj( pSrc ) )
			{
				g_DPlay.SendDuelNo( pSrc );		// pSrc���� �ź��ߴٴ°� �˸�.
			}
			bResult = TRUE;
		}
	} else
	// ��Ƽ ���.
	{
		if( nID == WIDC_YES ) 
		{
			// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
			// ������ �¶� �ߴٴ¸޽����� ����
			// �������� ������ �� ĳ���Ϳ��� ��� ����� ���۵ƴٴ°� ����
			// �� Ŭ�󿡼� ������ ����.
			CMover *pSrc = prj.GetMover( m_idSrc );	// ��� ��û��.
			if( IsValidObj( pSrc ) )
			{
				g_DPlay.SendDuelPartyYes( pSrc, g_pPlayer );		// ��� ��û �����.
			}
			bResult = TRUE;
		}
		else if( nID == WIDC_NO || nID == WTBID_CLOSE ) 
		{
			// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
			CMover *pSrc = prj.GetMover( m_idSrc );	// ��� ��û��.
			if( IsValidObj( pSrc ) )
			{
				g_DPlay.SendDuelPartyNo( pSrc );		// pSrc���� �ź��ߴٴ°� �˸�.
			}
			bResult = TRUE;
		}
	}
	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#if __VER >= 8     // 8�� ������� ������� PVP�����ϰ���   Neuz, World

CWndDuelResult::CWndDuelResult() 
{ 
	memset( m_szSrc, 0, sizeof(m_szSrc) );
	m_idSrc = NULL_ID;
	m_bDuelWin = FALSE;
} 
CWndDuelResult::~CWndDuelResult() 
{ 
} 
void CWndDuelResult::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndDuelResult::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;

	if( m_bDuelWin == FALSE )
		strTemp.Format( _T( prj.GetText(TID_DIAG_DUELRESULT_LOSE) ), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	else
		strTemp.Format( _T( prj.GetText(TID_DIAG_DUELRESULT_WIN) ), m_szSrc ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	
	pWndText->SetString( strTemp );

	// ������ �߾����� �ű�� �κ�.
	/*
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
	*/
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( ( rectRoot.right - rectWindow.Width() ) / 2, 70 );
	Move( point );	
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndDuelResult::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_DUEL_RESULT, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndDuelConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndDuelResult::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndDuelResult::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndDuelResult::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndDuelResult::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndDuelResult::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = TRUE;

	if( m_bDuelWin  ) 
	{
		if(g_pPlayer->SendActMsg( OBJMSG_MOTION, MTI_HIPKIDDING, ANILOOP_1PLAY ) == 1 )
			g_DPlay.SendPlayerBehavior();
	}
	else 
	{
		if(g_pPlayer->SendActMsg( OBJMSG_MOTION, MTI_HEAD, ANILOOP_CONT ) == 1 )
			g_DPlay.SendPlayerBehavior();
	}

	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#endif	// __VER >= 8  
