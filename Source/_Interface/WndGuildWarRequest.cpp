#include "stdafx.h"
#include "ResData.h"
#include "DefineText.h"
#include "WndGuildWarRequest.h"

#include "dpclient.h"

extern	CDPClient	g_DPlay;

/****************************************************
  WndId : APP_GUILD_WARREQUEST - �������ûâ
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_YES - Button
  CtrlId : WIDC_NO - No
****************************************************/

CWndGuildWarRequest::CWndGuildWarRequest() 
{ 
	m_idEnemyGuild = 0;
} 
CWndGuildWarRequest::~CWndGuildWarRequest() 
{ 
	m_idEnemyGuild = 0;
} 
void CWndGuildWarRequest::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildWarRequest::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CGuild *pEnemyGuild = g_GuildMng.GetGuild( m_idEnemyGuild );
	if( pEnemyGuild )
	{
		CWndEdit *pWndEdit = (CWndEdit *)GetDlgItem( WIDC_EDIT1 );

		CString strText;
		strText.Format( prj.GetText( TID_GAME_GUILDWARREQUEST ), pEnemyGuild->m_szGuild, m_szMaster );
		pWndEdit->SetString( strText );
		pWndEdit->EnableWindow( FALSE );
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildWarRequest::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_WARREQUEST, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndGuildWarRequest::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndGuildWarRequest::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildWarRequest::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildWarRequest::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildWarRequest::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildWarRequest::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_YES:
		// ������ ������.
		g_DPlay.SendAcptWar( g_pPlayer->m_idPlayer, m_idEnemyGuild );
		Destroy();
		break;
	case WIDC_NO:
		Destroy();
		break;
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

