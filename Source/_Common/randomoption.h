#ifndef __RANDOM_OPTION_EX_H__
#define	__RANDOM_OPTION_EX_H__

#include <vector>

#define	MAX_RANDOM_OPTION	3

//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
#define _AWAKE_SAFE_FLAG	0x2000000000000000

#define _AWAKE_OLD_VALUE	1
#define _AWAKE_NEW_VALUE	2
//	mulcom	END100405	���� ��ȣ�� �η縶��


typedef	struct	_AdjData
{
	short	nAdj;
	DWORD	dwProb;
	_AdjData( short n, DWORD dw )
	{
		nAdj	= n;
		dwProb	= dw;
	}
}	AdjData;

typedef struct	_RANDOM_OPTION
{
	int		nDst;	// �Ķ����
	int		nProb;	//	�� �Ķ���Ͱ� ���� Ȯ��
	vector<AdjData>	aAdjData;
	_RANDOM_OPTION()
	{
		nDst	= 0;
		nProb	= 0;
	}
	_RANDOM_OPTION( _RANDOM_OPTION* pRandomOption )
	{
		nDst	= pRandomOption->nDst;
		nProb	= pRandomOption->nProb;
		aAdjData.assign( pRandomOption->aAdjData.begin(), pRandomOption->aAdjData.end() );
	}
}	RANDOM_OPTION;

class CRandomOptionProperty
{
public:
#if __VER >= 12 // __PET_0519
	// �ý��� �� ������ ���� ���� �߰�
	enum	{	eAwakening, eBlessing,	eSystemPet, eEatPet, eMaxRandomOptionKind	};		// ���� �ɼ� ����	// ����: 0, �ູ: 1
#else	// __PET_0519
	enum	{	eAwakening, eBlessing,	eMaxRandomOptionKind	};		// ���� �ɼ� ����	// ����: 0, �ູ: 1
#endif	// __PET_0519

#if __VER >= 12 // __J12_0
	enum	{	eAwakeningExtension	= 3	};	// 0: ��, 1: ��, 2: �Ӹ�	// ���� Ȯ��
#endif	// __J12_0

	CRandomOptionProperty();
	virtual	~CRandomOptionProperty();
	static	CRandomOptionProperty*	GetInstance();
	
	BOOL	LoadScript( LPCTSTR szFile );
	void	LoadScriptBlock( CScript& s, int nRandomOptionKind );

	// i��° ���� �ɼ� �Ķ���Ϳ� ���� ��ȯ, ��������� FALSE ��ȯ
	BOOL	GetParam( __int64 nRandomOptItemId, int i, int* pnDst, int* pnAdj );
	// �ο��� ���� �ɼ� �Ķ������ ���� ��ȯ
	int		GetRandomOptionSize( __int64 nRandomOptItemId );

	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	int		GetViewRandomOptionSize( __int64 n64RandomOptItemId );
	//	mulcom	END100405	���� ��ȣ�� �η縶��

	// ���� ������ ���� ���� �ɼ��� ���� ��ȯ
	int		GetRandomOptionKind( CItemElem* pItemElem );

	// ���� �ɼ� �ο�
	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	//BOOL	GenRandomOption( __int64* pnRandomOptItemId, int nRandomOptionKind, int nParts );
	BOOL	GenRandomOption( __int64* pnRandomOptItemId, int nRandomOptionKind, int nParts, bool bDecreaseFlag = false );
	//	mulcom	END100405	���� ��ȣ�� �η縶��

	// ���� �ɼ� �ʱ�ȭ
	void	InitializeRandomOption( __int64* pnRandomOptItemId );

	// �ش� �Ķ���͸� ���� ���� �ɼ� ���� ��°�� ����	- GenRandomOption���� ȣ��.
	void	SetParam( __int64* pnRandomOptItemId, int nDst, int nAdj );
private:
	int		DetermineRandomOptionSize( int nRandomOptionKind );
	RANDOM_OPTION*	DetermineRandomOptionDst( int nRandomOptionKind, int nParts );

	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
	//short DetermineRandomOptionAdj( RANDOM_OPTION* pRandomOption );
	short DetermineRandomOptionAdj( RANDOM_OPTION* pRandomOption, bool bDecreaseAdj = false );
	//	mulcom	END100405	���� ��ȣ�� �η縶��

	int		GetRandomOptionKindIndex( int nRandomOptionKind, int nParts );
	int		GetUpperProbability( int iRandomOptionKindIndex );
#if __VER >= 12 // __J12_0
	void	AwakeningExtension( void );		// ���� �ູ ���̺� Ȯ��
#endif	// __J12_0
private:
	int		m_anRandomOptionProb[eMaxRandomOptionKind][MAX_RANDOM_OPTION];
#if __VER >= 12 // __J12_0
	// �⺻ + Ȯ��
	vector<RANDOM_OPTION>	m_aRandomOption[eMaxRandomOptionKind + eAwakeningExtension];
#else	// __J12_0
	vector<RANDOM_OPTION>	m_aRandomOption[eMaxRandomOptionKind];
#endif	// __J12_0


	//	mulcom	BEGIN100405	���� ��ȣ�� �η縶��
public:
	bool	IsCheckedSafeFlag( __int64 n64RandomeOption );
	void	SetSafeFlag( __int64* pn64RandomOption );
	void	ResetSafeFlag( __int64* pn64RandomOption );
	//	mulcom	END100405	���� ��ȣ�� �η縶��
};


#define g_xRandomOptionProperty		CRandomOptionProperty::GetInstance()

#endif	//__RANDOM_OPTION_EX_H__