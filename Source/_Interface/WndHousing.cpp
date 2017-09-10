#include "stdafx.h"
#include "resData.h"
#include "defineText.h"

#if __VER >= 13 // __HOUSING
#ifdef __CLIENT

#include "WndHousing.h"
#include "DPClient.h"

#if __VER >= 15 // __GUILD_HOUSE
#include "GuildHouse.h"
#include "WndGuildHouse.h"
#endif

extern	CDPClient	g_DPlay;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************
  WndId : APP_HOUSING - �Ͽ�¡
  CtrlId : WIDC_BUTTON1 - ��ġ�ϱ�
  CtrlId : WIDC_BUTTON2 - ��ü�ϱ�
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_STATIC1 - ����
  CtrlId : WIDC_STATIC2 - ����
  CtrlId : WIDC_STATIC3 - �����۸�
  CtrlId : WIDC_STATIC4 - ���� �ð�
  CtrlId : WIDC_STATIC5 - ��ġ
  CtrlId : WIDC_LISTBOX1 - Listbox
****************************************************/

bool CompType_Greater( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_nType > second.m_nType;
}
bool CompType_Smaller( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_nType < second.m_nType;
}

bool CompDeploy_Greater( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_bDeploy > second.m_bDeploy;
}
bool CompDeploy_Smaller( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_bDeploy < second.m_bDeploy;
}

bool CompTime_Greater( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_dwTime > second.m_dwTime;
}
bool CompTime_Smaller( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	return first.m_dwTime < second.m_dwTime;
}

bool CompName_Greater( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	if(first.m_strName.Compare(second.m_strName) < 0)
		return true;
	else
		return false;
}
bool CompName_Smaller( HOUSING_ITEM& first, HOUSING_ITEM& second )
{
	if(first.m_strName.Compare(second.m_strName) > 0)
		return true;
	else
		return false;
}

CWndHousing::CWndHousing() 
{ 
	m_vecItem.clear();
	m_mapItem.clear();
	m_nSortType = WIDC_STATIC2;
	m_bIsGreater = TRUE;
	m_nSelected = 0;
} 

CWndHousing::~CWndHousing() 
{ 
} 

void CWndHousing::SerializeRegInfo( CAr& ar, DWORD& dwVersion )
{
	CWndNeuz::SerializeRegInfo( ar, dwVersion );
}

void CWndHousing::OnDraw( C2DRender* p2DRender ) 
{ 
	if(m_vecItem.size())
	{
		vector<HOUSING_ITEM>::iterator iter;
		CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
		LPWNDCTRL		pCustom = NULL;
		DWORD			dwColor;
		int				nIndex = 0;

		int nListFontHeight = pWndListBox->GetFontHeight() + 1;

		pCustom = GetWndCtrl( WIDC_LISTBOX1 );
		dwColor = D3DCOLOR_ARGB( 255, 0, 0, 0 );

		int nDrawCount = 0;
		int nLine = 0;

		for(iter = m_mapItem.begin(); iter != m_mapItem.end(); ++iter)
		{
			if(m_nSelected != 0 && (m_nSelected - 1) == nLine)
				dwColor = D3DCOLOR_ARGB( 255, 0, 0, 255 );
			else
				dwColor = D3DCOLOR_ARGB( 255, 0, 0, 0 );

			if(nLine < pWndListBox->GetScrollPos()) 
			{
				nLine++;
				continue;	
			}
			else
				nLine++;

			if(nDrawCount >= 10) continue;	// 10�� ������ ������Ѵ�

			// Ÿ��
			switch(iter->m_nType)
			{
				case IK3_BED:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_BED), dwColor);
					break;
				case IK3_SOFA:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_SOFA), dwColor);
					break;
				case IK3_WARDROBE:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_WARDROBE), dwColor);
					break;
				case IK3_CLOSET:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_CLOSET), dwColor);
					break;
				case IK3_TABLE:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_TABLE), dwColor);
					break;
				case IK3_CABINET:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_CABINET), dwColor);
					break;
				case IK3_PROPS:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_PROPS), dwColor);
					break;
				case IK3_WALLPAPER:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_WALLPAPER), dwColor);
					break;
				case IK3_CARPET:
					p2DRender->TextOut( pCustom->rect.left + 5, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, prj.GetText(TID_GAME_HOUSING_CARPET), dwColor);
					break;
			};
			// �̸�
			p2DRender->TextOut( pCustom->rect.left + 85, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, iter->m_strName, dwColor);

			// ���� �ð�
			time_t		t = (time_t)iter->m_dwTime - time_null();
			if(t < 0)	t = 0;
			CTimeSpan	ts( t );
			CString		strTime;

			// gmpbigsun : -_- 1�ð� �̸��̸� �ð��� ǥ���ϰ� ���ִ� -> ������ 100105
			if( ts.GetDays() <= 0 && ts.GetHours() <= 0)
				strTime.Format( prj.GetText( TID_GAME_TIME_1HOUR ));
			else
				strTime.Format( prj.GetText( TID_GAME_REMAIN_TIME ), static_cast<int>(ts.GetDays()), ts.GetHours());

			p2DRender->TextOut( pCustom->rect.left + 250, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, strTime, dwColor);

			// ��ġ ����
			if(iter->m_bDeploy)
				p2DRender->TextOut( pCustom->rect.left + 375, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, "O", dwColor);
			else
				p2DRender->TextOut( pCustom->rect.left + 375, pCustom->rect.top + 8 + (nIndex)*nListFontHeight, "X", dwColor);

			++nIndex;
			++nDrawCount;
		}
	}
} 

void CWndHousing::RefreshItemList()
{

	HOUSING_ITEM	tmpItem;
	int				nIndex = 1;

	// ������ ����Ʈ�� �� ���� �����Ѵ�
	m_vecItem.clear();
	m_mapItem.clear();
	
	CHousing::GetInstance()->GetHousingList( m_vecItem );

	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );

	pWndListBox->ResetContent();
	
	for(vector<HOUSINGINFO>::iterator iter = m_vecItem.begin(); iter != m_vecItem.end(); ++iter)
	{
		ItemProp *pItemProp = prj.GetItemProp(iter->dwItemId);
		
		pWndListBox->AddString(" ");
		tmpItem.m_nIndex  = nIndex;
		tmpItem.m_bDeploy = iter->bSetup;
		tmpItem.m_dwTime  = iter->tKeepTime; 
		tmpItem.m_strName = pItemProp->szName;
		tmpItem.m_strDesc = pItemProp->szCommand;
		tmpItem.m_nType   = pItemProp->dwItemKind3;
		tmpItem.dwItemId  = iter->dwItemId;

		m_mapItem.push_back(tmpItem);
		++nIndex;
	}
	
	Sort();
	m_nSelected = 0;
}

void CWndHousing::Sort()
{
	if(m_mapItem.size() > 1)
	{
		switch(m_nSortType)
		{
			case WIDC_STATIC2:		// Ÿ��
				if(m_bIsGreater)
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompType_Greater);
				else
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompType_Smaller);
				break;
			case WIDC_STATIC3:		// �����۸�
				if(m_bIsGreater)
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompName_Greater);
				else
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompName_Smaller);
				break;
			case WIDC_STATIC4:		// �����ð�
				if(m_bIsGreater)
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompTime_Greater);
				else
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompTime_Smaller);
				break;
			case WIDC_STATIC5:		// ��ġ
				if(m_bIsGreater)
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompDeploy_Greater);
				else
					std::sort( m_mapItem.begin(), m_mapItem.end(), CompDeploy_Smaller);
				break;
		};
	}
}

