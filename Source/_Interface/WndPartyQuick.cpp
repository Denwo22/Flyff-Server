#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndPartyQuick.h"
#include "WndManager.h"
#include "Party.h"
#include "WndParty.h"

#if __VER >= 11 // __SYS_PLAYER_DATA
#include "playerdata.h"
#endif	// __SYS_PLAYER_DATA


#if __VER >= 8 //__CSC_VER8_2

#define MAX_PARTY_MEMBER 8

int StaticID[MAX_PARTY_MEMBER] = {WIDC_STATIC_MEM1, WIDC_STATIC_MEM2, WIDC_STATIC_MEM3, WIDC_STATIC_MEM4,
WIDC_STATIC_MEM5, WIDC_STATIC_MEM6, WIDC_STATIC_MEM7, WIDC_STATIC_MEM8};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndPartyQuick
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndPartyQuick::CWndPartyQuick() 
{ 
	m_pVBGauge = NULL;
	m_MemberCount = 0;
	m_pWndParty = NULL;
	m_pFocusMember = NULL;
} 

CWndPartyQuick::~CWndPartyQuick() 
{ 
	DeleteDeviceObjects();
} 

HRESULT CWndPartyQuick::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pVBGauge == NULL )
		return m_pApp->m_pd3dDevice->CreateVertexBuffer( sizeof( TEXTUREVERTEX2 ) * 3 * 6, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_TEXTUREVERTEX2, D3DPOOL_DEFAULT, &m_pVBGauge, NULL );
	return S_OK;
}
HRESULT CWndPartyQuick::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVBGauge );
	return S_OK;
}
HRESULT CWndPartyQuick::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE( m_pVBGauge );
	return S_OK;
}

void CWndPartyQuick::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	CWndNeuz::SerializeRegInfo( ar, dwVersion );
}

void CWndPartyQuick::OnDraw( C2DRender* p2DRender ) 
{ 
	// ��Ƽ ���� ���
	//Static control
	int nMax = g_Party.m_nSizeofMember;
	int i;
	CRect rect;

	SetActiveMember(nMax);

	for(i=0; i<nMax; i++)
	{
		LPWNDCTRL lpWndCtrl = GetWndCtrl( StaticID[i] );
		rect = lpWndCtrl->rect;
		CMover* pObjMember = prj.GetUserByID( g_Party.m_aMember[i].m_uPlayerId );
		CString strMember;
		
		if(m_pFocusMember == pObjMember)
			p2DRender->RenderFillRect( rect, 0x60ffff00 );

		// ���¿� ���� �� ����
		DWORD dwColor = 0xff000000;
		u_long  nLeadMember = g_Party.m_aMember[i].m_uPlayerId;
		if( IsValidObj(pObjMember) )
		{
			if( pObjMember->GetHitPoint() == 0 ) 
				dwColor = 0xffff0000; // ������
			else if( ((FLOAT)pObjMember->GetHitPoint()) / ((FLOAT)pObjMember->GetMaxHitPoint()) <.1f ) 
				dwColor = 0xffffff00; // HP 10% �����γ�
			if(g_Party.IsLeader(nLeadMember)) //Leader Color Set
			{
				dwColor = 0xff1fb72d; //���� �ؾ���...
#if __VER >= 10 // __LEGEND
				if(pObjMember->IsMaster())
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_MASTER ), pObjMember->GetLevel(), pObjMember->GetName() );
				else if(pObjMember->IsHero())
#if __VER >= 15 // __HERO129_VER15				// 15�� ����� ����Ȯ��
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_HERO ), pObjMember->GetLevel(), pObjMember->GetName() );
	#else	// 15�� ����� ����Ȯ��
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_HERO_BEFORE ), pObjMember->GetName() );
	#endif	// 15�� ����� ����Ȯ��
				else 
					strMember.Format( "%d. %s", pObjMember->GetLevel(), pObjMember->GetName() );
#else //__LEGEND
				strMember.Format( "%d. %s", pObjMember->GetLevel(), pObjMember->GetName() );
#endif //__LEGEND
			}
			else
			{
#if __VER >= 10 // __LEGEND
				if(pObjMember->IsMaster())
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_MASTER ), pObjMember->GetLevel(), pObjMember->GetName() );
				else if(pObjMember->IsHero())
#if __VER >= 15 // __HERO129_VER15				// 15�� ����� ����Ȯ��
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_HERO ), pObjMember->GetLevel(), pObjMember->GetName() );
	#else	// 15�� ����� ����Ȯ��
					strMember.Format( prj.GetText( TID_GAME_QUICK_MARK_HERO_BEFORE ), pObjMember->GetName() );
	#endif	// 15�� ����� ����Ȯ��
				else 
					strMember.Format( "%d. %s", pObjMember->GetLevel(), pObjMember->GetName() );
