#include "stdafx.h"
#include "Sfx.h"
#include "light.h"
#include "defineobj.h"
#include "defineSkill.h"
#include "defineSound.h"
#include "..\_Common\ParticleMng.h"

#include "dpclient.h"
extern	CDPClient	g_DPlay;


#include "party.h"
extern	CParty g_Party;


#ifndef __VM_0820
MemPooler<CSfxGenNormalDmg01>*	CSfxGenNormalDmg01::m_pPool	= new MemPooler<CSfxGenNormalDmg01>( 128 );
MemPooler<CSfxGenMoveMark>*	CSfxGenMoveMark::m_pPool	= new MemPooler<CSfxGenMoveMark>( 4 );
MemPooler<CSfxGenWaterCircle>*	CSfxGenWaterCircle::m_pPool	= new MemPooler<CSfxGenWaterCircle>( 64 );
MemPooler<CSfxGenRainCircle>*	CSfxGenRainCircle::m_pPool	= new MemPooler<CSfxGenRainCircle>( 64 );
#endif	// __VM_0820


CSfxGenMoveMark* g_pMoveMark=NULL;





//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
static int g_nEffect = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ϲ� ȿ���� 

// �Ϲ� �������� ������ ���� �� ȿ�� 
CSfxGenNormalDmg01::CSfxGenNormalDmg01()
{
}
CSfxGenNormalDmg01::~CSfxGenNormalDmg01()
{
}
void CSfxGenNormalDmg01::Process()
{
	m_nFrame++; // �� ������ �����Ű��
	if(m_pSfxObj->Process()) // SFX�� �ִϸ��̼��� ó���ؼ� SFX�� ����� ��������
	{
		Delete(); // �ڽ��� �����
	}
	// �̰��� Process�� �⺻. �ܼ� ��� SFX�� ��κ� �� ���¸� ���Ѵ�.
}
void CSfxGenNormalDmg01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos();		// 2006/6/20 xuzhu
//	m_pSfxObj->m_vPos = GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}
// �Ϲ� �������� ������ ���� �� ȿ�� 
CSfxGenSuperDmg01::CSfxGenSuperDmg01()
{
}
CSfxGenSuperDmg01::~CSfxGenSuperDmg01()
{
}
void CSfxGenSuperDmg01::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
	// ���� �ܼ� ��� Process�̴�.
}
void CSfxGenSuperDmg01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ��ų�� ������ ���� �� ȿ�� 
CSfxGenSkillDmg01::CSfxGenSkillDmg01()
{
}
CSfxGenSkillDmg01::~CSfxGenSkillDmg01()
{
}
void CSfxGenSkillDmg01::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenSkillDmg01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ���Ͱ� ������ ���� �� ȿ�� 
CSfxGenMonsterDmg01::CSfxGenMonsterDmg01()
{
}
CSfxGenMonsterDmg01::~CSfxGenMonsterDmg01()
{
}
void CSfxGenMonsterDmg01::Process()
{
	m_nFrame++; // �������� �����Ű��
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc ); // �پ�ٴ� �� ID�� Mover�� �����͸� ����´�.
	if(pObjSrc) SetPos(pObjSrc->GetPos()); // �׳��� ������ �׳��� ��ġ�� �̵���Ű��
	if(m_pSfxObj->Process()) // SFX�� �ִϸ��̼��� ó���ؼ� SFX�� ����� ��������
	{
		Delete(); // �ڽ��� �����.
	}
	// �̰��� Ư�� Mover�� �پ�ٴϴ� SFX�� Process �⺻���̴�.
}
void CSfxGenMonsterDmg01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ��Ȱ ȿ�� 
CSfxGenRestoration01::CSfxGenRestoration01()
{
}
CSfxGenRestoration01::~CSfxGenRestoration01()
{
}
void CSfxGenRestoration01::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
	// ���� m_idSrc�� ����ٴϴ� Process�̴�.
}
void CSfxGenRestoration01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ġ�� ȿ�� 
CSfxGenCure::CSfxGenCure()
{
}
CSfxGenCure::~CSfxGenCure()
{
}
void CSfxGenCure::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenCure::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ȿ�� ���� ȿ�� 
CSfxGenIncrease01::CSfxGenIncrease01()
{
}
CSfxGenIncrease01::~CSfxGenIncrease01()
{

}
void CSfxGenIncrease01::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenIncrease01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ���� �� ȿ�� 
CSfxGenLevelUp::CSfxGenLevelUp()
{
}
CSfxGenLevelUp::~CSfxGenLevelUp()
{
}
void CSfxGenLevelUp::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) {
		SetPos(pObjSrc->GetPos()+D3DXVECTOR3(.0f,.1f,.0f));
		SetAngle(-pObjSrc->GetAngle()); // ����ٴϴ� Process�� �������� ��ġ��Ű�� �κ��� �߰��Ǿ���.
	}
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenLevelUp::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate=D3DXVECTOR3(.0f,GetAngle(),.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �شܽ�ų ����Ʈ : ��Ʈ��Ī 01
CSfxTroStretching01::CSfxTroStretching01()
{
}
CSfxTroStretching01::~CSfxTroStretching01()
{
}
void CSfxTroStretching01::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) {
		SetPos(pObjSrc->GetPos()+D3DXVECTOR3(.0f,.1f,.0f));
		SetAngle(-pObjSrc->GetAngle()); // ����ٴϴ� Process�� �������� ��ġ��Ű�� �κ��� �߰��Ǿ���.
	}
	if(m_pSfxObj->Process())
	{
		Delete();
		
		if( pObjSrc )
		{
			D3DXVECTOR3 v = pObjSrc->GetPos();
			
			ItemProp* pItemProp = prj.GetPartySkill( ST_STRETCHING );
			
			CSfx *pSfx = CreateSfx( g_Neuz.m_pd3dDevice, pItemProp->dwSfxObj2, v, pObjSrc->GetId(), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), NULL_ID, -1 );	// �Ӹ����� sfx����.
		}
		
	}
}

void CSfxTroStretching01::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate=D3DXVECTOR3(.0f,GetAngle(),.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �شܽ�ų ����Ʈ : ��Ʈ��Ī 02
CSfxTroStretching02::CSfxTroStretching02()
{
}
CSfxTroStretching02::~CSfxTroStretching02()
{
}
void CSfxTroStretching02::Process()
{
	m_nFrame++;
	if( m_nSec == 0 )
	{
		if( m_pSfxObj->Process() )
			Delete();
	}
	else
	{
		if( m_pSfxObj->Process() )
			m_pSfxObj->m_nCurFrame = 0;
		if( m_nSec != -1 )
		{
			if( m_nFrame > ( m_nSec * 60 ) )
				Delete();
		}
	}

	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );

	if(pObjSrc) 
	{
		if( !pObjSrc->m_pActMover->IsSit() || g_Party.m_nModeTime[PARTY_STRETCHING_MODE] == FALSE )
		{
			Delete();
		}
		else
		{
			SetPos(pObjSrc->GetPos()+D3DXVECTOR3(.0f,.1f,.0f));
			SetAngle(-pObjSrc->GetAngle()); // ����ٴϴ� Process�� �������� ��ġ��Ű�� �κ��� �߰��Ǿ���.
		}
	}
	else
		Delete();
}

void CSfxTroStretching02::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate=D3DXVECTOR3(.0f,GetAngle(),.0f);
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �شܽ�ų ����Ʈ : ����~
CSfxTroBlitz::CSfxTroBlitz()
{
}
CSfxTroBlitz::~CSfxTroBlitz()
{
}
void CSfxTroBlitz::Process()
{
	m_nFrame++;
	if( m_nSec == 0 )
	{
		if( m_pSfxObj->Process() )
			Delete();
	}
	else
	{
		if( m_pSfxObj->Process() )
			m_pSfxObj->m_nCurFrame = 0;
		if( m_nSec != -1 )
		{
			if( m_nFrame > ( m_nSec * 60 ) )
				Delete();
		}
	}
}

void CSfxTroBlitz::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );

	if( IsInvalidObj(pObjDest) )	
		return;

	m_pSfxObj->m_vPos = pObjDest->GetPos();
	m_pSfxObj->m_vPos.y += ((pObjDest->m_pModel->GetMaxHeight()*pObjDest->GetScale().y) - (pObjDest->GetScale().y*0.7f));
	
	m_pSfxObj->m_vRotate=D3DXVECTOR3(.0f,GetAngle(),.0f);
	m_pSfxObj->m_vScale = pObjDest->GetScale();
	
	m_pSfxObj->Render( pd3dDevice );
}

// �α��� ȿ�� 
CSfxGenLogin::CSfxGenLogin()
{
}
CSfxGenLogin::~CSfxGenLogin()
{
}
void CSfxGenLogin::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenLogin::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ���� ȿ�� 
CSfxGenWarp::CSfxGenWarp()
{
}
CSfxGenWarp::~CSfxGenWarp()
{
}
void CSfxGenWarp::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenWarp::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �÷��̾� ���� �� ȿ�� 
CSfxGenPcDie::CSfxGenPcDie()
{
}
CSfxGenPcDie::~CSfxGenPcDie()
{
}
void CSfxGenPcDie::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenPcDie::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ���� ���� �� �� ȿ�� 
CSfxGenMonsterSpawn::CSfxGenMonsterSpawn()
{
}
CSfxGenMonsterSpawn::~CSfxGenMonsterSpawn()
{
}
void CSfxGenMonsterSpawn::Process()
{
	m_nFrame++;
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(pObjSrc) SetPos(pObjSrc->GetPos());
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenMonsterSpawn::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �Ϲ� �������� ������ ���� �� ȿ�� 
CSfxGenMoveMark::CSfxGenMoveMark()
{
}
CSfxGenMoveMark::~CSfxGenMoveMark()
{
}
void CSfxGenMoveMark::Process()
{
	m_nFrame++;
	g_pMoveMark=this;
	if(m_pSfxObj->Process())
	{
		g_pMoveMark=NULL;
		Delete();
	}
}
void CSfxGenMoveMark::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vPos.y += 0.05f;
	m_pSfxObj->m_vScale = GetScale();

	m_pSfxObj->Render2( pd3dDevice );
}

// �Ϲ� �������� ������ ���� �� ȿ�� 
CSfxGenWaterCircle::CSfxGenWaterCircle()
{
}
CSfxGenWaterCircle::~CSfxGenWaterCircle()
{
}
void CSfxGenWaterCircle::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenWaterCircle::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

CSfxGenRainCircle::CSfxGenRainCircle()
{
}
CSfxGenRainCircle::~CSfxGenRainCircle()
{
}
void CSfxGenRainCircle::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}

	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vScale    = GetScale();
}
void CSfxGenRainCircle::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->Render2( pd3dDevice );
}


// �Ϲ� �������� ������ ���� �� ȿ�� 
CSfxGenWaterCrown::CSfxGenWaterCrown()
{
}
CSfxGenWaterCrown::~CSfxGenWaterCrown()
{
}
void CSfxGenWaterCrown::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxGenWaterCrown::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ȿ���� 

// ���������� �߻��ϴ� �ϵ��� �߻�ü.
CSfxItemWandAtkAir::CSfxItemWandAtkAir()
{
	m_SfxObj2.SetSfx( "sfx_HitWand01" ); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit = FALSE; // ���� �ȸ¾Ҵ�.
}
CSfxItemWandAtkAir::~CSfxItemWandAtkAir()
{
}

int CSfxItemWandAtkAir::SetSfx( LPDIRECT3DDEVICE9 pd3dDevice, int nIndex, 
								 D3DXVECTOR3& vPosSrc, OBJID idSrc, D3DXVECTOR3& vPosDest, OBJID idDest, int nSec )
{
	CSfx::SetSfx( pd3dDevice, nIndex, vPosSrc, idSrc, vPosDest, idDest, nSec );		// �⺻�ʱ�ȭ �����ϰ�.

	CMover* pObjSrc = prj.GetMover( m_idSrc );		// �߻���.
	if( IsInvalidObj(pObjSrc) )		return 0;		// ����.
	CMover* pObjDest = prj.GetMover( m_idDest );	// Ÿ������ ������.
	if( IsInvalidObj(pObjDest) )	return 0;		// ����.
	
	// !!!!!!!!!!!! SFX�� SetPos�Ҷ� ������ ó�� ���� �ʵ��� �ٲ���.
	SetPos( pObjSrc->GetPos() + D3DXVECTOR3( 0, 1.0f, 0 ) );		// �߻����� ��ǥ���� �ణ ������ �߻��.
	m_pSfxObj->m_vPos = GetPos();
	m_vPosDest = pObjDest->GetPos();		// Ÿ���� ������.

	return 1;
}