void CWndHousing::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	RefreshItemList();

	CWndButton* pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
	CWndButton* pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
	pWndButton1->EnableWindow(FALSE);
	pWndButton2->EnableWindow(FALSE);

	//ListBox
	CSize size = g_Neuz.m_2DRender.m_pFont->GetTextExtent( "123" );

	if(size.cy+2 > 16)
	{
		CRect rect;
		LPWNDCTRL lpWndCtrl;

		CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
		lpWndCtrl = GetWndCtrl( WIDC_LISTBOX1 );
		rect = lpWndCtrl->rect;
		rect.bottom += ((size.cy+2)*10) - (rect.bottom-rect.top) + 4;
		//��Ʈ ũ�� ���� ������ ����Ʈ�ڽ� ũ�� ����
		pWndList->SetWndRect(rect);

		//â �и��� ��� �Ʒ��� ũ�� ����
		CRect rectStatic, recText;
		CWndStatic* pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
		lpWndCtrl = GetWndCtrl( WIDC_STATIC1 );
		rectStatic = lpWndCtrl->rect;
		pWndStatic->Move(rectStatic.left, rect.bottom + 10);

		CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
		lpWndCtrl = GetWndCtrl( WIDC_TEXT1 );
		recText = lpWndCtrl->rect;
		recText.top = rect.bottom + rectStatic.Height() + 18;
		pWndText->SetWndRect(recText, TRUE);
	}

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndHousing::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_HOUSING, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndHousing::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndHousing::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndHousing::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
}

BOOL CWndHousing::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	return TRUE;
}

void CWndHousing::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	
	LPWNDCTRL pCustom = NULL;
	pCustom = GetWndCtrl( WIDC_STATIC2 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC2 - ����
		if(m_nSortType == WIDC_STATIC2)
			m_bIsGreater = !m_bIsGreater;
		else
		{
			m_nSortType = WIDC_STATIC2;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
		CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
		pWndButton1->EnableWindow(FALSE);
		pWndButton2->EnableWindow(FALSE);
	}
	pCustom = GetWndCtrl( WIDC_STATIC3 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC3 - �����۸�
		if(m_nSortType == WIDC_STATIC3)
			m_bIsGreater = !m_bIsGreater;
		else
		{
			m_nSortType = WIDC_STATIC3;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
		CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
		pWndButton1->EnableWindow(FALSE);
		pWndButton2->EnableWindow(FALSE);
	}
	pCustom = GetWndCtrl( WIDC_STATIC4 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC4 - ���� �ð�
		if(m_nSortType == WIDC_STATIC4)
			m_bIsGreater = !m_bIsGreater;
		else
		{
			m_nSortType = WIDC_STATIC4;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
		CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
		pWndButton1->EnableWindow(FALSE);
		pWndButton2->EnableWindow(FALSE);
	}
	pCustom = GetWndCtrl( WIDC_STATIC5 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC5 - ��ġ
		if(m_nSortType == WIDC_STATIC5)
			m_bIsGreater = !m_bIsGreater;
		else
		{
			m_nSortType = WIDC_STATIC5;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
		CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
		pWndButton1->EnableWindow(FALSE);
		pWndButton2->EnableWindow(FALSE);
	}
	
} 
void CWndHousing::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndHousing::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	int				nLoop		= 0;

	switch(nID)
	{
		case WIDC_LISTBOX1: // view ctrl
			{
				CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
				CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
				CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
				CWndText*		pWndText	= (CWndText*)GetDlgItem( WIDC_TEXT1 );
				
				m_nSelected					= pWndListBox->GetCurSel() + 1;

				for(vector<HOUSING_ITEM>::iterator iter = m_mapItem.begin(); iter != m_mapItem.end(); ++iter)
				{
					++nLoop;
					if(m_nSelected > nLoop)	continue;
					else if(m_nSelected == nLoop)
					{
						
						// ������ ����
						pWndText->SetString(iter->m_strDesc);
						
						//��ġ�Ǿ����� ������ư Ȱ��/��ġ��ư ��Ȱ��, �ȵǾ����� �ݴ��
						if(iter->m_bDeploy)
						{
							pWndButton1->EnableWindow(FALSE);
							pWndButton2->EnableWindow(FALSE);
							if(g_pPlayer)
							{
								CWorld* pWorld = g_pPlayer->GetWorld();
								if(pWorld)
								{
									if(pWorld->GetID() == WI_WORLD_MINIROOM && CDeployManager::GetInstance()->IsMyRoom())
										pWndButton2->EnableWindow(TRUE);
								} 
							}
						}
						else
						{
							pWndButton1->EnableWindow(FALSE);
							pWndButton2->EnableWindow(FALSE);
							if(g_pPlayer)
							{
								CWorld* pWorld = g_pPlayer->GetWorld();
								if(pWorld)
								{
									if(pWorld->GetID() == WI_WORLD_MINIROOM && CDeployManager::GetInstance()->IsMyRoom())
										pWndButton1->EnableWindow(TRUE);
								} 
							}
						}
					}
				}
			}
			break;

		case WIDC_BUTTON1:// ��ġ��ư
			{
				if(CDeployManager::GetInstance()->IsReady())
						CDeployManager::GetInstance()->EndDeploy();

				CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
				for(vector<HOUSING_ITEM>::iterator iter = m_mapItem.begin(); iter != m_mapItem.end(); ++iter)
				{
					++nLoop;
					if(m_nSelected > nLoop)	continue;
					else if(m_nSelected == nLoop)
					{
						HOUSINGINFO		tmpData;
						ItemProp*		pItemProp = prj.GetItemProp(iter->dwItemId);
						tmpData.bSetup    = TRUE;
						tmpData.dwItemId  = iter->dwItemId;
						tmpData.fAngle    = 0.0f;
						tmpData.tKeepTime = 0;

						if(iter->m_nType == IK3_WALLPAPER || iter->m_nType == IK3_CARPET)
						{
							g_DPlay.SendHousingReqSetupFurniture(tmpData);
						}
						else
						{
							CDeployManager::GetInstance()->LoadToDeploy(pItemProp->dwLinkKind, tmpData);
							CWndWorld* pWndWorld = (CWndWorld*)GetWndBase( APP_WORLD );
							if(pWndWorld) pWndWorld->SetFocus();
						}

						CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
						CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
						pWndButton1->EnableWindow(FALSE);
						pWndButton2->EnableWindow(FALSE);
					}
				}
			}
			break;

		case WIDC_BUTTON2:// ������ư
			if(!CDeployManager::GetInstance()->IsReady())
			{
				CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
				for(vector<HOUSING_ITEM>::iterator iter = m_mapItem.begin(); iter != m_mapItem.end(); ++iter)
				{
					++nLoop;
					if(m_nSelected > nLoop)	continue;
					else if(m_nSelected == nLoop)
					{
						HOUSINGINFO		tmpData;
						ItemProp*		pItemProp = prj.GetItemProp(iter->dwItemId);
						tmpData.bSetup    = FALSE;
						tmpData.dwItemId  = iter->dwItemId;
						tmpData.fAngle    = 0.0f;
						tmpData.tKeepTime = 0;
						g_DPlay.SendHousingReqSetupFurniture( tmpData );
						CWndButton*		pWndButton1 = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
						CWndButton*		pWndButton2 = (CWndButton*)GetDlgItem(WIDC_BUTTON2);
						pWndButton1->EnableWindow(FALSE);
						pWndButton2->EnableWindow(FALSE);
					}
				}	
			}
			break;
	
		
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWndGuildHousing
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if __VER >= 15 // __GUILD_HOUSE

CWndGuildHousing::CWndGuildHousing( )
{
	m_eSection = GS_FURNITURE;		// 'R'
	m_bDeploying = FALSE;
	m_pGHShowOne = NULL;
	m_dwComboCurrIK3 = 0;
	m_nSelectedSort = 0;
}

CWndGuildHousing::~CWndGuildHousing( )
{
	SAFE_DELETE( m_pGHShowOne );
}

BOOL CWndGuildHousing::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GH_FURNITURE_STORAGE, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndGuildHousing::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	CWndComboBox* pWndCombo = (CWndComboBox*)GetDlgItem( WIDC_COMBOBOX1 );
	if( pWndCombo )
	{
		pWndCombo->AddWndStyle( EBS_READONLY );
		pWndCombo->ResetContent( );
		int nIndex = -1;

		nIndex = pWndCombo->AddString( GETTEXT(TID_GAME_GUILDHOUSE_FURKIND) );
		pWndCombo->SetItemData( nIndex, 0 );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_BED ) );
		pWndCombo->SetItemData( nIndex, IK3_BED );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_TABLE ) );
		pWndCombo->SetItemData( nIndex, IK3_TABLE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_SOFA ) );
		pWndCombo->SetItemData( nIndex, IK3_SOFA );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_DESK ) );
		pWndCombo->SetItemData( nIndex, IK3_DESK );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CHAIR ) );
		pWndCombo->SetItemData( nIndex, IK3_CHAIR );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CASE ) );
		pWndCombo->SetItemData( nIndex, IK3_CASE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CLOSET ) );
		pWndCombo->SetItemData( nIndex, IK3_CLOSET );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_WARDROBE ) );
		pWndCombo->SetItemData( nIndex, IK3_WARDROBE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CARPET ) );
		pWndCombo->SetItemData( nIndex, IK3_CARPET );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_WALLPAPER ) );
		pWndCombo->SetItemData( nIndex, IK3_WALLPAPER );
	}

	pWndCombo->SetCurSel( 0 );

	int sHeight = 30;

	CRect rect( 368, 10, 408, 40 );
	for( int i = 0; i < GH_MAX_VIEW_CAPACITY; ++i )
	{
		rect.top = 55 + ( i * sHeight ) + ( i + 1 ) * 10;
		rect.bottom = 65 + ( i * sHeight ) + sHeight + ( i + 1 ) * 10;
		m_wndButton[i].Create( _T("BTN"), WBS_CHILD, rect, this, i ); 
		m_wndButton[i].SetWndRect( rect );
		m_wndButton[i].SetVisible( FALSE );
	}

	//����Ʈ�ڽ��� �� �������� ���� 
	CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	pWndListBox->m_nLineSpace = (int)( ( 40 - pWndListBox->GetFontHeight( ) ) * 0.5f );

	// ���⿡ �ڵ��ϼ���
	RefreshItemList();

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();

	CWndStatic* pStatic = NULL;
	pStatic = (CWndStatic *)GetDlgItem( WIDC_STATIC4 );
	pStatic->SetVisible( FALSE );

	pStatic = (CWndStatic *)GetDlgItem( WIDC_STATIC5 );
	pStatic->SetVisible( FALSE );

	m_texUp.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "LvUp.bmp" ), 0xffff00ff );
	m_texDown.LoadTexture( m_pApp->m_pd3dDevice, MakePath( DIR_THEME, "LvDn.bmp" ), 0xffff00ff );
	
} 


