// MiniGame.h: interface for the CMiniGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_)
#define AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __EVE_MINIGAME

class CUser;
class CMiniGame  
{
public:
	enum { KAWIBAWIBO_WIN, KAWIBAWIBO_LOST, KAWIBAWIBO_DRAW, KAWIBAWIBO_FAILED, KAWIBAWIBO_GETITEM	};
	enum { KAWI, BAWI, BO };
	enum { ALPHABET_NOTENOUGH_MONEY = 998, ALPHABET_FAILED = 999, };
	enum { FIVESYSTEM_BETOK = 996, FIVESYSTEM_FAILED = 997, FIVESYSTEM_OVERMAX = 998, FIVESYSTEM_NOTENOUGH = 999 };

	struct __ITEMGAME
	{
		DWORD	dwItemId;	// ���̵�
		int		nItemCount;	// ����
		DWORD	dwPer;		// �ۼ�Ʈ
		__ITEMGAME()
		{
			dwItemId = 0;
			nItemCount = 0;
			dwPer = 0;
		};
	};
	
	struct __ALPHABET
	{
		int		nPublic;	// ���� ���� ��ġ
		CString strWord;	// ���� �ܾ�
		CString	strQuestion;	// ����
		vector<__ITEMGAME>	vecItem;	// ���� ������
		__ALPHABET():nPublic(1),strWord(_T("")), strQuestion(_T("")) {}
	};
	
	struct __KAWIBAWIBORESULT
	{
		int		nMy;
		int		nYou;
		int		nResult;
	};

	struct __KAWIBAWIBO
	{
		DWORD	dwDraw;		// ��� Ȯ��
		DWORD	dwLost;		// �й� Ȯ��
		DWORD	dwWin;		// �¸� Ȯ��
		DWORD	dwItemId;	// ���̵�
		int		nItemCount;	// ������ ����
		__KAWIBAWIBO()
		{
			dwDraw = 0;		
			dwLost = 0;	
			dwWin = 0;	
			dwItemId = 0;
			nItemCount = 0;
		};
	};

	struct __FIVESYSTEM		// (����) ������ ���� ������ Ȯ��
	{
		DWORD	dwMin;		// Min Ȯ��
		DWORD	dwMax;		// Max Ȯ��
		__FIVESYSTEM()
		{
			dwMin = 0;
			dwMax = 0;
		};
	};



public:
	CMiniGame();
	virtual ~CMiniGame();

	BOOL Load_Alphabet();	// ���ĺ� �ܾ� ���߱�
	BOOL Load_KawiBawiBo();	// ����, ����, ��
	BOOL Load_FiveSystem();	// ����
	BOOL Load_Reassemble(); // ���� 
	char GetCharacter( DWORD ItemId ); // ������ ID �´� ���ĺ� �� ���� ����
	int	GetKawibawiboYou( int nMy, int nResult );
#ifdef __WORLDSERVER
	int Result_Kawibawibo( CUser* pUser );
	BOOL ResultItem_Kawibawibo( CUser* pUser );
	__KAWIBAWIBO FindKawibawibo( int nWinCount );
	
	BOOL Result_Reassemble( CUser* pUser, OBJID* objItemId, int nCount );
	BOOL ResultItem_Reassemble( CUser* pUser );
	__ITEMGAME GetReassembleRandomItem();
	void OpenWnd_Reassemble( CUser* pUser );
	
	void OpenWnd_Alphabet( CUser* pUser );
	int Result_Alphabet( CUser* pUser, OBJID* objItemId, int nCount, int nQuestionID );
	BOOL ResultItem_Alphabet( CUser* pUser, int nNum );
	__ITEMGAME GetAlphabetRandomItem( int nNum );
	char GetPublicCharacter( int nQuestionNum, int nPublicAlphabet );
		
	void OpenWnd_FiveSystem( CUser* pUser );
	void DestroyWnd_FiveSystem( CUser* pUser );
	int Bet_FiveSystem( CUser* pUser, int nBetNum, int nBetPenya );
	int Result_FiveSystem( CUser* pUser );
	BOOL ResultPenya_FiveSystem( CUser* pUser, int nResult );
#endif // __WORLDSERVER
	
	int m_nBetMinPenya;				// ���� - �ּ� ���� �ݾ�
	int m_nBetMaxPenya;				// ���� - �ִ� ���� �ݾ�
	int m_nMultiple;				// ���� - ��� ���� ���
	
	int m_nPayPenya;				// ���ĺ� ���� ���
	
	int nKawiBawiBo_MaxWin;			// ���������� �ִ� ���� �ε���

	vector<__ALPHABET>		m_vecAlphabet;	
	map<int, __KAWIBAWIBO>  m_mapKawiBawiBo;
	map<int, __FIVESYSTEM>	m_mapFiveSystem;
	vector<__ITEMGAME>		m_vecReassembleItem;
	vector<DWORD>	m_vecReassembleCard;
	vector<__KAWIBAWIBORESULT> m_vecKawibawiboResult;
};

#endif // __EVE_MINIGAME
#endif // !defined(AFX_MINIGAME_H__79AC4FCB_D41A_4407_ACF3_0FDF796F2ED1__INCLUDED_)
