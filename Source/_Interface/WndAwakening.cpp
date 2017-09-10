#include "stdafx.h"
#include "resData.h"
#include "WndAwakening.h"
#include "DPClient.h"
#include "WndManager.h"
#include "defineText.h"

#if __VER >= 11 // __SYS_IDENTIFY
#include "randomoption.h"
#endif	// __SYS_IDENTIFY

extern CDPClient g_DPlay;

/****************************************************
  WndId : APP_AWAKENING - ������ ���� â
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_STATIC1 - 
  CtrlId : WIDC_BUTTON1 - Button
****************************************************/
#if __VER >= 11 // __SYS_IDENTIFY

// gmpbigsun ( 10_04_05 ) : CWndAwakening class�� ���� ������ ����
CWndAwakening::CWndAwakening() 
{
	m_pItemElem  = NULL;
	m_pEItemProp = NULL;
	m_pText		 = NULL;
	m_pTexture	 = NULL;
} 
CWndAwakening::~CWndAwakening() 
{ 
} 
void CWndAwakening::OnDraw( C2DRender* p2DRender ) 
{ 

	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
			
	if(m_pTexture)
		m_pTexture->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );
		
} 

BOOL CWndAwakening::process()
{
/*
	ItemProp* pItemProp;
	
	if(m_pItemElem != NULL)
	{
		pItemProp = m_pItemElem->GetProp();
		LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
		if(pItemProp != NULL)
		{
			m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pItemProp->szIcon), 0xffff00ff );
		}
	} 
*/
	return TRUE;

}

void CWndAwakening::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	// ���⿡ �ڵ��ϼ���
	CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);

	if(::GetLanguage() == LANG_FRE)
		pButton->SetTexture(g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, _T( "ButOk2.bmp" ) ), TRUE);

	pButton->EnableWindow(FALSE);
	m_pText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	SetDescription();

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndAwakening::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_AWAKENING, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWnd::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/

void CWndAwakening::OnDestroy()
{
	if(m_pItemElem != NULL)
	{
		if( !g_pPlayer->m_vtInfo.IsTrading( m_pItemElem ) )
			m_pItemElem->SetExtra(0);
	}
}

BOOL CWndAwakening::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 

void CWndAwakening::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 

void CWndAwakening::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 

void CWndAwakening::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 

BOOL CWndAwakening::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
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
				g_DPlay.SendAwakening(m_pItemElem->m_dwObjId);
				Destroy();
			}
		}
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndAwakening::OnLButtonDblClk( UINT nFlags, CPoint point )
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

BOOL CWndAwakening::OnDropIcon( LPSHORTCUT pShortcut, CPoint point )
{
	CItemElem* pTempElem;
	pTempElem  = (CItemElem*)g_pPlayer->GetItemId( pShortcut->m_dwId );

	int nRandomOptionKind	= g_xRandomOptionProperty->GetRandomOptionKind( pTempElem );
	if( nRandomOptionKind  != CRandomOptionProperty::eAwakening )
	{
		// ������ ����� �ƴմϴ�.
		//g_WndMng.PutString( prj.GetText( TID_GAME_INVALID_TARGET_ITEM ), NULL, prj.GetTextColor( TID_GAME_INVALID_TARGET_ITEM  ) );
		g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_INVALID_TARGET_ITEM ) );
		return FALSE;
	}
	if( g_xRandomOptionProperty->GetRandomOptionSize( pTempElem->GetRandomOptItemId() ) > 0 )
	{
		// �̹� ������ �������Դϴ�.
		//g_WndMng.PutString( prj.GetText( TID_GAME_AWAKE_OR_BLESSEDNESS01 ), NULL, prj.GetTextColor( TID_GAME_AWAKE_OR_BLESSEDNESS01  ) );
		g_WndMng.OpenMessageBox( prj.GetText( TID_GAME_AWAKE_OR_BLESSEDNESS01 ) );
		return FALSE;
	}
	
	if(pTempElem != NULL)
	{
		if(m_pItemElem) m_pItemElem->SetExtra(0);
		m_pItemElem = pTempElem;
		m_pEItemProp = m_pItemElem->GetProp();
		m_pItemElem->SetExtra(m_pItemElem->GetExtra()+1);
		CWndButton* pButton = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		pButton->EnableWindow(TRUE);
		
		LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
		if(m_pEItemProp != NULL)
		{
			m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, m_pEItemProp->szIcon), 0xffff00ff );
		}
	}

	return TRUE;

}