void CWndGuildHousing::OnDraw( C2DRender* p2DRender ) 
{ 
	if( !m_cWndItems.empty() )
	{
		vector<HOUSING_ITEM>::iterator iter;
		CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
		assert( pWndListBox );
	
		LPWNDCTRL		pCustom = NULL;
		DWORD			dwColor;
		int				nIndex = 0;
		static const int nListFontHeight = 40; //pWndListBox->GetFontHeight() + 1;

		UpdateIRButton( );

		pCustom = GetWndCtrl( WIDC_LISTBOX1 );
		dwColor = D3DCOLOR_ARGB( 255, 0, 0, 0 );

		int nDrawCount = 0;
		int nLine = 0;

		bool bSelected = false;
		CRect renderBoxRect = pCustom->rect;
		renderBoxRect.left += 5;
		renderBoxRect.right -= 28;
		
		CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
		if( !pWndText )
			return;

		BOOL bNoSelecting = TRUE;

		for(iter = m_cWndItems.begin(); iter != m_cWndItems.end(); ++iter)
		{
			//���� ����
			bSelected = ( m_nSelected != 0 && (m_nSelected - 1) == nLine );
	
			//���� �ö� ���� �н� 
			if(nLine < pWndListBox->GetScrollPos()) 
			{
				nLine++;
				continue;	
			}
			else
				nLine++;

			//����� �ִ��ġ 
			if(nDrawCount >= GH_MAX_VIEW_CAPACITY )		
				continue;	

			const HOUSING_ITEM& kItem = *iter;

			if( kItem.m_bDeploy )
				dwColor = 0xff0000ff;
			else dwColor = 0xff000000;

			//Render icon
			CPoint pt( pCustom->rect.left + 16, pCustom->rect.top + 10 + (nIndex)*nListFontHeight );
			ItemProp* pProp = prj.GetItemProp( kItem.m_nIndex );
			if( !pProp )
				continue;

		//	if( IsSection( GS_TELEPORTER ) )
		//		p2DRender->TextOut( pCustom->rect.left + 55, pCustom->rect.top + 20 + ( nIndex )*nListFontHeight, "1Lv", 0xffcc11cc );

			CTexture* pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon), 0xffff00ff );
			if( pTexture )
				p2DRender->RenderTexture( pt, pTexture );
		
			// �̸�
			p2DRender->TextOut( pCustom->rect.left + 80, pCustom->rect.top + 20 + (nIndex)*nListFontHeight, kItem.m_strName, dwColor);

			// ���� �ð�
			bool bGoTime = true;
			time_t		t = (time_t)iter->m_dwTime - time_null();
			if(t < 0)	
			{
				t = iter->m_dwTime;	// ����Ȱ��� �ڵ����� ������ ������ �̰�� ���� ��ġ�� �ȉ� ����.( Prop �ð� ���� )
				bGoTime = false;
			}
			CTimeSpan	ts( t );
			CString		strTime;

			if( ts.GetDays( ) )
				strTime.Format( prj.GetText( TID_PK_LIMIT_DAY ), ts.GetDays() );
			else if( ts.GetHours( ) )
				strTime.Format( prj.GetText(TID_PK_LIMIT_HOUR ), ts.GetHours() );
			else 
				strTime.Format( prj.GetText(TID_PK_LIMIT_MINUTE ), ts.GetMinutes() );

			//gmpbigsun(100326) : ������ Ư����ȣ ���� �� ����(NEW)->�ܺηκ��� �о��
			if( !bGoTime )
				strTime = strTime + CString( prj.GetText( TID_GAME_TOOLTIP_NEWFURNITURE ) ); 		//��ġ�� �ѹ��� �ȴ�( �ð��� �Ȱ��� )			

			p2DRender->TextOut( pCustom->rect.left + 260, pCustom->rect.top + 20 + (nIndex)*nListFontHeight, strTime, dwColor);

			//���ÿ��� �ڽ�
			if( bSelected )
			{
				bNoSelecting = FALSE;
				renderBoxRect.top = renderBoxRect.top + 4 + (nIndex)*nListFontHeight,
				renderBoxRect.bottom = renderBoxRect.top + 40;
				p2DRender->RenderFillRect( renderBoxRect, 0x55ff0000 );

				// ��ɼ��� 
				pWndText->SetString( kItem.m_strDesc );
			}
		
			++nIndex;
			++nDrawCount;
		}

		if( bNoSelecting )
			pWndText->SetString( "" );
	}

	if( m_nSelectedSort != 0 )
	{
		//��Ʈ ��ũ 
		CWndStatic* pWndStatic = (CWndStatic*)GetDlgItem( m_nSelectedSort );
		if( pWndStatic )
		{
			CRect rect = pWndStatic->GetClientRect( TRUE );

			if( m_bIsGreater )	//��������
				p2DRender->RenderTexture( CPoint( rect.left-18, rect.top ), &m_texUp );
			else	//��������
				p2DRender->RenderTexture( CPoint( rect.left-18, rect.top ), &m_texDown );
		}
	}
} 