//
//
//
void CSfxItemWandAtkAir::ShootSfx( float fAngXZ, float fAngY, float fSpeed )
{
	AngleToVector( &m_vDelta, fAngXZ, fAngY, fSpeed );		// �ʱ� �߻���.	- �ϵ尡 ���ϰ� �ִ� �������� ��� ����.
}

	
void CSfxItemWandAtkAir::Process()
{
	FLOAT	fRadius = 0.6f;		// Ÿ���� �ݰ�.
	CMover* pObjDest = prj.GetMover( m_idDest );		// Ÿ������ ������.
	if( IsValidObj(pObjDest) )		// ���� �߻�ü�� ���ư��� �߿� Ÿ���� �������ٸ� ������ �־��� ��ǥ�� ���ư��� �ı��ȴ�.
	{
		m_vPosDest = pObjDest->GetPos();
		m_vPosDest.y += 1.0f;
		if( pObjDest->IsNPC() )		// ���ǰ�� Ÿ���� ũ�⸦ ���� ũ�� ����.
			fRadius = 3.0f;
	}
	
	if( m_bHit == FALSE )	// ���� �ȸ��� �����̸�
	{ 
		m_pSfxObj->Process(); // SFX �ִϸ��̼��� ó���ϰ�
		if( m_pSfxObj->m_nCurFrame == 20 ) // ���� 20�����ӱ��� ����������
		{
			m_pSfxObj->m_nCurFrame = 0; // ��ǥ���� ���������� ������Ű�� ���� �ٽ� 0���������� �������´�.
		}

		// ���� ��ġ���� ��ǥ������������ ���� ����.
		D3DXVECTOR3 vDist = m_vPosDest - GetPos();
		D3DXVECTOR3 vDistXZ = vDist;
		vDistXZ.y = 0;		// XZ����� ���� ���̸� ���ϱ� ����.

		FLOAT fAngXZ = D3DXToDegree( atan2( vDist.x, -vDist.z ) );		// ��ǥ������������ XZ���� ����.
		FLOAT fDistXZ = D3DXVec3Length( &vDistXZ );				// ��ǥ������������ XZ��鿡���� ����.
		FLOAT fAngH  = D3DXToDegree( atan2( fDistXZ, vDist.y ) );		// ��ǥ������������ ���� ����.

		fAngH -= 90.0f;
		fAngH = -fAngH;
		D3DXVECTOR3	vAcc;
		AngleToVector( &vAcc, fAngXZ, fAngH, 0.007f );		// �ǽð����� ��ǥ������ ���ϴ� ��.
		
		FLOAT fSpeedSq = D3DXVec3LengthSq( &m_vDelta );		// ������.  ���� �߻�ü�� �ӵ��� ����.
		if( fSpeedSq < (0.45f * 0.45f) )
		{
			m_vDelta += vAcc;		// ���� ������ ���ο� ���� ����.
		}
		m_vDelta *= 0.985f;		// ������ �༭ �����̻� �ӵ��� �������� �ʵ��� ����.
		//m_vDelta *= 0.965f;		// ������ �༭ �����̻� �ӵ��� �������� �ʵ��� ����.
		D3DXVECTOR3 vPos = GetPos();
		vPos += m_vDelta;
		SetPos( vPos );		// ���� ��ġ ����.

		m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = fAngXZ;	// sfx model�� ������ ��ȭ ������.
		
		if( D3DXVec3LengthSq( &vDist ) < (fRadius * fRadius) )  // ��ǥ���� �Ÿ��� xx cm������ ��쿡��...
		{			
			m_bHit = TRUE; // ���� �ɷ� �Ǵ��Ѵ�.
			m_SfxObj2.m_vPos = GetPos();		// ���� ��ġ���� ����.
			DamageToTarget(); // ���⼭ �´¼���ó��
		}

		if( IsValidObj(pObjDest) )
		{
			if( pObjDest == CMover::GetActiveMover() )	// �� Ÿ������ ��� ���ƿ��� ���� ������
				g_ParticleMng.CreateParticle( 0, GetPos(), D3DXVECTOR3(0,0,0), pObjDest->GetPos().y );
			else
				g_ParticleMng.CreateParticle( 1, GetPos(), D3DXVECTOR3(0,0,0), pObjDest->GetPos().y );
			
		}
		
	}
	else 
	{
		if( m_SfxObj2.Process() )		// ���������� �´� ���� ��µǴ� SFX�� ����� ������
		{ 
			Delete(); // �ڽ��� �����.
		}
	}
	m_pSfxObj->m_vPos = GetPos();
	
	m_nFrame++;
	if( m_nFrame >= 60 * 8 )	
	{
		m_bHit = TRUE;
		m_SfxObj2.m_vPos = GetPos();		// ���� ��ġ���� ����.
		if( g_pPlayer->IsActiveMover() )
			g_DPlay.SendSfxClear( m_idSfxHit );		// �̰��� �����Ǿ��ٴ°� ������ �˸�.
	}
}

void CSfxItemWandAtkAir::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if( m_bHit == FALSE ) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}



CSfxMagicMiAtk1::CSfxMagicMiAtk1()
{
	m_SfxObj2.SetSfx( "sfx_HitWand01" ); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit = FALSE; // ���� �ȸ¾Ҵ�.
	m_fCenter = 0;
	m_fRadiusXZ = 0.2f;		// ����Ʈ 20��ġ.
}
CSfxMagicMiAtk1::~CSfxMagicMiAtk1()
{
}


void CSfxMagicMiAtk1::Process()
{
	// �̰��� ��ǥ������ ���󰡼� �´� SFX�� �Ϲ����� Process�̴�.
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest ) 
	{
		if( pObjDest->IsDie() )
		{
			Delete();
			return;
		}

		// �߻� ��ǥ������ ����� �߾�.
		float fHeight = fabs(pObjDest->m_pModel->m_vMax.y) / 2.0f;	// ���̹�Ʈ�������� ���߿� ���ֱⶫ�� ���� ������ Max���� ���ߴ�.
		fHeight *= pObjDest->GetScale().x;
		m_fCenter = fHeight;	
		
		m_vPosDest = pObjDest->GetPos();		// Ÿ���� ��ġ ���� - Ÿ���� ��� �����̹Ƿ�...
		m_vPosDest.y += m_fCenter;
		if( m_nFrame == 0 )
		{
			// �� ù�����ӿ��� ���.
			FLOAT fLenX = fabs(pObjDest->m_pModel->m_vMax.x - pObjDest->m_pModel->m_vMin.x);
			FLOAT fLenZ = fabs(pObjDest->m_pModel->m_vMax.z - pObjDest->m_pModel->m_vMin.z);
			m_fRadiusXZ = ((fLenX + fLenZ) / 2.0f);		// Xũ�� Zũ�� ����� ��
			m_fRadiusXZ *= pObjDest->GetScale().x;
			m_fRadiusXZ /= 3.0f;	// ��Ȯ�� �������� ������� ������(/2)���� ���� ũ�� ���� 3�� ����.
		}
	} else
	{
		Delete();
		return;
	}

		
	if( m_bHit == FALSE )		// ���� �ȸ��� �����̸�
	{ 
		m_pSfxObj->Process(); // SFX �ִϸ��̼��� ó���ϰ�
		if( m_pSfxObj->m_nCurFrame == 20 ) // ���� 20�����ӱ��� ����������
		{
			m_pSfxObj->m_nCurFrame=0; // ��ǥ���� ���������� ������Ű�� ���� �ٽ� 0���������� �������´�.
		}

		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDelta = m_vPosDest - vPos;		// ���� ��ġ���� Ÿ�ٹ��������� ����.
		D3DXVec3Normalize( &vDelta, &vDelta );		// ���ֺ���ȭ - 1����ȭ.
		vPos += (vDelta * 0.4f);					// 20��ġ�� �ٿ��� ������ġ�� ����.
		
		SetPos( vPos );				// ���� ��ġ ����.
		m_pSfxObj->m_vPos = vPos;	// SFXModel�� ��ġ�� ����.

		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_vPosDest - vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );

		D3DXVECTOR3 vYPW;

		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x = m_SfxObj2.m_vRotate.x = D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z = m_SfxObj2.m_vRotate.z = D3DXToDegree(vYPW.z);

		vDelta = GetPos() - m_vPosDest;
		if( D3DXVec3LengthSq( &vDelta ) < m_fRadiusXZ * m_fRadiusXZ )		// ��ǥ���� �Ÿ��� 20cm������ ��쿡��...
		{ 
			m_SfxObj2.m_vPos = GetPos();//m_vPosDest;		// Ÿ���� ��ġ�� ���� ����Ʈ ��ǥ.
			m_SfxObj2.m_vPos.y+=1.0f;
			m_bHit = TRUE; // ���� �ɷ� �Ǵ��Ѵ�.
		}
	}
	else 
	{
		if( m_SfxObj2.m_nCurFrame == 0 )		// m_SfxObj2�� �´� ���� ��µǴ� SFX�̴�.
		{ 
			DamageToTarget(); // ���⼭ �´¼���ó��
		}
		if( m_SfxObj2.Process() )		// ���������� �´� ���� ��µǴ� SFX�� ����� ������
		{ 
			Delete(); // �ڽ��� �����.
		}
	}
	m_nFrame++;
	if( m_nFrame >= SEC1 * 10 )		// �ʹ����� �i�ư��� �ʰ� 10�������� �Ҹ�
		Delete();
}
void CSfxMagicMiAtk1::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if( m_bHit == FALSE ) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}



CSfxItemWandAtk1::CSfxItemWandAtk1()
{
	m_SfxObj2.SetSfx( "sfx_HitWand01" ); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit = FALSE; // ���� �ȸ¾Ҵ�.
	m_fCenter = 0;
	m_fRadiusXZ = 0.2f;		// ����Ʈ 20��ġ.
}
CSfxItemWandAtk1::~CSfxItemWandAtk1()
{
}


void CSfxItemWandAtk1::Process()
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest ) 
	{
		if( pObjDest->IsDie() )
		{
			Delete();
			return;
		}

		// �߻� ��ǥ������ ����� �߾�.
		float fHeight = fabs(pObjDest->m_pModel->m_vMax.y) / 2.0f;	// ���̹�Ʈ�������� ���߿� ���ֱⶫ�� ���� ������ Max���� ���ߴ�.
		fHeight *= pObjDest->GetScale().x;
		m_fCenter = fHeight;	
		
		m_vPosDest = pObjDest->GetPos();		// Ÿ���� ��ġ ���� - Ÿ���� ��� �����̹Ƿ�...
		m_vPosDest.y += m_fCenter;
		if( m_nFrame == 0 )
		{
			// �� ù�����ӿ��� ���.
			FLOAT fLenX = fabs(pObjDest->m_pModel->m_vMax.x - pObjDest->m_pModel->m_vMin.x);
			FLOAT fLenZ = fabs(pObjDest->m_pModel->m_vMax.z - pObjDest->m_pModel->m_vMin.z);
			m_fRadiusXZ = ((fLenX + fLenZ) / 2.0f);		// Xũ�� Zũ�� ����� ��
			m_fRadiusXZ *= pObjDest->GetScale().x;
			m_fRadiusXZ /= 3.0f;	// ��Ȯ�� �������� ������� ������(/2)���� ���� ũ�� ���� 3�� ����.
		}
	} else
	{
		Delete();
		return;
	}

		
	if( m_bHit == FALSE )		// ���� �ȸ��� �����̸�
	{ 
		m_pSfxObj->Process(); // SFX �ִϸ��̼��� ó���ϰ�
		if( m_pSfxObj->m_nCurFrame == 20 ) // ���� 20�����ӱ��� ����������
		{
			m_pSfxObj->m_nCurFrame=0; // ��ǥ���� ���������� ������Ű�� ���� �ٽ� 0���������� �������´�.
		}

		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDelta = m_vPosDest - vPos;		// ���� ��ġ���� Ÿ�ٹ��������� ����.
		D3DXVec3Normalize( &vDelta, &vDelta );		// ���ֺ���ȭ - 1����ȭ.
		vPos += (vDelta * 0.2f);					// 20��ġ�� �ٿ��� ������ġ�� ����.
		
		SetPos( vPos );				// ���� ��ġ ����.
		m_pSfxObj->m_vPos = vPos;	// SFXModel�� ��ġ�� ����.

		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_vPosDest - vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );

		D3DXVECTOR3 vYPW;

		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x = m_SfxObj2.m_vRotate.x = D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z = m_SfxObj2.m_vRotate.z = D3DXToDegree(vYPW.z);

		vDelta = GetPos() - m_vPosDest;
		if( D3DXVec3LengthSq( &vDelta ) < m_fRadiusXZ * m_fRadiusXZ )		// ��ǥ���� �Ÿ��� 20cm������ ��쿡��...
		{ 
			m_SfxObj2.m_vPos = GetPos();//m_vPosDest;		// Ÿ���� ��ġ�� ���� ����Ʈ ��ǥ.
			m_SfxObj2.m_vPos.y+=1.0f;
			m_bHit = TRUE; // ���� �ɷ� �Ǵ��Ѵ�.
		}
	}
	else 
	{
		if( m_SfxObj2.m_nCurFrame == 0 )		// m_SfxObj2�� �´� ���� ��µǴ� SFX�̴�.
		{ 
			DamageToTarget(); // ���⼭ �´¼���ó��
		}
		if( m_SfxObj2.Process() )		// ���������� �´� ���� ��µǴ� SFX�� ����� ������
		{ 
			Delete(); // �ڽ��� �����.
		}
	}
	m_nFrame++;
	if( m_nFrame >= SEC1 * 10 )		// �ʹ����� �i�ư��� �ʰ� 10�������� �Ҹ�
		Delete();
}
void CSfxItemWandAtk1::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if( m_bHit == FALSE ) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}


CSfxItemRangeAtk1::CSfxItemRangeAtk1() : CSfxShoot()
{
	m_pTail			= NULL;
}
CSfxItemRangeAtk1::~CSfxItemRangeAtk1()
{
	if( m_pTail )
		g_TailEffectMng.Delete( m_pTail );
	m_pTail = NULL;
}

