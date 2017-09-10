#include "stdafx.h"
#include "ResData.h"
#include "WndGuildWarPeace.h"
#include "dpclient.h"

extern	CDPClient	g_DPlay;

/****************************************************
  WndId : APP_GUILD_WARPEACE - ����â
  CtrlId : WIDC_YES - Yes
  CtrlId : WIDC_NO - No
  CtrlId : WIDC_STATIC1 - ����忡 ������ ��û�ϰڽ��ϱ�?
****************************************************/

CWndGuildWarPeace::CWndGuildWarPeace() 
{ 
} 
CWndGuildWarPeace::~CWndGuildWarPeace() 
{ 
} 
void CWndGuildWarPeace::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildWarPeace::OnInitialUpdate() 
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
BOOL CWndGuildWarPeace::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_WARPEACE, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndGuildWarPeace::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndGuildWarPeace::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildWarPeace::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildWarPeace::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildWarPeace::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildWarPeace::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID ) 
	{
	case WIDC_YES:
		// ������û�� ������ ����.
		g_DPlay.SendQueryTruce( g_pPlayer->m_idPlayer );
		Destroy();
		break;
	case WIDC_NO:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
