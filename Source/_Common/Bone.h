#ifndef	__BONE_H__
#define	__BONE_H__

#include <d3dx9.h>
#include "file.h"

#define		MAX_VS_BONE		28		// VS���� �ִ��밡���� ���뷹�������� ����.

struct BONE;
struct BONE
{
	BONE		*m_pParent;			// ���� �θ� ���

//	int			m_nID;				// �������� ��� �ο��� ��ȣ - ������ �ʿ������ ����.
	int			m_nParentIdx;		// �θ� �ε���
	char		m_szName[32];		// * ��Ʈ�������� ��� �̸�
	D3DXMATRIX		m_mTM;		// ase���� TM
	D3DXMATRIX		m_mInverseTM;	// * �̸� ��ȯ�� �ι���TM 
	D3DXMATRIX		m_mLocalTM;	// * �̸���ȯ�� ���� TM
};

#define MAX_MDL_EVENT		8	// #event xx

//
// ����Ʈ�� ����
class CBones
{
public:
	int		m_nID;				// ���� ���� ID(���ϳ��� �ִ�)
	char	m_szName[64];		// ���� ���ϸ� - ���߿� �̰� ������.
	int		m_nMaxBone;			// ���� ����
	BONE	*m_pBones;			// ���� ����Ʈ
	int		m_bSendVS;			// �������� MAX_VS_BONE���� ������...

	D3DXMATRIX	m_mLocalRH;		// ������ ������ RHand�κ����� ���TM
	D3DXMATRIX	m_mLocalLH;		// ������ ������ LHand�κ����� ���TM
	D3DXMATRIX	m_mLocalShield;	// ������ LForeArm���κ����� ���TM
	D3DXMATRIX	m_mLocalKnuckle;	// ��Ŭ.
	int		m_nRHandIdx;		// ������ �ε���
	int		m_nLHandIdx;		// �޼� �ε���
	int		m_nRArmIdx;			// ������ �ȶ�
	int		m_nLArmIdx;			// �޼� �ȶ� �ε���.
	D3DXVECTOR3	m_vEvent[MAX_MDL_EVENT];	
//	BONE	*m_pEventParent[MAX_MDL_EVENT];	// �̺�Ʈ ��ǥ�� �θ�
	int		m_nEventParentIdx[MAX_MDL_EVENT];	// �̺�Ʈ��ǥ�� �θ��ε���.
	
private:
	void Init( void );
	void Destroy( void );

public:
	CBones();
	~CBones();

//	BONE	*FindBone( int nID );								// nID���� ������ ���� ã��
	BONE	*GetBone( int nIdx ) { return &m_pBones[ nIdx ]; }

	int		LoadBone( LPCTSTR szFileName );		// ������ �� ������ ����
};


#if __VER < 12 // __LEAK_0827
#define		MAX_BONES			128
#endif	// __LEAK_0827

// read only bone database
class CBonesMng
{
public:
#if __VER >= 12 // __LEAK_0827
	map<string, CBones*>	m_mapBones;
#else	// __LEAK_0827
	int			m_nSize;							// �����ڰ� �ε��ؼ� ������ �ִ� ���� ����� �󸶳� �Ǵ°�.
	int			m_nMax;								// ����
	CBones *m_pBonesAry[ MAX_BONES ];
#endif	// __LEAK_0827

//#ifdef __WORLDSERVER
//	CRIT_SEC	m_AccessLock;
//#endif	// __WORLDSERVER

	CBonesMng();
	~CBonesMng();

	void	Init( void );
	void	Destroy( void );

	CBones *LoadBone( LPCTSTR szFileName );									// Bone������ �о �޸𸮿� �����Ų��.

};

extern CBonesMng		g_BonesMng;


//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define		MA_HIT			(0x00000001)		// Ÿ��
#define		MA_SOUND		(0x00000002)		// �Ҹ�
#define		MA_QUAKE		(0x00000004)		// ����.
struct TM_ANIMATION
{
//	D3DXMATRIX			m_mAniTM;
	D3DXQUATERNION		m_qRot;
	D3DXVECTOR3			m_vPos;
};

// ����� ���� ������ ���谡 �ִ�.  ���� ũ�Ⱑ ����, ���� ������ ���� ����� �ƴϸ� ������ �� �����ִ�.
// ĳ���� �����ϳ��� ���� Ŭ����
struct	BONE_FRAME
{
	TM_ANIMATION	*m_pFrame;
	D3DXMATRIX		m_mLocalTM;
};

// �� �������� �Ӽ���
struct	MOTION_ATTR
{
	DWORD	m_dwAttr;
//	char	m_szFileName[32];
	int		m_nSndID;		// ���� ���ҽ� ���̵�
	float	m_fFrame;		// �Ҽ��� ������ ��Ȯ�� ������ ��ȣ 
};

