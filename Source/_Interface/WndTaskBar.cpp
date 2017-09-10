// WndArcane.cpp: implementation of the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "DialogMsg.h"
#include "WndField.h"
#include "WndManager.h"
#include "WndRegVend.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;


_ERROR_STATE g_Error_State;


#define WND_WIDTH 210
#define TASKBAR_HEIGHT 48


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 작업 윈도 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CWndShortcut::CWndShortcut()
{
}
CWndShortcut::~CWndShortcut()
{
}
void CWndShortcut::OnDraw(C2DRender* p2DRender)
{
	CWndButton::OnDraw( p2DRender );
}
BOOL CWndShortcut::Process()
{
	return CWndButton::Process();
}
void CWndShortcut::PaintFrame( C2DRender* p2DRender ) 
{
	CWndButton::PaintFrame( p2DRender );
} 
void CWndShortcut::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonUp( nFlags, point );
}
void CWndShortcut::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonDown( nFlags, point );
}
void CWndShortcut::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonUp( nFlags, point );
}
void CWndShortcut::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonDown( nFlags, point );
}
void CWndShortcut::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWndButton::OnLButtonDblClk( nFlags, point );
}
void CWndShortcut::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CWndButton::OnRButtonDblClk( nFlags, point );
}
void CWndShortcut::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWndButton::OnKeyDown( nChar, nRepCnt, nFlags );
}
void CWndShortcut::OnMouseMove(UINT nFlags, CPoint point)
{
	CWndButton::OnMouseMove( nFlags, point );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 퀵 리스트 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndQuickList::CWndQuickList() 
{ 
} 
CWndQuickList::~CWndQuickList() 
{ 
} 
void CWndQuickList::OnDraw( C2DRender* p2DRender ) 
{
	CWndTaskBar* pWndTaskBar = (CWndTaskBar*)m_pParentWnd;
	// 아이템 아이콘 출력 
	CPoint point = CPoint( 0, 0);//POINT_ITEM;
	for( int y = 0; y < 4; y++ )
	{
		for( int i = 0; i < MAX_SLOT_ITEM; i++ )
		{
			point = CPoint( i * 32 + 32 , y * 38 + 6 );
			LPSHORTCUT lpShortcut = &pWndTaskBar->m_aSlotItem[ y ][ i ] ;
			if( !lpShortcut->IsEmpty() )
			{
				if( lpShortcut->m_pTexture )
					p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
				if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
				{
					CItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
					CItemElem* pItemElem = (CItemElem*)pItemBase;

					if( pItemElem  )
					{
						if( pItemElem->GetProp()->dwPackMax > 1 )
						{
							TCHAR szTemp[ 32 ];
							_stprintf( szTemp, "%d", pItemElem->m_nItemNum );
							CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
							p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
							p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
						}
					}
					else
						lpShortcut->m_dwShortcut = SHORTCUT_NONE; 				
				}
			}
		}
	}
} 
void CWndQuickList::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// 처음 이 함수를 부르면 윈도가 열린다.
BOOL CWndQuickList::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy에서 설정한 리소스로 윈도를 연다.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_QUICKLIST, 0, CPoint( 0, 0 ), pWndParent );
	
} 
/*
  직접 윈도를 열때 사용 
BOOL CWndQuickList::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndQuickList::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndQuickList::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndQuickList::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndQuickList::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndQuickList::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 
//#ifdef __NEWINTERFACE

//#define TASKSHORTCUT 45
#define ICON_SIZE    32
#define SKILL_SIZE   28

/*
#define POINT_APPLET_X  87
#define POINT_APPLET_Y   9
#define POINT_ITEM_X   295
#define POINT_ITEM_Y     9
#define POINT_QUEUE_X  649
#define POINT_QUEUE_Y    9
*/
DWORD   POINT_APPLET_X ;
DWORD   POINT_APPLET_Y ;
DWORD   POINT_ITEM_X   ;
DWORD   POINT_ITEM_Y   ;
DWORD   POINT_QUEUE_X  ;
DWORD   POINT_QUEUE_Y  ;

#define HORIRC_APPLET( nIndex ) CRect( POINT_APPLET_X + nIndex * ICON_SIZE, 6, POINT_APPLET_X + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_APPLET( nIndex ) CRect( 6, POINT_APPLET_X + nIndex * ICON_SIZE, 3 + ICON_SIZE, POINT_APPLET_X + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_ITEM( nIndex )   CRect( POINT_ITEM_X + nIndex * ICON_SIZE, 6, POINT_ITEM_X + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_ITEM( nIndex )   CRect( 6, POINT_ITEM_X + nIndex * ICON_SIZE, 3 + ICON_SIZE, POINT_ITEM_X + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_QUEUE( nIndex )  CRect( POINT_QUEUE_X + nIndex * SKILL_SIZE, 6, POINT_QUEUE_X + nIndex * SKILL_SIZE + SKILL_SIZE, 7 + SKILL_SIZE )
#define VERTRC_QUEUE( nIndex )  CRect( 6, POINT_QUEUE_X + nIndex * SKILL_SIZE, 6 + SKILL_SIZE, POINT_QUEUE_X + nIndex * SKILL_SIZE + SKILL_SIZE )

#define RECT_APPLET CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + m_nMaxSlotApplet * ICON_SIZE , POINT_APPLET_Y + ICON_SIZE  )
#define RECT_ITEM   CRect( POINT_ITEM_X  , POINT_ITEM_Y  , POINT_ITEM_X   + MAX_SLOT_ITEM   * ICON_SIZE , POINT_ITEM_Y   + ICON_SIZE  )
#define RECT_QUEUE  CRect( POINT_QUEUE_X , POINT_QUEUE_Y , POINT_QUEUE_X  + MAX_SLOT_QUEUE  * SKILL_SIZE, POINT_QUEUE_Y  + SKILL_SIZE )

#define POINT_APPLET CPoint( POINT_APPLET_X, POINT_APPLET_Y )
#define POINT_ITEM   CPoint( POINT_ITEM_X  , POINT_ITEM_Y   )
#define POINT_QUEUE  CPoint( POINT_QUEUE_X , POINT_QUEUE_Y  )

//#else
/*
#define TASKSHORTCUT 45
#define ICON_SIZE    32
#define SKILL_SIZE   28

#define POINT_APPLET_X  89
#define POINT_APPLET_Y   8
#define POINT_ITEM_X   306
#define POINT_ITEM_Y     8
#define POINT_QUEUE_X  648
#define POINT_QUEUE_Y    6

#define HORIRC_APPLET( nIndex ) CRect( 89 + nIndex * ICON_SIZE, 6, 89 + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_APPLET( nIndex ) CRect( 6, 89 + nIndex * ICON_SIZE, 3 + ICON_SIZE, 292 + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_ITEM( nIndex )   CRect( 306 + nIndex * ICON_SIZE, 6, 306 + nIndex * ICON_SIZE + ICON_SIZE , 6 + ICON_SIZE )
#define VERTRC_ITEM( nIndex )   CRect( 6, 292 + nIndex * ICON_SIZE, 3 + ICON_SIZE, 292 + nIndex * ICON_SIZE + ICON_SIZE )
#define HORIRC_QUEUE( nIndex )  CRect( 648 + nIndex * SKILL_SIZE, 6, 648 + nIndex * SKILL_SIZE + SKILL_SIZE, 7 + SKILL_SIZE )
#define VERTRC_QUEUE( nIndex )  CRect( 6, 648 + nIndex * SKILL_SIZE, 6 + 32, 648 + nIndex * SKILL_SIZE + SKILL_SIZE )

#define RECT_APPLET CRect(  89, 6, 280, 38 )
#define RECT_ITEM   CRect( 306, 6, 612, 38 )
#define RECT_QUEUE  CRect( 648, 6, 790, 38 )

#define POINT_APPLET CPoint( POINT_APPLET_X, POINT_APPLET_Y )
#define POINT_ITEM   CPoint( POINT_ITEM_X  , POINT_ITEM_Y   )
#define POINT_QUEUE  CPoint( POINT_QUEUE_X , POINT_QUEUE_Y  )

#endif
  */
void CWndTaskBar::OnDestroyChildWnd( CWndBase* pWndChild )
{
	if( m_pWndQuickList == pWndChild )
		SAFE_DELETE( m_pWndQuickList );
}

void CWndTaskBar::Serialize( CAr & ar )
{
	if( ar.IsStoring() )
	{
		int nCount	= 0;
		u_long uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_APPLET; i++ )
		{
			if( m_aSlotApplet[i].m_dwShortcut == SHORTCUT_NONE )		// 값이 들어있는지 검사
			{
				ar << i;
				ar << m_aSlotApplet[i].m_dwShortcut <<	m_aSlotApplet[i].m_dwId << m_aSlotApplet[i].m_dwType;
				ar << m_aSlotApplet[i].m_dwIndex <<	m_aSlotApplet[i].m_dwUserId << m_aSlotApplet[i].m_dwData;
				if( m_aSlotApplet[i].m_dwShortcut == SHORTCUT_CHAT )
					ar.WriteString( m_aSlotApplet[i].m_szString );
				nCount++;
			}
		}
		int nBufSize;
		LPBYTE lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;

		nCount	= 0;
		uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j = 0; j < MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_NONE )		// 값이 들어있는지 검사
				{
					ar << i << j;
					ar << m_aSlotItem[i][j].m_dwShortcut <<	m_aSlotItem[i][j].m_dwId << m_aSlotItem[i][j].m_dwType;
					ar << m_aSlotItem[i][j].m_dwIndex << m_aSlotItem[i][j].m_dwUserId << m_aSlotItem[i][j].m_dwData;
					if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT)
						ar.WriteString( m_aSlotItem[i][j].m_szString );
					nCount++;
					
				}
			}
		}
		lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;

		nCount	= 0;
		uOffset	= ar.GetOffset();
		ar << nCount;
		for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
		{
			if( m_aSlotQueue[i].m_dwShortcut == SHORTCUT_NONE )		// 값이 들어있는지 검사
			{
				ar << i;
				ar << m_aSlotQueue[i].m_dwShortcut << m_aSlotQueue[i].m_dwId << m_aSlotQueue[i].m_dwType;
				ar << m_aSlotQueue[i].m_dwIndex << m_aSlotQueue[i].m_dwUserId << m_aSlotQueue[i].m_dwData;
				nCount++;
			}
		}
 		ar << m_nActionPoint;

		lpBuf	= ar.GetBuffer( &nBufSize );
		*(UNALIGNED int*)( lpBuf + uOffset )	= nCount;
	}
	else
	{
		InitTaskBar();
		int nCount, nIndex;
		ar >> nCount;	// applet count
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex;
			ar >> m_aSlotApplet[nIndex].m_dwShortcut >>	m_aSlotApplet[nIndex].m_dwId >> m_aSlotApplet[nIndex].m_dwType;
			ar >> m_aSlotApplet[nIndex].m_dwIndex >> m_aSlotApplet[nIndex].m_dwUserId >> m_aSlotApplet[nIndex].m_dwData;
			if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_CHAT)
				ar.ReadString( m_aSlotApplet[nIndex].m_szString, MAX_SHORTCUT_STRING );
			m_aSlotApplet[nIndex].m_dwIndex = nIndex;
			SetTaskBarTexture( &m_aSlotApplet[nIndex] );

			if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_SKILL )	
			{
				ItemProp* pProp;
				if( m_aSlotApplet[nIndex].m_dwType == 2 )
					pProp =  prj.GetPartySkill( m_aSlotApplet[nIndex].m_dwId );
				else
				{
					LPSKILL pSkill = g_pPlayer->GetSkill( 0, m_aSlotApplet[nIndex].m_dwId );
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
				}
				if( pProp == NULL || (pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL) )	
				{
					m_aSlotApplet[nIndex].Empty();
				}
			}
#ifdef __CLIENT
#if __VER >= 8	// __JEFF_VER_8
			else if( m_aSlotApplet[nIndex].m_dwShortcut == SHORTCUT_ITEM )
			{
				if( g_pPlayer )
				{
					CItemBase* pItemBase	= g_pPlayer->GetItemId( m_aSlotApplet[nIndex].m_dwId );
					if( pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// 병합 가능한 아이템이면?
						m_aSlotApplet[nIndex].m_dwItemId	= pItemBase->m_dwItemId;
				}
			}
#endif	// __JEFF_VER_8
#endif	// __CLIENT
		}
		ar >> nCount;	// slot item count
		int nIndex2;
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex >> nIndex2;	// index
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwShortcut >>	m_aSlotItem[nIndex][nIndex2].m_dwId >> m_aSlotItem[nIndex][nIndex2].m_dwType;
			ar >> m_aSlotItem[nIndex][nIndex2].m_dwIndex >> m_aSlotItem[nIndex][nIndex2].m_dwUserId >> m_aSlotItem[nIndex][nIndex2].m_dwData;
			if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_CHAT )
				ar.ReadString( m_aSlotItem[nIndex][nIndex2].m_szString, MAX_SHORTCUT_STRING );
#ifdef __CLIENT
#if __VER >= 8	// __JEFF_VER_8
			else if( m_aSlotItem[nIndex][nIndex2].m_dwShortcut == SHORTCUT_ITEM )
			{
				if( g_pPlayer )
				{
					CItemBase* pItemBase	= g_pPlayer->GetItemId( m_aSlotItem[nIndex][nIndex2].m_dwId );
					if( pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// 병합 가능한 아이템이면?
						m_aSlotItem[nIndex][nIndex2].m_dwItemId	= pItemBase->m_dwItemId;
				}
			}
#endif	// __JEFF_VER_8
#endif	// __CLIENT
			m_aSlotItem[nIndex][nIndex2].m_dwIndex = nIndex2;
			SetTaskBarTexture( &m_aSlotItem[nIndex][nIndex2] );
		}

		ar >> nCount;
		for( int i = 0; i < nCount; i++ )
		{
			ar >> nIndex;
			ar >> m_aSlotQueue[nIndex].m_dwShortcut >> m_aSlotQueue[nIndex].m_dwId >> m_aSlotQueue[nIndex].m_dwType;
			ar >> m_aSlotQueue[nIndex].m_dwIndex >> m_aSlotQueue[nIndex].m_dwUserId >> m_aSlotQueue[nIndex].m_dwData;

			m_aSlotQueue[nIndex].m_dwIndex = nIndex;

			LPSKILL lpSkill = g_pPlayer->GetSkill( m_aSlotQueue[nIndex].m_dwType, m_aSlotQueue[nIndex].m_dwId );
			//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon ) );
			m_aSlotQueue[nIndex].m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, FALSE );
		}
		ar >> m_nActionPoint;
		m_nCurQueueNum = nCount;
	}
}

