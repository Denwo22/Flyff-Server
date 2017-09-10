
//Author : gmpbigsun
//Date : 2009_11_16
// Tab winodw ( of guild window )

#pragma once 

#if __VER >= 15 // __GUILD_HOUSE

class CWndGuildTabPower : public CWndNeuz 
{ 
	// ��������쿡�� �߰��Ǵ� �� ������ ( ��� �Ͽ콺�� ���� ���� ���� )
public: 
	CWndGuildTabPower(); 
	virtual ~CWndGuildTabPower(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

	void UpdateData();
	void SetData( DWORD dwPower[] );
	void EnableButton(BOOL bEnable);

protected:
	DWORD m_adwPower[ MAX_GM_LEVEL ];
	BOOL m_bChanedCheckBox;

}; 

#endif //__GUILD_HOUSE