#include "stdafx.h"

#if __VER >= 13 // __HOUSING
#include "housing.h"
#endif // __HOUSING
#if __VER >= 15 // __GUILD_HOUSE
#include "GuildHouse.h"
#endif
//
//	vPos������ ���� ���̸� ���.  ����, ��������Ʈ ��� �����̴�.
//
FLOAT CWorld::GetFullHeight( const D3DXVECTOR3& vPos )
{
	static D3DXVECTOR3 vDir( 0.0f, -2000.0f, 0.0f );	// �Ʒ��� 2000
	D3DXVECTOR3 vEnd = vPos + vDir;
	D3DXVECTOR3 vIntersect;
	FLOAT fDist, fDistMin = 4194304.0f;

	CObj* pObj;
	CObj* pMinObj = NULL;
	CModel* pModel;
	int nRange = 0;

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel = pObj->m_pModel;
		if( pModel->IsTouchOBB_Line( vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
		{
			vIntersect -= vPos;							// vPos�κ����� ��뺤��
			fDist = D3DXVec3LengthSq( &vIntersect );	// ������� �Ÿ�
			if( fDist < fDistMin )						// �� �������� ������ �𸣴� ���� ����� ������Ʈ�� ��������.
			{
				fDistMin = fDist;
				pMinObj = pObj;
			}
		}
	}
	END_LINKMAP
	
	if( pMinObj )	// ���̿� �ɸ� ���� ����� ������Ʈ�� �ִ°�	
	{
		pModel = pMinObj->m_pModel;
		// ���̿� �ﰢ������ ���� �˻�
	#ifdef __CPU_UTILDOWN_060502
		D3DXVECTOR3	*pTris[3];
		((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTris, vPos, vEnd, pMinObj->GetMatrixWorld(), &fDist );
		if( *pTris )
		{
			D3DXVECTOR3 v = vPos + fDist * vDir;
			return v.y;
		}
	#else
		D3DXVECTOR3 *pTri = pModel->IntersectRayTri( vPos, vDir, pMinObj->GetMatrixWorld(), &vIntersect, &fDist );
		if( pTri )
			return vIntersect.y;
	#endif
	}
	return GetLandHeight( vPos.x, vPos.z );
}

#if __VER >= 13 // __HOUSING
#ifdef __CLIENT
// ���� ĳ���Ͱ� �ִ� ��ġ�� �ʿ��� �ش��̸��� ���� ������Ʈ�� ã�´�
CObj *CWorld::GetObjByName(TCHAR* ObjName)
{
	CObj* pObj;
	if(g_pPlayer)
	{
		FOR_LINKMAP( this, g_pPlayer->m_vPos, pObj, 0, CObj::linkStatic, nDefaultLayer )
		{
			char* pString = ((CModelObject *)pObj->m_pModel)->GetObject3D()->m_szFileName;
			if( strcmp(ObjName, pString) == 0 )
			{
				return pObj;
			}
		}
		END_LINKMAP
	}
	
	return NULL;
}

void CWorld::ForceTexture(LPDIRECT3DTEXTURE9 pNewTex)
{
	for( int i = 0; i < m_nLandWidth * m_nLandHeight; i++)
	{
		if(m_apLand[ i ]) m_apLand[ i ]->ForceTexture(pNewTex);
	}

}


BOOL	CWorld::ProcessObjCollision(D3DXVECTOR3 vPos, CObj* pTargetObj, CObj* pWallObj)
{
	// gmpbigsun: ���Լ��� �Ͽ�¡ ���� �浹���� �Լ��� 
	CObj	*pObj	= NULL;

	if( TestOBBIntersect( &pWallObj->m_OBB, &pTargetObj->m_OBB ) )  // �� �� �浹�� ���ж�°��� ���� ����ٴ� ..
	{
		if( TestTriIntersect( pWallObj, pTargetObj ) )
			return TRUE;
	}
	else
		return TRUE;		// out of room

	// ������Ʈ�� ������ ������ �����˻��Ѵ�
	int nRange = OLD_MPU / m_iMPU;					// gmpbigsun:MPU�� 4 �����ϰ�� �� ������ġ�� �����Ǿ�� ��.
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkDynamic, 0 )
	{
		if( pObj->GetType() == OT_CTRL )
		{
#if __VER >= 15 // __GUILD_HOUSE
			if( ((CCtrl*)pObj)->GetId( ) != GuildHouse->m_dwSelectedObjID )		//gmpbigsun: �� �缳ġ�� ��� ���纻�� ������ �浹�н� 
#endif
			{
				if(pObj != pTargetObj)
				{
					// ������Ʈ OBB���� �˻�
					if(TestOBBIntersect(&pObj->m_OBB, &pTargetObj->m_OBB))
						return TRUE;
				}
			}
		}
	}
	END_LINKMAP
	
// 	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, 0)
// 	{
// 		if(pWallObj == pObj)
// 		{
// 			// pWallObj�� Ư���ϰ� ����ü�� ���ΰ� �ִ� �ڽ��̱⶧���� �浹�� �����Ѵٴ� ���� �뿡�� ��� ���̶�� �ǹ�
// 			if(TestOBBIntersect(&pObj->m_OBB, &pTargetObj->m_OBB))
// 			{
// 				if(TestTriIntersect(pObj, pTargetObj))
// 						return TRUE;	
// 			}
// 			else	
// 				return TRUE;
// 		}
// 	}
// 	END_LINKMAP

	return FALSE;
}

BOOL	CWorld::TestTriIntersect(CObj* pWallObj, CObj* pTargetObj)
{
	GMOBJECT* pTargetGm;

	if(((CModelObject*)pTargetObj->m_pModel)->GetObject3D()->m_CollObject.m_Type != GMT_ERROR)
	{
		// �浹 ������Ʈ�� ������ �浹 ������Ʈ��
		pTargetGm = &((CModelObject*)pTargetObj->m_pModel)->GetObject3D()->m_CollObject;
	}
	else
		pTargetGm = ((CModelObject*)pTargetObj->m_pModel)->GetObject3D()->GetGMOBJECT();
	
	if( ((CModelObject*)pWallObj->m_pModel)->GetObject3D()->SimpleTriIntersect(pWallObj->GetMatrixWorld(), pTargetGm, pTargetObj->GetMatrixWorld()))
		return TRUE;
	
	return FALSE;

}

BOOL	CWorld::TestOBBIntersect(BBOX* BoxA, BBOX* BoxB)
{
	float		fReal[3][3];
	float		ra, rb, t;
	long		i, k;
	D3DXVECTOR3 vDist = BoxB->Center - BoxA->Center;	// �� �ڽ����� ����
	float		fTrans[3]; 

#if __VER >= 15 // __BS_BBOX_ABS_EXTENT
 	for( i = 0; i < 3; ++i )			//backface�� ���°�� Extent�� ������ ����. Export�Ҷ� �ذ��ϴ°�...
 	{
		if( BoxA->Extent[i] < 0 )
 			BoxA->Extent[i] = -( BoxA->Extent[i] );
		if( BoxB->Extent[i] < 0 )
 			BoxB->Extent[i] = -( BoxB->Extent[i] );
 	}
#endif

	for( i = 0; i < 3; ++i)	fTrans[i] = D3DXVec3Dot(&vDist, &BoxA->Axis[i]);
	//ȸ�� ��Ʈ���� ���
	for( i=0 ; i<3 ; i++ )
		for( k=0 ; k<3 ; k++ )
		{
			fReal[i][k] = D3DXVec3Dot(&BoxA->Axis[i], &BoxB->Axis[k]); 
		}
	
	//A�ڽ��� �⺻ ���͵� ���
	for( i=0 ; i<3 ; i++ )
	{
		ra = BoxA->Extent[i];
		rb = BoxB->Extent[0]*fabs(fReal[i][0]) + BoxB->Extent[1]*fabs(fReal[i][1]) + BoxB->Extent[2]*fabs(fReal[i][2]);
		t = fabs( fTrans[i] );
		if( t > ra + rb )	return FALSE;
	}

	//B�ڽ��� �⺻ ���͵� ���
	for( k=0 ; k<3 ; k++ )
	{
		ra = BoxA->Extent[0]*fabs(fReal[0][k]) + BoxA->Extent[1]*fabs(fReal[1][k]) + BoxA->Extent[2]*fabs(fReal[2][k]);
		rb = BoxB->Extent[k];
		t = fabs( fTrans[0]*fReal[0][k] + fTrans[1]*fReal[1][k] + fTrans[2]*fReal[2][k] );
		if( t > ra + rb )	return FALSE;
	}

	//���� ���ϱ�
	ra = BoxA->Extent[1]*fabs(fReal[2][0]) + BoxA->Extent[2]*fabs(fReal[1][0]);
	rb = BoxB->Extent[1]*fabs(fReal[0][2]) + BoxB->Extent[2]*fabs(fReal[0][1]);
	t = fabs( fTrans[2]*fReal[1][0] - fTrans[1]*fReal[2][0] );

	if( t > ra + rb )	return FALSE;

	//L = A0 x B1
	ra = BoxA->Extent[1]*fabs(fReal[2][1]) + BoxA->Extent[2]*fabs(fReal[1][1]);
	rb = BoxB->Extent[0]*fabs(fReal[0][2]) + BoxB->Extent[2]*fabs(fReal[0][0]);
	t = fabs( fTrans[2]*fReal[1][1] - fTrans[1]*fReal[2][1] );

	if( t > ra + rb )	return FALSE;

	//L = A0 x B2
	ra = BoxA->Extent[1]*fabs(fReal[2][2]) + BoxA->Extent[2]*fabs(fReal[1][2]);
	rb = BoxB->Extent[0]*fabs(fReal[0][1]) + BoxB->Extent[1]*fabs(fReal[0][0]);
	t = fabs( fTrans[2]*fReal[1][2] - fTrans[1]*fReal[2][2] );

	if( t > ra + rb )	return FALSE;

	//L = A1 x B0
	ra = BoxA->Extent[0]*fabs(fReal[2][0]) + BoxA->Extent[2]*fabs(fReal[0][0]);
	rb = BoxB->Extent[1]*fabs(fReal[1][2]) + BoxB->Extent[2]*fabs(fReal[1][1]);
	t = fabs( fTrans[0]*fReal[2][0] - fTrans[2]*fReal[0][0] );

	if( t > ra + rb )	return FALSE;

	//L = A1 x B1
	ra = BoxA->Extent[0]*fabs(fReal[2][1]) + BoxA->Extent[2]*fabs(fReal[0][1]);
	rb = BoxB->Extent[0]*fabs(fReal[1][2]) + BoxB->Extent[2]*fabs(fReal[1][0]);
	t = fabs( fTrans[0]*fReal[2][1] - fTrans[2]*fReal[0][1] );

	if( t > ra + rb )	return FALSE;

	//L = A1 x B2
	ra = BoxA->Extent[0]*fabs(fReal[2][2]) + BoxA->Extent[2]*fabs(fReal[0][2]);
	rb = BoxB->Extent[0]*fabs(fReal[1][1]) + BoxB->Extent[1]*fabs(fReal[1][0]);
	t = fabs( fTrans[0]*fReal[2][2] - fTrans[2]*fReal[0][2] );

	if( t > ra + rb )	return FALSE;

	//L = A2 x B0
	ra = BoxA->Extent[0]*fabs(fReal[1][0]) + BoxA->Extent[1]*fabs(fReal[0][0]);
	rb = BoxB->Extent[1]*fabs(fReal[2][2]) + BoxB->Extent[2]*fabs(fReal[2][1]);
	t = fabs( fTrans[1]*fReal[0][0] - fTrans[0]*fReal[1][0] );

	if( t > ra + rb )	return FALSE;

	//L = A2 x B1
	ra = BoxA->Extent[0]*fabs(fReal[1][1]) + BoxA->Extent[1]*fabs(fReal[0][1]);
	rb = BoxB->Extent[0] *fabs(fReal[2][2]) + BoxB->Extent[2]*fabs(fReal[2][0]);
	t = fabs( fTrans[1]*fReal[0][1] - fTrans[0]*fReal[1][1] );

	if( t > ra + rb )	return FALSE;

	//L = A2 x B2
	ra = BoxA->Extent[0]*fabs(fReal[1][2]) + BoxA->Extent[1]*fabs(fReal[0][2]);
	rb = BoxB->Extent[0]*fabs(fReal[2][1]) + BoxB->Extent[1]*fabs(fReal[2][0]);
	t = fabs( fTrans[1]*fReal[0][2] - fTrans[0]*fReal[1][2] );

	if( t > ra + rb )	return FALSE;

	// ��ġ�ϴ� �и����� �ϳ��� ����~ 

	return TRUE;
}
#endif // __CLIENT
#endif // __HOUSING

//
//	vPos�ٷ� �Ʒ����� ���� ���.  �ٷ� �Ʒ��ʿ� �ɸ��°� ���ٸ� ������ ���ٴ� ���̷� ���.
//
FLOAT CWorld::GetUnderHeight( const D3DXVECTOR3 &vPos )
{
	static D3DXVECTOR3 vDir( 0.0f, -1000.0f, 0.0f );	
	D3DXVECTOR3 vEnd = vPos + vDir;
	Segment3 segment( vPos, vEnd );

	CObj* pObj;
	CObj* pMinObj = NULL;
	CModel* pModel;

	int nRange = 0;
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			pMinObj = pObj;		// ã������ �ٷ� Ƣ��� - �ӵ����� �̷���...
			goto LP1;
		}
	}
	END_LINKMAP

