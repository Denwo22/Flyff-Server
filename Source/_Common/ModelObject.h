#ifndef		__MODELOBJECT_H__
#define		__MODELOBJECT_H__

#include "Object3D.h"
#include "Bone.h"
#include "xuzhu.h"
#include "xUtil3D.h"
#include "ModelGlobal.h"
#ifdef __ATTACH_MODEL
#include <boost/shared_ptr.hpp>
#endif //__ATTACH_MODEL

#define		MAX_SF_SLERP		3		// Ű��Ű ���̸� ��ܰ�� ������ ���ΰ�
#define		MAX_SF_SWDFORCE		512
#define		MAX_SF_SPLINE		4096

struct FVF_SWDFORCE
{
    D3DXVECTOR3 position;	// The 3D position for the vertex
    DWORD		color;		// The vertex color
};
#define	D3DFVF_SWDFORCE			(D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CSwordForce
{
public:
	int		m_nMaxVertex;
	int		m_nMaxSpline;
	int		m_nMaxDraw;			// ���ö��� ����Ʈ�߿� �׸� ����Ʈ
	DWORD	m_dwColor;			// �˱���.
	FVF_SWDFORCE	m_aList[ MAX_SF_SWDFORCE ];
	FVF_SWDFORCE	m_aList2[ MAX_SF_SWDFORCE ];
	FVF_SWDFORCE	m_aSpline[ MAX_SF_SPLINE ];

	CSwordForce() { Init(); }
	~CSwordForce() { Destroy(); }

	void	Init( void )
	{
		m_nMaxVertex = 0;
		m_nMaxSpline = 0;
		m_nMaxDraw = 0;
		m_dwColor = D3DCOLOR_ARGB( 255, 120, 120, 230 );
		memset( m_aList, 0, sizeof(FVF_SWDFORCE) * MAX_SF_SWDFORCE );
		memset( m_aSpline, 0, sizeof(FVF_SWDFORCE) * MAX_SF_SPLINE );
	}
	void	Destroy( void )
	{
		Init();
	}

	void	Clear( void )
	{
		m_nMaxVertex = 0;
		m_nMaxSpline = 0;
		m_nMaxDraw = 0;
	}
	BOOL	IsEmpty( void ) { (m_nMaxSpline == 0) ? TRUE : FALSE; }

	void	Add( D3DXVECTOR3 v1, D3DXVECTOR3 v2 );
	void	Process( void );
	void	Draw( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld );
	void	MakeSpline( void );

};

//
// ��ü�� �����ϴ� o3d��.  Element�� Ī����.
//
struct O3D_ELEMENT
{
	int				m_nParentIdx;
	CObject3D		*m_pObject3D;
	D3DXMATRIX		m_mLocalTM;				// m_pParent�κ����� LocalTM. �θ� ���Ƿ� �ٲ�� �ֱⶫ�� ���� ����.
	D3DXMATRIX		m_mLocalRH;				// �����տ� ��� ������ ���ø�Ʈ����
	D3DXMATRIX		m_mLocalLH;				// �޼տ� ��� ������ ���ø�Ʈ����
	D3DXMATRIX		m_mLocalShield;			// ������ġ�� ���� ���� "
	D3DXMATRIX		m_mLocalKnuckle;		// ��Ŭ��ġ�� ���� ���� "

