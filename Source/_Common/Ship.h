#ifndef __SHIP_H__
#define __SHIP_H__

#include "Ctrl.h"
#include "..\_AIInterface\ActionShip.h"

#define MAX_LINKCTRL	128

/// ����� 
class CShip : public CCtrl
{
private:
	CMover *m_pMover;			// �� ���� ����. �����ϴ� ���.
	CActionShip m_Act;
	
protected:
	int		m_nCount;
	D3DXVECTOR3		m_vDelta;
	D3DXVECTOR3		m_vDeltaUnit, m_vAcc;		// ����, �������� ��
	FLOAT			m_fDeltaAng;
	FLOAT	m_fAccAng;		// ȸ�� ���ӵ�.
	OBJID	m_LinkCtrl[ MAX_LINKCTRL ];
	
	virtual void Init( void );
	virtual void Destroy( void );
public:
	friend CActionShip;
	
	CShip();
	virtual ~CShip();
	
	CMover *GetMover( void ) { return m_pMover; }				// �� ���� ������ ������.
	void SetMover( CMover *pMover ) { m_pMover = pMover; }		// �� �����ϴ� ����� ����.
	int		SendActMsg( OBJMSG dwMsg, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0 )
	{
		return m_Act.SendActMsg( dwMsg, nParam1, nParam2, nParam3 );
	}
	void	AddCtrl( OBJID idCtrl );
	OBJID*	FindCtrl( OBJID idCtrl );
	void	RemoveCtrl( OBJID idCtrl );
	D3DXVECTOR3 GetDelta( void ) { return m_vDelta; }
	FLOAT		GetDeltaAng( void ) { return m_fDeltaAng; }

	void Control( void );
	virtual void Process();
		
};

// �ö󸮽� ������ ������ ���� ��
class CShipLoop : public CShip
{
private:
	
protected:
	void Init( void );
	void Destroy( void );
public:
	
	CShipLoop();
	virtual ~CShipLoop();
	
	void Process();
	
};


#ifdef __CLIENT
extern CShip *g_pShip;
#endif


#endif // SHIP_h