LP1:
	if( pMinObj )	// ���̿� �ɸ� ���� ����� ������Ʈ�� �ִ°�	
	{
		FLOAT fDist;
		pModel = pMinObj->m_pModel;
		// ���̿� �ﰢ������ ���� �˻�
		D3DXVECTOR3	*pTri[3];
		((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pMinObj->GetMatrixWorld(), &fDist );
		if( *pTri )
		{
			D3DXVECTOR3 vIntersect = vPos + fDist * vDir;
			return vIntersect.y;
		}
	}
	return GetLandHeight( vPos.x, vPos.z );
}

FLOAT	CWorld::GetItemHeight( const D3DXVECTOR3 & vPos )
{
	D3DXVECTOR3 vDir( 0.0f, -3.0f, 0.0f );
	D3DXVECTOR3 vIntersect;
	float fMaxY	= -65535.0f;
	D3DXVECTOR3 vOut( 0.0f, 0.0f, 0.0f );
	CObj* pObj;
	CModel* pModel;
	int nRange = 0;

	D3DXVECTOR3 vEnd( vPos + vDir );
	Segment3 segment( vPos, vEnd );
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel	= pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
			{
				if( vIntersect.y > fMaxY )
					fMaxY = vIntersect.y;	// �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
			}
		}
	}
	END_LINKMAP
	if( fMaxY != -65535.0f )
		return fMaxY;
	return GetLandHeight( vPos );
}