#else //__LEGEND
				strMember.Format( "%d. %s", pObjMember->GetLevel(), pObjMember->GetName() );
#endif //__LEGEND
			}
		}
		else
		{
			dwColor = 0xff878787; // ����Ʈ�� ������ ���³�
			if( g_Party.m_aMember[i].m_bRemove ) 
				dwColor = 0xff000000; // ������ ���³�
#if __VER >= 11 // __SYS_PLAYER_DATA
			const char* pszPlayer	= CPlayerDataCenter::GetInstance()->GetPlayerString( g_Party.m_aMember[i].m_uPlayerId );
#else	// __SYS_PLAYER_DATA
			const char* pszPlayer	= g_Party.m_aMember[i].m_szName;
#endif	// __SYS_PLAYER_DATA

#if __VER >= 11 // __MA_VER11_03	// neuz�ش�â(B)���� �شܿ��� �ָ� ������ �־ ������ ǥ�� �ǵ���
			CString	strTemp2;
	#if __VER >= 11 // __SYS_PLAYER_DATA
			PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData( g_Party.m_aMember[i].m_uPlayerId );
			int nLevel	= pPlayerData->data.nLevel;
			int nJob	= pPlayerData->data.nJob;
	#else	// __SYS_PLAYER_DATA
			int nLevel	= g_Party.m_aMember[i].m_nLevel;
			int nJob	= g_Party.m_aMember[i].m_nJob;
	#endif	// __SYS_PLAYER_DATA

			if( MAX_PROFESSIONAL <= nJob && nJob < MAX_MASTER )
				strTemp2.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_MASTER ) );
			else if( MAX_MASTER <= nJob )
#if __VER >= 15 // __HERO129_VER15 // 15�� ����� ����Ȯ��
				strTemp2.Format( "%d%s", nLevel, prj.GetText( TID_GAME_TOOLTIP_MARK_HERO ) );
#else // 15�� ����� ����Ȯ��
				strTemp2 = prj.GetText( TID_GAME_TOOLTIP_MARK_HERO_BEFORE );
#endif // 15�� ����� ����Ȯ��
			else 
				strTemp2.Format( "%d", nLevel );

			if(g_Party.IsLeader(nLeadMember)) //Leader Set ���� �ؾ���...
				strMember.Format( "%s. %s", strTemp2,pszPlayer );
			else
				strMember.Format( "%s. %s", strTemp2,pszPlayer );

#else	//	__MA_VER11_03	// neuz�ش�â(B)���� �شܿ��� �ָ� ������ �־ ������ ǥ�� �ǵ���
			if(g_Party.IsLeader(nLeadMember)) //Leader Set ���� �ؾ���...
				strMember.Format( "??. %s", pszPlayer );
			else
				strMember.Format( "??. %s", pszPlayer );
#endif	//	__MA_VER11_03	// neuz�ش�â(B)���� �شܿ��� �ָ� ������ �־ ������ ǥ�� �ǵ���

		}
		//Member - Level, Name Draw
		//�� �̸��� ... ����.
		if( strMember.GetLength() > 13 ) 
		{
			int	nReduceCount = 0;

			for( nReduceCount=0; nReduceCount <13; )
			{
				if( IsDBCSLeadByte( strMember[ nReduceCount ] ) )
					nReduceCount+=2;
				else
					nReduceCount++;
			}

			strMember = strMember.Left( nReduceCount );
			strMember+="...";
		}

		p2DRender->TextOut( rect.TopLeft().x+5, rect.TopLeft().y+5, strMember, dwColor );

		//Member - Gauge Draw
		rect.TopLeft().y += 18;
		rect.TopLeft().x += 2;
		rect.BottomRight().x -= 2;
		int nWidth	= pObjMember ? pObjMember->GetHitPointPercent( rect.Width() ) : 0;
		CRect rectTemp = rect; 
		rectTemp.right = rectTemp.left + nWidth;
		if( rect.right < rectTemp.right )
			rectTemp.right = rect.right;
		m_pTheme->RenderGauge( p2DRender, &rect, 0xffffffff, m_pVBGauge, &m_texGauEmptyNormal );
		m_pTheme->RenderGauge( p2DRender, &rectTemp, 0x64ff0000, m_pVBGauge, &m_texGauFillNormal );
	}
} 