void CSfxItemRangeAtk1::Process()
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );	// ����ź ���̱⶧���� Ÿ���� �׻� �о���� �Ѵ�.
	if( IsValidObj(pObjDest) )
	{
		m_vPosDest = pObjDest->GetPos() + D3DXVECTOR3( 0, 1.0f, 0 );
	} else
	{
		// ���ư��� Ÿ���� ���������� ��������ǥ�� ���ư��� ������.
		Delete();	// this�� ����.
		int a = 0;
	}
	
	if( m_bHit == FALSE ) 
	{
		if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDelta = m_vPosDest - vPos;
		D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
		vPos += vDelta * 0.7f;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
		m_pSfxObj->m_vPos = vPos;
		SetPos( vPos );
		
		{
			// ������ü ���⺤�ͷ� �� ���� ������~~~
			D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			D3DXVECTOR3 vDestNor  = m_vPosDest - m_pSfxObj->m_vPos;
			D3DXVECTOR3 vAxis;
			D3DXQUATERNION   qRot;
			
			FLOAT       fTheta;
			D3DXVec3Normalize( &vDestNor, &vDestNor );
			D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
			fTheta = D3DXVec3Dot( &vDir, &vDestNor );
			D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
			
			D3DXVECTOR3 vYPW;
			
			QuaternionRotationToYPW( qRot, vYPW );
			m_pSfxObj->m_vRotate.x = m_SfxObj2.m_vRotate.x = D3DXToDegree(vYPW.x);
			m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = D3DXToDegree(vYPW.y);
			m_pSfxObj->m_vRotate.z = m_SfxObj2.m_vRotate.z = D3DXToDegree(vYPW.z);

		}
		
		if( m_pTail )
		{
			if( m_pTail->GetType() != 2 )	// �����Ǿ��� ������ �Ϲݺ������ �ƴϸ� 
				m_pTail->ChangeTexture( D3DDEVICE, "etc_Tail2.bmp", 2 );	// �Ϲݺ�������� �ؽ��� ��ü.
		}
		if( m_pTail == NULL )	// ���� �Ҵ� �ȵ����� �Ҵ��ϰ�.
		{
			m_pTail = (CTailEffectBelt*)g_TailEffectMng.AddEffect( g_Neuz.m_pd3dDevice, "etc_Tail2.bmp", 2, 0.35f );
		}
		
		D3DXVECTOR3	vPos1, vPos2;
		FLOAT		fAngXZ = GetAngle();
		FLOAT		fAngH  = GetAngleX();
		
		fAngXZ -= 90.0f;
		if( fAngXZ < 0 )
			fAngXZ += 360.0f;
		AngleToVector( &vPos1, fAngXZ, -fAngH, 0.05f );
		vPos1 += GetPos();
		
		fAngXZ = GetAngle();
		fAngH  = GetAngleX();
		
		fAngXZ += 90.0f;
		if( fAngXZ > 360.0f )
			fAngXZ -= 360.0f;
		AngleToVector( &vPos2, fAngXZ, -fAngH, 0.05f );
		vPos2 += GetPos();

		if( m_pTail )
			m_pTail->CreateTail( vPos1, vPos2 );
		
		vDelta = m_pSfxObj->m_vPos - m_vPosDest;		// ������ �Ÿ�
		if( D3DXVec3LengthSq( &vDelta ) < 0.4f * 0.4f )		// ���� �Ÿ����� �ٰ����� �浹.
		{
			if( m_dwSndHit != NULL_ID )
				PLAYSND( m_dwSndHit, &GetPos() );		// ���� �÷���.
			DamageToTarget();
			// ���� ������Ʈ ����.
			if( m_dwSfxHit != NULL_ID )
			{
				CSfx *pSfx = CreateSfx( D3DDEVICE, m_dwSfxHit, GetPos(), m_idSrc, m_vPosDest, m_idDest, 0 );
				if( pSfx )
				{
					pSfx->SetAngle( m_pSfxObj->m_vRotate.y );
				}
			}
			Delete();	// this�� ����.
		}
	}
	m_nFrame ++;

	if( m_nFrame >= SEC1 * 10 )		// �ʹ����� �i�ư��� �ʰ� 10�������� �Ҹ�
		Delete();	
}
void CSfxItemRangeAtk1::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render2( pd3dDevice );
}

// ������
D3DXVECTOR3	CSfxItemYoyoAtk::SplineSlerp( D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR3 *v3, D3DXVECTOR3 *v4, float fSlerp )
{
	D3DXVECTOR3		vResult;
	
	float	t = fSlerp;
	float	t2 = t * t;
	float	t3 = t2 * t;
	
	D3DXVECTOR3		m0, m1;
	const float alpha = 0.0f;
	
	m0 = ((1 - alpha) / 2.0f) * 
		((*v2 - *v1) + *v3 - *v2);
	m1 = ((1 - alpha) / 2.0f) *
		((*v3 - *v2) + *v4 - *v3);
	
	vResult = (((2 * t3) - (3 * t2) + 1) * *v2) +
		((t3 - (2 * t2) + t) * m0) + 
		((t3 - t2) * m1) +
		(((-2 * t3) + (3 * t2)) *
		*v3 );
	
	return vResult;
}

#define MAX_DELAY    10
CSfxItemYoyoAtk::CSfxItemYoyoAtk() : CSfxShoot()
{
	m_pTail			= NULL;
	m_nStep			= 0;
	m_nCount        = 0;
	m_nType			= 0;
	m_nDelayCount = 0;
}

CSfxItemYoyoAtk::~CSfxItemYoyoAtk()
{
	if( m_pTail )
		g_TailEffectMng.Delete( m_pTail );
	m_pTail = NULL;
}


void CSfxItemYoyoAtk::MakePath(int nType)
{
	// ���� 5����
	m_fMaxLength = 50.0f;
	
	CMover* pObj = (CMover*)prj.GetCtrl( m_idSrc );	

	if( !IsValidObj(pObj) )
	{
		Delete();	// this�� ����.
		return;
	}

	FLOAT fLength;
	
	fLength = D3DXVec3LengthSq( &D3DXVECTOR3(m_vPosDest - GetPos()) );

	if( fLength > 7.0f * 7.0f )  // 7���� �̻� �� ����
		fLength = 49.0f;

	if( fLength < 3.0f * 3.0f )  // 3���ͺ��� ������ 2����
		fLength = 12.0f;

	fLength /= 5.0f;

	m_nType = nType;

	///////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3		aNewPos[7];

	m_v3SrcPos = GetPos();
	D3DXVECTOR3 vLocal;
	
	// 5���� �� ��ġ����
	AngleToVectorXZ( &vLocal, 0, fLength );
	aNewPos[0] = vLocal;
	aNewPos[0].y -= 0.2f;
	
	AngleToVectorXZ( &vLocal, 0, fLength+0.5f );
	aNewPos[1] = vLocal;
	aNewPos[1].y -= 0.2f;

	AngleToVectorXZ( &vLocal, 0, fLength+1.0f );
	aNewPos[2] = vLocal;
	aNewPos[2].y -= 0.1f;

	// ����
	AngleToVectorXZ( &vLocal, 0, fLength+1.2f );
	aNewPos[3] = vLocal;

	// �ڵ��ư���
	AngleToVectorXZ( &vLocal, 0, fLength+1.0f );
	aNewPos[4] = vLocal;
	aNewPos[4].y += 0.1f;
	
	AngleToVectorXZ( &vLocal, 0, fLength+0.5f );
	aNewPos[5] = vLocal;
	aNewPos[5].y += 0.2f;
	
	AngleToVectorXZ( &vLocal, 0, fLength );
	aNewPos[6] = vLocal;
	aNewPos[6].y += 0.2f;

	D3DXVECTOR3 vSlp;
	int		a, b, c, d;
	int		nMaxVertex = 7;

	m_nMaxSpline = 0;
	for( int i = 0; i < nMaxVertex-1; i ++ )
	{
		// i ~ i+1���̸� �����Ѵ�.  
		a = i - 1;		if( a < 0 )	a = 0;
		b = i;
		c = i+1;
		d = i+2;		if( d >= nMaxVertex )	d = nMaxVertex - 1;
		for( int j = 0; j < MAX_SF_SLERP+1; j ++ )
		{
			if( m_nMaxSpline >= 30 )
			{
				LPCTSTR szErr = Error( "CSfxItemYoyoAtk Spline : ���� �ʰ� %d", m_nMaxSpline );
				//ADDERRORMSG( szErr );
				break;
			}
			vSlp = SplineSlerp( &aNewPos[a], &aNewPos[b], &aNewPos[c], &aNewPos[d], (float)j / MAX_SF_SLERP );
			m_aSpline[ m_nMaxSpline ] = vSlp;
			m_nMaxSpline ++;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 vDestNor  = m_vPosDest - GetPos();

	D3DXVECTOR3 vAxis;
	D3DXQUATERNION   qRot;
	
	FLOAT       fTheta;
	D3DXVec3Normalize( &vDestNor, &vDestNor );
	D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
	fTheta = D3DXVec3Dot( &vDir, &vDestNor );
	D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
	
	D3DXMATRIX mRot;
	D3DXMatrixRotationQuaternion( &mRot, &qRot );
	///////////////////////////////////////////////////////////////////////////////////////////////
				
	// ���ö��� ������ ���� ���� ��ȯ
	D3DXVECTOR3 vPosPos    = m_v3SrcPos;
	D3DXMATRIX mScal;
	D3DXMatrixScaling( &mScal, 1.0f, 1.0f, 1.0f );

	for( int i=0; i<m_nMaxSpline; i++ )
	{
		D3DXMATRIX mWorld = mRot * mScal * pObj->GetMatrixTrans();
		mWorld._42 = vPosPos.y;
		D3DXVec3TransformCoord( &m_aSpline[i], &m_aSpline[i], &mWorld );
	}
}

void CSfxItemYoyoAtk::Process()
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );	

	if( !IsValidObj(pObjSrc) )
	{
		Delete();	// this�� ����.
		return;
	}
	
	if( m_bHit == FALSE ) 
	{
		if( pObjSrc )
		{
			((CModelObject*)pObjSrc->m_pModel)->SetEffect( m_nType, XE_HIDE ); 
		}

		if( m_nDelayCount )
		{
			m_nDelayCount--;
			return;
		}

		// ���ư��� �ܰ�
		if( m_nStep == 0 )
		{
			D3DXVECTOR3 vPos   = GetPos();
			D3DXVECTOR3 vDelta = m_aSpline[0] - vPos;
			D3DXVec3Normalize( &vDelta, &vDelta );	
			vPos += vDelta * 0.3f;		
			m_pSfxObj->m_vPos = vPos;
			SetPos( vPos );
			
			FLOAT fLen = D3DXVec3LengthSq( &D3DXVECTOR3(m_aSpline[0] - vPos) );
			if( fLen <= 0.05f )
			{
				m_nStep = 1;
				m_nDelayCount = 6;
			}
		}
		else
		// ȸ���ϴ� �ܰ�
		if( m_nStep == 1 )
		{
			D3DXVECTOR3 vPosPos    = m_aSpline[m_nCount];
			
			m_pSfxObj->m_vPos = vPosPos;
			SetPos( vPosPos );

			m_nCount++;

			if( m_nCount >= m_nMaxSpline )
			{
				m_nStep = 2;
			}
		}
		else
		// �ǵ��� ���� �ܰ�
		if( m_nStep == 2 )
		{
			D3DXVECTOR3 v3SrcPos;
			CModelObject *pModel = (CModelObject *)pObjSrc->m_pModel;
			pModel->GetHandPos( &v3SrcPos, m_nType, pObjSrc->GetMatrixWorld() );
			
			D3DXVECTOR3 vPos   = GetPos();
			D3DXVECTOR3 vDelta = v3SrcPos - vPos;
			D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
			vPos += vDelta * 0.4f;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
			m_pSfxObj->m_vPos = vPos;
			SetPos( vPos );

			FLOAT fLen = D3DXVec3LengthSq( &D3DXVECTOR3(v3SrcPos - vPos) );
			if( fLen <= 0.1f )
			{
				if( m_dwSndHit != NULL_ID )
					PLAYSND( m_dwSndHit, &GetPos() );		// ���� �÷���.

				// ���� ������Ʈ ����.
				if( m_dwSfxHit != NULL_ID )
				{
					CSfx *pSfx = CreateSfx( D3DDEVICE, m_dwSfxHit, GetPos(), m_idSrc, m_vPosDest, m_idDest, 0 );
					if( pSfx )
					{
						pSfx->SetAngle( m_pSfxObj->m_vRotate.y );
					}
				}
				Delete();	// this�� ����.
				return;
			}
		}

		if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		if( m_pTail == NULL )	// ���� �Ҵ� �ȵ����� �Ҵ��ϰ�.
		{
			if( m_pSfxObj->m_pSfxBase->Part(0) )
				m_pTail = (CTailEffectModel*)g_TailEffectMng.AddEffect( g_Neuz.m_pd3dDevice, m_pSfxObj->m_pSfxBase->Part(0)->m_strTex, 100, 30.0f );
		}

		if( m_pTail )
		{
			D3DXMATRIX mWorld;
			D3DXMatrixTranslation( &mWorld, GetPos().x, GetPos().y, GetPos().z );
			m_pTail->CreateTail( &mWorld );
		}
	}
	m_nFrame ++;

	if( m_nFrame >= SEC1 * 5 )		// �ʹ����� �i�ư��� �ʰ� 5�������� �Ҹ�
		Delete();	
}

void CSfxItemYoyoAtk::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render2( pd3dDevice );
}



// ���͸� ȭ�� �����Ѵ�. �ϳ��� ��¥~
CSfxItemRangeAtk_JunkBow::CSfxItemRangeAtk_JunkBow() : CSfxShoot()
{
	m_pTail[0]			 = NULL;
	m_pTail[1]			 = NULL;
	m_pTail[2]			 = NULL;
}
CSfxItemRangeAtk_JunkBow::~CSfxItemRangeAtk_JunkBow()
{
	for( int i=0; i<3; i++ )
	{
		if( m_pTail[i] )
			g_TailEffectMng.Delete( m_pTail[i] );

		m_pTail[i] = NULL;
	}
}