void CWndGuildHousing::RefreshItemList( )
{
	HOUSING_ITEM	tmpItem;
	int				nIndex = 1;

	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	int currScrollPos = pWndListBox->GetScrollPos( );		// ���� ��ũ�� ��ġ�� �����صд�.
	pWndListBox->ResetContent();

	// ����, ���� ��ü�� ����flag
	BOOL bWallTex = FALSE;
	BOOL bTileTex = FALSE;

	// ������ ����Ʈ�� �� ���� �����Ѵ�
	m_cWndItems.clear();

	BOOL bShowWanted = FALSE;
	if( 0 != m_dwComboCurrIK3 )
		bShowWanted = TRUE;

	int iMaxSize = GuildHouse->GetFurnitureListSize( );
	for( int i = 0; i < iMaxSize; ++i )
	{
		GH_Fntr_Info* pInfo = GuildHouse->GetFurnitureInfoPtr( i );
		if( !pInfo )
			continue;

		ItemProp* pProp = prj.GetItemProp( pInfo->dwItemId );
		if( !pProp )
		{
			assert( 0 );
			continue;
		}

		if( bShowWanted && m_dwComboCurrIK3 != pProp->dwItemKind3 )			//�޺��ڽ��� ���������� �����Ѱ�� �ش簡������ �����ش�.
			continue;

		// ���� ������ ������ �ڷ����� �н�, ���� ������ �ڷ����͸� ���� �н� 
		if( GS_FURNITURE == m_eSection )
		{
			if( IK2_GUILDHOUSE_NPC == pProp->dwItemKind2 )
				continue;
		}
		else
		if( GS_TELEPORTER == m_eSection )
		{
			// ����, ����/���� 
			if( IK2_GUILDHOUSE_FURNITURE == pProp->dwItemKind2 || IK2_GUILDHOUSE_PAPERING == pProp->dwItemKind2 )  
				continue;
		}

		pWndListBox->AddString( " " );

		tmpItem.m_nIndex = pInfo->dwItemId;
		tmpItem.m_bDeploy = pInfo->bSetup;
		tmpItem.m_dwTime = pInfo->tKeepTime;
		
		// ���� ��ġ�� �ѹ��� �ȉ°�� �����ð��� ����(�����κ���)�ʴ´�. ��� Prop data�� ����.
		if( 0 >= tmpItem.m_dwTime )
			tmpItem.m_dwTime = pProp->dwAbilityMin * 60;
		tmpItem.dwItemId = pInfo->objId;
		tmpItem.m_strName = pProp->szName;		//������Ƽ �̸� 
		tmpItem.m_strDesc = pProp->szCommand;	//  "      ���� 
		tmpItem.m_nSlotIndex = i;

		m_cWndItems.push_back( tmpItem );
		
		++nIndex;
	}

	GuildHouse->ApplyEFTexture( );
	
	Sort();

	// Ÿ���� �ִٸ� �����Ŀ� �ش������ ���õȻ��·� ...
	if( !IsValidObjID( GuildHouse->m_dwSelectedObjID ) )
		m_nSelected = 0;
	else if( !SetSelectedByObjID( GuildHouse->m_dwSelectedObjID ) )
		m_nSelected = 0;

	pWndListBox->SetScrollPos( currScrollPos );

	UpdateIRButton( );
}

void CWndGuildHousing::UpdateIRButton( )
{
	// �����ۺ� ��ư Update!!
	if( !g_pPlayer )
		return;

	if( !g_pPlayer->GetWorld( ) )
		return;

	BOOL bResult = FALSE;
	CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )
		bResult = pGuild->IsAuthority( g_pPlayer->m_idPlayer, PF_GUILDHOUSE_FURNITURE );
	
	if( bResult )
	{
		if( !g_pPlayer->GetWorld()->IsWorldGuildHouse() )			//����Ͽ콺 ���尡 �ƴϸ� 
			bResult = FALSE;
	}

	SetEnableInstallBtns( bResult );

	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	int nScrollPos = pWndListBox->GetScrollPos();		//�� ��༮�� ���� top�ΰ�?
	if( nScrollPos < 0 )
		pWndListBox->SetScrollPos( 0 );

	int iMaxSize = m_cWndItems.size( );
	for( int i = 0; i < GH_MAX_VIEW_CAPACITY; ++i )
	{
		int top = i + nScrollPos;						//���� �����ʹ� ��ũ�� �����ǿ� ������ �ް�
		
		CWndButton* pBtn = &m_wndButton[i];

		//�ڷ����� â�ϰ�� ��ġ�� �༮�� �ִٸ� ��ġ �ȵȳ༮���� disable 
		if( top < iMaxSize )
		{
			//�����Ͱ� �����ϴ°�츸 ��ư�� ���̰� �ϰ� ��ġ/��ü �� ��Ȳ�� �°� �ٲپ��ش�.
			pBtn->SetVisible( TRUE );		//��ư�� ������ 0���ʹ�.
			
			if( m_cWndItems[ top ].m_bDeploy )		
			{
				// ��ü�ϴ� �̹��� ( ��ġ�� �Ǿ������Ƿ� )
				
				//sunTODO: Ȯ�� �̹��� ������ ��ü��� 
				pBtn->SetTexture( D3DDEVICE, MakePath( DIR_THEME, "Buttlockerinstall.BMP" ), TRUE );
				pBtn->SetToolTip( GETTEXT(TID_TOOLTIP_GUILDHOUSE_BUTT_DISMANTLE) );		//������ ��ü�˴ϴ�.
			}
			else									
			{
				// ��ġ�ϴ� �̹��� ( ��ü�� �Ǿ������Ƿ� )
				
				//sunTODO: Ȯ�� �̹��� ������ ��ü���
				pBtn->SetTexture( D3DDEVICE, MakePath( DIR_THEME, "Buttlockerdismantle.BMP" ), TRUE );
				pBtn->SetToolTip( GETTEXT(TID_TOOLTIP_GUILDHOUSE_BUTT_INSTALL) );		//������ ��ġ�˴ϴ�.
			}
		}
		else 
			pBtn->SetVisible( FALSE );
	}

	// �ڷ����͸� ��ġ�Ҽ� �ִ°�� ��ġ���� �ڷ����� �ܿ��� disable ��Ų��.
	if( bResult && IsSection( GS_TELEPORTER ) )
	{
		BOOL bInstalledTeleporter = FALSE;
		for( int i = 0; i < iMaxSize; ++i )
		{
			if( m_cWndItems[ i ].m_bDeploy )
				bInstalledTeleporter = TRUE;

			m_wndButton[i].EnableWindow(TRUE);
		}

		if( bInstalledTeleporter )
		{
			for( int btnIndex = 0; btnIndex < iMaxSize; ++btnIndex )
			{
				if( !m_cWndItems[ btnIndex ].m_bDeploy )
					m_wndButton[ btnIndex ].EnableWindow( FALSE );
			}
		}
	}
}

void CWndGuildHousing::Sort()
{
	if( !m_cWndItems.empty( ) )
	{
		switch(m_nSortType)
		{
			case WIDC_STATIC1:		// �����۸�
				if(m_bIsGreater)
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompName_Greater);
				else
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompName_Smaller);
				break;
			case WIDC_STATIC2:		// �����ð�
				if(m_bIsGreater)
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompTime_Greater);
				else
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompTime_Smaller);
				break;
			case WIDC_STATIC3:		// ��ġ
				if(m_bIsGreater)
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompDeploy_Greater);
				else
					std::sort( m_cWndItems.begin(), m_cWndItems.end(), CompDeploy_Smaller);
				break;
		}

	}

}