#if __VER >= 15 // __BOUND_BOX_COLLISION
FLOAT CWorld::GetOverHeightForPlayer( D3DXVECTOR3 &vPos, CObj* pExceptionObj )
{
	static D3DXVECTOR3 vDir( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 vIntersect, vEnd;
	FLOAT fDist;

	CObj* pObj;
	CObj* pMinObj = NULL;
	CModel* pModel;
	int nRange = 0;
	vEnd = vPos + vDir;
	Segment3 segment( vPos, vEnd );
	Segment3 point( vPos, vPos );

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			if( pModel->TestIntersectionOBB_Line( point, pObj ) == TRUE )
			{
				pMinObj = pObj;
			}
			else
			{
				pMinObj = pObj;
				goto LP1;
			}
		}
	}
	END_LINKMAP
LP1:
	if( pMinObj )	// ���̿� �ɸ� ���� ����� ������Ʈ�� �ִ°�
	{
		pModel = pMinObj->m_pModel;
		// ���̿� �ﰢ������ ���� �˻�
		D3DXVECTOR3	*pTri[3];
		((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pMinObj->GetMatrixWorld(), &fDist );
		if( *pTri )
		{
			vIntersect = vPos + fDist * vDir;
			return vIntersect.y;
		}
	}
	return 65535.0f;	// �ɸ��� �� ������ �ִ�ġ�� ����
}
#endif // __BOUND_BOX_COLLISION

//
//	vPos�ٷ� ������ ���� ���
//
FLOAT CWorld::GetOverHeight( D3DXVECTOR3 &vPos, CObj* pExceptionObj )
{
	static D3DXVECTOR3 vDir( 0.0f, 1.0f, 0.0f );	
	D3DXVECTOR3 vIntersect, vEnd;
	FLOAT fDist;

	CObj* pObj;
	CObj* pMinObj = NULL;
	CModel* pModel;
	int nRange = 0;
	vEnd = vPos + vDir;
	Segment3 segment( vPos, vEnd );

#if __VER >= 15 // __BOUND_BOX_COLLISION
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			pMinObj = pObj;		// ã������ �ٷ� Ƣ���
			goto LP1;
		}
	}
	END_LINKMAP
#else // __BOUND_BOX_COLLISION
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel = pObj->m_pModel;
#if __VER >= 13 &&  defined(__CLIENT) // __HOUSING
		if(GetID() == WI_WORLD_MINIROOM)
		{
			if(pObj != GetObjByName("obj_miniwall01.o3d") &&pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE)
			{
				pMinObj = pObj;		// ã������ �ٷ� Ƣ���
				goto LP1;
			}
		}
#if __VER >= 14 // __INSTANCE_DUNGEON
		else if(GetID() == WI_INSTANCE_OMINOUS || GetID() == WI_INSTANCE_OMINOUS_1 )
		{
			char* pString = ((CModelObject *)pObj->m_pModel)->GetObject3D()->m_szFileName;
			if(strcmp(pString, "obj_ceiling_01.o3d") == 0 || strcmp(pString, "obj_ceiling_02.o3d") == 0)
			{
				if(pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE)
				{
					pMinObj = pObj;		// ã������ �ٷ� Ƣ���
					goto LP1;
				}
			}
		}
#endif // __INSTANCE_DUNGEON
		else
		{
			if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
			{
				pMinObj = pObj;		// ã������ �ٷ� Ƣ���
				goto LP1;
			}
		}
#else
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			pMinObj = pObj;		// ã������ �ٷ� Ƣ���
			goto LP1;
		}
#endif
	}
	END_LINKMAP
#endif // __BOUND_BOX_COLLISION
LP1:
	if( pMinObj )	// ���̿� �ɸ� ���� ����� ������Ʈ�� �ִ°�
	{
		pModel = pMinObj->m_pModel;
		// ���̿� �ﰢ������ ���� �˻�
		D3DXVECTOR3	*pTri[3];
		((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pMinObj->GetMatrixWorld(), &fDist );
		if( *pTri )
		{
			vIntersect = vPos + fDist * vDir;
			return vIntersect.y;
		}
	}
	return 65535.0f;	// �ɸ��°� ������ �ִ�ġ�� ����
}

