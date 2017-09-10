#include "stdafx.h"
#include "CreateObj.h"

	#include "..\_AIInterface\AIInterface.h"
#ifdef __WORLDSERVER
	#include "..\_AIInterface\AIMonster.h"
	#include "..\_AIInterface\AIMonster2.h"
	#include "..\_AIInterface\AIClockWorks.h"
	#include "..\_AIInterface\AIPet.h"
	#include "..\_AIInterface\AIBigMuscle.h"
	#include "..\_AIInterface\AIKrrr.h"
	#include "..\_AIInterface\AIBear.h"
	#if __VER >= 10	// __METEONYKER_0608
		#include "..\_AIInterface\aimeteonyker.h"
	#endif	// __METEONYKER_0608
#endif // __WORLDSERVER


//////////////////////////////////////////////////////////////////////
// class static member 
//////////////////////////////////////////////////////////////////////

BOOL		CObj::m_bCollision		= TRUE;
BOOL		CObj::m_bAnimate		= TRUE;
CObj*		CObj::m_pObjActive		= NULL;
CObj*		CObj::m_pObjHighlight	= NULL; 
int			CObj::m_nMethod			= METHOD_NONE;
//CRIT_SEC	CObj::m_csMethod;

#ifdef __CLIENT
	D3DXVECTOR3		g_vPosShip;
	int				g_ShipExp = FALSE;
#endif
//////////////////////////////////////////////////////////////////////
// CObj
//////////////////////////////////////////////////////////////////////

CObj::CObj()
#ifdef __LAYER_1015
:
m_nLayer( nDefaultLayer )
#endif	// __LAYER_1015
{
	m_dwType		= OT_OBJ;
	m_dwIndex		= 0;
	m_dwFlags       = 0;
	m_dwObjAryIdx	= 0xffffffff;
	m_pModel		= NULL;
	m_fAngle		= 0.0f;
	m_fAngX			= 0.0f;
	m_fAngZ			= 0.0f;				// Z�� ����
	
	m_vPos			= D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScale		= D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_pPrev			= NULL; // ���� ��� 
	m_pNext			= NULL; // ���� ��� 
	m_pWorld		= NULL;
	
	m_dwAIInterface      = 0;
	m_pAIInterface	     = NULL;
	m_dwAIInterfaceState = 2;

#ifdef __WORLDSERVER
	m_dwPatrolIndex      = NULL_ID;
	m_nPatrolIndexCount	 = 0;
	m_bPatrolCycle		 = 0;			// ��ü ��ȯ�̳�? ��->ó��->�� �����̳�
	m_bPatrolReverse	 = 0;		// ���� �������	
#endif	

	SetUpdateMatrix( TRUE );
	SetPosChanged( TRUE );

	m_dwLinkLevel	= 0;
	SetDelete( FALSE );

	D3DXMatrixScaling( &m_matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixRotationY( &m_matRotation, D3DXToRadian( -m_fAngle ) );
	D3DXMatrixTranslation( &m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixIdentity( &m_matWorld );

#ifdef __WORLDSERVER
	m_vRemoval		= m_vLink	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	SetVirtual( FALSE );
#else	// !__WORLDSERVER
	m_fScrAngle		= 0.0f;
	m_fScrAngleX	= 0.0f;
	m_ppViewPtr		= NULL;
    m_cullstate		= CS_UNKNOWN;
	m_wBlendFactor	= 255;
	m_pCamera		= NULL;
	m_fDistCamera	= 0.0f;
	m_dy			= 0.0f;
	m_nEvent		= 0;
	SetVisible( TRUE );
	
	#ifdef __YSMOOTH_OBJ	
		m_bSmooth       = FALSE;
		m_bSmoothOld    = FALSE;
		m_nSmoothCount  = 0;
	#endif //__YSMOOTH_OBJ
#endif	// !__WORLDSERVER
}

CObj::~CObj()
{
	// �ִϸ��̼��� ������ �͸� ���⼭ �ı��� �����ϴ�.
	// �ִϸ��̼��� �Ұ����� ���� CModelMng���� �����ϰ� �ı��Ѵ�.
	if( m_pModel && m_pModel->IsAniable() )
		SAFE_DELETE( m_pModel );

	SAFE_DELETE( m_pAIInterface );
#ifndef __WORLDSERVER
	SAFE_DELETE( m_pCamera );
#endif	// __WORLDSERVER
}

void CObj::InitProp( BOOL bInitAI )
{
}

ObjProp* CObj::GetProp() 
{ 
	return prj.GetProp( m_dwType, m_dwIndex ); 
}

void CObj::UpdateLocalMatrix(void)
{
	D3DXMatrixScaling( &m_matScale, m_vScale.x, m_vScale.y, m_vScale.z );
#if __VER >= 11 // __FIX_ROTATE
	D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( m_fAngZ ) );
#else
	D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( -m_fAngZ ) );
#endif
	
	D3DXMatrixTranslation( &m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
	UpdateMatrix();
}

void CObj::UpdateMatrix()
{
    // Recompute m_mat, m_vecBoundsWorld, and m_planeBoundsWorld 
    // when the thing's position, orientation, or bounding box has changed

	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &m_matScale );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &m_matRotation );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, &m_matTrans );

	if( m_pModel )
	{
		// OBB�� ������ ���� �����Ѵ�.
		m_OBB.UpdateMartix( m_pModel->m_vMin, m_pModel->m_vMax, m_matScale, m_matRotation, m_matWorld );
	}

