#include "stdafx.h"
#include "Sfx.h"

#ifdef __CLIENT
#include "dpclient.h"
extern	CDPClient	g_DPlay;
#endif	// __CLIENT

#include "defineObj.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSfx::CSfx()
{
	m_dwType = OT_SFX;
	m_idSrc = NULL_ID;
	m_idDest = NULL_ID;
	m_vPosDest = D3DXVECTOR3( 0, 0, 0 );
	m_nFrame = 0;
	m_nSec = -1;
#ifdef __CLIENT
	m_idSfxHit	= 0;
	m_nMagicPower = 0;
	m_dwSkill = NULL_ID;
//	m_dwSkillLevel = 0;
#endif	// __CLIENT
	m_pSfxObj = NULL;
}
CSfx::CSfx( int nIndex, const OBJID idSrc, const D3DXVECTOR3& vPosSrc, const OBJID idDest, const D3DXVECTOR3& vPosDest )
{
//	SetSfx( nIndex, idSrc, vPosSrc, idDest, vPosDest );
#ifdef __CLIENT
	m_idSfxHit	= 0;
	m_nMagicPower = 0;
#endif	// __CLIENT
	m_pSfxObj = NULL;
}
CSfx::~CSfx()
{
#ifdef __CLIENT
	if( m_idSfxHit )
	{
		if( m_idSrc == NULL_ID )	// �̷���Ȳ�� ���ܼ� �ȵȴ�.
		{
			LPCTSTR szErr = Error( "CSfx::~CSfx : %d %d %d", m_dwIndex, m_idSfxHit, m_idSrc );
			//ADDERRORMSG( szErr );
		} 
	#ifdef __CLIENT
		else
			g_DPlay.SendSfxClear( m_idSfxHit, m_idSrc );	// m_idSfxHit�� �ִ� �ſ����� ������ ��� ������ ������ 
	#endif//__CLIENT
	}
#endif // __CLIENT
}

BOOL CSfx::SetIndex( LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwIndex, BOOL bInitProp )
{
	BOOL bResult = FALSE;
	m_idSrc = NULL_ID;
	m_idDest = NULL_ID;
	m_vPosDest = D3DXVECTOR3( 0, 0, 0);

	if( dwIndex >= 0 ) 
	{
#ifndef __WORLDSERVER
		bResult = SetTypeIndex( pd3dDevice, OT_SFX, dwIndex, bInitProp );
		m_pSfxObj = (CSfxModel*)m_pModel;
#endif	// __WORLDSERVER
	}
	UpdateLocalMatrix();
	return bResult;
}

int	CSfx::SetSfx( LPDIRECT3DDEVICE9 pd3dDevice, int nIndex, 
				   D3DXVECTOR3& vPosSrc, OBJID idSrc, D3DXVECTOR3& vPosDest, OBJID idDest, int nSec )
{
	m_idSrc = idSrc;
	m_idDest = idDest;
	m_vPosDest = vPosDest;
	m_nSec = nSec;

	vPosSrc.y += 0.01f; // ���鿡�� �ణ �������. �ȱ׷��� �ؿ� ������ ���۰Ÿ�.
	SetPos( vPosSrc );
	
	if( nIndex >= 0 ) 
	{
#ifndef __WORLDSERVER
		SetTypeIndex( pd3dDevice, OT_SFX, nIndex,TRUE);
		m_pSfxObj = (CSfxModel*)m_pModel;
#endif	// __WORLDSERVER
		return 1;
	}
	UpdateLocalMatrix();

	if( m_pSfxObj )
		return 1;

	return 0;
}