//
//	vPos-vDir�� �̵����͸� �޾� ������ �浹�� �˻��ϰ�
//	�� ��ó���� ����Ͽ� pOut���� �Ѱ��ش�.
//
#ifdef __LAYER_1015
BOOL	CWorld::ProcessCollision( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, int nSlideCnt, int nLayer )
#else	// __LAYER_1015
BOOL	CWorld::ProcessCollision( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, int nSlideCnt )
#endif	// __LAYER_1015
{
	_PROFILE("CWorld::ProcessCollision()");

	int nRange = 0;
	CObj	*pObj = NULL;
	CModel	*pModel = NULL;
	D3DXVECTOR3	vEnd, vIntersect, vSlide;
	
	vEnd = vPos + vDir;		// ���̸� �������� �ٲ۴�.
	Segment3 segment( vPos, vEnd );
	
	*pOut = vDir;

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkDynamic, nLayer )
	{
		if( pObj->GetType() == OT_CTRL ) 
		{
			// ���а� ������ƮOBB�� �˻�.
			pModel = pObj->m_pModel;
			if( pModel->TestIntersectionOBB_Line( segment, pObj, FALSE ) == TRUE )
			{
#if __VER >= 13 // __HOUSING
				BOOL IsColl;

				if(GetID() == WI_WORLD_MINIROOM)
					IsColl = ((CModelObject *)pModel)->GetObject3D()->SlideVectorXZ2( &vSlide, &vIntersect, vPos, vEnd, pObj->GetMatrixWorld());
				else
					IsColl = ((CModelObject *)pModel)->GetObject3D()->SlideVectorXZ2( &vSlide, &vIntersect, vPos, vEnd, pObj->GetMatrixWorld(), 0);

				if(IsColl == TRUE)
#else // __HOUSING
				if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorXZ2( &vSlide, &vIntersect, vPos, vEnd, pObj->GetMatrixWorld(), 0 ) == TRUE )
#endif // __HOUSING
				{
					// ����������� ���� �浹������ �ѹ��� ����. ��Ŀ�ú��.
					D3DXVECTOR3 vE = vIntersect + vSlide;		// �̲����� ����.
					*pOut = vSlide;
					if( ++nSlideCnt <= 1 )
					{
#ifdef __LAYER_1015
						int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt, nLayer );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#else	//__LAYER_1015
						int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#endif	// __LAYER_1015
						if( r )
						{
							pOut->x = pOut->y = pOut->z = 0;	// 2�� �ε������� ������ ������ ����.
						}
					}
					return 1;
				}
			}
		}
	}
	END_LINKMAP
		
		
 	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ���а� ������ƮOBB�� �˻�.
		pModel = pObj->m_pModel;
		
		//sun!!
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// ���а� �ﰢ���� ���� �˻�.  �����Ѵٸ� slide vector�� ���ϰ�
			// �������� �ʴ´ٸ� �� FALSE�� �����Ѵ�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorXZ2( &vSlide, &vIntersect, vPos, vEnd, pObj->GetMatrixWorld() ) == TRUE )
			{
				//D3DXVECTOR3 vE = vIntersect + vSlide;
				*pOut = vSlide;
			#if __VER >= 11 // __FIX_COLLISION	
				if( (++nSlideCnt <= 1) && ((D3DXVec3Length(&vDir) < 3.0f)) )
				{
#ifdef __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt, nLayer );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#else	// __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#endif	// __LAYER_1015
					if( r )	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
					{
						pOut->x = pOut->y = pOut->z = 0;	// 2�� �ε������� ������ ������ ����.
					}
				}
				else if( nSlideCnt <= 1 )
				{	
#ifdef __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt, nLayer );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#else	// __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#endif	// __LAYER_1015
					if( r )	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
					{
						pOut->x = pOut->y = pOut->z = 0;	// 2�� �ε������� ������ ������ ����.
					}
				}
			#else
				D3DXVECTOR3 vE = vIntersect + vSlide;
				*pOut = vSlide;

				if( ++nSlideCnt <= 1 )
				{	
#ifdef __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt, nLayer );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#else	// __LAYER_1015
					int r = ProcessCollision( pOut, vPos, vSlide, nSlideCnt );	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
#endif	// __LAYER_1015
					if( r )	// �浹�� �������� �̲��������������� ���ͷ� �ٽ��ѹ� �˻�.
					{
						pOut->x = pOut->y = pOut->z = 0;	// 2�� �ε������� ������ ������ ����.
					}
				}
#endif
				return 2;
			}
		}
	}
	END_LINKMAP
		
	return FALSE;
}


//
//	vPos-vDir�� �̵����͸� �޾� ������ �浹�� �˻��ϰ�
//	�� ��ó���� ����Ͽ� pOut���� �Ѱ��ش�.
//
BOOL	CWorld::ProcessCollisionReflection( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, int nRecusCnt )
{
	int nRange = 0;
	CObj	*pObj = NULL;
	CModel	*pModel = NULL;
	D3DXVECTOR3	vEnd, vIntersect, vN, v1;
	D3DXMATRIX mTM;
	FLOAT	fDist;
	D3DXVECTOR3	*pTri[3];

	vEnd = vPos + vDir;		// ���̸� �������� �ٲ۴�.
	Segment3 segment( vPos, vEnd );

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ���а� ������ƮOBB�� �˻�.
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// ���а� �ﰢ���� ���� �˻�.  pTri�� ��ܳ��´�.
			((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pObj->GetMatrixWorld(), &fDist );
			if( *pTri )
			{
				// (2*(-D dot N)) * N + D
				v1 = *pTri[1] - *pTri[0];
				vN = *pTri[2] - *pTri[0];
				D3DXVec3Cross( &vN, &v1, &vN );
				mTM = pObj->GetMatrixWorld();
				mTM._41 = mTM._42 = mTM._43 = 0;
				D3DXVec3TransformCoord( &vN, &vN, &mTM );	// mTM�� �������� �ִٸ� �븻�� ���� Ŀ���ⶫ�� Normalize�� ���߿� �ؾ� �Ѵ�.
				D3DXVec3Normalize( &vN, &vN );
				
				v1.x = -vDir.x;	v1.y = -vDir.y;	v1.z = -vDir.z;	// -D
				FLOAT fLenN = D3DXVec3Dot( &v1, &vN );			// -D dot N
				vN *= (fLenN * 2.0f);							// (-D dot N) * 2  �̶� N�� �������� ���¿����Ѵ�.
				*pOut = vDir + vN;								// ���� �ݻ纤��.

				if( nRecusCnt++ < 3 )
					ProcessCollisionReflection( pOut, vPos, *pOut, nRecusCnt );
				
				return TRUE;
			}
		}
	}
	END_LINKMAP

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkAirShip, nDefaultLayer )
	{
		// ���а� ������ƮOBB�� �˻�.
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// ���а� �ﰢ���� ���� �˻�.  pTri�� ��ܳ��´�.
			((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pObj->GetMatrixWorld(), &fDist );
			if( *pTri )
			{
				// (2*(-D dot N)) * N + D
				v1 = *pTri[1] - *pTri[0];
				vN = *pTri[2] - *pTri[0];
				D3DXVec3Cross( &vN, &v1, &vN );
				mTM = pObj->GetMatrixWorld();
				mTM._41 = mTM._42 = mTM._43 = 0;
				D3DXVec3TransformCoord( &vN, &vN, &mTM );	// mTM�� �������� �ִٸ� �븻�� ���� Ŀ���ⶫ�� Normalize�� ���߿� �ؾ� �Ѵ�.
				D3DXVec3Normalize( &vN, &vN );
				
				v1.x = -vDir.x;	v1.y = -vDir.y;	v1.z = -vDir.z;	// -D
				FLOAT fLenN = D3DXVec3Dot( &v1, &vN );			// -D dot N
				vN *= (fLenN * 2.0f);							// (-D dot N) * 2  �̶� N�� �������� ���¿����Ѵ�.
				*pOut = vDir + vN;								// ���� �ݻ纤��.

				if( nRecusCnt++ < 3 )
					ProcessCollisionReflection( pOut, vPos, *pOut, nRecusCnt );
				
				return TRUE;
			}
		}
	}
	END_LINKMAP

	return FALSE;
}