#ifdef __CLIENT
	// Transform bounding box coords from local space to world space
	for( int i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &m_vecBoundsWorld[i], &m_vecBoundsLocal[i], &m_matWorld );

	// Determine planes of the bounding box
	D3DXPlaneFromPoints( &m_planeBoundsWorld[0], &m_vecBoundsWorld[0], 
		&m_vecBoundsWorld[1], &m_vecBoundsWorld[2] ); // Near
	D3DXPlaneFromPoints( &m_planeBoundsWorld[1], &m_vecBoundsWorld[6], 
		&m_vecBoundsWorld[7], &m_vecBoundsWorld[5] ); // Far
	D3DXPlaneFromPoints( &m_planeBoundsWorld[2], &m_vecBoundsWorld[2], 
		&m_vecBoundsWorld[6], &m_vecBoundsWorld[4] ); // Left
	D3DXPlaneFromPoints( &m_planeBoundsWorld[3], &m_vecBoundsWorld[7], 
		&m_vecBoundsWorld[3], &m_vecBoundsWorld[5] ); // Right
	D3DXPlaneFromPoints( &m_planeBoundsWorld[4], &m_vecBoundsWorld[2], 
		&m_vecBoundsWorld[3], &m_vecBoundsWorld[6] ); // Top
	D3DXPlaneFromPoints( &m_planeBoundsWorld[5], &m_vecBoundsWorld[1], 
		&m_vecBoundsWorld[0], &m_vecBoundsWorld[4] ); // Bottom
#endif // __CLIENT

	SetUpdateMatrix( FALSE );
}

// Parameters 
//   pObj : �߰��� ������Ʈ ������
//  
// Remarks  
//   ���� this ������ ������Ʈ�� �߰��Ѵ�.
//
void CObj::InsPrevNode(CObj* pObj)
{
	pObj->m_pNext = this;
	pObj->m_pPrev = m_pPrev;
	if(m_pPrev)
		m_pPrev->m_pNext = pObj;
	m_pPrev = pObj;
}
//
// Parameters 
//   pObj : �߰��� ������Ʈ ������
//  
// Remarks  
//   ���� this ������ ������Ʈ�� �߰��Ѵ�.
//
void CObj::InsNextNode(CObj* pObj)
{
	pObj->m_pNext = m_pNext;
	pObj->m_pPrev = this;
	if(m_pNext)
		m_pNext->m_pPrev = pObj;
	m_pNext = pObj;
}
//
// Remarks   
//   ������Ʈ�� ��� ���ῡ�� �����Ѵ�. 
//
void CObj::DelNode()
{
	if(m_pPrev)
	{	
		m_pPrev->m_pNext = m_pNext;
		if(m_pPrev->m_pNext == this)
			m_pPrev->m_pNext = NULL;
	}
	if(m_pNext)
	{
		m_pNext->m_pPrev = m_pPrev;
		if(m_pNext->m_pPrev == this)
			m_pNext->m_pPrev = NULL;
	}
	m_pNext = m_pPrev = NULL;
}

