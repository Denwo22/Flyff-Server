#ifndef __AI_PET_H
#define __AI_PET_H

#include "..\_AIInterface\AIInterface.h"
#include "mempooler.h"
#include "Mover.h"

class CAIPet : public CAIInterface
{
private:
	OBJID m_idOwner;		// ���δ�
	BOOL	m_bLootMove;				// �����Ϸ� ������.
	DWORD	m_idLootItem;				// ������ ������.
	int		m_nState;					// ���� ����.
	
	void MoveToDst(	OBJID idTarget );
	void MoveToDst(	D3DXVECTOR3 vDst );

	BOOL  MoveProcessIdle( const AIMSG & msg );
	BOOL  MoveProcessRage( const AIMSG & msg );

	void Init( void );
	void Destroy( void );
	
public:
	CAIPet();
	CAIPet( CObj* pObj );
	virtual ~CAIPet();

	virtual void InitAI();

	void	SetOwner( OBJID idOwner ) { m_idOwner = idOwner; }
	BOOL	GetLootMove( void ) { return m_bLootMove; }
	DWORD	GetLootItem( void ) { return m_idLootItem; }
	int		GetState( void ) { return m_nState; }
	BOOL	StateInit( const AIMSG & msg );
	BOOL	StateIdle( const AIMSG & msg );
	BOOL	StateRage( const AIMSG & msg );

	BOOL SubItemLoot( void );

#if __VER >= 12 // __PET_0519
	// ���ΰ� ���� ������ ����, ���� �� ���� ȿ�� ����
	void	SetItem( CMover* pPlayer, CItemElem* pItem );
	// �������κ��� ���� ���� ȿ�� ����
	void	ResetItem( void );
	// �ش� ���� ������ �ĺ��� ��ȯ
	OBJID	GetPetItemId( void )	{	return m_idPetItem;		}
private:
	void	SetSkillId( DWORD dwSkill )		{ m_dwSkillId = dwSkill; }	// ��� �� ����
	DWORD	GetSkillId()	{	return m_dwSkillId;	}	// ��� �� ����
	void	SetSkill( CMover* pPlayer, ItemProp* pProp );	// ��� �� ����
	void	ResetSkill( CMover* pPlayer );	// ��� �� ����
	DWORD	m_dwSkillId;	// ��� �� ����
	OBJID	m_idPetItem;	// ���� ������ �ĺ���
#endif	// __PET_0519
private:
	BOOL	NotOwnedPetInactivated();

public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CAIPet>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CAIPet::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CAIPet::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CAIPet::m_pPool->Free( (CAIPet*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CAIPet::m_pPool->Free( (CAIPet*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
	
	DECLARE_AISTATE()
};

#endif

