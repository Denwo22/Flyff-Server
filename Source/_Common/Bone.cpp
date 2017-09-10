#include "stdafx.h"
#include <stdio.h>
#include "model.h"
#include "Bone.h"



CBonesMng		g_BonesMng;

////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////
CBones :: CBones()
{
	Init();
}

CBones :: ~CBones()
{
	Destroy();
}

void	CBones :: Init( void )
{
	m_nID = 0;
	memset( m_szName, 0, sizeof(m_szName) );
	m_nMaxBone = 0;
	m_pBones = NULL;
	m_nRHandIdx = m_nLHandIdx = m_nRArmIdx = m_nLArmIdx = 0;

	D3DXMatrixIdentity( &m_mLocalRH );
	D3DXMatrixIdentity( &m_mLocalLH );
	D3DXMatrixIdentity( &m_mLocalShield );
	D3DXMatrixIdentity( &m_mLocalKnuckle );

	memset( m_vEvent, 0, sizeof(m_vEvent) );
//	memset( m_pEventParent, 0, sizeof(m_pEventParent) );
	memset( m_nEventParentIdx, 0, sizeof(m_nEventParentIdx) );

}

void	CBones :: Destroy( void )
{
	SAFE_DELETE_ARRAY( m_pBones );
	Init();
}

//
//  ������߿��� nID���� ������ ���븦 ã��
//
/*
BONE	*CBones :: FindBone( int nID )
{
	int		i;
	BONE	*pBone = m_pBones;

	i = m_nMaxBone;
	while( i-- )
	{
		if( pBone->m_nID == nID )	return pBone;
		pBone ++;
	}
	
	return NULL;
}
*/

//
//
//
int		CBones :: LoadBone( LPCTSTR szFileName )
{
	int		nNumBone;
	int		i;
	int		nLen;
	int		nVer;

	CResFile resFp;
	BOOL bRet = resFp.Open( MakePath( DIR_MODEL, szFileName ), "rb" );	
	if( bRet == FALSE )		
		return	FAIL;

	resFp.Read( &nVer, 4, 1 );		// version
	if( nVer < VER_BONE )
	{
		Error( "%s�� ������ %d.  �ֽŹ����� %d", szFileName, nVer, VER_BONE );
		resFp.Close();
		return FAIL;
	}

	// ID�б⸦ ����.
	resFp.Read( &m_nID, 4, 1 );

	if( strlen( szFileName ) > sizeof(m_szName) )
	{
		Error( "%s : file name is too long", szFileName );
		resFp.Close();
		return FAIL;
	}

	// ���ϸ� ī��
	strcpy( m_szName, szFileName );

	resFp.Read( &nNumBone, 4, 1 );			// �� ���� ����
	m_nMaxBone = nNumBone;
	m_pBones = new BONE[ nNumBone ];			// �� ���� ��ŭ �Ҵ�
	memset( m_pBones, 0, sizeof(BONE) * nNumBone );		// zero clear

	for( i = 0; i < nNumBone; i ++ )
	{
		resFp.Read( &nLen, 4, 1 );		// �� ���Ա���
		resFp.Read( m_pBones[i].m_szName,		nLen, 1 );		// bone node �̸� ����
		resFp.Read( &m_pBones[i].m_mTM,			sizeof(D3DXMATRIX), 1 );			// World(Object) TM
		resFp.Read( &m_pBones[i].m_mInverseTM,	sizeof(D3DXMATRIX), 1 );			// Inv NODE TM
		resFp.Read( &m_pBones[i].m_mLocalTM,		sizeof(D3DXMATRIX), 1 );			// LocalTM
		resFp.Read( &m_pBones[i].m_nParentIdx,	4, 1 );								// parent bone index
		if( strcmpi( m_pBones[i].m_szName + 6, "r hand") == 0 )	// �θ��̸��� R Hand��...
			m_nRHandIdx = i;
		if( strcmpi( m_pBones[i].m_szName + 6, "l hand") == 0 )	// �θ��̸��� L Hand��...
			m_nLHandIdx = i;
		if( strcmpi(m_pBones[i].m_szName + 6, "l forearm") == 0 )	// �θ��̸��� L ForeArm��...
			m_nLArmIdx = i;
		if( strcmpi(m_pBones[i].m_szName + 6, "r forearm") == 0 )	// �θ��̸��� R ForeArm��...
			m_nRArmIdx = i;
	}

	resFp.Read( &m_bSendVS, 4, 1 );
	
	// ������ ������� ��ġ�� ���
	resFp.Read( &m_mLocalRH, sizeof(D3DXMATRIX), 1 );
	resFp.Read( &m_mLocalShield, sizeof(D3DXMATRIX), 1 );
	resFp.Read( &m_mLocalKnuckle, sizeof(D3DXMATRIX), 1 );
	
	// m_vEvent�� �߰��� ����.
//	int		nParentIdx[4];
	if( nVer == 5 )
	{
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * 4, 1 );
		resFp.Read( m_nEventParentIdx, sizeof(int) * 4, 1 );
	} else
	if( nVer >= 6 )
	{
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * MAX_MDL_EVENT, 1 );
		resFp.Read( m_nEventParentIdx, sizeof(int) * MAX_MDL_EVENT, 1 );
	}

	if( nVer == 7 )
	{
		resFp.Read( &m_mLocalLH, sizeof(D3DXMATRIX), 1 );
	}

	// �����ͷ� �Ҵ�.
