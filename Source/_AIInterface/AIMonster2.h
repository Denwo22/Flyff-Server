#ifndef __AI_MONSTER2_H
#define __AI_MONSTER2_H

#include "AIInterface.h"

class CObj;

class CAIMonster2 : public CAIInterface
{
public:
	CAIMonster2();
	CAIMonster2( CObj* pObj );
	virtual ~CAIMonster2();
	
	virtual void	RouteMessage();
	virtual void	SendAIMsg( DWORD dwMessage, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	virtual void	PostAIMsg( DWORD dwMessage, DWORD dwParam1 = 0, DWORD dwParam2 = 0 );
	virtual void	InitAI();

	DWORD			GetState() { return m_nState; }

private:
	DWORD			m_dwFsmType;
	DWORD			m_nState;
	DWORD			m_dwTick;
	D3DXVECTOR3		m_vPosBegin;

	DWORD			m_idLastAttacker;		// ���� ������ �� id 
	OBJID			m_idTarget;				// ���� ��� id 
	D3DXVECTOR3		m_vTarget;				// ���� ��� ��ġ 
	int				m_nAttackType;			// ���� ���
	float			m_fAttackRange;			// ���� ���� 
	DWORD			m_dwReattack;			// �� ���� �ð� 

	void	Init();
	BOOL	Check( int nInput, DWORD dwCurTick, DWORD dwValue );		// �ֱ��� �˻�( polling )
	BOOL	IsControllable();
	void	ProcessAIMsg( int nInput, int nOutput, DWORD dwParam1, DWORD dwParam2 );
	void	OnBeginState( int nInput, DWORD dwParam1, DWORD dwParam2 );
	void	OnEndState( int nInput, DWORD dwParam1, DWORD dwParam2 );
	BOOL	BeginAttack();
	void	MoveToDst( const D3DXVECTOR3& vDst );
	BOOL	SelectTarget();
	BOOL	Search();
	BOOL	IsInValidTarget();
};

#endif