void CWndPartyQuick::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	m_pBtnParty = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );

	for(int i=0; i<MAX_PARTY_MEMBER; i++)
	{
		m_pWndMemberStatic[i] = (CWndStatic*)GetDlgItem( StaticID[i] );
	}

	m_texGauEmptyNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "GauEmptySmall.bmp" ), 0xffff00ff, TRUE );
	m_texGauFillNormal.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "GauFillSmall.bmp" ), 0xffff00ff, TRUE );
	
	SetActiveMember(g_Party.m_nSizeofMember);
	
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CPoint point( 30, (rectRoot.bottom - rectRoot.top) / 4);
	Move( point );
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndPartyQuick::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	if(g_Party.GetSizeofMember() < 2) //���۰� �Բ� ���� ��� Quickâ�� ���� �ʵ��� �Ѵ�. Party������ ���� ���� �Ұ�.
		return FALSE;
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_PARTY_QUICK, 0, CPoint( 0, 0 ), pWndParent );
} 

BOOL CWndPartyQuick::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndPartyQuick::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndPartyQuick::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	CRect rect;
	LPWNDCTRL lpWndCtrl;
	int nMax = g_Party.m_nSizeofMember;

#if __VER >= 11 // __CSC_VER11_2
	CWndTaskBar* pTaskBar = g_WndMng.m_pWndTaskBar;
	if(((CWndWorld*)g_WndMng.m_pWndWorld)->m_bAutoAttack || pTaskBar->m_nExecute != 0)
		return;
#endif //__CSC_VER11_2
	
	for(int i=0; i<nMax; i++) 
	{
		lpWndCtrl = GetWndCtrl( StaticID[i] );
		rect = lpWndCtrl->rect;
		if( rect.PtInRect( point ) )
		{
			((CWndWorld*)g_WndMng.m_pWndWorld)->m_pSelectRenderObj = NULL; //���콺�� �ɷ� �׷������� ����� Ÿ���� ��ƾ� �Ȱ�ģ��..
			m_pFocusMember = prj.GetUserByID( g_Party.m_aMember[ i ].m_uPlayerId );
			if( g_pPlayer != m_pFocusMember ) 
			{
				if( IsValidObj( m_pFocusMember ) ) 
				{
					g_WorldMng()->SetObjFocus( m_pFocusMember );
					CWndWorld* pWndWorld = g_WndMng.m_pWndWorld;
					if(pWndWorld)
						pWndWorld->m_pRenderTargetObj = NULL;
				}
			}
			else
				g_WorldMng()->SetObjFocus( NULL );
			return;
		}
	}
} 

void CWndPartyQuick::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
}

void CWndPartyQuick::OnLButtonDblClk( UINT nFlags, CPoint point )
{
}

void CWndPartyQuick::OnRButtonDblClk( UINT nFlags, CPoint point )
{
}

BOOL CWndPartyQuick::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 ) 
	{
		m_pWndParty = (CWndParty*)g_WndMng.GetApplet( APP_PARTY );
		if(m_pWndParty == NULL)
			m_pWndParty = (CWndParty*)g_WndMng.CreateApplet( APP_PARTY );
		else
		{
			m_pWndParty->Destroy();
			m_pWndParty = NULL;
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
}

void CWndPartyQuick::SetActiveMember(int MemberNum)
{
	int i, height;
	CRect rect;
	
	if(m_MemberCount != MemberNum)
	{
		rect = GetWindowRect(TRUE);
		height = 60;

		for(i=0; i<MemberNum; i++)
		{
			CWndStatic* lpWndStatic;
			lpWndStatic = (CWndStatic*) GetDlgItem( StaticID[i] );
			lpWndStatic->EnableWindow(TRUE);
			lpWndStatic->SetVisible(TRUE);
			height += 34;
		}
		for(i=MemberNum; i<MAX_PARTY_MEMBER; i++)
		{
			CWndStatic* lpWndStatic;
			lpWndStatic = (CWndStatic*) GetDlgItem( StaticID[i] );
			lpWndStatic->EnableWindow(FALSE);
			lpWndStatic->SetVisible(FALSE);
		}
		rect.bottom = rect.top + height;
		//Button Move
		CRect btnRect;
		btnRect.top = height - 50;
		btnRect.bottom = btnRect.top + 10;
		btnRect.left = 114;
		btnRect.right = btnRect.left + 10;
		m_pBtnParty->SetWndRect(btnRect);

		SetWndRect(rect);
		AdjustWndBase();
		m_MemberCount = MemberNum;
	}
}

#endif //__CSC_VER8_2