//	for( i = 0; i < 4; i ++ )
//	{
//		m_pEventParent[i] = &m_pBones[ nParentIdx[i] ];
//	}

	resFp.Close();

	// �θ� �����͸� ����
	for( i = 0; i < nNumBone; i ++ )
	{
		if( m_pBones[i].m_nParentIdx == -1 )			// �θ� ������ �θ������ʹ� ��
			m_pBones[i].m_pParent = NULL;
		else
			m_pBones[i].m_pParent = &m_pBones[ m_pBones[i].m_nParentIdx ];
	}

	return SUCCESS;
}







////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////
CBonesMng :: CBonesMng()
{
#if __VER < 12 // __LEAK_0827
	Init();
#endif	// __LEAK_0827
}

CBonesMng :: ~CBonesMng()
{
	Destroy();
}


void	CBonesMng :: Init( void )
{
#if __VER < 12 // __LEAK_0827
	int		i;
	m_nSize = 0;
	m_nMax = 0;
	for( i = 0; i < MAX_BONES; i ++ )	m_pBonesAry[i] = NULL;
#endif	// __LEAK_0827
}

void	CBonesMng :: Destroy( void )
{
#if __VER >= 12 // __LEAK_0827
	for( map<string, CBones*>::iterator i = m_mapBones.begin(); i != m_mapBones.end(); ++i )
		safe_delete( i->second );
	m_mapBones.clear();
#else	// __LEAK_0827
	int		i;
	CBones	**pAry = m_pBonesAry;
	CBones	*pBone;

	for( i = 0; i < MAX_BONES; i ++ )
	{
		pBone = *pAry++;
		if( pBone )
		{
			SAFE_DELETE( pBone );
		}
	}
	Init();
#endif	// __LEAK_0827
}

// �� ����Load��û�� ������ ������ �о� �޸𸮿� �����Ѵ�.
// �����Ҷ� �ߺ��Ȱ��� �̹� �ε��Ǿ��� ���� �����͸� �����Ѵ�.
// ������ ù�Ӹ��� ���� ���̵� �־ �˻��Ҷ� �����̸����� �������� ���̵�� �ϵ��� �Ѵ�.
CBones *CBonesMng :: LoadBone( LPCTSTR szFileName )
{
#if __VER >= 12 // __LEAK_0827
	char sFile[MAX_PATH]	= { 0,};
	strcpy( sFile, szFileName );
	strlwr( sFile );

	map<string, CBones*>::iterator i	= m_mapBones.find( sFile );
	if( i != m_mapBones.end() )
		return i->second;
	// �ε��Ȱ� �ƴϾ��ٸ�.  ������ ����Ÿ ����.
	CBones* pBones	= new CBones;
	if( pBones->LoadBone( szFileName ) == FAIL )
	{
		safe_delete( pBones );
		return NULL;
	}
	bool bResult	= m_mapBones.insert( map<string, CBones*>::value_type( sFile, pBones ) ).second;
	return pBones;
#else	// __LEAK_0827
	int		i;
	CBones **pAry = m_pBonesAry;
	CBones *pBones;
	int		nIdx = -1;

	// �̹� �޸𸮿� ���� �Ǿ����� �˻�, ���ÿ� ����� �˻�
	for( i = 0; i < MAX_BONES; i ++ )
	{
		pBones = *pAry++;
		if( pBones )				// ���� �ƴѰ��� ���� ���̵� �ε��Ǿ� �ִٴ� ��
		{
			if( strcmpi(pBones->m_szName, szFileName) == 0 )		// ������ ã������ �װ� ����
//			if( pBones->m_nIDstrcmp(pBones->m_szName, szFileName) == 0 )		// ������ ã������ �װ� ����
				return pBones;
		}
		else
		{
			if( nIdx == -1 )		nIdx = i;			// ����� ������ ����ó�� ����� ���°��� ����ص�
		}
	}

	pBones = new CBones;
	// �ε��Ȱ� �ƴϾ��ٸ�.  ������ ����Ÿ ����.
	if( pBones->LoadBone( szFileName ) == FAIL )
		return NULL;

	// ���� �������͸� ����Ʈ�� ���
	m_pBonesAry[ nIdx ] = pBones;

	m_nMax ++;			// ���� �� ���� ����

	return pBones;		// ���� �� ������ ����
#endif	// __LEAK_0827
}

