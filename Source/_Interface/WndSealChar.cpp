#include "stdafx.h"
#include "defineText.h"
#include "AppDefine.h"
#include "WndSealChar.h"

#include "DPClient.h"
extern	CDPClient	g_DPlay;


/****************************************************
 

****************************************************/

CWndSealChar::CWndSealChar() 
{ 

} 
CWndSealChar::~CWndSealChar() 
{ 
} 
void CWndSealChar::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndSealChar::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;


	strTemp.Format( _T( prj.GetText(TID_DIAG_SEAL_CHAR_TEXT01) ) ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	
	pWndText->SetString( strTemp );

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
BOOL CWndSealChar::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_SEAL_CHAR, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
 
*/
BOOL CWndSealChar::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndSealChar::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndSealChar::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndSealChar::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndSealChar::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = FALSE;
	if( nID == WIDC_OK ) 
	{
		g_DPlay.SendSealChar(  );		// �ɸ����ν�û
		bResult = TRUE;
	}
	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 


CWndSealCharSelect::CWndSealCharSelect() 
{ 
	memset( m_szSrc1, 0, sizeof(m_szSrc1) );
	m_idSrc1 = NULL_ID;
	memset( m_szSrc2, 0, sizeof(m_szSrc2) );
	m_idSrc2 = NULL_ID;
	m_lPlayerSlot1 = -1;
	m_lPlayerSlot2 = -1;
	m_sCount = 0;
} 
CWndSealCharSelect::~CWndSealCharSelect() 
{ 
} 
void CWndSealCharSelect::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndSealCharSelect::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp,strName1,strName2;

	strTemp.Format( _T( prj.GetText(TID_DIAG_SEAL_CHAR_SELECT_TEXT01) ) ); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );

	CWndButton* pWndButton[ 2 ];
	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO2 );

	pWndButton[ 0 ]->SetGroup( TRUE );
	pWndButton[ 0 ]->SetCheck( TRUE );



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
BOOL CWndSealCharSelect::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_SEAL_CHAR_SELECT, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
 
*/
BOOL CWndSealCharSelect::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndSealCharSelect::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndSealCharSelect::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndSealCharSelect::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndSealCharSelect::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = FALSE;
	if( nID == WIDC_OK ) 
	{
		UINT nRadios[2] = { WIDC_RADIO1, WIDC_RADIO2 };
		int i = NULL;
		for( ; i<2; ++i )
		{
			CWndButton* pButton = (CWndButton* )GetDlgItem( nRadios[i] );
			if( pButton->GetCheck() )
				break;
		}

		if( (( i == 0 ) && (m_idSrc1 < 1 ) ) || (( i == 1 ) && (m_idSrc2 < 1 ) ) )
			bResult = FALSE;
		else
		{
			CWndSealCharSend* pWndCWndSealCharSend = (CWndSealCharSend*)g_WndMng.GetWndBase( APP_SEAL_CHAR_SEND );
			if( !pWndCWndSealCharSend )
			{
				pWndCWndSealCharSend = new CWndSealCharSend;
				pWndCWndSealCharSend->Initialize( &g_WndMng );
			}

			if(i == 0)
				pWndCWndSealCharSend->SetData(m_idSrc1,m_szSrc1);
			else if(i == 1)
				pWndCWndSealCharSend->SetData(m_idSrc2,m_szSrc2);

	//		g_DPlay.SendSealChar(  );		// �ɸ����ν�û
			bResult = TRUE;
		}
	}
	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndSealCharSelect::SetData( short sCount,LONG lPlayerSolt1,LONG lPlayerSolt2,u_long uPlayerID1,u_long uPlayerID2, char* szName1, char* szName2 )
{
	strcpy( m_szSrc1, szName1 );
	strcpy( m_szSrc2, szName2 );

	m_idSrc1 = uPlayerID1;
	m_idSrc2 = uPlayerID2;
	m_lPlayerSlot1 = lPlayerSolt1;
	m_lPlayerSlot2 = lPlayerSolt2;
	m_sCount = sCount;

	CWndButton* pWndButton[ 2 ];
	
	pWndButton[ 0 ] = (CWndButton*)GetDlgItem( WIDC_RADIO1 );
	pWndButton[ 1 ] = (CWndButton*)GetDlgItem( WIDC_RADIO2 );

	pWndButton[ 0 ]->SetTitle( m_szSrc1 );
	pWndButton[ 1 ]->SetTitle( m_szSrc2 );
}