// ��ǥ ���� 3x3�׸��� ������ �ﰢ����ǥ�� ��ĵ�� vDir�� �浹�ϴ� �ﰢ���� ã�Ƴ���.
D3DXVECTOR3 *FindTouchLandTri( D3DXVECTOR3 *pTris, int nMaxTri, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vDir, FLOAT *pfDist )
{
	int		i;
	D3DXVECTOR3	*pv0, *pv1, *pv2;
	D3DXVECTOR3	v1, v2;
	FLOAT	fDist;
	BOOL	bRet;
	
	for( i = 0; i < nMaxTri; i ++ )
	{
		pv0 = pTris++;		
		pv1 = pTris++;		
		pv2 = pTris++;
		
		bRet = IsTouchRayTri( pv0, pv1, pv2, &vPos, &vDir, &fDist );
		if( bRet && fDist >= 0.0f )	// �ݴ���� ���� �˻����� ����.
		{
			*pfDist = fDist;
			return pv0;
		}
	}
	
	return NULL;
}


// bSkipTrans:������Ǵ� ������Ʈ�� ��ŵ��.  bWithTerrain:�������� �����ؼ� �˻���.
//
//	���ΰ� �ֺ�������Ʈ���� �浹�� �˻��ؼ�, �������� ����Ѵ�.
// ���� : �˻���� ������Ʈ�� �þ�����ü�� ������ ������Ʈ�� ������� �Ѵ�.
// ī�޶� �浹�� �̰ɷ� �ϸ� �ȵȴ�. ī�޶� vPos�� ����ü�� ���ϴ� ������Ʈ�� ��� �ٲ��.
//
BOOL	CWorld::IntersectObjLine( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd, BOOL bSkipTrans, BOOL bWithTerrain, BOOL bWithObject )
{
	D3DXVECTOR3	vDir;
#ifdef __CLIENT
	if( bWithObject )
	{
		CObj* pObj;
		CObj* pNonCullObjs[ 10000 ];
		CObj **pList = m_aobjCull;
		int nNonCullNum = 0;
		for( int i = 0; i < m_nObjCullSize; i++ )
		{
			pObj = *pList++;
			if( pObj )
			{
				if( pObj->GetType() == OT_OBJ || pObj->GetType() == OT_CTRL )
				{
					if( pObj->IsCull() == FALSE /*&& pObj->GetModel()->m_pModelElem->m_dwDistant <= 1*/ ) 
					{
						if( nNonCullNum >= 10000 )
						{
							Error( "CWorld::IntersectObjLine : ������ �Ѿ." );
							break;
						}
						if( bSkipTrans && pObj->m_pModel->m_pModelElem->m_bTrans )	// �������� �Ǵ� ������Ʈ�� �˻��󿡼� ������.
							continue;
						pNonCullObjs[ nNonCullNum++ ] = pObj;
					}
				}
			}
		}
		
		int ObjSortNearToFar( const VOID* arg1, const VOID* arg2 );
		qsort( pNonCullObjs, nNonCullNum, sizeof(CObj*), ObjSortNearToFar ); 

		CModel	*pModel = NULL;
		D3DXVECTOR3	vIntersect;
		FLOAT fDist;
		Segment3 segment( vPos, vEnd );

		for( int i = nNonCullNum - 1; i >= 0; i-- )
		{
			pObj = (CObj*)pNonCullObjs[ i ];
			pModel = pObj->m_pModel;
			BOOL bCollObj = TRUE;
			if( pObj->GetType() == OT_CTRL )	bCollObj = FALSE;
			if( pModel->TestIntersectionOBB_Line( segment, pObj, bCollObj ) == TRUE )
			{
				D3DXVECTOR3	*pTri[3];
				((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pObj->GetMatrixWorld(), &fDist, bCollObj );
				if( *pTri )
				{
					if( pOut )
					{
						vDir = vEnd - vPos;
						*pOut = vPos + fDist * vDir;
					}
					return TRUE;
				}
			}
		}
	} // bWithObject
#endif // CLIENT

	BOOL bRet = FALSE;
	
	if( bWithTerrain )		// �������� �˻���.
	{
		vDir = vEnd - vPos;		// ���̸� ����.
		D3DXVec3Normalize( &vDir, &vDir );
		FLOAT fDist = IntersectRayTerrain2( vPos, vDir );
		if( fDist )
		{
			*pOut = vPos + (vDir * fDist);
			bRet = TRUE;
		}
	}

	return bRet;
}



//
// vPos�� �߽����� ������ �ݰ泻�� ������Ʈ�� ������� �˻�
// ī�޶� �浹�� �̰ɷ� �ؾ��Ѵ�.
// CLIENT ����
BOOL	CWorld::IntersectObjLine2( D3DXVECTOR3 *pOut, const D3DXVECTOR3 &vPos, const D3DXVECTOR3 &vEnd, BOOL bSkipTrans, BOOL bWithTerrain, BOOL bWithObject )
{
	BOOL bRet = FALSE;
#ifdef __CLIENT
	int nRange = 0;
	CObj	*pObj = NULL;
	CModel	*pModel = NULL;
	D3DXVECTOR3	vDir, vIntersect;
	FLOAT fDist;
	BOOL bAble = TRUE;
	Segment3 segment( vPos, vEnd );

#if __VER >= 12 // __CAM_FAST_RECOVER
	bool			bIsCol    = false;
	D3DXVECTOR3		vTempEnd  = vEnd;
	float			fShortest = 0.0f;

	if(m_nLandWidth < 4) nRange = 5;
	nRange = 5;
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		bAble = TRUE;
		if( bSkipTrans && pObj->m_pModel->m_pModelElem->m_bTrans )	// �������� �Ǵ� ������Ʈ�� �˻��󿡼� ������.
			bAble = FALSE;
		
		if( bAble )
		{
			// ���а� ������ƮOBB�� �˻�.
			pModel = pObj->m_pModel;
			if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
			{
				D3DXVECTOR3	*pTri[3];
				((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vTempEnd, pObj->GetMatrixWorld(), &fDist);
				if( *pTri )
				{
					if( pOut )
					{
						vDir = vTempEnd - vPos;
						if(bIsCol)
						{
							if(fDist < fShortest)
								fShortest = fDist;
							*pOut = vPos + fShortest * vDir;
							segment.Direction = vTempEnd;
						}
						else
						{
							*pOut = vPos + fDist * vDir;
							segment.Direction = vTempEnd;
							fShortest = fDist;
							bIsCol = true;
						}
					}
				}
			}
		}
	}
	END_LINKMAP
	
	if(bIsCol) return TRUE;
#else
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		bAble = TRUE;
		if( bSkipTrans && pObj->m_pModel->m_pModelElem->m_bTrans )	// �������� �Ǵ� ������Ʈ�� �˻��󿡼� ������.
			bAble = FALSE;
		
		if( bAble )
		{
			// ���а� ������ƮOBB�� �˻�.
			pModel = pObj->m_pModel;
			if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
			{
				D3DXVECTOR3	*pTri[3];
				((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pObj->GetMatrixWorld(), &fDist);
				if( *pTri )
				{
					if( pOut )
					{
						vDir = vEnd - vPos;
						*pOut = vPos + fDist * vDir;
					}
					return TRUE;
				}
			}
		}
	}
	END_LINKMAP
#endif
	// ˬ: ī�޶� �浹���� ����ϴ� Ŭ���̾�Ʈ �ڵ��̹Ƿ� ���� ���� 0���� ����
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkDynamic, nTempLayer )
	{
		if( pObj->GetType() == OT_CTRL ) 
		{
			bAble = TRUE;
			if( bSkipTrans && pObj->m_pModel->m_pModelElem->m_bTrans )	// �������� �Ǵ� ������Ʈ�� �˻��󿡼� ������.
				bAble = FALSE;
			
			if( bAble )
			{
				// ���а� ������ƮOBB�� �˻�.
				pModel = pObj->m_pModel;
				if( pModel->TestIntersectionOBB_Line( segment, pObj, FALSE ) == TRUE )
				{
					D3DXVECTOR3	*pTri[3];		// ��Ʈ�� ������Ʈ�� �浹�޽� �Ⱦ�.
					((CModelObject *)pModel)->GetObject3D()->FindTouchTriLine( pTri, vPos, vEnd, pObj->GetMatrixWorld(), &fDist, TRUE);
					if( *pTri )
					{
						if( pOut )
						{
							vDir = vEnd - vPos;
							*pOut = vPos + fDist * vDir;
						}
						return TRUE;
					}
				}
			}
		}
	}
	END_LINKMAP
	
	if( bWithTerrain )		// �������� �˻���.
	{
		vDir = vEnd - vPos;		// ���̸� ����.
		D3DXVec3Normalize( &vDir, &vDir );
		FLOAT fDist = IntersectRayTerrain2( vPos, vDir );
		if( fDist )
		{
			*pOut = vPos + (vDir * fDist);
			bRet = TRUE;
		}
	}
#endif	// __CLIENT
	return bRet;
}

