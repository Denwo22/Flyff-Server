#include "StdAfx.h"
#include "RichEdit.h"
#include "MySplashWnd.h"

CMySplashWnd::CMySplashWnd( UINT nBitmapID, UINT nDuration /*= 2500*/ )
{
	m_nBitmapID		= nBitmapID;
	m_nDuration		= nDuration;
}

BEGIN_MESSAGE_MAP( CMySplashWnd, CWnd )
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CMySplashWnd::Create()
{
	if( GetBitmapAndPalette( m_nBitmapID, m_bitmap, m_pal ) == FALSE )
	{
		TRACE1( "Could not load bitmap resource - %d\n", m_nBitmapID );
		return FALSE;
	}

	BITMAP bm;
	m_bitmap.GetObject( sizeof(BITMAP), &bm );

	// First create an invisible window
	m_wndInvisible.CreateEx( WS_EX_TOPMOST, AfxRegisterWndClass( CS_CLASSDC ), _T(""), WS_POPUP, 0, 0, bm.bmWidth, bm.bmHeight, NULL, NULL );

	// Create the splash window with invisible parent as parent
	BOOL bRetVal	= CWnd::CreateEx( WS_EX_TOPMOST, AfxRegisterWndClass( CS_CLASSDC ), _T(""), WS_POPUP, 0, 0, bm.bmWidth, bm.bmHeight, m_wndInvisible.m_hWnd, NULL );

	CenterWindow();
	ShowWindow( SW_SHOW );
	UpdateWindow();

	// Create the timer.
	m_nTimerID	= SetTimer( 1, m_nDuration, NULL );
	ASSERT( m_nTimerID );

	return bRetVal;
}

BOOL CMySplashWnd::GetBitmapAndPalette( UINT nIDResource, CBitmap & bitmap, CPalette & pal )
{
	LPCTSTR lpszResourceName	= (LPCTSTR)nIDResource;

	HBITMAP hBmp	= (HBITMAP)::LoadImage( AfxGetInstanceHandle(), lpszResourceName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );

	if( hBmp == NULL)
		return FALSE;

	bitmap.Attach( hBmp );

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo	= ds.dsBmih;
	bitmap.GetObject( sizeof(ds), &ds );

	int nColors		= bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if color > 256.
	CClientDC dc( NULL );	// Desktop DC
	if( nColors > 256 )
	{
		pal.CreateHalftonePalette( &dc );
	}
	else
	{
		// Create the palette
		RGBQUAD* pRGB	= new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC( &dc );
		memDC.SelectObject( &bitmap );
		GetDIBColorTable( memDC, 0, nColors, pRGB );

		UINT nSize	= sizeof(LOGPALETTE) + ( sizeof(PALETTEENTRY) * nColors );
		LOGPALETTE* pLP		= (LOGPALETTE*)new BYTE[nSize];
		pLP->palVersion		= 0x300;
		pLP->palNumEntries	= nColors;
		for( int i = 0; i < nColors; i++ )
		{
			pLP->palPalEntry[i].peRed	= pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen	= pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue	= pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags		= 0;
		}

		pal.CreatePalette( pLP );

		SAFE_DELETE_ARRAY( pLP );
		SAFE_DELETE_ARRAY( pRGB );
	}
		
	return TRUE;
}

void CMySplashWnd::OnPaint()
{
	CPaintDC dc( this );	// device contect for painting

	// Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );

	CBitmap* pBmpOld	= memDC.SelectObject( &m_bitmap );

	// Select and realize the palette
	if( dc.GetDeviceCaps( RASTERCAPS ) & RC_PALETTE && m_pal.m_hObject != NULL )
	{
		dc.SelectPalette( &m_pal, FALSE );
		dc.RealizePalette();
	}

	// Window is same size as bitmap
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	dc.BitBlt( 0, 0, rcWnd.Width(), rcWnd.Height(), &memDC, 0, 0, SRCCOPY );

	// Restore bitmap in memDC
	memDC.SelectObject( pBmpOld );

	// Do not call CWnd::OnPaint() for painting message
}

void CMySplashWnd::OnTimer( UINT nIDEvent )
{
	if( m_nTimerID == nIDEvent )
	{
		// Destroy the timer and splash window
		KillTimer( m_nTimerID );
		m_wndInvisible.DestroyWindow();
		safe_delete( this );
		return;
	}
	
	CWnd::OnTimer( nIDEvent );
}

BOOL CMySplashWnd::PreTranslateMessage( MSG* pMsg )
{
	ASSERT( pMsg != NULL );

	if( pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN ||
		pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN )
	{
		KillTimer( m_nTimerID );
		m_wndInvisible.DestroyWindow();
		safe_delete( this );
		return 1;
	}
	
	return CWnd::PreTranslateMessage( pMsg );
}