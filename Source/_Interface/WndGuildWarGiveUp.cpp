#include "stdafx.h"
#include "ResData.h"
#include "WndGuildWarGiveUp.h"
#include "dpclient.h"

extern	CDPClient	g_DPlay;


/****************************************************
  WndId : APP_GUILD_WARGIVEUP - �׺�â
  CtrlId : WIDC_YES - Button
  CtrlId : WIDC_NO - Button
  CtrlId : WIDC_STATIC1 - ��������� �׺��� �ϰڽ��ϱ�?
****************************************************/

CWndGuildWarGiveUp::CWndGuildWarGiveUp() 
{ 
} 
CWndGuildWarGiveUp::~CWndGuildWarGiveUp() 
{ 
} 
void CWndGuildWarGiveUp::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildWarGiveUp::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildWarGiveUp::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_WARGIVEUP, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndGuildWarGiveUp::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndGuildWarGiveUp::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildWarGiveUp::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildWarGiveUp::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildWarGiveUp::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildWarGiveUp::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_YES:
		// �׺��ߴٴ°� ������ ����.
		if( g_pPlayer )
			g_DPlay.SendSurrender( g_pPlayer->m_idPlayer );
		Destroy();
		break;
	case WIDC_NO:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
