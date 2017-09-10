

#include "stdafx.h"
#include "resData.h"
#include "WndRoomList.h"
#include "defineText.h"
#include "playerdata.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if __VER >= 13 // __HOUSING
/****************************************************
  WndId : APP_MINIROOM_LIST - �̴Ϸ� ���
  CtrlId : WIDC_BUTTON1 - Button
  CtrlId : WIDC_BUTTON2 - Button
  CtrlId : WIDC_BUTTON3 - Button
  CtrlId : WIDC_LISTBOX1 - Listbox
  CtrlId : WIDC_TEXT1 - ���� ������ ������ �̴Ϸ��� ĳ���� ����Դϴ�.
****************************************************/

CWndRoomList::CWndRoomList() 
{ 
} 
CWndRoomList::~CWndRoomList() 
{ 
	
} 
void CWndRoomList::OnDraw( C2DRender* p2DRender ) 
{ 
} 

void CWndRoomList::Refresh()
{
	// ����Ʈ�� ���� ����
	CWndListBox*				pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	vector<DWORD>::iterator		iter		= CHousing::GetInstance()->m_vecVisitable.begin();
	int							nIndex		= 0;
	
	pWndListBox->ResetContent();
	
	for(;iter != CHousing::GetInstance()->m_vecVisitable.end(); ++iter)
	{
		PlayerData* pPlayerData	= CPlayerDataCenter::GetInstance()->GetPlayerData(*iter);
		if(pPlayerData)
		{
			CString	strName;
			strName.Format("%d. %s", nIndex + 1, pPlayerData->szPlayer);
			pWndListBox->AddString(strName);
			pWndListBox->SetItemData(nIndex, *iter);
			++nIndex;
		}
	}
}

void CWndRoomList::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	CHousing::GetInstance()->m_vecVisitable.clear();
	g_DPlay.SendHousingReqVisitableList();

	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	pWndListBox->m_nSelectColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	// ������ �߾����� �ű�� �κ�.
	CRect rectRoot = m_pWndRoot->GetLayoutRect();
	CRect rectWindow = GetWindowRect();
	CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndRoomList::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_MINIROOM_LIST, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndRoomList::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndRoomList::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndRoomList::OnSize( UINT nType, int cx, int cy ) 
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndRoomList::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndRoomList::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndRoomList::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LISTBOX1 );
	switch(nID)
	{
		case WIDC_BUTTON1:// ok ��ư
			if(pWndListBox->GetCurSel() >= 0)
			{
				
				DWORD dwID = pWndListBox->GetItemData(pWndListBox->GetCurSel());
				if(dwID >= 0)
					g_DPlay.SendHousingVisitRoom(dwID);
				Destroy();
			}
			break;

		case WIDC_BUTTON2:// refresh ��ư
			g_DPlay.SendHousingReqVisitableList();
			break;

		case WIDC_BUTTON3:// cancel ��ư
			Destroy();
			break;
	};

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

#endif // __HOUSING