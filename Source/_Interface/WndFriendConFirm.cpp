#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndFriendConFirm.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;

#include "WndManager.h"

/****************************************************
  WndId : APP_FRIEND_CONFIRM - Confirm
****************************************************/

CWndFriendConFirm::CWndFriendConFirm() 
{
	m_uLeader = m_uMember = 0;
	m_nLeaderSex= 0;
	m_szLeaderName[ 0 ] = '\0';
} 
CWndFriendConFirm::~CWndFriendConFirm() 
{ 
} 

void CWndFriendConFirm::SetMember( u_long uLeader, u_long uMember, LONG nLeaderJob, BYTE nLeaderSex, char * szLeadName )
{
	m_uLeader = uLeader;
	m_uMember = uMember;
	m_nLeaderSex = nLeaderSex;
	m_nLeaderJob = nLeaderJob;
	strcpy( m_szLeaderName, szLeadName );
}
void CWndFriendConFirm::OnDraw( C2DRender* p2DRender ) 
{ 
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;
	strTemp.Format(_T( prj.GetText(TID_DIAG_0071) ),m_szLeaderName); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
//	strTemp.Format(_T("%s ���� ģ�� �߰� ��û�� ���Խ��ϴ�. �³��Ͻðڽ��ϱ�?"),m_szLeaderName); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );
} 
void CWndFriendConFirm::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	

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
BOOL CWndFriendConFirm::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_FRIEND_CONFIRM, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndFriendConFirm::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndFriendConFirm::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndFriendConFirm::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndFriendConFirm::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndFriendConFirm::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndFriendConFirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_YES ) 
	{
		// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendAddFriend( m_uLeader, m_nLeaderJob, m_nLeaderSex );
		Destroy();	// �����ı��� �ٲ�. -xuzhu- 09/16
	}
	else 
	if( nID == WIDC_NO )
	{
		// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
		g_DPlay.SendFriendCancel( m_uLeader, m_uMember );
		Destroy();	// �����ı��� �ٲ�. -xuzhu- 09/16
	}
	
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

/****************************************************
  WndId : APP_ADDFRIEND - ģ�� �߰�
  CtrlId : WIDC_STATIC1 - �߰��� �̸��� �Է��ϼ���
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_STATIC2 - ��  �� :
  CtrlId : WIDC_OK - Ȯ��
  CtrlId : WIDC_CANCEL - ���
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
	
	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
	pWndEdit->SetFocus();

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
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_ADDFRIEND, 0, CPoint( 0, 0 ), pWndParent );
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
	if( nID == WIDC_OK ) 
	{
		LPCTSTR szAddName;
		CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
		szAddName = pWndEdit->GetString();
		if( strlen( szAddName ) < MAX_NAME )
		{
			if( strcmp( szAddName, g_pPlayer->GetName() ) != 0 )
			{
				if( g_pPlayer->GetWorld() && g_pPlayer->GetWorld()->GetID() == WI_WORLD_GUILDWAR )
				{
					g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_GUILDCOMBAT_CANNOT_FRIENDADD ) );	// "�����ؾ��� : �������忡�� ģ���߰��� �Ҽ� �����ϴ�" );
				}
				else
				{
					g_DPlay.SendAddFriendNameReqest( szAddName );
					//g_WndMng.PutString( "�޽��� �߰� ��û���Դϴ�. ��ø� ��ٷ��ּ���", NULL, 0xffff0000 );
					CString str;
					str.Format( prj.GetText(TID_GAME_MSGINVATE), szAddName );
					g_WndMng.PutString( str, NULL, prj.GetTextColor(TID_GAME_MSGINVATE) );
				}
				Destroy( );	
			}
			else
			{
				pWndEdit->SetString("");
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0056) ) );
//				g_WndMng.OpenMessageBox( "�ڱ� �ڽ��� �߰��Ҽ� �����ϴ�. �ٽ� �Է����ּ���." );
			}			
		}
		else
		{
			pWndEdit->SetString("");
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0057) ) );
//			g_WndMng.OpenMessageBox( "�̸��� �ʹ� ��ϴ�. �ٽ� �Է����ּ���." );
		}
		// ����ٰ� �¶��ϴ� ó�� �߰��Ͻÿ�
//		g_DPlay.SendAddFriend( m_uLeader, m_nLeaderSex );
	}
	else 
	if( nID == WIDC_CANCEL )
	{
		// ����ٰ� �ź��ϴ� ó�� �߰��Ͻÿ�
//		g_DPlay.SendFriendCancel( m_uLeader, m_uMember );
		Destroy( );
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