////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////
CMotionMng		g_MotionMng;


CMotionMng :: CMotionMng()
{
#if __VER < 12 // __LEAK_0827
	Init();
#endif	// __LEAK_0827
}

CMotionMng :: ~CMotionMng()
{
	Destroy();
}

void	CMotionMng :: Init( void )
{
#if __VER < 12 // __LEAK_0827
	int		i;
	m_nSize = 0;
	m_nMax = 0;
	m_nCachePos = 0;

	for( i = 0; i < MAX_MOTION; i ++ )
		m_pMotionAry[i] = NULL;
	
	for( i = 0; i < MAX_MOTION_CACHE; i ++ )
		m_pCache[i] = NULL;
#endif	// __LEAK_0827
}

void	CMotionMng :: Destroy( void )
{
#if __VER >= 12 // __LEAK_0827
	for( map<string, CMotion*>::iterator i = m_mapMotions.begin(); i != m_mapMotions.end(); ++i )
		safe_delete( i->second );
	m_mapMotions.clear();
#else	// __LEAK_0827
	int		i;
	CMotion		**pAry = m_pMotionAry;
	CMotion		*pMotion;

	for( i = 0; i < MAX_MOTION; i ++ )
	{
		pMotion = *pAry++;
		if( pMotion )
		{
			SAFE_DELETE( pMotion );
		}
	}
	Init();
#endif	// __LEAK_0827
}

//
//
//
CMotion *CMotionMng :: LoadMotion( LPCTSTR szFileName )
{
#if __VER >= 12 // __LEAK_0827
	char sFile[MAX_PATH]	= { 0,};
	strcpy( sFile, szFileName );
	strlwr( sFile );

	map<string, CMotion*>::iterator i	= m_mapMotions.find( sFile );
	if( i != m_mapMotions.end() )
		return i->second;

	// �ε��Ȱ� �ƴϾ��ٸ�.  ������ ����Ÿ ����.
	CMotion* pMotion	= new CMotion;
	if( pMotion->LoadMotion( szFileName ) == FAIL )
	{
		safe_delete( pMotion );
		return NULL;
	}
	bool bResult	= m_mapMotions.insert( map<string, CMotion*>::value_type( sFile, pMotion ) ).second;
	return pMotion;
#else	// __LEAK_0827
	int		i;
	CMotion **pAry, *pMotion;
	int		nIdx = -1;

	// �̹� �޸𸮿� ���� �Ǿ����� �˻�, ���ÿ� ����� �˻�
	// �ϴ� ĳ���� �˻�
	pAry = m_pCache;
	for( i = 0; i < MAX_MOTION_CACHE; i ++ )
	{
		pMotion = *pAry++;
		if( pMotion )
		{
			if( strcmpi(pMotion->m_szName, szFileName) == 0 )		// ������ ã������ �װ� ����
				return pMotion;
		}
	}

	// ĳ������ ��ã������ ����Ʈ���� �˻�
	pAry = m_pMotionAry;
	for( i = 0; i < MAX_MOTION; i ++ )
	{
		pMotion = *pAry++;
		if( pMotion )				// ���� �ƴѰ��� ���� ����̵� �ε��Ǿ� �ִٴ� ��
		{
			if( strcmpi(pMotion->m_szName, szFileName) == 0 )		// ������ ã������ �װ� ����
				return pMotion;
		} else
		{
			if( nIdx == -1 )		nIdx = i;			// ����� ������ ����ó�� ����� ���°��� ����ص�
		}
	}
	if( nIdx == -1 )	
	{
		Error( "%s : ���� �� �ִ� �Ѱ踦 �ʰ��ߴ�", szFileName );
	}

	pMotion = new CMotion;
	// �ε��Ȱ� �ƴϾ��ٸ�.  ������ ����Ÿ ����.
	if( pMotion->LoadMotion( szFileName ) == FAIL )
	{
		safe_delete( pMotion );
		return NULL;
	}
	// ���� ��������͸� ����Ʈ�� ���
	m_pMotionAry[ nIdx ] = pMotion;
	
	// ĳ�ÿ��� ����
	m_pCache[ m_nCachePos++ ] = pMotion;	
	if( m_nCachePos >= MAX_MOTION_CACHE )		m_nCachePos = 0;

	m_nMax ++;			// ���� ��� ���� ����

	return pMotion;		// ���� ��� ������ ����
#endif	// __LEAK_0827
}


