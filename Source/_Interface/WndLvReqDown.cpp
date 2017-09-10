#include "stdafx.h"
#include "resData.h"
#include "WndLvReqDown.h"
#include "DPClient.h"


extern CDPClient g_DPlay;

/****************************************************
  WndId : APP_LVREQDOWN - ���뷹�� �϶� ����â
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_BUTTON1 - Button
  CtrlId : WIDC_STATIC1 - 
****************************************************/

#if __VER >= 11 // __SYS_IDENTIFY
CWndLvReqDown::CWndLvReqDown() 
{ 

	m_pItemElem  = NULL;
	m_pEItemProp = NULL;
	m_pTexture   = NULL;
	
} 
CWndLvReqDown::~CWndLvReqDown() 
{ 
} 
void CWndLvReqDown::OnDraw( C2DRender* p2DRender ) 
{ 
	ItemProp* pItemProp;
	
	if(m_pItemElem != NULL)
	{
		pItemProp = m_pItemElem->GetProp();
		LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
		
		if(m_pTexture != NULL)
			m_pTexture->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );
	} 
} 
void CWndLvReqDown::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);

	if(::GetLanguage() == LANG_FRE)
		pButton->SetTexture(g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, _T( "ButOk2.bmp" ) ), TRUE);

	pButton->EnableWindow(FALSE);
	m_pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	SetDescription();
	/*
	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );*/
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndLvReqDown::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_LVREQDOWN, 0, CPoint( 0, 0 ), pWndParent );
} 


void CWndLvReqDown::OnDestroy()
{
	if(m_pItemElem != NULL)
	{
		if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
			m_pItemElem->SetExtra(0);
	}
}

/*
  ���� ������ ���� ��� 
BOOL CWndLvReqDown::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndLvReqDown::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndLvReqDown::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndLvReqDown::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndLvReqDown::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndLvReqDown::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		//������ ������ �˸���.
		if(m_pItemElem != NULL)
		{
			CWndButton* pButton;
			pButton = (CWndButton*)GetDlgItem( WIDC_BUTTON1 );
			pButton->EnableWindow(FALSE);

			// ������ ó�� ��û�ϴ� �Լ� ȣ���ؾ���
			if(m_pItemElem)
			{
				g_DPlay.SendRemoveItemLevelDown(m_pItemElem->m_dwObjId);
				Destroy();
			}
		}
	}
	/*
	if(message == WIN_ITEMDROP)
	{
		CItemElem* pTempElem;
		LPSHORTCUT pShortCut = (LPSHORTCUT)pLResult;
		pTempElem  = (CItemElem*)g_pPlayer->GetItemId( pShortCut->m_dwId );

		// �϶� ���°� �� �����۸� �ø� �� �ִ�. 
		if(m_pItemElem == NULL && pTempElem != NULL)
		{
			if(pTempElem->GetLevelDown() < 0)
			{
				m_pItemElem = (CItemElem*)g_pPlayer->GetItemId( pShortCut->m_dwId );
				m_pEItemProp = m_pItemElem->GetProp();
				m_pItemElem->SetExtra(m_pItemElem->GetExtra()+1);
				CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_START);
				pButton->EnableWindow(TRUE);
			}
		}
	}
	*/
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndLvReqDown::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	if(!m_pItemElem) return;
	CRect rect;
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
	rect = wndCtrl->rect;
	if( rect.PtInRect( point ) )
	{
		m_pItemElem->SetExtra(0);
		CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		pButton->EnableWindow(FALSE);
		m_pItemElem = NULL;
		m_pEItemProp = NULL;
		m_pTexture	= NULL;
	}
}

BOOL CWndLvReqDown::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	
	CItemElem* pTempElem;
	pTempElem  = (CItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );

	// �϶� ���°� �� �����۸� �ø� �� �ִ�. 
	if( pTempElem != NULL)
	{
		if(pTempElem->GetLevelDown() < 0)
		{
			if(m_pItemElem) m_pItemElem->SetExtra(0);
			m_pItemElem = pTempElem;
			m_pEItemProp = m_pItemElem->GetProp();
			if(m_pEItemProp != NULL)
			{
				m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, m_pEItemProp->szIcon), 0xffff00ff );
			}
			m_pItemElem->SetExtra(m_pItemElem->GetExtra()+1);
			CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
			pButton->EnableWindow(TRUE);
		}
	}

	return TRUE;
}

void CWndLvReqDown::SetDescription()
{

	CScript scanner;
	BOOL checkflag;
	CHAR* szChar;


	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "ReqLvDown.inc" ) ));
	szChar = scanner.m_pProg;
	
	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();	
	}
}
#endif	// __SYS_IDENTIFY