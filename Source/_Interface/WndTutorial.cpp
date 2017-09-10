#include "stdafx.h"
#include "resData.h"
#include "WndTutorial.h"


#if __VER >= 12 // __MOD_TUTORIAL
/****************************************************
  WndId : APP_TUTORIAL_VIEW - Ʃ�丮��
  CtrlId : WIDC_TREE1 - 
  CtrlId : WIDC_TEXT1 - 
  CtrlId : WIDC_TEXT2 - 
****************************************************/

CWndTutorial::CWndTutorial() 
{ 
	m_strKeyword.Empty();
} 
CWndTutorial::~CWndTutorial() 
{ 
	m_strKeyword.Empty();
} 
void CWndTutorial::OnDraw( C2DRender* p2DRender ) 
{ 
	if(m_strKeyword.IsEmpty()) return;

	DWORD dwLeft = m_rectClient.Width() * 50 / 100;
	CRect rect;

	rect.SetRect( dwLeft + 5, 5, m_rectClient.Width() - 5, 23 );

	p2DRender->TextOut( rect.left + 12, rect.top + 10, m_strKeyword, 0xff000000 );

} 
void CWndTutorial::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	DWORD dwRight = m_rectClient.Width() * 50 / 100;

	LoadTutorial( MakePath( DIR_CLIENT,  _T( "tutorial.inc" ) ) );

	// ������ �߾����� �ű�� �κ�.
	// CRect rectRoot = m_pWndRoot->GetLayoutRect();
	// CRect rectWindow = GetWindowRect();
	// CPoint point( rectRoot.right - rectWindow.Width(), 110 );
	// Move( point );
	MoveParentCenter();
} 
// ó�� �� �Լ��� �θ��� ������ ������.
BOOL CWndTutorial::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_TUTORIAL_VIEW, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
  ���� ������ ���� ��� 
BOOL CWndTutorial::Initialize( CWndBase* pWndParent, DWORD dwWndId ) 
{ 
	CRect rectWindow = m_pWndRoot->GetWindowRect(); 
	CRect rect( 50 ,50, 300, 300 ); 
	SetTitle( _T( "title" ) ); 
	return CWndNeuz::Create( WBS_THICKFRAME | WBS_MOVE | WBS_SOUND | WBS_CAPTION, rect, pWndParent, dwWndId ); 
} 
*/
BOOL CWndTutorial::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndTutorial::OnSize( UINT nType, int cx, int cy ) 
{ 
	DWORD dwRight = m_rectClient.Width() * 50 / 100;
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndTutorial::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndTutorial::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndTutorial::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	switch(nID)
	{
		case WIDC_LIST1: // view ctrl
			{
				CWndListBox* pWndListBox = (CWndListBox*)GetDlgItem( WIDC_LIST1 );
				int sel = pWndListBox->GetCurSel();
				map<int, TUTORIAL_STRING>::iterator iter = m_mapTutorial.find(pWndListBox->GetCurSel());
				m_strKeyword = iter->second.strTitle;
				CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT2 );
				pWndText->m_string.Init( m_pFont, &pWndText->GetClientRect() );
				pWndText->m_string.SetString("");
				pWndText->m_string.AddParsingString(LPCTSTR(iter->second.strContents));
				pWndText->UpdateScrollBar();
			}
			break;
	}

	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

BOOL CWndTutorial::LoadTutorial(LPCTSTR lpszFileName)
{
	
	CScript			s;
	int				nIndex = 0;
	CString			strKeyword;
	TUTORIAL_STRING temp;

	if( s.Load( lpszFileName ) == FALSE )
		return FALSE;
	s.tok = 0;
	m_mapTutorial.clear();

	// {
	//	����(���ڿ�)
	//	����(���ڿ�)
	// }
	while( s.tok != FINISHED )
	{
		s.GetToken();	// {
		s.GetToken();	//
		temp.strTitle		= s.token;
		s.GetToken();
		temp.strContents	= s.token;
		m_mapTutorial.insert(map<int, TUTORIAL_STRING>::value_type(nIndex, temp));
		s.GetToken();	// }

		// �Ϸ��� ���������� �߰��Ѵ�
		if(g_Option.m_nTutorialLv >= nIndex + 1)
		{
			CWndListBox* pWndListBox	= (CWndListBox*)GetDlgItem( WIDC_LIST1 );
			pWndListBox->AddString( temp.strTitle );
		}
		++nIndex;
	}
	return TRUE;

}

BOOL CWndTutorial::AddToList(int nIndex)
{
	return false;
}

#endif