#ifndef __WNDGUILD_TAB_MEMBER__H
#define __WNDGUILD_TAB_MEMBER__H

struct MEMBERLIST
{
	DWORD	idPlayer;	// �÷��̾� ���̵�.
	int nMemberLv;		// ȣĪ
	int nJob;			// ���� 
	int nSex;			// ��
	int	nLevel;			// ĳ������
	BOOL bIsOnLine;     // �¶���, ��������
	int		nGiveGold;					// ��忡 ����� ���
	DWORD	dwGivePxpCount;				// ��忡 ����� PXPȽ��( ��ų ����ġ )
	short	nWin;							// ������ �̰���?
	short	nLose;						// ������ �����?
	char szName[ MAX_NAME ];	// �̸�.
	char szAlias[ MAX_GM_ALIAS ];	// ��Ī
	int nClass;				// ���
};

#define MAX_MEMBER	128

class CWndGuildTabMember : public CWndNeuz 
{ 
//	CWndListCtrl *m_pListCtrl;
	int		m_nCurrentList;			// ��µ� �������Ʈ�� ���� �ε���.
	int		m_nSelect;

	int		m_nMxOld, m_nMyOld;			// ���� ��ǥ.
	CWndMenu	m_wndMenu;			// �˾� �޴�.
	CWndScrollBar m_wndScrollBar;

#if __VER >= 12 // __CSC_VER12_2
	BOOL m_bSortbyMemberLevel;
	BOOL m_bSortbyLevel;
	BOOL m_bSortbyJob;
	BOOL m_bSortbyName;
	BOOL m_bSortbyAlias;
#endif //__CSC_VER12_2

public: 
	void UpdateData();
	int		m_nMax;
	MEMBERLIST	m_aList[ MAX_MEMBER ];

	CWndGuildTabMember(); 
	~CWndGuildTabMember(); 

	CRect	m_rectParent;			// ���â�� �»�� ��ǥ.
	
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual	void OnRButtonUp( UINT nFlags, CPoint point );
	virtual void OnRButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseMove(UINT nFlags, CPoint point );
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void OnMouseWndSurface( CPoint point );

#if __VER >= 12 // __CSC_VER12_2
	void SortbyMemberLevel();
	void SortbyJob();
	void SortbyLevel();
	void SortbyName();
	void SortbyAlias();
#endif //__CSC_VER12_2		
}; 

#endif