CWndSealCharSend::CWndSealCharSend() 
{ 
	memset( m_szSrc1, 0, sizeof(m_szSrc1) );
	m_idSrc1 = NULL_ID;
} 
CWndSealCharSend::~CWndSealCharSend() 
{ 
} 
void CWndSealCharSend::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndSealCharSend::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;

	strTemp.Format( _T( prj.GetText(TID_DIAG_SEAL_CHAR_SEND_TEXT01) ), m_szSrc1); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );


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
BOOL CWndSealCharSend::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_SEAL_CHAR_SEND, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
 
*/
BOOL CWndSealCharSend::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndSealCharSend::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndSealCharSend::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndSealCharSend::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndSealCharSend::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = TRUE;
	if( nID == WIDC_YES ) 
	{

		g_DPlay.SendSealCharConm( m_idSrc1 );		// �ɸ����ν�û
		bResult = TRUE;
//		g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_DIAG_0023), MB_OK, TRUE );	//�����κ��� ������ �����Ǿ����ϴ�.		  

	}
	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndSealCharSend::SetData( u_long uPlayerID1, char* szName1 )
{
	strcpy( m_szSrc1, szName1 );

	m_idSrc1 = uPlayerID1;
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );

	CString strTemp;

	strTemp.Format( _T( prj.GetText(TID_DIAG_SEAL_CHAR_SEND_TEXT01) ), m_szSrc1); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );

}



CWndSealCharSet::CWndSealCharSet() 
{ 
	memset( m_szSrc1, 0, sizeof(m_szSrc1) );
	m_idSrc1 = NULL_ID;
} 
CWndSealCharSet::~CWndSealCharSet() 
{ 
} 
void CWndSealCharSet::OnDraw( C2DRender* p2DRender ) 
{ 
} 
void CWndSealCharSet::OnInitialUpdate() 
{ 
	CWndNeuz::OnInitialUpdate(); 
	// ���⿡ �ڵ��ϼ���
	
	CWndText* pWndText = (CWndText*)GetDlgItem( WIDC_TEXT1 );
	//	CWndStatic* pWndStatic;
	//	pWndStatic = (CWndStatic*)GetDlgItem( WIDC_STATIC1 );
	CString strTemp;

	strTemp.Format( _T( prj.GetText(TID_DIAG_SEAL_CHAR_SET_TEXT01) ), m_szSrc1); // �޽��� �ٲٷ��� �̰� �ٲٽÿ�
	pWndText->SetString( strTemp );

	m_dwData	= 0;

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
BOOL CWndSealCharSet::Initialize( CWndBase* pWndParent, DWORD /*dwWndId*/ ) 
{ 
	// Daisy���� ������ ���ҽ��� ������ ����.
	return CWndNeuz::InitDialog( g_Neuz.GetSafeHwnd(), APP_SEAL_CHAR_SET, 0, CPoint( 0, 0 ), pWndParent );
} 
/*
 
*/
BOOL CWndSealCharSet::OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ) 
{ 
	return CWndNeuz::OnCommand( nID, dwMessage, pWndBase ); 
} 
void CWndSealCharSet::OnSize( UINT nType, int cx, int cy ) \
{ 
	CWndNeuz::OnSize( nType, cx, cy ); 
} 
void CWndSealCharSet::OnLButtonUp( UINT nFlags, CPoint point ) 
{ 
} 
void CWndSealCharSet::OnLButtonDown( UINT nFlags, CPoint point ) 
{ 
} 
BOOL CWndSealCharSet::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ) 
{ 
	BOOL bResult = TRUE;
	if( nID == WIDC_YES ) 
	{
		g_DPlay.SendSealCharSet(m_dwData);
	
//		g_WndMng.OpenMessageBoxUpper( prj.GetText(TID_DIAG_0023), MB_OK, TRUE );	//�����κ��� ������ �����Ǿ����ϴ�.		

	}
	// �ݵ��!!! TRUE�� �ؼ� �޸𸮿��� ������!!
	if( bResult )
		Destroy( /*TRUE*/ );
	return CWndNeuz::OnChildNotify( message, nID, pLResult ); 
} 

void CWndSealCharSet::SetData( DWORD dwId, WORD wReset )
{
	m_dwData	= dwId;

}