	// CModelObject���� ���������� ������ �ִ� m_ppd3d_VB�� 
	// ��Ű�� ������Ʈ�� ó���� ���� �������� ���ؽ����� �����̴�.  
	// CObject3D�� �ε��Ǹ鼭 �̰��� ����ũ ������ ������ �ִٸ�
	// CObject3D���� m_ppd3d_VB�� ���ؽ� ���۸� ���� ���� �ʰ�.
	// �� ���� ���̳��ͷ����� CModelObject�� m_ppd3d_VB�� ���ؽ� ���۸� �����ϰ� �� ������ �̰��� ����Ÿ ������ �Ѵ�.
	// ���ؽ������� ����ִ� �޸� ���۴� �׻� CObject3D���� ���� �ְ�(read only��)
	// ����ƽ ������Ʈ�ΰ� ���̳��� ������Ʈ�ΰ��� ���� ���ؽ� ���۴� CObject3D�� ���� �ְų� CModelObject�� ���� �ְ� �ȴ�.
	// 
	LPDIRECT3DVERTEXBUFFER9		*m_ppd3d_VB;	// ���� Ǯ - ���ﶩ �̰͸� ������ �ȴ� .
	LPDIRECT3DVERTEXBUFFER9		*m_ppd3d_VB1;	// ��Ų������Ʈ�� ���� �������� ���ؽ����� m_pObject3D->m_nMaxObject��ŭ �����ȴ�. - ���� ������ �ȵȴ�.
	LPDIRECT3DVERTEXBUFFER9		*m_ppd3d_VB2;	// LOD2
	LPDIRECT3DVERTEXBUFFER9		*m_ppd3d_VB3;	// LOD3
	LPDIRECT3DVERTEXBUFFER9		*m_ppd3d_VBSel;	// ���� ���õ� ����
	LPDIRECT3DTEXTURE9			m_pTextureEx;	// �߰� �ؽ���.(��Ƽ�ؽ��ķ� ���� �������� ����.) �ؽ��Ľ�������1���� ����.
	MATERIAL					m_Mtrl;			// mes�� ���Ե� �ؽ����̿ܿ� �ؽ��ĸ� ���� ����.  ���⿡ ���� ���ٸ� ����Ʈ���� ����.
	int							m_nEffect;		// �� ������ �ܺ����� ����Ʈ ����.
	int							m_nPartsIdx;
};

#define		MAX_ELEMENT		21

#ifdef __ATTACH_MODEL
class CModelObject;
typedef	boost::shared_ptr<CModelObject>	SpModelObject;
typedef	map<int, SpModelObject> MapSpModelObject;
#endif //__ATTACH_MODEL

// �ִϸ��̼� ��ü. ���ӳ� ������Ʈ�� �ּҴ���.
// ����� ���⿡ ���ԵǾ� �ֱ⶧���� �������� �Ϲ��̵� �ִϸ��̼��� ��Ų�ٸ�
// �ݵ�� �̰����� �������Ѽ� ����.
// �������� ������ǥ�� ������.
class CModelObject : public CModel
{
public:
	BOOL				m_bMotionBlending;				// ��� ���� �÷���
	float				m_fBlendWeight;					// ���Ʈ�������� ����ġ 0.0���� 1.0���� �Ǹ� ������.
	D3DXMATRIX			*m_pBaseBoneInv;				// InverseTM - destroy���� ����
	BOOL				m_bCollObj;						// �浹�޽ø� �����ϰ� �ִ°�?
	LPDIRECT3DTEXTURE9	m_pCloakTexture;				// �ܺ� ���� ���� �ؽ��� 
	O3D_ELEMENT			m_Element[ MAX_ELEMENT ];		// ��ü�� �����ϴ� o3d��.  Element�� Ī����.
#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT
	int					m_nEffectCount;
#endif //__Y_ADV_ENCHANT_EFFECT
	
public:
	// Bone / animation
	// ���� Motion�� Element�ȿ� �־�� �´°������� ������ ����ȭ �ϱ� ���ؼ� 
	// CModelObject�� ���´�.  1���� n�޽� ������ �� �����ϱⰡ ���� �����̴�.
	CBones				*m_pBone;						// ���� Ŭ���� - �̰��� ���� �ı���Ű�� �ȵȴ�.
	CMotion				*m_pMotionOld;					// ���� ���
	CMotion				*m_pMotion;						// ���� �ε��� ��� ������ - ���� �ı���Ű�� �ȵȴ�.
	CSwordForce			*m_pForce;						// �˱�
	CSwordForce			*m_pForce2;						// �˱�-�޼�
	int					m_nFrameOld;					// ���������� �����ӹ�ȣ
	D3DXMATRIX			*m_mUpdateBone;					// ������ �ִϸ��̼� ����� ��ȯ�� ��Ʈ���� - ���� ������ŭ ������ - ���밡 ���� ������Ʈ�� �̰��� ������� ����.
	CPartsEffect		*m_pPartsEffect;				// ������ �ٴ� ����Ʈ.
	CPartsEffect		*m_pPartsEffect2;				// �޼� ���⿡ �ٴ� ����Ʈ.
	
#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT
	CPartsEffect		*m_pPartsEffect1_Detail;				// ������ �ٴ� ����Ʈ.
	CPartsEffect		*m_pPartsEffect2_Detail;				// �޼� ���⿡ �ٴ� ����Ʈ.
#endif //__Y_ADV_ENCHANT_EFFECT