void CWndAwakening::SetDescription()
{
	CScript scanner;
	BOOL checkflag;
	CHAR* szChar;


	checkflag = scanner.Load( MakePath( DIR_CLIENT,  _T( "ItemAwakening.inc" ) ));
	szChar = scanner.m_pProg;
	
	if(m_pText != NULL && checkflag)
	{
		m_pText->m_string.AddParsingString( szChar );
		m_pText->ResetString();	
	}
}
#endif

#ifdef __PROTECT_AWAKE
CWndSelectAwakeCase::CWndSelectAwakeCase( )
: m_dwDeltaTime( 0 ),
m_dwItemIndex( 0 ),
m_pTexture( NULL ),
m_pVertexBufferGauge(NULL), 
m_pTexGuage( NULL )
{
	m_dwOldTime = g_tmCurrent;

	m_byObjID = 0;
	m_dwSerialNum = 0;
	m_n64NewOption = 0;
}

CWndSelectAwakeCase::~CWndSelectAwakeCase( )
{
	DeleteDeviceObjects();
}
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndSelectAwakeCase::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_AWAKE_SELECTCASE, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndSelectAwakeCase::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	
	// ���⿡ �ڵ��ϼ���

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	ItemProp* pProp = (ItemProp*)prj.GetItemProp( m_dwItemIndex );
	if( pProp )
		m_pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon ), 0xffff00ff );
	
	m_pTexGuage = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_THEME, "Wndguage.tga"   ), 0xffff00ff );

	if( !m_pTexGuage )
		Error( "CWndSelectAwakeCase::OnInitialUpdate m_pTexGuage(Wndguage.tga) is NULL" );

	AddWndStyle( WBS_MODAL );

	RestoreDeviceObjects( );
} 


BOOL CWndSelectAwakeCase::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID == WIDC_BUTTON1 )
	{
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_OLD_VALUE );
		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE03 ) );	//���õ�
		Destroy();
	}
	else if( nID == WIDC_BUTTON2 )
	{
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_NEW_VALUE );
		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE03 ) );	//���õ�
		Destroy();
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

const static DWORD AWAKE_KEEP_TIME = 60000;	//60��

BOOL CWndSelectAwakeCase::process()
{
	return TRUE;
}

void CWndSelectAwakeCase::OnDraw( C2DRender* p2DRender ) 
{ 
	if( !g_pPlayer )
		return;

	CItemElem* pItemElem = (CItemElem*)g_pPlayer->GetItemId( m_byObjID );
	if( !pItemElem )
	{
		assert( 0 );
		return;
	}

	m_dwDeltaTime += (g_tmCurrent - m_dwOldTime);
	m_dwOldTime = g_tmCurrent;

	if( m_dwDeltaTime > AWAKE_KEEP_TIME )
	{
		//���ѽð��� ������ ����ɼ����� ��û 
		g_DPlay.SendSelectedAwakeningValue( m_byObjID, m_dwSerialNum, _AWAKE_OLD_VALUE );

		g_WndMng.PutString( GETTEXT( TID_GAME_REGARDLESS_USE02 ) );	//���ýð� �ʰ�
		
		m_dwDeltaTime = 0;
		Destroy();
	}

	//old
	OutputOptionString( p2DRender, pItemElem );

	//new
	OutputOptionString( p2DRender, pItemElem, TRUE );


	CRect rect = GetWindowRect();
	rect.left += 14;
	rect.top += 174;

	// �ð� / 100 * 360(�ѱ���) / 600 (�ѽð�)
	rect.right = LONG(( rect.left + (AWAKE_KEEP_TIME - m_dwDeltaTime) / 100 ) * 0.6f) ;		//�������� �� ���� ���̸� �ִ��ʷ�...
	rect.bottom = rect.top + 20;

//	if( m_pTexGuage )
//		p2DRender->RenderFillRect( rect, 0xffff0000, m_pTexGuage->m_pTexture  );

	m_pTheme->RenderGauge(p2DRender, &rect, 0xffffffff, m_pVertexBufferGauge, m_pTexGuage);

	// draw icon
	LPWNDCTRL wndCtrl = GetWndCtrl( WIDC_STATIC1 );
	assert( wndCtrl );
			
	if(m_pTexture)
		m_pTexture->Render( p2DRender, CPoint( wndCtrl->rect.left, wndCtrl->rect.top ) );
} 