void CWndTaskBar::SetTaskBarTexture( LPSHORTCUT pShortcut )
{
	if( pShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( pShortcut->m_dwId );
		if( pAppletFunc )
		{
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
		} else
		{
#ifndef __BS_CONSOLE
			Error( "CWndTaskBar::SetTaskBarTexture : %s %d", g_pPlayer->GetName(), pShortcut->m_dwId );
#endif
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_ITEM )
	{
		CItemBase* pItemBase;
		pItemBase	= g_pPlayer->GetItemId( pShortcut->m_dwId );
		if( pItemBase )
			pShortcut->m_pTexture	= pItemBase->GetTexture();
	}
	else if ( pShortcut->m_dwShortcut == SHORTCUT_SKILL)
	{
		if( pShortcut->m_dwType == 2 )
		{
			ItemProp* pProp =  prj.GetPartySkill( pShortcut->m_dwId );
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff );
		}
		else
		{	
			LPSKILL lpSkill = g_pPlayer->GetSkill( pShortcut->m_dwType, pShortcut->m_dwId );
			ItemProp* pSkillProp = prj.m_aPropSkill.GetAt( lpSkill->dwSkill );
			if( pSkillProp )
				pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pSkillProp->szIcon ), 0xffff00ff );
		}
	}
#if __VER >= 12 // __LORD
	else if ( pShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
	{
		CCLord* pLord									= CCLord::Instance();
		CLordSkillComponentExecutable* pComponent		= pLord->GetSkills()->GetSkill(pShortcut->m_dwId);	
		if(pComponent) pShortcut->m_pTexture							= pComponent->GetTexture();
	}
#endif
	else if ( pShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		MotionProp* pMotionProp = prj.GetMotionProp( pShortcut->m_dwId );
		if(pMotionProp)			//061206 ma	8차에 들어갈 모션관리를 위해 버전 추가	propMotion.txt
		{
			pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
			pShortcut->m_pTexture = pMotionProp->pTexture;
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
		pShortcut->m_pTexture	= m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, "icon_MacroChat.dds" ), 0xffff00ff );
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_EMOTICON )
	{
		if( pShortcut->m_dwId >= 0 && pShortcut->m_dwId < MAX_EMOTICON_NUM  )
		{
			TCHAR buffer[MAX_SHORTCUT_STRING] = { 0 };
			_tcscat( buffer, "/" );
			_tcscat( buffer, g_DialogMsg.m_EmiticonCmd[pShortcut->m_dwId].m_szCommand );
			_tcscpy( pShortcut->m_szString, buffer );
			pShortcut->m_pTexture =	g_DialogMsg.m_texEmoticonUser.GetAt(pShortcut->m_dwId);
		}
	}
	else if( pShortcut->m_dwShortcut == SHORTCUT_SKILLFUN )
	{
		pShortcut->m_pTexture	= m_pTexture;
	}
}

/*
* 액션 포인트
. 액션 포인트 변수 CWndTaskBar::m_nActionPoint
. 스킬바에 반투명 사각형 그리기
. 액션포인트 시리얼라이즈
. 패턴공격의 2번째 공격을 사용시에는 액션포인트 증가 1점씩 - 100이 한계
. 액션 포인트에 따른 쉐이드 그리기 - 누적포인트에따라 다르다.
. 스킬사용시 단계별로 액션포인트 소모량 다르게.

*/

CWndTaskBar::CWndTaskBar()
{
	m_nCurQueue = -1;
	m_nCurQueueNum = 0;
	//m_nPosition = TASKBAR_RIGHT;//LEFT;//TASKBAR_TOP;
	m_nPosition = TASKBAR_BOTTOM;//LEFT;//TASKBAR_TOP;
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	memset( &m_aSlotSkill, 0, sizeof( m_aSlotSkill ) );

	m_pWndQuickList = NULL;
	m_pSelectShortcut = NULL;
	m_bStartTimeBar = FALSE;
	m_nSkillBar = 0;
	m_nUsedSkillQueue = 0;
	m_nExecute = 0;
	m_nSlotIndex = 0;
	m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
	m_nActionPoint = 0;
	m_dwHighAlpha = 0;
//	m_nMaxSlotApplet = 5;
}

CWndTaskBar::~CWndTaskBar()
{
	/*
	for( int i = 0; i < m_awndShortCut.GetSize(); i++ )
	{
		CWndButton* pButton = (CWndButton*)m_awndShortCut.GetAt( i );
		SAFE_DELETE( pButton );
	}
	*/
}

void CWndTaskBar::InitTaskBar()
{
	m_nCurQueue = -1;
	m_nCurQueueNum = 0;
	//m_nPosition = TASKBAR_RIGHT;//LEFT;//TASKBAR_TOP;
	m_nPosition = TASKBAR_BOTTOM;//LEFT;//TASKBAR_TOP;
	memset( m_aSlotApplet, 0, sizeof( m_aSlotApplet ) );
	memset( m_aSlotItem  , 0, sizeof( m_aSlotItem ) );
	memset( m_aSlotQueue , 0, sizeof( m_aSlotQueue ) );
	
	m_pWndQuickList = NULL;
	m_pSelectShortcut = NULL;
	m_bStartTimeBar = FALSE;
	m_nSkillBar = 0;
	m_nUsedSkillQueue = 0;
	m_nExecute = 0;
	m_nSlotIndex = 0;
	m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
	m_nActionPoint = 0;
	m_dwHighAlpha = 0;
	//m_nMaxSlotApplet = 5;
}

void CWndTaskBar::PutTooTip( LPSHORTCUT pShortcut, CPoint point, CRect* pRect )
{
	ClientToScreen( &point );
	ClientToScreen( pRect );
	if( pShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
		AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( pShortcut->m_dwId ); 
		if( pAppletFunc )
		{
			//do 
		//	{
			
			//CString string;
			CEditString strEdit;
			if( pAppletFunc->m_cHotkey == 0 )
				strEdit.AddString( pAppletFunc->m_pAppletDesc );
			else
			{
				strEdit.AddString( pAppletFunc->m_pAppletDesc );
				strEdit.AddString( "\n" );
				CString string;
				string.Format( "[%s %c]", prj.GetText( TID_GAME_TOOLTIP_HOTKEY ), pAppletFunc->m_cHotkey );
				strEdit.AddString( string, 0xff0000ff );
			}
			//CEditString strEdit;
			//strEdit.SetParsingString( string );
			//strEdit.SetAt( 0, (CHAR)(aaa >> 8) );
			//strEdit.SetAt( 1, (CHAR)(aaa & 0xff00) );
			//aaa++;
			//TRACE( "aaa %x\n", aaa );
			g_toolTip.PutToolTip( pShortcut->m_dwId, strEdit, *pRect, point, 0 );
			//} while( aaa < 0xffff );
		}
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == SHORTCUT_EMOTICON )
	{
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_ITEM  )
	{
		g_WndMng.PutToolTip_Item( pShortcut->m_dwType, pShortcut->m_dwId, point, pRect );
	}
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )
	{
		if( pShortcut->m_dwType == 2 )
		{
			g_WndMng.PutToolTip_Troupe( pShortcut->m_dwId, point, pRect );
		}
		else
//		if( pShortcut->m_dwType == 0 || pShortcut->m_dwType == 1 || pShortcut->m_dwType == 3 )
		{
			LPSKILL lpSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
			g_WndMng.PutToolTip_Skill( lpSkill->dwSkill, lpSkill->dwLevel, point, pRect );
		}
	}
#if __VER >= 12 // __LORD
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_LORDSKILL )
	{
		CCLord*							pLord		= CCLord::Instance();
		CLordSkillComponentExecutable*	pComponent	= pLord->GetSkills()->GetSkill(pShortcut->m_dwId);
		CString							string, str;
		CEditString						strEdit;

		if(!pComponent) return;

		string.Format( "#b#cff2fbe6d%s#nb#nc\n%s", pComponent->GetName(), pComponent->GetDesc());
		if(pComponent->GetTick() != 0)
		{
			str.Format( prj.GetText(TID_TOOLTIP_COOLTIME), pComponent->GetTick(), 0);
			string += "\n";
			string += str;
		}
	
		strEdit.SetParsingString( string );
		g_toolTip.PutToolTip(10000, strEdit, *pRect, point, 0);
	}
#endif
	else	
	if( pShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		MotionProp* pMotionProp = prj.GetMotionProp( pShortcut->m_dwId );
		if(!pMotionProp)		//061206 ma	8차에 들어갈 모션관리를 위해 버전 추가	propMotion.txt
			return;

		CString string;

		string.Format( "%s", pMotionProp->szDesc );
		
		if( pMotionProp->dwID == MOT_BASE_CHEER )
		{
			if( g_pPlayer )
			{
				CString str;
				str.Format( prj.GetText(TID_CHEER_MESSAGE1), g_pPlayer->m_nCheerPoint );

				string += "\n\n";
				string += str;

				if( g_pPlayer->m_nCheerPoint < MAX_CHEERPOINT )
				{
					CTimeSpan ct( ((g_pPlayer->m_dwTickCheer - GetTickCount()) / 1000) );		// 남은시간을 초단위로 변환해서 넘겨줌

					if( ct.GetMinutes() <= 0 )
					{
						str.Format( prj.GetText(TID_PK_LIMIT_SECOND), ct.GetSeconds() );
					}
					else
					{
						str.Format( prj.GetText(TID_PK_LIMIT_MINUTE), ct.GetMinutes() );
					}
					
					string += "\n";
					string += (prj.GetText(TID_CHEER_MESSAGE2)+str);
				}
			}
		}

		CEditString strEdit;
		strEdit.SetParsingString( string );
		g_toolTip.PutToolTip( pShortcut->m_dwId , strEdit, *pRect, point, 0 );
	}
	else
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILLFUN )
	{
		strcpy( pShortcut->m_szString, prj.GetText(TID_GAME_SKILLSHORTCUT) );
		g_toolTip.PutToolTip( 10000, pShortcut->m_szString, *pRect, point, 0 );
	}
		
}

void CWndTaskBar::OnMouseWndSurface( CPoint point )
{
	CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( lpShortcut->m_dwId );
			
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point, &rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}

	rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( !IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( SKILL_SIZE, 0 );
	}

	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);

	if( rect.PtInRect( point) )
	{
		SHORTCUT Shortcut;
		Shortcut.m_dwShortcut = SHORTCUT_SKILLFUN;
		PutTooTip( &Shortcut, point,&rect );
	}
}

#define DRAW_HOTKEY( p2DRender, point, cHotkey ) { CString string; string.Format( "%c", cHotkey ); \
					p2DRender->TextOut( point.x - 0 + 2, point.y - 0 - 4, string, 0xffffffff ); }
					
					
/*
#define DRAW_HOTKEY( p2DRender, point, cHotkey ) {  \
	if( cHotkey >= 'A' && cHotkey <= 'Z' ) m_texPack.GetAt( cHotkey - 'A' )->Render( p2DRender, point, m_nAlphaCount );  \
	if( cHotkey >= '0' && cHotkey <= '9' ) m_texPack.GetAt( cHotkey - '0' + 26 )->Render( p2DRender, point, m_nAlphaCount ); }
*/


void CWndTaskBar::OnDraw( C2DRender* p2DRender )
{
	if( NULL == g_pPlayer )
		return;

	DWORD dwCur = g_tmCurrent;
	CRect rect = GetWindowRect();
	CRect rectSkillQueue;

	CD3DFont* pOldFont = p2DRender->GetFont(); 
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontWndTitle ); 
		
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		 rectSkillQueue.SetRect( rect.Width() - 32 * 4 - 6, 0, rect.Width(), rect.Height() );
	}
	else
	if( m_nPosition == TASKBAR_LEFT || m_nPosition == TASKBAR_RIGHT )
	{
		rectSkillQueue.SetRect( 0, rect.Height() - 32 * 4 - 6, rect.Width(), rect.Height() );
	}

	// 드래그 하이라이트 슬롯 표시
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		DWORD dwColor;
		if( m_dwHighAlpha < 128 )
			dwColor = D3DCOLOR_ARGB( m_dwHighAlpha, 0, 0, 255 );
		else
			dwColor = D3DCOLOR_ARGB( 128 - ( m_dwHighAlpha - 128 ), 0, 0, 255 );

		LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
		LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
		LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

		CRect rectApplet = pApplet->rect;
		CRect rectItem = pItem->rect;
		CRect rectSkill = pSkill->rect;

		rectApplet.top = 42;
		rectApplet.bottom = 47;
		rectApplet.left -= 6;
		rectApplet.right += 6;
		rectItem.top = 42;
		rectItem.bottom = 47;
		rectItem.left -= 6;
		rectItem.right += 6;
		rectSkill.top = 42;
		rectSkill.bottom = 47;
		rectSkill.left -= 3;
		rectSkill.right += 4;
		
		switch( m_GlobalShortcut.m_dwShortcut )
		{
			case SHORTCUT_APPLET:
			case SHORTCUT_ITEM:
			case SHORTCUT_MOTION:
				p2DRender->RenderFillRect( rectApplet, dwColor );
				p2DRender->RenderFillRect( rectItem, dwColor );
				break;
			case SHORTCUT_SKILL:
				if( m_GlobalShortcut.m_dwType == 0 )
				{
					DWORD dwSkill = g_pPlayer->GetSkill( 0, m_GlobalShortcut.m_dwId )->dwSkill;
					DWORD dwComboStyleSrc = prj.GetSkillProp( dwSkill )->dwComboStyle;
					if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// 스텝이나 제네럴이면 합격
						p2DRender->RenderFillRect( rectItem, dwColor );		
					p2DRender->RenderFillRect( rectSkill, dwColor );
				}
				else
					p2DRender->RenderFillRect( rectItem, dwColor );
				break;
		}		
	}

	// 애플렛 아이콘 출력
	CPoint point = POINT_APPLET;
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				CItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				CItemElem* pItemElem = (CItemElem*)pItemBase;
				if( pItemElem )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];
