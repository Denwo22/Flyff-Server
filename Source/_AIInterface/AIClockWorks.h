#ifndef __AI_CLOCKWORKS_H
#define __AI_CLOCKWORKS_H

#include "mempooler.h"

class CAIClockWorks : public CAIInterface
{
	D3DXVECTOR3 m_vPosBegin;		// ���� ������ ��ǥ.
	int		m_nEvent;				// ���� ���� ����.
	int		m_nAttackType;			// ���� ���.
	DWORD	m_tmReattack;			// ����� Ÿ�̸�.
	DWORD	m_tmAddReattack;		// ����� Ÿ�̸��� ����������.(�� ���������� �� ���� ��� ��쵵 ����)
	DWORD	m_tmTrace;				// �����ð�.
	DWORD	m_tmTimeOver;			// ����ġ ���� ��Ȳ���� ���̻� ������ ���ϴ��� �ϴ°�츦 ����
	OBJID	m_idTarget;				// ���� ���
	D3DXVECTOR3	m_vTarget;			// ���� ��ġ.
	int		m_nAppearCnt;			// ���� ī����.
	
	OBJID	m_idLastAttacker;		// �� ���������� ������ ����.

	BOOL	m_bDefenseMode;			// ����¼� ���.
	
	BOOL	MoveProcessIdle();
	BOOL	MoveProcessRage();
	BOOL	MoveProcessRunaway();
	
	BOOL	StopProcessIdle();

	BOOL	SelectTarget( void );		// ������ Ÿ�̹��� ������ Ÿ���� ������.
		
	void Init( void );
	void Destroy( void );

public:
	
	CAIClockWorks();
	CAIClockWorks( CObj* pObj );
	virtual ~CAIClockWorks();
	
	virtual void InitAI();

	int GetEvent( void ) { return m_nEvent; }
	BOOL	StateInit( const AIMSG & msg );
	BOOL	StateIdle( const AIMSG & msg );
	BOOL	StateWander( const AIMSG & msg );
	BOOL	StateRunaway( const AIMSG & msg );
	BOOL	StateEvade( const AIMSG & msg );
	BOOL	StateRage( const AIMSG & msg );
	
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CAIClockWorks>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CAIClockWorks::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CAIClockWorks::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CAIClockWorks::m_pPool->Free( (CAIClockWorks*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CAIClockWorks::m_pPool->Free( (CAIClockWorks*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
	
	DECLARE_AISTATE()
};

#endif

