#include "stdafx.h"
#include "ActionMover.h"
#include "DefineObj.h"


BOOL CanCollisionFly( CMover* pMover )
{
#ifdef __CLIENT
	if( CObj::m_bCollision && (pMover->IsActiveMover() || pMover->IsEmptyDest()) )
		return TRUE;
#else
	if( pMover->IsNPC() || pMover->IsEmptyDest() )
		return TRUE;
#endif
	return FALSE;
}

//
//
//
BOOL CActionMover::ProcessCollisionFly( D3DXVECTOR3 *pPos )
{
	if( m_fAccPower == 0.0f && m_vDeltaAccu == D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )	// ��������?
		return FALSE;

	CMover*		pMover = m_pMover;
	D3DXVECTOR3	vStart = *pPos;
	D3DXVECTOR3 vDelta = m_vDeltaAccu;
	CWorld*		pWorld = GetWorld();

	pMover->SetIAObjLink( NULL );
	vStart.y += 0.5f;				// vStart ������ ��ǥ + 0.5 = ���ڷ� ����
	
	BOOL fResult = FALSE;
	if( CanCollisionFly( pMover) )
	{
		D3DXVECTOR3 vDir = m_vDeltaAccu;
	#ifdef __WORLDSERVER
		vDir *= 0.25f;
	#endif
		float fLen = D3DXVec3Length( &vDir );		// ������� ������ ���̸� �ϴ� ����.
		if( fLen > 0.001f )
		{
			FLOAT fR = 1.0f;
			if( fLen < 0.65f )							// �� ���̰� 0.65f ���ٵ� ª����
			{
				fR = 0.65f / fLen;						// ��質 ������ ����ؼ�
				vDir *= fR;								// ���͸� �׸�ŭ �ÿ���.
			}
			fResult	= pWorld->ProcessCollisionReflection( &vDelta, vStart, vDir, 0 );
			if( fResult )	
			{
				vDelta /= fR;
				vDelta *= 0.7f;
				m_vDelta = vDelta;
			}
		}
	}

	// 1. y�� ����
	pPos->y += vDelta.y;

	// 2. x�� ����
	pPos->x += vDelta.x;
	pWorld->ClipX( pPos->x );
	int nAttr = pWorld->GetHeightAttribute( pPos->x, pPos->z );		// �̵��� ��ġ�� �Ӽ� ����.
	switch( nAttr )
	{
	case HATTR_NOMOVE:	// �̵� ����.
		pPos->x -= vDelta.x;		// �̵��Ϸ��� ���� �̵� ���� �����̸� ��ǥ�� �ٽ� ��
		break;
	}

	// 3. z�� ����
	pPos->z += vDelta.z;
	pWorld->ClipZ( pPos->z );
	if( pWorld->m_bFly )
		nAttr = pWorld->GetHeightAttribute( pPos->x, pPos->z );		
	else
		nAttr = HATTR_NOFLY;

	switch( nAttr )
	{
	case HATTR_NOMOVE:	// �̵� ����.
		pPos->z -= vDelta.z;		// �̵��Ϸ��� ���� �̵� ���� �����̸� ��ǥ�� �ٽ� ��
		break;
	case HATTR_DIE:		// �̿����� ���� ����.
		pMover->DoDie( NULL );
		break;
	case HATTR_NOFLY:	// ���� ���� - �ڵ����� ������ Ǯ��.
		pPos->x -= vDelta.x;
		pPos->z -= vDelta.z;		// �̵��Ϸ��� ���� �̵� ���� �����̸� ��ǥ�� �ٽ� ��
		pPos->y -= vDelta.y;
		pMover->UnequipRide();
		break;
	}

	m_fCurrentHeight = pWorld->GetFullHeight( D3DXVECTOR3(pPos->x, pPos->y + 1.0f, pPos->z) );

	if( pPos->y > pWorld->m_fMaxHeight )
		pPos->y = pWorld->m_fMaxHeight;
	else
	{
		LPWATERHEIGHT pWaterHeight = pWorld->GetWaterHeight( *pPos );
		if( pWaterHeight && pWaterHeight->byWaterTexture == WTYPE_CLOUD && m_fCurrentHeight == 0.0f )	
		{
			if( pPos->y < pWorld->m_fMinHeight )		
				pPos->y = pWorld->m_fMinHeight;
		}
		else
		{
			if( pPos->y < m_fCurrentHeight )		
			{
				pPos->y = m_fCurrentHeight;
				fResult = TRUE;
			}
		}
	}

	return fResult;
}