#if __VER >= 8	// __JEFF_VER_8
						_stprintf( szTemp, "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemCount( pItemElem->m_dwItemId ): 0 );
#else	// __JEFF_VER_8
						_stprintf( szTemp, "%d", pItemElem->m_nItemNum );
#endif	// __JEFF_VER_8
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
						p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
					DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
					if( dwGroup )
					{
						DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// 이벤트 종료 시각 
						if( dwEnd != 0 && dwEnd > dwCur )  
						{
							DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// 이벤트 시작 시각 
							RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
						}
					}
				}
				else
				{
#if __VER >= 8	// __JEFF_VER_8
					ItemProp* pItemProp	= prj.GetItemProp( lpShortcut->m_dwItemId );
					if( pItemProp && pItemProp->dwPackMax > 1 )	// 병합 가능한 아이템이면?
					{
						DWORD dwId	= g_pPlayer->m_Inventory.Find( lpShortcut->m_dwItemId );
						if( dwId != NULL_ID )	// 같은 종류의 아이템이 있다면?
						{
							lpShortcut->m_dwId	= dwId;
							g_DPlay.SendAddAppletTaskBar( i, lpShortcut );
						}
						else
						{
							lpShortcut->Empty();
							g_DPlay.SendRemoveAppletTaskBar( i );
						}
					}
					else
					{
						lpShortcut->Empty();
						g_DPlay.SendRemoveAppletTaskBar( i );
					}
#else	// __JEFF_VER_8
					lpShortcut->Empty();
					g_DPlay.SendRemoveAppletTaskBar( i );
#endif	// __JEFF_VER_8
				}
			}
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_APPLET )
			{
				AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( lpShortcut->m_dwId ); 
				if( pAppletFunc && pAppletFunc->m_cHotkey )
				{
					CPoint ptHotkey( point.x + 8, point.y - 9 );
					DRAW_HOTKEY( p2DRender, ptHotkey, pAppletFunc->m_cHotkey );
				}
			}
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //극단스킬은 쿨타임 관련 Render를 하지 않는다.
			{
				RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		#if __VER >= 12 // __LORD
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
					RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		#endif
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					ItemProp* pItem = prj.GetItemProp( g_AddSMMode.dwSMItemID[SM_ESCAPE] );
					
					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}	
		}
		point += CPoint( ICON_SIZE, 0 );
	}
	// 아이템 아이콘 출력 
	point = POINT_ITEM;
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				CItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				CItemElem* pItemElem = (CItemElem*)pItemBase;
				if( pItemElem  )
				{
					if( pItemElem->GetProp()->dwPackMax > 1 )
					{
						TCHAR szTemp[ 32 ];
#if __VER >= 8	// __JEFF_VER_8
						_stprintf( szTemp, "%d", g_pPlayer?g_pPlayer->m_Inventory.GetItemCount( pItemElem->m_dwItemId ): 0 );
#else	// __JEFF_VER_8
						_stprintf( szTemp, "%d", pItemElem->m_nItemNum );
#endif	// __JEFF_VER_8
						CSize size = m_p2DRender->m_pFont->GetTextExtent( szTemp );
						p2DRender->TextOut( point.x + 32 - size.cx, point.y + 32 - size.cy, szTemp, 0xff0000ff );
						p2DRender->TextOut( point.x + 31 - size.cx, point.y + 31 - size.cy, szTemp, 0xffb0b0f0 );
					}
				}

				DWORD dwGroup = g_pPlayer->m_cooltimeMgr.GetGroup( pItemElem->GetProp() );
				if( dwGroup )
				{
					DWORD dwEnd = g_pPlayer->m_cooltimeMgr.GetTime( dwGroup );		// 이벤트 종료 시각 
					if( dwEnd != 0 && dwEnd > dwCur )
					{
						DWORD dwBase = g_pPlayer->m_cooltimeMgr.GetBase( dwGroup );	// 이벤트 시작 시각 
						RenderRadar( p2DRender, point, dwCur - dwBase, dwEnd - dwBase );
					}
				}
			} 
			else if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL && lpShortcut->m_dwType != 2 ) //극단스킬은 쿨타임 관련 Render를 하지 않는다.
			{
				RenderCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		#if __VER >= 12 // __LORD
			else if( lpShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
					RenderLordCollTime( point, lpShortcut->m_dwId, p2DRender );
			}
		#endif
			else
			if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
			{
				if( lpShortcut->m_dwId == MOT_BASE_ESCAPE )
				{
					ItemProp* pItem = prj.GetItemProp( g_AddSMMode.dwSMItemID[SM_ESCAPE] );

					if( pItem && g_pPlayer )
					{
						CTimeSpan ct( g_pPlayer->m_dwSMTime[SM_ESCAPE] );
						
						if( ct.GetTotalSeconds() )
						{
							point.y -= 1;
							point.x += 1;
							RenderRadar( m_p2DRender, point, (DWORD)( pItem->dwCircleTime - ct.GetTotalSeconds() ), pItem->dwCircleTime );
						}
					}
				}
			}

				
		}
		// hotkey 출력 
		CHAR cHotkey = ( i == 9 ) ? '0' : '1' + i;
		CPoint ptHotkey( point.x + 8, point.y - 9 );
		//DRAW_HOTKEY( p2DRender, ptHotkey, cHotkey );
		point += CPoint( ICON_SIZE, 0 );
	}
	
	point = CPoint( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y );
	p2DRender->RenderTexture( point, m_pTexture );

	// 스킬 큐 
	point = POINT_QUEUE;
	//p2DRender->TextOut( point.x - 5, point.y - 16, "ACTION SLOT" );
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_pTexture )
			{
				p2DRender->RenderTexture( point, lpShortcut->m_pTexture );
//				LPSKILL pSkill = g_pPlayer->GetSkill( lpShortcut->m_dwType, lpShortcut->m_dwId );
//				if( g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] > 0 )
//					p2DRender->TextOut( point.x, point.y, g_pPlayer->m_nReUseDelay[ lpShortcut->m_dwId ] );

			}
		}
		point += CPoint( SKILL_SIZE, 0 );
	}

	
	if( !( g_pPlayer->IsSMMode( SM_ACTPOINT ) ) )
	{
	#define LV2MAXAP	10		// 2단계 스킬이 다차는데 필요한 action point
	#define LV3MAXAP	25		
	#define LV4MAXAP	50
	#define LV5MAXAP	100

		point = POINT_QUEUE;
		point.x += SKILL_SIZE;	// 스킬 2번째칸부터 쉐이드된다.
		{
			int nAP2Size;	// 액션포인트를 하이라이트양으로 변환한값.
			if( m_nActionPoint >= LV4MAXAP )	// LV4MAXAP를 넘어서면 5단계 시작.
				nAP2Size = (SKILL_SIZE * 3) + (((m_nActionPoint - LV4MAXAP) * SKILL_SIZE) / (LV5MAXAP - LV4MAXAP));
			else if( m_nActionPoint >= LV3MAXAP )
				nAP2Size = (SKILL_SIZE * 2) + (((m_nActionPoint - LV3MAXAP) * SKILL_SIZE) / (LV4MAXAP - LV3MAXAP));
			else if( m_nActionPoint >= LV2MAXAP )
				nAP2Size = (SKILL_SIZE * 1) + (((m_nActionPoint - LV2MAXAP) * SKILL_SIZE) / (LV3MAXAP - LV2MAXAP));
			else 
				nAP2Size = (m_nActionPoint * SKILL_SIZE) / LV2MAXAP;
			
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			p2DRender->m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
			point.x += nAP2Size;
			nAP2Size = (SKILL_SIZE * 4) - nAP2Size;
			p2DRender->RenderFillRect( CRect(point.x, point.y,  point.x + nAP2Size, point.y + SKILL_SIZE + 2), 
										D3DCOLOR_ARGB( 0x7f, 0, 0, 0 ) );
			
			if( m_nCurQueueNum > 0 )
			{
				int show = 1;
				
				if( m_nActionPoint >= LV2MAXAP )
					show = 2;
				if( m_nActionPoint >= LV3MAXAP )
					show = 3;
				if( m_nActionPoint >= LV4MAXAP )
					show = 4;
				if( m_nActionPoint >= LV5MAXAP )
					show = 5;
				
				if( m_nCurQueueNum <= show )
					show = m_nCurQueueNum;
				
				CPoint Point = CPoint( POINT_QUEUE_X, POINT_QUEUE_Y+1 );

				RenderOutLineLamp( POINT_QUEUE_X, POINT_QUEUE_Y+1, show, SKILL_SIZE );
			}
		}
	}

	CWndButton* pWndButton = (CWndButton*)GetDlgItem( WIDC_UP );
	rect = pWndButton->GetWindowRect( TRUE );
	rect.top = rect.bottom - 2;
	p2DRender->TextOut( rect.left, rect.top, m_nSlotIndex + 1, 0xffffffff );
	p2DRender->SetFont( pOldFont );
}

void CWndTaskBar::UpdateItem()
{
	if( g_pPlayer == NULL )
		return;
	
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() )
		{
			if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
			{
				CItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
				CItemElem* pItemElem = (CItemElem*)pItemBase;

				if( pItemElem )
				{
					if( lpShortcut->m_pTexture != pItemElem->GetTexture() )
						lpShortcut->m_pTexture = pItemElem->GetTexture();
				}
				else
				{
#if __VER >= 8	// __JEFF_VER_8
					ItemProp* pItemProp	= prj.GetItemProp( lpShortcut->m_dwItemId );
					if( pItemProp && pItemProp->dwPackMax > 1 )	// 병합 가능한 아이템이면?
					{
						DWORD dwId	= g_pPlayer->m_Inventory.Find( lpShortcut->m_dwItemId );
						if( dwId != NULL_ID )	// 같은 종류의 아이템이 있다면?
						{
							lpShortcut->m_dwId	= dwId;
							g_DPlay.SendAddItemTaskBar( m_nSlotIndex, i, lpShortcut );
						}
						else
						{
							lpShortcut->Empty(); 				
							g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, i );
						}
					}
					else
					{
						lpShortcut->Empty(); 				
						g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, i );
					}
#else	// __JEFF_VER_8
					lpShortcut->Empty(); 				
					g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, i );
#endif	// __JEFF_VER_8
				}
			}
		}
	}
}

void CWndTaskBar::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
}
void CWndTaskBar::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	LPWNDCTRL pApplet = GetWndCtrl( WIDC_CUSTOM1 );
	LPWNDCTRL pItem   = GetWndCtrl( WIDC_CUSTOM2 );
	LPWNDCTRL pSkill  = GetWndCtrl( WIDC_CUSTOM3 );

	POINT_APPLET_X = pApplet->rect.left;
	POINT_APPLET_Y = pApplet->rect.top;
	POINT_ITEM_X   = pItem->rect.left;
	POINT_ITEM_Y   = pItem->rect.top;
	POINT_QUEUE_X  = pSkill->rect.left;
	POINT_QUEUE_Y  = pSkill->rect.top;

	CRect rect( 5, 7, 754, 40 );

	g_WndMng.m_pWndTaskBar = this;

	{
		rect = g_Neuz.GetDeviceRect();
		m_pWndRoot->m_rectLayout = rect;
		switch( m_nPosition )
		{
		case TASKBAR_TOP:
			rect.bottom = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.top = rect.bottom;
			break;
		case TASKBAR_BOTTOM:
			rect.top = rect.bottom - TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.bottom = rect.top;
			break;
		case TASKBAR_LEFT:
			rect.right = TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.left = rect.right;
			break;
		case TASKBAR_RIGHT:
			rect.left = rect.right - TASKBAR_HEIGHT;
			m_pWndRoot->m_rectLayout.right = rect.left;
			break;
		}
		SetWndRect( rect );
	}
	m_texPack.LoadScript( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "hotkey.inc" ) );

	m_menuShortcut.CreateMenu( this );	
	m_menuShortcut.AppendMenu( 0, 0 ,_T( "삭제" ) );

	m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, "icon_ActionSkill.dds" ), 0xffff00ff );
	m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;
	m_aSlotSkill.m_pTexture   = m_pTexture;
}

HRESULT CWndTaskBar::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.InvalidateDeviceObjects();
#endif //__YDEBUG
	return S_OK;
}

HRESULT CWndTaskBar::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.DeleteDeviceObjects();
#endif //__YDEBUG
	return S_OK;
}

HRESULT CWndTaskBar::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	
#ifdef __YDEBUG
	m_texPack.RestoreDeviceObjects(m_pApp->m_pd3dDevice);
#endif //__YDEBUG
	
	return 0;
}

BOOL CWndTaskBar::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CRect rect = g_Neuz.GetDeviceRect();
	//m_rectLayout = rect;
/*
	switch( m_nPosition )
	{
	case TASKBAR_TOP:
		rect.bottom = TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.top = rect.bottom;
		break;
	case TASKBAR_BOTTOM:
		rect.top = rect.bottom - TASKBAR_HEIGHT;
		m_pWndRoot->m_rectLayout.bottom = rect.top;
		break;
	case TASKBAR_LEFT:
		break;
	case TASKBAR_RIGHT:
		break;
	}
	*/
	//m_pWndRoot->SetWndRect( rectRoot );

