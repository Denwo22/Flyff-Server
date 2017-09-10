// Sfx.h: interface for the CSfx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SFX_H__D652787A_4E67_419F_AB52_0A8E2FED08AE__INCLUDED_)
#define AFX_SFX_H__D652787A_4E67_419F_AB52_0A8E2FED08AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "light.h"
class CSfxGenMoveMark;
extern CSfxGenMoveMark* g_pMoveMark;

#ifdef __CLIENT
#include "mempooler.h"
#endif	// __CLIENT

class CSfxModel;

/// ȿ��(��ƼŬ, �߻�ü)�� ó���ϴ� base class
class CSfx  : public CCtrl
{
protected:
	CSfxModel m_SfxObj2; // ���� ȿ�� : ����ϴ��� ������  
	CSfxModel m_SfxObj3; // ���� ȿ�� : ����ϴ��� ������ 

#if defined( __CLIENT ) 
	DWORD	m_dwSkill;		// ��ų�߻�ü�� ��ų ���̵�..
#endif	// 
	
public:
	int m_nSec;
	int m_nFrame;

	CSfxModel* m_pSfxObj; // ����Ʈ �� ������
	OBJID       m_idSrc;    // ��� ID
	OBJID       m_idDest;   // ������ ID
	D3DXVECTOR3 m_vPosDest; // ���� ��ġ
//	D3DXVECTOR3 m_vPosCur;  // ���� ��ġ - �̰� �� �ʿ��Ѱɱ�.
#if defined( __CLIENT ) 
	CLight  m_light;
	int		m_idSfxHit;
	int		m_nMagicPower;	// ���� �ܰ�
//	DWORD	m_dwSkillLevel;	// ��ų����.
#endif	// 

	CSfx();
	CSfx( int nIndex, const OBJID idSrc, const D3DXVECTOR3& vPosSrc, const OBJID idDest, const D3DXVECTOR3& vPosDest );
	virtual ~CSfx();

#if defined( __CLIENT ) 
	void	SetSkill( DWORD dwSkill )
	{
		m_dwSkill = dwSkill;
	}
	DWORD	GetSkill()
	{
		return m_dwSkill;
	}
#endif
	
	void Process(); // �� �����Ӹ��� ��ġ�̵�, �ִϸ��̼� �� ó��
	void DamageToTarget( int nDmgCnt = 0, float fDmgAngle = 0, float fDmgPower = 0, int nMaxDmgCnt = 1 ); // ��ǥ���� �¾����� �������� �ش�
	BOOL SetIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwIndex, BOOL bInitProp = FALSE );

	virtual	int SetSfx( LPDIRECT3DDEVICE9 pd3dDevice, int nIndex, D3DXVECTOR3& vPosSrc, OBJID idSrc, D3DXVECTOR3& vPosDest, OBJID idDest, int nSec = 0 ); // ����� SFX ����
	virtual void ShootSfx( float fAngXZ, float fAngY, float fSpeed ) {}
	virtual void SetPartLink( int nPart ) {}
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice ); // ����
#endif	// __WORLDSERVER

};

// ��� ���ư��� �°� ������ �Ϲ����� ����Ʈ
class CSfxShoot : public CSfx
{
public:
	BOOL	m_bHit; // �����ߴ����� ����
	BOOL	m_bDir;
	DWORD	m_dwSndHit;		// �¾����� �Ҹ�.
	DWORD	m_dwSfxHit;		// ���� ����Ʈ
	FLOAT	m_fHitScale;	// ���߾� �����ϸ�.
	
	CSfxShoot();
	virtual ~CSfxShoot();
	
	void	SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetDir( BOOL bDir );		// ���⼺�ִ� sfx �ΰ�.
	void	SetSndHit( DWORD dwSndHit ) { m_dwSndHit = dwSndHit; }
	
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// CSfx�� �Ȱ��� idSrc�� ȸ��������� �Ȱ�����.
class CSfxRotate : public CSfx
{
public:
	CSfxRotate() {}
	~CSfxRotate() {}