void CSfxItemRangeAtk_JunkBow::Process()
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );	// ����ź ���̱⶧���� Ÿ���� �׻� �о���� �Ѵ�.
	if( IsValidObj(pObjDest) )
	{
		m_vPosDest = pObjDest->GetPos() + D3DXVECTOR3( 0, 1.0f, 0 );
	} else
	{
		// ���ư��� Ÿ���� ���������� ��������ǥ�� ���ư��� ������.
		Delete();	// this�� ����.
		int a = 0;
	}
	
	if( m_bHit == FALSE ) 
	{
		if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vPos = GetPos();
		D3DXVECTOR3 vDelta = m_vPosDest - vPos;
		D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
		vPos += vDelta * 0.7f;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
		m_pSfxObj->m_vPos = vPos;
		SetPos( vPos );
		
		//if( m_bDir )
		{
			// ������ü ���⺤�ͷ� �� ���� ������~~~
			D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			D3DXVECTOR3 vDestNor  = m_vPosDest - m_pSfxObj->m_vPos;
			D3DXVECTOR3 vAxis;
			D3DXQUATERNION   qRot;
			
			FLOAT       fTheta;
			D3DXVec3Normalize( &vDestNor, &vDestNor );
			D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
			fTheta = D3DXVec3Dot( &vDir, &vDestNor );
			D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
			
			D3DXVECTOR3 vYPW;
			
			QuaternionRotationToYPW( qRot, vYPW );
			m_pSfxObj->m_vRotate.x = m_SfxObj2.m_vRotate.x = D3DXToDegree(vYPW.x);
			m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = D3DXToDegree(vYPW.y);
			m_pSfxObj->m_vRotate.z = m_SfxObj2.m_vRotate.z = D3DXToDegree(vYPW.z);

		}
		
		for( int i=0; i<3; i++ )
		{
			if( m_pTail[i] )
			{
				if( m_pTail[i]->GetType() != 2 )	// �����Ǿ��� ������ �Ϲݺ������ �ƴϸ� 
					m_pTail[i]->ChangeTexture( D3DDEVICE, "etc_Tail2.bmp", 2 );	// �Ϲݺ�������� �ؽ��� ��ü.
			}
			if( m_pTail[i] == NULL )	// ���� �Ҵ� �ȵ����� �Ҵ��ϰ�.
			{
				m_pTail[i] = (CTailEffectBelt*)g_TailEffectMng.AddEffect( g_Neuz.m_pd3dDevice, "etc_Tail2.bmp", 2, 0.35f );
			}
		}
		
		D3DXVECTOR3	vPos1, vPos2;
		FLOAT		fAngXZ = GetAngle();
		FLOAT		fAngH  = GetAngleX();
		
		fAngXZ -= 90.0f;
		if( fAngXZ < 0 )
			fAngXZ += 360.0f;
		AngleToVector( &vPos1, fAngXZ, -fAngH, 0.05f );
		vPos1 += GetPos();
		
		fAngXZ = GetAngle();
		fAngH  = GetAngleX();
		
		fAngXZ += 90.0f;
		if( fAngXZ > 360.0f )
			fAngXZ -= 360.0f;
		AngleToVector( &vPos2, fAngXZ, -fAngH, 0.05f );
		vPos2 += GetPos();

		vPos1.y -= 0.5f;
		vPos2.y -= 0.5f;
		
		for( int i=0; i<3; i++ )
		{
			if( m_pTail[i] )
			{
				m_pTail[i]->CreateTail( vPos1, vPos2 );
			}

			vPos1.y += 0.5f;
			vPos2.y += 0.5f;
		}
		
		vDelta = m_pSfxObj->m_vPos - m_vPosDest;		// ������ �Ÿ�
		if( D3DXVec3LengthSq( &vDelta ) < 0.4f * 0.4f )		// ���� �Ÿ����� �ٰ����� �浹.
		{
			if( m_dwSndHit != NULL_ID )
				PLAYSND( m_dwSndHit, &GetPos() );		// ���� �÷���.
			DamageToTarget();
			// ���� ������Ʈ ����.
			if( m_dwSfxHit != NULL_ID )
			{
				CSfx *pSfx = CreateSfx( D3DDEVICE, m_dwSfxHit, GetPos(), m_idSrc, m_vPosDest, m_idDest, 0 );
				if( pSfx )
				{
					pSfx->SetAngle( m_pSfxObj->m_vRotate.y );
				}
			}
			Delete();	// this�� ����.
		}
	}
	m_nFrame ++;

	if( m_nFrame >= SEC1 * 10 )		// �ʹ����� �i�ư��� �ʰ� 10�������� �Ҹ�
		Delete();	
}
void CSfxItemRangeAtk_JunkBow::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();

	// ��ǥ ���
	D3DXVECTOR3 vPosBackup = m_pSfxObj->m_vPos;
	// ��¥ ȭ��
	m_pSfxObj->Render2( pd3dDevice );
	
	// ��¥ 1
	m_pSfxObj->m_vPos.y += 0.5f;
	m_pSfxObj->Render2( pd3dDevice );

	// ��¥ 2
	m_pSfxObj->m_vPos = vPosBackup;
	m_pSfxObj->m_vPos.y -= 0.5f;
	m_pSfxObj->Render2( pd3dDevice );


	// ��ǥ����
	m_pSfxObj->m_vPos = vPosBackup;
	
}


// �Ʒ��� �������� ȭ�� ����Ʈ
CSfxItemRangeAtk1_Allow::CSfxItemRangeAtk1_Allow() : CSfxShoot()
{
	m_nFrame = 0;
	m_fSpeed = xRandomF( 0.7f ) + 0.2f;
}

CSfxItemRangeAtk1_Allow::~CSfxItemRangeAtk1_Allow()
{
}

void CSfxItemRangeAtk1_Allow::Process()
{
	if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
	{
		m_pSfxObj->m_nCurFrame = 0;
	}

	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDelta = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�

	vPos += vDelta * m_fSpeed;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.

	CWorld* pWorld = g_WorldMng.Get();
	FLOAT fHeight = pWorld->GetLandHeight_Fast( vPos.x, vPos.z );

	// �� ���̺��� ������ �ȵ�
	if( vPos.y < fHeight )
	{
		m_nFrame++;
		vPos.y = fHeight;
		
		CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_RAG_BOW_ARROWRAIN01, vPos );//, m_idSrc, vPos, m_idDest, 0 );		
		Delete();
	}

	m_pSfxObj->m_vPos = vPos;
		
	SetPos( vPos );
}
void CSfxItemRangeAtk1_Allow::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render2( pd3dDevice );
}

#define MAX_ALLOW		50

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
CSfxItemRangeAtk1_AllowRain::CSfxItemRangeAtk1_AllowRain() : CSfxShoot()
{
	m_nFrame = 0;
	m_nCount = 0;
}

CSfxItemRangeAtk1_AllowRain::~CSfxItemRangeAtk1_AllowRain()
{
}

void CSfxItemRangeAtk1_AllowRain::Process()
{
	D3DXVECTOR3 vPos = GetPos();
	
	CWorld* pWorld = g_WorldMng.Get();
	FLOAT fHeight = pWorld->GetLandHeight_Fast( vPos.x, vPos.z );
	
	// 10���������� ����߸�
	vPos.y = fHeight + xRandomF(2.0f) + 9.0f;

	m_nFrame++;

	if( m_nCount < MAX_ALLOW )
	{
		if( m_nFrame >= SEC1 * 0.07f )
		{
			m_nFrame = 0;
			vPos.x += (xRandomF( 6.0f ) + -3.0f);
			vPos.z += (xRandomF( 6.0f ) + -3.0f);

			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_RAG_BOW_ARROWRAIN, vPos, m_idSrc, m_vPosDest, m_idDest, 0 );

			vPos.x += (xRandomF( 6.0f ) + -3.0f);
			vPos.z += (xRandomF( 6.0f ) + -3.0f);
			
			pSfx = CreateSfx( D3DDEVICE, XI_SKILL_RAG_BOW_ARROWRAIN, vPos, m_idSrc, m_vPosDest, m_idDest, 0 );
			m_nCount++;
		}
	}
	else
		Delete();
}
void CSfxItemRangeAtk1_AllowRain::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
}

CSfxItemRangeAtk1_Stone::CSfxItemRangeAtk1_Stone() : CSfxShoot()
{
	m_nFrame = 0;
	m_fSpeed = xRandomF( 0.7f ) + 0.7f;
}

CSfxItemRangeAtk1_Stone::~CSfxItemRangeAtk1_Stone()
{
}

void CSfxItemRangeAtk1_Stone::Process()
{
	if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
	{
		m_pSfxObj->m_nCurFrame = 0;
	}
	
	D3DXVECTOR3 vPos = GetPos();
	D3DXVECTOR3 vDelta = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
	
	vPos += vDelta * m_fSpeed;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
	
	CWorld* pWorld = g_WorldMng.Get();
	FLOAT fHeight = pWorld->GetLandHeight_Fast( vPos.x, vPos.z );
	
	// �� ���̺��� ������ �ȵ�
	if( vPos.y < fHeight+2.5f )
	{
		m_nFrame++;
		vPos.y = fHeight;
		
		CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_CIRCLE_DUST, vPos );		
		if( pSfx )
		{
			pSfx->SetScale( D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );					
		}			
		Delete();
	}
	
	m_pSfxObj->m_vPos = vPos;
	
	SetPos( vPos );
}
void CSfxItemRangeAtk1_Stone::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render2( pd3dDevice );
}

#define MAX_STONE		50

// CSfxItemRangeAtk1_Allow���� ���� - ���� ���Ǿ����� �κ�
CSfxItemRangeAtk1_StoneRain::CSfxItemRangeAtk1_StoneRain() : CSfxShoot()
{
	m_nFrame = 0;
	m_nCount = 0;
}

CSfxItemRangeAtk1_StoneRain::~CSfxItemRangeAtk1_StoneRain()
{
}

void CSfxItemRangeAtk1_StoneRain::Process()
{
	D3DXVECTOR3 vPos = GetPos();
	
	CWorld* pWorld = g_WorldMng.Get();
	FLOAT fHeight = pWorld->GetLandHeight_Fast( vPos.x, vPos.z );
	
	// 10���������� ����߸�
	vPos.y = fHeight + xRandomF(2.0f) + 40.0f;
	
	m_nFrame++;
	
	if( m_nCount < MAX_STONE )
	{
		if( m_nFrame >= SEC1 * 0.2f )
		{
			m_nFrame = 0;
			vPos.x += (xRandomF( 40.0f ) + -20.0f);
			vPos.z += (xRandomF( 40.0f ) + -20.0f);
			
			CSfx *pSfx = CreateSfx( D3DDEVICE, XI_SKILL_DROP_DUST, vPos, m_idSrc, m_vPosDest, m_idDest, 0 );
			if( pSfx )
			{
				pSfx->SetScale( D3DXVECTOR3( 8.0f, 8.0f, 8.0f ) );					
			}
			m_nCount++;
		}
	}
	else
		Delete();
}
void CSfxItemRangeAtk1_StoneRain::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
}

//
// ���������� ������� ������ ��Ÿ� ����
//
CSfxAtkStraight::CSfxAtkStraight()
{
	m_dwExplosion = NULL_ID;
}
CSfxAtkStraight::~CSfxAtkStraight()
{
}

// �߻�ü�� �̵��������� �������. 
void CSfxAtkStraight::ShootSfx( float fAngXZ, float fAngH, float fSpeed, DWORD dwExplosion )
{
	m_pSfxObj->m_vPos = GetPos();		// sfx���� ��ġ�� ����.
	m_dwExplosion = dwExplosion;

	AngleToVector( &m_vDelta, fAngXZ, fAngH, fSpeed );		// �̵��� vDelta�� ����.

	// ������ü ���⺤�ͷ� �� ���� ������~~~
	D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	D3DXVECTOR3 vDestNor  = m_vDelta;//m_vPosDest - GetPos();
	D3DXVECTOR3 vAxis;
	D3DXQUATERNION   qRot;
	
	FLOAT       fTheta;
	D3DXVec3Normalize( &vDestNor, &vDestNor );
	D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
	fTheta = D3DXVec3Dot( &vDir, &vDestNor );
	D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
	
	D3DXVECTOR3 vYPW;
	
	QuaternionRotationToYPW( qRot, vYPW );
	m_pSfxObj->m_vRotate.x = D3DXToDegree(vYPW.x);
	m_pSfxObj->m_vRotate.y = D3DXToDegree(vYPW.y);
	m_pSfxObj->m_vRotate.z = D3DXToDegree(vYPW.z);
	
}
				  
void CSfxAtkStraight::Process()
{
	if( m_pSfxObj->Process() )			// SFX �ִϸ��̼��� ó���ϰ�
		m_pSfxObj->m_nCurFrame = 0;		// ��ǥ���� ���������� ������Ű�� ���� �ٽ� 0���������� �������´�.
	
	m_pSfxObj->m_vPos += m_vDelta;		// ��ǥ �̵�.
	SetPos( m_pSfxObj->m_vPos );		// this�� ��ǥ ���� �̵�.
	
#if __VER >= 10 // __Y_DRAGON_FIRE	
	#ifdef __CLIENT
		//D3DXVECTOR3 vDir = v3 - v1;
		//D3DXVec3Normalize( &vDir, &vDir );
		D3DXVECTOR3 vTemp = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 vScal = D3DXVECTOR3( 0.3f, 0.3f, 0.3f );
		D3DXVECTOR3 vModelScal = GetScale();

		vScal.x *= vModelScal.x;
		vScal.y *= vModelScal.y;
		vScal.z *= vModelScal.z;

		for( int i = 0; i < 2; i ++ )
		{
			//vTemp = vDir * (xRandomF( 0.2f ) + 0.1f );
			//vTemp *= 0.8f;
			//vTemp.y -= (xRandomF( 0.05f ) + 0.01f );
			
			//extern CPartsFireDragon	g_FireDragon;
			//g_FireDragon.Create( D3DDEVICE, m_pSfxObj->m_vPos, XI_NAT_FIRE01_ADV, vScal, vTemp );
		}
	#endif
#endif //__Y_DRAGON_FIRE

	if( IsRangeObj( m_vPosDest, 0 ) )							// ��ǥ��ǥ�� �ٴٸ���
	{
		CreateSfx( D3DDEVICE, m_dwExplosion, m_vPosDest );		// ���� ����Ʈ
		Delete();												// �� ����.
		return;
	}

	m_nFrame++;
	if( m_nFrame > 60 * 3 )
	{
		Delete();
	}
}
void CSfxAtkStraight::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render2( pd3dDevice, NULL );
	
}


