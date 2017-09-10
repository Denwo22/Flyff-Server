#ifndef __WNDSELECTCH__H
#define __WNDSELECTCH__H

#ifdef __AZRIA_1023
class CWndSelectCh : public CWndNeuz 
{ 
private:
	int m_nItemId;
	int m_nChCount;

public: 
	CWndSelectCh(int nItemId, int nChCount); 
	~CWndSelectCh(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 
#endif

#endif