////////////////////////////////////////////////////////////////////////////////////
///////////////
///////////////
///////////////
///////////////
///////////////
///////////////
////////////////////////////////////////////////////////////////////////////////////

CMotion :: CMotion()
{
	Init();
}

CMotion :: ~CMotion()
{
	Destroy();
}

void	CMotion :: Init( void )
{
	m_pMotion = NULL;
	m_pPath = NULL;
	m_pBoneFrame = NULL;
	m_fPerSlerp = 0.5f;
	m_nMaxEvent = 0;
	m_pBoneInfo = NULL;
	memset( m_vEvent, 0, sizeof(m_vEvent) );
	m_pAttr = NULL;
	m_pBoneInfo = NULL;
}

void	CMotion :: Destroy( void )
{
	int		i;

	SAFE_DELETE_ARRAY( m_pAttr );
	SAFE_DELETE_ARRAY( m_pMotion );
	SAFE_DELETE_ARRAY( m_pPath );
	if( m_pBoneFrame )
	{
		for( i = 0; i < m_nMaxBone; i ++ )
			m_pBoneFrame[i].m_pFrame = NULL;
	}
	SAFE_DELETE_ARRAY( m_pBoneFrame );
	SAFE_DELETE_ARRAY( m_pBoneInfo );

	Init();
}

//
//
//
int		CMotion :: LoadMotion( LPCTSTR szFileName )
{
	int		nNumBone;
	int		nNumFrame;
//	int		nNumSize;
//	int		i, j;
	int		nVer;

	CResFile resFp;
	BOOL bRet = resFp.Open( MakePath( DIR_MODEL, szFileName ), "rb" );
	if( bRet == FALSE )	
	{
		LPCTSTR szStr = Error( "%s : ã�� �� ����", szFileName );
		//ADDERRORMSG( szStr );
		resFp.Close();
		return FAIL;
	}

	resFp.Read( &nVer, 4, 1 );		// version
	if( nVer != VER_MOTION )
	{
		Error( "%s�� ������ %d.  �ֽŹ����� %d", szFileName, nVer, VER_MOTION );
		resFp.Close();
		return FAIL;
	}

	// ID�б⸦ ����.
	resFp.Read( &m_nID, 4, 1 );

	if( strlen( szFileName ) > sizeof(m_szName) )
	{
		Error( "%s : file name is too long", szFileName );
		resFp.Close();
		return FAIL;
	}

	// ���ϸ� ī��
	strcpy( m_szName, szFileName );
	
	resFp.Read( &m_fPerSlerp, sizeof(float), 1 );		// 
	resFp.Seek( 32, SEEK_CUR );		// reserved

	resFp.Read( &nNumBone, 4, 1 );			// ���� ���� ����
	resFp.Read( &nNumFrame, 4, 1 );		// �ִϸ��̼� ������ ���� ����
	m_nMaxFrame = nNumFrame;
	m_nMaxBone = nNumBone;

	if( nNumFrame <= 0 )
	{
		Error( "CMotion::LoadMotion : %s read MaxFrame = %d", szFileName, nNumFrame );
		resFp.Close();
		return FAIL;
	}

	// path 
	int nTemp;
	resFp.Read( &nTemp, 4, 1 );	// path������ �ִ°�?
	if( nTemp )
	{
		m_pPath = new D3DXVECTOR3[ nNumFrame ];
		resFp.Read( m_pPath, sizeof(D3DXVECTOR3) * nNumFrame, 1 );		// nNumFrame��ŭ �ѹ濡 �о������.
	}

	//
	// TM_Animation ����Ÿ ����.
	ReadTM( &resFp, nNumBone, nNumFrame );
	
	// ������ �Ӽ� ����.
	resFp.Read( m_pAttr, sizeof(MOTION_ATTR) * nNumFrame, 1 );

	resFp.Read( &m_nMaxEvent, 4, 1 );	// �̺�Ʈ ��ǥ ����
	if( m_nMaxEvent > 0 )
		resFp.Read( m_vEvent, sizeof(D3DXVECTOR3) * m_nMaxEvent, 1 );
	

	resFp.Close();

//	m_vPivot = m_pMotion[0].m_vPos;			// Bip01�� POS���� ����.  ����� ���� ��ǥ��.

	return SUCCESS;
}