//CMainFrame
	//rect.top = rect.bottom;
	//rect.bottom += TASKBAR_HEIGHT;
//	rect.bottom = CLIENT_HEIGHT;
	//m_strMessage = lpszMessage;
	//m_dwType = nType;
	//SetTitle("메지시 윈도");
//	return CWndBase::Create( 0 | /*WBS_MODAL|*/ WBS_MANAGER | WBS_SOUND | WBS_NOFRAME, rect, &g_WndMng, APP_TASKBAR );
	BOOL bResult;
	if( g_Option.m_nResWidth == 800 )
	{
		m_nMaxSlotApplet = 5;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR800, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else
	if( g_Option.m_nResWidth == 1024 )
	{
		m_nMaxSlotApplet = 10;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1024, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else
	if( g_Option.m_nResWidth == 1280 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1280, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
#if __VER >= 9 // __CSC_VER9_RESOLUTION
	else if( g_Option.m_nResWidth == 1360 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1360W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1400 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1400, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1440 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1440W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1600 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1600, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}
	else if( g_Option.m_nResWidth == 1680 )
	{
		m_nMaxSlotApplet = MAX_SLOT_APPLET;
		bResult = CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TASKBAR1680W, WBS_MANAGER | WBS_SOUND, CPoint( 0, 0 ), pWndParent );
	}	
#endif //__CSC_VER9_RESOLUTION
	
	DelWndStyle( WBS_MOVE );
	return bResult;
	
}
BOOL CWndTaskBar::RemoveDeleteObj()
{
	/*
	for( int i = 0; i < m_awndShortCut.GetSize(); i++ )
	{
		CWndButton* pWndButton = (CWndButton*)m_awndShortCut.GetAt( i );
		if( pWndButton ) 
		{
			if( pWndButton->m_shortcut.m_dwShortcut == SHORTCUT_OBJECT )
			{
				CObj* pObj = (CObj*) pWndButton->m_shortcut.m_dwData;
				if( IsValidObj( pObj ) == FALSE )
					pWndButton->m_shortcut.m_dwData = 0;
			}
			else
			if( pWndButton->m_shortcut.m_dwShortcut == SHORTCUT_ITEM )
			{
				CItemBase* pItemBase = (CItemBase*) pWndButton->m_shortcut.m_dwData;
				if( pItemBase && pItemBase->IsEmpty() )
					pWndButton->Destroy( TRUE );
			}
		}
	}
	*/
	return TRUE;
}
void CWndTaskBar::SetItemSlot( int nSlot )
{
	if( nSlot >= 0 && nSlot < MAX_SLOT_ITEM_COUNT)
		m_paSlotItem = m_aSlotItem[ m_nSlotIndex = nSlot ];
}
BOOL CWndTaskBar::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	CWndButton* pWndButton = (CWndButton*) pLResult;
	if( message == WIN_ITEMDROP )
	{
		/*
		LPSHORTCUT pShortcut = (LPSHORTCUT) pLResult;
		if( pShortcut && pShortcut->m_dwData == 2 ) // 스킬 큐 버튼의 아이디는 1000 이상 
		{
			SetSkillQueue( nID - 1000, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_dwData, pShortcut->m_pTexture );
		}
		*/
	}
	else
	switch( nID )
	{
		case 101:
			/*
			if( m_pWndQuickList )
				m_pWndQuickList->Destroy();
			else
			{
				m_pWndQuickList = new CWndQuickList;
				m_pWndQuickList->Initialize( this );//&g_WndMng );
			}
			*/
			break;
		case WIDC_UP:
			m_nSlotIndex--;
			if( m_nSlotIndex < 0 ) m_nSlotIndex = MAX_SLOT_ITEM_COUNT-1;
			m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
			break;
		case WIDC_DOWN:
			m_nSlotIndex++;
			if( m_nSlotIndex > MAX_SLOT_ITEM_COUNT-1 ) m_nSlotIndex = 0;
			m_paSlotItem = m_aSlotItem[ m_nSlotIndex ];
			break;
		case WIDC_BUTTON1: // Menu
		{
			g_WndMng.OpenMenu();
			CRect rect = g_Neuz.GetDeviceRect();
			CRect rectMenu = m_wndMenu.GetScreenRect();
			switch( m_nPosition )
			{
			case TASKBAR_TOP:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, TASKBAR_HEIGHT ) );
				break;
			case TASKBAR_BOTTOM:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( 0, rect.bottom - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Height() ) );
				break;
			case TASKBAR_LEFT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( TASKBAR_HEIGHT, 0 ) );
				break;
			case TASKBAR_RIGHT:
				if( g_WndMng.m_pWndMenu )
					g_WndMng.m_pWndMenu->Move( CPoint( rect.right - TASKBAR_HEIGHT - g_WndMng.m_pWndMenu->GetWndRect().Width(), 0 ) );
				break;
			}

		}
		default:
			{
				switch( pWndButton->m_shortcut.m_dwIndex )
				{
					case APP_TRADE:		g_DPlay.SendTrade( (CMover*)g_WorldMng.Get()->GetObjFocus() );	break;
					default:
						g_WndMng.ObjectExecutor( &pWndButton->m_shortcut );	
						break;
				}
			}
	}
	return TRUE;
}
BOOL CWndTaskBar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
	return TRUE;
}
void CWndTaskBar::OnSize(UINT nType, int cx, int cy)
{
	CWndNeuz::OnSize( nType, cx, cy );
//	CRect rect = GetClientRect();
//	rect.DeflateRect( 5, 5);
//	rect.right = 37;
	//ect rect( 5, 5, 44, 40 );//GetClientRect();
	//wndMenu.SetWndRect( rect );

//ect = GetClientRect();

	// Create Command button
	//for(int i = 0; i < 10; i++)
		//m_pWndCommand[i].SetWndRect(CRect(60+i*18,3,76+i*18,21));

	//int i;
	// Create Tray button
	//int nMax = 3;
	//int nStart = (rect.Width() - nMax * 26);
	//for(i = 0; i < nMax; i++)
//		m_pWndTray[i].SetWndRect(CRect(nStart + i * 25, 6, nStart + 24 + i * 25, 30));
}
void CWndTaskBar::RemoveSkillQueue( int nIndex, BOOL bSend )
{
	if( m_nExecute )	return;		// 스킬큐 실행중엔 빼지지도 않는다.
BACK:
	if( !m_aSlotQueue[ nIndex ].IsEmpty() )
	{
		ItemProp* pItemProp = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nIndex ].m_dwId )->GetProp();
		DWORD dwComboStyleSrc = pItemProp->dwComboStyle;
		int i = NULL;
		for( i = nIndex; i < MAX_SLOT_QUEUE - 1; i++ )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i + 1 ], sizeof( SHORTCUT ) );
			m_aSlotQueue[ i ].m_dwIndex = i;
		}
		m_aSlotQueue[ i ].m_dwShortcut = SHORTCUT_NONE;
		m_aSlotQueue[ i ].m_dwId = 0;
		m_nCurQueueNum--;
		if( m_nCurQueueNum < 0 ) 
			m_nCurQueueNum = 0;
		if( CheckAddSkill( dwComboStyleSrc, nIndex /*- 1*/ ) == FALSE )
			goto BACK;
	}
	if( bSend )
		g_DPlay.SendSkillTaskBar( );
	// 스킬을 지우는 곳 서버로 보내야함.	

}
void CWndTaskBar::OnRButtonUp( UINT nFlags, CPoint point )
{
	CRect rect = RECT_APPLET;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.x /= ICON_SIZE;
		if( FALSE == m_aSlotApplet[ point.x ].IsEmpty() )
		{
#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == 400 )
				return;
#endif //__MAINSERVER
			
			m_aSlotApplet[ point.x ].Empty();			
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( point.x ) );		// 아이템 삭제 서버로 전송
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;

		point.x /= ICON_SIZE;
		if( FALSE == m_paSlotItem[ point.x ].IsEmpty() )
		{
			m_paSlotItem[ point.x ].Empty();
			g_DPlay.SendRemoveItemTaskBar( (BYTE)( m_nSlotIndex ), (BYTE)( point.x ) );	// 아이템 삭제 서버로 전송			
		}

	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		RemoveSkillQueue( point.x );
	}
}

BOOL CWndTaskBar::SetShortcut( int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture, int nWhere )
{
	LPSHORTCUT pShortcut = NULL;
	//CWndShortcut* pWndButton = new CWndShortcut;
//	pWndButton->Create( _T( "" ), 0, CRect( 65 + nIndex * 32, 3, 65 + nIndex * 32 + 32 , 3 + 32), this, dwId );

	//pShortcut = m_aSlotApplet[ nIndex ];
#if __VER >= 11 // __CSC_VER11_5
	// Chat Shortcut 10개로 제한
	if(dwShortcut == SHORTCUT_CHAT)
	{
		int nchatshortcut = 0;
		for( int i=0; i<MAX_SLOT_ITEM_COUNT; i++ )
		{
			for( int j=0; j<MAX_SLOT_ITEM; j++ )
			{
				if( m_aSlotItem[i][j].m_dwShortcut == SHORTCUT_CHAT )
					nchatshortcut++;
			}
		}
		for( int k = 0; k < m_nMaxSlotApplet; ++k )
			if( m_aSlotApplet[ k ].m_dwShortcut == SHORTCUT_CHAT )
				++nchatshortcut;

		if(nchatshortcut > 9)
		{
			g_WndMng.PutString( prj.GetText( TID_GAME_MAX_SHORTCUT_CHAT ), NULL, prj.GetTextColor( TID_GAME_MAX_SHORTCUT_CHAT ) );
			return FALSE;
		}
	}
#endif //__CSC_VER11_5

	switch( m_nPosition )
	{
	case TASKBAR_TOP:
	case TASKBAR_BOTTOM:
		if( nWhere == 0 )
			pShortcut = &m_aSlotApplet[ nIndex ];
		else
		if( nWhere == 1 )
			pShortcut = &m_paSlotItem[ nIndex ];
		break;
	case TASKBAR_LEFT:
	case TASKBAR_RIGHT:
		if( nWhere == 0 )
			pShortcut = &m_aSlotApplet[ nIndex ];
		else
		if( nWhere == 1 )
			pShortcut = &m_paSlotItem[ nIndex ];
		break;
	}
	if( pTexture == NULL )
	{
		CItemBase* pItemBase;
		if( dwShortcut == SHORTCUT_APPLET ) 
		{
			AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( dwId );
			pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice,  MakePath( DIR_ICON, pAppletFunc->m_pszIconName ), 0xffff00ff );
				//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pAppletFunc->m_pszIconName ) );
			//pCardElem->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pCardElem->GetProp()->szIcon), 0xff000000 );

		}
		else
		if( dwShortcut == SHORTCUT_ITEM ) 
		{
			pItemBase = g_pPlayer->GetItemId( dwId );
			CTexture* pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xffff00ff );
			pItemBase->SetTexture( pTexture ); 
			pShortcut->m_pTexture = pTexture ;
		}
		else
		if( dwShortcut == SHORTCUT_MOTION ) 
		{
			MotionProp* pMotionProp = prj.GetMotionProp( dwId );
			if(pMotionProp)		//061206 ma	8차에 들어갈 모션관리를 위해 버전 추가	propMotion.txt
			{
				pMotionProp->pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pMotionProp->szIconName ), 0xffff00ff );
				pShortcut->m_pTexture = pMotionProp->pTexture;
				//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
			}
			else
				return FALSE;
		}
		/*
		else
		if( dwShortcut == SHORTCUT_CARD ) 
		{
			pItemBase = g_pPlayer->m_Card.GetAt( dwId );//m_apItem[ i ] ;
			pItemBase->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xff000000 );
			pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
		}
		else
		if( dwShortcut == SHORTCUT_JACKBOX ) 
		{
			pItemBase = g_pPlayer->m_Cube.GetAt( dwId );//m_apItem[ i ] ;
			pItemBase->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemBase->GetProp()->szIcon), 0xff000000 );
			pWndButton->SetTexture( g_Neuz.m_pd3dDevice, pItemBase->m_pTexture );
		}
		*/
		else
		if( dwShortcut == SHORTCUT_SKILL ) 
		{
			if( pShortcut->m_dwType == 2 )
			{
				ItemProp* pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, pProp->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff );
			}
			else
			{	
				LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );
				//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon ) );
				pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, lpSkill->GetProp()->szIcon/*pItemBase->GetProp()->szIcon*/), 0xffff00ff, TRUE );
			}
		}	
		else
		if( dwShortcut == SHORTCUT_SKILLFUN ) 
			pShortcut->m_pTexture = pTexture;
			
	}
	else 
		pShortcut->m_pTexture = pTexture;

#if __VER >= 8	// __JEFF_VER_8
	if( dwShortcut == SHORTCUT_ITEM )
	{
		CItemBase* pItemBase	= g_pPlayer->GetItemId( dwId );
		if( pShortcut && pItemBase && pItemBase->GetProp()->dwPackMax > 1 )	// 병합 가능한 아이템이면?
			pShortcut->m_dwItemId	= pItemBase->m_dwItemId;
	}
#endif	// __JEFF_VER_8

	pShortcut->m_dwShortcut = dwShortcut   ;
	pShortcut->m_dwType     = dwType;
	pShortcut->m_dwIndex    = nIndex;//0;//dwIndex;
	pShortcut->m_dwId       = dwId;
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = nWhere;
	strcpy( pShortcut->m_szString, m_GlobalShortcut.m_szString );//, sizeof(pShortcut->m_szString) );
	//pWndButton->SetTitle( m_GlobalShortcut.m_szString );
#if __VER >= 12 // __LORD
	if( dwShortcut == SHORTCUT_LORDSKILL)
		pShortcut->m_dwId--;
