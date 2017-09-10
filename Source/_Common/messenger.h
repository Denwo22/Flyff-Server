#ifndef __MESSENGER_H__
#define	__MESSENGER_H__

#include "Ar.h"

#define FRS_ONLINE		0	// �⺻ : ������ 
#define FRS_OFFLINE		1	// �ڵ� : �α׿���
#define FRS_BLOCK	2	// ���� : ���� 
#define FRS_ABSENT	3	// ���� : �ڸ����
#define FRS_HARDPLAY	4	// ���� : ������ 
#define FRS_EAT		5	// ���� : �Ļ��� 
#define FRS_REST	6	// ���� : �޽���
#define FRS_MOVE	7	// ���� : �̵��� 
#define FRS_DIE		8	// �ڵ� : ��� 
#define FRS_DANGER		9	// �ڵ� : ����
#define FRS_OFFLINEBLOCK	10
#define	FRS_AUTOABSENT		11	// �ڵ� : �ڵ� �ڸ����	
#define MAX_FRIENDSTAT		12	// �ƽ���

#define MAX_FRIEND		200 // �ƽ��� ģ�� ��� Ƚ��

typedef struct tagFRIEND
{
	BOOL bGroup;	// TRUE�̸� �׷�, �ƴϸ� ���� 
	DWORD dwState;	// ���� 0 �̸� ����, 1�̸� disconnect, 2�̸� �ڸ� ���.....
	DWORD dwUserId;
#if __VER < 11 // __SYS_PLAYER_DATA
	BYTE  nSex;
	LONG  nJob;
	TCHAR szName[ 64 ];
#endif	// __SYS_PLAYER_DATA
	BOOL  bSave;
#ifdef __CLIENT
	u_long m_uIdofMulti;
#endif // __CLIENT

	tagFRIEND()
	{
		bGroup = FALSE;
		dwState = FRS_OFFLINE;
		dwUserId = 0;
#if __VER < 11 // __SYS_PLAYER_DATA
		nSex = 0;
		nJob = 0;
		szName[0] = '\0';
#endif	// __SYS_PLAYER_DATA
		bSave = FALSE;
#ifdef __CLIENT
		m_uIdofMulti = 100;
#endif // __CLIENT
	}
} FRIEND,* LPFRIEND;

typedef	map< u_long, FRIEND*>	C2FriendPtr;

class CMessenger
{
public:
	C2FriendPtr m_aFriend; // ���� ����� ������( ģ�� ��� )
	C2FriendPtr m_adifferntFriend; // ���� ����� ������
	DWORD		m_dwMyState;

public:
#if __VER >= 11 // __SYS_PLAYER_DATA
	BOOL		AddFriend( u_long uidPlayer );
	BOOL		AddFriend( u_long uidPlayer, DWORD dwSate, BOOL bSave = FALSE );
#else	// __SYS_PLAYER_DATA
	BOOL		AddFriend( u_long uidPlayer, LONG nJob, BYTE nSex );
	BOOL		AddFriend( u_long uidPlayer, LONG nJob, BYTE nSex, LPSTR lpName );
	BOOL		AddFriend( u_long uidPlayer, LONG nJob, BYTE nSex, DWORD dwSate, BOOL bSave = FALSE );
#endif	// __SYS_PLAYER_DATA
	void		SetSave( void );
	BOOL		RemoveFriend( u_long uidPlayer );
	BOOL		RemoveDifferntFriend( u_long uidPlayer );
	LPFRIEND	GetFriend( u_long uidPlayer );
	LPFRIEND	GetDefferntFriend( u_long uidPlayer );
	BOOL		IsFriend( u_long uidPlayer );
	BOOL		IsDefferntFriend( u_long uidPlayer );
	BOOL		AddDefferntFriend( u_long uidPlayer );
	void		Clear( void );
	void		Serialize( CAr & ar );
	DWORD		GetState() { return m_dwMyState; };
	LPFRIEND    GetAt( int nIndex );
	int			GetSize();
	virtual	CMessenger&	CMessenger::operator=( CMessenger & rMessenger );

	CMessenger();
	~CMessenger();
};

#endif	//	_MESSENGER_H