class CMotion
{
private:
	TM_ANIMATION	*m_pMotion;			// ���������� ������ �ִϸ��̼��� ����ִ�.  �ִϰ� ���� ����� LocalTM�� �ִ�.
	MOTION_ATTR		*m_pAttr;			// ������ũ�⸸ŭ �Ҵ�.  �� �����ӼӼ�.
	int		m_nMaxFrame;				// ��� �����Ӽ�
public:
	int		m_nID;						// ����� ���� ID(���ϳ��� �ִ�)
	char	m_szName[64];				// ��� ���ϸ� - ���߿� �̰� ������.
	float	m_fPerSlerp;				//
	int		m_nMaxBone;					// �̰��� �������� Ȯ�ο� �����ϻ��̴�.  ���� ���� ������ �ʿ��ϴٸ� CBone���� �о�� �Ѵ�.
	D3DXVECTOR3	m_vPivot;				// 0,0,0(��)���ؿ��� ���(��Ʈ)�� ��ǥ.  0��Ű�� ��ǥ�� ����.
	BONE_FRAME	*m_pBoneFrame;		// ���밹�� ��ŭ �Ҵ�. �ε��� ������ ����ϱ� ������ �����͸� ���ġ�Ѵ�.  �������� ���°��� NULL
	BONE		*m_pBoneInfo;		// ���뱸������
	int		m_nMaxEvent;
	D3DXVECTOR3	m_vEvent[4];
	D3DXVECTOR3		*m_pPath;			// path
	
	CMotion();
	~CMotion();

	void Init( void );
	void Destroy( void );

	MOTION_ATTR *GetMotionAttr( int nNumFrm ) { return m_pAttr + nNumFrm; }	
	DWORD	GetAttr( int nNumFrm ) 
	{ 
		return m_pAttr[ nNumFrm ].m_dwAttr; 
	}
//#if 0
	DWORD	IsAttrHit( float fOldFrm, float fNumFrm ) 
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_HIT )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr->m_dwAttr;
			return 0;
	}
	MOTION_ATTR *IsAttrSound( float fOldFrm, float fNumFrm )
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_SOUND )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr;
			return NULL;
	}
	DWORD	IsAttrQuake( float fOldFrm, float fNumFrm ) 
	{ 
		MOTION_ATTR		*pAttr = &m_pAttr[ (int)fNumFrm ];
		if( pAttr->m_dwAttr & MA_QUAKE )
			if( fOldFrm < pAttr->m_fFrame && pAttr->m_fFrame <= fNumFrm )	// ���� �������̶� ���� ������ ���̿� Ÿ���� ����־��°�.
				return pAttr->m_dwAttr;
			return 0;
	}
	void	SetAttr( float fNumFrm, DWORD dwAttr ) 
	{
		m_pAttr[ (int)fNumFrm ].m_dwAttr |= dwAttr;
		m_pAttr[ (int)fNumFrm ].m_fFrame = fNumFrm;
	}
	void	ResetAttr( int nNumFrm, DWORD dwAttr ) { m_pAttr[ nNumFrm ].m_dwAttr &= (~dwAttr); }
	TM_ANIMATION*	GetFrame( int nNumFrm ) { return &m_pMotion[nNumFrm]; }
	int		GetMaxFrame( void ) { return m_nMaxFrame; }

	int		LoadMotion( LPCTSTR strFileName );			// ���Ͽ��� ��������� ����
	void	ReadTM( CResFile *file, int nNumBone, int nNumFrame );

	void	AnimateBone( D3DXMATRIX *pmUpdateBone, CMotion *pMotionOld, float fFrameCurrent, int nNextFrame, int nFrameOld, BOOL bMotionTrans, float fBlendWeight );
	
};

#if __VER < 12 // __LEAK_0827
#define		MAX_MOTION					2048
#define		MAX_MOTION_CACHE		16
#endif	// __LEAK_0827

// read only motion database
class CMotionMng
{
public:
#if __VER >= 12 // __LEAK_0827
	map<string, CMotion*>	m_mapMotions;
#else	// __LEAK_0827
	int		m_nSize;
	int		m_nMax;
	int		m_nCachePos;				// ĳ�� ť �ε���
	CMotion		*m_pCache[ MAX_MOTION_CACHE ];			// �ֱٿ� ���� ������ �����͸� ������ ����
	CMotion		*m_pMotionAry[ MAX_MOTION ];
#endif	// __LEAK_0827
	
	CMotionMng();
	~CMotionMng();

	void	Init( void );
	void	Destroy( void );

	CMotion	*LoadMotion( LPCTSTR strFileName );			// ������ �޸𸮿� �����Ѵ�.  �ߺ��ؼ� �ø��� �ʴ´�.
};

extern CMotionMng		g_MotionMng;




#endif