//
//  Motion�� core�κ��� ����. �ܺο��� �ܵ� ȣ�� ����.
//
void	CMotion :: ReadTM( CResFile *file, int nNumBone, int nNumFrame )
{
	int		nNumSize;
	int		nFrame;
	int		i;

	m_nMaxBone = nNumBone;		// LoadMotion()���� �ҷ����ٸ� �̺κ��� �ʿ������ ReadTM�� ���� �Ҹ� ���� ������ �̰� �ʿ��ϴ�.
	m_nMaxFrame = nNumFrame;

	//--- ���뱸������ ����.
	m_pBoneInfo = new BONE[ nNumBone ];			// �� ���� ��ŭ �Ҵ�
	memset( m_pBoneInfo, 0, sizeof(BONE) * nNumBone );		// zero clear
	
	int		nLen;
	for( i = 0; i < nNumBone; i ++ )
	{
		file->Read( &nLen, 4, 1 );
		if( nLen > 32 )		
			Error("CMotion::ReadTM - %s bonename is too long", m_szName );

		file->Read( m_pBoneInfo[i].m_szName,	nLen, 1 );		// bone node �̸�
		file->Read( &m_pBoneInfo[i].m_mInverseTM,	sizeof(D3DXMATRIX), 1 );			// Inv NODE TM
		file->Read( &m_pBoneInfo[i].m_mLocalTM,		sizeof(D3DXMATRIX), 1 );			// LocalTM
		file->Read( &m_pBoneInfo[i].m_nParentIdx,	4, 1 );								// parent bone index
	}
	
	// �θ� �����͸� ����
	for( i = 0; i < nNumBone; i ++ )
	{
		if( m_pBoneInfo[i].m_nParentIdx == -1 )			// �θ� ������ �θ������ʹ� ��
			m_pBoneInfo[i].m_pParent = NULL;
		else
			m_pBoneInfo[i].m_pParent = &m_pBoneInfo[ m_pBoneInfo[i].m_nParentIdx ];
	}
	

	file->Read( &nNumSize, 4, 1 );			// ������ ������ ���� - �޸� Ǯ ������
	//--- ��� ����.
	m_pMotion		= new TM_ANIMATION[ nNumSize ];		// �޸� Ǯ
	m_pBoneFrame	= new BONE_FRAME[ nNumBone ];
	m_pAttr			= new MOTION_ATTR[ nNumFrame ];
	memset( m_pAttr, 0, sizeof(MOTION_ATTR) * nNumFrame );	// nNumSize���µ� nNumFrame�� �´°� ����.
	TM_ANIMATION	*p = m_pMotion;
	int		nCnt = 0;
	
	// ���� �� ��ŭ ����
	for( i = 0; i < nNumBone; i ++ )
	{
		file->Read( &nFrame, 4, 1 );
		if( nFrame == 1 )		// 1�̸� ���� ���뿡 ������ ����
		{
			m_pBoneFrame[i].m_pFrame = p;
			file->Read( m_pBoneFrame[i].m_pFrame, sizeof(TM_ANIMATION) * nNumFrame, 1 );		// �ѹ濡 �о������.
			p += nNumFrame;
			nCnt += nNumFrame;
//			for( j = 0; j < nNumFrame; j ++ )
//			{
//				file->Read( &(p->m_mAniTM), sizeof(D3DXMATRIX), 1 );
//				file->Read( &(p->m_qRot), sizeof(D3DXQUATERNION), 1 );
//				file->Read( &(p->m_vPos), sizeof(D3DXVECTOR3), 1 );
//				p ++;
//				nCnt ++;
//			}
		} else			// ���� ���뿡 ������ ����
		{
			file->Read( &(m_pBoneFrame[i].m_mLocalTM), sizeof(D3DXMATRIX), 1 );			// �������� ������ LocalTM�� �а�
			m_pBoneFrame[i].m_pFrame = NULL;
			// m_mLocalTM�� �־����Ƿ� �޸� Ǯ���� �����ʿ� ����.
		}
	}
	
	if( nCnt != nNumSize )
	{
		Error( "%s : frame size error", m_szName );
	}	
}