	D3DXVECTOR3			m_vForce1, m_vForce2;
#if __VER >= 9 // __CSC_VER9_5
	D3DXVECTOR3			m_vForce3, m_vForce4;
#endif //__CSC_VER9_5

#ifdef __ATTACH_MODEL
private:
	MapSpModelObject	m_mapAttachModel;
#endif //__ATTACH_MODEL

private:
	void				Init( int nParts );
	void				Init( void );
	void				Destroy( void );
	BOOL				Destroy( int nParts );

public:
	CModelObject();
	virtual ~CModelObject();

	void				CreateForce( int nParts );		// �˱�Ŭ���� ����
	int					GetRHandIdx( void ) { return m_pBone->m_nRHandIdx; }
	int					GetLHandIdx( void ) { return m_pBone->m_nLHandIdx; }
	int					GetRArmIdx( void ) { return m_pBone->m_nRArmIdx; }
	int					GetLArmIdx( void ) { return m_pBone->m_nLArmIdx; }

#ifdef __ATTACH_MODEL
	void				SetAttachModel(int nEventIndex, CModelObject* pModelObject);
	void				SetDetachModel(int nEventIndex);
	void				RenderAttachModel(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld);
	int					RenderAttachModelElem(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld);
	void				FrameMoveAttachModel( D3DXVECTOR3 *pvSndPos = NULL, float fSpeed = 1.0f );

	void				InitAttachModelDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	void				RestoreAttachModelDeviceObjects();
	void				InvalidateAttachModelDeviceObjects();
	void				DeleteAttachModelDeviceObjects();
#endif //__ATTACH_MODEL
#if __VER >= 14 // __WING_ITEM
	CString				GetMotionFileName( const CString& rstrMotion ) const;
#endif // __WING_ITEM

	O3D_ELEMENT	*GetParts( int nParts ) 
	{ 
		for( int i=0; i<MAX_ELEMENT; i++ )
		{
			if( m_Element[i].m_nPartsIdx == nParts )
				return &(m_Element[i]);
		}
		return NULL; 
	}

	int	GetPartsIdx( int nParts ) 
	{ 
		for( int i=0; i<MAX_ELEMENT; i++ )
		{
			if( m_Element[i].m_nPartsIdx == nParts )
				return i;
		}
		return -1; 
	}
	
	CObject3D	*GetObject3D( void ) { return m_Element[0].m_pObject3D; }		// �̰��� ���ϸ޽����� ����.

	CObject3D	*GetObject3D( int nNum ) 
	{ 
		for( int i=0; i<MAX_ELEMENT; i++ )
		{
			if( m_Element[i].m_nPartsIdx == nNum )
				return m_Element[i].m_pObject3D;
		}
		return NULL; 
	}		
	void	SetExtTexture( LPDIRECT3DTEXTURE9 pTexture ) { m_pCloakTexture = pTexture; }
	void	GetForcePos( D3DXVECTOR3 *vOut, int nIdx, int nParts, const D3DXMATRIX &mWorld );
	void	GetForcePos( D3DXVECTOR3 *vOut, int nIdx, int nParts, const D3DXMATRIX &mWorld, float fScale);
	void	GetHandPos( D3DXVECTOR3 *vOut, int nParts, const D3DXMATRIX &mWorld );
		
