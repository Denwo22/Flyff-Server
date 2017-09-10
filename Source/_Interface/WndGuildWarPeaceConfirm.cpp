#include "stdafx.h"
#include "ResData.h"
#include "WndGuildWarPeaceConfirm.h"
#include "dpclient.h"

extern	CDPClient	g_DPlay;


/****************************************************
  WndId : APP_GUILD_WARPEACECONFIRM - ��������â
  CtrlId : WIDC_YES - Yes
  CtrlId : WIDC_NO - No
  CtrlId : WIDC_STATIC1 - ������û�� ���Խ��ϴ�.
  CtrlId : WIDC_STATIC2 - �����Ͻðڽ��ϱ�?
****************************************************/

CWndGuildWarPeaceConfirm::CWndGuildWarPeaceConfirm() 
{ 
} 
CWndGuildWarPeaceConfirm::~CWndGuildWarPeaceConfirm() 
{ 
} 
void CWndGuildWarPeaceConfirm::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildWarPeaceConfirm::OnInitialUpdate() 
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
BOOL CWndGuildWarPeaceConfirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_WARPEACECONFIRM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndGuildWarPeaceConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndGuildWarPeaceConfirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildWarPeaceConfirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildWarPeaceConfirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildWarPeaceConfirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildWarPeaceConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch( nID )
	{
	case WIDC_YES:
		// ������ �¶� �Ѵٴ°��� ������ ����.
		if( g_pPlayer )
			g_DPlay.SendAcptTruce( g_pPlayer->m_idPlayer );
		Destroy();
		break;
	case WIDC_NO:
		Destroy();
		break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