//
// ������ �ִϸ��̼�
// 
void	CMotion :: AnimateBone( D3DXMATRIX *pmUpdateBone, CMotion *pMotionOld, float fFrameCurrent, int nNextFrame, int nFrameOld, BOOL bMotionTrans, float fBlendWeight )
{
	int		i;
	BONE	*pBone = m_pBoneInfo;		// �� ���� ���� ���� ������.
	TM_ANIMATION	*pFrame = NULL;					// ���� ������
	TM_ANIMATION	*pNext = NULL;					// ���� ������
	TM_ANIMATION	*pFrameOld = NULL;				// ��������� ������
//	D3DXMATRIX	*pMatBone = m_mUpdateBone;
	D3DXMATRIX	*pMatBones = pmUpdateBone;			// ������Ʈ ��Ʈ���� ����Ʈ ���� ������
	int			nCurrFrame;
	float		fSlp;
	D3DXQUATERNION		qSlerp;
	D3DXVECTOR3			vSlerp;
	BONE_FRAME	*pBoneFrame = NULL;
	BONE_FRAME	*pBoneFrameOld = NULL;		// ���� ��������
	D3DXMATRIX	m1, m2;

	if( pMotionOld == NULL )	// pMotionOld-> �Ҷ� ������ ������ �ȳ�����.
		pMotionOld = this;

	// �ִϸ��̼� ������ �ִ� ���
	i = m_nMaxBone;
	// ������ ���� Slerp ���.
	nCurrFrame = (int)fFrameCurrent;				// �Ҽ��� ������ �����θ�..
	fSlp = fFrameCurrent - (float)nCurrFrame;	// �Ҽ��� �κи� ������
	/////////////////////////////

//	int	nNextFrame = GetNextFrame();
	pBoneFrame	  = m_pBoneFrame;
	pBoneFrameOld = pMotionOld->m_pBoneFrame;		// ���������� ���� ����
	// ���� ������ŭ ���� ����
	while( i-- )
	{
		if( pBoneFrameOld->m_pFrame )
			pFrameOld = &pBoneFrameOld->m_pFrame[nFrameOld];		// �ϴ��� 0���� �Ѵ�.  ��������� nCurrFrame�� ����ϰ� �־�� �Ѵ�.
		else
			pFrameOld = NULL;		// �ϴ��� 0���� �Ѵ�.  ��������� nCurrFrame�� ����ϰ� �־�� �Ѵ�.
		if( pBoneFrame->m_pFrame )		// �� ���뿡 �������� ������
		{
			pFrame = &pBoneFrame->m_pFrame[ nCurrFrame ];		// ���� ������ ������ ����
			pNext  = &pBoneFrame->m_pFrame[ nNextFrame ];		// ���� ������ ������ ����

			D3DXQuaternionSlerp( &qSlerp, &pFrame->m_qRot, &pNext->m_qRot, fSlp );	// ������ ���ʹϾ� ����
			D3DXVec3Lerp( &vSlerp, &pFrame->m_vPos, &pNext->m_vPos, fSlp );		// ������ Pos���� ����

			if( bMotionTrans )	// ���� ��ǰ� ���� �ؾ� �ϴ°�
			{
				float	fBlendSlp = fBlendWeight;
				if( pFrameOld )
				{
					D3DXQuaternionSlerp( &qSlerp, &pFrameOld->m_qRot, &qSlerp, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &pFrameOld->m_vPos, &vSlerp, fBlendSlp );		// ������ Pos���� ����
				} else
				{
					D3DXQUATERNION	qLocal;
					D3DXVECTOR3		vLocal;
					D3DXMATRIX		*pmLocalTM = &pBoneFrameOld->m_mLocalTM;
					D3DXQuaternionRotationMatrix( &qLocal, pmLocalTM );
					vLocal.x = pmLocalTM->_41;
					vLocal.y = pmLocalTM->_42;
					vLocal.z = pmLocalTM->_43;
					D3DXQuaternionSlerp( &qSlerp, &qLocal, &qSlerp, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &vLocal, &vSlerp, fBlendSlp );		// ������ Pos���� ����
				}
			}

			// matAniTM���
			// �̵���� ����
			D3DXMatrixTranslation( &m1,	 vSlerp.x,  vSlerp.y,  vSlerp.z );
		
			// ���ʹϿ� ������ ȸ��Ű���� ȸ����ķ� ����
			D3DXMatrixRotationQuaternion( &m2, &qSlerp );
			m2 *= m1;		// �̵���� X ȸ����� = �ִ����

			if( pBone->m_pParent )
				m2 *= pMatBones[ pBone->m_nParentIdx ];		// �θ� ���ٸ� ��Ʈ�ε� �׵���İ� ���� �ʿ� ��� �����ߴ�.
//				else
//					m2 = m2 * *pmWorld;  // �̺κ��� �����ƴ�.  mCenter�� �׵����.
			
			// WorldTM = LocalTM * ParentTM;
			// ���� ��ȯ�� �����Ʈ������ ����� �־ Render()���� SetTransfrom��.
		} else
		// �� ���뿡 �������� ������.
		{
			if( bMotionTrans )	// ���� ��ǰ� ���� �ؾ� �ϴ°�
			{
				float	fBlendSlp = fBlendWeight;
				D3DXQUATERNION	qLocal;
				D3DXVECTOR3		vLocal;
				D3DXMATRIX		*pmLocalTM = &pBoneFrame->m_mLocalTM;
				D3DXQuaternionRotationMatrix( &qLocal, pmLocalTM );
				vLocal.x = pmLocalTM->_41;
				vLocal.y = pmLocalTM->_42;
				vLocal.z = pmLocalTM->_43;	// ������������ LocalTM�� ���ʹϿ°� ���ͷ� ����.

				if( pFrameOld )
				{
					D3DXQuaternionSlerp( &qSlerp, &pFrameOld->m_qRot, &qLocal, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &pFrameOld->m_vPos, &vLocal, fBlendSlp );		// ������ Pos���� ����
				} else
				{
					D3DXQUATERNION	qLocal2;
					D3DXVECTOR3		vLocal2;
					D3DXMATRIX		*pmLocalTM2 = &pBoneFrameOld->m_mLocalTM;
					D3DXQuaternionRotationMatrix( &qLocal2, pmLocalTM2 );
					vLocal2.x = pmLocalTM2->_41;
					vLocal2.y = pmLocalTM2->_42;
					vLocal2.z = pmLocalTM2->_43;
					D3DXQuaternionSlerp( &qSlerp, &qLocal2, &qLocal, fBlendSlp );	// ������ǰ� ���� �������� ����
					D3DXVec3Lerp( &vSlerp, &vLocal2, &vLocal, fBlendSlp );		// ������ Pos���� ����
				}
				D3DXMatrixTranslation( &m1,	 vSlerp.x,  vSlerp.y,  vSlerp.z );
			
				// ���ʹϿ� ������ ȸ��Ű���� ȸ����ķ� ����
				D3DXMatrixRotationQuaternion( &m2, &qSlerp );
				m2 *= m1;			// �̵���� X ȸ����� = �ִ����
			} else
				m2 = pBoneFrame->m_mLocalTM;

			if( pBone->m_pParent )
				m2 *= pMatBones[ pBone->m_nParentIdx ];
//				else
//					m2 = m2 * *pmWorld;
		}

		*pmUpdateBone = m2;		// �� ������ ���� ���� ��ȯ ��Ʈ����

		pBoneFrame ++;
		pBoneFrameOld ++;
		pBone ++;					// �� ++
		pmUpdateBone ++;			// �� Ʈ������ ��Ʈ���� ++
	}
}