CSfxItemWandAtk2::CSfxItemWandAtk2()
{
	m_SfxObj2.SetSfx("sfx_HitWand01"); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit=FALSE;
}
CSfxItemWandAtk2::~CSfxItemWandAtk2()
{
}
void CSfxItemWandAtk2::Process()
{
	if(m_nFrame==0) {
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if(pObjDest) m_SfxObj2.m_vPos=pObjDest->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);

	if(m_bHit==FALSE) {
		m_pSfxObj->Process();
		if(m_pSfxObj->m_nCurFrame==20)
		{
			m_pSfxObj->m_nCurFrame=0;
		}
		D3DXVECTOR3 vDelta=m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos-m_SfxObj2.m_vPos;
		FLOAT fAngle;
		if(vDelta.z>0) {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f)+180;
		}
		else {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f);
		}
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=fAngle;
		if(D3DXVec3Length(&vDelta)<.2f) {
			m_bHit=TRUE;
		}
	}
	else {
		if(m_SfxObj2.m_nCurFrame==0) {
			// ����ٰ� �´¼���ó��
			DamageToTarget();
		}
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxItemWandAtk2::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
CSfxItemWandAtk3::CSfxItemWandAtk3()
{
	m_SfxObj2.SetSfx("sfx_HitWand01"); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit=FALSE;
}
CSfxItemWandAtk3::~CSfxItemWandAtk3()
{
}
void CSfxItemWandAtk3::Process()
{
	if(m_nFrame==0) {
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if(pObjDest) m_SfxObj2.m_vPos=pObjDest->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);

	if(m_bHit==FALSE) {
		m_pSfxObj->Process();
		if(m_pSfxObj->m_nCurFrame==20)
		{
			m_pSfxObj->m_nCurFrame=0;
		}
		D3DXVECTOR3 vDelta=m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos-m_SfxObj2.m_vPos;
		FLOAT fAngle;
		if(vDelta.z>0) {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f)+180;
		}
		else {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f);
		}
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=fAngle;
		if(D3DXVec3Length(&vDelta)<.2f) {
			m_bHit=TRUE;
		}
	}
	else {
		if(m_SfxObj2.m_nCurFrame==0) {
			// ����ٰ� �´¼���ó��
			DamageToTarget();
		}
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxItemWandAtk3::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
CSfxItemWandAtk4::CSfxItemWandAtk4()
{
	m_SfxObj2.SetSfx("sfx_HitWand01"); // �´� ������ SFX�� ���� �ε��Ѵ�.
	m_bHit=FALSE;
}
CSfxItemWandAtk4::~CSfxItemWandAtk4()
{
}
void CSfxItemWandAtk4::Process()
{
	if(m_nFrame==0) {
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if(pObjDest) m_SfxObj2.m_vPos=pObjDest->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);

	if(m_bHit==FALSE) {
		m_pSfxObj->Process();
		if(m_pSfxObj->m_nCurFrame==20)
		{
			m_pSfxObj->m_nCurFrame=0;
		}
		D3DXVECTOR3 vDelta=m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos-m_SfxObj2.m_vPos;
		FLOAT fAngle;
		if(vDelta.z>0) {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f)+180;
		}
		else {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f);
		}
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=fAngle;
		if(D3DXVec3Length(&vDelta)<.2f) {
			m_bHit=TRUE;
		}
	}
	else {
		if(m_SfxObj2.m_nCurFrame==0) {
			// ����ٰ� �´¼���ó��
			DamageToTarget();
		}
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxItemWandAtk4::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ų ȿ����

// ����� �⺻ ��ų 1
CSfxSkillVagOverCutter::CSfxSkillVagOverCutter()
{
}
CSfxSkillVagOverCutter::~CSfxSkillVagOverCutter()
{
}
void CSfxSkillVagOverCutter::Process()
{
	m_nFrame++;
	{		
		CMover* pObjDst = (CMover*)prj.GetCtrl( m_idDest );
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if( pObjDst && pObjSrc ) 
		{
			SetPos( pObjDst->GetPos() + D3DXVECTOR3( .0f, 1.0f, .0f) );
			SetAngle( 180.0f - pObjSrc->GetAngle() ); // �������� ����� ��ġ��Ų��.
		}
		if( m_pSfxObj->Process() )
			Delete();
	}
}
void CSfxSkillVagOverCutter::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}
// ����� �⺻ ��ų 2
CSfxSkillVagCleanHit::CSfxSkillVagCleanHit()
{
}
CSfxSkillVagCleanHit::~CSfxSkillVagCleanHit()
{
}
void CSfxSkillVagCleanHit::Process()
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if( IsValidObj(pObjSrc) ) 
	{
		SetPos( m_vPosDest );		// ��ǥ������ �׻� �߻��Ǿ� ����.
		SetAngle( 180.0f - pObjSrc->GetAngle() ); // �������� ����� ��ġ��Ų��.
	}
	if( m_pSfxObj->Process() )		// �ִϸ��̼��� ������ ��.
		Delete();

	m_nFrame++;
	
}
void CSfxSkillVagCleanHit::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();//+180;
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}


// ������ǥ ���� sfx
CSfxFixed::CSfxFixed()
{
}
CSfxFixed::~CSfxFixed()
{
}
void CSfxFixed::Process()
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if( IsValidObj(pObjSrc) ) 
	{
		SetPos( m_vPosDest );		// ��ǥ������ �׻� �߻��Ǿ� ����.
	}
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
	
	m_nFrame++;
}

void CSfxFixed::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ����� �⺻ ��ų 3
CSfxSkillVagBrandish::CSfxSkillVagBrandish()
{
}
CSfxSkillVagBrandish::~CSfxSkillVagBrandish()
{
}
void CSfxSkillVagBrandish::Process()
{
	m_nFrame++;
	{ 
		CMover* pObjDst = (CMover*)prj.GetCtrl( m_idDest );
		if( pObjDst ) 
		{
			SetPos( pObjDst->GetPos() + D3DXVECTOR3( .0f, 1.0f, .0f)  );
			SetAngle( 180.0f - pObjDst->GetAngle() );
		}
		if(m_pSfxObj->Process())
			Delete();
	}
}
void CSfxSkillVagBrandish::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// �Ӽųʸ� �⺻ ��ų 1
CSfxSkillMerKeenWheel::CSfxSkillMerKeenWheel()
{
}
CSfxSkillMerKeenWheel::~CSfxSkillMerKeenWheel()
{
}
void CSfxSkillMerKeenWheel::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxSkillMerKeenWheel::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}
// �Ӽųʸ� �⺻ ��ų 2
CSfxSkillMerSplmash::CSfxSkillMerSplmash()
{
	m_SfxObj2.SetSfx(XI_SKILL_MER_ONE_SPLMASH02);
}
CSfxSkillMerSplmash::~CSfxSkillMerSplmash()
{
}
void CSfxSkillMerSplmash::Process()
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if(m_nFrame==0 && pObjSrc) 
	{
		SetPos(pObjSrc->GetPos());
		SetAngle(180.0f-pObjSrc->GetAngle());
	}
	m_pSfxObj->Process();
	if( m_nFrame > 85 ) 
	{
		if( m_SfxObj2.Process() )
		{
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMerSplmash::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_SfxObj2.m_vPos=GetPos();
	m_SfxObj2.m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_SfxObj2.m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
	if(m_nFrame>85) 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
// �Ӽųʸ� �⺻ ��ų 3
CSfxSkillMerBlindSide::CSfxSkillMerBlindSide()
{
}
CSfxSkillMerBlindSide::~CSfxSkillMerBlindSide()
{
}
void CSfxSkillMerBlindSide::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxSkillMerBlindSide::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ��� ��Ŭ��ų
CSfxSkillAssBurstcrack::CSfxSkillAssBurstcrack()
{
}
CSfxSkillAssBurstcrack::~CSfxSkillAssBurstcrack()
{
}
void CSfxSkillAssBurstcrack::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}

	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if( IsValidObj( pObjSrc ) )
	{
		m_pSfxObj->m_vPos = GetPos();
		m_pSfxObj->m_vPos.y += 0.1f;
		m_pSfxObj->m_vRotate.y = 180 - pObjSrc->GetAngle();
		m_pSfxObj->m_vScale    = D3DXVECTOR3( 0.6f, 0.6f, 0.6f );
	}
	else
		Delete();
}
void CSfxSkillAssBurstcrack::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->Render( pd3dDevice );
}

CSfxSkillAssTampinghole::CSfxSkillAssTampinghole()
{
}
CSfxSkillAssTampinghole::~CSfxSkillAssTampinghole()
{
}
void CSfxSkillAssTampinghole::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}

	CMover* pObjSrc  = (CMover*)prj.GetCtrl( m_idSrc );
	
	if( IsValidObj( pObjSrc ) )
	{
		m_pSfxObj->m_vPos = GetPos();
		m_pSfxObj->m_vPos.y += 1.0f;
		
		m_pSfxObj->m_vRotate.y = 180 - pObjSrc->GetAngle();
		
		D3DXMATRIX mRot;
		D3DXVECTOR3 vDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXMatrixRotationY( &mRot, D3DXToRadian( m_pSfxObj->m_vRotate.y ) );
		D3DXVec3TransformCoord( &vDir, &vDir, &mRot );
		
		D3DXVec3Normalize( &vDir, &vDir );
		
		m_pSfxObj->m_vPos += vDir*1.0f;
	}
	else
		Delete();
}
void CSfxSkillAssTampinghole::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->Render( pd3dDevice );
}

CSfxNpcDirSteam::CSfxNpcDirSteam()
{
	m_fInit = TRUE;
}
CSfxNpcDirSteam::~CSfxNpcDirSteam()
{
}
void CSfxNpcDirSteam::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}

	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );

	D3DXVECTOR3 vDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

	if( IsInvalidObj(pObjDest) )
	{
		Delete();
		return;
	}
	if( IsValidObj( pObjSrc ) )
	{
		MoverProp* pMoverProp = prj.GetMoverProp( pObjSrc->GetIndex() );

		if( pMoverProp == NULL )
			return;

		switch( pMoverProp->dwClass )
		{
			case RANK_LOW:
			case RANK_NORMAL:
			case RANK_CAPTAIN:
			case RANK_BOSS:
				{
					m_pSfxObj->m_vRotate.y = 180 - pObjSrc->GetAngle();
					
					CModelObject *pModel = (CModelObject *)pObjSrc->m_pModel;
					D3DXVECTOR3 v;
					pModel->GetEventPos( &v, 0 );
					D3DXVec3TransformCoord( &m_pSfxObj->m_vPos, &v, &pObjSrc->GetMatrixWorld() );

					if( pMoverProp->dwClass == RANK_BOSS )
						m_pSfxObj->m_vScale = D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
					else
						m_pSfxObj->m_vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
				}
				break;
		}
	}
	else
		Delete();
	
	if( m_fInit )
	{
 		vDir                  = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = pObjDest->GetPos() - m_pSfxObj->m_vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
		
		D3DXVECTOR3 vYPW;
		QuaternionRotationToYPW( qRot, vYPW );
		
		m_pSfxObj->m_vRotate.x = D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y = D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z = D3DXToDegree(vYPW.z);
		
		if( m_pSfxObj->m_vRotate.x >= 45.0f )
			m_pSfxObj->m_vRotate.x = 45.0f;
		
		if( m_pSfxObj->m_vRotate.z >= 45.0f )
			m_pSfxObj->m_vRotate.z = 45.0f;
		
		m_fInit = FALSE;
	}
}
void CSfxNpcDirSteam::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->Render2( pd3dDevice );
}


// ������ �� �ֹ� �ܱ�
CSfxSkillMagFireCasting::CSfxSkillMagFireCasting()
{
}
CSfxSkillMagFireCasting::~CSfxSkillMagFireCasting()
{
}
void CSfxSkillMagFireCasting::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxSkillMagFireCasting::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
	if( IsValidObj( pObjSrc ) )
	{
		m_pSfxObj->m_vPos = GetPos();
		m_pSfxObj->m_vPos.y += 1.0f;
		m_pSfxObj->m_vRotate.y = 180 - pObjSrc->GetAngle();
		m_pSfxObj->Render( pd3dDevice );
	}
}
// ������ �ٶ� �ֹ� �ܱ�
CSfxSkillMagWindCasting::CSfxSkillMagWindCasting()
{
}
CSfxSkillMagWindCasting::~CSfxSkillMagWindCasting()
{
}
void CSfxSkillMagWindCasting::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxSkillMagWindCasting::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vPos.y += 0.2f;
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}

// ������ �⺻ ��ų 1
CSfxSkillMagStrongWind::CSfxSkillMagStrongWind()
{
	m_SfxObj2.SetSfx(XI_SKILL_MAG_WIND_STRONGWIND01_01);		// �¾����� ȿ��.
	m_bHit = FALSE;
	m_fAngle = 0;
}
CSfxSkillMagStrongWind::~CSfxSkillMagStrongWind()
{
}
				  