// �־��� ���̿� �浹�ϴ� �ٴ��� üũ. - ������ ���ݿ� ����.
FLOAT CWorld::IntersectRayTerrain2( const D3DXVECTOR3 &vPickRayOrig, const D3DXVECTOR3 &vPickRayDir )
{
#ifdef __CLIENT	
	D3DXVECTOR3 vPickRayAdd ;
	D3DXVECTOR3 vPickRayCur;
	D3DXVECTOR3 vLength;
	D3DXVECTOR3 v1, v2, v3, v4;
	D3DXVECTOR3 _v1, _v2, _v3, _v4;
	BOOL bTriangle1 = FALSE;
	D3DXVECTOR3 vecIntersect;
	FLOAT fDist;               // Ray-Intersection Parameter Distance
	FLOAT fNearDist = m_fFarPlane;                    
	
	vPickRayCur = vPickRayOrig;
	vPickRayAdd = vPickRayDir;
	vPickRayAdd.x *= (FLOAT)MPU * 2;
	vPickRayAdd.z *= (FLOAT)MPU * 2;
	vPickRayAdd.y *= (FLOAT)MPU * 2;
	
	FLOAT fFarPlane = m_fFarPlane;
	fFarPlane /= 2;
	do 
	{
		vPickRayCur += vPickRayAdd;
		if( VecInWorld( vPickRayCur ) )
		{
			CLandscape* pLand = GetLandscape( vPickRayCur );
			if( pLand  && pLand->isVisibile() )
			{
				for( int z = -MPU; z <= MPU; z += MPU )
				{
					for( int x = -MPU; x <= MPU; x += MPU )
					{
						int tempx = (int)( vPickRayCur.x - x );
						int tempy = (int)( vPickRayCur.z - z );
						v1=D3DXVECTOR3( (FLOAT)( tempx       ), GetLandHeight( ( FLOAT ) tempx      , ( FLOAT ) tempy       ), (FLOAT)( tempy       ) );
						v2=D3DXVECTOR3( (FLOAT)( tempx + MPU ), GetLandHeight( ( FLOAT ) tempx + MPU, ( FLOAT ) tempy       ), (FLOAT)( tempy       ) );
						v3=D3DXVECTOR3( (FLOAT)( tempx       ), GetLandHeight( ( FLOAT ) tempx      , ( FLOAT ) tempy + MPU ), (FLOAT)( tempy + MPU ) );
						v4=D3DXVECTOR3( (FLOAT)( tempx + MPU ), GetLandHeight( ( FLOAT ) tempx + MPU, ( FLOAT ) tempy + MPU ), (FLOAT)( tempy + MPU ) );
						if( IntersectTriangle( v1, v2, v3, vPickRayOrig, vPickRayDir, &vecIntersect, &fDist ) ) 
						{
							if( fDist < fNearDist ) { fNearDist = fDist; bTriangle1 = TRUE; }
						}
						else
							if( IntersectTriangle( v2, v4, v3, vPickRayOrig, vPickRayDir, &vecIntersect, &fDist ) ) 
							{
								if( fDist < fNearDist ) {	fNearDist = fDist; bTriangle1 = TRUE; }
							}
					}
				}
			}
		}
		vLength = vPickRayOrig - vPickRayCur;
	} while( D3DXVec3LengthSq( &vLength ) < (fFarPlane * fFarPlane) );
	if( bTriangle1 )
		return fNearDist;
#else // CLIENT
	D3DXVECTOR3 vTri[6];
	FLOAT	x, z;
	int		i, j;
	FLOAT	fDist = 0, fMinDist = 65535.0f;
	BOOL	bRet = FALSE; 

	x = vPickRayOrig.x;		z = vPickRayOrig.z;

	for( i = -10; i < 10; i ++ )
	{
		for( j = -10; j < 10; j ++ )
		{
			if( GetLandTri2( x + (m_iMPU * j), z + (m_iMPU * i), vTri ) )
			{
				bRet = IsTouchRayTri( &vTri[0], &vTri[1], &vTri[2], &vPickRayOrig, &vPickRayDir, &fDist );
				if( bRet && fDist >= 0 )
				{
					if( fDist < fMinDist )
					{
						fMinDist = fDist;
					}
				}
				bRet = IsTouchRayTri( &vTri[3], &vTri[4], &vTri[5], &vPickRayOrig, &vPickRayDir, &fDist );
				if( bRet && fDist >= 0 )
				{
					if( fDist < fMinDist )
					{
						fMinDist = fDist;
					}
				}
			}
		}
	}

	if( fDist )
		return fMinDist;

#endif // not CLIENT

	return 0;
}