BOOL CWndGuildHousing::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	if( nID >= 0 && nID < GH_MAX_VIEW_CAPACITY )
	{
		//����Ʈ�ȿ� �����ۺ� ��ư�� ���ȴ�..

		// ��ġ, �缳ġ, ��ü�� ���� �ʴ� ä���ϰ�� �޼��� ����ϰ� Retrun
		if( !GuildHouse->IsSetFurnitureChannel( ) )
		{
			g_WndMng.PutString( GETTEXT( TID_GAME_GUILDHOUSE_INSIDE_INSTALL_CHANNEL  ) );
			return CWndNeuz::OnChildNotify( message, nID, pLResult );
		}

		//��ũ���� �Ȱ�� nID�� ž�ε����� ���� ���̴�.
		CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
		int scrollPos = pWndListBox->GetScrollPos( );

		//���콺 ���� �ø���� ScrollPos�� ������ �Ǵ����� ���� 
		if( scrollPos < 0 )
			pWndListBox->SetScrollPos( 0 );					
		int resultIndex = nID + pWndListBox->GetScrollPos( );

		assert( resultIndex > -1 );

		HOUSING_ITEM& kItem = m_cWndItems[ resultIndex ];
		assert( (int)( m_cWndItems.size() ) > resultIndex );

		if( !kItem.m_bDeploy )		//��ġ���� �ʾҴٸ� 
		{
			kItem.m_vPos = g_pPlayer->GetPos( );
			kItem.m_fAngle = 0.0f;

			ItemProp* pItemProp = prj.GetItemProp( kItem.m_nIndex );
			assert( pItemProp );

			// �ڷ�����, ����, ������ ��� �ٷ� ��ġ ��û , ������ ��� �������� ����
			if( IK3_TELEPORTER == pItemProp->dwItemKind3 || IK3_WALLPAPER == pItemProp->dwItemKind3 || IK3_CARPET == pItemProp->dwItemKind3 )
			{
				GH_Fntr_Info* pInfo = GuildHouse->GetFurnitureInfoPtr( kItem.m_nSlotIndex );
				assert( pInfo );
				// �̹� ��ġ�Ǿ� �ִ� �ڷ����Ͱ� �ִ��� �˻� 
				GuildHouse->SendClientToWorld( GUILDHOUSE_PCKTTYPE_SETUP, *pInfo, kItem.m_nSlotIndex );
			}else
			{
				GuildDeploy()->LoadToDeploy(pItemProp->dwLinkKind, kItem );
				GuildHouse->m_iMode = GUILDHOUSE_PCKTTYPE_SETUP;
				
				CWndWorld* pWndWorld = (CWndWorld*)GetWndBase( APP_WORLD );
				if(pWndWorld) 
					pWndWorld->SetFocus();

				GuildHouse->m_nWndDeployingIndex = resultIndex;
			}
		}
		else
		{
			GH_Fntr_Info* pInfo = GuildHouse->GetFurnitureInfoPtr( kItem.m_nSlotIndex );
			assert( pInfo );
			GuildHouse->SendClientToWorld( GUILDHOUSE_PCKTTYPE_REMOVE, *pInfo, kItem.m_nSlotIndex );
		}
	}

	switch(nID)
	{
		case WIDC_LISTBOX1: // view ctrl
		{
			int	nLoop = 0;
			CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
			m_nSelected					= pWndListBox->GetCurSel() + 1;

			for(vector<HOUSING_ITEM>::iterator iter = m_cWndItems.begin(); iter != m_cWndItems.end(); ++iter)
			{
				HOUSING_ITEM& kItem = *iter;
				++nLoop;
				if(m_nSelected > nLoop)	continue;
				else if(m_nSelected == nLoop)
				{
					//���� ���õ� �������̶�� 
					GuildHouse->m_dwSelectedObjID = kItem.dwItemId;
				}
			}
		}
		break;

		case WIDC_COMBOBOX1:
			{
				if( message == WNM_SELCHANGE )
				{
					CWndComboBox* pCombo = (CWndComboBox*)GetDlgItem(WIDC_COMBOBOX1);
					int curSel = pCombo->GetCurSel();
					m_dwComboCurrIK3 = pCombo->GetItemData( curSel );

					CWndListBox*	pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
					pWndListBox->SetCurSel( 0 );		// �޺��ڽ� �������� �����Ѱ�� ��ũ�� ��ġ�� �ֻ����� 
					RefreshItemList( );
				}
			}
			break;
	}



	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndGuildHousing::SetSelectedByObjID( OBJID objID )
{
	// ������Ʈ ���̵�� ���� �����ϴ� Data index�� �̴´�. ������Ʈ�� ���������� �����쿡 �ڵ����ñ���� ���� ������� 

	int totalSize = m_cWndItems.size();
	for( int i = 0; i < totalSize; ++i )
	{
		HOUSING_ITEM& kItem = m_cWndItems[ i ];
		if( kItem.dwItemId == objID )
		{
			m_nSelected = i+1;

			FixScrollBar( m_nSelected );
			
			return TRUE;
		}
	}

	return FALSE;
}

void CWndGuildHousing::FixScrollBar( const int nSelected )
{
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	if( !pWndListBox )
		return;

	int curScrollPos = pWndListBox->GetScrollPos( );
	int curSelected = pWndListBox->GetCurSel( );

	int nDis = 0;
	if( m_nSelected < ( curScrollPos + 1 ) )			//���ùڽ��� ��ũ�� ���� �ִ°�� 
	{
		nDis = m_nSelected - 1;
	}
	else if ( m_nSelected > curScrollPos + GH_MAX_VIEW_CAPACITY )	// ���ùڽ��� ��ũ�� �ִ뿵�� �Ʒ� �ִ°�� 
	{
		nDis = m_nSelected - GH_MAX_VIEW_CAPACITY;
	}
	else 
	{
		return;
	}

	pWndListBox->SetScrollPos( nDis );
}

int CWndGuildHousing::GetWndItemIndexByObjID( OBJID objID )
{
	int totalSize = m_cWndItems.size();
	for( int i = 0; i < totalSize; ++i )
	{
		HOUSING_ITEM& kItem = m_cWndItems[ i ];
		if( kItem.dwItemId == objID )
		{
			return i;
		}
	}

	return -1;
}

void CWndGuildHousing::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
	
	LPWNDCTRL pCustom = NULL;
	pCustom = GetWndCtrl( WIDC_STATIC1 );
	if( PtInRect(&pCustom->rect, point) )
	{
		if(m_nSortType == WIDC_STATIC1)
		{
			m_bIsGreater = !m_bIsGreater;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC1 );
		}
		else
		{
			m_bIsGreater = TRUE;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC1 );
			m_nSortType = WIDC_STATIC1;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
	}
	pCustom = GetWndCtrl( WIDC_STATIC2 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC2 - �ð� 
		if(m_nSortType == WIDC_STATIC2)
		{
			m_bIsGreater = !m_bIsGreater;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC2 );
		}
		else
		{
			m_bIsGreater = TRUE;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC2 );
			m_nSortType = WIDC_STATIC2;
			m_bIsGreater = TRUE;
		}
		RefreshItemList();
	}
	pCustom = GetWndCtrl( WIDC_STATIC3 );
	if( PtInRect(&pCustom->rect, point) )
	{
		//CtrlId : WIDC_STATIC3 - ��ġ����
		if(m_nSortType == WIDC_STATIC3)
		{
			m_bIsGreater = !m_bIsGreater;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC3 );
		}
		else
		{
			m_bIsGreater = TRUE;
			UpdateSortTextColor( m_nSortType, WIDC_STATIC3 );
			m_nSortType = WIDC_STATIC3;
			m_bIsGreater = TRUE;
		}
		
		RefreshItemList();
	}
} 

void CWndGuildHousing::UpdateSortTextColor( int oldType, int newType )
{
	// 2010_04_06 : �������� �������� ����ǥ��->������ ǥ��
	m_nSelectedSort = newType;

	//static const DWORD orgColor = 0xff2e70a9;		//�� ...
	//static const CString strUP = "^";
	//static const CString strDOWN = " ";
	//CWndStatic* pStatic = NULL;
	//pStatic = (CWndStatic*)GetDlgItem( oldType );
	//if( pStatic )
	//{
	//	pStatic->m_dwColor = orgColor;
	//	CString curTitle = pStatic->GetTitle( );

	//	if( -1 != curTitle.Find( strUP ) || -1 != curTitle.Find( strDOWN ) )
	//	{
	//		curTitle.Delete( curTitle.GetLength() - 1, 1 );
	//		pStatic->SetTitle( curTitle );
	//	}
	//}

	//pStatic = (CWndStatic*)GetDlgItem( newType );
	//if( pStatic )
	//{
	//	pStatic->m_dwColor = 0xffff1111;
	//	CString curTitle = pStatic->GetTitle( );
	//	if( m_bIsGreater )
	//		curTitle = curTitle + strUP;
	//	else curTitle = curTitle + strDOWN;

	//	pStatic->SetTitle( curTitle );
	//}
}

