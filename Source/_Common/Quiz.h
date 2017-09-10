// Quiz.h: interface for the CQuiz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUIZ_H__96B9AD5D_E263_4FAB_AA19_715640D07381__INCLUDED_)
#define AFX_QUIZ_H__96B9AD5D_E263_4FAB_AA19_715640D07381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __QUIZ
#if defined(__DBSERVER) || defined(__WORLDSERVER)
#include "ar.h"
#define		REMAIN_QUIZ			30
#endif // defined(__DBSERVER) || defined(__WORLDSERVER)
#define		TYPE_OX				1
#define		TYPE_4C				2
#define		MAX_EXAMPLE			4
#define		MAX_QUIZ_SIZE		1024
#define		LIMIT_USER_COUNT	50


class CQuiz 
{
public:
	enum {	QE_CLOSE, QE_OPEN, QE_CLOSE_WAIT, QE_WATCHINGZONE_OPEN, QE_QUESTION, QE_CORRECT_ANSWER, QE_DROP_OUT, QE_WINNER	};
	
	CQuiz();
	~CQuiz();
	
	static CQuiz* GetInstance();

	void	SetType( int nType )	{	m_nType = nType;	}
	int		GetType()	{	return m_nType;	}

private:
	int			m_nType;		// O/X or multiple choice...
	
#if defined(__DBSERVER) || defined(__WORLDSERVER)
public:
	typedef struct _QUIZLIST
	{
		int		nQuizId;		// ������ id
		CString	strQuestion;	// ���� ����
		int		nCorrect;		// ���� ����
		int		nItemId;		// ������ ������ ��� ������ ������ id
		int		nItemNum;		// ������ ����
		
		void Serialize( CAr & ar )
		{
			if( ar.IsStoring() )
			{
				ar << nQuizId;
				ar.WriteString( strQuestion );
				ar << nCorrect;
				ar << nItemId;
				ar << nItemNum;
			}
			else
			{
				ar >> nQuizId;
				char szTemp[MAX_QUIZ_SIZE] = {0, };
				ar.ReadString( szTemp, MAX_QUIZ_SIZE );
				strQuestion = szTemp;
				strQuestion.Replace( "|", "\r\n" );
				ar >> nCorrect;
				ar >> nItemId;
				ar >> nItemNum;
			}
		}
	} QUIZLIST;
	typedef vector<QUIZLIST>	VEC_QUIZLIST;
	
	void	SetAutoMode( BOOL bAuto )	{	m_bAuto = bAuto;	}
	BOOL	IsAutoMode()	{	return m_bAuto;		}
	void	SetRun( BOOL bRun )		{	m_bRun = bRun;	}
	BOOL	IsRun()		{	return m_bRun;	}
	DWORD	GetEntranceTime()	{	return m_dwEntranceTime;	}
	void	Clear();
	void	LoadScript();
#endif // defined(__DBSERVER) || defined(__WORLDSERVER)
	
#if defined(__CLIENT) || defined(__WORLDSERVER)
public:
	enum {	ZONE_QUIZ = 100, ZONE_1, ZONE_2, ZONE_3, ZONE_4, ZONE_WATCHING, ZONE_EXIT	};
	
	void	SetState( int nState )	{	m_nState = nState;	}
	int		GetState()	{	return m_nState;	}
	void	InitQuizData( int nType );
	int		GetZoneType( CMover* pMover );
#endif // defined(__CLIENT) || defined(__WORLDSERVER)
	
#ifdef __WORLDSERVER
public:
	void	SetNextTime( DWORD dwNextTime )		{	m_dwNextTime = dwNextTime;	}
	DWORD	GetNextTime()		{	return m_dwNextTime;	}
	
	void	Process();
	void	OpenQuizEvent();
	void	MakeQuizList( QUIZLIST & QL );
	void	SetNPC();
	void	RemoveNPC();
	void	NoticeMessage();
	int		EntranceQuizEvent( CUser* pUser );
	int		TeleportToQuizEvent( CUser* pUser, int nZone );
	D3DXVECTOR3	GetTeleportPos( int nZone );
	void	DropOutWrongUser( int nQuizId, int nCorrect, int nItemId = 0, int nItemNum = 0 );
	BOOL	PtInCorrectZoneRect( CUser* pUser, int nCorrect );
	int		GetUserSelectExample( CUser* pUser );
	void	GoOut( CUser* pUser );
	void	CloseQuizEvent();
private:
	BOOL	IsInQuizEventPlayer( CWorld* pWorld, int nLayer, int nIndex );
#endif // __WORLDSERVER
	
#if defined(__DBSERVER) || defined(__WORLDSERVER)
private:
	BOOL			m_bAuto;						// GM or Auto
	BOOL			m_bRun;
	DWORD			m_dwEntranceTime;				// ���� �ð�
	
public:
	int				m_nQuizSize;					// ���� ���� ��
	VEC_QUIZLIST	m_vecQuizList;					// ���� ����Ʈ
#endif // defined(__DBSERVER) || defined(__WORLDSERVER)
	
#if defined(__CLIENT) || defined(__WORLDSERVER)
private:
	int				m_nState;						// ���� ����
	
	typedef struct _QUIZDATA
	{
		D3DXVECTOR3		vQuizZonePos;				// ���� �� ��ǥ
		D3DXVECTOR3		vWatchingZonePos;			// ��� �� ��ǥ
		D3DXVECTOR3		aExamplePos[MAX_EXAMPLE];	// ���� ���� ��ǥ
		CRect			aExampleRect[MAX_EXAMPLE];	// ���� ���� ����
		CRect			QuizEventRect;				// ���� �̺�Ʈ �� ����
	} QUIZDATA;
	
public:
	QUIZDATA		m_sQuizData;
	//	CObj*			m_pVObj;
	//	CObj*			m_pHObj;
#endif // defined(__CLIENT) || defined(__WORLDSERVER)
	
#ifdef __WORLDSERVER
private:
	struct QUIZNPC
	{
		OBJID	idNpc;
		DWORD	dwNPCId;
		string	strCharKey;
		D3DXVECTOR3	vPos;
	};

	BOOL			m_bSelectLog;
	int				m_nChannel;						// ���� ä��
	int				m_nQuizCount;					// Ǯ���� ���� ��
	DWORD			m_dwWaitTime;					// ��� �ð�
	DWORD			m_dwQuestionTime;				// ���� Ǯ�� �ð�
	DWORD			m_dwAnswerTime;					// ���� ���� �ð�
	DWORD			m_dwWatchingZoneOpenTime;		// ����� ���� �ð�
	DWORD			m_dwCloseWaitTime;				// ���� �̺�Ʈ ���� ��� �ð�
	DWORD			m_dwNextTime;
	DWORD			m_dwPrizeItemId;				// ���� ��ǰ ������ ID
	int				m_nPrizeItemNum;				// ���� ��ǰ ������ ����

	QUIZNPC			m_sNPC;
#endif // __WORLDSERVER
	
#ifdef __CLIENT
public:
	string			m_strQuestion;
	int				m_nQCount;
#endif // __CLIENT
};

#endif // __QUIZ
#endif // !defined(AFX_QUIZ_H__96B9AD5D_E263_4FAB_AA19_715640D07381__INCLUDED_)
