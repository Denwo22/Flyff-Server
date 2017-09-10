#ifdef __EVE_MINIGAME

#ifndef __WNDMINIGAME__H
#define __WNDMINIGAME__H

class CWndKawiBawiBoGameWin;

class CWndKawiBawiBoGame : public CWndNeuz 
{
	enum MyPosition {KAWI, BAWI, BO};
public:
	int m_nWinningCount;			//���� ��
	int m_nWinningMaxCount;			//���� �ִ�ġ
	int m_nMyChoice;				//���� ������ ī�� (����, ����, ��)
	int m_nComChoice;				//Com�� ������ ī��
	int m_nCount;
	int m_nDelay;					//Com ���¸� ȸ����Ű�� Delay��
	int m_nResult;					//������ ���� ����� �� ����
	int m_nPrevResult;
	int m_nStatus;					//������ ���� (Delay���� ���� ���� ���)
	int m_nItemCount;
	int m_nNextItemCount;
	BOOL m_bCheckCoupon;			//���������� ������ �ʿ���ϴ��� Ȯ��
	
	DWORD m_dwRItemId;
	DWORD m_dwRNextItemId;

	CString m_strChoice[3];			//����, ����, �� Text
	
	CWndStatic*	m_pStWinningCount;	//���� ǥ��
	CWndStatic*	m_pStMyChoice;		//���� ������ ī�� ǥ��
	CWndStatic*	m_pStComChoice;		//Com�� ������ ī�� ǥ��
	
	CWndKawiBawiBoGameWin* m_pWndGameWin;
public: 
	CWndKawiBawiBoGame(); 
	virtual ~CWndKawiBawiBoGame();
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL Process ();

	void ReceiveResult(int nResult, int nItemCount, int nNextItemCount, DWORD dwItemId, DWORD dwNextItemId);
	void CreateWinningWnd();
	void RefreshInfo();
	void DisableAllBtn();
	void EnableAllBtn();
}; 

class CWndKawiBawiBoGameWin : public CWndNeuz 
{
public:
	CWndKawiBawiBoGame* m_pWndGame;

	int m_nItemCount;
	int m_nNextItemCount;
	DWORD m_dwItemID;
	DWORD m_dwNextItemID;
	
public: 
	CWndKawiBawiBoGameWin(); 
	virtual ~CWndKawiBawiBoGameWin(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual void OnMouseWndSurface( CPoint point );

	void SetInfo(int nItemCount, int nNextItemCount, DWORD dwItemId, DWORD dwNextItemId, int nWinningCount, int nWinningMaxCount);
}; 

class CWndKawiBawiBoGameConfirm : public CWndNeuz 
{	
public:
	CWndButton m_wndButton1;
	CWndButton m_wndButton2;
protected:
	CString m_strText;
	CWndText m_wndText;
	
public:
	CWndKawiBawiBoGameConfirm();
	~CWndKawiBawiBoGameConfirm();
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwWndId = 0 );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
}; 

class CWndFindWordGame : public CWndNeuz 
{
public:
	int m_nPublic;				//��Ʈ�� �־����� ������ ��ġ
	OBJID m_firstwordID;		//��Ʈ�� �־����� ù��° ����
	OBJID m_itemID[5];			//�� ������ Object ID
	CItemElem* m_pItemElem[5];	//������ ���� ����.
	int m_pStaticID[5];			//5���� WndID -> Picture (ondropicon ����� ����)
	int m_nSelectCtrl;			//���õ� Wnd ���.
	CWndText* m_pText;
	BOOL m_bGetFirst;			//��Ʈ�� �־����� ���ڸ� �޾Ҵ�.
	BOOL m_bStart;
	int m_nQuestionID;			// ���� ��ȣ
	
	CWndInventory* m_pWndInventory;
public: 
	CWndFindWordGame();
	virtual ~CWndFindWordGame(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL Process ();

	int HitTest( CPoint point );
	void SetFirstWord(int nPublic, char nChar);
	void SetWord(CItemElem* pItemElem);
	void SetDescription(CHAR* szChar);
	void ReceiveResult(int nResult, DWORD dwItemId, int nItemCount);
}; 

class CWndDiceGameTender;
class CWndDiceGameMessage;

class CWndDiceGame : public CWndNeuz 
{
	enum DiceNum {DICE_ONE, DICE_TWO, DICE_THREE, DICE_FOUR, DICE_FIVE, DICE_SIX};
public:
	int m_nPenya[6];		//�� ��ȣ�� ������ �ݾ�
	int m_nSelectCtrl;		//������ ��ȣ
	int m_nDiceChoiceNum;	//���õ� �ֻ����� �����
	int m_nDiceNum;			//�ֻ��� ��ȣ
	int m_pBtnID[6];		//Penya �ö� Ctrl�� ID
	int m_nCount;
	int m_nDelay;			//Com ���¸� ȸ����Ű�� Delay��
	int m_nStatus;			//������ ���� (Delay���� ���� ���� ���)
	int m_nMaxPenya;		//�ִ� �����ݾ�
	int m_nMinPenya;		//�ּ� �����ݾ�
	int m_nMultiple;		//������� ���
	int m_nPrizePenya;		//������� �׼�
	int m_pStaticID[6];
	int m_pStaticRID[6];
	int m_pStaticNum[6];
	int m_nAlpha;