void CObj::Process()
{
#ifdef __CLIENT
	LPMODELELEM lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	if( lpModelElem == NULL )
		return;					// Model�� ������ ������� ���� 

	if( lpModelElem->m_dwModelType == MODELTYPE_SFX )
	{
		if( ((CSfxModel*)m_pModel)->Process() )
			((CSfxModel*)m_pModel)->m_nCurFrame = 0;
	}
	else
	{
		D3DXVECTOR3 vPos = GetPos();
		m_pModel->FrameMove( &vPos );		// ���弭������ CObj::m_pModel�� FrameMove�� �����ʿ� ����.
	}

	if( m_pModel->m_pModelElem->m_bTrans )
	{
		D3DXVECTOR3 vPosObj, vPosAct, vPosCam;
		float fLengthObj, fLengthAct;
		if( GetActiveObj() != NULL )
		{
			vPosCam = m_pWorld->m_pCamera->m_vPos;
			vPosObj = GetPos() - vPosCam;
			vPosAct = GetActiveObj()->GetPos() - vPosCam;
			fLengthObj = D3DXVec3LengthSq( &vPosObj );	// Length�� LengthSq�� �ٲ�. xuzhu
			fLengthAct = D3DXVec3LengthSq( &vPosAct );
		}		
		// ī�޶�� ���� ������ ���  
		if( fLengthObj < fLengthAct )
		{
			// ���� ī�޶� ���̿� �ִ� ������Ʈ�� ����üũ�Ͽ� �ɸ��� ������� �Ѵ�.
			FLOAT fDist;
			D3DXVECTOR3 vIntersect;
			D3DXVECTOR3 vPickRayDir = vPosAct;
			D3DXVec3Normalize( &( vPickRayDir ), &( vPickRayDir ) );
			if( Pick( &vPosCam, &vPickRayDir, &vIntersect, &fDist, FALSE, FALSE ) )
			{
				m_wBlendFactor-=10;
				if( m_wBlendFactor < 64 ) m_wBlendFactor = 64;
			}
			else
			{
				m_wBlendFactor+=10;
				if( m_wBlendFactor > 255 ) m_wBlendFactor = 255;
			}
		}
		else
		{
			m_wBlendFactor+=5;
			if( m_wBlendFactor > 255 ) m_wBlendFactor = 255;
		}
	} 
#endif // __CLIENT
}

void CObj::ProcessAI()
{
	if( m_pAIInterface && GetType() == OT_MOVER )
		m_pAIInterface->RouteMessage();
}

void CObj::SetOnLand()
{  
	if( GetModel()->m_pModelElem->m_bFly == FALSE )
	{
		D3DXVECTOR3 vPos = GetPos();
		vPos.y = m_pWorld->GetLandHeight( GetPos() );
		SetPos( vPos );
	}
}


void CObj::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
#ifndef __WORLDSERVER
	if( !IsVisible() || ( IsCull() && GetType() != 1 ))
		return;

	if( IsUpdateMatrix() )
		UpdateMatrix();

	// Model�� ������ ������� ���� 
	LPMODELELEM lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	if( lpModelElem == NULL )
		return;
	if( lpModelElem->m_dwModelType == MODELTYPE_SFX )
	{
		((CSfxModel*)m_pModel)->m_vPos = GetPos();
		((CSfxModel*)m_pModel)->m_vPos.y += 0.2f;
		((CSfxModel*)m_pModel)->m_vRotate.y = GetAngle();
		((CSfxModel*)m_pModel)->m_vScale = GetScale();
		((CSfxModel*)m_pModel)->m_matScale = m_matScale;
		((CSfxModel*)m_pModel)->Render( pd3dDevice, NULL );
		return;
	}

	CModel* pModel = m_pModel;
	DWORD dwModelType = pModel->GetModelType();
	
	pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	int wBlendFactor = m_wBlendFactor;
	if( m_pModel->m_nNoEffect )
		wBlendFactor = 255;

	m_pModel->SetBlendFactor( wBlendFactor );
	
	if( pModel->m_nNoEffect == 0 )
	{
		if( m_pWorld->m_bIsIndoor == 1 )
			pModel->SetGroup( 0 );
		else
		{
			int fDist = (int)( 50.0f );
			switch( g_Option.m_nObjectDetail )
			{
			case 0 :	fDist = (int)( 50.0f );	break;
			case 1 :	fDist = (int)( 30.0f );	break;
			case 2 :	fDist = (int)( 15.0f );	break;
			}
			int nLevel = (int)( m_fDistCamera / fDist );
			if( nLevel >= 2 )	nLevel = 2;
			if( nLevel < 0 )	
			{
				Error( "CObj::Render : %d, lod lv=%d %f", m_dwIndex, nLevel, m_fDistCamera );
				nLevel = 0;
			}
			
			pModel->SetGroup( nLevel );
		}
	} 
	else
	{	
		pModel->SetGroup( 2 );		// �׸��� ������ ���� ���� �ܰ�� ��´�.
	}

	if( pModel->GetType() == MODELTYPE_MESH || pModel->GetType() == MODELTYPE_ANIMATED_MESH )
	{
		if( ((CModelObject*)pModel)->m_pModelElem->m_nTextureEx > 0 )		// Ȯ���ؽ��� ���³��ΰ�?
			((CModelObject*)pModel)->SetTextureEx( ((CModelObject*)pModel)->m_pModelElem->m_nTextureEx );
		else
			((CModelObject*)pModel)->SetTextureEx( 0 );
	}
	
	pModel->Render( pd3dDevice, &m_matWorld ); 
#endif
}


CModel* CObj::LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwType, DWORD dwIndex ) 
{
	return prj.m_modelMng.LoadModel( pd3dDevice, dwType, dwIndex );
}

void CObj::ResetScale()
{
	m_vScale.x = m_pModel->m_pModelElem->m_fScale;
	m_vScale.y = m_pModel->m_pModelElem->m_fScale;
	m_vScale.z = m_pModel->m_pModelElem->m_fScale;
}