#ifdef __SLIDE_060502

// fCosine : -0.19080 == cos(101)
//           -0.37864 == cos(112)
BOOL CanSlide( const D3DXVECTOR3& v0, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2,
				 float fCosine, D3DXVECTOR3* pOut )
{
	// 1. �븻 ��� 
	D3DXVECTOR3	u = v1 - v0;		
	D3DXVECTOR3	v = v2 - v0;

	D3DXVECTOR3 vN;
	D3DXVec3Cross( &vN, &u, &v );					
	D3DXVec3Normalize( &vN, &vN );

	// 2. ������ �̻� ������ �鸸 �����̵� ���� ���� 
	D3DXVECTOR3	vDir( 0.0f, -1.0f, 0.0f );		
	if( D3DXVec3Dot( &vN, &vDir ) > fCosine )
	{
		vDir.y = -0.2f;									// �̲������� �ӵ� 
		CalcSlideVec( pOut, vDir, vN );					// �̲����� ���� ����
		return TRUE;
	}
	else
		return FALSE;
}

//
// �߷¹����� �浹�˻� �� �̲����� ó��.
// �߷¹����� ���ΰ� �浹�ϴ� �ﰢ���� ã�� �� ������ ��ǥ�� ��� Slide������� ��ǥ�� ����.
FLOAT CWorld::ProcessUnderCollision( D3DXVECTOR3 *pOut, CObj **ppObj, D3DXVECTOR3 &vPos )
{
	D3DXVECTOR3		vDir( 0.0f, -1.5f, 0.0f );	// Ű����(1.5)
	D3DXVECTOR3		vIntersect;
	float			fMaxY = -65535.0f;			
	D3DXVECTOR3		vOut( 0.0f, 0.0f, 0.0f );	
	CObj*			pObj;
	CModel*			pModel;
	int				nRange = 0;

	D3DXVECTOR3 vEnd( vPos + vDir );
	Segment3 segment( vPos, vEnd );
#if __VER >= 13 // __HOUSING

#if __VER >= 15 // __GUILD_HOUSE
	if(GetID() == WI_WORLD_MINIROOM || IsWorldGuildHouse() )
#else
	if(GetID() == WI_WORLD_MINIROOM )
#endif //__GUILD_HOUSE
	{
		FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkDynamic, nDefaultLayer )
		{

			// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
			pModel = pObj->m_pModel;
			if( pObj->GetType() == OT_CTRL ) 
			{
				if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
				{
					// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
					if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
					{
						if( vIntersect.y > fMaxY )
						{
							fMaxY = vIntersect.y;	// �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
							*pOut = vOut;
							*ppObj = pObj;			// �浹�� ������Ʈ.
						}
					}
				}
			}
		}
		END_LINKMAP
	}
#endif // __HOUSING
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
			{
				if( vIntersect.y > fMaxY )
				{
					fMaxY = vIntersect.y;	// �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
					*pOut = vOut;
					*ppObj = pObj;			// �浹�� ������Ʈ.
				}
			}
		}
	}
	END_LINKMAP

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkAirShip, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
			{
				if( vIntersect.y > fMaxY )
				{
					fMaxY = vIntersect.y;	// �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
					*pOut = vOut;
					*ppObj = pObj;			// �浹�� ������Ʈ.
				}
			}
		}
	}
	END_LINKMAP

	// �������Ϳ� ������ �浹���� ����.
	FLOAT fDist;
	D3DXVECTOR3 vLandTri[3];
	GetLandTri( vPos.x, vPos.z, vLandTri );		// ���� ��ġ�� ����Ʈ�� �ﰢ�� ����
	BOOL b = IsTouchRayTri( &vLandTri[0], &vLandTri[1], &vLandTri[2], &vPos, &vDir, &fDist );
	
#ifdef _DEBUG
	if( b == FALSE )
		Error( "ProcessUnderCollision - D3DXIntersectTri x:%f z:%f", vPos.x, vPos.z );
#endif
	vIntersect.x = vPos.x;
	vIntersect.y = (vPos.y + fDist * vDir.y);		// �������̿� �����ﰢ���� ������ - �̰��� ���̷ε� ���δ�.
	vIntersect.z = vPos.z;

	if( fMaxY > vIntersect.y )  // �������� ������ ���������� ������?
	{		
		if( pOut->y )			// �������� �Ʒ��� �̲��� �� �� ����.
		{
			if( pOut->y < vIntersect.y )
				pOut->y = vIntersect.y;
		}
	}
	else
	{
		*pOut = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		fMaxY = vIntersect.y;
	
		// �������� �̲��� - 112�� �̻� ������ ���� ��츸 �̲�������. 
		if( CanSlide( vLandTri[0], vLandTri[1], vLandTri[2], -0.37864f, &vDir ) )	 // -0.37864f = cos(112) 
		{
			D3DXVECTOR3 vTris[3 * 9 * 2];							// �ﰢ�� 54��
			int nCount = GetLandTris( vPos.x, vPos.z, vTris ) / 3;	// vPos������ 9�� �׸���ﰢ�� ����.
			// ���������� slide�������� �ﰢ���� �浹�ϴ°� �ִ���...
			D3DXVECTOR3* p = FindTouchLandTri( vTris, nCount, vIntersect, vDir, &fDist );	
			if( p )
				*pOut = vIntersect + vDir * fDist;
			else
				*pOut = vIntersect + vDir;
		}
	}

	return fMaxY;
}

