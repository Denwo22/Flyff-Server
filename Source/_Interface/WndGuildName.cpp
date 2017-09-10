#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndGuildName.h"
#include "WndManager.h"
#include "DPClient.h"
extern	CDPClient	g_DPlay;


/****************************************************
  WndId : APP_GUILDNAME - ������â
  CtrlId : WIDC_EDIT1 - 
  CtrlId : WIDC_STATIC1 - ����Ī�� �Է����ּ���.
  CtrlId : WIDC_OK - Button
  CtrlId : WIDC_CANCEL - Button
****************************************************/

CWndGuildName::CWndGuildName() 
{ 
	m_nId	= 0xff;
} 
CWndGuildName::~CWndGuildName() 
{ 
} 

void CWndGuildName::OnDraw( C2DRender* p2DRender ) 
{ 
#ifdef __S_SERVER_UNIFY
	if( g_WndMng.m_bAllAction == FALSE )
	{
		CRect rectRoot = m_pWndRoot->GetLayoutRect();
		CRect rectWindow = GetWindowRect();
		CPoint point( rectRoot.right - rectWindow.Width(), 110 );
		Move( point );
		MoveParentCenter();		
	}
#endif // __S_SERVER_UNIFY
} 
void CWndGuildName::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
	{
		CWndEdit *pWndEdit = (CWndEdit *)GetDlgItem( WIDC_EDIT1 );
		pWndEdit->SetString( pGuild->m_szGuild );		// ����Ʈ �̸��� ����Ʈ �ڽ��� �Է���.
	}
	else
	{
		Destroy();
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndGuildName::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILDNAME, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndGuildName::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildName::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildName::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildName::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildName::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_OK )
	{
		CWndEdit *pWndEdit = (CWndEdit *)GetDlgItem( WIDC_EDIT1 );
		LPCTSTR szName = pWndEdit->GetString();

		// �̰����� szName�� ������ ������ �ڵ带 �����ÿ�.
		CString strGuild = szName;
		strGuild.TrimLeft();
		strGuild.TrimRight();
		CHAR c = strGuild.GetAt( 0 );
		
		int nLength = strGuild.GetLength();
#ifdef __RULE_0516
		if( nLength < 6 || nLength > 24 )
#else	// __RULE_0516
		if( nLength < 3 || nLength > 32 )
#endif	// __RULE_0516
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0011) ) );
			//				g_WndMng.OpenMessageBox( _T( "��Ī�� 3���� �̻�, 16���� ���Ϸ� �Է� �Է��Ͻʽÿ�." ) );
			return TRUE;
		}
		else
		if( isdigit2( c ) && !IsDBCSLeadByte( strGuild.GetAt( 0 ) ) )
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0012) ) );
			//				g_WndMng.OpenMessageBox( _T( "��Ī�� ù���ڸ� ���ڷ� ����� �� �����ϴ�." ) );
			return TRUE;
		}
		else
		for( int i = 0; i < strGuild.GetLength(); i++ )
		{
			c = strGuild.GetAt( i );
			// ���ڳ� ���ĺ��� �ƴ� ���� �ǽ�����.
			if( IsDBCSLeadByte( c ) == TRUE ) 
			{
				CHAR c2 = strGuild.GetAt( ++i );
				WORD word = ( ( c << 8 ) & 0xff00 ) | ( c2 & 0x00ff );
				if( ::GetLanguage() == LANG_KOR )
				{
					if( IsHangul( word ) == FALSE ) 
					{
						g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0014) ) );
						return TRUE;
					}
				}
			}
			else
			if( !IsCyrillic( c ) && ( isalnum( c ) == FALSE || iscntrl( c ) )  )
			{
				// Ư�� ���ڵ� �ƴϴ� (�� ��Ʈ�� �Ǵ� !@#$%^&**()... ������)
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0013) ) );
				return TRUE;
			}
		}

		if( prj.IsInvalidName( szName ) 
#ifdef __RULE_0615
			|| prj.IsAllowedLetter( szName ) == FALSE
#endif	// __RULE_0615
			)
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0020) ) );
			return TRUE;
		}

		if( m_nId == 0xff )
			g_DPlay.SendGuildSetName( (LPCSTR)strGuild );
		else
			g_DPlay.SendQuerySetGuildName( (LPCSTR)strGuild, m_nId );

		Destroy();
	}
	else if( nID == WIDC_CANCEL || nID == WTBID_CLOSE )
	{
#ifdef __S_SERVER_UNIFY
		if( g_WndMng.m_bAllAction == FALSE )
			return TRUE;
#endif // __S_SERVER_UNIFY
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


/****************************************************
  WndId : APP_GUILD_NICKNAME - ��庰Ī����
  CtrlId : WIDC_OK - OK
  CtrlId : WIDC_CANCEL - Cancel
  CtrlId : WIDC_STATIC1 - ������ �� ��Ī�� �Է����ּ���.
  CtrlId : WIDC_EDIT1 - 
****************************************************/

CWndGuildNickName::CWndGuildNickName() 
{ 
	m_idPlayer = 0;
} 
CWndGuildNickName::~CWndGuildNickName() 
{ 
} 
void CWndGuildNickName::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndGuildNickName::OnInitialUpdate() 
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
BOOL CWndGuildNickName::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILD_NICKNAME, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndGuildNickName::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndGuildNickName::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndGuildNickName::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndGuildNickName::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndGuildNickName::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{
	if( nID == WIDC_OK )
	{
		CWndEdit *pEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
		CString strNickName = pEdit->GetString();
		
		strNickName.TrimLeft();
		strNickName.TrimRight();
		LPCTSTR lpszString = strNickName;
		if( strNickName.IsEmpty() == FALSE )
		{
			CHAR c = strNickName[ 0 ];
			
			int nLength = strNickName.GetLength();
		
			int nMaxLen = 0;
			int nMinLen = 0;

			// �ѱ��� ��Ī 3~12�� ����...
#ifndef __RULE_0615
			if( ::GetLanguage() == LANG_KOR )
#endif	// __RULE_0615
			{
				nMinLen = 2;
				nMaxLen = 12;
			}
#ifndef __RULE_0615
			else
			{
				nMinLen = 3;
				nMaxLen = 10;			
			}
#endif	// __RULE_0615
									
			if( nLength < nMinLen || nLength > nMaxLen )
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0011_01) ) );
//				g_WndMng.OpenMessageBox( _T( "��Ī�� 3���� �̻�, 16���� ���Ϸ� �Է� �Է��Ͻʽÿ�." ) );
				return TRUE;
			}
			else
			if( IsDBCSLeadByte( c ) == FALSE && isdigit2( c ) ) 
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0012) ) );
//				g_WndMng.OpenMessageBox( _T( "��Ī�� ù���ڸ� ���ڷ� ����� �� �����ϴ�." ) );
				return TRUE;
			}
			else
			{
				for( int i = 0; i < strNickName.GetLength(); i++ )
				{
					c = strNickName[ i ];
					// ���ڳ� ���ĺ��� �ƴ� ���� �ǽ�����.
					if( IsDBCSLeadByte( c ) == TRUE ) 
					{
						CHAR c2 = strNickName[ ++i ];
						WORD word = ( ( c << 8 ) & 0xff00 ) | ( c2 & 0x00ff );
						if( ::GetLanguage() == LANG_KOR )
						{
							if( IsHangul( word ) == FALSE ) 
							{
								g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0014) ) );
								return TRUE;
							}
						}
					}
					else
						if( !IsCyrillic( c ) && ( isalnum( c ) == FALSE || iscntrl( c ) )  )
						{
							// Ư�� ���ڵ� �ƴϴ� (�� ��Ʈ�� �Ǵ� !@#$%^&**()... ������)
							g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0013) ) );
							return TRUE;
						}
				}

			}
			
			if( prj.IsInvalidName( strNickName ) 
#ifdef __RULE_0615
				|| prj.IsAllowedLetter( strNickName ) == FALSE
#endif	// __RULE_0615
				)
			{
				g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0020) ) );
				//				g_WndMng.OpenMessageBox( _T( "����Ҽ� ���� �̸��Դϴ�" ) );
				return TRUE;
			}
			CGuild* pGuild = g_pPlayer->GetGuild();
			if( pGuild )
			{
				CGuildMember* pGuildMember = pGuild->GetMember( g_pPlayer->m_idPlayer );
				if( pGuildMember )
				{
					if( strcmp( pGuildMember->m_szAlias, strNickName ) )
					{
						g_DPlay.SendGuildNickName( m_idPlayer, strNickName );
						Destroy();	
					}
				}
			}
		}
		else
		{
			g_WndMng.OpenMessageBox( _T( prj.GetText(TID_DIAG_0011) ) );
		}
	}
	else if( nID == WIDC_CANCEL )
	{
		Destroy();
		//		g_WndMng.OpenMessageBox( _T( "��ȸ�ش� ��Ī�� ���ؾ� �մϴ�." ) );
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}