	int		MovePart( int nDstPart, int nSrcPart );
	void	SetMotionBlending( BOOL bFlag = TRUE );
	void	TakeOffParts( int nParts );		// ���������Ҷ� ����.
	void	SetGroup( int nNum ) 
	{
		if( nNum < 0 || nNum >= MAX_GROUP )
		{
			Error( "SetGroup : lv=%d", nNum );
			nNum = 0;
		}

		for( int i = 0; i < MAX_ELEMENT; i ++ )
		{
			if( m_Element[i].m_pObject3D )	
			{
				if( m_Element[i].m_pObject3D->m_bLOD == 0 )		
					m_Element[i].m_pObject3D->SetLOD( 0 );		// LOD����Ÿ�� ���ٸ� �׻� High�����θ�..
				else
				{
					m_Element[i].m_pObject3D->SetLOD( nNum );
					switch( nNum )
					{
					case 0:		m_Element[i].m_ppd3d_VBSel = m_Element[i].m_ppd3d_VB1;	break;		// lod�� ���õɶ����� ���� �����͸� ����Ī ����.
					case 1:		m_Element[i].m_ppd3d_VBSel = m_Element[i].m_ppd3d_VB2;	break;
					case 2:		m_Element[i].m_ppd3d_VBSel = m_Element[i].m_ppd3d_VB3;	break;
					}
				}
				
			}
		}
	}		// Set LOD Group
	// �� ������ ������ ����Ʈ�� �ܺο��� �����Ҷ� ����.	XE_??? �ø�� �����ؼ� ����.
	O3D_ELEMENT*	SetEffect( int nParts, int nEffect ) 
	{ 
		if( GetParts(nParts) )
		{
			GetParts(nParts)->m_nEffect |= nEffect; 
			return GetParts(nParts);
		}
		return NULL;
	} 
	O3D_ELEMENT*	SetEffectOff( int nParts, int nEffect ) 
	{ 
		if( GetParts(nParts) )
		{
			GetParts(nParts)->m_nEffect &= (~nEffect); 
			return GetParts(nParts);
		}
		return NULL;
	} 
	void	SetTextureEx( int nNumEx )		// ��� �ؽ��� ���� ������
	{
		
		m_Element[0].m_pObject3D->SetTextureEx( nNumEx );		// ���ʹ� [0]�ϳ��� ���ٰ� �����ϰ� ����.
	}

	
	BOOL	IsLoadAttr( void ) { if(m_pMotion) return TRUE; else return FALSE; }
	MOTION_ATTR *GetMotionAttr( int nNumFrm ) { return m_pMotion->GetMotionAttr(nNumFrm); }	
	DWORD	IsAttrHit( void ) 
	{ 
		if( m_pMotion )
			return m_pMotion->IsAttrHit( m_fFrameOld, m_fFrameCurrent ); 
		else
			return GetObject3D()->IsAttrHit( m_fFrameCurrent );
	}	// ���� �����ӿ� Ÿ�ݼӼ��� �ִ°�?
	DWORD	IsAttrHit( float fNumFrm ) { return m_pMotion->IsAttrHit( m_fFrameOld, fNumFrm ); }	// nNumFrm�����ӿ� Ÿ�ݼӼ��� �ִ°�?
	MOTION_ATTR *IsAttrSound( void ) 
	{ 
		if( m_pMotion == NULL )
		{
			LPCTSTR szErr = Error( "IsAttrSound : pMotion==NULL %d %d", m_fFrameOld, m_fFrameCurrent );
			//ADDERRORMSG( szErr );
		}
		return m_pMotion->IsAttrSound( m_fFrameOld, m_fFrameCurrent ); 
	}	// ���� �����ӿ� ����Ӽ��� �ִ°�?
	MOTION_ATTR *IsAttrSound( float fNumFrm ) 
	{ 
		if( m_pMotion)	//gmpbigsun: m_pMotion�� NULL�ΰ�� shit ..
			return m_pMotion->IsAttrSound( m_fFrameOld, fNumFrm ); 	// nNumFrm�����ӿ� ����Ӽ��� �ִ°�?	
		return NULL;
	}
	DWORD	IsAttrQuake( float fNumFrm ) { return m_pMotion->IsAttrQuake( m_fFrameOld, fNumFrm ); }
	DWORD	IsAttrQuake( void ) { return m_pMotion->IsAttrQuake( m_fFrameOld, m_fFrameCurrent ); }
	void	SetAttr( float fNumFrm, DWORD dwAttr ) { m_pMotion->SetAttr( fNumFrm, dwAttr ); }
	void	UpdateMatrixBone( void )
	{
		if( m_pMotion )		// ���� �������� ��ǿ� ���� ��Ʈ������ ���Ž�Ŵ.
			m_pMotion->AnimateBone( m_mUpdateBone, m_pMotionOld, m_fFrameCurrent, GetNextFrame(), m_nFrameOld, m_bMotionBlending, m_fBlendWeight );	
	}
	void	GetEventPos( D3DXVECTOR3 *pOut, int nIdx = 0 )		// ������Ʈ�� ���� �������� ���� �̺�Ʈ��ǥ.
	{ 
		if( m_pBone == NULL )	return;
		if( m_mUpdateBone == NULL )	return;
		D3DXVec3TransformCoord( pOut, &m_pBone->m_vEvent[nIdx], &m_mUpdateBone[ m_pBone->m_nEventParentIdx[nIdx] ] );
		// ���� ����� pOut�� ���ñ����̹Ƿ� ���� m_mWorld�� ������� �Ѵ�.
		// vFinal = pOut * m_mWorld;
	}
	D3DXMATRIX *GetMatrixBone( int nBoneIdx )
	{
		if( m_pBone == NULL )	return NULL;
		if( m_mUpdateBone == NULL )	return NULL;
		return &m_mUpdateBone[ nBoneIdx ];
	}
	BOOL	IsEmptyElement( void )
	{
		for( int i = 0; i < MAX_ELEMENT; i ++ )
			if( m_Element[i].m_pObject3D )	return FALSE;
		return TRUE;
	}
	D3DXVECTOR3 *GetPath( void ) { return ( m_pMotion ) ? m_pMotion->m_pPath : NULL; }

