
#include "stdafx.h"
#include "resData.h"
#include "WndQuitRoom.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if __VER >= 13 // __HOUSING
/****************************************************
  WndId : APP_QUIT_ROOM - �̴Ϸ�
  CtrlId : WIDC_STATIC1 - �̴Ϸ뿡�� ���� �Ͻðڽ��ϱ�?
  CtrlId : WIDC_BUTTON1 - Button
  CtrlId : WIDC_BUTTON2 - Button
****************************************************/

CWndQuitRoom::CWndQuitRoom() 
{ 
} 
CWndQuitRoom::~CWndQuitRoom() 
{ 
} 
void CWndQuitRoom::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndQuitRoom::OnInitialUpdate() 
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
BOOL CWndQuitRoom::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_QUIT_ROOM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndQuitRoom::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndQuitRoom::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndQuitRoom::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndQuitRoom::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndQuitRoom::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndQuitRoom::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch(nID)
	{
		case WIDC_BUTTON1:// ok ��ư
			g_DPlay.SendHousingGoOut();
			Destroy();
			break;

		case WIDC_BUTTON2:// cancel ��ư
			Destroy();
			break;
	};
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#endif // __HOUSING