BOOL CObj::SetTypeIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwType, DWORD dwIndex, BOOL bInitProp )
{
	m_dwIndex = dwIndex;
	m_pModel = LoadModel( pd3dDevice, dwType, dwIndex );
	if( m_pModel ) 
	{ 
		if( bInitProp )
		{
			ResetScale();
			UpdateBoundBox();
			InitProp();
		}
		else
		{
#ifdef __WORLDSERVER
#if __VER >= 11 // __FIX_COLLISION
			if(dwType != OT_OBJ)
#endif
			ResetScale();
#endif //__WORLDSERVER
			UpdateBoundBox();
		}

		return TRUE; 
	} 

	return FALSE;
}

BOOL CObj::UpdateBoundBox()
{
	if( m_pModel == NULL ) 
		return FALSE;

	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	if( pBB )
	{
		//     - z
		//   3 | 2
		// - --+-- + x
		//   0 | 1
		//
		//   7 | 6
		// - --+-- + x
		//   4 | 5
	#ifdef __CLIENT
		m_vecBoundsLocal[0] = D3DXVECTOR3( m_pModel->m_vMax.x, m_pModel->m_vMax.y, m_pModel->m_vMax.z );
		m_vecBoundsLocal[1] = D3DXVECTOR3( m_pModel->m_vMax.x, m_pModel->m_vMax.y, m_pModel->m_vMin.z );
		m_vecBoundsLocal[2] = D3DXVECTOR3( m_pModel->m_vMax.x, m_pModel->m_vMin.y, m_pModel->m_vMax.z );
		m_vecBoundsLocal[3] = D3DXVECTOR3( m_pModel->m_vMax.x, m_pModel->m_vMin.y, m_pModel->m_vMin.z );
		m_vecBoundsLocal[4] = D3DXVECTOR3( m_pModel->m_vMin.x, m_pModel->m_vMax.y, m_pModel->m_vMax.z );
		m_vecBoundsLocal[5] = D3DXVECTOR3( m_pModel->m_vMin.x, m_pModel->m_vMax.y, m_pModel->m_vMin.z );
		m_vecBoundsLocal[6] = D3DXVECTOR3( m_pModel->m_vMin.x, m_pModel->m_vMin.y, m_pModel->m_vMin.z );
		m_vecBoundsLocal[7] = D3DXVECTOR3( m_pModel->m_vMin.x, m_pModel->m_vMin.y, m_pModel->m_vMax.z );
	#endif // __CLIENT
		UpdateLocalMatrix();
	}

#if !defined(__WORLDSERVER)
	CalcLinkLevel();
#endif	// !defined(__WORLDSERVER)

	return TRUE;
}


#ifdef __BS_ADDOBJATTR_INVISIBLE
enum FD_OBJ_ATTR	 // data�� ���� �б⿡�� �����ϴ� �Ӽ���
{
	FOA_Invisible_Collision = 0x00010000,		// �浹 0, ���� x
	FOA_Invisible = 0x00020000,					// �浹 x, ���� x
};
#endif

BOOL CObj::Read( CFileIO* pFile )
{
	D3DXVECTOR3		vAxis;  // ������� �ʴ´�.
	DWORD			dwMotion;			
	DWORD			dwAIIntreface;
	DWORD			dwAI2;

	pFile->Read( &m_fAngle,      sizeof( m_fAngle ) );
	pFile->Read( &vAxis,         sizeof( vAxis ) );
	pFile->Read( &m_vPos,        sizeof( m_vPos ) );
	pFile->Read( &m_vScale,      sizeof( m_vScale ) );
	pFile->Read( &m_dwType,      sizeof( m_dwType ) ); 

	//gmpbigsun: m_dwType�� �߰������� Invisible collision �Ӽ��� �߰��ƴ� ( since 2009_08_13 ), ���������Ϳ� ȣȯ 

#ifdef __BS_ADDOBJATTR_INVISIBLE
	if( m_dwType & FOA_Invisible_Collision )		// �Ӽ��� �߰��� ��츸!!
	{
		m_dwType &= ~FOA_Invisible_Collision;
		SetVisible( FALSE );
	}

	if( m_dwType & FOA_Invisible )
	{
		m_dwType &= ~FOA_Invisible;
		SetVisible( FALSE );
	}
#endif

	pFile->Read( &m_dwIndex,     sizeof( m_dwIndex ) );
	pFile->Read( &dwMotion,      sizeof( dwMotion ) );
	pFile->Read( &dwAIIntreface, sizeof( dwAIIntreface ) );
	pFile->Read( &dwAI2,         sizeof( dwAI2 ) );

	m_fAngX = vAxis.x;
	m_fAngZ = vAxis.z;
	
	InitProp( FALSE );		// FALSE - AI��������  SetAIInterface( pProp->dwAI );
	if( m_dwType == OT_MOVER )
	{
		((CMover* )this)->m_dwMotion = dwMotion;
	#ifdef __WORLDSERVER
		SetAIInterface( dwAIIntreface, dwAI2 );
	#endif 
	}

	// ���� ������ ���ؼ� ���� �� �����Ѵ�. �̴� MPU�� �ٲ��� �� ������Ʈ����
	// �ٲ� ���� ��ǥ ������ �������ǰ� �ϱ� ���� ���̴�. ���� ��ũ ����ÿ��� �ʿ��ϴ�.

	// gmpbigsun : �̸̹������ �ʿ� ���ؼ� ���������� ���� ���� ������ ���Ѵ�� ���������ϱ��� ����� �������� �������� ...���̴�.
	//				���� ������ ������ OLD_MPU�� �������� �ܼ��� �������� �����Ѵ�.
	m_vPos.x	*= FLOAT( OLD_MPU );
	m_vPos.z	*= FLOAT( OLD_MPU );

#ifdef __WORLDSERVER
	m_vLink		= m_vPos;
#endif	// __WORLDSERVER

	return TRUE;
}

