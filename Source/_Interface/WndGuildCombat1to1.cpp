#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndManager.h"
#include "WndGuildCombat1to1.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;

#if __VER >= 11 // __SYS_PLAYER_DATA
#include "playerdata.h"
#endif	// __SYS_PLAYER_DATA

#if __VER >= 11 // __GUILD_COMBAT_1TO1

//////////////////////////////////////////////////////////////////////////
// 1:1 ��� �Ĺ� ������ ����
//////////////////////////////////////////////////////////////////////////

CWndGuildCombat1to1Selection::CWndGuildCombat1to1Selection() 
{
	m_vecGuildList.clear();
	m_mapSelectPlayer.clear();
	m_vecSelectPlayer.clear();
//	nMaxJoinMember = 0;
//	nMaxWarMember  = 0;
}

CWndGuildCombat1to1Selection::~CWndGuildCombat1to1Selection() 
{
}

//void CWndGuildCombat1to1Selection::SetDefender( u_long uiPlayer ) 
//{
//	m_uidDefender = uiPlayer;
//}

void CWndGuildCombat1to1Selection::AddCombatPlayer( u_long uiPlayer ) 
{
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	m_vecSelectPlayer.push_back( uiPlayer );

	CGuild* pGuild = g_pPlayer->GetGuild();
	
	map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.find( uiPlayer );
	CGuildMember* pMember = i->second;
				
	CString str;
	//str.Format( "Lv%.2d	%.16s %.10s", pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#if __VER >= 11 // __SYS_PLAYER_DATA
	PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
	str.Format( "No.%d  Lv%.2d	%.16s %.10s", pWndList->GetCount()+1, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
	str.Format( "No.%d  Lv%.2d	%.16s %.10s", pWndList->GetCount()+1, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA

	pWndList->AddString( str );			
} 

/*
void CWndGuildCombat1to1Selection::SetMemberSize( int nMaxJoin,  int nMaxWar ) 
{ 
	nMaxJoinMember = nMaxJoin; 
	nMaxWarMember  = nMaxWar; 

	CWndStatic* pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC3 );
	
	CString str;
	str.Format( prj.GetText(TID_GAME_GUILDCOMBAT_OFFER_INFO), nMaxWarMember, nMaxJoinMember );
	pWndStatic->SetTitle( str );	
}
*/

void CWndGuildCombat1to1Selection::AddGuildPlayer( u_long uiPlayer ) 
{
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	m_vecGuildList.push_back( uiPlayer );
	
	CGuild* pGuild = g_pPlayer->GetGuild();
	
	map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.find( uiPlayer );
	CGuildMember* pMember = i->second;
				
	CString str;
#if __VER >= 11 // __SYS_PLAYER_DATA
	PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
	str.Format( "Lv%.2d	%.16s %.10s", pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
	str.Format( "Lv%.2d	%.16s %.10s", pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
	pWndList->AddString( str );			
} 

void CWndGuildCombat1to1Selection::RemoveGuildPlayer( int nIndex ) 
{
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	
	pWndList->DeleteString( nIndex );
	m_vecGuildList.erase( m_vecGuildList.begin() + nIndex );
} 

void CWndGuildCombat1to1Selection::RemoveCombatPlayer( int nIndex ) 
{
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	
	pWndList->DeleteString( nIndex );
	m_vecSelectPlayer.erase( m_vecSelectPlayer.begin() + nIndex );

	//Align Text No.
	int nListCount = pWndList->GetCount();
	for(int i=nIndex; i<nListCount; i++)
	{
		CString temp;

		CGuild* pGuild = g_pPlayer->GetGuild();
		map<u_long, CGuildMember*>::iterator iter = pGuild->m_mapPMember.find( m_vecSelectPlayer[i] );
		CGuildMember* pMember = iter->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
		PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
		temp.Format( "No.%d  Lv%.2d	%.16s %.10s", i+1, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
		temp.Format( "No.%d  Lv%.2d	%.16s %.10s", i+1, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
		pWndList->SetString( i, temp );
	}
}

void CWndGuildCombat1to1Selection::UpDateGuildListBox() 
{
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );

	if( pWndList )
	{
		pWndList->ResetContent();
		
		m_vecGuildList.clear();
		m_mapSelectPlayer.clear();

		CGuild* pGuild = g_pPlayer->GetGuild();
		if( pGuild )
		{
			// �������� ����
			CGuildMember* pMember;
			for( map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.begin(); i != pGuild->m_mapPMember.end(); ++i )
			{
				pMember		= i->second;				
#if __VER >= 11 // __SYS_PLAYER_DATA
				PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
				if( pPlayerData->data.uLogin > 0 )
					m_mapSelectPlayer.insert( make_pair( pPlayerData->data.nLevel, pMember ) );
#else	// __SYS_PLAYER_DATA
				if( pMember->m_nLogin )
					m_mapSelectPlayer.insert( make_pair( pMember->m_nLevel, pMember ) );
#endif	// __SYS_PLAYER_DATA
			}

			// ����Ʈ�� �߰�			
			CString str;
			for( multimap<int, CGuildMember*>::iterator j = m_mapSelectPlayer.begin(); j != m_mapSelectPlayer.end(); ++j )
			{
				pMember		= j->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
				PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
				if( pPlayerData->data.uLogin > 0 )
#else	// __SYS_PLAYER_DATA
				if( pMember->m_nLogin )
#endif	// __SYS_PLAYER_DATA
				{
#if __VER >= 11 // __SYS_PLAYER_DATA
					str.Format( "Lv%.2d	%.16s %.10s", pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
					str.Format( "Lv%.2d	%.16s %.10s", pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
					pWndList->AddString( str );	
					m_vecGuildList.push_back( pMember->m_idPlayer );
				}
			}
		}
	}
}

u_long CWndGuildCombat1to1Selection::FindCombatPlayer(u_long uiPlayer)
{
	for( int i = 0; i < (int)( m_vecSelectPlayer.size() ); i++ )
	{
		if( m_vecSelectPlayer[i] == uiPlayer )
			return m_vecSelectPlayer[i];
	}
	
	return -1;
}

u_long CWndGuildCombat1to1Selection::FindGuildPlayer(u_long uiPlayer)
{
	for( int i = 0; i < (int)( m_vecGuildList.size() ); i++ )
	{
		if( m_vecGuildList[i] == uiPlayer )
			return m_vecGuildList[i];
	}
	
	return -1;
}

void CWndGuildCombat1to1Selection::OnDraw( C2DRender* p2DRender ) 
{
/*	CWndButton* pWndStatic = (CWndButton*)GetDlgItem( WIDC_BUTTON8 );

	if( pWndStatic )
	{
		pWndStatic->EnableWindow( FALSE );

//		if( m_uidDefender <= 0 )
//		{
//			pWndStatic->SetVisible( FALSE );
//		}
//		else
//		{
			pWndStatic->SetVisible( TRUE );

			for( int i=0; i<m_vecSelectPlayer.size(); i++ )
			{
//				if( m_uidDefender == m_vecSelectPlayer[i] )
//				{
//					break;
//				}
			}

			pWndStatic->Move( 330, ( i * 17 ) + 75 );			
//		}
	}

	CRect crect;
	crect.left = 349;
	crect.top  = 74;
	crect.right = 587;
	crect.bottom = crect.top+(nMaxWarMember*17);
	p2DRender->RenderFillRect( crect, D3DCOLOR_ARGB( 40, 0, 0, 220 ) );

	crect.left = 349;
	crect.top  = crect.bottom;
	crect.right = 587;
	crect.bottom = 377;
	p2DRender->RenderFillRect( crect, D3DCOLOR_ARGB( 40, 220, 0, 0 ) );
*/
}

void CWndGuildCombat1to1Selection::EnableFinish( BOOL bFlag )
{
	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_FINISH );

	if( pWndButton )
	{
		pWndButton->EnableWindow( bFlag );	
	}
}

void CWndGuildCombat1to1Selection::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	// �ð� ���������� �Ǵ�
	if( g_GuildCombat1to1Mng.m_nState != CGuildCombat1to1Mng::GC1TO1_OPEN )
	{
		g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_CANNOT_MAKEUP) ); //������ ����ۼ��� �� �� �����ϴ�.
		Destroy();
		return;
	}
	
	MoveParentCenter();

	UpDateGuildListBox();
} 

BOOL CWndGuildCombat1to1Selection::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILDCOMBAT_1TO1_SELECTION, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndGuildCombat1to1Selection::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndGuildCombat1to1Selection::OnSize( UINT nType, int cx, int cy )
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndGuildCombat1to1Selection::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 

void CWndGuildCombat1to1Selection::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

void CWndGuildCombat1to1Selection::Reset()
{
//	m_uidDefender = 0;
	UpDateGuildListBox();
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	pWndListBox->ResetContent();
	m_vecSelectPlayer.clear();	
}

BOOL CWndGuildCombat1to1Selection::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 ) // ������ ���
	{
		CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );

		int nCurSel = pWndListBox->GetCurSel();
		if( nCurSel == -1 )
			return FALSE;

		if( g_GuildCombat1to1Mng.m_nMaxJoinPlayer < (int)( m_vecSelectPlayer.size() ) )
		{
			CString str;
			str.Format( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_SELECTION_MAX), g_GuildCombat1to1Mng.m_nMaxJoinPlayer );
			g_WndMng.OpenMessageBox( str );
			return FALSE;
		}

		CGuild *pGuild = g_pPlayer->GetGuild();

		if( pGuild )
		{
			CGuildMember* pGuildMember = pGuild->GetMember( m_vecGuildList[nCurSel] );

			if( pGuildMember )
			{
#if __VER >= 11 // __SYS_PLAYER_DATA
				if( CPlayerDataCenter::GetInstance()->GetPlayerData( pGuildMember->m_idPlayer )->data.nLevel < g_GuildCombat1to1Mng.m_nMinJoinPlayerLevel )
#else	// __SYS_PLAYER_DATA
				if( pGuildMember->m_nLevel < g_GuildCombat1to1Mng.m_nMinJoinPlayerLevel )
#endif	// __SYS_PLAYER_DATA
				{
					g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_LIMIT_LEVEL_NOTICE) ); //������ ����� ���� 30�̻��� �Ǿ�� �մϴ�.
					return FALSE;
				}
			}	
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_NOT_GUILD_MEMBER) );	//�������� �����ϴ� ����� �ɹ��� �ƴմϴ�.			
				return FALSE;
			}
		}

		u_long uiPlayer;
		uiPlayer = FindCombatPlayer( m_vecGuildList[nCurSel] );

		if( uiPlayer != -1 )
		{
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_ALREADY_ENTRY) ); //�̹� ��ϵǾ� �ֽ��ϴ�. �ٽ� ������ּ���.
			return FALSE;
		}
 
		AddCombatPlayer( m_vecGuildList[nCurSel] );		
		RemoveGuildPlayer( nCurSel );		
	}
	else if( nID == WIDC_BUTTON2 ) // ������ ���
	{
		CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
		
		int nCurSel = pWndListBox->GetCurSel();
		if( nCurSel == -1 )
			return FALSE;

		u_long uiPlayer;
		uiPlayer = FindGuildPlayer( m_vecSelectPlayer[nCurSel] );
		
		if( uiPlayer == -1 )
		{
			// ��帮��Ʈ�� ���ٸ� �߰� 
			AddGuildPlayer( m_vecSelectPlayer[nCurSel] );		
			RemoveCombatPlayer( nCurSel );		
		}
		else
		{
			RemoveCombatPlayer( nCurSel );		
		}		
	}
	else if( nID == WIDC_BUTTON3 ) // ������ ���� ����
	{
		CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
		
		int nCurSel = pWndListBox->GetCurSel();
		if( nCurSel == -1 || nCurSel == 0 )
			return FALSE;
/*		
		CString temp1, temp2;
		pWndListBox->GetText( nCurSel-1, temp1 );
		pWndListBox->GetText( nCurSel,   temp2 );

		pWndListBox->SetString( nCurSel-1, temp2 );
		pWndListBox->SetString( nCurSel, temp1 );
*/		
		u_long uiTemp = 0;

		uiTemp						 = m_vecSelectPlayer[nCurSel];
		m_vecSelectPlayer[nCurSel]   = m_vecSelectPlayer[nCurSel-1];
		m_vecSelectPlayer[nCurSel-1] = uiTemp;
		
		CString temp1, temp2;

		CGuild* pGuild = g_pPlayer->GetGuild();
		map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.find( m_vecSelectPlayer[nCurSel] );
		CGuildMember* pMember = i->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
		PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
		temp1.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+1, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
		temp1.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+1, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
		i = pGuild->m_mapPMember.find( m_vecSelectPlayer[nCurSel-1] );
		pMember = i->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
		pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
		temp2.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
		temp2.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
		pWndListBox->SetString( nCurSel, temp1 );
		pWndListBox->SetString( nCurSel-1, temp2 );

		pWndListBox->SetCurSel(nCurSel-1);
		//m_nDefenderIndex = nCurSel-1;
	}
	else if( nID == WIDC_BUTTON4 ) // ������ ���� �Ʒ�
	{
		CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
		
		int nCurSel = pWndListBox->GetCurSel();
		if( nCurSel == -1 || nCurSel == pWndListBox->GetCount()-1 )
			return FALSE;
/*		
		CString temp1, temp2;
		pWndListBox->GetText( nCurSel, temp1 );
		pWndListBox->GetText( nCurSel+1,   temp2 );
		
		pWndListBox->SetString( nCurSel, temp2 );
		pWndListBox->SetString( nCurSel+1, temp1 );
*/		
		u_long uiTemp = 0;
		
		uiTemp						 = m_vecSelectPlayer[nCurSel];
		m_vecSelectPlayer[nCurSel]   = m_vecSelectPlayer[nCurSel+1];
		m_vecSelectPlayer[nCurSel+1] = uiTemp;

		CString temp1, temp2;

		CGuild* pGuild = g_pPlayer->GetGuild();
		map<u_long, CGuildMember*>::iterator i = pGuild->m_mapPMember.find( m_vecSelectPlayer[nCurSel] );
		CGuildMember* pMember = i->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
		PlayerData* pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
		temp1.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+1, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
		temp1.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+1, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
		i = pGuild->m_mapPMember.find( m_vecSelectPlayer[nCurSel+1] );
		pMember = i->second;
#if __VER >= 11 // __SYS_PLAYER_DATA
		pPlayerData		= CPlayerDataCenter::GetInstance()->GetPlayerData( pMember->m_idPlayer );
		temp2.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+2, pPlayerData->data.nLevel, pPlayerData->szPlayer, prj.m_aJob[ pPlayerData->data.nJob ].szName );