	BOOL m_bSendStart;		//������ ����� ��û ��
	BOOL m_bReciveResult;	//������ ���� ����� �޾Ҵ��� Ȯ��
	BOOL m_bEnd;			//�ֻ��� ������ ���� ����
	BOOL m_bFlsh;

	CString m_strPathDice[6];
	CString m_strPathDiceNum[6];
	
	CWndText* m_pCost[6];	//Penya �ø� Ctrl
	CWndText* m_pRCost[6];	//Penya ��� Ctrl
	CWndText* m_pText;
	
	CWndDiceGameTender* m_pWndDiceTender;
	CWndDiceGameMessage* m_pWndMessageBox;
	
public: 
	CWndDiceGame(); 
	virtual ~CWndDiceGame(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL Process ();
	
	void RefreshInfo();
	void SetMinMaxPenya(int nMinPenya, int nMaxPenya, int nMultiple);	//�ּ�/�ִ� �����ݾ�, ��ǰ���� ����� ����
	void CheckPenyaUsuable(int nPenya, int nSelect);	//�����Ǵ� �ݾ��� ������ �ݾ������� Ȯ��
	void SetTenderPenya(int nSelect, int nPenya);		//������ �ݾ��̶�� �����Ѵ�.
	void ReceiveResult(int nDiceNum, int nPenya);		//������ ���� �ֻ����� ��� ���� ����
	void SetDescription(CHAR* szChar);
	void RefreshCtrl();
	void ResetPenya();
}; 

class CWndDiceGameTender : public CWndNeuz 
{
public:
	CWndDiceGame* m_pDiceGame;
	int m_nTenderPenya;
	int m_nSelect;
	int m_nMaxPenya;
	int m_nMinPenya;
	
public: 
	CWndDiceGameTender(); 
	virtual ~CWndDiceGameTender(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point );

	void SetMinMaxPenya(int nMinPenya, int nMaxPenya);
	void SetSelectNum(int nSelect);
};

class CWndDiceGameMessage : public CWndNeuz
{
public:
	CWndButton m_wndButton;
protected:
	CString m_strText;
	CWndText m_wndText;
	
public:
	CWndDiceGameMessage();
	~CWndDiceGameMessage();
	
	virtual void OnDestroy();
	virtual BOOL Initialize( LPCTSTR lpszMessage, CWndBase* pWndParent, DWORD nType = MB_OK );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
};

class CWndPuzzleGame : public CWndNeuz 
{
public:
	DWORD m_itemID[9];			//�� ������ Object ID
	OBJID m_objItemID[9];
	CItemElem* m_pItemElem[9];	//������ ���� ����.
	int m_pStaticID[9];			//5���� WndID -> Picture (ondropicon ����� ����)
	int m_nSelectCtrl;			//���õ� Wnd ���.
	CWndText* m_pText;
	BOOL m_bStart;

	CWndInventory* m_pWndInventory;
public: 
	CWndPuzzleGame(); 
	virtual ~CWndPuzzleGame(); 
	
	virtual void OnDestroy();
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD nType = MB_OK ); 
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult ); 
	virtual void OnDraw( C2DRender* p2DRender ); 
	virtual	void OnInitialUpdate(); 
	virtual BOOL OnCommand( UINT nID, DWORD dwMessage, CWndBase* pWndBase ); 
	virtual void OnSize( UINT nType, int cx, int cy ); 
	virtual void OnLButtonUp( UINT nFlags, CPoint point ); 
	virtual void OnLButtonDown( UINT nFlags, CPoint point ); 
	virtual BOOL OnDropIcon( LPSHORTCUT pShortcut, CPoint point );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
	virtual BOOL Process ();
	
	int HitTest( CPoint point );
	void SetPuzzle(vector<DWORD> vPuzzle);
	void SetPicture(CItemElem* pItemElem);
	void ReceiveResult();
	void SetDescription(CHAR* szChar);
}; 

#endif //__WNDMINIGAME__H
#endif //__EVE_MINIGAME