void CObj::SetPos( const D3DXVECTOR3& vPos )      
{ 
	if( m_vPos == vPos )
		return;

	SetPosChanged( TRUE );		// ��ǥ�� ����Ǿ����� ���� 

	CWorld* pWorld = GetWorld();
#ifdef __WORLDSERVER
	if( pWorld )
	{
		if( pWorld->VecInWorld( vPos ) == FALSE )
			return;

		D3DXVECTOR3	vOld	= GetLinkPos() / (float)( pWorld->m_iMPU );
		D3DXVECTOR3 vCur	= vPos / (float)( pWorld->m_iMPU );
		if( (int)vOld.x != (int)vCur.x || (int)vOld.z != (int)vCur.z )
		{
			if( m_vRemoval == D3DXVECTOR3( 0, 0, 0 ) )
			{
				pWorld->m_apModifyLink[pWorld->m_cbModifyLink++]	= this;
				if( pWorld->m_cbModifyLink >= MAX_MODIFYLINK )
					Error( "MODIFYLINK//MAX//%d", pWorld->m_cbModifyLink ); 
			}

			m_vRemoval = vPos;
			m_vPos	= vPos;
		}
		else 
		{
			m_vPos  = vPos;
			m_vLink	= vPos;

			if( m_vRemoval != D3DXVECTOR3( 0, 0, 0 ) )
			{
				for( int i = 0; i < pWorld->m_cbModifyLink; i++ )
				{
					if( pWorld->m_apModifyLink[i] == this )
					{
						pWorld->m_apModifyLink[i]	= NULL;
						m_vRemoval	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
						break;
					}
				}
			}
		}
	}
	else 
	{
		m_vPos	= vPos;
		m_vLink	= m_vPos;
	}
#else	// __WORLDSERVER
	if( pWorld )
	{
		if( pWorld->VecInWorld( vPos ) == FALSE )
			return;

		D3DXVECTOR3	vOld	= GetPos() / (FLOAT)( MPU );
		D3DXVECTOR3 vCur	= vPos / (FLOAT)( MPU );
		if( (int)vOld.x != (int)vCur.x || (int)vOld.z != (int)vCur.z )
		{
			int nOldX = int( vOld.x / MAP_SIZE );
			int nOldZ = int( vOld.z / MAP_SIZE );
			int nCurX = int( vCur.x / MAP_SIZE );
			int nCurZ = int( vCur.z / MAP_SIZE );   
			if( nOldX != nCurX || nOldZ != nCurZ )
			{
				CLandscape* pOldLandscape	= pWorld->m_apLand[ nOldX + nOldZ * pWorld->m_nLandWidth ];
				if( NULL != pOldLandscape )
				{
					pOldLandscape->RemoveObjLink( this );
					pOldLandscape->RemoveObjArray( this );
				}
				m_vPos = vPos;
				CLandscape* pCurLandscape	= pWorld->m_apLand[ nCurX + nCurZ * pWorld->m_nLandWidth ];
				if( NULL != pCurLandscape )
				{
					pCurLandscape->InsertObjLink( this );
					pCurLandscape->AddObjArray( this );
				}
			}
			else
			{
				// ���� Landscape �ȿ����� �̵� 
				int nWidth = 0;
				if(m_pModel) 
					nWidth	= (int)( m_pModel->GetMaxWidth() );
				DWORD dwLinkType	= GetLinkType();
				if( pWorld->GetObjInLinkMap( m_vPos, dwLinkType, m_dwLinkLevel ) == this )
					pWorld->SetObjInLinkMap( m_vPos, dwLinkType, m_dwLinkLevel, m_pNext );
				DelNode();
				CObj* pObj	= pWorld->GetObjInLinkMap( vPos, dwLinkType, m_dwLinkLevel );
				if( pObj )
					pObj->InsNextNode( this );
				else
					pWorld->SetObjInLinkMap( vPos, dwLinkType, m_dwLinkLevel, this );
				m_vPos	= vPos; 
			}
		}
		else 
		{
			m_vPos	= vPos;
		}
	}
	else 
	{
		m_vPos	= vPos;
	}
#endif	// __WORLDSERVER

#ifndef __CLIENT
	D3DXMatrixTranslation( &m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
	SetUpdateMatrix( TRUE );
#else	// !__CLIENT
	if( GetType() != OT_MOVER )
	{
		D3DXMatrixTranslation( &m_matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
		SetUpdateMatrix( TRUE );
	}
#endif	// __CLIENT

	return;
}

void CObj::AddAngle( FLOAT fAngle )
{
	SetAngle( m_fAngle + fAngle );
}

void CObj::SetAngle( FLOAT fAngle )
{ 
	m_fAngle = (float)( (360.0/65536) * ((int)(fAngle*(65536/360.0)) & 65535) );

	FLOAT fAngZ = 0.0f;
	fAngZ = -m_fAngZ;

#if !defined(__CLIENT)
	D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( fAngZ ) );
	SetUpdateMatrix( TRUE );
#else	// not __CLIENT
	
	// OT_MOVER�� interpolate���� 
	if( GetType() != OT_MOVER )
	{
		D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( fAngZ ) );
		SetUpdateMatrix( TRUE );
	}
#endif	// __CLIENT
}