#endif
	if( nWhere == 0 ) //m_aSlotApplet
	{
		g_DPlay.SendAddAppletTaskBar( nIndex, pShortcut );
	}
	else
	if( nWhere == 1 ) //m_paSlotItem
	{
		g_DPlay.SendAddItemTaskBar( m_nSlotIndex, nIndex, pShortcut );
	}
	return TRUE;
}
BOOL CWndTaskBar::CheckAddSkill( int nSkillStyleSrc, int nQueueDest  )
{
	/*
	START -> FINISH, FG 
	CIRCLE -> START, CIRCLE
	FINISH -> START, CIRCLE
	*/
	DWORD dwComboStyleSrc = nSkillStyleSrc;
	if( nQueueDest == 0 )		// 첫칸에 넣으려 시도 했을경우
	{
		/*
		if( 이미 스킬창에 등록되어 있다 )
		{
			if( 삽입하려는게 CT_GENERAL인가 )
				if( 이미 삽입되어 있는게 CT_GENERAL인가 )
				return TRUE;
			return FALSE;		// CT_GENERAL이외의 것을 첫칸에 삽입하려면 모두 실패.
		} else
		{
			// 아무것도 등록되어 있지 않다.
			// 스텝, 제네럴만 합격.
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  ) 
				return TRUE;
			return FALSE;	// 그외엔 모두 불합격.
		}
		 */
		if( m_nCurQueueNum == 0 )		// 비어있다
		{
			if( dwComboStyleSrc == CT_STEP  || dwComboStyleSrc == CT_GENERAL  )		// 스텝이나 제네럴이면 합격
				return TRUE;
			return	FALSE;
		} else
		// 뭔가 있다.
		{
			DWORD dwComboStyleDest;
			DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest ].m_dwId )->dwSkill;
				//m_aSlotQueue[ nQueueDest ].m_dwId;	// 넣으려는 칸의 스킬
			if( dwSkill )
				dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// 그칸의 스타일
			else 
				dwComboStyleDest = 0;
			if( dwComboStyleSrc == CT_GENERAL && dwComboStyleDest == CT_GENERAL )	// 이미있는것도 제네럴이고 삽입하려는것도 제네럴이면 합격
				return TRUE;
			return FALSE;	// 그외는 모두 첫칸에 삽입할 수 없음.
		}
	}
	else
	if( nQueueDest == 4 )		// 마지막칸에 넣으려고 했을경우
	{
		if( dwComboStyleSrc == CT_STEP )	// 스텝은 첫칸이 아니면 무조건 못넣는다.
			return FALSE;
		DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest - 1 ].m_dwId )->dwSkill;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// 넣으려는 칸의 앞에있는 스킬
		DWORD dwComboStylePrev;
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// 앞칸의 스타일
		else
			dwComboStylePrev = 0;
		if( dwComboStylePrev == CT_FINISH )		// 앞 스킬이 피니쉬면 더이상 넣을 수 없다.
			return FALSE;
		// 서클기술을 넣으려하고 앞칸에 제네럴이면 못넣음.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// 피니쉬(암놈)를 넣으려하고 앞칸에 제네럴이면 못넣음.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// 제네럴을 넣으려하고 앞칸이 제네럴이 아니면 못넣음.
		if( dwComboStyleSrc == CT_GENERAL && dwComboStylePrev != CT_GENERAL )
			return FALSE;
		
		return TRUE;	// 그외는 모두 합격
	}
	else
	// 그외 중간에 넣으려 한경우.
	{
		// 둘째칸 이상부터 넣으려고 했을때 처리.
		if( dwComboStyleSrc == CT_STEP )	// 스텝은 첫칸이 아니면 무조건 못넣는다.
			return FALSE;
		DWORD dwComboStylePrev;
		DWORD dwComboStyleDest;
		DWORD dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest - 1 ].m_dwId )->dwSkill;
			//m_aSlotQueue[ nQueueDest - 1 ].m_dwId;	// 넣으려는 칸의 앞에있는 스킬
		if( dwSkill )
			dwComboStylePrev = prj.GetSkillProp( dwSkill )->dwComboStyle;		// 앞칸의 스타일
		else
			dwComboStylePrev = 0;
		dwSkill = g_pPlayer->GetSkill( 0, m_aSlotQueue[ nQueueDest ].m_dwId )->dwSkill;
		//dwSkill = m_aSlotQueue[ nQueueDest ].m_dwId;	// 넣으려는 칸의 스킬
		if( dwSkill )
			dwComboStyleDest = prj.GetSkillProp( dwSkill )->dwComboStyle;		// 그칸의 스타일
		else
			dwComboStyleDest = 0;
		if( dwComboStylePrev == CT_FINISH )		// 앞 스킬이 피니쉬면 더이상 넣을 수 없다.
			return FALSE;
		// 서클기술을 넣으려하고 앞칸에 제네럴이면 못넣음.
		if( dwComboStyleSrc == CT_CIRCLE && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// 피니쉬(암놈)를 넣으려하고 앞칸에 제네럴이면 못넣음.
		if( dwComboStyleSrc == CT_FINISH && dwComboStylePrev == CT_GENERAL )
			return FALSE;
		// 피니쉬를 넣으려하고 그칸에 서클이면 못넣음.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_CIRCLE )
			return FALSE;
		// 피니쉬를 넣으려하고 그칸에 피니쉬이면 못넣음.
		if( dwComboStyleSrc == CT_FINISH && dwComboStyleDest == CT_FINISH )
			return FALSE;
		// 제네럴을 넣으려하고 
		if( dwComboStyleSrc == CT_GENERAL )
		{
			if( dwComboStylePrev != CT_GENERAL )	// 앞칸이 제네럴이 아니면 못넣음.
				return FALSE;
			if( dwComboStyleDest != CT_GENERAL && m_nCurQueueNum > nQueueDest )	// 그칸이 제네럴이 아니면 못넣는데 맨 끝이면 넣을수 있다.
					return FALSE;
		}
		
		return TRUE;	// 그 외는 모두 합격
	}
	return FALSE;
}
BOOL CWndTaskBar::SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture )
{
	if( m_nExecute )		return FALSE;		// 스킬큐 실행중엔 등록 안됨.
	if( m_nCurQueueNum >= 5 )
		return FALSE;

	// m_nCurQueueNum 보다 작은 슬롯에 비어있는게 있다면 오류 상태다. 클리어하자.
	for( int i = 0; i < m_nCurQueueNum; i++ )
	{
		if( m_aSlotQueue[ i ].IsEmpty() )
		{
			for( i = 0; i < m_nCurQueueNum; i++ )
				m_aSlotQueue[ i ].Empty();
			m_nCurQueueNum = 0;
			break;
		}
	}
	 if( nIndex > m_nCurQueueNum )
		 nIndex = m_nCurQueueNum;

	LPSHORTCUT pShortcut = NULL;
	CRect rect = GetWindowRect();
	LPSKILL lpSkill = g_pPlayer->GetSkill( 0, dwId );
	DWORD dwSkill = lpSkill->dwSkill;
	DWORD dwLevel = lpSkill->dwLevel;
	ItemProp *pSkillProp = prj.GetSkillProp( dwSkill );
	DWORD dwComboStyleSrc = pSkillProp->dwComboStyle;

	if( dwLevel <= 0 )
		return FALSE;

#if __VER >= 9	// __SKILL_0706
	AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, dwLevel );
	ASSERT( pAddSkillProp );
	if( (int)pAddSkillProp->dwCooldown > 0 )	//  쿨타임있는 스킬은 액션슬롯에 못들어감
#else	// __SKILL_0705
	if( (int)(pSkillProp->dwSkillReady) > 0 )	//  쿨타임있는 스킬은 액션슬롯에 못들어감
#endif	// __SKILL_0705
	{
		CString str;
		str.Format( prj.GetText(TID_GAME_SKILLLNOTUSE), pSkillProp->szName );
		g_WndMng.PutString( str , NULL, prj.GetTextColor(TID_GAME_SKILLLNOTUSE) );
		return FALSE;
	}

//	DWORD dwComboStyleDest;
//	ItemProp* pItemProp1, * pItemProp2,* pItemProp3; 
	if( nIndex < m_nCurQueueNum ) // insert		// nIndex는 세팅할 칸의 인덱스
	{
		if( !CheckAddSkill( dwComboStyleSrc, nIndex ) )		// Src를 nIndex에 넣어도 되는지 체크
			return FALSE;
		for( int i = m_nCurQueueNum; i > nIndex; i-- )
		{
			memcpy( &m_aSlotQueue[ i ], &m_aSlotQueue[ i - 1 ], sizeof( SHORTCUT ) );
		}
		pShortcut = &m_aSlotQueue[ nIndex ];
		m_nCurQueueNum++;
	}
	else // add
	{
		if( !CheckAddSkill( dwComboStyleSrc,  m_nCurQueueNum ) )	// 제일 마지막 칸에 넣어도 되는지 테스트.
			return FALSE;
		pShortcut = &m_aSlotQueue[ m_nCurQueueNum ];
		nIndex = m_nCurQueueNum;
		m_nCurQueueNum++;
	}
	m_nCurQueue = -1;
	if( pTexture == NULL )
	{
		LPSKILL lpSkill = g_pPlayer->GetSkill( dwType, dwId );
		pShortcut->m_pTexture = m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, lpSkill->GetProp()->szIcon), 0xff000000, FALSE );
	}
	else pShortcut->m_pTexture = pTexture;


	pShortcut->m_dwShortcut = SHORTCUT_SKILL   ;
	pShortcut->m_dwType     = dwType; // 직업 
	pShortcut->m_dwIndex    = nIndex;//dwIndex; // 스킬 콘트롤에서의 순서 
	pShortcut->m_dwId       = dwId; // 스킬 인덱스 
	pShortcut->m_dwUserId   = 0 ;
	pShortcut->m_dwData     = 2;
	_tcscpy( pShortcut->m_szString, m_GlobalShortcut.m_szString );
	g_DPlay.SendSkillTaskBar();
	// 스킬 등록 여기서 서버로 보내야 함
	return TRUE;
}

BOOL CWndTaskBar::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CRect rect = GetWindowRect();
	rect.DeflateRect( 3, 3);
	// 밖으로 버렸을 경우 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// 포인트가 테스크바가 아닌 곳이라면. 의도적으로 숏컷을 지우려고 태스크바 밖에 놓은 것임. 지우자.
		if( GetClientRect().PtInRect( point ) == FALSE )
		{
			// 아이템 삭제 서버로 전송
			if( lpShortcut->m_dwData == 0 )
				g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 1 )
				g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
			else
			if( lpShortcut->m_dwData == 2 )
			{
				RemoveSkillQueue( lpShortcut->m_dwIndex );
				return TRUE;
				//g_DPlay.SendSkillTaskBar( );
			}
			lpShortcut->Empty();
			return TRUE;
		}
	//	return TRUE;
	}
	CWndBase* pWndFrame =  pShortcut->m_pFromWnd->GetFrameWnd();


#if __VER >= 10 // __CSC_VER9_1
#if __VER >= 12 // __LORD
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL3 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY && pWndFrame->GetWndId() != APP_LORD_SKILL) )
#else
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL3 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY ) )
#endif
#else
	if( pWndFrame && ( pWndFrame->GetWndId() != APP_INVENTORY && pWndFrame != g_WndMng.m_pWndTaskBar && pWndFrame->GetWndId() != APP_COMMUNICATION_CHAT && pWndFrame->GetWndId() != APP_SKILL1 && pWndFrame->GetWndId() != APP_MOTION && pWndFrame->GetWndId() != APP_PARTY ) )
#endif //__CSC_VER9_1
	{
		SetForbid( TRUE );
		return FALSE;
	}
	
	if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType != 2 ) // 극단스킬은 안들어감
	{
		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
		if( pSkill && (pSkill->dwLevel <= 0 || g_pPlayer->CheckSkill( pSkill->dwSkill ) == FALSE) )
		{
			SetForbid( TRUE );
			return FALSE;
		}
	}
#if __VER >= 12 // __LORD
	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN && pShortcut->m_dwShortcut != SHORTCUT_LORDSKILL)
#else
	if( pShortcut->m_dwId == 0 && strlen( pShortcut->m_szString ) < 1 && pShortcut->m_dwShortcut != SHORTCUT_SKILLFUN)
#endif
	{
		SetForbid( TRUE );
		return FALSE;
	}
	// 내부에서 이동되거나, 외부에서 내부로 
	if( m_nPosition == TASKBAR_TOP || m_nPosition == TASKBAR_BOTTOM )
	{
		BOOL bForbid = TRUE;
		CRect rect = RECT_APPLET;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// 내부에서 이동 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;			

				if( &m_aSlotApplet[ point.x ] == lpCurShortcut )
				{
					//SetForbid( TRUE );
					return FALSE;
				}
				// 오리지날이 액션슬롯이다.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
			{
				ItemProp* pProp;
				if( pShortcut->m_dwType == 2 )
					pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				else
				{
					LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
				}
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// 퀵슬롯에 등록하려는 스킬이 스텝기술이 아닌건 등록 못함.
				}
/*				
				ItemProp *pProp = prj.GetSkillProp( pShortcut->m_dwId );	
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// 퀵슬롯에 등록하려는 스킬이 스텝기술이 아닌건 등록 못함.
				}				
				*/
				//if( pProp->dwComboStyle != CT_STEP )
				//{
				//	SetForbid( TRUE );
				//	return FALSE;	// 퀵슬롯에 등록하려는 스킬이 스텝기술이 아닌건 등록 못함.
				//}
			}

#ifdef __MAINSERVER
			if( point.x == 0 && m_aSlotApplet[ point.x ].m_dwId == 400 )
				return FALSE;
#endif //__MAINSERVER
				
			SetShortcut( point.x, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture, 0 );
			bForbid = FALSE;
		}
		rect = RECT_ITEM;