//
// nDmgCnt : �Ϲ������� 0 : ���ӵ������� ����Ұ�쿡 0�� �ƴѰ��� ���´�.
//
void CSfx::DamageToTarget( int nDmgCnt, float fDmgAngle, float fDmgPower, int nMaxDmgCnt )
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	CCtrl* pObjDest = prj.GetCtrl( m_idDest );
	
	if( IsInvalidObj(pObjSrc) )		return;		// ������ �� ���������� �̷��� �����Ѱ��� m_idSfxHit�� Clear���ִ��۾��� �ʿ��ϴ�.
	if(	IsInvalidObj(pObjDest) )	return;

	if( pObjDest->GetType() == OT_MOVER )
	{
		CMover* pMover = (CMover*) pObjDest;
#ifdef __CLIENT
		PLAYSND( pMover->GetProp()->dwSndDmg2, &pMover->GetPos() );	// ������ ������ Ÿ����.	
#endif 

#ifdef __CLIENT
		// ����� �÷��̾��̰ų� / ����� �÷��̾ �ƴѵ� �������� �÷��̾��ϰ�� ����
		if( pObjSrc->IsActiveMover() || (pObjSrc->IsPlayer() == FALSE && pObjDest->IsActiveObj()) )
		{
			pMover->SetDmgCnt( 10 );	// �߻�ü �¾Ƶ� ���� ��鸰��,
			g_DPlay.SendSfxHit( m_idSfxHit, m_nMagicPower, m_dwSkill, pObjSrc->GetId(), nDmgCnt, fDmgAngle, fDmgPower );
			if( nMaxDmgCnt == 1 )	// �ѹ�¥�� �������� id�� Ŭ���� ��.
				m_idSfxHit = 0;		// 0���� �س��� this�� �����ɶ� SendSfxClear�� �� ������ �ʴ´�.
		}
#endif	// __CLIENT
	}
}
void CSfx::Process()
{
#ifdef _DEBUG
	if( m_dwIndex == XI_SKILL_MAG_FIRE_HOTAIR01 )	// ����� �Ϸ��� �̰� �ٲ� ������.
	{
		int a = 0;
	}
#endif
	m_nFrame++;
	if( m_nSec == 0 )	// 0�� 1ȸ �÷����� ����.
	{
		if( m_pSfxObj->Process() )		// return true�� �ִϸ��̼� ��.
			Delete();
	}
	else
	{
		// �ݺ� �ִϸ��̼�
		if( m_pSfxObj->Process() )		
			m_pSfxObj->m_nCurFrame = 0;
		if( m_nSec != -1 )		// ���ѹݺ�(-1)�� �ƴϸ�
		{
			if( m_nFrame > ( m_nSec * 60 ) )	// �ð� üũ�� �Ѵ�.
				Delete();
		}
	}

#ifdef __CLIENT
	if(m_pSfxObj->m_pSfxBase != NULL)
	{
		for( int j=0; j<m_pSfxObj->m_pSfxBase->m_apParts.GetSize(); j++ )
		{
			if( m_pSfxObj->m_pSfxBase->Part(j)->m_nType != SFXPARTTYPE_MESH )
				continue;
			
			CModelObject* pMesh = NULL;
			
			pMesh = g_SfxMeshMng.Mesh( m_pSfxObj->m_pSfxBase->Part(j)->m_strTex );
			
			if( pMesh )
				pMesh->FrameMove();
		}
	}
#endif //__CLIENT

	if( m_idDest != NULL_ID )		// Dest�� �����Ǿ� ������.
	{
		CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
		if( IsValidObj( pObjDest ) )		// ��ȿ�ѳ��ΰ�?
			m_vPosDest = pObjDest->GetPos();	// ��� ��ǥ�� ��� �޾Ƶ�.  Invalid���°� �Ǹ� ������ ��ǥ�� ���õȴ�.
		
		SetPos( m_vPosDest );	// Ÿ�ٿ� ������Ʈ �ߵ�.
#ifdef __CLIENT
	#if __VER >= 11 // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
		if( m_dwIndex == XI_SKILL_PSY_HERO_STONE02 )
		{
			if( IsValidObj( pObjDest ) )		// ��ȿ�ѳ��ΰ�?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj(XI_SKILL_PSY_HERO_STONE02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
	#endif // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
		// 091022 mirchang - �����ؼ�, ��ٸ��� ��ų ���� ���� �� sfx ����
		if( m_dwIndex == XI_SKILL_MER_SHIELD_PANBARRIER02 )
		{
			if( IsValidObj( pObjDest ) )		// ��ȿ�ѳ��ΰ�?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj( XI_SKILL_MER_SHIELD_PANBARRIER02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
		if( m_dwIndex == XI_SKILL_MER_SHIELD_PROTECTION02 )
		{
			if( IsValidObj( pObjDest ) )		// ��ȿ�ѳ��ΰ�?
			{
				DWORD dwTmpID =  pObjDest->GetRemoveSfxObj( XI_SKILL_MER_SHIELD_PROTECTION02 );
				if( dwTmpID && m_nFrame > 0)
				{
					Delete();
					return;
				}
			}
		}
#endif	// __CLIENT
	} else
	{
		// Dest�� �����Ǿ� ���� ������. Src��...
		if( m_idSrc != NULL_ID )
		{
			CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
			if( IsValidObj( pObjSrc ) )			// �ҽ����̵� �����Ǿ� ������
				SetPos( pObjSrc->GetPos() );	// �ҽ����� ���� �ߵ�.
			else
				Delete();
		}
	}
}
#ifndef __WORLDSERVER
// y�����θ� ȸ������ ����.
void CSfx::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( !IsVisible() || ( IsCull() && GetType() != 1 ))
		return;
	if( IsUpdateMatrix() )
		UpdateMatrix();
	
	LPMODELELEM lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	// Model�� ������ ������� ���� 
	if( lpModelElem == NULL )
		return;

	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vPos.y += 0.2f;		// 0.2���̳� �ø��ʿ䰡 �ֳ�?
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
	
}
#endif // not worldserver