BOOL CWndGuildHousing::InitBySection( GH_SECTION eSection )
{
	SetSection( eSection );
	CWndComboBox* pWndCombo = (CWndComboBox*)GetDlgItem( WIDC_COMBOBOX1 );
	if( !pWndCombo )
		return FALSE;
		
	pWndCombo->SetCurSel( 0 );
	
	m_dwComboCurrIK3 = 0;

	if( GS_FURNITURE == eSection )
	{
		SetTitle( GETTEXT( TID_GAME_GUILDHOUSE_LOCKER ) ); //"Guild furniture storage" );
		pWndCombo->EnableWindow( TRUE );
	}
	else if( GS_TELEPORTER == eSection )
	{
		SetTitle( GETTEXT( TID_GAME_GUILDHOUSE_TELEPORTER ) ); //"Teleporter storage" );
		pWndCombo->EnableWindow( FALSE );
	}
	
	AutoAddingComboItems( );
	RefreshItemList( );

	return TRUE;
}

void CWndGuildHousing::AutoAddingComboItems( )
{
	CWndComboBox* pWndCombo = (CWndComboBox*)GetDlgItem( WIDC_COMBOBOX1 );
	if( !pWndCombo )
		return;

	pWndCombo->ResetContent( );

	if( GS_FURNITURE == m_eSection )
	{
		int nIndex = -1;

		nIndex = pWndCombo->AddString( GETTEXT(TID_GAME_GUILDHOUSE_FURKIND) );
		pWndCombo->SetItemData( nIndex, 0 );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_BED ) );
		pWndCombo->SetItemData( nIndex, IK3_BED );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_TABLE ) );
		pWndCombo->SetItemData( nIndex, IK3_TABLE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_SOFA ) );
		pWndCombo->SetItemData( nIndex, IK3_SOFA );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_DESK ) );
		pWndCombo->SetItemData( nIndex, IK3_DESK );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CHAIR ) );
		pWndCombo->SetItemData( nIndex, IK3_CHAIR );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CASE ) );
		pWndCombo->SetItemData( nIndex, IK3_CASE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CLOSET ) );
		pWndCombo->SetItemData( nIndex, IK3_CLOSET );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_WARDROBE ) );
		pWndCombo->SetItemData( nIndex, IK3_WARDROBE );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_CARPET ) );
		pWndCombo->SetItemData( nIndex, IK3_CARPET );

		nIndex = pWndCombo->AddString( GETTEXT( TID_MMI_GHOUSE_WALLPAPER ) );
		pWndCombo->SetItemData( nIndex, IK3_WALLPAPER );
	}
	else if( GS_TELEPORTER == m_eSection )
	{
		int index = pWndCombo->AddString( "TELEPORTER" );
		pWndCombo->SetItemData( index, 0 );
	}

	pWndCombo->SetCurSel( 0 );
}

BOOL CWndGuildHousing::Process()
{
	static CRect sRect;
	static CRect sOldRect = GetWindowRect( );
	static int snBackScrollPos = 0;
	
	// ��ġ/�缳ġ �����, ���� Visible���¿� ���� ����� ������ �����Ѵ�.( ��ġ ���� )
 	if( GUILDHOUSE_PCKTTYPE_SETUP == GuildHouse->m_iMode )
 	{
		sRect = m_rectCurrentWindow;

 		if( !m_bDeploying )
 		{
 			m_bDeploying = TRUE;
 			SetVisible( FALSE );
		
			SAFE_DELETE( m_pGHShowOne );
			m_pGHShowOne = new CWndGHouseShowOneUnit;
			m_pGHShowOne->Initialize( this );
			int wndItemIndex = GuildHouse->m_nWndDeployingIndex;
			
			assert( wndItemIndex > -1 && wndItemIndex < (int)( m_cWndItems.size() ) );

			if( wndItemIndex > -1 && wndItemIndex < (int)( m_cWndItems.size() ) )
				m_pGHShowOne->SetItem( m_cWndItems[ wndItemIndex ] );
		
		}
	}
	else
	{
		if( m_bDeploying )
		{
			SetVisible( TRUE );
			m_bDeploying = FALSE;
			GuildHouse->m_nWndDeployingIndex = -1;
			SAFE_DELETE( m_pGHShowOne );
		}
	}

	CWndStatic* pEndTime   = (CWndStatic*)GetDlgItem( WIDC_STATIC5 );
	if( !pEndTime )
		return FALSE;

	time_t	t = (time_t)GuildHouse->GetUpkeepTime() - time_null();
	if(t < 0)	
	{
		pEndTime->SetTitle( GETTEXT( TID_GAME_GUILDHOUSE_EXPIRATION ) ); //����Ͽ콺�� �����Ⱓ�� ����Ǿ����ϴ�.
		return TRUE;
	}
		
	CTimeSpan	ts( t );
	static CString	strDays, strHours, strMins, strSecs;
	CString strBind;
	const CString strSpace = " ";

	if( ts.GetDays( ) )
	{
		strDays.Format( prj.GetText( TID_PK_LIMIT_DAY ), ts.GetDays() );
		strBind += ( strSpace + strDays );
	}
	if( ts.GetHours( ) )
	{
		strHours.Format( prj.GetText(TID_PK_LIMIT_HOUR ), ts.GetHours() );
		strBind += ( strSpace + strHours );
	}
	if( ts.GetMinutes( ) )
	{
		strMins.Format( prj.GetText(TID_PK_LIMIT_MINUTE ), ts.GetMinutes() );
		strBind += ( strSpace + strMins );
	}
	if( ts.GetSeconds( ) )
	{
		strSecs.Format( prj.GetText(TID_PK_LIMIT_SECOND ), ts.GetSeconds() );
		strBind += ( strSpace + strSecs );
	}

	pEndTime->SetTitle( strBind );

	return TRUE;
}

void CWndGuildHousing::SetEnableInstallBtns( BOOL bEnable )
{
	for( int i = 0; i < GH_MAX_VIEW_CAPACITY; ++i )
	{
		m_wndButton[ i ].EnableWindow( bEnable );
	}
}