void CSfxSkillMagStrongWind::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = prj.GetMover( m_idSrc );
		if(IsValidObj(pObjSrc)) 
			m_pSfxObj->m_vPos = pObjSrc->GetPos();
	}
	CMover* pObjDest = prj.GetMover( m_idDest );
	if(IsValidObj(pObjDest)) 
		m_SfxObj2.m_vPos = pObjDest->GetPos();

	if( m_bHit == FALSE ) 
	{
		m_pSfxObj->Process();
		if( m_pSfxObj->m_nCurFrame == 60 )
		{
			m_pSfxObj->m_nCurFrame = 30;
		}
		D3DXVECTOR3 vDelta=m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize( &vDelta, &vDelta );
		m_pSfxObj->m_vPos += vDelta * .2f;
		SetPos( m_pSfxObj->m_vPos );
		vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		FLOAT fAngle;
		fAngle = atan2( vDelta.x, -vDelta.z );
		fAngle = D3DXToDegree( fAngle );
		m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = fAngle;
		if( D3DXVec3Length(&vDelta) < 0.2f ) 
		{
			PLAYSND( SND_PC_SKILLM_STRONGWIND2, &GetPos() );     
			m_bHit = TRUE;
		} else
			m_fAngle = fAngle;	// else���� ���������� �ʹ� ������ �پ������� ������ ������ ������ �ֱ⶧����..
	}
	else {
		if(m_SfxObj2.m_nCurFrame==0)
		{
			float fDmgPower = 0;
			ItemProp *pSkillProp = prj.GetSkillProp( m_dwSkill );
			if( pSkillProp && pSkillProp->dwDmgShift != NULL_ID )
				fDmgPower = (int)pSkillProp->dwDmgShift * 0.2f;		// �����Ѱ����� ��ȯ�ؼ� ���.
			int fAngle = (int)( m_fAngle );
			if( IsValidObj(pObjDest) )	// �������� �и��� ����.
			{
				if( pObjDest->GetProp()->dwClass == RANK_SUPER || pObjDest->GetProp()->dwClass == RANK_MIDBOSS )
				{
					fAngle = 0;		fDmgPower = 0;
				}
			}
			
			DamageToTarget( 0, (float)( fAngle ), fDmgPower );
			// Ÿ�ٿ� �������� ���� ������ �ܺ��� 0.x�� ����.
			if( IsValidObj(pObjDest) )
			{
				// �������ö��� �߿� 
					if( (pObjDest->m_pActMover->GetState() & OBJSTA_DMG_FLY_ALL) == 0 )
						AngleToVectorXZ( &pObjDest->m_pActMover->m_vDeltaE, m_fAngle, fDmgPower );	
			}
		}
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMagStrongWind::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
// ������ �⺻ ��ų 2
CSfxSkillMagSwordWind::CSfxSkillMagSwordWind()
{
	m_SfxObj2.SetSfx(XI_SKILL_MAG_WIND_SWORDWIND01_01);
	m_bHit=FALSE;
}
CSfxSkillMagSwordWind::~CSfxSkillMagSwordWind()
{
}
void CSfxSkillMagSwordWind::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,0.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if(pObjDest) m_SfxObj2.m_vPos=pObjDest->GetPos()+D3DXVECTOR3(.0f,0.0f,.0f);

	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Process();
		if(m_pSfxObj->m_nCurFrame>=30)
		{
			m_pSfxObj->m_nCurFrame=0;
			m_pSfxObj->Process();
		}
		D3DXVECTOR3 vDelta=m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos-m_SfxObj2.m_vPos;

		FLOAT fAngle;
		if(vDelta.z>0) {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f)+180;
		}
		else {
			fAngle=(float)(atan(vDelta.x/vDelta.z)*180/3.1415926f);
		}
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=fAngle;
		if(D3DXVec3Length(&vDelta)<.4f) 
		{
			PLAYSND( SND_PC_SKILLM_SWORDWIND2, &GetPos() );     
			m_bHit=TRUE;
		}
	}
	else 
	{
		if(m_SfxObj2.m_nCurFrame==0)
			DamageToTarget();
		
		if(m_SfxObj2.Process()) 
			Delete();
	}
	m_nFrame++;
}
void CSfxSkillMagSwordWind::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
// ������ �⺻ ��ų 3
CSfxSkillMagFireBoomerang::CSfxSkillMagFireBoomerang()
{
	m_SfxObj2.SetSfx( XI_SKILL_MAG_FIRE_BOOMERANG01_01 );
	m_bHit=FALSE;
}
CSfxSkillMagFireBoomerang::~CSfxSkillMagFireBoomerang()
{
}
void CSfxSkillMagFireBoomerang::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest )
	{
		m_SfxObj2.m_vPos = pObjDest->GetPos() + D3DXVECTOR3(.0f,0.3f,.0f);
		m_SfxObj2.m_vScale = D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
	}
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Process();
		if( m_pSfxObj->m_nCurFrame == 20 )
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;


		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
		
		D3DXVECTOR3 vYPW;
		
		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x=m_SfxObj2.m_vRotate.x=D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z=m_SfxObj2.m_vRotate.z=D3DXToDegree(vYPW.z);

		if( D3DXVec3Length( &vDelta ) < .2f ) 
		{
			PLAYSND( SND_PC_SKILLM_FIREBOOMERANG2, &GetPos() );
			m_bHit=TRUE;
		}
	}
	else 
	{
		if(m_SfxObj2.m_nCurFrame==0)
			DamageToTarget();
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMagFireBoomerang::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}
// ������ �⺻ ��ų 4
CSfxSkillMagFireBomb::CSfxSkillMagFireBomb()
{
}
CSfxSkillMagFireBomb::~CSfxSkillMagFireBomb()
{
}
void CSfxSkillMagFireBomb::Process()
{
	m_nFrame++;
	if(m_pSfxObj->Process())
	{
		Delete();
	}
}
void CSfxSkillMagFireBomb::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vPos=GetPos();
	m_pSfxObj->m_vRotate.y=GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render( pd3dDevice );
}
// ������ �⺻ ��ų 5
CSfxSkillMagHotAir::CSfxSkillMagHotAir()
{
	m_SfxObj2.SetSfx(XI_SKILL_MAG_FIRE_HOTAIR01_01);		// Ÿ�������� �� ������ ȸ���ϴ� ȿ��
	m_nDmgCnt = 0;
}
CSfxSkillMagHotAir::~CSfxSkillMagHotAir()
{
}
void CSfxSkillMagHotAir::Process()
{
	CMover* pObjSrc = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pObjSrc ) )
	{
		Delete();
		return;
	}
	{
		m_pSfxObj->m_vPos = pObjSrc->GetPos();		// �߻������� Ÿ�������� ��Ÿ���� ȿ��
		m_pSfxObj->m_vRotate = D3DXVECTOR3(.0f,-pObjSrc->GetAngle(),.0f);
	}
	BOOL res1 = m_pSfxObj->Process();
	CMover* pObjDest = prj.GetMover( m_idDest );
	if( IsInvalidObj( pObjDest ) )
	{
		Delete();
		return;
	}
#ifdef __CLIENT
	#if __VER >= 11 // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
	else
	{
		DWORD dwTmpID =  pObjDest->GetRemoveSfxObj(XI_SKILL_MAG_FIRE_HOTAIR01 );
		if( dwTmpID && m_nFrame > 0)
		{
			Delete();
			return;
		}
	}
	#endif // __MA_VER11_06				// Ȯ����ų ȿ������ world,neuz
#endif	// __CLIENT

	m_SfxObj2.m_vPos = pObjDest->GetPos();

	ItemProp *pSkillProp = prj.GetSkillProp( m_dwSkill );
	if( pSkillProp == NULL )		// �����ϸ� ������.
	{
		Error( "CSfxSkillMagHotAir::Process : ��ų(%d)�� ������Ƽ�� ����.", m_dwSkill );
		Delete();
		return;
	}
	AddSkillProp *pAddSkillProp = prj.GetAddSkillProp( pSkillProp->dwSubDefine, m_nMagicPower );	// MagicPower�� ���� 
	if( pAddSkillProp == NULL )
	{
		Error( "CSfxSkillMagHotAir::Process : ��ų(%d)�� �ֵ� ������Ƽ�� ����.", m_dwSkill );
		Delete();
		return;
	}
	
	BOOL bHitFrame = FALSE;
	int	nPainFrame = (int)((pAddSkillProp->dwPainTime / 1000.0f) * SEC1);
	if( (m_nFrame % nPainFrame) == 0 )	// nPainFrame ���� �ѹ��� ������ ��.
	{
		int	nMaxDmgCnt = (pAddSkillProp->dwSkillTime / pAddSkillProp->dwPainTime) + 1;		// ��� �������� �Գ�.
		// ˬ	2006/11/9	// ù��°�� ����, �� �� ���� ���� ó��
//		DamageToTarget( m_nDmgCnt++, 0, 0, nMaxDmgCnt );
		if( m_nDmgCnt++ == 0 )
			DamageToTarget( m_nDmgCnt, 0, 0, nMaxDmgCnt );
		if( m_nDmgCnt >= nMaxDmgCnt )
			m_idSfxHit = 0;
		PLAYSND( SND_PC_SKILLM_HOTAIR2, &GetPos() );
	}
	BOOL res2 = m_SfxObj2.Process();
	if( res2 )
		m_SfxObj2.m_nCurFrame = 0;
	int nFrameMax = (int)((pAddSkillProp->dwSkillTime / 1000.0f) * SEC1);	// �ִ� ���ӽð�(�����Ӵ���).
	if( m_nFrame >= nFrameMax )
	{
		Delete();
	}
	m_nFrame++;
}
void CSfxSkillMagHotAir::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->Render( pd3dDevice );
	m_pSfxObj->m_vScale = GetScale();
	if( !g_Option.m_bSFXRenderOff )
		m_SfxObj2.Render( pd3dDevice );
}



////////////////////////////////////////////////////////////////
void CSfxDuelParty::Process()
{
	CSfx::Process();
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}

	switch( m_nType )
	{
	case ENEMY_PVP_PARTY:
		{
			// process�� src������ �شܵ���� �������� ����.
			if( pMover->m_nDuel != 2 || ( pMover->m_nDuel == 2 && ( pMover->m_nDuelState == 300 || pMover->m_nDuelState == 0 ) ) )		
			{
				DeleteSfx( pMover );
			}
			break;
		}
	case ENEMY_WAR:
		{
			if( pMover->m_idWar == 0 )		// process�� src������ ������ �������� ����.
			{
				DeleteSfx( pMover );
			}
			break;
		}
	case ENEMY_PK:		
		{
			if( !( g_Neuz.m_NeuzEnemy.IsPKing( pMover->GetId() ) ) )
			{
				DeleteSfx( pMover );
			}
			break;
		}
	case ENEMY_PVP:
		{
			if( pMover->m_nDuel != 1 )
			{
				DeleteSfx( pMover );
			}
			break;
		}
	}
}

void CSfxDuelParty::DeleteSfx( CMover* pEnemy )
{
	pEnemy->m_dwFlag &= (~MVRF_WARMARK);
	Delete();
}

void	CSfxDuelParty::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
		return;
			
	if( pMover->HasBuff( BUFF_SKILL, SI_ACR_SUP_DARKILLUSION ) || pMover->IsMode( TRANSPARENT_MODE ) )
		return;

	CSfx::Render(pd3dDevice);
}



////////////////////////////////////////////////////////////////
CSfxSetItem::~CSfxSetItem() 
{
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsValidObj( pMover ) )
	{
		pMover->m_dwFlag &= (~MVRF_SETITEMSFX);
	}
}

void CSfxSetItem::Process()
{
	CSfx::Process();
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}

	int nAbilityOption	= pMover->GetSetItemClient();
	if( nAbilityOption < 3 )
	{
		pMover->m_dwFlag &= (~MVRF_SETITEMSFX);
		Delete();
		return;
	}
}

void	CSfxSetItem::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
		return;
	
	if( ((pMover->IsMode( TRANSPARENT_MODE ) ) == 0) )
		CSfx::Render(pd3dDevice);
}

////////////////////////////////////////////////////////////////
void CSfxCollect::Process()
{
	m_nFrame++;

	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;
	((CModelObject *)pMover->m_pModel)->GetForcePos( &vPos, 0, PARTS_RWEAPON, pMover->GetMatrixWorld() );
	
	SetPos( vPos );

	if( m_pSfxObj->Process() )
		m_pSfxObj->m_nCurFrame = 0;


	if( m_pSfxObj->m_nCurFrame >= 145 ) // ���� �ݺ�.
		m_pSfxObj->m_nCurFrame = 30; // ��ǥ���� ���������� ������Ű�� ���� �ٽ� 0���������� �������´�.
	
	// �÷��̾��� ���°� ä���� �������� 
	if( pMover->m_pActMover->GetActionState() != OBJSTA_COLLECT )
	{
		pMover->m_dwFlag &= (~MVRF_COLLECT);		// SFX����.
		Delete();		// sfx ����.
	}
}

void CSfxCollect::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}


////////////////////////////////////////////////////////////////
void CSfxMushmootCharge::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	if( pMover->m_dwFlag & MVRF_HITCONT )		// ������ �������� ��� �����ϸ� ������.
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;
	
	((CModelObject *)pMover->m_pModel)->GetEventPos( &vPos, m_nEventPos );			// ���� ��ǥ.
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );	// ����������� ��ȯ.
	
	SetPos( vPos );
	SetAngle( -pMover->GetAngle() );
	
	if( m_pSfxObj->Process() )
		m_pSfxObj->m_nCurFrame = 0;
}

void CSfxMushmootCharge::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}

////////////////////////////////////////////////////////////////
void CSfxClockWorksCharge::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	if( pMover->m_dwFlag & MVRF_HITCONT )		// ������ �������� ��� �����ϸ� ������.
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;

	((CModelObject *)pMover->m_pModel)->GetEventPos( &vPos, 2 );			// ���� ��ǥ.
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );	// ����������� ��ȯ.
	
	SetPos( vPos );
	SetAngle( -pMover->GetAngle() );
	
	if( m_pSfxObj->Process() )
		m_pSfxObj->m_nCurFrame = 0;
}

void CSfxClockWorksCharge::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}