//		rect.right += 32;
		if( rect.PtInRect( point ) )
		{
			point.x -= rect.left;
			point.y -= rect.top;
			point.x /= ICON_SIZE;
			// 내부에서 이동 
			if( pShortcut->m_pFromWnd == this )
			{
				LPSHORTCUT lpCurShortcut = (LPSHORTCUT) pShortcut->m_dwData;
				if( &m_aSlotItem[ m_nSlotIndex ][ point.x ] == lpCurShortcut )
					return FALSE;
				// 오리지날이 액션슬롯이다.
				//if( lpCurShortcut->m_dwData == 2 )
				//	RemoveSkillQueue( lpCurShortcut->m_dwIndex );
			}
			if( pShortcut->m_dwShortcut == SHORTCUT_CHAT )
			{
				CString sChat	= m_GlobalShortcut.m_szString;
				sChat.Replace( "--", "" );
				sChat.Replace( "'", "" );
				sChat.Replace( "/*", "" );
				sChat.Replace( "*/", "" );
				strcpy( m_GlobalShortcut.m_szString, (const char*)sChat );
			}
			else if( pShortcut->m_dwShortcut == SHORTCUT_SKILL )	
			{
				ItemProp* pProp;
				if( pShortcut->m_dwType == 2 )
					pProp =  prj.GetPartySkill( pShortcut->m_dwId );
				else
				{
					LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
					pProp = prj.GetSkillProp( pSkill->dwSkill );	
				}
				if( pProp->dwComboStyle != CT_STEP && pProp->dwComboStyle != CT_GENERAL )	
				{
					SetForbid( TRUE );
					return FALSE;	// 퀵슬롯에 등록하려는 스킬이 스텝기술이 아닌건 등록 못함.
				}
			}
			SetShortcut( point.x, pShortcut->m_dwShortcut, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture, 1 );
			bForbid = FALSE;
		}
		rect = RECT_QUEUE;
		if( rect.PtInRect( point ) )
		{
			if( pShortcut->m_dwShortcut == SHORTCUT_SKILL && pShortcut->m_dwType == 0 )		// 스킬일경우만 등록
			{
				point.x -= rect.left;
				point.y -= rect.top;
				point.x /= SKILL_SIZE;
				// 내부에서 이동 ; 보통은 아래 부분에서 삭제를 처리하나, 스킬 큐에서의 이동은 삭제후 바로 추가해야하기 때문에 여기서 삭제를 한다.
				if( pShortcut->m_pFromWnd == this )
				{
					LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
					if( lpShortcut->m_dwData == 2 )
					{
						RemoveSkillQueue( lpShortcut->m_dwIndex, FALSE );
						SetSkillQueue( point.x, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture );// == FALSE )
						return TRUE;
					}
				}
				if( SetSkillQueue( point.x, pShortcut->m_dwType, pShortcut->m_dwId, pShortcut->m_pTexture ) == FALSE )
					SetForbid( TRUE );
				bForbid = FALSE;
			}
		}
		if( pShortcut->m_pFromWnd != this )
			SetForbid( bForbid );
	}
	// 안으로 버렸을 경우, 또는 안에서 이동했을 경우 오리지날 부분 삭제  
	// 태스크바에 있는 아이템을 슬롯 안의 공백 부분에 드래그했을 경우 
	if( pShortcut->m_pFromWnd == this )
	{
		LPSHORTCUT lpShortcut = (LPSHORTCUT) pShortcut->m_dwData;
		// 아이템 삭제 서버로 전송
		if( lpShortcut->m_dwData == 0 )
			g_DPlay.SendRemoveAppletTaskBar( (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 1 )
			g_DPlay.SendRemoveItemTaskBar( m_nSlotIndex, (BYTE)( lpShortcut->m_dwIndex ) );
		else
		if( lpShortcut->m_dwData == 2 )
		{
			RemoveSkillQueue( lpShortcut->m_dwIndex );
			//g_DPlay.SendSkillTaskBar( );
			return TRUE;
		}
		lpShortcut->Empty();
	}
	return TRUE;
}
LPSHORTCUT CWndTaskBar::Select( CPoint point )
{
	CRect rect = RECT_APPLET;
	LPSHORTCUT pShortcut = NULL;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_aSlotApplet[ point.x ];
		if( !pShortcut->IsEmpty() )
		{
			return pShortcut;
		}
	}
	rect = RECT_ITEM;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= ICON_SIZE;
		pShortcut = &m_paSlotItem[ point.x ];
		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}
	rect = RECT_QUEUE;
	if( rect.PtInRect( point ) )
	{
		point.x -= rect.left;
		point.y -= rect.top;
		point.x /= SKILL_SIZE;
		pShortcut = &m_aSlotQueue[ point.x ];
		if( !pShortcut->IsEmpty() )
			return pShortcut;

	}
	rect = CRect( POINT_QUEUE_X - ICON_SIZE - 5, POINT_QUEUE_Y, (POINT_QUEUE_X - ICON_SIZE - 5) + 32, POINT_QUEUE_Y + 32);
	if( rect.PtInRect( point ) )
	{
		m_aSlotSkill.m_dwShortcut = SHORTCUT_SKILLFUN;

		pShortcut   = &m_aSlotSkill;

		if( !pShortcut->IsEmpty() )
			return pShortcut;
	}

	return NULL;
}
void CWndTaskBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bLButtonDown && IsPush() )
	{
	//PSHORTCUT pShortcut = Select( point );
	//	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() )
		if( m_pSelectShortcut && !m_pSelectShortcut->IsEmpty() ) //&& m_pSelectShortcut->m_dwShortcut != SHORTCUT_SKILL )
		{
#ifdef __MAINSERVER
			if( m_pSelectShortcut->m_dwId == 400 )
				return;
#endif //__MAINSERVER
#if __VER >= 12 // __LORD
			if(m_pSelectShortcut->m_dwShortcut == SHORTCUT_LORDSKILL)
			{
				m_pSelectShortcut->m_dwId++;
			}
#endif
			memcpy( &m_GlobalShortcut, m_pSelectShortcut, sizeof( m_GlobalShortcut ) );
			m_GlobalShortcut.m_pFromWnd = this;
			m_GlobalShortcut.m_dwData = (DWORD)m_pSelectShortcut;
			m_pSelectShortcut = NULL;
		}
	}
	CWndNeuz::OnMouseMove( nFlags, point );
}

void CWndTaskBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(g_WndMng.m_pWndMenu && g_WndMng.m_pWndMenu->IsVisible())
		g_WndMng.m_pWndMenu->SetVisible(FALSE);

	m_pSelectShortcut = Select( point );
	SetCapture();
}
void CWndTaskBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	LPSHORTCUT pShortcut = Select( point );
	
	if( pShortcut && pShortcut == m_pSelectShortcut && !pShortcut->IsEmpty() && pShortcut->m_dwData != 2 )
	{
		g_WndMng.ObjectExecutor( pShortcut );
	}

}

BOOL CWndTaskBar::OnEraseBkgnd(C2DRender* p2DRender)
{
	return TRUE;
}
void CWndTaskBar::PaintFrame( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
	//m_pTheme->RenderWndTaskBar( p2DRender, &rect );
	CWndBase::PaintFrame( p2DRender );
}

LPSKILL CWndTaskBar::GetCurrentSkillQueue()
{
	LPSKILL pSkill = NULL;
	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

	if( pShortcut && pShortcut->IsEmpty() == FALSE )
		pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );

	return pSkill;
}
//
// 스킬큐 실행 고!
//
BOOL CWndTaskBar::UseSkillQueue( CCtrl* pTargetObj )
{
	if( m_nExecute == 2 )	return FALSE;	// 이미 스킬바 실행중이라면 다시 들어오지 않음.
	if( g_pPlayer->m_pActMover->IsActAttack() )			return FALSE;		// 이미 공격동작이 이뤄지고 있으면 들어오지 마라.
	if( g_pPlayer->m_pActMover->IsActJump() )			return FALSE;		// 점프중엔 스킬사용 안됨.
//	if( g_pPlayer->m_pActMover->IsActDamage() )			return FALSE;		// 데미지 모션중엔 스킬사용 안됨.
	if( g_pPlayer->m_pActMover->IsFly() )				return FALSE;
//	if( g_pPlayer->m_dwFlag & MVRF_SKILL )		return FALSE;				// 이미 스킬 사용중이면 사용 안됨.
	if( pTargetObj && pTargetObj->GetType() != OT_MOVER )	return FALSE;

	// 오브젝트에 오른쪽 클릭 했을때
	if( pTargetObj )
	{
		if( pTargetObj->GetType() == OT_OBJ )		// 오브젝트가 OT_OBJ(배경)이면 선택 안한걸로 간주함.
			m_idTarget = NULL_ID;
		else
			m_idTarget = ((CCtrl*)pTargetObj)->GetId();		// OT_OBJ가 아니면 아이디를 가져옴
	}
	else
		m_idTarget = NULL_ID;		// 타겟을 선택하지 않았으면 NULL_ID
	CMover *pTargetMover = (CMover*)pTargetObj;		// prj.GetMover( m_idTarget );
	if( IsInvalidObj( pTargetMover ) )		// 거시기한 타겟이었으면 타겟 안한걸로 간주.
		m_idTarget = NULL_ID;

	
	m_nUsedSkillQueue = 0;
	
	LPSHORTCUT pShortcut = &m_aSlotQueue[ m_nUsedSkillQueue ];

	if( pShortcut->IsEmpty() == FALSE )
	{
		LPSKILL pSkill = g_pPlayer->GetSkill( 0, pShortcut->m_dwId );
		if( pSkill == NULL )	return FALSE;
		
		if( g_pPlayer->IsBullet( pSkill->GetProp() ) == FALSE )
			return FALSE;

		m_nExecute = 1;		// 1 스킬바사용 실행대기중 
		// 여기엔 m_idTarget이 NULL_ID가 들어갈수도 있다.
		if( g_pPlayer->CMD_SetUseSkill( m_idTarget, pShortcut->m_dwId, SUT_QUEUESTART ) == 0 )		// 실행할 명령을 셋팅. 이동 + 스킬사용이 합쳐진 명령.
		{
			OnCancelSkill();	// 첫스킬부터 실패했다면 스킬큐 사용을 취소.
		} else
		{	// success
			// 1단계 쓸때는 ap가 소모되지 않는다.
			ItemProp *pItemProp = g_pPlayer->GetActiveHandItemProp();
			if( pItemProp )
			{   // 손에 들고 있는게 스태프나 치어스틱이 아닐때만 NEXTSKILL_NONE로 변경. 안하면 스킬이 이어지지 않음.
				if( pItemProp->dwItemKind3 != IK3_STAFF && pItemProp->dwItemKind3 != IK3_CHEERSTICK )			
					g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// 2006/06/12 스킬큐를 실행했으니 스킬큐사용 명령 클리어.-xuzhu-
			}

		}
		return TRUE;
	} else
		g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );	// 액션스킬이 비어있으면 취소.

	return FALSE;
}