extern BOOL IsDst_Rate( int nDstParam );
extern char *FindDstString( int nDstParam );
void CWndSelectAwakeCase::OutputOptionString( C2DRender* p2DRender, CItemElem* pItemElem, BOOL bNew )
{
	// �ɼ� ���ڿ� �׸���
	if( !pItemElem )
		return;

    int nSize = 0;
	if( !bNew )
		nSize = g_xRandomOptionProperty->GetRandomOptionSize( pItemElem->GetRandomOptItemId() );
	else nSize = g_xRandomOptionProperty->GetViewRandomOptionSize( m_n64NewOption );

	__int64 n64Options = 0;
	n64Options = ( bNew ? m_n64NewOption : pItemElem->GetRandomOptItemId() );

	CString str;

	// option
	for( int i = 0; i < nSize; i++ )
	{
		int nDst = 0, nAdj = 0;
		if( !g_xRandomOptionProperty->GetParam( n64Options, i, &nDst, &nAdj ) )
			continue;

		if( IsDst_Rate( nDst ) )
		{
			if( nDst == DST_ATTACKSPEED )
				str.Format( "\n%s %c%d%% ", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj / 2 / 10 ) );
			else
				str.Format( "\n%s %c%d%%", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj ) );
		}
		else
		{
			str.Format( "\n%s %c%d", FindDstString( nDst ), ( nAdj > 0? '+': '-' ), ::abs( nAdj ) );
		}

		//�� �������پ� ����
		if( bNew )
			p2DRender->TextOut( 70, 92 + i * 20, str, 0xff000000 );
		else p2DRender->TextOut( 70, 20 + i * 20, str, 0xff000000 );
	}
}

void CWndSelectAwakeCase::SetData( BYTE byObjID, DWORD dwSerialNum, __int64 n64NewOption ) 
{ 
	//server�� ������ �����͸� �����ϰ�, ������ Index�� �̾Ƽ� �������� �׷��� �غ�
	m_byObjID = byObjID;
	m_dwSerialNum = dwSerialNum;
	m_n64NewOption = n64NewOption; 

	if( !g_pPlayer )
	{
		assert( 0 );
		return;
	}

	CItemBase* pItemBase = g_pPlayer->GetItemId( m_byObjID );
	if( pItemBase )
	{
		ItemProp* pProp = pItemBase->GetProp( );
		if(pProp)
			m_dwItemIndex = pProp->dwID;
	}
}

HRESULT CWndSelectAwakeCase::RestoreDeviceObjects()
{
	CWndBase::RestoreDeviceObjects();
	if( m_pVertexBufferGauge == NULL )
		m_pApp->m_pd3dDevice->CreateVertexBuffer(sizeof(TEXTUREVERTEX2) * 3 * 6, 
												 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
												 D3DFVF_TEXTUREVERTEX2, 
												 D3DPOOL_DEFAULT, 
												 &m_pVertexBufferGauge, 
												 NULL);
	assert(m_pVertexBufferGauge != NULL);
	
	return S_OK;
}
HRESULT CWndSelectAwakeCase::InvalidateDeviceObjects()
{
	CWndBase::InvalidateDeviceObjects();
    SAFE_RELEASE( m_pVertexBufferGauge );
	return S_OK;
}
HRESULT CWndSelectAwakeCase::DeleteDeviceObjects()
{
	CWndBase::DeleteDeviceObjects();
	SAFE_RELEASE( m_pVertexBufferGauge );
	return S_OK;
}

#endif	//__PROTECT_AWAKE