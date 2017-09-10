#include "stdafx.h"
#include "Model.h"
#include "..\_DirectX\dxutil.h"


CModel::~CModel()
{
//	SAFE_DELETE( m_pBillboard );
	DeleteDeviceObjects();
}

// fSpeed : �ִϸ��̼� �ӵ��� �����ϰ� ������. 1.0�� �⺻�� 2.0�� �ι� ������.
void CModel::FrameMove( D3DXVECTOR3 *pvSndPos, float fSpeed )
{
	if( m_nPause )		
		return;
	if( m_nFrameMax == 0 )	
		return;		// �������� ���°� ���ʿ� ����.

	int		nCurrFrame;
	// ������ ���� Slerp ���.

	m_fFrameOld = m_fFrameCurrent;			// �ٷ� ���� ������ ��ȣ�� �޾Ƶ�.

	if( m_fFrameOld > (float)(m_nFrameMax - 1) )
		m_fFrameOld -= (float)m_nFrameMax;

	if( m_bSlow )
		m_fFrameCurrent += 0.1f;			// �ִϸ��̼� ����
	else
		m_fFrameCurrent += (m_fPerSlerp * fSpeed);			// �ִϸ��̼� ����


	nCurrFrame = (int)m_fFrameCurrent;				// �Ҽ��� ������ �����θ�..
	m_fSlp = m_fFrameCurrent - (float)nCurrFrame;	// �Ҽ��� �κи� ������

	if( m_nLoop & ANILOOP_LOOP )		// ���θ���϶�
	{
		if( m_fFrameCurrent > (float)(m_nFrameMax - 1) )
		{
			m_bEndFrame = TRUE;
			nCurrFrame %= m_nFrameMax;		// ó������ �ǵ���
		}
	} 
	else		// ���θ�尡 �ƴ϶�� ������ �����ӿ��� �����.
	{
		if( m_fFrameCurrent > (float)(m_nFrameMax - 1) )
		{
			m_bEndFrame = TRUE;
			m_fSlp = 0.0f;
			nCurrFrame = m_nFrameMax - 1;
		}
	}
	m_fFrameCurrent = (float)nCurrFrame + m_fSlp;	// ���ǻ� �ð������� ���� ������ ��ġ ����
}

void CModel::AddFrame( FLOAT fAddFrame )
{
}
void CModel::SetFrame( FLOAT fFrame )
{
}
BOOL CModel::Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld )
{
	return TRUE;
}
BOOL CModel::IsAniable()
{
	if( m_nModelType == MODELTYPE_MESH || m_nModelType == MODELTYPE_BILLBOARD ) 
		return FALSE;
	return TRUE;
}

//
//	���� �������� ���� �����ӹ�ȣ�� ����
//
int	CModel::GetNextFrame( void )
{
	int		nCurrFrm = (int)m_fFrameCurrent;
	int		nNextFrm;

	nNextFrm = nCurrFrm + 1;
	if( nNextFrm >= m_nFrameMax )		
	{
		if( m_nLoop & ANILOOP_LOOP )	
			nNextFrm = 0;			// ���ξִϸ��̼��̸� ó�������Ӱ� ����ɼ� �ֵ��� �Ѵ�.
		else													
			nNextFrm = m_nFrameMax - 1;		// �ƴϸ� ������ �����ӿ��� �����
	}

	return nNextFrm;
}

//
//	�ٿ��ڽ� vMin,vMax�� �������� �������� ������
//
float CModel::GetRadius( void )
{
	D3DXVECTOR3 vLen = m_vMax - m_vMin;
	float	fLen = D3DXVec3Length( &vLen );
	return fLen / 2.0f;
}

BOOL CModel::IntersectBB( const D3DXVECTOR3 &vRayOrig, const D3DXVECTOR3 &vRayDir, const D3DXMATRIX &mWorld, D3DXVECTOR3* pvIntersect, FLOAT* pfDist )
{
	// Collect all intersections
	D3DXVECTOR3 v1, v2, v3;
	D3DXVECTOR4 vOut;
	D3DXVECTOR3 vPos;

	//  3|2  
	//  -+-
	//	0|1
	//  
	//  7|6
	//  -+-
	//	4|5

	static int anIntexTable[ 12 * 3 ] =
	{
		0, 1, 2,  0, 2, 3,
		0, 4, 5,  0, 5, 1,
		4, 5, 6,  4, 6, 7,
		3, 2, 6,  3, 6, 7,
		0, 3, 7,  0, 7, 4,
		1, 2, 6,  1, 6, 5
	};
	const BOUND_BOX* pBB = GetBBVector();

	for( int i = 0; i < 12 * 3; i += 3 )
	{
		v1 = pBB->m_vPos[ anIntexTable[ i + 0 ] ];
		v2 = pBB->m_vPos[ anIntexTable[ i + 1 ] ];
		v3 = pBB->m_vPos[ anIntexTable[ i + 2 ] ];					

		D3DXVec3TransformCoord( &v1, &v1, &mWorld ); 
		D3DXVec3TransformCoord( &v2, &v2, &mWorld ); 
		D3DXVec3TransformCoord( &v3, &v3, &mWorld ); 
		
		if( IntersectTriangle( v1, v2, v3, vRayOrig, vRayDir, pvIntersect, pfDist ) ) 
			return TRUE;
	}
	return FALSE;
}