void CObj::AddAngleX( FLOAT fAngle )
{
	m_fAngX += fAngle;
	SetAngleX( m_fAngX ); 
}

void CObj::SetAngleX( FLOAT fAngX )
{ 
	m_fAngX	= fAngX;
	
	FLOAT fAngZ = 0.0f;
	fAngZ = -m_fAngZ;
		
#if !defined(__CLIENT)
	D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( fAngZ ) );
	SetUpdateMatrix( TRUE );
#else	// not __CLIENT
	if( GetType() != OT_MOVER )
	{
		D3DXMatrixRotationYawPitchRoll( &m_matRotation, D3DXToRadian( -m_fAngle ), D3DXToRadian( -m_fAngX ), D3DXToRadian( fAngZ ) );
		SetUpdateMatrix( TRUE );
	}
#endif	// __CLIENT
}

void CObj::SetScale( const D3DXVECTOR3& vScale )      
{ 
	m_vScale = vScale;
	D3DXMatrixScaling( &m_matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	SetUpdateMatrix( TRUE );
}

#ifdef __WORLDSERVER
// TODO_raiders: dwState����, �Լ��� mover�� �ű��� 
void CObj::SetAIInterface( DWORD dwAIInterface, DWORD dwState )
{
	if( m_dwAIInterface == dwAIInterface && m_dwAIInterfaceState == dwState )
		return;
	
	SAFE_DELETE( m_pAIInterface );
	m_pAIInterface = NULL;

	switch( dwAIInterface )
	{
		case AII_MONSTER:		m_pAIInterface = new CAIMonster( this );	break;
		case AII_CLOCKWORKS:	m_pAIInterface = new CAIClockWorks( this );	break;
		case AII_PET:			m_pAIInterface = new CAIPet( this );		break;
		case AII_BIGMUSCLE:		m_pAIInterface = new CAIBigMuscle( this );	break;
		case AII_KRRR:			m_pAIInterface = new CAIKrrr( this );		break;
		case AII_BEAR:			m_pAIInterface = new CAIBear( this );		break;
		case AII_VER2_TYPE0:	m_pAIInterface = new CAIMonster2( this );	break;
#if __VER >= 10	// __METEONYKER_0608
		case AII_METEONYKER:	m_pAIInterface	= new CAIMeteonyker( this );	break;
#endif	// __METEONYKER_0608
	}

	m_dwAIInterface		 = dwAIInterface;
	m_dwAIInterfaceState = dwState;
}
#endif // __WORLDSERVER


void CObj::Delete()
{
	m_pWorld->DeleteObj( this );
}

#ifdef __CLIENT
BOOL CObj::Pick( D3DXVECTOR3* pvPickRayOrig, D3DXVECTOR3* pvPickRayDir, D3DXVECTOR3* pvIntersect, FLOAT* pfDist, BOOL bOnlyBoundBox, BOOL bColl )
{
	if( m_pModel->IntersectBB( *pvPickRayOrig, *pvPickRayDir, GetMatrixWorld(), pvIntersect, pfDist ) == TRUE )	
	{
		if( bOnlyBoundBox == TRUE )
			return TRUE;
		if( m_pModel->Intersect( *pvPickRayOrig, *pvPickRayDir, GetMatrixWorld(), pvIntersect, pfDist, bColl ) == TRUE )
			return TRUE;
	}
	return FALSE;
}
#endif


// this�� �������� ����� - XZ����� ũ�⸸.
FLOAT	CObj::GetRadiusXZ( void )
{
	float	fLenX = fabs(m_pModel->m_vMax.x - m_pModel->m_vMin.x);
	float	fLenZ = fabs(m_pModel->m_vMax.z - m_pModel->m_vMin.z);
	
	float	fRadius = ((fLenX + fLenZ) / 2.0f) / 2.0f;		// this(Dest)���� ���(!) ������.
	fRadius *= m_vScale.x;		// ������ ����.
	
	return fRadius;
}

const float INV_6 = 1.0f / 6.0f;

// this�� �������� �����. = ��� ����� ���������� ���� 
FLOAT	CObj::GetRadius( void )
{
	float fRadius = fabs(m_pModel->m_vMax.x - m_pModel->m_vMin.x);
	fRadius      += fabs(m_pModel->m_vMax.y - m_pModel->m_vMin.y);
	fRadius      += fabs(m_pModel->m_vMax.z - m_pModel->m_vMin.z);
	
	fRadius *= INV_6;			// 3�� ����� ���� ������ = ((x + y + z) / 3.0f) / 2.0f 
	fRadius *= m_vScale.x;		// ������ ����.

	return fRadius;
}

// this�� �ݰ� + m_nArrivalRange ���� pObj�� �߽����� �����ִ°�?
BOOL	CObj::IsRangeObj( CObj *pOther, float fRange )
{
	if( m_pModel == NULL )	
		return FALSE;

	if( fRange == 0.0f )
		fRange = 1.0f;		

	float fRadius = GetRadius();
	fRadius *= 0.8f;							// ��� ������θ� �ϴϱ� �� Ŀ�� ���� �ٿ���.

	float fOther = pOther->GetRadius();
	fOther *= 0.8f;								// ��� ������θ� �ϴϱ� �� Ŀ�� ���� �ٿ���.

	fRadius += (fOther + fRange);

	D3DXVECTOR3 vDist = GetPos() - pOther->GetPos();	// ����ǥ�� ����
	float fDistSq = D3DXVec3LengthSq( &vDist );			// �ο�����Ʈ���� �Ÿ�Sq
	
	if( fDistSq < (fRadius * fRadius) )					// �������� �Ÿ��� �������� ������ �浹
		return TRUE;
	
	return FALSE;
}

//
// this(src) + m_nArrivalRange �� �ݰ� ���� vDest�� �߽����� �����ִ°�?
//
BOOL	CObj::IsRangeObj( const D3DXVECTOR3 &vDest, float fRange )
{
	if( m_pModel == NULL )	return FALSE;

	float	fRadius = 1.0f;
	fRadius *= 0.8f;							// ��� ������θ� �ϴϱ� �� Ŀ�� ���� �ٿ���.

	float fRadiusSrc = GetRadius();
	fRadiusSrc *= 0.8f;	

	fRadiusSrc += fRange;						// pSrc������������ m_nArrivalRange�� ����.
	fRadius += fRadiusSrc;
	
	D3DXVECTOR3 vDist = vDest - GetPos();				// ����ǥ�� ����
	float fDistSq = D3DXVec3LengthSq( &vDist );			// �ο�����Ʈ���� �Ÿ�Sq
	
	if( fDistSq < fRadius * fRadius )					// �������� �Ÿ��� �������� ������ �浹
		return TRUE;
	
	return FALSE;
}


//
//	���༱
//
void	CObj::ProcessAirShip( void )
{
#ifdef __CLIENT
	D3DXVECTOR3	vPos = GetPos();

	switch( m_nEvent )
	{
	case 0:		// ��� ����
		if( g_ShipExp == TRUE )
		{
			g_vPosShip = m_vPos;
			m_nEvent ++;
			m_nCnt = 0;
		}
		break;
	case 1:		// ���� ����
		{
			int		i;
			D3DXVECTOR3 vPos = GetPos();
			for( i = 0; i < 10; i ++ )
			{
				CObj* pObj = new CObj;
				vPos = GetPos();
				vPos.x += (10 - xRandom(20));
				vPos.y += (xRandom(20));
				vPos.z += (10 - xRandom(20));
				pObj->m_dwType = 1;
				pObj->SetScale( D3DXVECTOR3(50, 50, 50) );
				pObj->SetPos( vPos );
				pObj->SetIndex( g_Neuz.m_pd3dDevice, 22 );
				pObj->UpdateLocalMatrix();
				m_pWorld->AddObj( pObj );
			}
			PLAYSND( "CtrlCraftCrash.wav" , NULL );// ������ ���� �÷���
		}
		m_nEvent ++;
		break;
	case 2:	// ������.
		{
			float	h = m_pWorld->GetLandHeight( vPos.x, vPos.z );
			m_fAngX += 0.1f;
			SetAngleX( m_fAngX );
			if( m_dy > -2.0f )	m_dy -= 0.001f;
			vPos.y += m_dy;
			if( vPos.y - 10.0f < h )		// �ٴڿ� ó����
			{
				m_nEvent ++;
				m_pWorld->m_pCamera->SetQuake( 60 );
				PLAYSND( "CtrlCraftCrash.wav" );	// ������ ���� �÷���
			}
			SetPos( vPos );
			if( (m_nCnt & 7) == 0 )
			{
				CObj* pObj = new CObj;
				vPos = GetPos();
				vPos.x += (FLOAT)(160 - xRandom(320)) / 10.0f;
				vPos.y += (FLOAT)(xRandom(20)) + 10;
				vPos.z += (FLOAT)(160 - xRandom(320)) / 10.0f;
				pObj->m_dwType = 1;
				pObj->SetScale( D3DXVECTOR3(50, 50, 50) );
				pObj->SetPos( vPos );
				pObj->SetIndex( g_Neuz.m_pd3dDevice, 22 );
				pObj->UpdateLocalMatrix();
				m_pWorld->AddObj( pObj );
			}
			if( (m_nCnt & 15) == 0 && m_nCnt < 100 )
			{
				PLAYSND( "CtrlAirmine.wav", NULL );// ������ ���� �÷���
			}

		}
		break;
	case 3:	// �ٴڿ� ó����
		if( g_ShipExp == 0 )
		{
			m_nEvent = 0;
			SetPos( g_vPosShip );
			SetAngleX( 0 );
			m_dy = 0;
		}
		break;
	}
	m_nCnt ++;
#endif
}

#ifdef __CLIENT
#endif	// __CLIENT

DWORD CObj::GetLinkType( void )
{
	switch( m_dwType )
	{
		case OT_OBJ:
			return CObj::linkStatic;
		case OT_SHIP:
			return CObj::linkAirShip;
		case OT_MOVER:
			if( ( (CMover*)this )->IsPlayer() == TRUE )
				return CObj::linkPlayer;
		case OT_CTRL:
		case OT_SFX:
		case OT_ITEM:
			return CObj::linkDynamic;
	}
	ASSERT( 0 );
	return( -1 );
}

void CObj::RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor )
{
	return;
}