#else	// __SYS_PLAYER_DATA
		temp2.Format( "No.%d  Lv%.2d	%.16s %.10s", nCurSel+2, pMember->m_nLevel, prj.GetPlayerString( pMember->m_idPlayer ), prj.m_aJob[ pMember->m_nJob ].szName );
#endif	// __SYS_PLAYER_DATA
		pWndListBox->SetString( nCurSel, temp1 );
		pWndListBox->SetString( nCurSel+1, temp2 );

		pWndListBox->SetCurSel(nCurSel+1);
		//m_nDefenderIndex = nCurSel+1;
	}
	else if( nID == WIDC_RESET )
	{
		CGuildCombat1to1SelectionResetConfirm* pBox = new CGuildCombat1to1SelectionResetConfirm;
		g_WndMng.OpenCustomBox( "", pBox );
	}
	else if( nID == WIDC_FINISH )
	{
		// �ð� ���������� �Ǵ�
		if( g_GuildCombat1to1Mng.m_nState != CGuildCombat1to1Mng::GC1TO1_OPEN )
		{
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_CANNOT_MAKEUP) ); //������ ����ۼ��� �� �� �����ϴ�.
			Destroy();
			return FALSE;
		}
		
		if( g_GuildCombat1to1Mng.m_nMinJoinPlayer > (int)( m_vecSelectPlayer.size() ) )
		{
			CString str;
			str.Format( prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_MIN_PLAYER), g_GuildCombat1to1Mng.m_nMinJoinPlayer );
			g_WndMng.OpenMessageBox( str );
			return FALSE;
		}

		if( m_vecSelectPlayer.size() == 0 )
		{
			g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_HAVENOT_PLAYER) ); //�����ڰ� �����ϴ�. �����ڸ� �������ּ���.
			return FALSE;
		}

		CGuild *pGuild = g_pPlayer->GetGuild();
		CGuildMember* pGuildMemberl;

		if( pGuild )
		{
			BOOL bSkip = FALSE;

			// ������ �ɹ��߿� �����Ͱ� ŷ���� �ִ��� �˻縦�Ѵ�.
			// ���� �ϳ��� ������ ���� �Ұ���...
			for( int i=0; i<(int)( m_vecSelectPlayer.size() ); i++ )
			{
				pGuildMemberl = pGuild->GetMember( m_vecSelectPlayer[i] );

				if( pGuildMemberl )
				{
					if( pGuildMemberl->m_nMemberLv == GUD_MASTER || pGuildMemberl->m_nMemberLv == GUD_KINGPIN )
					{
						bSkip = TRUE;
						break;
					}
				}
			}

			if( bSkip )
			{
				g_DPlay.SendGC1to1MemberLienUp( m_vecSelectPlayer );
				Destroy();
			}
			else
			{
				g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_HAVENOT_MASTER) ); //������ ��ܿ� ��帶���ͳ� ŷ���� �������� �ʽ��ϴ�.
				return FALSE;
			}
		}
	}
	else if( nID == WIDC_CLOSE )
	{
		Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult );
}

