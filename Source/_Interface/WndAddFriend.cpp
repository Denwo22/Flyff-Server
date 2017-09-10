#include "stdafx.h"
#include "WndAddFriend.h"


/****************************************************
  WndId : APP_ADD_FRIEND - Add Friend
  CtrlId : WIDC_STATIC1 - ģ�� �̸�
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_OK - Ok
  CtrlId : WIDC_CANCEL - Cancel
****************************************************/

CWndAddFriend::CWndAddFriend() 
{ 
} 
CWndAddFriend::~CWndAddFriend() 
{ 
} 
void CWndAddFriend::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndAddFriend::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndEdit* pWndName = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );
	pWndName->SetFocus();

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndAddFriend::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_ADD_FRIEND, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndAddFriend::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndAddFriend::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	switch( nID )
	{
	case WIDC_OK:
		break;
	case WIDC_CANCEL:
		Destroy();
		break;
	}
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndAddFriend::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndAddFriend::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndAddFriend::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndAddFriend::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

