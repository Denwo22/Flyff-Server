#ifndef __WNDGUILDWARREQUEST__H
#define __WNDGUILDWARREQUEST__H

class CWndGuildWarRequest : public CWndNeuz 
{ 
	DWORD m_idEnemyGuild;
	char m_szMaster[MAX_PLAYER];
public: 
	CWndGuildWarRequest(); 
	~CWndGuildWarRequest(); 

	// Initialize�ϱ����� �������̵�� ���渶 �̸��� �޾Ƶ�.
	void	Set( DWORD idEnemyGuild, LPCTSTR szMaster ) 
	{ 
		m_idEnemyGuild = idEnemyGuild; 
		strcpy( m_szMaster, szMaster );
	}

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
