// WndArcane.h: interface for the CWndNeuz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)
#define AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWndShortcut : public CWndButton
{
public:
	CWndShortcut();
	~CWndShortcut();

	//BOOL Create(LPCTSTR lpszCaption,DWORD dwStyle,const RECT& rect,CWndBase* pParentWnd,UINT nID,CSprPack* pSprPack,int nSprIdx,int nColorTable = 0);
	//BOOL Create(LPCTSTR lpszCaption,DWORD dwStyle,const RECT& rect,CWndBase* pParentWnd,UINT nID);// { return Create(lpszCaption,dwStyle,rect,pParentWnd,nID,NULL,0); }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	BOOL Process();
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

/*
	void SetMenu(CWndMenu* pWndMenu);

	// Attributes
	//BOOL IsButtonStyle( DWORD dwStyle ) { return ( m_dwButtonStyle & dwStyle ) ? TRUE : FALSE; }
	void SetPushTime(int nTime); // 버튼을 누르고 nTime이후에 계속 OnCommand를 패어런트에게 보낸다.
	UINT GetState() const; // Retrieves the check state, highlight state, and focus state of a button control.
	void SetState(BOOL bHighlight); // Sets the highlighting state of a button control
	int  GetCheck() const; // Retrieves the check state of a button control.
	void SetCheck(int nCheck); // Sets the check state of a button control.
	//UINT GetButtonStyle() const; // Retrieves information about the button control style.
	//void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE); // Changes the style of a button.
	void SetString(CString strSndEffect); 
	BOOL IsHighLight() { return m_bHighLight; }
	void SetFontColor     (DWORD dwColor) { m_nFontColor      = (DWORD)dwColor; }
	void SetPushColor     (DWORD dwColor) { m_nPushColor      = (DWORD)dwColor; }
	void SetDisableColor  (DWORD dwColor) { m_nDisableColor   = (DWORD)dwColor; }
	void SetHighLightColor(DWORD dwColor) { m_nHighlightColor = (DWORD)dwColor; }
	DWORD GetFontColor     () { return m_nFontColor     ; }
	DWORD GetPushColor     () { return m_nPushColor     ; }
	DWORD GetDisableColor  () { return m_nDisableColor  ; }
	DWORD GetHighLightColor() { return m_nHighlightColor; }
	void SetWndExecute(CWndBase* pWndBase);
	CWndBase* GetWndExecute() { return m_pWndExecute; }
	void SetPushPoint(int x,int y) { m_ptPush = CPoint(x,y); }
	*/
};
class CWndQuickList : public CWndNeuz 
{ 
public: 
	CWndQuickList(); 
	~CWndQuickList(); 

	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

//////////////////////////////////////////////////////////////////////////////////////
// 작업 윈도 
// 메뉴 버튼, 명령 아이콘, 단축 아이콘, 활성화 기능 등등
//
class CWndTaskBar : public CWndNeuz
{
	DWORD m_dwHighAlpha;

public:
	BOOL		   IsShortcut( LPSHORTCUT lpShortcut, DWORD dwShortcut, DWORD dwId );
	CWndMenu       m_menuShortcut;
	CWndQuickList* m_pWndQuickList;
	CTexturePack   m_texPack;
	CWndButton     m_wndMenu;
	CWndButton     m_wndQuickList;
	CWndButton     m_wndQuickListUp;
	CWndButton     m_wndQuickListDn;
	SHORTCUT       m_aSlotApplet[ MAX_SLOT_APPLET ]; // 1 ~ 20
	SHORTCUT       m_aSlotItem  [ MAX_SLOT_ITEM_COUNT ][ MAX_SLOT_ITEM ]; // 1 ~ 0(10)
	SHORTCUT       m_aSlotQueue [ MAX_SLOT_QUEUE  ];
	LPSHORTCUT     m_paSlotItem;
	LPSHORTCUT     m_pSelectShortcut;
	SHORTCUT       m_aSlotSkill;
	CTexture*      m_pTexture;
	int            m_nMaxSlotApplet;
	BOOL           m_bStartTimeBar;
	int            m_nUsedSkillQueue;
	int            m_nPosition;
	int            m_nSkillBar;
	int            m_nCurQueueNum;
	int            m_nCurQueue;
	CTimer         m_timer;
	int			   m_nExecute;		// 0: 실행중이지 않음 1:실행대기중(스킬쓰러 달려가는중) 2:실행중.
	OBJID		   m_idTarget;		// 스킬사용대상.
	int            m_nSlotIndex;
	int		       m_nActionPoint;		// 액션 포인트 - 시리얼라이즈 대상.
#if __VER >= 12 // __LORD
	void RenderLordCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender );
#endif
	void RenderCollTime( CPoint pt, DWORD dwSkillId, C2DRender* p2DRender );
	void UpdateItem();
	void PutTooTip( LPSHORTCUT pShortcut, CPoint point, CRect* pRect );
	void SetItemSlot( int nSlot );
	BOOL CheckAddSkill( int nSkillStyleSrc, int nQueueDest );
	BOOL UseSkillQueue( CCtrl* pTargetObj );
	LPSKILL GetCurrentSkillQueue();
	BOOL SetSkillQueue( int nIndex, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL );
	BOOL SetShortcut( int nIndex, DWORD dwShortcut, DWORD dwType, DWORD dwId, CTexture* pTexture = NULL, int nWhere = 0 );
	void OnEndSkill( void );
	void OnCancelSkill( void );
	LPSHORTCUT Select( CPoint point );
	void RemoveSkillQueue( int nIndex, BOOL bSend = TRUE );
	BOOL RemoveDeleteObj();
	void InitTaskBar();

	CWndTaskBar();
	virtual ~CWndTaskBar();
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual void OnDraw(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point = 0 );
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual	void PaintFrame( C2DRender* p2DRender );
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual BOOL Process();
	virtual void OnMouseWndSurface( CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual	void OnDestroyChildWnd( CWndBase* pWndChild );
	void	Serialize( CAr & ar );
	void	SetTaskBarTexture( LPSHORTCUT pShortcut );
	
	HRESULT	RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
protected:
	void RenderOutLineLamp(int x, int y, int num, DWORD size);
};
//////////////////////////////////////////////////////////////////////////////////////
// 태스크바의 매뉴 
//
class CWndTaskMenu : public CWndMenu //Neuz
{
public:
	CWndTaskMenu();
	virtual ~CWndTaskMenu();
	void SetTexture( CWndButton* pWndButton );
//	virtual CItem* GetFocusItem() { return NULL; }
	virtual CWndButton* AppendMenu( CWndMenu* pWndMenu, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem );
	virtual void OnDraw(C2DRender* p2DRender);
	virtual void PaintFrame(C2DRender* p2DRender);
	virtual BOOL OnEraseBkgnd(C2DRender* p2DRender);
	virtual	void OnInitialUpdate();
	virtual BOOL Initialize(CWndBase* pWndParent = NULL,DWORD dwWndId = 0);
	// message
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase = NULL );
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnChildNotify(UINT message,UINT nID,LRESULT* pLResult);
	virtual BOOL Process();
	virtual void OnKillFocus(CWndBase* pNewWnd);
private:
	CWndMenu* m_pMenu1;
	CWndMenu* m_pMenu2;
	CWndMenu* m_pMenu3;
	CWndMenu* m_pMenu4;
	CWndMenu* m_pMenu5;
	CWndMenu* m_pMenu6;
	CWndMenu* m_pMenu7;
};
 
#endif // !defined(AFX_WNDTASKBAR_H__A93F3186_63D6_43C1_956F_EC8691E0C7D9__INCLUDED_)