BOOL CWndTaskBar::Process( void )
{
	// 툴팁 계속 갱신해야하는것들은 여기서...
	CPoint point = GetMousePoint();

	CRect rect = CRect( POINT_APPLET_X, POINT_APPLET_Y, POINT_APPLET_X + ICON_SIZE, POINT_APPLET_Y + ICON_SIZE );
	for( int i = 0; i < m_nMaxSlotApplet; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotApplet[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point) )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( lpShortcut->m_dwId );
			
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point, &rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	
	rect = CRect( POINT_ITEM_X, POINT_ITEM_Y, POINT_ITEM_X + ICON_SIZE, POINT_ITEM_Y + ICON_SIZE );
	for( int i = 0; i < MAX_SLOT_ITEM; i++ )
	{
		LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( ICON_SIZE, 0 );
	}
	rect = CRect( POINT_QUEUE_X, POINT_QUEUE_Y, POINT_QUEUE_X + SKILL_SIZE, POINT_QUEUE_Y + SKILL_SIZE );
	for( int i = 0; i < MAX_SLOT_QUEUE; i++ )
	{
		LPSHORTCUT lpShortcut = &m_aSlotQueue[ i ] ;
		if( !lpShortcut->IsEmpty() && rect.PtInRect( point)  )
		{
			if( IsShortcut( lpShortcut, SHORTCUT_MOTION, MOT_BASE_CHEER ) )
				PutTooTip( lpShortcut, point,&rect );
		}
		rect += CPoint( SKILL_SIZE, 0 );
	}

	if( m_nExecute )
	{
		if( m_idTarget != NULL_ID )		// m_idTarget이 NULL_ID인경우는 자기 자신에게 쓴경우기땜에 캔슬시켜선 안된다.
		{
			CMover* pObj = prj.GetMover( m_idTarget );
			if( IsInvalidObj(pObj) )						// 실행중에 대상이 없어졌다면 스킬사용을 멈춤.
				OnCancelSkill();
		}
//		if( pObj && pObj->IsDie() )						// 실행중 대상이 죽었으면 스킬바 실행 멈춤.
//			OnCancelSkill();

//		if( (g_pPlayer->m_dwFlag & MVRF_SKILL) == 0 )
//			OnCancelSkill();													// 스킬사용 해제
	}
	// 드래그 하이라이트 슬롯 알파 처리 
	if( m_GlobalShortcut.IsEmpty() == FALSE )
	{
		m_dwHighAlpha += 10;
		if( m_dwHighAlpha >= 256 )
			m_dwHighAlpha = 0;
	}
	// 드래그중에는 핫키를 사용할 수 없음 
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 핫키 처리 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( g_bKeyTable[ VK_RETURN ] ) 
		{
			g_bKeyTable[ VK_RETURN ] = FALSE;
			CWndBase* pWndBase = GetWndBase( APP_COMMUNICATION_CHAT );
			if( pWndBase == NULL )
			{
				g_WndMng.ObjectExecutor( SHORTCUT_APPLET, APP_COMMUNICATION_CHAT );
				CWndChat* pWndChat = (CWndChat*)GetWndBase( APP_COMMUNICATION_CHAT );
				if( pWndChat != NULL )
				{
					pWndChat->m_wndEdit.SetFocus();
				}
			}
		}
		else
		{
			AppletFunc* pApplet;
			DWORD dwIdApplet;
			POSITION pos = g_WndMng.m_mapAppletFunc.GetStartPosition();
			BOOL bKey = FALSE;
			while( pos )
			{
				g_WndMng.m_mapAppletFunc.GetNextAssoc( pos, dwIdApplet, (void*&)pApplet );
				
#ifdef __XKEYEDGE
				bKey = g_bKeyTable[ pApplet->m_cHotkey ];
				if( bKey && !g_bKeyTable2[ pApplet->m_cHotkey ] )
#else
				if( g_bKeyTable[ pApplet->m_cHotkey ] )			//gmpbigsun: 하우징 Y키( 가구창 ) 검사 
#endif
				{
					g_Error_State.m_ch = pApplet->m_cHotkey;
					g_Error_State.m_bIsShowWnd = TRUE;
					g_Error_State.m_dwWndId = pApplet->m_dwIdApplet;
					
					g_WndMng.ObjectExecutor( SHORTCUT_APPLET, pApplet->m_dwIdApplet );
#ifndef __XKEYEDGE					
					g_bKeyTable[ pApplet->m_cHotkey ] = FALSE;
#endif					
				}			
#ifdef __XKEYEDGE
				g_bKeyTable2[ pApplet->m_cHotkey ] = bKey;
				bKey = FALSE;
#endif

			}
			
			CWndRegVend* pWndRegVend = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_SELL);
			CWndRegVend* pWndVendorBuy = (CWndRegVend*)g_WndMng.GetWndBase(APP_VENDOREX_BUY);
			
			if( !pWndRegVend || !pWndVendorBuy )
			{
				DWORD *dwHotkey;
#ifdef __Y_INTERFACE_VER3				
				DWORD dwHotkey1[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
				DWORD dwHotkey2[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };

				if( g_Option.m_nInterface == 2 )
					dwHotkey = dwHotkey1;
				else
					dwHotkey = dwHotkey2;								
#else //__Y_INTERFACE_VER3
				DWORD dwHotkey1[10] = { 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79 };
				dwHotkey = dwHotkey1;
#endif //__Y_INTERFACE_VER3

				for( int i = 0; i < 10; i++ )
				{
					if( g_bKeyTable[ dwHotkey[i] ] && g_bSlotSwitchAboutEquipItem[ i ] == FALSE )
					{
						LPSHORTCUT lpShortcut = &m_paSlotItem[ i ] ;
						if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
						{
							CItemBase* pItemBase = g_pPlayer->GetItemId( lpShortcut->m_dwId );
							if( pItemBase )
							{
								int nPart = pItemBase->GetProp()->dwParts;
								if( nPart != -1 )
									g_bSlotSwitchAboutEquipItem[ i ] = TRUE;
							}
						}
						g_WndMng.ObjectExecutor( lpShortcut );	
						g_bKeyTable[ dwHotkey[i] ] = 0;
					}
				}			
			}
		}
	}

	UpdateItem();
	
	return CWndBase::Process();
}

// 스킬바 사용을 취소(중단)함.
void CWndTaskBar::OnCancelSkill( void )
{
	if( g_pPlayer->m_dwReqFlag & REQ_USESKILL )
		return;

	m_nExecute = 0;
	m_nUsedSkillQueue = 0;
	g_pPlayer->ClearDestObj();
	g_WndMng.m_pWndWorld->SetNextSkill( NEXTSKILL_NONE );
}

// 스킬 동작 끝났을때 호출.
void CWndTaskBar::OnEndSkill( void )
{
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 태스크 바의 매뉴 처리 윈도  
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndTaskMenu::CWndTaskMenu()
{
	m_pMenu1 = NULL;
	m_pMenu2 = NULL;
	m_pMenu3 = NULL;
	m_pMenu4 = NULL;
	m_pMenu5 = NULL;
	m_pMenu6 = NULL;
	m_pMenu7 = NULL;
}

CWndTaskMenu::~CWndTaskMenu()
{
	SAFE_DELETE( m_pMenu1 );
	SAFE_DELETE( m_pMenu2 );
	SAFE_DELETE( m_pMenu3 );
	SAFE_DELETE( m_pMenu4 );
	SAFE_DELETE( m_pMenu5 );
	SAFE_DELETE( m_pMenu6 );
	SAFE_DELETE( m_pMenu7 );
}
void CWndTaskMenu::OnDraw(C2DRender* p2DRender)
{
	return;
	p2DRender->TextOut( 10, 20,  _T( "MASQUERADE" ),  D3DCOLOR_ARGB( 255, 250, 250, 250 ) );

	CRect rect = GetWindowRect();
	// 테두리 박스 
	DWORD dwColor1t = D3DCOLOR_ARGB( 055,  91, 104, 205 );
	DWORD dwColor2t = D3DCOLOR_ARGB( 055, 116, 128, 220 );
	DWORD dwColor4t = D3DCOLOR_ARGB( 055, 143, 173, 245 );
	DWORD dwColor1b = D3DCOLOR_ARGB( 055,  41, 104, 155 );
	DWORD dwColor2b = D3DCOLOR_ARGB( 055,  66,  78, 170 );
	DWORD dwColor4b = D3DCOLOR_ARGB( 055, 143, 173, 245 );

	DWORD dwColor3t = D3DCOLOR_ARGB( 155, 250, 250, 250 );
	DWORD dwColor3b = D3DCOLOR_ARGB(  55,  00,  00,  00 );
	
	//p2DRender->RenderRoundRect( rect, dwColor1t, dwColor1t, dwColor1b, dwColor1b );
	rect.DeflateRect(1,1);
	//p2DRender->RenderRoundRect( rect, dwColor2t, dwColor2t, dwColor2b, dwColor2b );
//	rect.DeflateRect(1,1);

	CRect rect1 = rect; rect1.bottom = rect1.top + 40;
	CRect rect2 = rect; rect2.top    = rect2.top + 40;

	p2DRender->RenderFillRect( rect1, dwColor3t, dwColor3t, dwColor3b, dwColor3b );
	p2DRender->RenderFillRect( rect2, dwColor3b, dwColor3b, dwColor4b, dwColor4b );

//	p2DRender->RenderLine( CPoint( 10, 5 * 22 + 54 ), CPoint( 140, 5 * 22 + 54 ), dwColor1t );
	// client 영역 칠하기 
	//OnEraseBkgnd(p2DRender);
	//.OffsetRect(-rect.TopLeft());
	//p2DRender->PaintRect( rect,MKHIGHRGB(255>>3,255>>3,255>>3) );
	//p2DRender->TextOut( 5,20,"HP : 100 / 100", dwColor1t );
	//p2DRender->TextOut( 5,35,"MP : 100 / 100", dwColor1t );
}
BOOL CWndTaskMenu::Process()
{
	if(	CWndBase::m_GlobalShortcut.m_dwShortcut != SHORTCUT_NONE )
		return CWndBase::Process();

	if( IsVisible() == FALSE )
		return CWndBase::Process();

	for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
	{
		BOOL bHighLight = FALSE;
		CWndButton* pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
		if( pWndButton->GetClientRect( TRUE ).PtInRect( m_ptMouse ) )
		{
			// 모두 숨기기 
			if( pWndButton->m_pWndMenu == NULL || pWndButton->m_pWndMenu->IsVisible( ) == FALSE ) 
			{
				for( int i2 = 0; i2 < m_awndMenuItem.GetSize(); i2++)
				{
					if( ((CWndButton*)m_awndMenuItem.GetAt( i2 ) )->m_pWndMenu )
						((CWndButton*)m_awndMenuItem.GetAt( i2 ) )->m_pWndMenu->SetVisibleSub( FALSE );
				}
			}
			// 새 매뉴를 보이고 포커스 주기 
			if( pWndButton->m_pWndMenu )
			{
				if( pWndButton->m_pWndMenu->IsVisible() == FALSE )
				{
					CRect rect = pWndButton->GetScreenRect();
					pWndButton->m_pWndMenu->Move( CPoint( rect.right , rect.top ) );
				}
				// 메뉴의 좌표 지정 
				CRect rcButton = pWndButton->GetScreenRect();
				pWndButton->m_pWndMenu->Move( CPoint( rcButton.right, rcButton.top ) );
				// 그런데 그 메뉴가 화면을 벗어났다면 위치를 수정 
				CRect rcMenu = pWndButton->m_pWndMenu->GetScreenRect();
				CRect rcLayout = m_pWndRoot->GetLayoutRect();
				CPoint pt = rcMenu.TopLeft();
				if( rcMenu.right > rcLayout.right )
				{
					pt.x = rcButton.left - rcMenu.Width();
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				if( rcMenu.bottom > rcLayout.bottom )
				{
					pt.y -= rcMenu.bottom - rcLayout.bottom;
					//pWndButton->m_pWndMenu->Move( CPoint( rcButton.left - rcMenu.Width(), rcButton.top ) );
				}
				pWndButton->m_pWndMenu->Move( pt );
				pWndButton->m_pWndMenu->SetVisible( TRUE );
				pWndButton->m_pWndMenu->SetFocus();
			}
		}
	}
	return CWndBase::Process();
}
void CWndTaskMenu::SetTexture( CWndButton* pWndButton )
{
	pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, g_WndMng.GetAppletFunc( pWndButton->GetWndId() )->m_pszIconName ) );
}

void CWndTaskMenu::OnInitialUpdate()
{
	CWndMenu::OnInitialUpdate();

	CWndButton* pWndButton;
	// 0 ~ 9번 
	if( g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER ) )
	{
		pWndButton = AppendMenu( this, 0, APP_DEBUGINFO     , GETTEXT( TID_APP_DEBUGINFO     ) ); 
	}

	pWndButton = AppendMenu( this, 0, APP_STATUS1    , GETTEXT( TID_APP_STATUS        ) );
	pWndButton = AppendMenu( this, 0, APP_NAVIGATOR , GETTEXT( TID_APP_NAVIGATOR     ) );
#if __VER >= 13 // __RENEW_CHARINFO
	pWndButton = AppendMenu( this, 0, APP_CHARACTER3 , GETTEXT( TID_APP_CHARACTER     ) );
#elif __VER >= 9 // __CSC_VER9_2
	pWndButton = AppendMenu( this, 0, APP_CHARACTER2 , GETTEXT( TID_APP_CHARACTER     ) );
#else //__CSC_VER9_2
	pWndButton = AppendMenu( this, 0, APP_CHARACTER , GETTEXT( TID_APP_CHARACTER     ) );
#endif //__CSC_VER9_2
	pWndButton = AppendMenu( this, 0, APP_INVENTORY , GETTEXT( TID_APP_INVENTORY     ) );
#if __VER >= 11 // __SYS_POCKET
#ifndef __TMP_POCKET
	pWndButton = AppendMenu( this, 0, APP_BAG_EX , GETTEXT( TID_APP_BAG_EX     ) );
#endif
#if __VER >= 13 // __HOUSING
	pWndButton = AppendMenu( this, 0, APP_HOUSING, GETTEXT(TID_GAME_HOUSING_BOX));
#endif // __HOUSING
#endif // __SYS_POCKET
#if __VER >= 8 // __S8_VENDOR_REVISION
	pWndButton = AppendMenu( this, 0, APP_VENDOR_REVISION , GETTEXT( TID_APP_VENDOR     ) );
#else // __VER >= 8 // __S8_VENDOR_REVISION
	pWndButton = AppendMenu( this, 0, APP_VENDOREX , GETTEXT( TID_APP_VENDOR     ) );
#endif // __VER >= 8 // __S8_VENDOR_REVISION
#if __VER >= 10 // __CSC_VER9_1
	pWndButton = AppendMenu( this, 0, APP_SKILL3     , GETTEXT( TID_APP_SKILL         ) );
#else
	pWndButton = AppendMenu( this, 0, APP_SKILL1     , GETTEXT( TID_APP_SKILL         ) );
#endif //__CSC_VER9_1
#if __VER >= 15 // __IMPROVE_QUEST_INTERFACE
	pWndButton = AppendMenu( this, 0, APP_QUEST_EX_LIST     , GETTEXT( TID_APP_QUEST         ) );
#else // __IMPROVE_QUEST_INTERFACE
	pWndButton = AppendMenu( this, 0, APP_QUEST     , GETTEXT( TID_APP_QUEST         ) );
#endif // __IMPROVE_QUEST_INTERFACE
	pWndButton = AppendMenu( this, 0, APP_MOTION   , GETTEXT( TID_APP_MOTION       ) );

#if __VER >= 9  // __INSERT_MAP
#ifdef __IMPROVE_MAP_SYSTEM
	pWndButton = AppendMenu( this, 0, APP_MAP_EX, GETTEXT( TID_APP_MAP ) );
#else // __IMPROVE_MAP_SYSTEM
	pWndButton = AppendMenu( this, 0, APP_MAP, GETTEXT( TID_APP_MAP));//GETTEXT( TID_APP_MAP));
#endif // __IMPROVE_MAP_SYSTEM
#endif
	CWndButton* pWndButton7 = NULL;
	pWndButton7 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMITEM  ) ); pWndButton7->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) ); 

	CWndButton* pWndButton1 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMUNICATION ) ); pWndButton1->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton2 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_COMMUNITY     ) ); pWndButton2->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
//	CWndButton* pWndButton3 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_FIND          ) ); pWndButton3->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton5 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_INFOMATION    ) ); pWndButton5->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
	CWndButton* pWndButton6 = AppendMenu( this, 0, 0 , GETTEXT( TID_APP_HELPER        ) ); pWndButton6->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) ); // 사용자, 시냅스
	CWndButton* pWndButton4 = AppendMenu( this, 0, APP_OPTIONEX, GETTEXT(TID_APP_OPTION) );

	pWndButton = AppendMenu( this, 0, APP_LOGOUT    , GETTEXT( TID_APP_LOGOUT      ) );
	pWndButton = AppendMenu( this, 0, APP_QUIT      , GETTEXT( TID_APP_QUIT          ) );