////////////////////////////////////////////////////////////////
// Ŭ����ũ ���� ���ȿ��� �߻��Ҷ� ������ ����Ʈ.
void CSfxClockWorksCannon::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	
	pModel->GetEventPos( &vPos, 1 );			// �޼� ��ǥ.
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );	// ����������� ��ȯ.
	
	D3DXMATRIX mLArm = *(pModel->GetMatrixBone( 9 ));		// 9�� L ForeArm
	D3DXMatrixMultiply( &mLArm, pMover->GetMatrixWorldPtr(), &mLArm );
	D3DXQUATERNION qLArm;
	D3DXVECTOR3 vYPW;
	
	D3DXQuaternionRotationMatrix( &qLArm, &mLArm );			// ���� ��Ʈ������ ���ʹϾ����� �ٲ�.
	QuaternionRotationToYPW( qLArm, vYPW );
	vYPW.y += D3DXToRadian( -90.0f );
	m_pSfxObj->m_vRotate.x = D3DXToDegree(vYPW.x);
	m_pSfxObj->m_vRotate.y = D3DXToDegree(vYPW.y);
	m_pSfxObj->m_vRotate.z = D3DXToDegree(vYPW.z);

	SetPos( vPos );
	
	if( m_pSfxObj->Process() )
		Delete();
}

void CSfxClockWorksCannon::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render2( pd3dDevice, NULL );
}


///////////////////////////////////////////////////////////////////




// ������ Ice Missile
CSfxSkillMagIceMissile::CSfxSkillMagIceMissile()
{
	m_SfxObj2.SetSfx( XI_SKILL_MAG_WATER_ICEMISSILE01_01 );
	m_bHit=FALSE;
}
CSfxSkillMagIceMissile::~CSfxSkillMagIceMissile()
{
}
void CSfxSkillMagIceMissile::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest )
	{
		m_SfxObj2.m_vPos = pObjDest->GetPos() + D3DXVECTOR3(.0f,0.3f,.0f);
		m_SfxObj2.m_vScale = D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
	}
	if(m_bHit==FALSE) 
	{
		if( m_pSfxObj->Process() )
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;
		
	    // ������ü ���⺤�ͷ� �� ���� ������~~~
		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
		
		D3DXVECTOR3 vYPW;
		
		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x=m_SfxObj2.m_vRotate.x=D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z=m_SfxObj2.m_vRotate.z=D3DXToDegree(vYPW.z);

		if( D3DXVec3Length( &vDelta ) < .2f ) 
		{
			PLAYSND( SND_PC_SKILLM_FIREBOOMERANG2, &GetPos() );
			m_bHit=TRUE;
		}
	}
	else 
	{
		if(m_SfxObj2.m_nCurFrame==0)
			DamageToTarget();
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMagIceMissile::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}


// ������ Lightning Ball
CSfxSkillMagLightningBall::CSfxSkillMagLightningBall()
{
	m_SfxObj2.SetSfx( XI_SKILL_MAG_ELECTRICITY_LIGHTINGBALL01_01  );
	m_bHit=FALSE;
}
CSfxSkillMagLightningBall::~CSfxSkillMagLightningBall()
{
}
void CSfxSkillMagLightningBall::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest )
	{
		m_SfxObj2.m_vPos = pObjDest->GetPos() + D3DXVECTOR3(.0f,0.3f,.0f);
		m_SfxObj2.m_vScale = D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
	}
	if(m_bHit==FALSE) 
	{
		if( m_pSfxObj->Process() )
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;
		
		// ������ü ���⺤�ͷ� �� ���� ������~~~
		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
		
		D3DXVECTOR3 vYPW;
		
		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x=m_SfxObj2.m_vRotate.x=D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z=m_SfxObj2.m_vRotate.z=D3DXToDegree(vYPW.z);
		
		if( D3DXVec3Length( &vDelta ) < .2f ) 
		{
			PLAYSND( SND_PC_SKILLM_FIREBOOMERANG2, &GetPos() );
			m_bHit=TRUE;
		}
	}
	else 
	{
		if(m_SfxObj2.m_nCurFrame==0)
			DamageToTarget();
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMagLightningBall::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}

// ������ Spike Stone
CSfxSkillMagSpikeStone::CSfxSkillMagSpikeStone()
{
	m_SfxObj2.SetSfx( XI_SKILL_MAG_EARTH_SPIKESTONE01_01 );
	m_bHit=FALSE;
}
CSfxSkillMagSpikeStone::~CSfxSkillMagSpikeStone()
{
}
void CSfxSkillMagSpikeStone::Process()
{
	if(m_nFrame==0) 
	{
		CMover* pObjSrc = (CMover*)prj.GetCtrl( m_idSrc );
		if(pObjSrc) m_pSfxObj->m_vPos=pObjSrc->GetPos()+D3DXVECTOR3(.0f,1.0f,.0f);
	}
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( pObjDest )
	{
		m_SfxObj2.m_vPos = pObjDest->GetPos() + D3DXVECTOR3(.0f,0.3f,.0f);
		m_SfxObj2.m_vScale = D3DXVECTOR3( 2.0f, 2.0f, 2.0f );
	}
	if(m_bHit==FALSE) 
	{
		if( m_pSfxObj->Process() )
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize(&vDelta,&vDelta);
		m_pSfxObj->m_vPos+=vDelta*.2f;
		SetPos(m_pSfxObj->m_vPos);
		vDelta=m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;
		
		// ������ü ���⺤�ͷ� �� ���� ������~~~
		D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		D3DXVECTOR3 vDestNor  = m_SfxObj2.m_vPos-m_pSfxObj->m_vPos;
		D3DXVECTOR3 vAxis;
		D3DXQUATERNION   qRot;
		
		FLOAT       fTheta;
		D3DXVec3Normalize( &vDestNor, &vDestNor );
		D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
		fTheta = D3DXVec3Dot( &vDir, &vDestNor );
		D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
		
		D3DXVECTOR3 vYPW;
		
		QuaternionRotationToYPW( qRot, vYPW );
		m_pSfxObj->m_vRotate.x=m_SfxObj2.m_vRotate.x=D3DXToDegree(vYPW.x);
		m_pSfxObj->m_vRotate.y=m_SfxObj2.m_vRotate.y=D3DXToDegree(vYPW.y);
		m_pSfxObj->m_vRotate.z=m_SfxObj2.m_vRotate.z=D3DXToDegree(vYPW.z);
		
		if( D3DXVec3Length( &vDelta ) < .2f ) 
		{
			PLAYSND( SND_PC_SKILLM_FIREBOOMERANG2, &GetPos() );
			m_bHit=TRUE;
		}
	}
	else 
	{
		if(m_SfxObj2.m_nCurFrame==0)
			DamageToTarget();
		if(m_SfxObj2.Process()) {
			Delete();
		}
	}
	m_nFrame++;
}
void CSfxSkillMagSpikeStone::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	if(m_bHit==FALSE) 
	{
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}

////////////////////////////////////////////////////////////////
void CSfxRotate::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	pModel->GetEventPos( &vPos, 0 );	// ��帮 ��ǥ
	
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );

	SetPos( vPos );
	SetAngle( -pMover->GetAngle() );
	
	if( m_pSfxObj->Process() )
		m_pSfxObj->m_nCurFrame = 0;
	
	if( m_nSec != -1 )		// ���ѹݺ�(-1)�� �ƴϸ�
	{
		if( m_nFrame > ( m_nSec * 60 ) )
			Delete();
	}
}

void CSfxRotate::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( !IsVisible() || ( IsCull() && GetType() != 1 ))
		return;
	if( IsUpdateMatrix() )
		UpdateMatrix();
	
	LPMODELELEM lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	// Model�� ������ ������� ���� 
	if( lpModelElem == NULL )
		return;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )
		return;

	D3DXVECTOR3 vPos;
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	pModel->GetEventPos( &vPos, 0 );	// ��帮 ��ǥ
	
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );
	
	SetPos( vPos );
	
	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}


// ���ư��� �°� ������ �Ϲ����� ����Ʈ.
CSfxShoot::CSfxShoot()
{
	m_bHit = FALSE;
	m_dwSndHit = NULL_ID;
	m_bDir = FALSE;
	m_fHitScale = 1.0f;
	m_dwSfxHit = NULL_ID;
}
CSfxShoot::~CSfxShoot()
{
	m_bHit = FALSE;
	m_dwSndHit = NULL_ID;
	m_bDir = FALSE;
	m_fHitScale = 1.0f;
	m_dwSfxHit = NULL_ID;
}

// �¾����� ����Ʈ ���
void CSfxShoot::SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale ) 
{ 
}
void CSfxShoot::SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale ) 
{ 
}


// ���⼺ �ִ� sfx �ΰ�.
void CSfxShoot::SetDir( BOOL bDir )
{
	m_bDir = bDir;
}



void CSfxShoot::Process()
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );	// ����ź ���̱⶧���� Ÿ���� �׻� �о���� �Ѵ�.
	if( IsValidObj(pObjDest) )
	{
		m_vPosDest = pObjDest->GetPos() + D3DXVECTOR3( 0, 1.0f, 0 );
	}

	if( m_bHit == FALSE ) 
	{
		if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
		{
			m_pSfxObj->m_nCurFrame = 0;
		}
		D3DXVECTOR3 vDelta = m_vPosDest - m_pSfxObj->m_vPos;
		D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
		D3DXVECTOR3 vPos = GetPos();
		vPos += vDelta * 0.2f;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
		m_pSfxObj->m_vPos = vPos;
		SetPos( vPos );
		
		if( m_bDir )
		{
			// ������ü ���⺤�ͷ� �� ���� ������~~~
			D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			D3DXVECTOR3 vDestNor  = m_vPosDest - m_pSfxObj->m_vPos;
			D3DXVECTOR3 vAxis;
			D3DXQUATERNION   qRot;
			
			FLOAT       fTheta;
			D3DXVec3Normalize( &vDestNor, &vDestNor );
			D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
			fTheta = D3DXVec3Dot( &vDir, &vDestNor );
			D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
			
			D3DXVECTOR3 vYPW;
			
			QuaternionRotationToYPW( qRot, vYPW );
			m_pSfxObj->m_vRotate.x = /*m_SfxObj2.m_vRotate.x = */D3DXToDegree(vYPW.x);
			m_pSfxObj->m_vRotate.y = /*m_SfxObj2.m_vRotate.y = */D3DXToDegree(vYPW.y);
			m_pSfxObj->m_vRotate.z = /*m_SfxObj2.m_vRotate.z = */D3DXToDegree(vYPW.z);
		}
		
		vDelta = m_pSfxObj->m_vPos - m_vPosDest;		// ������ �Ÿ�
		if( D3DXVec3LengthSq( &vDelta ) < 0.2f * 0.2f )		// ���� �Ÿ����� �ٰ����� �浹.
		{
			if( m_dwSndHit != NULL_ID )
				PLAYSND( m_dwSndHit, &GetPos() );		// ���� �÷���.
			DamageToTarget();
			// ���� ������Ʈ ����.
			if( m_dwSfxHit != NULL_ID )
			{
				CSfx *pSfx = CreateSfx( D3DDEVICE, m_dwSfxHit, GetPos(), m_idSrc, m_vPosDest, m_idDest, 0 );
				if( pSfx )
				{
					pSfx->SetAngle( m_pSfxObj->m_vRotate.y );
				}
			}
			Delete();	// this�� ����.
		}
	}
	m_nFrame ++;
}
void CSfxShoot::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->Render2( pd3dDevice );
}


// ���ٴڿ� �پ ���ư��� ����Ʈ.
CSfxShootWave::CSfxShootWave()
{
	m_bHit = FALSE;
	m_dwSndHit = NULL_ID;
	m_bDir = FALSE;
	m_fHitScale = 1.0f;
	memset( m_vTail, 0, sizeof(m_vTail) );
	memset( m_nTailFrame, 0, sizeof(m_nTailFrame) );
}
CSfxShootWave::~CSfxShootWave()
{
	m_bHit = FALSE;
	m_dwSndHit = NULL_ID;
	m_bDir = FALSE;
	m_fHitScale = 1.0f;
	memset( m_vTail, 0, sizeof(m_vTail) );
	memset( m_nTailFrame, 0, sizeof(m_nTailFrame) );
}

// �¾����� ����Ʈ ���
void CSfxShootWave::SetHitSfx( LPCTSTR szFileName, OBJID idSrc, OBJID idDst, FLOAT fScale ) 
{ 
	m_SfxObj2.SetSfx( szFileName );		// �°� ������ ����Ʈ �ε�.

	m_fHitScale = fScale;
}
void CSfxShootWave::SetHitSfx( DWORD dwIndex, OBJID idSrc, OBJID idDst, FLOAT fScale ) 
{ 
	m_SfxObj2.SetSfx( dwIndex );		// �°� ������ ����Ʈ �ε�.
	
	m_fHitScale = fScale;
}

// ���⼺ �ִ� sfx �ΰ�.
void CSfxShootWave::SetDir( BOOL bDir )
{
	m_bDir = bDir;
}