void CWndGuildHousing::CheckChannel( )
{
	if( GUILDHOUSE_PCKTTYPE_SETUP == GuildHouse->m_iMode || GUILDHOUSE_PCKTTYPE_RESET == GuildHouse->m_iMode )
	{
		if( !GuildHouse->IsSetFurnitureChannel( ) )
			g_WndMng.PutString( GETTEXT( TID_GAME_GUILDHOUSE_INSIDE_INSTALL_CHANNEL  ) );	//���� ä�ο����� ��ġ �ȴ��..
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CWndGHouseShowOneUnit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CWndGHouseShowOneUnit::CWndGHouseShowOneUnit( )
{
	m_bNoCloseButton = TRUE;
}

CWndGHouseShowOneUnit::~CWndGHouseShowOneUnit( )
{
}

BOOL CWndGHouseShowOneUnit::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GH_SHOWONE, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndGHouseShowOneUnit::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	// ������ �߾�, ž���� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( (int)( rectRoot.right * 0.5f - rectWindow.Width() * 0.5f ), 0 );
	Move( point );
}

void CWndGHouseShowOneUnit::OnDraw( C2DRender* p2DRender )
{
	//Render icon
	LPWNDCTRL pCustom = GetWndCtrl( WIDC_LISTBOX1 );
	if( !pCustom )
		return;

 	CPoint pt( pCustom->rect.left + 10, pCustom->rect.top );
 	ItemProp* pProp = prj.GetItemProp( m_kItem.m_nIndex );
 	if( !pProp )
		return;
 
 	CTexture* pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, MakePath( DIR_ITEM, pProp->szIcon), 0xffff00ff );
 	if( pTexture )
 		p2DRender->RenderTexture( pt, pTexture );
 
 	// �̸�
 	p2DRender->TextOut( pCustom->rect.left + 50, pCustom->rect.top + 12, m_kItem.m_strName, 0xff000000 );
 
 	// ���� �ð�
 	bool bGoTime = true;
 	time_t		t = (time_t)m_kItem.m_dwTime - time_null();
 	if(t < 0)	
 	{
 		t = m_kItem.m_dwTime;	// ����Ȱ��� �ڵ����� ������ ������ �̰�� ���� ��ġ�� �ȉ� ����.( Prop �ð� ���� )
 		bGoTime = false;
 	}
 	CTimeSpan	ts( t );
 	CString		strTime;

	if( ts.GetDays( ) )
		strTime.Format( prj.GetText( TID_PK_LIMIT_DAY ), ts.GetDays() );
	else if( ts.GetHours( ) )
		strTime.Format( prj.GetText(TID_PK_LIMIT_HOUR ), ts.GetHours() );
	else 
		strTime.Format( prj.GetText(TID_PK_LIMIT_MINUTE ), ts.GetMinutes() );

	if( !bGoTime )
		strTime = strTime + CString( " (NEW)" );		//��ġ�� �ѹ��� �ȴ�( �ð��� �Ȱ��� )
	else strTime = strTime + CString( " (��)" );		//��ġ�� �ѹ��̻� �Ǿ �ð��� �帣���� 

	p2DRender->TextOut( pCustom->rect.left + 200, pCustom->rect.top + 12, strTime, 0xff000000 );

	static DWORD tmOld = timeGetTime( );
	static BOOL bRenderBox = TRUE;

	if( ( g_tmCurrent - tmOld ) > 600 )
	{
		tmOld = timeGetTime();
		bRenderBox = !bRenderBox; 
	}
	
	if( bRenderBox )
		p2DRender->RenderFillRect( pCustom->rect, 0x55ff0000 );
}

#endif // __GUILD_HOUSE

#endif // __HOUSING


#ifdef __GUILD_HOUSE_MIDDLE
//CWndGuildHouseBid( ����Ͽ콺 ���� )
CWndGuildHouseBid::CWndGuildHouseBid( )
: m_bMaster( FALSE ), 
m_n64TenderPenya( 0 ),
m_bWaitResult( FALSE ),
m_dwWaitTime( 0 )
{
	_cBidDatas.reserve( 6 );
}

CWndGuildHouseBid::~CWndGuildHouseBid( )
{
	_cBidDatas.clear();
}

BOOL CWndGuildHouseBid::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_GH_BID, 0, CPoint( 0, 0 ), pWndParent );
} 

void CWndGuildHouseBid::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 

	//check validity
	CWndEdit* pEdit1 = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pEdit2 = (CWndEdit*) GetDlgItem( WIDC_EDIT2 );
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );

	if( !pEdit1 || !pEdit2 || !pWndListBox )
	{
		Destroy( );
		return;
	}

	// ������ �߾�, ž���� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( (int)( rectRoot.right * 0.5f - rectWindow.Width() * 0.5f ), 0 );
	Move( point );

	CMover* pMoverTarget = (CMover*)g_WorldMng.Get()->GetObjFocus() ;
	if( !pMoverTarget )
	{
		g_WndMng.OpenMessageBox( "Can not find selected NPC" );
		Destroy( );
		return;
	}

	pEdit1->AddWndStyle( EBS_NUMBER );
	pEdit2->AddWndStyle( EBS_NUMBER );

	CWndStatic* pWndPenya = (CWndStatic*)GetDlgItem(WIDC_STATIC7);
	pWndPenya->AddWndStyle(WSS_MONEY);
	pWndPenya->AddWndStyle( WSS_ALIGNHRIGHT );


	ResetInputMoneyWindows( );

	//�����Ͱ� �ƴҰ��(��尡 ����������) ������ư ��Ȱ��
    CGuild* pGuild = g_pPlayer->GetGuild();
	if( pGuild )	
		m_bMaster = pGuild->IsMaster( g_pPlayer->m_idPlayer );		//GetId()�� �ƴ�!!!!!!!!!!!!!!!!!!
	
	if( !m_bMaster )
	{
		CWndButton* pWndBtnOK = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		if( pWndBtnOK )
			pWndBtnOK->EnableWindow( FALSE );
	}

	pWndListBox->AddString( "Updating list..." );
	g_DPlay.SendGuildHouseTenderMainWnd( WI_GUILDHOUSE_MIDDLE, pMoverTarget->GetIndex() );
}

void CWndGuildHouseBid::OnDraw( C2DRender* p2DRender )
{
	if( m_bMaster && m_bWaitResult )
	{
		CWndButton* pWndBtnOK = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
		if( !pWndBtnOK )
			return;

		DWORD dwDts = g_tmCurrent - m_dwWaitTime;

//		CRect rect = pWndBtnOK->GetWindowRect();
//		CString str;
//		str.Format( "%d Sec...", dwDts / 1000 );
//		p2DRender->TextOut( rect.left + 2, rect.bottom - 2, str, 0xffff0000 );

		if( dwDts > 5000 ) // �����κ��� ������� ������̰� , 5�ʰ� �������� 
		{
			m_dwWaitTime = 0;
			m_bWaitResult = FALSE;
		
			CWndButton* pWndBtnOK = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
			if( pWndBtnOK )
				pWndBtnOK->EnableWindow( TRUE );

		}
	}

}

BOOL CWndGuildHouseBid::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )
{
	switch( nID )
	{
	case WIDC_LISTBOX1:	//�Ͽ콺����Ʈ�� �ϳ��� ���õƴ�.
		RequestCurrHouseInfo( );		
		break;

	case WIDC_BUTTON1: // �����ϱ�
		{
			CWndEdit* pEdit1 = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );
			CWndEdit* pEdit2 = (CWndEdit*) GetDlgItem( WIDC_EDIT2 );
			CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	
			int nCurSel = pWndListBox->GetCurSel( );
			if( nCurSel < 0 || nCurSel >= (int)_cBidDatas.size() )
				break;

			GHBidData& kData = _cBidDatas[ nCurSel ];
	
			__int64 penya = 0, penrin = 0;
			CString str1 =  pEdit1->GetString( );
			CString str2 = pEdit2->GetString( );

			if( str1.GetLength() > 10 || str2.GetLength() > 10 )
			{
				g_WndMng.OpenMessageBox( "�߸��� �ݾ��Դϴ�. �ٽ��Է��ϼ��� ID�ʿ�" );
				ResetInputMoneyWindows( );
				break;
			}
			
			penya = _atoi64( str1.GetBuffer(0) );
			penrin = _atoi64( str2.GetBuffer(0) );

		//	sscanf( str1.GetBuffer(0), "%d", &penya );
		//	sscanf( str2.GetBuffer(0), "%d", &penrin );

			if( penya < 0 || penrin < 0 )
			{
				g_WndMng.OpenMessageBox( "�߸��� �ݾ��Դϴ�. �ٽ��Է��ϼ��� ID�ʿ�" );
				ResetInputMoneyWindows( );
				break;
			}

			__int64 totalPenya = ( penrin * 100000000 ) + penya;

			//��ķθ� 21���� �ʰ��Ҽ�����. �� �渰���� �ʰ����� 
			if( penya > 0x7d2b7500 )
			{
				g_WndMng.OpenMessageBox( "21�� penya�� �ʰ��Ҽ� �����ϴ�, penrin�� �̿��ϼ��� ID�ʿ�" );
				ResetInputMoneyWindows( );
				break;
			}

			if( totalPenya <= (__int64)kData._nBidMinPenya )
			{
				g_WndMng.OpenMessageBox( "�ּ������ݺ��� ū �ݾ��� �Է��ؾ� �մϴ� ID�ʿ�" );
				ResetInputMoneyWindows( );
				break;
			}

//			if( penrin > 0 )
//			{
//				CString temp;
//				temp.Format( " (%d penrin, %d penya) Are you sure? ", penrin, penya );
//				if( AfxMessageBox( (LPCTSTR)temp, MB_OKCANCEL ) == IDCANCEL )
//				{
//					ResetInputMoneyWindows( );
//					break;
//				}
//			}
									
			g_DPlay.SendGuildHouseTenderJoin( kData._id, (int)penrin, (int)penya );

			// �ߺ� Ŭ�� ���� .. ������ �޾Ƽ� Ǯ������.
			SetEnableWindow_Apply( FALSE, TRUE );
		}

		break;
		
	case WIDC_BUTTON2:	// �ݱ�
		Destroy();
		break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult );
}

