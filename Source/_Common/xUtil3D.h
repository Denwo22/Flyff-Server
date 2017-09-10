#ifndef	__XUTIL3D_H__
#define	__XUTIL3D_H__

void QuaternionRotationToYPW( const D3DXQUATERNION& qRot, D3DXVECTOR3& vYPW );

void	SetBB( D3DXVECTOR3 *pBBVList, const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax );

// AABB���� �浹����
int		IsTouchAABB( const D3DXVECTOR3 &vMin1, const D3DXVECTOR3 &vMax1,  
					 const D3DXVECTOR3 &vMin2, const D3DXVECTOR3 &vMax2 );

BOOL	IsTouchAABB_Line( const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax, 
						  const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2, 
						  D3DXVECTOR3 *pvIntersect );

BOOL	IsTouchOBB_Line( const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax, const D3DXMATRIX &m, 
						 const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2, D3DXVECTOR3 *pvIntersect );

BOOL	IsTouchRayTri( const D3DXVECTOR3 *v0, const D3DXVECTOR3 *v1, const D3DXVECTOR3 *v2,
					   const D3DXVECTOR3 *vOrig, const D3DXVECTOR3 *vDir, 
					   FLOAT* pfDist );

void	CalcSlideVec( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vDir, const D3DXVECTOR3 &vN );

inline void AngleToVector( D3DXVECTOR3 *vDelta, float fAngXZ, float fAngH, float fSpeed )
{
	float	fDist;

	fAngXZ = D3DXToRadian(fAngXZ);
	fAngH  = D3DXToRadian(fAngH);
	fDist = cosf(-fAngH) * fSpeed;
	vDelta->x = sinf(fAngXZ) * fDist;
	vDelta->z = -cosf(fAngXZ) * fDist;
	vDelta->y = -sinf(-fAngH) * fSpeed;
}

inline void AngleToVectorXZ( D3DXVECTOR3 *vDelta, float fAngXZ, float fSpeed )
{
	fAngXZ = D3DXToRadian(fAngXZ);
	vDelta->x = sinf(fAngXZ) * fSpeed;
	vDelta->z = -cosf(fAngXZ) * fSpeed;
	vDelta->y = 0;
}

// XZ����� ������ ������ ������ �Ѳ����� ����.
inline void xGetDegree( float *pfAngXZ, float *pfAngH, const D3DXVECTOR3 &vDist )
{
	D3DXVECTOR3 vDistXZ = vDist;
	vDistXZ.y = 0;
	FLOAT fAngXZ = D3DXToDegree( (FLOAT)atan2( vDist.x, -vDist.z ) );		// �켱 XZ����� ������ ���� ����
	FLOAT fLenXZ = D3DXVec3Length( &vDistXZ );						// y��ǥ�� ������ XZ��鿡���� ���̸� ����.
	FLOAT fAngH  = D3DXToDegree( (FLOAT)atan2( fLenXZ, vDist.y ) );		// XZ����� ���̿� y���̰��� ������ ����.
	fAngH -= 90.0f;
	
	*pfAngXZ = fAngXZ;
	*pfAngH  = fAngH;
}


//���� Ŭ���� (�� ������ Origin-Extent*Direction, Origin+Extent*Direction ) 
class Segment3
{
public:
	Segment3( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2 );

    D3DXVECTOR3 Origin, Direction;
    float Extent;
};

// oriented bound box
class BBOX
{
public:
	D3DXVECTOR3		Center;
	float			Extent[3];
	D3DXVECTOR3		Axis[3];

	void UpdateMartix( const D3DXVECTOR3 &vMin, const D3DXVECTOR3 &vMax,
		               const D3DXMATRIX& matScale, const D3DXMATRIX& matRotation, const D3DXMATRIX& matWorld );
};						  

bool IntrSegment3Box3_Test( const Segment3& rkSegment, const BBOX& rkBox );


#if 0
// Ray�� AABB���� �浹����
BOOL	IsTouchRayAABB( const D3DXVECTOR3 &vRayOrig, const D3DXVECTOR3 &vRayDir, 
					    const D3DXVECTOR3 &vBoxOrig, const D3DXVECTOR3 &vBoxSize );
BOOL	IsTouchRayAABB2( const D3DXVECTOR3& vRayOrig, const D3DXVECTOR3& vRayDir, 
					     const D3DXVECTOR3& vBoxOrig, const D3DXVECTOR3& vBoxSize );
BOOL	IsTouchRayAABB3( const D3DXVECTOR3& vRayOrig, const D3DXVECTOR3& vRayDir, 
					     const D3DXVECTOR3& vBoxOrig, const D3DXVECTOR3& vBoxSize );

BOOL	IsTouchRaySphere( const D3DXVECTOR3& vRayOrig, const D3DXVECTOR3& vRayDir, 
						  const D3DXVECTOR3& vPos, float fR );
BOOL	IsTouchRaySphere2( const D3DXVECTOR3& vRayOrig, const D3DXVECTOR3& vRayDir, 
						  const D3DXVECTOR3& vPos, float fR );

void	CalcRay( D3DXVECTOR3 *pvRayOrig, D3DXVECTOR3 *pvRayDir, 
				 int nScrW, int nScrH,
				 int nMouX, int nMouY, 
				 const D3DXMATRIX &mProj, const D3DXMATRIX &mView, const D3DXMATRIX *pmWorld = NULL );

void	CalcFaceNormal( D3DXVECTOR3 *pNormal, const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2, const D3DXVECTOR3 &v3 );
void	CalcFaceUnitNormal( D3DXVECTOR3 *pNormal, const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2, const D3DXVECTOR3 &v3 );

// XZ���󿡼��� ������ �������� �� ������ ����
inline FLOAT xGetDegreeXZ( float x, float z )
{
	return D3DXToDegree( (FLOAT)atan2( x, -z ) );
}

// XZ����� ���ͱ��̿� y��ǥ���� ����.
inline FLOAT xGetDegreeH( float fLenXZ, float y )
{
	return D3DXToDegree( (FLOAT)atan2( fLenXZ, y ) );
}
#endif // if 0

#endif