	int		IsHaveCollObj( void ) { return m_bCollObj; }
	HRESULT	CreateDeviceBuffer( O3D_ELEMENT *pElem );
	int		LoadBone( LPCTSTR szFileName );
	int		LoadMotion( LPCTSTR szFileName );
	int		LoadElement( LPCTSTR szFileName, int nParts = 0 );
	void	SetParent( int nParts, int nBoneIdx );
	void	SetTextureMulti( LPCTSTR szBitmap, int nParts );
	void	ClearTextureEx( int nParts );
	void	ChangeTexture( int nParts, LPCTSTR szSrc, LPCTSTR szDest );
	
	void	CreateParticle( int nParts, const D3DXMATRIX *pmWorld, int nType, int nLevel, int nLevel2 = 0 );
	void	RenderItemElec( int nParts, const D3DXMATRIX *pmWorld, int nLevel );
	void	RenderItemElec_Adv( int nParts, const D3DXMATRIX *pmWorld, int nLevel );

#ifndef __WORLDSERVER
	BOOL	Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld );
	void	RenderEffect( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld, DWORD dwItemKind3 = NULL_ID, int nLevelL = 0, int nLeveR = 0);
#ifdef __SHADOW
	BOOL	RenderShadow( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX *mWorld );
	void	SetStateShadow( LPDIRECT3DDEVICE9 pd3dDevice );
	void	ResetStateShadow( LPDIRECT3DDEVICE9 pd3dDevice );