//	if( GetLanguage() == LANG_TWN )
//		pWndButton	= AppendMenu( this, 0, APP_WEBBOX2	, GETTEXT( TID_TIP_QOODO	) );

	CWndBase::SetTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, _T( "WndTaskMenu.tga" ) ), TRUE );

	for(int i = 0; i < m_awndMenuItem.GetSize(); i++)
	{
		CWndButton* pWndButton = GetMenuItem( i );
		pWndButton->SetWndRect( CRect(  10, 50 + ( i * 22 ), m_pTexture->m_size.cx - 20, 50 + 20 + ( i * 22 ) ) );
	}
	SetWndRect( CRect( 0, 0, m_pTexture->m_size.cx, m_pTexture->m_size.cy ) );

//	CRect rect = g_WndMng.m_pWndTaskBar->m_wndMenu.GetScreenRect();
//	Move( CPoint( 0, rect.top - GetWindowRect().Height() ) );

	m_pMenu1 = new CWndMenu;
	m_pMenu1->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu1, 0, APP_COMMUNICATION_CHAT , GETTEXT( TID_APP_COMMUNICATION_CHAT    ) );
	pWndButton = AppendMenu( m_pMenu1, 0, APP_MESSENGER_         , GETTEXT( TID_APP_MESSENGER ) );
#if __VER >= 13 // __CSC_VER13_2
	pWndButton = AppendMenu( m_pMenu1, 0, APP_COUPLE_MAIN        , GETTEXT( TID_GAME_COUPLE ) );
#endif //__CSC_VER13_2
	pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton1->SetMenu( m_pMenu1 );

	m_pMenu2 = new CWndMenu;
	m_pMenu2->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu2, 0, APP_PARTY      , GETTEXT( TID_APP_PARTY          ) ) ;
	pWndButton = AppendMenu( m_pMenu2, 0, APP_GUILD      , GETTEXT( TID_APP_COMPANY         ) ) ;
	
#ifdef __GUILDVOTE
	pWndButton = AppendMenu( m_pMenu2, 0, APP_GUILD_VOTE      , GETTEXT( TID_GAME_TOOLTIP_GUILDVOTE     ) ) ;
#endif	
	pWndButton2->SetMenu( m_pMenu2 );

	m_pMenu5 = new CWndMenu;
	m_pMenu5->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu5, 0, APP_INFO_NOTICE   , GETTEXT( TID_APP_INFO_NOTICE   ) ); 
	pWndButton5->SetMenu( m_pMenu5 );

	m_pMenu6 = new CWndMenu;
	m_pMenu6->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_HELP , GETTEXT( TID_APP_HELPER_HELP ) );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_TIP  , GETTEXT( TID_APP_HELPER_TIP  ) );
	pWndButton = AppendMenu( m_pMenu6, 0, APP_HELPER_FAQ  , GETTEXT( TID_APP_HELPER_FAQ  ) );
#if __VER >= 12 // __MOD_TUTORIAL
	pWndButton = AppendMenu( m_pMenu6, 0, APP_INFOPANG , GETTEXT( TID_APP_INFOPANG ) );
	//pWndButton->SetTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ICON, _T( "icon_Folder.dds" ) ) );
#endif
#ifdef __NEW_WEB_BOX
#ifdef __INTERNALSERVER
	pWndButton = AppendMenu( m_pMenu6, 0, APP_WEBBOX2 , GETTEXT( TID_GAME_HELPER_WEB_BOX_ICON_TITLE ) );
#else // __INTERNALSERVER
	if( ( GetLanguage() == LANG_ENG && GetSubLanguage() == LANG_SUB_USA ) )
	{
		pWndButton = AppendMenu( m_pMenu6, 0, APP_WEBBOX2 , GETTEXT( TID_GAME_HELPER_WEB_BOX_ICON_TITLE ) );
	}
#endif // __INTERNALSERVER
#endif // __NEW_WEB_BOX
	pWndButton6->SetMenu( m_pMenu6 );
	
	m_pMenu7 = new CWndMenu;
	m_pMenu7->CreateMenu( this );
	pWndButton = AppendMenu( m_pMenu7, 0, APP_WEBBOX, GETTEXT( TID_APP_ITEMMALL ) ); pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton = AppendMenu( m_pMenu7, 0, APP_COMM_ITEM , GETTEXT( TID_APP_ITEMTIME ) ); pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; SetTexture( pWndButton );
	pWndButton7->SetMenu( m_pMenu7 );
}
CWndButton* CWndTaskMenu::AppendMenu( CWndMenu* pWndMenu, UINT nFlags, UINT nIDNewItem,	LPCTSTR lpszNewItem )
{
	CWndButton* pWndButton = (CWndButton*)pWndMenu->AppendMenu( nFlags, nIDNewItem, lpszNewItem );
	AppletFunc* pAppletFunc = g_WndMng.GetAppletFunc( nIDNewItem ); 
	if( pAppletFunc )
	{
		pWndButton->m_cHotkey = pAppletFunc->m_cHotkey;
		CString string;
		if( pAppletFunc->m_cHotkey == 0 )
			string.Format( "%s", pAppletFunc->m_pAppletDesc );
		else
			string.Format( "%s\n[%s %c]", pAppletFunc->m_pAppletDesc,  prj.GetText( TID_GAME_TOOLTIP_HOTKEY ), pAppletFunc->m_cHotkey );
		pWndButton->m_strToolTip = string;
		pWndButton->m_shortcut.m_dwShortcut = SHORTCUT_APPLET; 
		SetTexture( pWndButton );
	}
	
//	SetWndRect( CRect( 0, 0, 180, 50 + 5 + 5 + ( m_awndMenuItem.GetSize() * 22 ) ) );

	return pWndButton;
	/*

  CString string;
  if( pAppletFunc->m_cHotkey == 0 )
  string.Format( "애플렛\n%s", pAppletFunc->m_pAppletDesc );
  else
  string.Format( "애플렛\n%s\n[단축키 %c]", pAppletFunc->m_pAppletDesc, pAppletFunc->m_cHotkey );
  g_toolTip.PutToolTip( pShortcut->m_dwId , string, *pRect, point, 0 );
  
	CWndButton* pWndButton = new CWndButton;
	int nCount = m_awndMenuItem.GetSize();
	CSize size = m_pFont->GetTextExtent( lpszNewItem );
	if( size.cx > m_nLargeWidth )
		m_nLargeWidth = size.cx + 60;
	pWndButton->Create(lpszNewItem, WBS_MENUITEM | WBS_HIGHLIGHT, CRect( 2, 2 + ( nCount * 22 ), m_nLargeWidth, 2 + ( nCount * 22 ) + 20 ), this, nIDNewItem );
	m_awndMenuItem.Add( pWndButton );
	for( int i = 0; i < m_awndMenuItem.GetSize(); i++ )
	{
		pWndButton = (CWndButton*)m_awndMenuItem.GetAt( i );
		CRect rect = pWndButton->GetWindowRect( TRUE );
		rect.right = rect.left + m_nLargeWidth; 
		pWndButton->SetWndRect( rect );
	}
	SetWndRect( CRect( 0, 0, m_nLargeWidth + 10, 5 + 5 + ( ( nCount + 1 ) * 22 ) ) );
	return pWndButton;
	*/
}

BOOL CWndTaskMenu::Initialize(CWndBase* pWndParent,DWORD dwWndId)
{
	CreateMenu( &g_WndMng );
	SetVisible(TRUE);
	return TRUE;
}

void CWndTaskMenu::OnKillFocus(CWndBase* pNewWnd)
{
	if(g_WndMng.m_pWndTaskBar != pNewWnd)
		CWndMenu::OnKillFocus(pNewWnd);
}

BOOL CWndTaskMenu::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase )
{
//	g_WndMng.ObjectExecutor( SHORTCUT_APPLET, nID );
	return TRUE;
}
BOOL CWndTaskMenu::OnChildNotify(UINT message,UINT nID,LRESULT* pLResult)
{
	if( message == WNM_CLICKED )
		g_WndMng.ObjectExecutor( SHORTCUT_APPLET, nID );
	return TRUE;
}
void CWndTaskMenu::OnSize(UINT nType, int cx, int cy)
{
	CWndMenu::OnSize( nType, cx, cy );
}
void CWndTaskMenu::OnLButtonUp(UINT nFlags, CPoint point)
{
	//if(IsWndStyle(WBS_CAPTION) && m_bPickup)
	{//
	//	m_wndTitleBar.m_wndMinimize.SetVisible(TRUE);
		//m_wndTitleBar.m_wndMaximize.SetVisible(TRUE);
	}
}
void CWndTaskMenu::OnLButtonDown(UINT nFlags, CPoint point)
{
//	CWndBase::OnLButtonDown(nFlags,point
	if(IsWndRoot())
		return;
//	return;

}

void CWndTaskMenu::PaintFrame( C2DRender* p2DRender )
{
//	p2DRender->RenderFillRect( GetWindowRect(), D3DCOLOR_ARGB( 255, 76,124,193 ) );
//	p2DRender->RenderRect( GetWindowRect(), D3DCOLOR_ARGB( 255, 36,74,100 ) );
	CWndBase::PaintFrame( p2DRender );
}
BOOL CWndTaskMenu::OnEraseBkgnd( C2DRender* p2DRender )
{
	CRect rect = GetWindowRect();
	rect.top += 50;
//	p2DRender->RenderFillRect(rect, D3DCOLOR_ARGB( 255, 255,255,255 ) );
	return CWndBase::OnEraseBkgnd( p2DRender );
}


void CWndTaskBar::RenderCollTime(CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
{
	LPSKILL lpSkill = &g_pPlayer->m_aJobSkill[ dwSkillId ];
	if( lpSkill )
	{
		DWORD dwDelay = g_pPlayer->GetReuseDelay( dwSkillId );
		if( dwDelay > 0 )
		{
#if __VER >= 9	// __SKILL_0706
			ItemProp* pSkillProp	= lpSkill->GetProp();
			ASSERT( pSkillProp );
			AddSkillProp* pAddSkillProp	= prj.GetAddSkillProp( pSkillProp->dwSubDefine, lpSkill->dwLevel );
			ASSERT( pAddSkillProp );
			RenderRadar( p2DRender, pt, 
			             pAddSkillProp->dwCooldown - dwDelay, 
						 pAddSkillProp->dwCooldown );	
#else	// __SKILL_0706
			RenderRadar( p2DRender, pt, 
			             lpSkill->GetProp()->dwSkillReady - dwDelay, 
						 lpSkill->GetProp()->dwSkillReady );	
#endif	// __SKILL_0706
		}
	}				
}

#if __VER >= 12 // __LORD
	void CWndTaskBar::RenderLordCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender )
	{
		CCLord* pLord									= CCLord::Instance();
		CLordSkillComponentExecutable* pComponent		= pLord->GetSkills()->GetSkill(dwSkillId);
		if(!pComponent) return;
		DWORD dwDelay = 0;
		if(pComponent->GetTick() > 0)
			RenderRadar( p2DRender, pt, pComponent->GetCooltime() - pComponent->GetTick(), pComponent->GetCooltime() );	
	}
#endif

void CWndTaskBar::RenderOutLineLamp(int x, int y, int num, DWORD size)
{
	CPoint Point = CPoint( x, y );
	LONG   thick = 10;

	D3DXCOLOR dwColorDest2  = D3DCOLOR_ARGB( 0, 40, 0, 255 );

	D3DXCOLOR dwColor  = D3DCOLOR_ARGB( 196, 40, 0, 255 );

	D3DXCOLOR dwColor1 = D3DCOLOR_ARGB( 0, 255, 255, 255 );
	D3DXCOLOR dwColor2 = D3DCOLOR_ARGB( 0, 255, 255, 255 );

	static BOOL  bReverse = FALSE;
	static FLOAT fLerp = 0.0f;

	if( bReverse )
		fLerp-=0.05f;
	else
		fLerp+=0.05f;

	if( fLerp > 1.0f )
	{
		bReverse = TRUE;
		fLerp = 1.0f;
	}
	else
	if( fLerp < 0.0f )
	{
		bReverse = FALSE;
		fLerp = 0.0f;
	}
	
	D3DXColorLerp( &dwColor2, &dwColor, &dwColorDest2, fLerp);
	
	CRect Rect = CRect( 0, 0, (size * num), size );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left)-thick, Point.y+Rect.top, (Point.x+Rect.left), Point.y+Rect.bottom ),
		dwColor1, dwColor2, dwColor1, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.top)-thick, (Point.x+Rect.right), Point.y+Rect.top ),
		dwColor1, dwColor1, dwColor2, dwColor2 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.right), Point.y+Rect.top, (Point.x+Rect.right)+thick, Point.y+Rect.bottom ),
		dwColor2, dwColor1, dwColor2, dwColor1 );
	
	m_p2DRender->RenderFillRect( CRect( (Point.x+Rect.left), (Point.y+Rect.bottom), (Point.x+Rect.right), (Point.y+Rect.bottom)+thick ),
		dwColor2, dwColor2, dwColor1, dwColor1 );
}

BOOL CWndTaskBar::IsShortcut( LPSHORTCUT lpShortcut, DWORD dwShortcut, DWORD dwId )
{
	if( lpShortcut->m_dwShortcut == SHORTCUT_APPLET )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_SKILL )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_ITEM )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_CHAT )
	{
	}
	else
	if( lpShortcut->m_dwShortcut == SHORTCUT_MOTION )
	{
		if( lpShortcut->m_dwShortcut == dwShortcut )
		{
			MotionProp* pMotionProp = prj.GetMotionProp( dwId );
		
			if( pMotionProp )
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}