//////////////////////////////////////////////////////////////////////////
// 1:1 ��� �Ĺ� ��û�ϱ�
//////////////////////////////////////////////////////////////////////////

CWndGuildCombat1to1Offer::CWndGuildCombat1to1Offer(int nCombatType) 
{
	m_dwMinGold    = 0;
	m_dwBackupGold = 0;
	m_dwReqGold    = 0;
	m_nCombatType = nCombatType;
}

CWndGuildCombat1to1Offer::~CWndGuildCombat1to1Offer() 
{
}

void CWndGuildCombat1to1Offer::PaintFrame( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
	RenderWnd();
	// ����� Ÿ��Ʋ ���� �ؽ�Ʈ�� ����ϴ� �� 
	if( IsWndStyle( WBS_CAPTION ) )	
	{
		int y = 4;
		CD3DFont* pOldFont = p2DRender->GetFont();
		p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle );
		p2DRender->TextOut( 10, y, m_strTitle, m_dwColor );

		char szNameLevel[128] = {0,};

		if(m_nCombatType == 0)
			sprintf( szNameLevel, "%s", prj.GetText(TID_GAME_GUILDCOMBAT_OFFER) );
		else if(m_nCombatType == 1)
			sprintf( szNameLevel, "%s", prj.GetText(TID_GAME_GUILDCOMBAT_1TO1_OFFER) );

		SetTitle( szNameLevel );
		
		p2DRender->SetFont( pOldFont );
	}
}
		