#endif

#if __VER >= 15 // __GUILD_HOUSE
	int LoadClonedElement( LPCTSTR szFileName );		//gmpbigsun:������ ���� 
#endif

#endif //__WORLDSERVER

	void	MakeSWDForce( int nParts, DWORD dwItemKind3, BOOL bSlow = FALSE, DWORD dwColor = D3DCOLOR_ARGB(255,120,120,230), float fSpeed = 1.0f );		// m_pMotion�� �˱��� ����
	void	FrameMove( D3DXVECTOR3 *pvSndPos = NULL, float fSpeed = 1.0f );

	// collision detect
	BOOL	Intersect( const D3DXVECTOR3 &vRayOrig, const D3DXVECTOR3 &vRayDir, const D3DXMATRIX &mWorld, D3DXVECTOR3* pvIntersect, FLOAT* pfDist, BOOL bColl = FALSE  ) 
	{ 
		if( IntersectRayTri( vRayOrig, vRayDir, mWorld, pvIntersect, pfDist, bColl ) ) 
			return TRUE; 
		else 
			return FALSE; 
	}

	D3DXVECTOR3 *IntersectRayTri( const D3DXVECTOR3 &vRayOrig, const D3DXVECTOR3 &vRayDir, const D3DXMATRIX &mWorld, D3DXVECTOR3* pvIntersect, FLOAT* pfDist, BOOL bColl = FALSE ) 
	{	
		O3D_ELEMENT *pElem = m_Element;
		D3DXVECTOR3 *pTri;
		for( int i = 0; i < MAX_ELEMENT; i ++ )
		{
			if( pElem->m_pObject3D )
			{
				if( m_pBone )
					pElem->m_pObject3D->SetExternBone( m_mUpdateBone, m_pBaseBoneInv );	// ���庻�� �ִٸ� �װ��� �Ѱ��ش�.
				if( pTri = pElem->m_pObject3D->IntersectRayTri( vRayOrig, vRayDir, mWorld, pvIntersect, pfDist, bColl ) )
					return pTri;
			}
			pElem ++;
		}
		return NULL;
	}
	BOOL	IsTouchOBB_Line( const D3DXVECTOR3 &vStart, const D3DXVECTOR3 &vEnd, const D3DXMATRIX &mWorld, D3DXVECTOR3* pvIntersect, BOOL bNeedCollObject = TRUE )
	{ 
		CObject3D *pObj = m_Element[0].m_pObject3D;
		if( pObj->m_CollObject.m_Type == GMT_ERROR && bNeedCollObject )	// �浹�޽ð� ������ �˻����� ����.
			return FALSE;
		
		return ::IsTouchOBB_Line( m_vMin, m_vMax, mWorld, vStart, vEnd, pvIntersect );
	}

	BOOL	TestIntersectionOBB_Line( const Segment3& segment, const CObj* pObj, BOOL bNeedCollObject = TRUE );

	// virtual
	int LoadModel( LPCTSTR szFileName ) { return LoadElement( szFileName, 0 ); }

#ifdef __BS_EFFECT_LUA
	BOOL CModelObject::GetPosBone( D3DXVECTOR3* pOut, const char* bonename );
#endif

	// ����̽� �ʱ�ȭ �� ���� 
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

extern BOOL		g_bUsableVS;			// �ϵ���� ���ؽ� ���̴�(1.1)�� ���� �ִ°�.
extern DWORD	g_dwUsablePS_Level;		// �ϵ���� �ȼ����̴� ���� üũ 2.0���ϴ� 2 


#endif // MODELOBJECT