void CWndGuildHouseBid::SetEnableWindow_Apply( BOOL bEnable, BOOL bWait )
{
	if( !m_bMaster )
		return;

	CWndButton* pWndBtnOK = (CWndButton*)GetDlgItem(WIDC_BUTTON1);
	if( pWndBtnOK )
		pWndBtnOK->EnableWindow( bEnable );

	if( bWait )
	{
		m_bWaitResult = TRUE;
		m_dwWaitTime = g_tmCurrent; 
	}
}

void CWndGuildHouseBid::ResetInputMoneyWindows( )
{
	CWndEdit* pEdit1 = (CWndEdit*) GetDlgItem( WIDC_EDIT1 );
	CWndEdit* pEdit2 = (CWndEdit*) GetDlgItem( WIDC_EDIT2 );

	pEdit1->SetString( "" );
	pEdit2->SetString( "" );
}

void CWndGuildHouseBid::RequestCurrHouseInfo( )
{
	//���� ���õǾ��� �Ͽ콺�����ؼ� �����׸� ��û
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	int nCurSel = pWndListBox->GetCurSel( );
	if( nCurSel < 0 || nCurSel >= (int)_cBidDatas.size() )
		return;

	GHBidData& kData = _cBidDatas[ nCurSel ];
	
	g_DPlay.SendGuildHouseTenderInfoWnd( kData._id );

	CWndListBox* pWndListBox2 = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	if( pWndListBox2 )
	{
		pWndListBox2->ResetContent( );
		pWndListBox2->AddString( "Updating list..." );
	}
}

GHBidData* CWndGuildHouseBid::FindData( OBJID houseID )
{
	for( GHBidDataIter iter = _cBidDatas.begin(); iter != _cBidDatas.end(); ++iter )
	{
		GHBidData& kData = *iter;
		if( kData._id == houseID )
			return &kData;
	}

	return NULL;
}

void CWndGuildHouseBid::UpdateData_HouseList( OBJID houseID, const char* szName )
{
	//if cannot find, create new data
	GHBidData* pData = FindData( houseID );
	if( pData )
	{
		pData->_name = CString( szName );
	}
	else 
	{
		GHBidData kNewBid;
		kNewBid._id = houseID;
		kNewBid._name = CString( szName );

		_cBidDatas.push_back( kNewBid );
	}
}
	
void CWndGuildHouseBid::UpdateData_HouseInfo( OBJID houseID, const int nMinPenya, __int64 n64TenderPenya, vector< DWORD >& guildIDs )
{
	GHBidData* pData = FindData( houseID );
	if( pData )
	{
		pData->_nBidMinPenya = nMinPenya;
		pData->_cGuildList = guildIDs;
	}

	CGuild* pGuild = g_pPlayer->GetGuild( );
	if( pGuild )
	{
		vector<DWORD>::iterator finder = std::find( guildIDs.begin(), guildIDs.end(), pGuild->GetGuildId() );
		if( finder != guildIDs.end() )
		{
			m_n64TenderPenya = n64TenderPenya;
			assert( m_n64TenderPenya > 0 );
		}
	}
}

void CWndGuildHouseBid::RefreshWnd_HouseList( )
{
	//�Ͽ콺 ����Ʈ ����, ��������â�� �ʱ�ȭ
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	pWndList->ResetContent( );

	CString str;
	int seq = 1;
	for( GHBidDataIter iter = _cBidDatas.begin(); iter != _cBidDatas.end(); ++iter )
	{
		GHBidData& kData = *iter;
		str.Format( "%d. %s", seq, kData._name.GetBuffer(0) );

		pWndList->AddString( (LPCTSTR) str );

		++seq;
	}

	CWndListBox* pWndGuildList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	if( !pWndGuildList )
		return;

	pWndGuildList->ResetContent( );
	
	CWndStatic* pWndPenya = (CWndStatic*)GetDlgItem(WIDC_STATIC7);
	if( !pWndPenya )
		return;

	pWndPenya->SetTitle( "0" );

	//�Է�â �ʱ�ȭ
	CWndEdit* pWndEdit = (CWndEdit *)GetDlgItem( WIDC_EDIT1 );
	if( !pWndEdit )
		return;
	
	pWndEdit->Empty();
}

void CWndGuildHouseBid::RefreshWnd_HouseInfo( )
{
	//�ش� �Ͽ콺�� �������� ����
	CWndListBox* pWndList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	if( !pWndList )
		return;

	CWndListBox* pWndGuildList = (CWndListBox*)GetDlgItem( WIDC_LISTBOX2 );
	if( !pWndGuildList )
		return;

	size_t nSelected = pWndList->GetCurSel();
	assert( nSelected < _cBidDatas.size() );

	GHBidData& kData = _cBidDatas[ nSelected ];

	//�����â ����
	pWndGuildList->ResetContent( );
	CString str;
	char buffer[ 64 ] = {0,};

	if( kData._cGuildList.empty() )
	{
		//���� ������� ����
		pWndGuildList->AddString( "������尡 �����ϴ� ���ID �ʿ�" );
	}
	else
	{
		int seq = 1;
		char* szGuildName = NULL;
		for( vector<DWORD>::iterator iter = kData._cGuildList.begin(); iter != kData._cGuildList.end(); ++iter, ++seq )
		{
			DWORD id = *iter;
			CGuild* pGuild = g_GuildMng.GetGuild( id );
			if( !pGuild )
			{
				strcpy( szGuildName, "unknown guild" );
			//	pWndGuildList->AddString( "unknown guild" );
			//	continue;
			}
			else
			{
				szGuildName = pGuild->m_szGuild;

				//������ ǥ�� ( ������ ������ > 0 )
				if( g_pPlayer->GetGuild( ) == pGuild && 0 < m_n64TenderPenya )		
					_i64toa( m_n64TenderPenya, buffer, 10 );		//arg3 : ���
				else strcpy( buffer, "?" );
			}

			str.Format( "%d. %s   %s peyna", seq, szGuildName, buffer );
			pWndGuildList->AddString( (LPCTSTR)str );
		}
	}

	//�ּ���� ����
	CWndStatic* pWndPenya = (CWndStatic*)GetDlgItem(WIDC_STATIC7);
	if( !pWndPenya )
		return;

	str.Empty();
	str.Format( "%d", kData._nBidMinPenya );
//	MakeMoneyStyle( str );
	pWndPenya->SetTitle( str );

	//�Է�â �ʱ�ȭ
	ResetInputMoneyWindows();
}

void CWndGuildHouseBid::MakeMoneyStyle( OUT CString& str )
{
	//õ�������� ','�߰��ؼ� �����( 100000000 -> 100,000,000 )
	CString rst;
	int len = str.GetLength( );
	if( len < 4 )
		return;

	for( int i = 0; i < len; ++i )
	{
		if( i > 0 && i != (len-1) && i % 3 == 0 )		// i�� 3�� ����̸�
			rst.Insert( rst.GetLength(), ',' );
	
		rst.Insert( rst.GetLength(), str.GetAt( i ) );
	}

	str = rst;
}

#endif //__GUILD_HOUSE_MIDDLE

#endif // __CLIENT