#else // !__SLIDE_060502

//
// �߷¹����� �浹�˻� �� �̲����� ó��.
// �߷¹����� ���ΰ� �浹�ϴ� �ﰢ���� ã�� �� ������ ��ǥ�� ��� Slide������� ��ǥ�� ����.
// pType : �浹�� ������Ʈ�� Ÿ��. OT_OBJ�� OT_SHIP�̳�.
FLOAT CWorld::ProcessUnderCollision( D3DXVECTOR3 *pOut, CObj **pObjColl, D3DXVECTOR3 &vPos )
{
	D3DXVECTOR3 vDir( 0.0f, -1.5f, 0.0f );	// Ű����(1.5) +	������(0.5)
	D3DXVECTOR3 vIntersect, vEnd;
	float	fMaxY = -65535.0f;	// �浹�� ��ǥ�� ���� ���� ��ġ�� ã�� ����...
	D3DXVECTOR3 vOut( 0, 0, 0 );		// *pOut�� ���������� �Ѱ��� ��.

	CObj* pObj;
	CObj* pMinObj = NULL;
	CModel* pModel;
	int nRange = 0;
	//return GetLandHeight( vPos.x, vPos.z );
	vEnd = vPos + vDir;
	int		nCount = 0;
	Segment3 segment( vPos, vEnd );

#if __VER >= 13 // __HOUSING

#if __VER >= 15 // __GUILD_HOUSE
	if(GetID() == WI_WORLD_MINIROOM || IsWorldGuildHouse() )
#else
	if(GetID() == WI_WORLD_MINIROOM )
#endif //__GUILD_HOUSE
	{
		FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkDynamic, nDefaultLayer )
		{

			// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
			pModel = pObj->m_pModel;
			if( pObj->GetType() == OT_CTRL ) 
			{
				if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
				{
					// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
					if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
					{
						if( vIntersect.y > fMaxY )
						{
							fMaxY = vIntersect.y;	// �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
							*pOut = vOut;
							*pObjColl = pObj;			// �浹�� ������Ʈ.
						}
					}
				}
			}
		}
		END_LINKMAP
	}
#endif // __HOUSING
	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkStatic, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		nCount ++;
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
			{
				if( vIntersect.y > fMaxY )
				{
					fMaxY = vIntersect.y;	// // �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
					*pOut = vOut;
					*pObjColl = pObj;	// �浹�� ������Ʈ.
				}
			}
		}
	}
	END_LINKMAP

	FOR_LINKMAP( this, vPos, pObj, nRange, CObj::linkAirShip, nDefaultLayer )
	{
		// ����(vPos-vDir)�� ������ƮOBB�� �˻�.  
		nCount ++;
		pModel = pObj->m_pModel;
		if( pModel->TestIntersectionOBB_Line( segment, pObj ) == TRUE )
		{
			// �߷¹��� ���ΰ� �����ϴ� �ﰢ���� ã�� ������(����)�� ã�� �� �����̵� ���͸� vOut�� �޴´�.
			if( ((CModelObject *)pModel)->GetObject3D()->SlideVectorUnder( &vOut, vPos, vEnd, pObj->GetMatrixWorld(), &vIntersect ) == TRUE )
			{
				if( vIntersect.y > fMaxY )
				{
					fMaxY = vIntersect.y;	// // �浹�� �������߿� ���� ���� ���� ����....�̷��� �Ǽ� ���� ��������.
					*pOut = vOut;
					*pObjColl = pObj;	// �浹�� ������Ʈ.
				}
			}
		}
	}
	END_LINKMAP

	D3DXVECTOR3	v1, v2;
	// ���� ��ġ�� ����Ʈ�� �ﰢ�� ����
	// �������ͷ� ���ͼ�Ʈ ���� ���ϰ�(����) �����̵� ���.
	// 
	D3DXVECTOR3 pTri[3 * 9 * 2];		// �ﰢ�� 54��
	FLOAT	fDist;
	FLOAT	vx, vz;

	// �ٴ� �븻 ����
	vx = vPos.x;	vz = vPos.z;
	GetLandTri( vx, vz, pTri );
	v1 = pTri[1] - pTri[0];		// �븻 ��� ����.
	v2 = pTri[2] - pTri[0];
	D3DXVec3Cross( &v1, &v1, &v2 );		// �ٴ��� �븻 ���.
	D3DXVec3Normalize( &v1, &v1 );		// �������ͷ� ��ȯ

	// �������Ϳ� �ٴڸ��� �浹���� ����.
	BOOL b = IsTouchRayTri( &pTri[0], &pTri[1], &pTri[2], &vPos, &vDir, &fDist );
	vIntersect.x = vx;
	vIntersect.z = vz;
	vIntersect.y = (vPos.y + fDist * vDir.y); + 0.01f;		// �������̿� �ٴڻﰢ���� ������ - �̰��� ���̷ε� ���δ�.
	
#ifdef _XDEBUG
	if( b == FALSE )
		Error( "ProcessUnderCollision : Intersect����. ĳ������ ��ǥ�� ���� �����. %d %d", vPos.x, vPos.z );
#endif
	
	
	v2.x = v2.z = 0;	v2.y = -1.0f;	// �̰� �ݵ�� �������Ϳ��� �Ѵ�.
	// -0.19080f == cos(101)   -0.37864 == cos(112)
	D3DXVECTOR3 *p = NULL, vTemp9;
	int nMaxTri = 0;
	if( D3DXVec3Dot( &v1, &v2 ) > -0.37864f )	// dot > cos(theta) ������ �̻� ������ �鸸 �����̵带 �����Ų��. �κ��ʹ� �븻�̾�� �Ѵ�.
	{
		v2.y = -0.2f;
		CalcSlideVec( &vOut, v2, v1 );		// �̲����� ���� ����
		vTemp9 = vOut;
		nMaxTri = GetLandTris( vx, vz, pTri ) / 3;	// vPos������ 9�� �׸���ﰢ�� ����.
		p = FindTouchLandTri( pTri, nMaxTri, vIntersect, vOut, &fDist );	// ���������� slide�������� �ﰢ���� �浹�ϴ°� �ִ���...
		if( p )
			vOut = vIntersect + vOut * fDist;
		else
			vOut = vIntersect + vOut;

#ifdef __XUZHU
		if( fabs(vPos.z - vOut.z) > 10.0f )
		{
			int a=0;
		}
#endif
	}

	if( vIntersect.y > fMaxY )
	{
		fMaxY = vIntersect.y;
		*pOut = vOut;
	}

	return fMaxY;
}

#endif // not __SLIDE_060502