void CWndGuildCombat1to1Offer::OnDraw( C2DRender* p2DRender ) 
{	
}

void CWndGuildCombat1to1Offer::EnableAccept( BOOL bFlag )
{
	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_ACCEPT );

	if( pWndButton )
	{
		pWndButton->EnableWindow( bFlag );	
	}
}

void CWndGuildCombat1to1Offer::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	CWndText* m_pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	CScript scanner;
	BOOL checkflag;
	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "GuildCombatRedChipGuild.inc" ) ));
	char* szChar = scanner.m_pProg;
	
	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();	
	}
	
	MoveParentCenter();
} 

BOOL CWndGuildCombat1to1Offer::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GUILDCOMBAT_1TO1_OFFER, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndGuildCombat1to1Offer::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndGuildCombat1to1Offer::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndGuildCombat1to1Offer::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 

void CWndGuildCombat1to1Offer::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

void CWndGuildCombat1to1Offer::SetGold( DWORD nCost )
{
	CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );

	if( pWndEdit )
	{
		CString str;
		str.Format( "%d", nCost );
		pWndEdit->SetString(str);
	}				
}
/*
void CWndGuildCombat1to1Offer::SetTotalGold( __int64 nCost )
{
	CWndStatic* pStatic	= (CWndStatic *)GetDlgItem( WIDC_STATIC6 );
	pStatic->AddWndStyle(WSS_MONEY);
	
	if( pStatic )
	{
		CString str;
		str.Format( "%I64d", nCost );
		pStatic->SetTitle(str);
	}
}
*/
BOOL CWndGuildCombat1to1Offer::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_ACCEPT )
	{
		if( g_pPlayer )
		{
			CWndEdit* pWndEdit = (CWndEdit*)GetDlgItem( WIDC_EDIT1 );
			
			DWORD nCost;
			CString str = pWndEdit->GetString();
			nCost = atoi( str );

			if( m_dwReqGold != 0 )
			{
				if( nCost <= m_dwBackupGold )
				{
					g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_MORE_CURRENT_REQUEST) ); //���� ��ĺ��� �� ���� �ݾ����� ��û�� �Ͻñ� �ٶ��ϴ�.
					return FALSE;
				}
				
			}
			else
			{
				if( nCost < m_dwMinGold )
				{
					g_WndMng.OpenMessageBox( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_LIMIT_MIN) ); //�ּұݾ׺��� �� ���� ��ķ� ��û�Ͻñ� �ٶ��ϴ�.
					return FALSE;
				}
			}

			CWndGuildCombat1to1OfferMessageBox* pMsg = new CWndGuildCombat1to1OfferMessageBox;
			if( pMsg )
			{
				g_WndMng.OpenCustomBox( "", pMsg, this );
				CString str;

				if( m_dwReqGold == 0 )
				{
					str.Format( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_MORE_REQUEST), 0, nCost ); //������ ��û�� %d��Ŀ��� �߰��� %d��ĸ� ��û�ϰڽ��ϱ�?
				}
				else
				{
					str.Format( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_MORE_REQUEST), m_dwBackupGold, nCost-m_dwBackupGold ); //������ ��û�� %d��Ŀ��� �߰��� %d��ĸ� ��û�ϰڽ��ϱ�?
				}

				pMsg->SetValue( str, nCost );
			}
		}
	}
	else if( nID == WIDC_CLOSE )
	{
		Destroy();
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

//////////////////////////////////////////////////////////////////////////
// 1:1 ��� ���� ������ ���� Ȯ�� â
//////////////////////////////////////////////////////////////////////////

BOOL CGuildCombat1to1SelectionResetConfirm::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	return CWndMessageBox::Initialize( prj.GetText(TID_GAME_GUILDCOMBAT1TO1_REMAKE_MAKEUP), //����ۼ��� �ٽ� �Ͻðڽ��ϱ�?
		pWndParent, 
		MB_OKCANCEL );
}