//
//
//
BOOL CActionMover::ProcessCollisionGround( D3DXVECTOR3 *pPos )
{
	CMover*			pMover = m_pMover;
	CModelObject	*pModel = (CModelObject *)pMover->m_pModel;
	D3DXVECTOR3		vDelta = m_vDeltaAccu;
	BOOL			fResult = FALSE;
	CWorld*			pWorld = GetWorld();

	if( m_bMove )	
	{
		D3DXVECTOR3	vKneePos, vDeltaXZ ;
		
		vKneePos = *pPos;	
		vKneePos.y += 0.4f;		// ��������
		vDeltaXZ = vDelta;	
		vDeltaXZ.y = 0;			// �̵����Ϳ��� �����̵� ���и� ����


		//gmpbigsun : collision test-------------------------------------------------------------------------------
		float COLL_TEST_DIR = 6.5f;

#ifdef __BS_ADJUST_COLLISION

		if( pMover == g_pPlayer )		//���ΰ��ϰ�� 
		{
			COLL_TEST_DIR = 30.0f;

			DWORD dwMoveState = GetMoveState( );
			if( OBJSTA_FMOVE == dwMoveState )
			{
				if( !IsStateFlag( OBJSTAF_WALK ) ) 
					COLL_TEST_DIR = 8.0f;

			}

		}
#endif

		vDeltaXZ *= COLL_TEST_DIR;
	#ifdef __X15
		vDeltaXZ /= 4.0f;		// ������ 15���������� ������ vDeltaXZ�� 4��� Ŀ����. �׷��� �ٿ���� ��.
	#endif
	
		// ������ġ�� �̵����͸� �Ѱ��ָ� 
		// ������ �浹���θ� �˻��ؼ� �ٲ� �̵����͸� ����Ѵ�.
		BOOL bColl	= TRUE;
		if( pMover->IsNPC() && (pMover->GetIndex() >= MI_PET_LAWOLF && pMover->GetIndex() <= MI_PET_LASTINDEX) )
		{
			bColl = FALSE;		// ���ű��� ������� ���� ���� ���� ������Ʈ�� �浹�˻� ����.
		}

#if __VER >= 9	//__AI_0509
		if( pMover->IsNPC() && ( pMover->GetSpeedFactor() > 1.9F || pMover->m_dwAIInterface == AII_PET ) )	// ��ȯ���̸�
		{
			// �˹鰰�� ���� ������ ��Ȳ���� �浹üũ ����
			if( pMover->m_pActMover->m_vDeltaE.x == 0 && pMover->m_pActMover->m_vDeltaE.y == 0 && pMover->m_pActMover->m_vDeltaE.z == 0 )
				bColl = FALSE;
		}
#endif	// __AI_0509
#ifdef __CLIENT
#if __VER >= 9	// __PET_0410
		if( pMover->GetId() == NULL_ID )	// Ŭ���̾�Ʈ������ ������ ��ü��,
			bColl	= FALSE;
#endif	// __PET_0410
#endif	// __CLIENT

		bColl = TRUE;

		if( CObj::m_bCollision && bColl ) 
		{
			float fLen = D3DXVec3Length( &vDeltaXZ );		// ������� ������ ���̸� �ϴ� ����.
			if( fLen != 0.0f )
			{	/*
				FLOAT fR = 1.0f;
				if( fLen < 0.65f )							// �� ���̰� 0.65f ���ٵ� ª����
				{
					fR = 0.65f / fLen;						// ��質 ������ ����ؼ�
					vDeltaXZ *= fR;							// ���͸� �׸�ŭ �ÿ���.
				}*/
#ifdef __LAYER_1015
				fResult	= pWorld->ProcessCollision( &vDeltaXZ, vKneePos, vDeltaXZ, 0, pMover->GetLayer() );
#else	// __LAYER_1015
				fResult	= pWorld->ProcessCollision( &vDeltaXZ, vKneePos, vDeltaXZ, 0 );
#endif	// __LAYER_1015
				//vDeltaXZ /= fR;								// �浹����� �����Ŀ� �ٽ� ���� ���� ���󺹱�.
			}
		}

	#ifdef __X15
		vDeltaXZ *= 4.0f;	
	#endif
		vDeltaXZ /= COLL_TEST_DIR;

#ifdef __BS_CONSOLE
		if( pMover == g_pPlayer )
			vDeltaXZ *= gConsole()->_fSpeedMul;
#endif

	#if __VER >= 11 // __FIX_COLLISION
		if(fResult && (D3DXVec3Length(&vDeltaXZ) > 0.15f))
		{
			vDeltaXZ.y = m_vDeltaAccu.y = m_vDelta.y = 0;
			m_vDelta.x = vDeltaXZ.x;
			m_vDelta.z = vDeltaXZ.z;
		}
	#endif
		BOOL bCollision = FALSE;

		// x�� ���ϱ�
		pPos->x += vDeltaXZ.x;
		pWorld->ClipX( pPos->x );
		int nAttr = pWorld->GetHeightAttribute( pPos->x, pPos->z );		// �̵��� ��ġ�� �Ӽ� ����.
		switch( nAttr )
		{
		case HATTR_NOWALK:	// �ȱ� ����
		case HATTR_NOMOVE:	// �̵� ����.
			if( pMover->GetIAObjLink() == NULL )	// �踦 Ÿ�� �������� ���󿡼��� �Ӽ��� ���õȴ�.
			{
				pPos->x -= vDeltaXZ.x;		// �̵��Ϸ��� ���� �̵� ���� �����̸� ��ǥ�� �ٽ� ��
				bCollision = TRUE;
			}
			break;
		}

		// z�� ���ϱ�
		pPos->z += vDeltaXZ.z;
		pWorld->ClipZ( pPos->z );	
		nAttr = pWorld->GetHeightAttribute( pPos->x, pPos->z );		// �̵��� ��ġ�� �Ӽ� ����.
		switch( nAttr )	
		{
		case HATTR_NOWALK:	// �ȱ� ����
		case HATTR_NOMOVE:	// �̵� ����.
			if( pMover->GetIAObjLink() == NULL )
			{
				pPos->z -= vDeltaXZ.z;
				bCollision = TRUE;
			}
			break;
		case HATTR_DIE:		// �̿����� ���� ����. - die�� x, z�� �� �������� ���Ŀ� �˻��ؾ� �Ѵ�.
			pMover->DoDie( NULL );
			break;
		}
		
	#ifdef __WORLDSERVER
		if( bCollision )
			pMover->PostAIMsg( AIMSG_COLLISION );
	#endif
	} // if( m_bMove )

	// ��ǥ �������� ����(��ó��)
	if( GetDmgState() == OBJSTA_DISAPPEAR )		// �׾� ������� �߿� �����浹�˻� ����.
		return fResult;

	if( vDelta.y < 0.0f )		// ��ǥ�� ��������
	{
		if( m_bGround == 1 )	// �ٴڿ� �׳� ���ִ� ����(m_bGround == 1)�� �߰������� �浹�˻� ���ʿ� ����.
			return fResult;

		// ĳ���� �Ӹ����� �Ʒ��� ������ ������ �ɸ��°� ������ ������ ������ �����̵� ����� ����.
		// �ɸ��°� ���ٸ� ���߿� ���ִ°��̹Ƿ� pPos->y += delta.y�� ���ְ� h������ ���.
		// �� �����̵� ����Ҷ��� ���� �������̻����� ������ �������� ����. �ȱ׷��� ��� ������ �� �̲����� ���ö󰣴�.
		// �����̵带 ���Ҷ��� ������ �ٴڿ� ��������� �����ش�.

		D3DXVECTOR3 vSlide( 0.0f, 0.0f, 0.0f );		// slide����
		CObj *pObjColl = NULL;
		FLOAT h = pWorld->ProcessUnderCollision( &vSlide, &pObjColl, D3DXVECTOR3( pPos->x, pPos->y + 1.0f, pPos->z ) );
		m_fCurrentHeight = h;

		pPos->y += vDelta.y;
		
		BOOL bLanding = FALSE;
		if( GetJumpState() == OBJSTA_SJUMP3 )	// �ϰ����� ���¿�����
		{
 			if( pPos->y < h )	// �ٴڿ� ������
				bLanding = TRUE;
		} 
		else	
		{
			// �׳� ��ǥ�� �������� �ִ�����( ��Ż���� �������ٰų�.  �ɾ�ٰ� �����ٰų�)
			if( pPos->y - 0.25f > h )		// �߹ٴ� �Ʒ� xx������ �� ������ �˻��ߴµ��� �ٴں��� ���ٸ� 
			{
				if( vDelta.y < -0.1f )		// �������� �ӵ��� 0.x���� Ŭ���� �ϰ�����.  ��¦ �������°� �� ��ǥ�� ������.
				{
					#if __VER >= 11 // __SYS_COLLECTING
						if( (GetState() & OBJSTA_COLLECT)== 0 )
					#endif

#if __VER >= 15 // __BS_FIXED_KNOCKBACK
						if( pMover->IsPlayer() && !( GetDmgState() & OBJSTA_DMG_FLY_ALL ) )			//���ʹ� �ϰ� �ִ� ���� 
#endif
					SetJumpState( OBJSTA_SJUMP3 );		// �ϰ� ���·� ��ȯ

					pMover->OnActDrop();				// �̺�Ʈ ȣ��
				}
			} 
			else				// �߾Ʒ� 0.25�������� �ٴ��� �ִٸ� �׳� �߹ٴ� �ٿ���
				bLanding = TRUE;
		} 
		
		if( bLanding )			// ����? 
		{
#ifdef __JEFF_11_6
			BOOL bReturn	= pMover->IsNPC() && pMover->GetSpeedFactor() > 1.9F;
#endif	// __JEFF_11_6
			if( vSlide.y
#ifdef __JEFF_11_6
				&& !bReturn
#endif	// __JEFF_11_6
				)
			{
				*pPos = vSlide;
				m_bGround = 2;	// ���� ��� �ߴµ� �̲��� ���� �ִ�.
			#ifdef __WORLDSERVER
				pMover->PostAIMsg( AIMSG_COLLISION );
			#endif
			} 
			else
			{
				pPos->y = h;
				m_bGround = 1;	// �̲��� ���� �ʰ� �ִ�.
			}

			if( pObjColl )
			{
				if( pObjColl->GetType()	== OT_SHIP )	// ������ ������Ʈ�� SHIP�̸�
					pMover->OnActIALanding( pObjColl, *pPos );
				else
					pMover->SetIAObjLink( NULL );
			} 
			else
				pMover->SetIAObjLink( NULL );	// ���� �ߴµ� ������Ʈ�� �ƴϸ� IA���� �����ɷ� ����.
			
			m_vDelta.y = 0;
		}
	} 
	else if( vDelta.y > 0 )			// ��ǥ�� �ö���
	{
#if __VER >= 15 // __BOUND_BOX_COLLISION
		FLOAT h = 65535.0f;			// pPos��ġ�� õ�� ����
		if( pMover->IsPlayer() )
		{
			h = pWorld->GetOverHeightForPlayer( D3DXVECTOR3( pPos->x, pPos->y + 1.5f, pPos->z ), pMover );
		}
		else
		{
			h = pWorld->GetOverHeight( D3DXVECTOR3( pPos->x, pPos->y + 1.5f, pPos->z ), pMover );
		}
#else // __BOUND_BOX_COLLISION
		// pPos��ġ�� õ�� ����
		FLOAT h	= pWorld->GetOverHeight( D3DXVECTOR3( pPos->x, pPos->y + 1.5f, pPos->z ) );
#endif // __BOUND_BOX_COLLISION
			
		pPos->y += vDelta.y;		// ����� ������
		if( pPos->y + 1.5f > h )	// �Ӹ� �κ��� õ�忡 ������� 
		{
			pPos->y = h - 1.5f ;	// y��ǥ ����
			m_vDelta.y = -0.01f;	// �ö󰡴��� ����
			fResult = TRUE;			// �浹 
		}
	}
	
	return fResult;
}