void CObj::SetWorld( CWorld* pWorld )	
{ 
	m_pWorld = pWorld; 
#ifdef __WORLDSERVER
	if( m_pWorld == NULL )
		return;

	CalcLinkLevel();	
#endif //  __WORLDSERVER	
}

// ��ũ������ �����Ѵ�.
void CObj::CalcLinkLevel()
{
	if( m_pModel == NULL )		// ��ų���� ���� ���������Ʈ������ ���� ���� ������Ʈ�� �����.
	{
		m_dwLinkLevel = 0;
		return;
	}

	float fThisWidth = 0.0f;
	if( m_pModel->IsNullBoundBox() == FALSE )
	{
		D3DXMATRIX mWorld = GetMatrixWorld();
		D3DXVECTOR4 vMin, vMax;
		D3DXVec3Transform( &vMin, &m_pModel->m_vMin, &mWorld );	
		D3DXVec3Transform( &vMax, &m_pModel->m_vMax, &mWorld );	
		fThisWidth =( fabs(vMax.x - vMin.x) > fabs(vMax.z - vMin.z) ) ? vMax.x - vMin.x : vMax.z - vMin.z; 
		fThisWidth = fabs( fThisWidth );
	}

#ifdef __WORLDSERVER
	#ifdef __LAYER_1015
		m_dwLinkLevel = m_pWorld->m_linkMap.CalcLinkLevel( this, fThisWidth, GetLayer() );
	#else	// __LAYER_1015
		m_dwLinkLevel = m_pWorld->m_linkMap.CalcLinkLevel( this, fThisWidth );
	#endif	// __LAYER_1015
#else

	int nWidthLink, nWidthPerGrid;
	nWidthLink	= MAP_SIZE * 2;
	int i = NULL;
	for( ; i < MAX_LINKLEVEL; i++ )
	{
		// 16	// 8	// 4	// 2	// 1	// 1	-	// 32	// 64	// 128	// 256	// 512	// 1024
		// 128	// 64	// 32	// 16	-	// 4	// 8	// 16	// 32
		nWidthLink	/= 2;
		if( nWidthLink == 0 )	
			nWidthLink	= 1;

		nWidthPerGrid	= MAP_SIZE * MPU / nWidthLink;
		if( fThisWidth < (float)nWidthPerGrid )
			break;
	}

	if( i == MAX_LINKLEVEL )
		i = MAX_LINKLEVEL - 1;

	m_dwLinkLevel	= i;
#endif // !__WORLDSERVER
}