void CSfxShootWave::Process()
{
	CMover* pObjDest = (CMover*)prj.GetCtrl( m_idDest );
	if( IsValidObj(pObjDest) )
	{
		m_SfxObj2.m_vPos = pObjDest->GetPos();
		m_SfxObj2.m_vScale = D3DXVECTOR3( m_fHitScale, m_fHitScale, m_fHitScale );			// ũ��� fHitScale��
	} else
	{
		// ���ư��� Ÿ���� ���������� ��������ǥ�� ���ư��� ������.
	}

	if( (m_nFrame & 3) == 0 )
	{
		for( int i = MAX_SHOOTWAVE_TAIL-1; i > 0; i -- )
		{
			m_vTail[i] = m_vTail[i-1];	// �ڷ� ��ĭ�� �δ�.
			m_nTailFrame[i] = m_nTailFrame[i-1];
			m_nTailFrame[i]++;
			if( m_nTailFrame[i] >= 56 )
			{
				m_vTail[i].x = m_vTail[i].y = m_vTail[i].z = 0;
				m_nTailFrame[i] = 0;
			}
		}
		m_vTail[0] = GetPos();
		m_nTailFrame[0] = 0;
	}

	if( m_pSfxObj->Process() )		// �ִϸ��̼��� ����.
	{
		m_pSfxObj->m_nCurFrame = 0;
	}
	
	if( m_bHit == FALSE ) 
	{
		D3DXVECTOR3 vDelta = m_SfxObj2.m_vPos - m_pSfxObj->m_vPos;
		D3DXVec3Normalize( &vDelta, &vDelta );		// Ÿ���� �������� ���ؼ�
		D3DXVECTOR3 vPos = GetPos(), vTemp;

		CWorld *pWorld = GetWorld();
		if( pWorld == NULL )	
		{
			Delete();
			return;
		}
		vPos += vDelta * 0.25f;			// �� ���ͷ� �ӵ��� �����ؼ� ��ǥ �̵�.
		vTemp = vPos;
		vTemp.y += 1.5f;
		FLOAT y = pWorld->GetUnderHeight( vTemp );	// �ٴ��� ���� ����,...
		vPos.y = y;
		vPos.y	= m_SfxObj2.m_vPos.y;		// ˬ
		m_pSfxObj->m_vPos = vPos;
		SetPos( vPos );
		
		if( m_bDir )
		{
			// ������ü ���⺤�ͷ� �� ���� ������~~~
			D3DXVECTOR3 vDir      = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			D3DXVECTOR3 vDestNor  = m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;
			D3DXVECTOR3 vAxis;
			D3DXQUATERNION   qRot;
			
			FLOAT       fTheta;
			D3DXVec3Normalize( &vDestNor, &vDestNor );
			D3DXVec3Cross( &vAxis, &vDir, &vDestNor );
			fTheta = D3DXVec3Dot( &vDir, &vDestNor );
			D3DXQuaternionRotationAxis( &qRot, &vAxis, acosf( fTheta ) );
			
			D3DXVECTOR3 vYPW;
			
			QuaternionRotationToYPW( qRot, vYPW );
			m_pSfxObj->m_vRotate.x = m_SfxObj2.m_vRotate.x = D3DXToDegree(vYPW.x);
			m_pSfxObj->m_vRotate.y = m_SfxObj2.m_vRotate.y = D3DXToDegree(vYPW.y);
			m_pSfxObj->m_vRotate.z = m_SfxObj2.m_vRotate.z = D3DXToDegree(vYPW.z);
		}
		
		vDelta = m_pSfxObj->m_vPos - m_SfxObj2.m_vPos;		// ������ �Ÿ�
		if( D3DXVec3LengthSq( &vDelta ) < 0.4f * 0.4f )		// ���� �Ÿ����� �ٰ����� �浹.
		{
			if( m_dwSndHit != NULL_ID )
				PLAYSND( m_dwSndHit, &GetPos() );		// ���� �÷���.
			m_bHit = TRUE;
			DamageToTarget();
		}
	}
	else 
	{
		if( m_SfxObj2.Process() )		// �ִϸ��̼� �� �������� ����.
			Delete();
	}
	m_nFrame ++;
}
void CSfxShootWave::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pSfxObj->m_vScale = GetScale();
	int nFrame = m_pSfxObj->m_nCurFrame;

	for( int i = 0; i < MAX_SHOOTWAVE_TAIL; i ++ )
	{
		if( m_vTail[i].x )
		{
			m_pSfxObj->m_vPos = m_vTail[i];
			m_pSfxObj->Render2( pd3dDevice );
		}
	}
	
	if( m_bHit == FALSE ) 
	{
		m_pSfxObj->m_vPos = GetPos();
		m_pSfxObj->Render2( pd3dDevice );
	}
	else 
	{
		if( !g_Option.m_bSFXRenderOff )
			m_SfxObj2.Render( pd3dDevice );
	}
}

////////////////////////////////////////////////////////////////
// ������ Ư�� ������ ��ũ�Ǵ� Sfx
void CSfxPartsLink::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;

	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	if( m_nPartsLink == 0 )		// ������
	{
		pModel->GetHandPos( &vPos, PARTS_RWEAPON, pMover->GetMatrixWorld() );		// ������ �ָ� ������ǥ ����.
	} else
	{
		pModel->GetHandPos( &vPos, PARTS_LWEAPON, pMover->GetMatrixWorld() );
	}
	
	SetPos( vPos );
	
	if( m_pSfxObj->Process() )
	{
		m_bEndFrame = TRUE;
	}
}

void CSfxPartsLink::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}



void CSfxPartsLinkBlade::Process() // ���̵� ��������Ʈ�� �и� ��ų������ �Ҹ�
{
	CSfxPartsLink::Process();

	CMover *pMover = prj.GetMover( m_idSrc );
	
	// �и���ų ���� ������ ����.
	if( pMover && (pMover->m_pActMover->GetState() & OBJSTA_ATK_ALL) != OBJSTA_ATK_MELEESKILL )
	{
		Delete();		// sfx ����.
	}
}

void CSfxPartsLinkJst::Process() // ������ ��������Ʈ�� ���������̸� �Ҹ�
{
	CSfxPartsLink::Process();
	
	if( m_bEndFrame )
	{
		Delete();		// sfx ����.
	}
}

////////////////////////////////////////////////////////////////
// ��ũ�ι� ������ Ư�� ������ ��ũ�Ǵ� Sfx
void CSfxAllowPartsLink::Process()
{
	m_nFrame++;
	
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos;
	
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
	if( m_nPartsLink == 0 )		// ������
	{
		pModel->GetHandPos( &vPos, PARTS_RWEAPON, pMover->GetMatrixWorld() );		// ������ �ָ� ������ǥ ����.
	} else
	{
		pModel->GetHandPos( &vPos, PARTS_LWEAPON, pMover->GetMatrixWorld() );
	}
	
	SetPos( vPos );
	
	if( m_pSfxObj->Process() )
		Delete();
}

void CSfxAllowPartsLink::Render( LPDIRECT3DDEVICE9 pd3dDevice )
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
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}

void CSfxReady::Process()
{
	CSfx::Process();

	CMover *pSrc = prj.GetMover( m_idSrc );		// �����ڿ��� �ߵ�.
	if( IsValidObj( pSrc ) )
	{
		if( pSrc->IsStateMode( STATE_BASEMOTION_MODE ) == FALSE )	// �������� �������� 
			Delete();		// ����.
	} else
		Delete();		// �����ڰ� ������� ����.
}


CSfxPartsLinkShoulder::CSfxPartsLinkShoulder():m_nOldAbilityOption(0), m_v3Offset(D3DXVECTOR3( 0.0f, 0.0f, 0.0f ))
{
#ifdef __SFX_OPT
	SetSkill(FALSE);
#endif
}
CSfxPartsLinkShoulder::~CSfxPartsLinkShoulder() 
{
	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsValidObj( pMover ) )
	{
		pMover->m_dwFlag &= (~MVRF_SETITEMSFX);
	}
}

void CSfxPartsLinkShoulder::Process()
{
	CSfx::Process();

	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	int nAbilityOption	= pMover->GetSetItemClient();
	if( nAbilityOption < 3 || nAbilityOption != m_nOldAbilityOption )
	{
		pMover->m_dwFlag &= (~MVRF_SETITEMSFX);
		Delete();
		return;
	}
}


void	CSfxPartsLinkShoulder::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	CMover *pMover = prj.GetMover( m_idSrc );

	if( pMover->IsActiveMover() && CWorld::m_nZoomLevel != 0 )
		return;

#ifdef __QUIZ
	if( pMover->GetWorld()->GetID() == WI_WORLD_QUIZ && pMover->IsPlayer() == TRUE && pMover->IsDisguise() == TRUE )
		return;
#endif // __QUIZ
	
	D3DXVECTOR3 vVel = D3DXVECTOR3( 0.0f, 0.04f, 0.0f );
	D3DXVECTOR3 vPos;
	D3DXMATRIX  mPartMatrix;
	
	CModelObject *pModel = (CModelObject *)pMover->m_pModel;

	if( m_nPartsLink == 0 )		// ���� ���
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(15)), &(pMover->GetMatrixWorld()) );
	} 
	else
	if( m_nPartsLink == 1 )		// �� ���
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(11)), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 2 )    // ���� �Ȳ�ġ
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(16)), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 3 )	   // �� �Ȳ�ġ
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(12)), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 4 )   // �����ʾ�� + �Ȳ�ġ ����
	{
		D3DXMATRIX  mMatrix1;
		D3DXMATRIX  mMatrix2;

		D3DXMatrixMultiply( &mMatrix1, (pModel->GetMatrixBone(15)), &(pMover->GetMatrixWorld()) );
		D3DXMatrixMultiply( &mMatrix2, (pModel->GetMatrixBone(16)), &(pMover->GetMatrixWorld()) );

		D3DXVECTOR3 vPos1 = D3DXVECTOR3( mMatrix1._41, mMatrix1._42, mMatrix1._43 );
		D3DXVECTOR3 vPos2 = D3DXVECTOR3( mMatrix2._41, mMatrix2._42, mMatrix2._43 );

		D3DXVECTOR3 vPos3;
		D3DXVec3Lerp( &vPos3, &vPos1, &vPos2, 0.6f );

		mPartMatrix = mMatrix1;
		mPartMatrix._41 = vPos3.x; 
		mPartMatrix._42 = vPos3.y; 
		mPartMatrix._43 = vPos3.z; 
	}
	else
	if( m_nPartsLink == 5 )   // ���ʾ�� + �Ȳ�ġ ����
	{
		D3DXMATRIX  mMatrix1;
		D3DXMATRIX  mMatrix2;
		
		D3DXMatrixMultiply( &mMatrix1, (pModel->GetMatrixBone(11)), &(pMover->GetMatrixWorld()) );
		D3DXMatrixMultiply( &mMatrix2, (pModel->GetMatrixBone(12)), &(pMover->GetMatrixWorld()) );
		
		D3DXVECTOR3 vPos1 = D3DXVECTOR3( mMatrix1._41, mMatrix1._42, mMatrix1._43 );
		D3DXVECTOR3 vPos2 = D3DXVECTOR3( mMatrix2._41, mMatrix2._42, mMatrix2._43 );
		
		D3DXVECTOR3 vPos3;
		D3DXVec3Lerp( &vPos3, &vPos1, &vPos2, 0.6f );
		
		mPartMatrix = mMatrix1;
		mPartMatrix._41 = vPos3.x; 
		mPartMatrix._42 = vPos3.y; 
		mPartMatrix._43 = vPos3.z; 
	}
	else
	if( m_nPartsLink == 6 )    // �� �߰�
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(2)), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 7 )	   // �� �Ʒ�
	{
		D3DXMatrixMultiply( &mPartMatrix, (pModel->GetMatrixBone(3)), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 8 )    // ������
	{
		D3DXMatrixMultiply( &mPartMatrix, pModel->GetMatrixBone(pModel->GetRHandIdx()), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 9 )	   // �޼�
	{
		D3DXMatrixMultiply( &mPartMatrix, pModel->GetMatrixBone(pModel->GetLHandIdx()), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 26 )	   // �޼�
	{
		D3DXMatrixMultiply( &mPartMatrix, pModel->GetMatrixBone(m_nPartsLink), &(pMover->GetMatrixWorld()) );
	}
	else
	if( m_nPartsLink == 29 )	   // �޼�
	{
		D3DXMatrixMultiply( &mPartMatrix, pModel->GetMatrixBone(m_nPartsLink), &(pMover->GetMatrixWorld()) );
	}
		
			
	vPos.x = mPartMatrix._41;
	vPos.y = mPartMatrix._42-0.05f;
	vPos.z = mPartMatrix._43;

	vPos -= m_v3Offset;
	
	SetPos( vPos );

	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
		return;
	if( ((pMover->IsMode( TRANSPARENT_MODE ) ) == 0) )
		CSfx::Render(pd3dDevice);
}

#if __VER >= 12 // __ITEMCREATEMON_S0602
CSfxCursor::CSfxCursor()
{
}
CSfxCursor::~CSfxCursor()
{
}
void CSfxCursor::Process()
{
	SetPos( g_Neuz.m_vCursorPos );		// ���콺 ��ǥ
	m_pSfxObj->Process();

	if( m_nSec != 0 )	// 0�� 1ȸ �÷����� ����.
	{
		// �ݺ� �ִϸ��̼�
		if( m_pSfxObj->Process() )		
			m_pSfxObj->m_nCurFrame = 0;
	}

	if( g_Neuz.m_pCreateMonItem == NULL )
		Delete();
}
#endif // __ITEMCREATEMON_S0602


CSfxLinkMover::CSfxLinkMover( )
{
}

CSfxLinkMover::~CSfxLinkMover( )
{
}

void CSfxLinkMover::Process( )
{
	m_nFrame++;
	
	CMover* pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )	// process�� src ������ ���������� �ڵ� ����
	{
		Delete();
		return;
	}
	
	D3DXVECTOR3 vPos = pMover->GetPos( );
	
	SetPos( vPos );
	
	if( m_pSfxObj->Process() )
	{
		m_pSfxObj->m_nCurFrame = 0;
	}
}

void CSfxLinkMover::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( !IsVisible() || ( IsCull() && GetType() != 1 ))
		return;
	if( IsUpdateMatrix() )
		UpdateMatrix();
	
	LPMODELELEM lpModelElem = prj.m_modelMng.GetModelElem( m_dwType, m_dwIndex );
	// Model�� ������ ������� ���� 
	if( lpModelElem == NULL )
		return;

	CMover *pMover = prj.GetMover( m_idSrc );
	if( IsInvalidObj( pMover ) )
		return;

	D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f );
//	CModelObject *pModel = (CModelObject *)pMover->m_pModel;
//	pModel->GetEventPos( &vPos, 0 );	// ��帮 ��ǥ
	
	D3DXVec3TransformCoord( &vPos, &vPos, pMover->GetMatrixWorldPtr() );
	
	SetPos( vPos );

	m_pSfxObj->m_vPos = GetPos();
	m_pSfxObj->m_vRotate.y = GetAngle();
	m_pSfxObj->m_vScale = GetScale();
	m_pSfxObj->m_matScale = m_matScale;
	m_pSfxObj->Render( pd3dDevice, NULL );
}