	void Process();
#ifndef __WORLDSERVER
	void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif
};

class CSfxDuelParty : public CSfx
{
public:
	int	m_nType;
	CSfxDuelParty() { m_nType = 0; };	// 0:���  1:����.
	~CSfxDuelParty() {};

	void Process();
	void DeleteSfx( CMover* pEnemy );
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif		
};

class CSfxSetItem : public CSfx
{
public:
	CSfxSetItem() {};
	~CSfxSetItem();
	void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif		
};


class CSfxCollect : public CSfx
{
public:
	CSfxCollect() {};	
	~CSfxCollect() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
};

// Ŭ����ũ ���� ������ ����.
class CSfxClockWorksCharge : public CSfx
{
public:
	CSfxClockWorksCharge() {};	
	~CSfxClockWorksCharge() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
};

// Ŭ����ũ ���� ���ȿ��� �߻��Ҷ� ������ ����Ʈ.
class CSfxClockWorksCannon : public CSfx
{
public:
	CSfxClockWorksCannon() {};	
	~CSfxClockWorksCannon() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
};

// �ӽ���Ʈ ���� ������ ����.
class CSfxMushmootCharge : public CSfx
{
public:
	int	m_nEventPos;
	CSfxMushmootCharge() {};	
	~CSfxMushmootCharge() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WORLDSERVER
// �Ϲ� ����Ʈ : ��� ������
class CSfxGenNormalDmg01 : public CSfx
{
public:
	CSfxGenNormalDmg01();
	virtual ~CSfxGenNormalDmg01();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
#ifdef __CLIENT
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static	MemPooler<CSfxGenNormalDmg01>* m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenNormalDmg01::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenNormalDmg01::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenNormalDmg01::m_pPool->Free( (CSfxGenNormalDmg01*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenNormalDmg01::m_pPool->Free( (CSfxGenNormalDmg01*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};

// �شܽ�ų ����Ʈ : ��Ʈ��Ī01
class CSfxTroStretching01 : public CSfx
{
public:
	CSfxTroStretching01();
	virtual ~CSfxTroStretching01();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};
// �شܽ�ų ����Ʈ : ��Ʈ��Ī02
class CSfxTroStretching02 : public CSfx
{
public:
	CSfxTroStretching02();
	virtual ~CSfxTroStretching02();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};
// �شܽ�ų ����Ʈ : ���߰���
class CSfxTroBlitz : public CSfx
{
public:
	CSfxTroBlitz();
	virtual ~CSfxTroBlitz();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenSuperDmg01 : public CSfx
{
public:
	CSfxGenSuperDmg01();
	virtual ~CSfxGenSuperDmg01();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenSkillDmg01 : public CSfx
{
public:
	CSfxGenSkillDmg01();
	virtual ~CSfxGenSkillDmg01();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ���� �Ⱦ�
class CSfxGenMonsterDmg01 : public CSfx
{
public:
	CSfxGenMonsterDmg01();
	virtual ~CSfxGenMonsterDmg01();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ��Ȱ
class CSfxGenRestoration01 : public CSfx
{
public:
	CSfxGenRestoration01();
	virtual ~CSfxGenRestoration01();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ġ��
class CSfxGenCure: public CSfx
{
public:
	CSfxGenCure();
	virtual ~CSfxGenCure();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ��ȭ
class CSfxGenIncrease01 : public CSfx
{
public:
	CSfxGenIncrease01();
	virtual ~CSfxGenIncrease01();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ������
class CSfxGenLevelUp : public CSfx
{
public:
	CSfxGenLevelUp();
	virtual ~CSfxGenLevelUp();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : �α���
class CSfxGenLogin : public CSfx
{
public:
	CSfxGenLogin();
	virtual ~CSfxGenLogin();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ����
class CSfxGenWarp : public CSfx
{
public:
	CSfxGenWarp();
	virtual ~CSfxGenWarp();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : ���
class CSfxGenPcDie : public CSfx
{
public:
	CSfxGenPcDie();
	virtual ~CSfxGenPcDie();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : �� ������
class CSfxGenMonsterSpawn : public CSfx
{
public:
	CSfxGenMonsterSpawn();
	virtual ~CSfxGenMonsterSpawn();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : �̵���ġ
class CSfxGenMoveMark: public CSfx
{
public:
	CSfxGenMoveMark();
	virtual ~CSfxGenMoveMark();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
#ifdef __CLIENT
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenMoveMark>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenMoveMark::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenMoveMark::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenMoveMark::m_pPool->Free( (CSfxGenMoveMark*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenMoveMark::m_pPool->Free( (CSfxGenMoveMark*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};
// �Ϲ� ����Ʈ : ���� ����
class CSfxGenWaterCircle: public CSfx
{
public:
	CSfxGenWaterCircle();
	virtual ~CSfxGenWaterCircle();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
#ifdef __CLIENT
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenWaterCircle>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenWaterCircle::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenWaterCircle::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenWaterCircle::m_pPool->Free( (CSfxGenWaterCircle*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenWaterCircle::m_pPool->Free( (CSfxGenWaterCircle*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};

// �Ϲ� ����Ʈ : ���� ����
class CSfxGenRainCircle: public CSfx
{
public:
	CSfxGenRainCircle();
	virtual ~CSfxGenRainCircle();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
#ifdef __CLIENT
public:
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxGenRainCircle>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxGenRainCircle::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxGenRainCircle::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxGenRainCircle::m_pPool->Free( (CSfxGenRainCircle*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxGenRainCircle::m_pPool->Free( (CSfxGenRainCircle*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};

// �Ϲ� ����Ʈ : �� Ƣ���
class CSfxGenWaterCrown: public CSfx
{
public:
	CSfxGenWaterCrown();
	virtual ~CSfxGenWaterCrown();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CSfxMagicMiAtk1 : public CSfxShoot
{
public:
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxMagicMiAtk1();
	virtual ~CSfxMagicMiAtk1();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// �ϵ�� ������ ���� �ܰ躰�� 4����
// �Ϲ� ����Ʈ : �ϵ�1
class CSfxItemWandAtk1 : public CSfxShoot
{
public:
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxItemWandAtk1();
	virtual ~CSfxItemWandAtk1();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};

class CSfxItemRangeAtk1 : public CSfxShoot
{
public:
#if 1 
#ifdef __CLIENT
	CTailEffectBelt*	m_pTail;		// ������ ����Ʈ	
#endif
	FLOAT			m_fOriLen;
	D3DXVECTOR3		m_v3SrcPos;
#endif
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxItemRangeAtk1();
	virtual ~CSfxItemRangeAtk1();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

// �Ʒ��� �������� ȭ�� ����Ʈ
class CSfxItemRangeAtk1_Allow : public CSfxShoot
{
public:
	FLOAT	m_fSpeed;
	CSfxItemRangeAtk1_Allow();
	virtual ~CSfxItemRangeAtk1_Allow();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
class CSfxItemRangeAtk1_AllowRain : public CSfxShoot
{
public:
	int		m_nCount;
	CSfxItemRangeAtk1_AllowRain();
	virtual ~CSfxItemRangeAtk1_AllowRain();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
class CSfxItemRangeAtk1_Stone : public CSfxShoot
{
public:
	FLOAT	m_fSpeed;
	CSfxItemRangeAtk1_Stone();
	virtual ~CSfxItemRangeAtk1_Stone();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


class CSfxItemRangeAtk1_StoneRain : public CSfxShoot
{
public:
	int		m_nCount;
	CSfxItemRangeAtk1_StoneRain();
	virtual ~CSfxItemRangeAtk1_StoneRain();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxItemRangeAtk_JunkBow : public CSfxShoot
{
public:
#if 1 
#ifdef __CLIENT
	CTailEffectBelt*	m_pTail[3];		// ������ ����Ʈ	
#endif
	FLOAT			m_fOriLen;
	D3DXVECTOR3		m_v3SrcPos;
#endif
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.
	CSfxItemRangeAtk_JunkBow();
	virtual ~CSfxItemRangeAtk_JunkBow();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxItemYoyoAtk : public CSfxShoot
{
public:
#ifdef __CLIENT
	int				m_nMaxSpline;
	CTailEffectModel*	m_pTail;		// ������ ����Ʈ	
	D3DXVECTOR3		m_aSpline[30];
	int				m_nStep;
	int				m_nType;
	int				m_nCount;

	int				m_nTailIndex;
	CSfxModel		m_pSfxTailModel[20];
	void			ProcessTail();
	void			AddSfxTail( CSfxModel* pSfxModel );	// �¾����� ����Ʈ ���
	
#endif
	FLOAT			m_fMaxLength;
	D3DXVECTOR3		m_v3SrcPos;
	D3DXVECTOR3		m_vDelta;
	int				m_nDelayCount;
	
	

	
	FLOAT m_fCenter;		// Ÿ���� ��ǥ.y ����ġ.
	FLOAT m_fRadiusXZ;		// Ÿ���� XZ����� ������.

	void  MakePath(int nType);
	D3DXVECTOR3	SplineSlerp( D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3, D3DXVECTOR3 *v4, float fSlerp );
	
	CSfxItemYoyoAtk();
	virtual ~CSfxItemYoyoAtk();
	virtual void Process();
#ifndef __WORLDSERVER
	void	RenderTail( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// ���������� �߻��ϴ� ��Ÿ� źȯ��.
class CSfxAtkStraight : public CSfxShoot
{
	D3DXVECTOR3 m_vDelta;			// �̵� ������.
	DWORD		m_dwExplosion;		// �°��� ������ ���� ���̵�.
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxAtkStraight();
	virtual ~CSfxAtkStraight();
	virtual void Process();
	void ShootSfx( float fAngXZ, float fAngH, float fSpeed, DWORD dwExplosion );		// �߻�ü�� �̵��������� �������. 
	
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxItemWandAtkAir : public CSfxShoot
{
	D3DXVECTOR3		m_vDelta;		// ����.
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtkAir();
	virtual ~CSfxItemWandAtkAir();
	virtual void Process();
	virtual	int SetSfx( LPDIRECT3DDEVICE9 pd3dDevice, int nIndex, D3DXVECTOR3& vPosSrc, OBJID idSrc, D3DXVECTOR3& vPosDest, OBJID idDest, int nSec = 0 ); // ����� SFX ����
	virtual void ShootSfx( float fAngXZ, float fAngY, float fSpeed );
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// �Ϲ� ����Ʈ : �ϵ�2
class CSfxItemWandAtk2 : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk2();
	virtual ~CSfxItemWandAtk2();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : �ϵ�3
class CSfxItemWandAtk3 : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk3();
	virtual ~CSfxItemWandAtk3();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ϲ� ����Ʈ : �ϵ�4
class CSfxItemWandAtk4 : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxItemWandAtk4();
	virtual ~CSfxItemWandAtk4();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� ��ų : ����Ŀ��
class CSfxSkillVagOverCutter: public CSfx
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxSkillVagOverCutter();
	virtual ~CSfxSkillVagOverCutter();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ����� ��ų : Ŭ����Ʈ
class CSfxSkillVagCleanHit : public CSfx
{
public:
	CSfxSkillVagCleanHit();
	virtual ~CSfxSkillVagCleanHit();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ����� ��ų : �귣��
class CSfxSkillVagBrandish : public CSfx
{
public:
	CSfxSkillVagBrandish();
	virtual ~CSfxSkillVagBrandish();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};

// �Ӽųʸ� ��ų : Ų��
class CSfxSkillMerKeenWheel : public CSfx
{
public:
	CSfxSkillMerKeenWheel();
	virtual ~CSfxSkillMerKeenWheel();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ӽųʸ� ��ų : ���øŽ�
class CSfxSkillMerSplmash : public CSfx
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxSkillMerSplmash();
	virtual ~CSfxSkillMerSplmash();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ӽųʸ� ��ų : ����ε� ���̵�
class CSfxSkillMerBlindSide : public CSfx
{
public:
	CSfxSkillMerBlindSide();
	virtual ~CSfxSkillMerBlindSide();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ӽųʸ� ��ų : �ҹٸ���
class CSfxSkillMerPanBarrier : public CSfx
{
public:
	//CTimer m_timer;
	//BOOL m_bSfxObj2;
	//CSfxModel m_SfxObj2;
	CSfxSkillMerPanBarrier();
	virtual ~CSfxSkillMerPanBarrier();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// �Ӽųʸ� ��ų : �����ؼ�
class CSfxSkillMerProtection : public CSfx
{
public:
	//CTimer m_timer;
	//BOOL m_bSfxObj2;
	//CSfxModel m_SfxObj2;
	CSfxSkillMerProtection();
	virtual ~CSfxSkillMerProtection();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};


// ������ �ֹ��ܱ�
class CSfxSkillMagFireCasting : public CSfx
{
public:
	CSfxSkillMagFireCasting();
	virtual ~CSfxSkillMagFireCasting();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
class CSfxSkillMagWindCasting : public CSfx
{
public:
	CSfxSkillMagWindCasting();
	virtual ~CSfxSkillMagWindCasting();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};

// ��� ��Ŭ�߰�..
#ifdef __CLIENT

class CSfxSkillAssBurstcrack : public CSfx
{
public:
	CSfxSkillAssBurstcrack();
	virtual ~CSfxSkillAssBurstcrack();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxSkillAssTampinghole : public CSfx
{
public:
	CSfxSkillAssTampinghole();
	virtual ~CSfxSkillAssTampinghole();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxNpcDirSteam : public CSfx
{
public:
	BOOL m_fInit;

	CSfxNpcDirSteam();
	virtual ~CSfxNpcDirSteam();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

#endif // __CLIENT
	

// ������ ��ų : ��Ʈ������
class CSfxSkillMagStrongWind : public CSfxShoot
{
	float m_fAngle;	
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagStrongWind();
	virtual ~CSfxSkillMagStrongWind();
//	int SetSfx( LPDIRECT3DDEVICE9 pd3dDevice, int nIndex, D3DXVECTOR3& vPosSrc, OBJID idSrc, D3DXVECTOR3& vPosDest, OBJID idDest, int nSec );
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ������ ��ų : �ҵ�����
class CSfxSkillMagSwordWind : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagSwordWind();
	virtual ~CSfxSkillMagSwordWind();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ������ ��ų : ���̾�θ޶�
class CSfxSkillMagFireBoomerang : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagFireBoomerang();
	virtual ~CSfxSkillMagFireBoomerang();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ������ ��ų : ���̾��
class CSfxSkillMagFireBomb : public CSfx
{
public:
	CSfxSkillMagFireBomb();
	virtual ~CSfxSkillMagFireBomb();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
// ������ ��ų : �ֿ���
class CSfxSkillMagHotAir : public CSfxShoot
{
	int m_nDmgCnt;		// ���ӵ������� ������ ī��Ʈ. ������ ���϶����� �ϳ��� �ö󰣴�.
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	CSfxSkillMagHotAir();
	virtual ~CSfxSkillMagHotAir();
	virtual void Process();
	#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	#endif	// __WORLDSERVER
};
#endif	// __WORLDSERVER



// ������ ��ų : ���̽��̻���
class CSfxSkillMagIceMissile : public CSfxShoot
{
public:
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagIceMissile();
	virtual ~CSfxSkillMagIceMissile();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

// ������ ��ų : ����Ʈ�� ��
class CSfxSkillMagLightningBall : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagLightningBall();
	virtual ~CSfxSkillMagLightningBall();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// ������ ��ų : ������ũ ����
class CSfxSkillMagSpikeStone : public CSfxShoot
{
public:
	//CSfxModel m_SfxObj2; // ���߽� ����Ʈ ǥ�ÿ�
	BOOL m_bHit; // �����ߴ����� ����
	CSfxSkillMagSpikeStone();
	virtual ~CSfxSkillMagSpikeStone();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


#define MAX_SHOOTWAVE_TAIL		8
// �ٴڿ� �پ ���ư��� ����Ʈ.
class CSfxShootWave : public CSfxShoot
{
public:
	BOOL	m_bHit; // �����ߴ����� ����
	BOOL	m_bDir;
	DWORD	m_dwSndHit;		// �¾����� �Ҹ�.
	FLOAT	m_fHitScale;	// ���߾� �����ϸ�.
	D3DXVECTOR3 m_vTail[MAX_SHOOTWAVE_TAIL];	// �������� ��ǥ�� ���ʷ� ����.
	int		m_nTailFrame[ MAX_SHOOTWAVE_TAIL ];	// ������ ��ȣ.

	CSfxShootWave();
	virtual ~CSfxShootWave();
	
	void	SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale = 1.0f );	// �¾����� ����Ʈ ���
	void	SetDir( BOOL bDir );		// ���⼺�ִ� sfx �ΰ�.
	void	SetSndHit( DWORD dwSndHit ) { m_dwSndHit = dwSndHit; }
	
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};


// ������ǥ ���� ���� sfx
class CSfxFixed : public CSfx
{
	float m_fAngle;
public:
	CSfxFixed();
	virtual ~CSfxFixed();
	virtual void Process();
#ifndef __WORLDSERVER
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
#endif	// __WORLDSERVER
};

class CSfxPartsLink : public CSfx	// ��������Ǵ� ����Ʈ		
{
public:
	BOOL	m_bEndFrame;
	int		m_nPartsLink;		// ��ũ�� ��ġ. 0:������ 1:�޼�
	CSfxPartsLink() 
	{ 
		m_bEndFrame = FALSE;
		m_nPartsLink = 0; 
	}
	~CSfxPartsLink() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void SetPartLink( int nPart ) { m_nPartsLink = nPart; }
	
};

// ������ ��ũ�Ǵ� sfx(���̵��� �տ� ���)
class CSfxPartsLinkBlade : public CSfxPartsLink		// ���̵� ��Ʈ ��ũ ���� : �и� ����� ������ �Ҹ�
{
public:
	CSfxPartsLinkBlade() {};
	~CSfxPartsLinkBlade() {};
	
	void Process();
};

class CSfxPartsLinkJst : public CSfxPartsLink		// ������ ���� ��ũ ���� : ����Ʈ�� EndFrame�̸� �Ҹ�
{
public:
	CSfxPartsLinkJst() {};
	~CSfxPartsLinkJst() {};
	
	void Process();
};



// ������ ��ũ�Ǵ� sfx(��ũ�ι��� �տ� ���)
class CSfxAllowPartsLink : public CSfx
{
public:
	int		m_nPartsLink;		// ��ũ�� ��ġ. 0:������ 1:�޼�
	CSfxAllowPartsLink() { m_nPartsLink = 0; }
	~CSfxAllowPartsLink() {};
	
	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual void SetPartLink( int nPart ) { m_nPartsLink = nPart; }
	
};

#ifndef __WORLDSERVER
class CSfxPartsLinkShoulder : public CSfxPartsLink		// ���̵� ��Ʈ ��ũ ���� : �и� ����� ������ �Ҹ�
{
public:
	int				m_nOldAbilityOption;
	D3DXVECTOR3		m_v3Offset;

	CSfxPartsLinkShoulder();
	~CSfxPartsLinkShoulder();

	void Process();
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );
};
#endif //__WORLDSERVER

// ������ ���� ����Ʈ
class CSfxReady : public CSfx
{
public:
	CSfxReady() {}
	~CSfxReady() {}

	void Process();
};

#if __VER >= 12 // __ITEMCREATEMON_S0602
class CSfxCursor : public CSfx
{
public:
	CSfxCursor();
	virtual ~CSfxCursor();
	virtual void Process();
};
#endif // __ITEMCREATEMON_S0602

class CSfxLinkMover : public CSfx			
{
public:
	//gmpbigsun : ������ �پ� �ٴϴ� �Ϲ����� ��Ȳ�����
	CSfxLinkMover( );
	virtual ~CSfxLinkMover( );
	virtual void Process( );
	virtual void Render( LPDIRECT3DDEVICE9 pd3dDevice );

protected:
	DWORD  _idSFX;
};

#endif // !defined(AFX_SFX_H__D652787A_4E67_419F_AB52_0A8E2FED08AE__INCLUDED_)