BOOL CGuildCombat1to1SelectionResetConfirm::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
	switch( nID )
	{
		case IDOK:
			{
				CWndGuildCombat1to1Selection *pGuildCombat1to1Selection = (CWndGuildCombat1to1Selection*)g_WndMng.GetWndBase( APP_GUILDCOMBAT_1TO1_SELECTION );

				if( pGuildCombat1to1Selection )
					pGuildCombat1to1Selection->Reset();

				Destroy();
			}
			break;
		case IDCANCEL:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

//////////////////////////////////////////////////////////////////////////
// 1:1 ��� ���� ���� Ȯ�� â
//////////////////////////////////////////////////////////////////////////

BOOL CWndGuildCombat1to1OfferMessageBox::Initialize( CWndBase* pWndParent, DWORD dwWndId )
{
	return CWndMessageBox::Initialize( "", 
		pWndParent, 
		MB_OKCANCEL );	
}

void CWndGuildCombat1to1OfferMessageBox::SetValue( CString str, DWORD nCost )
{
	m_wndText.SetString( str );
	m_nCost = nCost;	
}

BOOL CWndGuildCombat1to1OfferMessageBox::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
	switch( nID )
	{
		case IDOK:
			{
				CWndGuildCombat1to1Offer* pWndGuildCombat1to1Offer = (CWndGuildCombat1to1Offer*)g_WndMng.GetWndBase(APP_GUILDCOMBAT_1TO1_OFFER);
				if(pWndGuildCombat1to1Offer)
				{
					if(pWndGuildCombat1to1Offer->m_nCombatType == 0)
					{
						g_DPlay.SendGuildCombatApp( m_nCost );
						SAFE_DELETE(g_WndMng.m_pWndGuildCombatOffer);
					}
					else if(pWndGuildCombat1to1Offer->m_nCombatType == 1)
					{
						g_DPlay.SendGC1to1Tender( m_nCost );
						SAFE_DELETE(g_WndMng.m_pWndGuildCombat1to1Offer);
					}
				}
				Destroy();
			}
			break;
		case IDCANCEL:
			Destroy();
			break;
	}
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#endif //__GUILD_COMBAT_1TO1