#include "stdafx.h"
#include "defineObj.h"
#include "guild.h"
#include "authorization.h"
#include "AppDefine.h"
#include "sfx.h"
#include "party.h"


extern	CPartyMng	g_PartyMng;

#include "eveschool.h"
#include "defineskill.h"
#include "commonctrl.h"

extern CGuildCombat g_GuildCombatMng;


#ifndef __WORLDSERVER

//////////////////////////////////////////////////////////////////////////////////////////////
// CMover
//////////////////////////////////////////////////////////////////////////////////////////////
void CMover::SetRenderPartsEffect( int nParts )
{
	CModelObject* pModel = (CModelObject*)m_pModel;
	CItemElem *pItemElem = NULL;
	ItemProp *pItemProp = NULL;

	if( IsActiveMover() )
	{
		pItemElem = GetEquipItem( nParts );	// ������ ���⿡
		if( pItemElem )
			pItemProp = pItemElem->GetProp();
	} 
	else
	{
		DWORD dwItemID	= m_aEquipInfo[nParts].dwId;
		if( dwItemID != NULL_ID )
			pItemProp = prj.GetItemProp( dwItemID );
	}
	
	if( pItemProp )
	{
		if( pItemProp->nReflect > 0 )	// ���÷�Ʈ�� �ɷ�������
		{
			pModel->SetEffect( nParts, XE_REFLECT );	// ���÷�Ʈ �ɼ����� ����.

			if( nParts == PARTS_RWEAPON && pItemProp->dwItemKind3 == IK3_YOYO )
				pModel->SetEffect( PARTS_LWEAPON, XE_REFLECT );	// ���÷�Ʈ �ɼ����� ����.
		}
		
		BOOL bExec = FALSE;
		
		switch( pItemProp->dwSfxElemental )
		{
		case ELEMENTAL_FIRE:	pModel->SetEffect( nParts, XE_ITEM_FIRE | (5 << 24) );	bExec = TRUE; break; // �� Ÿ�� �ɼ�.
		case ELEMENTAL_ELEC:	pModel->SetEffect( nParts, XE_ITEM_ELEC | (0 << 24) );	bExec = TRUE; break; // ���� �ɼ�.
		case ELEMENTAL_WATER:	pModel->SetEffect( nParts, XE_ITEM_WATER | (5 << 24));	bExec = TRUE; break; // �� �ɼ�
		case ELEMENTAL_WIND:	pModel->SetEffect( nParts, XE_ITEM_WIND  | (5 << 24));	bExec = TRUE; break; // �ٶ� �ɼ�
		case ELEMENTAL_EARTH:	pModel->SetEffect( nParts, XE_ITEM_EARTH | (5 << 24));	bExec = TRUE; break;// �� �ɼ�
			break;
		}
		
		if( pItemProp->dwSfxElemental == -1 )
		{			
			int nAttrLevel = 0;
			int	nAttr = 0;
			int nLevel = 0;
			if( pItemElem )
			{
				nAttrLevel = pItemElem->m_nResistAbilityOption;
				nAttr = pItemElem->m_bItemResist;
				nLevel = pItemElem->GetAbilityOption();
			} else
			{
				nAttr	   = (m_aEquipInfo[nParts].nOption & 0x00FF0000) >> 16;
				nAttrLevel = (m_aEquipInfo[nParts].nOption & 0xFF000000) >> 24;
				nLevel = m_aEquipInfo[nParts].nOption & 0xFF;
			}
			
			int nEffLevel = 0;
#if __VER >= 13 // __EXT_ENCHANT
			if( nAttrLevel > 10 )
				nAttrLevel = 10;
#endif // __EXT_ENCHANT
			if( nAttr && (nAttrLevel > 10 || nAttrLevel < 0) )	// �Ӽ��� �ִµ� �Ӽ��������� �̻��Ҷ�.
			{
				LPCTSTR szErr = Error( "m_nResistAbilityOption=%d %s", nAttrLevel, GetName() );
				//ADDERRORMSG( szErr );
				nAttrLevel = 10;
			}

			DWORD dwItemFire = XE_ITEM_FIRE;
			DWORD dwItemElec = XE_ITEM_ELEC;
			DWORD dwItemWater = XE_ITEM_WATER;
			DWORD dwItemWind = XE_ITEM_WIND;
			DWORD dwItemEarth = XE_ITEM_EARTH;
			DWORD dwItemNone = XE_ITEM_GEN;

#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT
#ifndef __CSC_ENCHANT_EFFECT_2
			if( prj.m_nEnchantLimitLevel[2] >= nLevel )
				return;
#endif //__CSC_ENCHANT_EFFECT_2
			{
				nEffLevel = nLevel;

				switch( pItemProp->dwReferStat1 )
				{
					case WEAPON_GENERAL:
					case WEAPON_UNIQUE:
						{
							if( prj.m_nEnchantLimitLevel[0] > nAttrLevel )
								return;
						}
						break;
					case WEAPON_ULTIMATE:
						{
							if( prj.m_nEnchantLimitLevel[1] > nAttrLevel )
								return;
							
							dwItemFire = XE_ITEM_FIRE_AL;
							dwItemElec = XE_ITEM_ELEC_AL;
							dwItemWater = XE_ITEM_WATER_AL;
							dwItemWind = XE_ITEM_WIND_AL;
							dwItemEarth = XE_ITEM_EARTH_AL;
							dwItemNone = XE_ITEM_GEN_AL;
						}
						break;
				}
			}
#else //__Y_ADV_ENCHANT_EFFECT
			if( nAttrLevel == 10 )		nEffLevel = 5;
			else if( nAttrLevel == 9 )	nEffLevel = 4;
			else if( nAttrLevel == 8 )	nEffLevel = 3;
			else if( nAttrLevel == 7 )	nEffLevel = 2;
			else if( nAttrLevel >= 4 )	nEffLevel = 1;	// 4 ~ 6
			else 
				nEffLevel = 0;
#endif //__Y_ADV_ENCHANT_EFFECT
			if( nEffLevel >= 0 )
#ifdef __CSC_ENCHANT_EFFECT_2
#else //__CSC_ENCHANT_EFFECT_2
			if( nEffLevel >= 1 )	// �Ӽ����÷��� 1,2,3�� ����Ʈ ����.
#endif //__CSC_ENCHANT_EFFECT_2
			{
				nEffLevel <<= 24;
				
				switch( nAttr )
				{
			#if __VER >= 9 // __Y_ADV_ENCHANT_EFFECT
				case 0:						pModel->SetEffect( nParts, dwItemNone  | nEffLevel );	break;	
			#endif //__Y_ADV_ENCHANT_EFFECT
				case SAI79::FIRE:			pModel->SetEffect( nParts, dwItemFire | nEffLevel );	break;	
				case SAI79::ELECTRICITY:	pModel->SetEffect( nParts, dwItemElec | nEffLevel );	break;
				case SAI79::WATER:			pModel->SetEffect( nParts, dwItemWater | nEffLevel );	break;
				case SAI79::WIND:			pModel->SetEffect( nParts, dwItemWind | nEffLevel );	break;
				case SAI79::EARTH:			pModel->SetEffect( nParts, dwItemEarth | nEffLevel );	break;
				}
			}
		}
	}
}
void CMover::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	MoverProp *pMoverProp = GetProp();
	if( pMoverProp == NULL )
	{
		LPCTSTR szErr = Error( "CMover::Render : ������Ƽ ������ %s", GetName() );
		//ADDERRORMSG( szErr );
	}
#ifdef _DEBUG
	if( g_pPlayer == this )
	{
		int a = 0;
	} else
	{
		int a = 0;
	}

#endif

	if( !IsVisible() || IsCull() )
		return;

#ifdef __CLIENT		
	if( IsActiveMover() && m_pActMover->IsFly() )
	{
//		if( g_Neuz.m_camera.m_fZoom < 1.0f )		// ������ Ȯ������� �Ⱥ��̰� ����.
//			return;
	}

	D3DXVECTOR3 vPos = GetPos();

#if 0	
	if( !m_pActMover->IsFly() )
	{
		if( GetWorld() )
		{
			D3DXVECTOR3 vec3Tri[3];
			GetWorld()->GetLandTri( vPos.x, vPos.z, vec3Tri );
			
			D3DXVECTOR3 vVector1 = vec3Tri[2] - vec3Tri[0];
			D3DXVECTOR3 vVector2 = vec3Tri[1] - vec3Tri[0];
			D3DXVECTOR3 vNormal;
			D3DXVec3Cross( &vNormal, &vVector1, &vVector2);	
			D3DXVec3Normalize( &vNormal, &vNormal );
			
			D3DXVECTOR3 v3Up = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
			D3DXVECTOR3 v3Cross;
			FLOAT fDot;
			FLOAT fTheta;
			D3DXVec3Cross( &v3Cross, &v3Up, &vNormal );
			fDot = D3DXVec3Dot( &v3Up, &vNormal );
			fTheta = acos( fDot );
			
			D3DXQUATERNION qDirMap;
			D3DXQuaternionRotationAxis( &qDirMap, &v3Cross, fTheta );
			
			D3DXVECTOR3 vYPW;
			QuaternionRotationToYPW( qDirMap, vYPW );
			
			if( GetAngle() > 180 )
			{
				//					vYPW.x = -vYPW.x;
			}
			
			FLOAT ff = D3DXToDegree(vYPW.y);
			
			FLOAT fa = (360.0/65536) * ((int)(ff*(65536/360.0)) & 65535);
			
			//				SetAngle(fa);				
			//				SetAngleZ(D3DXToDegree(-vYPW.z));				
			//				SetAngleX(D3DXToDegree(-vYPW.x));
		}
	}
#endif //0

#endif

	pd3dDevice->SetMaterial( g_TextureMng.GetMaterial( pd3dDevice, 0 ) );

	D3DXMATRIX  mWorld;

/*	
	mWorld = m_matWorld;		// ���ĺ��ʹ� m_matWorld�� �������� matWorld�� ���� 

	if( IsPlayer() )
		mWorld._42 += 0.07f;		// ������ �� ���̸�ŭ �÷���.
	// ��ǥ �̵� 
*/
 #ifdef __CLIENT
	if( IsPlayer() )
	{
		D3DXVECTOR3 vPosHeel = vPos + D3DXVECTOR3( 0.0f, 0.07f, 0.0f ) ;
		D3DXMatrixTranslation( &m_matTrans, vPosHeel.x, vPosHeel.y, vPosHeel.z );
	}
//	if( IsUpdateMatrix() )
	{
		UpdateMatrix();		// Ŭ�󿡼� ��Ʈ���� ������ �������� �Ѵ�. �������� Process���� �Ѵ�.
	}
 #endif
	mWorld = m_matWorld;		// ���ĺ��ʹ� m_matWorld�� �������� matWorld�� ���� 

#ifdef __CLIENT
	switch( GetIndex() )
	{
	case MI_SYLIACA1:
	case MI_SYLIACA2:
	case MI_SYLIACA3:
	case MI_SYLIACA4:
		if( m_pModel )
			mWorld._42	= (float)( mWorld._42 + (fabs(m_pModel->m_vMax.y - m_pModel->m_vMin.y) / 2.0) );
		break;
	}
#endif
	
	// Ƽ�� ������ ����...
	if( GetIndex() == MI_MASA_TINA )
	{
		ResetScale();
		D3DXMatrixScaling(&m_matScale, GetScale().x, GetScale().y, GetScale().z );
	}

	// ������ ��鸮�� ó��.
	if( IsNPC() && m_nDmgCnt > 0 && pMoverProp->dwClass != RANK_SUPER )		// �������� ��鸮�� ����.
	{
		{
			D3DXMATRIX matDmg;
			int cx = -25 + (xRandom(50));
			int cz = -25 + (xRandom(50));
			D3DXMatrixTranslation( &matDmg, (float)cx / 100.0f, 0, (float)cz / 100.0f );
			D3DXMatrixMultiply( &mWorld, &mWorld, &matDmg );
		}
	}

	D3DXVECTOR3 vPos2 = GetPos() - GetWorld()->m_pCamera->m_vPos;
	int nLength = (int)( D3DXVec3Length( &vPos2 ) );
	if( nLength > CWorld::m_fFarPlane ) 
		nLength = (int)( CWorld::m_fFarPlane );
	if( nLength > CWorld::m_fFarPlane - ( CWorld::m_fFarPlane / 4 ) )
		return;

	CItemElem *pItemElem = NULL, pItemElemCloak;
	ItemProp *pItemProp = NULL;

	CModelObject* pModel = (CModelObject*)m_pModel;

	OverCoatItemRenderCheck(pModel);	

	if( IsActiveMover() )
	{
		pItemElem = GetEquipItem( PARTS_RWEAPON );	// ������ ���⿡
		if( pItemElem )
			pItemProp = pItemElem->GetProp();
	} 
	else
	{
		DWORD dwItemID = m_aEquipInfo[PARTS_RWEAPON].dwId;
		if( dwItemID != NULL_ID )
			pItemProp = prj.GetItemProp( dwItemID );
	}

	// ����ִ� ������ ����Ʈ ����.
	SetRenderPartsEffect( PARTS_RWEAPON );
	SetRenderPartsEffect( PARTS_LWEAPON );

	if( IsActiveMover() )
	{
		pItemElem = GetEquipItem( PARTS_SHIELD );	// ���п�
		if( pItemElem )
			pItemProp = pItemElem->GetProp();
	} else
	{
		DWORD dwItemID	= m_aEquipInfo[PARTS_SHIELD].dwId;	// activemover �� �ƴѳ��� ������������� ������.
		if( dwItemID != NULL_ID )
			pItemProp = prj.GetItemProp( dwItemID );
	}
	if( pItemProp )
	{
		if( pItemProp->nReflect > 0 )	// ���÷�Ʈ�� �ɷ�������
			pModel->SetEffect( PARTS_SHIELD, XE_REFLECT );	// ���÷�Ʈ �ɼ����� ����.
	}
	// ���� �ؽ��� ������ ����.
	pModel->SetExtTexture( m_pCloakTexture );

	if( pModel )
	{
		O3D_ELEMENT *pElem = pModel->GetParts( PARTS_HAIR );

		if( pElem )
		{
			if( pElem->m_pObject3D )
			{
				pElem->m_pObject3D->m_fAmbient[0] = m_fHairColorR;
				pElem->m_pObject3D->m_fAmbient[1] = m_fHairColorG;
				pElem->m_pObject3D->m_fAmbient[2] = m_fHairColorB;
			}
		}
	}
	
	if( GetActiveObj() == this ) 
		m_pModel->SetGroup( 0 );
	else
	{	
		if( m_pModel->m_nNoEffect == 0 )
		{
			float fDist = 50.0f;
			switch( g_Option.m_nObjectDetail )
			{
			case 0 :	fDist = 20.0f;	break;
			case 1 :	fDist = 10.0f;	break;
			case 2 :	fDist = 5.0f;	break;
			}
			int nLevel = (int)( m_fDistCamera / fDist );
			if( nLevel >= 2 )	nLevel = 2;
			if( nLevel < 0 )	
			{
				Error( "CMover::Render : %s, lod lv=%d %f", m_szName, nLevel, m_fDistCamera );
				nLevel = 0;
			}
			m_pModel->SetGroup( nLevel );
		} else
			m_pModel->SetGroup( 2 );		// �׸��� ������ ���� ���� �ܰ�� ����.
	}

	if( pModel->m_pModelElem->m_nTextureEx > 0 )		// Ȯ���ؽ��� ���³��ΰ�?
		pModel->SetTextureEx( pModel->m_pModelElem->m_nTextureEx );
	else
		pModel->SetTextureEx( 0 );

#ifdef __CLIENT
	if( IsActiveMover() )
	{
		// ����� �Ӹ�ī�� �������ϴ°��̳�?  // �κ��� �ִ� ��� 
		CItemElem* pItemElem	= GetEquipItem( PARTS_CAP );
		O3D_ELEMENT* pElement   = NULL;
		
		if( pItemElem )
		{
			ItemProp* pItemProp = pItemElem->GetProp();
			if( pItemProp && pItemProp->dwBasePartsIgnore != -1 )
			{
				pElement = ((CModelObject*)m_pModel)->SetEffect(pItemProp->dwBasePartsIgnore, XE_HIDE);
			}
			
			// �����ǻ��� �Ծ������ �Ӹ��������ΰ��� ������ ���� ���ڸ� �������� �ٲ۴�
			CItemElem* pItemElemOvercoat	= GetEquipItem( PARTS_HAT );
						
			if( pItemElemOvercoat )
			{
				if( !pItemElemOvercoat->IsFlag( CItemElem::expired ) )
				{
					ItemProp* pItemPropOC = pItemElemOvercoat->GetProp();
					if( pItemPropOC && pItemPropOC->dwBasePartsIgnore != -1 )
					{
						if( pItemPropOC->dwBasePartsIgnore == PARTS_HEAD )
							((CModelObject*)m_pModel)->SetEffect(PARTS_HAIR, XE_HIDE );
						
						((CModelObject*)m_pModel)->SetEffect(pItemPropOC->dwBasePartsIgnore, XE_HIDE );
					}
					else
					{
						if( pElement )
							pElement->m_nEffect &= ~XE_HIDE;
					}
				}
			}
		}
		else
		{
			// �����ǻ��� �Ծ������ �Ӹ��������ΰ��� ������ ���� ���ڸ� �������� �ٲ۴�
			CItemElem* pItemElemOvercoat	= GetEquipItem( PARTS_HAT );
			
			if( pItemElemOvercoat )
			{
				if( !pItemElemOvercoat->IsFlag( CItemElem::expired ) )
				{
					ItemProp* pItemPropOC = pItemElemOvercoat->GetProp();
					if( pItemPropOC && pItemPropOC->dwBasePartsIgnore != -1 )
					{
						if( pItemPropOC->dwBasePartsIgnore == PARTS_HEAD )
							((CModelObject*)m_pModel)->SetEffect(PARTS_HAIR, XE_HIDE );

						((CModelObject*)m_pModel)->SetEffect(pItemPropOC->dwBasePartsIgnore, XE_HIDE );
					}
				}
			}	
		}		
	}
	else
	{
		// ����� �Ӹ�ī�� �������ϴ°��̳�?  // �κ��� ���°��
		DWORD dwId	= m_aEquipInfo[PARTS_CAP].dwId;
		O3D_ELEMENT*	pElement = NULL;
		
		if( dwId != NULL_ID )
		{
			ItemProp* pItemProp	= prj.GetItemProp( dwId );
			
			if( pItemProp && pItemProp->dwBasePartsIgnore != -1 )
			{
				pElement = pModel->SetEffect(pItemProp->dwBasePartsIgnore, XE_HIDE);
			}
			
			// �����ǻ��� �Ծ������ �Ӹ��������ΰ��� ������ ���� ���ڸ� �������� �ٲ۴�
			dwId	= m_aEquipInfo[PARTS_HAT].dwId;
			if( dwId != NULL_ID )
			{
				if( !(m_aEquipInfo[PARTS_HAT].byFlag & CItemElem::expired) )
				{
					pItemProp	= prj.GetItemProp( dwId );
					if( pItemProp && pItemProp->dwBasePartsIgnore != -1 )
					{
						if( pItemProp->dwBasePartsIgnore == PARTS_HEAD )
							((CModelObject*)m_pModel)->SetEffect(PARTS_HAIR, XE_HIDE );

						((CModelObject*)m_pModel)->SetEffect(pItemProp->dwBasePartsIgnore, XE_HIDE );
					}
					else
					{
						if( pElement )
							pElement->m_nEffect &= ~XE_HIDE;
					}
				}
			}
		}
		else
		{
			// �����ǻ��� �Ծ������ �Ӹ��������ΰ��� ������ ���� ���ڸ� �������� �ٲ۴�
			dwId	= m_aEquipInfo[PARTS_HAT].dwId;
			if( dwId != NULL_ID )
			{
				if( !(m_aEquipInfo[PARTS_HAT].byFlag & CItemElem::expired) )
				{					
					pItemProp	= prj.GetItemProp( dwId );
					if( pItemProp && pItemProp->dwBasePartsIgnore != -1 )
					{
						if( pItemProp->dwBasePartsIgnore == PARTS_HEAD )
							((CModelObject*)m_pModel)->SetEffect(PARTS_HAIR, XE_HIDE );
						
						((CModelObject*)m_pModel)->SetEffect(pItemProp->dwBasePartsIgnore, XE_HIDE );
					}
					else
					{
						if( pElement )
							pElement->m_nEffect &= ~XE_HIDE;
					}
				}
			}
													
		}
	}
#endif //__CLIENT
	
		if( IsMode( TRANSPARENT_MODE ) )		// ������°� �ƴҶ��� ����.
		{
			if( g_eLocal.GetState( EVE_SCHOOL ) == 1 &&		// �б� ������ ������ ����,
				g_eLocal.GetState( EVE_SCHOOL_BATTLE ) < SBS_START &&	// ������ ���� ����,
				!IsAuthHigher( AUTH_GAMEMASTER ) )	// �Ϲ� �����,
			{
				m_pModel->Render( pd3dDevice, &mWorld ); 
			}
			else
			{
				m_pModel->SetBlendFactor( 80 );
				if( IsActiveMover() ||		// �ڱ��ڽ��� ���������� ��� ...Ȥ��
					(IsActiveMover() == FALSE && g_pPlayer->IsAuthHigher( AUTH_GAMEMASTER )) )		// Ÿ���ε� �÷��̾ �׸���.
					m_pModel->Render( pd3dDevice, &mWorld );		// ���������� ���
				m_pModel->SetBlendFactor( 255 );
			}
		}
		else
		{

#ifdef __YSMOOTH_OBJ
			if( m_bSmooth )//&& m_pModel->m_nNoEffect != 2 )
			{
				m_nSmoothCount += 5;

				if( m_nSmoothCount > 255 )
				{
					m_wBlendFactor = 255;
					m_nSmoothCount = 0;
					m_bSmooth      = FALSE;
				}
				else
					m_wBlendFactor = m_nSmoothCount;
			}
#endif //__YSMOOTH_OBJ
			
			m_pModel->SetBlendFactor(m_wBlendFactor);
			m_pModel->Render( pd3dDevice, &mWorld );	// �Ϲ� ���� ���
		}


	if( m_pObjHighlight == this )
	{
	//	m_pObjHighlight = NULL;
		if( m_pModel->m_nNoEffect == 0 )
		{
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		}

		if( IsMode( TRANSPARENT_MODE ) == 0 )		// ������°� �ƴҶ��� ����.
		{
			if( m_pModel->m_nNoEffect == 0 )
			{
				m_pModel->Render( pd3dDevice, &mWorld ); 
				pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
				pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
				pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			}
		}

	}
	if( m_pActMover )
		m_pActMover->Render( pd3dDevice, &mWorld ); 
	
#ifdef __CLIENT
#ifdef __AMAKI
	if( g_pPlayer != this )
	{
		if( m_pRide && m_pActMover->IsFly() )
			m_pRide->Render( pd3dDevice, &mWorld );
	}
#else
	if( IsMode( TRANSPARENT_MODE ) == 0 )		// ������°� �ƴҶ��� ����.
	{
		D3DXMATRIX mRide = m_matWorld;

		if( m_pRide && m_pActMover->IsFly() )
		{
			m_pRide->m_nNoEffect = m_pModel->m_nNoEffect;
			//D3DXMATRIX mRide = m_matWorld;
			ItemProp* pRideProp = prj.GetItemProp( m_dwRideItemIdx );		// ���� Ÿ���ִ� Ż���� ������Ƽ.

			if( pRideProp && pRideProp->dwItemKind3 == IK3_STICK )
			{
				if( pModel->GetMatrixBone(0) )	// ���ڷ�� ��Ʈ ���� �����Ŵ.
				{
					D3DXMATRIX mBoundY;
					D3DXMatrixTranslation( &mBoundY, 0, pModel->GetMatrixBone(0)->_42, 0 );
					D3DXMatrixMultiply( &mRide, &mBoundY, &m_matWorld );
				}
			}
#if __VER >= 14 // __WING_ITEM
			if( pRideProp && pRideProp->dwItemKind3 == IK3_WING )
			{
				static const int SPINE1_BONE = 4;	// �� �α� ô��(������ �������� ĳ���� ���� �������� ������)
				static const int SPINE2_BONE = 5;	// �� �α� ô��(������ ���� �������� �������� ��ġ�� �� �αٿ� ���δ�)
				D3DXMATRIX* pmatSpine1Bone = pModel->GetMatrixBone( SPINE1_BONE );
				D3DXMATRIX* pmatSpine2Bone = pModel->GetMatrixBone( SPINE2_BONE );
				if( pmatSpine1Bone && pmatSpine2Bone )
				{
					D3DXMATRIX matEvent;
					D3DXMatrixIdentity( &matEvent );

					D3DXMATRIX matRotation;
					D3DXMatrixIdentity( &matRotation );
					D3DXMatrixRotationX( &matRotation, D3DXToRadian( 180 ) );
					D3DXMatrixMultiply( &matEvent, &matRotation, &matEvent );
					D3DXMatrixRotationZ( &matRotation, D3DXToRadian( -90 ) );
					D3DXMatrixMultiply( &matEvent, &matRotation, &matEvent );
					D3DXMatrixMultiply( &matEvent, &matEvent, pmatSpine1Bone );

					// ���� ������ ���ؼ��� �ӽ� ��ġ ���� (���� �ݵ�� ��� ������ else �� �ڵ�� ���ϵǾ�� ��)
					if( pRideProp && ( pRideProp->dwID == II_RID_RID_WIN_FEATHER01 || pRideProp->dwID == II_RID_RID_WIN_FEATHER02 ) )
					{
						matEvent._41 = pmatSpine2Bone->_41;
						matEvent._42 = pmatSpine2Bone->_42 - 0.02f;
						matEvent._43 = pmatSpine2Bone->_43 + 0.1f;
					}
					else
					{
						matEvent._41 = pmatSpine2Bone->_41;
						matEvent._42 = pmatSpine2Bone->_42;
						matEvent._43 = pmatSpine2Bone->_43;
					}

					D3DXMatrixMultiply( &mRide, &matEvent, &m_matWorld );
				}
			}
#endif // __WING_ITEM
#ifdef __CSC_EXTEXTURE
			m_pRide->SetTextureEx(m_pRide->m_pModelElem->m_nTextureEx);
#endif //__CSC_EXTEXTURE
			m_pRide->Render( pd3dDevice, &mRide );
			m_pRide->m_nNoEffect = 0;

#ifdef __CLIENT
			// �Ϻ����� ��ȸ���� �ƴϴ�...������������ ��ȸ��~
			if( pRideProp && pRideProp->dwID == II_RID_RID_BOR_LADOLF || pRideProp->dwID == II_RID_RID_BOR_JLADOLF
#if __VER >= 9
				|| pRideProp->dwID == II_RID_RID_BOR_JLADOLF_S || pRideProp->dwID == II_RID_RID_BOR_LADOLF_S
#endif	//
				)
			{
				if( m_pLadolf == NULL )
				{ 
					m_pLadolf = new CModelObject;
					m_pLadolf->InitDeviceObjects( pd3dDevice );
					m_pLadolf->LoadBone( "mvr_Ladolf.chr" );
					m_pLadolf->LoadElement( "mvr_Ladolf.o3d", 0 );
					m_pLadolf->LoadMotion( "mvr_Ladolf_stand.ani" );
				}

				D3DXMATRIX mLadolf = mRide;
				D3DXVECTOR3 vLocal;

				FLOAT fAngXZ = GetAngle();
				FLOAT fAngH  = GetAngleX();
				AngleToVector( &vLocal, fAngXZ, -fAngH-10.0f, 1.4f );

				// ���� ���� - ������
				if( m_pActMover->GetStateFlag() & OBJSTAF_ACC )
				{
					if( m_dwLadolfFlag == 0 )
					{
						m_pLadolf->LoadMotion( "mvr_Ladolf_walk.ani" );
						m_pLadolf->SetMotionBlending( TRUE );
						m_pLadolf->SetLoop( ANILOOP_LOOP );
						m_dwLadolfFlag = 1;
					}
				}
				else
				{
					if( D3DXVec3LengthSq( &m_pActMover->m_vDelta ) < 0.001f )
					{
						if( m_dwLadolfFlag == 1 )
						{
							m_pLadolf->LoadMotion( "mvr_Ladolf_stand.ani" );
							m_pLadolf->SetMotionBlending( TRUE );
							m_pLadolf->SetLoop( ANILOOP_LOOP );
							m_dwLadolfFlag = 0;
						}
					}
				}
			
				mLadolf._41 += vLocal.x;
				mLadolf._42 += vLocal.y;
				mLadolf._43 += vLocal.z;
				
				m_pLadolf->m_nNoEffect = m_pModel->m_nNoEffect;
				m_pLadolf->Render( pd3dDevice, &mLadolf );
				m_pLadolf->m_nNoEffect = 0;
			}
#endif //__CLIENT

		}
	}
#endif

#endif

#ifdef __CLIENT
	// ������ ���� xx���� �̳��� �����°��� TAB���� �ڵ� Ÿ����.
#if __VER >= 8 //__CSC_VER8_5
	if(m_pAngelFlag && m_pAngel != NULL)
	{
		m_pAngel->m_nNoEffect = m_pModel->m_nNoEffect;
		m_pAngel->Render( pd3dDevice, &m_AngelWorldM );
		m_pAngel->m_nNoEffect = 0;
	}
#endif //__CSC_VER8_5

#ifdef __EVE_BALLOON
	if(m_pBalloonFlag && m_pBalloon != NULL)
	{
		m_pBalloon->m_nNoEffect = m_pModel->m_nNoEffect;
		m_pBalloon->Render( pd3dDevice, &m_BalloonWorldM );
		m_pBalloon->m_nNoEffect = 0;
	}
#endif //__EVE_BALLOON
	
	if( /*IsPlayer() &&*/ IsActiveMover() == FALSE && g_pPlayer->m_pActMover->IsFly() )
	{
		if( GetProp()->bKillable == 1 && GetProp()->dwFlying == 1 )		// ���̴°� �����ѳ� / ����� �� Ÿ������ ������. 
		{
			D3DXVECTOR3	vCamera = g_Neuz.m_camera.m_vLookAt - g_Neuz.m_camera.GetPos();			// ī�޶� ���� ���麤��
			D3DXVECTOR3 vObj	= GetPos() - g_Neuz.m_camera.GetPos();	// ī�޶󿡼� this������Ʈ�� ����.
			D3DXVec3Normalize( &vCamera, &vCamera );		// �븻������.
			D3DXVec3Normalize( &vObj, &vObj );
			FLOAT fDot = D3DXVec3Dot( &vCamera, &vObj );
			if( fDot > cosf(D3DXToRadian(15.0f)) )
			{
				CWndWorld *pWndWorld = (CWndWorld *)g_WndMng.m_pWndWorld;
				if( pWndWorld )
				{
					pWndWorld->AddFlyTarget( GetId() );
				}
			}
		}
	}
	
	// ������ �⺻������ �޷��ִ� ����Ʈ. - �̰� ���������� �Ⱦ�������.
	if( (m_dwFlag & MVRF_EFFECT1) == 0 )
	{
		CSfx *pSfx = NULL;
		switch( m_dwIndex )
		{
		case MI_RISEM1:
		case MI_RISEM2:
		case MI_RISEM3:
		case MI_RISEM4:
			pSfx = CreateSfx( D3DDEVICE, XI_NPCRISEMSIGN, GetPos(), GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );
			if( pSfx )
				pSfx->SetScale( GetScale() );
		}
		if( pSfx )
			m_dwFlag |= MVRF_EFFECT1;
	}
	if( m_pModel->m_nNoEffect == 0 )
		RenderQuestEmoticon( pd3dDevice );

#if __VER >= 8 //__Y_EYE_FLASH_8
	if( IsActiveMover() && m_pModel && IsDie() && HasBuffByIk3(IK3_TEXT_DISGUISE) == FALSE )
	{
		CModelObject* pModelObj = (CModelObject*)m_pModel;
		
		O3D_ELEMENT* pElement = NULL;
		pElement = pModelObj->GetParts(PARTS_HEAD);
		
		if( pElement && pElement->m_pObject3D )
		{
			GMOBJECT* pGmObj = pElement->m_pObject3D->GetGMOBJECT(0);
			
			if( pGmObj )
			{
				(*pGmObj->m_pMtrlBlkTexture) = CMover::m_pTextureEye[m_bySex][m_dwHeadMesh];
			}
		}
	}
#endif //__Y_EYE_FLASH_8

#endif
	
}

// �̰��� ȣ��ɶ��� �ٷ� ������ CMover::Render()�� ȣ��Ǿ�� �Ѵ�.
void CMover::RenderPartsEffect( LPDIRECT3DDEVICE9 pd3dDevice )
{
#ifdef _DEBUG
	if( g_pPlayer == this )
	{
		int a = 0;
	} else
	{
		int a = 0;
	}
#endif

	if( IsMode( TRANSPARENT_MODE ) )		// ������°� �ƴҶ��� ����.
		return;

	if( !IsVisible() || IsCull() )
		return;

	pd3dDevice->SetMaterial( g_TextureMng.GetMaterial( pd3dDevice, 0 ) );

	D3DXVECTOR3 vPos = GetPos();
	D3DXMATRIX  matWorld;
	
	matWorld = m_matWorld;

	D3DXVECTOR3 vPos2 = GetPos() - GetWorld()->m_pCamera->m_vPos;
	int nLength = (int)( D3DXVec3Length( &vPos2 ) );
	if( nLength > CWorld::m_fFarPlane ) 
		nLength = (int)( CWorld::m_fFarPlane );
	if( nLength > CWorld::m_fFarPlane - ( CWorld::m_fFarPlane / 4 ) )
		return;
	//m_pModel->SetBlendFactor( 255 );//255 - nBlendFactor );

	CItemElem *pItemElem = NULL;
	ItemProp *pItemProp = NULL;

	if( IsActiveMover() )
	{
		pItemElem = GetEquipItem( PARTS_RWEAPON );	// ������ ���⿡
		if( pItemElem )
			pItemProp = pItemElem->GetProp();
	} else
	{
		DWORD dwItemID = m_aEquipInfo[PARTS_RWEAPON].dwId;
		if( dwItemID != NULL_ID )
			pItemProp = prj.GetItemProp( dwItemID );
	}
	
	if( IsActiveMover() )
	{
		pItemElem = GetEquipItem( PARTS_SHIELD );	// ���п�
		if( pItemElem )
			pItemProp = pItemElem->GetProp();
	} else
	{
		DWORD dwItemID = m_aEquipInfo[PARTS_SHIELD].dwId;	// activemover �� �ƴѳ��� ������������� ������.
		if( dwItemID != NULL_ID )
			pItemProp = prj.GetItemProp( dwItemID );
	}
	if( pItemProp )
	{
		if( pItemProp->nReflect > 0 )	// ���÷�Ʈ�� �ɷ�������
			((CModelObject*)m_pModel)->SetEffect( PARTS_SHIELD, XE_REFLECT );	// ���÷�Ʈ �ɼ����� ����.
	}
	
	m_pModel->SetGroup( 0 );
	if( IsMode( TRANSPARENT_MODE ) == 0 )		// ������°� �ƴҶ��� ����.
	{
		if( pItemProp )
		{
			int nLO = 0, nRO = 0;
			
			if( IsActiveMover() )
			{
				if( GetEquipItem( PARTS_LWEAPON ) )
				{
					nLO = GetEquipItem( PARTS_LWEAPON )->m_nResistAbilityOption;
				}

			}
			else
			{
				nLO = (m_aEquipInfo[PARTS_LWEAPON].nOption & 0xFF000000) >> 24;
			}
			
			if( IsActiveMover() )
			{
				if( GetEquipItem( PARTS_RWEAPON ) )
				{
					nRO = GetEquipItem( PARTS_RWEAPON )->m_nResistAbilityOption;
				}
			}
			else
			{
				nRO = (m_aEquipInfo[PARTS_RWEAPON].nOption & 0xFF000000) >> 24;
			}

			m_pModel->RenderEffect( pd3dDevice, &matWorld, pItemProp->dwItemKind3, nLO, nRO ); 
		}
		else
			m_pModel->RenderEffect( pd3dDevice, &matWorld, NULL_ID ); 		
	}

	extern CPartsLaser g_Laser;
	D3DXMatrixIdentity( &matWorld );
	g_Laser.Render( pd3dDevice, &matWorld, g_ModelGlobal.m_vCameraPos, g_ModelGlobal.m_vCameraForward );

#if __VER >= 10 // __Y_DRAGON_FIRE
	if( m_dwIndex == MI_DU_METEONYKER || m_dwIndex == MI_DU_METEONYKER2 || m_dwIndex == MI_DU_METEONYKER3 ||
		m_dwIndex == MI_DU_METEONYKER4 )
	{
		extern CPartsFireDragon	g_FireDragon;
		g_FireDragon.Render( pd3dDevice, &matWorld );
	}
#endif //__Y_DRAGON_FIRE
}


#ifdef __CLIENT

// nValue ( 1,2,3,4 )
void CMover::RenderGauge( LPDIRECT3DDEVICE9 pd3dDevice, int nValue )
{
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();

    pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;

	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);

	vOut.y -= 40;

	int nGaugeWidth = 80;
	CPoint point( (LONG)( vOut.x - nGaugeWidth / 2 ), (LONG)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);

	CPoint p = CPoint( rect.left, rect.top );
	LONG   y = 200;

	MakeEven( p.x );

	g_Neuz.m_TextureGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, y, 0.8f, 0.55f);

	switch( nValue )
	{
		case 4:
			g_Neuz.m_TextureGauge[4].RenderScal(&(g_Neuz.m_2DRender), p, y, 0.8f, 0.55f);
		case 3:
			g_Neuz.m_TextureGauge[3].RenderScal(&(g_Neuz.m_2DRender), p, y, 0.8f, 0.55f);
		case 2:
			g_Neuz.m_TextureGauge[2].RenderScal(&(g_Neuz.m_2DRender), p, y, 0.8f, 0.55f);
		case 1:
			g_Neuz.m_TextureGauge[1].RenderScal(&(g_Neuz.m_2DRender), p, y, 0.8f, 0.55f);
			break;
		case 0:		// �׸��� �ʴ´�.
			break;
		default:
			ASSERT( 0 );
			break;
	}
	
#ifdef __XUZHU
	g_Neuz.m_2DRender.TextOut( 2, 160, GetHitPoint(), D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
#endif
}


// ����� �ͺ��׷���
void CMover::RenderTurboGauge( LPDIRECT3DDEVICE9 pd3dDevice, DWORD nColor, int nValue, int nMaxValue )
{
	if( IsMode( TRANSPARENT_MODE ) )
		return;
	if( !IsVisible() )
		return;
	//	pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
    pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
//	vOut.y -= 40;
	
	CPoint point( (LONG)( vOut.x - 80 / 2 ), (LONG)( vOut.y - 40 - 3 ) );		// ������ width 100
	CPoint p = CPoint( point.x - 2, point.y - 2 );

	MakeEven( p.x );
	
	g_Neuz.m_TextureTurboGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		// �������� �׵θ�(�Ͳ�?)
	
	//int nHitWidth = nValue * nGaugeWidth / nMaxValue;
	
	int nWidth = g_Neuz.m_TextureTurboGauge[1].m_size.cx * nValue / nMaxValue;
	
	CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureTurboGauge[1].m_size.cy );
	
	if( m_pActMover->GetStateFlag() & OBJSTAF_TURBO )
	{
		if( g_nRenderCnt & 1 )
		{
			g_Neuz.m_TextureTurboGauge[1].Render( &g_Neuz.m_2DRender, p, p2, 200, 0.8f, 0.55f);
		}
		else
		{
			int nWidthScale = (int)( (96 * nValue / nMaxValue) * 0.8f );
			CRect rect( p.x + 1, p.y + 1, p.x + nWidthScale, p.y + 6 );
			g_Neuz.m_2DRender.RenderFillRect( rect, 0xffffffff );
		}
		
	} else
		g_Neuz.m_TextureTurboGauge[1].Render( &g_Neuz.m_2DRender, p, p2, 200, 0.8f, 0.55f);

#ifdef __XUZHU
	g_Neuz.m_2DRender.TextOut( p.x + 128, p.y, nValue, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
#endif

	// �ӵ� ǥ��.

	extern int g_nFlySpeed;
	char szBuff[64];
	int		nLen, i, nIdx;
	sprintf( szBuff, "%d", g_nFlySpeed );
	nLen = strlen( szBuff );

//	vPosHeight = vPos;
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	vPosHeight.y = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	

	// ������Ʈ ���.
	p.x = (LONG)( vOut.x + 30 );
	p.y = (LONG)( vOut.y + 0 );

	MakeEven( p.x );

	for( i = 0; i < nLen; i ++ )
	{
		nIdx = szBuff[i] - '0';
		g_WndMng.m_pWndWorld->m_texFontDigital.GetAt(nIdx)->Render( &g_Neuz.m_2DRender, p );		// ���� ������.
		p.x += 16;
		
	}

	g_WndMng.m_pWndWorld->m_texFontDigital.GetAt(10)->Render( &g_Neuz.m_2DRender, p );		// ���� ������.
	
	
}
#endif // __CLIENT

#ifdef __CLIENT
void CMover::RenderQuestEmoticon( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( IsPlayer() || m_bShowQuestEmoticon == FALSE )
		return;
	LPCHARACTER lpCharacter = GetCharacter();

	if( lpCharacter == NULL ) return;

	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();

    pd3dDevice->GetViewport( &vp );

	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );

	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;

	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	//vOutFLOAT fHeight = ( vOut[0].y - vOut[4].y ) - 10f;
	CPoint point;
	point.x = (LONG)( vOut.x );
	point.y = (LONG)( vOut.y );

	MakeEven( point.x );
	//point.x -= pFont->GetTextExtent( szName ).cx / 2;
	point.y -= 50;

	if( m_nQuestEmoticonIndex  > 0 )	
	{
		
        pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
        pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
        pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		point.x	= (LONG)( point.x - (16 * m_fQuestEmoticonScale) );
		point.y	= (LONG)( point.y - (16 * m_fQuestEmoticonScale) );
		g_Neuz.m_texQuestEmoticon.GetAt( m_nQuestEmoticonIndex )->RenderScal(&(g_Neuz.m_2DRender), point, 255, m_fQuestEmoticonScale, m_fQuestEmoticonScale );

		pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        TRUE );
	}
}
#endif // __CLIENT

void CMover::RenderChrState(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( !IsVisible() )
		return;

	if( IsPlayer() )
	{
		if( IsCull() == FALSE )
		{
			// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
			D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
			D3DVIEWPORT9 vp;
			const BOUND_BOX* pBB;
			
			if( m_pModel )
				pBB	= m_pModel->GetBBVector();
			else
				return;
			
			pd3dDevice->GetViewport( &vp );

			D3DXMATRIX matTrans;
			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y , vPos.z);
			
			D3DXMatrixMultiply( &matWorld, &matWorld, &GetMatrixScale() );
			D3DXMatrixMultiply( &matWorld, &matWorld, &GetMatrixRotation() );
			D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
			
			vPosHeight = pBB->m_vPos[0];
			vPosHeight.x = 0;
			vPosHeight.z = 0;
			
			D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
				&GetWorld()->m_pCamera->m_matView, &matWorld);
			
			CPoint point;
			point.x = (LONG)( vOut.x - 32 / 2 );
			point.y = (LONG)( vOut.y - 32 );
			
			MakeEven( point.x );			

			CTexture* pTexture = NULL;
			if( GetAdjParam( DST_CHRSTATE ) & CHS_STUN )
			{
				pTexture = g_DialogMsg.m_texEmoticon.GetAt(DLGEMOT_STUN);
			}

			if( pTexture )
			{
				g_Neuz.m_2DRender.RenderTexture( point, pTexture, 220 );
			}
		}
	}
}
#if __VER >= 13 // __HONORABLE_TITLE
LPCTSTR	CMover::GetTitle()
{
	return m_strTitle.GetBuffer(0);
}

void	CMover::SetTitle(LPCTSTR pTitle)
{
	m_strTitle = pTitle;
}
#endif	// __HONORABLE_TITLE

void CMover::RenderName( LPDIRECT3DDEVICE9 pd3dDevice, CD3DFont* pFont, DWORD dwColor )
{
	if( !IsVisible() )
		return;
	
	if( g_Option.m_nPlayerName == FALSE && IsActiveMover() )
		return;

	if( g_Option.m_nOtherPlayerName == FALSE && !IsActiveMover() && IsPlayer() )
		return;

	char szName[ 256 ];
	strcpy( szName, m_szName );

	BOOL  bOtherColor = FALSE;
	DWORD dwNewColor;

	if( IsPlayer() )
	{
		if( GetWorld()->GetID() == WI_WORLD_GUILDWAR && g_pPlayer->GetGuild() && GetGuild() )
		{
			bOtherColor = TRUE;
			if( g_pPlayer->GetGuild()->GetGuildId() == GetGuild()->GetGuildId() )
				dwNewColor = COLOR_GUILDCOMBAT;
			else
				dwNewColor = COLOR_PLAYER;							
		}
		// ���� ���� ��ǥ��.
#if __VER >= 13 // __HONORABLE_TITLE
		if( IsChaotic() )
			dwColor = prj.m_PKSetting.dwChaoColor;
		else if( IsPKPink() )
			dwColor = prj.m_PKSetting.dwReadyColor;
		else if( m_dwAuthorization >= AUTH_ADMINISTRATOR ) // GM / Admin colors
			dwColor = COLOR_ADMINISTRATOR;
		else if( m_dwAuthorization >= AUTH_GAMEMASTER )
			dwColor = COLOR_GAMEMASTER;
		else
			dwColor = prj.m_PKSetting.dwGeneralColor;
		
		CString strFameName = GetTitle();
		if( strFameName.IsEmpty() == FALSE )
		{
			CString strName;
			strName = "[";
			strName += strFameName;
			strName += "] ";
			strName += m_szName;
			strcpy( szName, (LPCTSTR)strName );
		}		

		// GM / Admin tag
		if(m_dwAuthorization >= AUTH_GAMEMASTER)
		{
			CString strName;
			strName = szName;
			if(m_dwAuthorization >= AUTH_ADMINISTRATOR)
			{
				strName += " [Admin]";
			}
			else
			{
				strName += " [GM]";
			}
			strcpy( szName, (LPCTSTR)strName );
		}
#else
#if __VER >= 8 // __S8_PK
		if( IsChaotic() )
			dwColor = prj.m_PKSetting.dwChaoColor;
		else if( IsPKPink() )
			dwColor = prj.m_PKSetting.dwReadyColor;
		else
			dwColor = prj.m_PKSetting.dwGeneralColor;
		
		CString strFameName = GetFameName();
		if( strFameName.IsEmpty() == FALSE )
		{
			CString strName;
			strName = "[";
			strName += strFameName;
			strName += "] ";
			strName += m_szName;
			strcpy( szName, (LPCTSTR)strName );
		}
#else // __VER >= 8 // __S8_PK
		KarmaProp* pKarmaProp = prj.GetKarmaProp(m_nSlaughter);
		if( pKarmaProp )
		{
			dwColor = pKarmaProp->dwColor;
			CString strFameName = GetFameName();
			if( strFameName.IsEmpty() == FALSE )
			{
				CString strName;
				strName = "[";
				strName += strFameName;
				strName += "] ";
				strName += m_szName;
				strcpy( szName, (LPCTSTR)strName );
			}		
		}
#endif // __VER >= 8 // __S8_PK
#endif
	}

	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos, vPosHeight;

	//��ȯ���� GetScrPos�� ��ġ�� ���Ѵ�.
	MoverProp* pMoverProp = GetProp();
	if( pMoverProp && ( pMoverProp->dwAI == AII_PET || pMoverProp->dwAI == AII_EGG ) )
		vPos = GetScrPos();
	else
		vPos = GetPos();
	
    D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();

    pd3dDevice->GetViewport( &vp );

	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );

	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;

	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	//vOutFLOAT fHeight = ( vOut[0].y - vOut[4].y ) - 10f;
	CPoint point;
	point.x = (LONG)( vOut.x );
	point.y = (LONG)( vOut.y );

	MakeEven( point.x );
	point.x -= pFont->GetTextExtent( szName ).cx / 2;
	point.y -= 20;
	point.x -= 3;
	
	// Ư�� ĳ���� �̸� ��� 
	if( m_dwAuthorization >= AUTH_GAMEMASTER )
	{
		if( ( GetIndex() == MI_CROWNIBLIS || GetIndex() == MI_CROWNSHADE || GetIndex() == MI_CROWNBUBBLE ) )
		{
			strcpy( szName, GetProp()->szName ); 
			point.x = (LONG)( vOut.x );
			point.y = (LONG)( vOut.y );
			MakeEven( point.x );
			point.x -= pFont->GetTextExtent( szName ).cx / 2;
			point.y -= 20;
			dwColor = 0xffa0a0ff;
		}
		else
		if( GetIndex() == MI_DWARPET || GetIndex() == MI_DWARPETMAS )
		{
			TCHAR szStr[ 64 ];
			dwColor = 0xffa0a0ff;
			sprintf( szStr, "[%s]", GetProp()->szName );
			point.x = (LONG)( vOut.x );
			point.y = (LONG)( vOut.y );
			MakeEven( point.x );
			point.x -= pFont->GetTextExtent( szStr ).cx / 2;
			point.y -= 35;
			pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), dwColor, szStr );
			point.x = (LONG)( vOut.x );
			point.y = (LONG)( vOut.y );
			MakeEven( point.x );
			point.x -= pFont->GetTextExtent( szName ).cx / 2;
			point.y -= 20;
		}
	}

#ifdef __CLIENT
	if( IsPlayer() )
	{
#if __VER >= 8 // __S8_PK
		if( dwColor == prj.m_PKSetting.dwGeneralColor )		// ��ũ���̳� ī�����̸� �ٲ��� �ʰ� ����
#endif // __VER >= 8 // __S8_PK
		if( g_Party.FindMember( m_idPlayer ) != -1 )		// ��Ƽ����� ���� �ٸ��� ǥ��.
			dwColor = COLOR_PARTY;
		
		if( bOtherColor )
			dwColor = dwNewColor;
		
		pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), dwColor, szName );

		if( IsMode( EVENT_OLDBOY_MODE ) )
		{
			int nWidth = pFont->GetTextExtent( szName ).cx;
			pFont->DrawText( (FLOAT)( point.x+nWidth ), (FLOAT)( point.y ), D3DCOLOR_XRGB( 0, 200, 0 ), " ��" );
		}
	}
	else
#endif
	{
		if( g_Option.m_nMonName )
		{				
			LPCHARACTER lpCharacter = GetCharacter();
			if( lpCharacter && lpCharacter->m_nStructure != -1 ) 
			{
				LPSTRUCTURE lpStr = &prj.m_aStructure[ lpCharacter->m_nStructure ];
				TCHAR szStr[ 64 ];
				sprintf( szStr, "[%s]", lpStr->szName );
				point.x = (LONG)( vOut.x );
				point.y = (LONG)( vOut.y );
				MakeEven( point.x );
				point.x -= pFont->GetTextExtent( szStr ).cx / 2;
				point.y -= 35;
				pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), COLOR_NPC, szStr );
				point.x = (LONG)( vOut.x );
				point.y = (LONG)( vOut.y );
				MakeEven( point.x );
				point.x -= pFont->GetTextExtent( szName ).cx / 2;
				point.y -= 20;
			}
			//sprintf( szNameTemp, "NPC %s", m_szName );
			//pFont->DrawText( vOut.x + 1, vOut.y + 1, 0xff000000, szName );
			if( IsPeaceful() )
			{
				pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), COLOR_NPC, szName );
			}
			else
			{
				if( m_bActiveAttack )	// ���� ���ʹ� �����Ÿ���.
				{
					pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), 0xffff0000, szName );						
				} else
				{
					pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), COLOR_MONSTER, szName );
				}
			}
		}
	}
	// �÷��̾ �׸��� ����� ������ �����ش�.
	if( GetWorld()->m_bViewIdState )
	{
		char szBuff[256];
		CString strName;
		CMover *pAttacker = prj.GetMover( m_idAttacker );
		if( pAttacker )
			strName = pAttacker->m_szName;
		sprintf( szBuff, "0x%x %08x, %d %d, %d(%s)", m_pActMover->GetStateFlag(), m_pActMover->GetState(), GetId(), m_pActMover->m_bGround, (int)m_idAttacker, strName );
		pFont->DrawText( vOut.x, vOut.y - 50, dwColor, szBuff );
		
	}
/*#ifdef __XUZHU
	if( GetAdjParam( DST_CHRSTATE ) & CHS_STUN )
		pFont->DrawText( point.x, point.y - 20, dwColor, "����" );
	if( GetAdjParam( DST_CHRSTATE ) & CHS_POISON )
		pFont->DrawText( point.x + 40, point.y - 20, dwColor, "��" );
	if( GetAdjParam( DST_CHRSTATE ) & CHS_DARK )
		pFont->DrawText( point.x + 80, point.y - 20, dwColor, "����" );
	if( GetAdjParam( DST_CHRSTATE ) & CHS_BLEEDING )
		pFont->DrawText( point.x + 100, point.y - 20, dwColor, "����" );
#endif // */
	
// Render Guild Name & Logo
	CGuild* pGuild = GetGuild();	

	BOOL bSkip = FALSE;
	// ����̰� �������̸� ����� �ΰ� �ȱ׸���.
	if( IsAuthHigher( AUTH_GAMEMASTER ) == TRUE )
	{
		if( HasBuffByIk3(IK3_TEXT_DISGUISE) )
			bSkip = TRUE;
	}
	
#if __VER >= 10 // __CSC_VER9_1
	//���� �� ���� ������ ǥ�� �߰�.
	point.x = (LONG)( vOut.x );
	point.y = (LONG)( vOut.y );
	MakeEven( point.x );
	//pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	
	if(!bSkip && IsPlayer())
	{
		BYTE checkhero = GetLegendChar();
		CTexture* pTexture;
		CString strPath;
		CSize cs1 = pFont->GetTextExtent( szName );
		CSize cs2;
		
		if(pGuild != NULL)
		{
			cs2 = pFont->GetTextExtent( pGuild->m_szGuild );

			if( cs1.cx > cs2.cx )
				point.x += cs1.cx / 2;
			else
				point.x += cs2.cx / 2;
		}
		else
			point.x += cs1.cx / 2;
	
		if(checkhero == LEGEND_CLASS_MASTER && m_nLevel >= 60 && m_nLevel <= 120) //������ ���� ���.
		{
			if(/*m_nLevel >= 60 && */m_nLevel < 70) //Level Down�� ��츦 �����ؼ� �ּ�ó��.
			//	strPath = MakePath( DIR_ICON, "icon_Expert1.dds");
				pTexture = g_Neuz.m_pMasterIcon[0];
			else if(m_nLevel >= 70 && m_nLevel < 80)
			//	strPath = MakePath( DIR_ICON, "icon_Expert2.dds");
				pTexture = g_Neuz.m_pMasterIcon[1];
			else if(m_nLevel >= 80 && m_nLevel < 90)
			//	strPath = MakePath( DIR_ICON, "icon_Expert3.dds");
				pTexture = g_Neuz.m_pMasterIcon[2];
			else if(m_nLevel >= 90 && m_nLevel < 100)
			//	strPath = MakePath( DIR_ICON, "icon_Expert4.dds");
				pTexture = g_Neuz.m_pMasterIcon[3];
			else if(m_nLevel >= 100 && m_nLevel < 110)
			//	strPath = MakePath( DIR_ICON, "icon_Expert5.dds");
				pTexture = g_Neuz.m_pMasterIcon[4];
			else if(m_nLevel >= 110 && m_nLevel <= 120)
			//	strPath = MakePath( DIR_ICON, "icon_Expert6.dds");
				pTexture = g_Neuz.m_pMasterIcon[5];

			//pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
			if(pTexture != NULL)
			{
				point.y -= pTexture->m_size.cy + 5;
				pTexture->Render( &g_Neuz.m_2DRender, point );
			}
		}
		else if(checkhero == LEGEND_CLASS_HERO) //������ ���.
		{
			//strPath = MakePath( DIR_ICON, "icon_Hero.dds");
			//pTexture = CWndBase::m_textureMng.AddTexture( g_Neuz.m_pd3dDevice, strPath, 0xffff00ff );
			pTexture = g_Neuz.m_pHeroIcon;
			if(pTexture != NULL)
			{
				point.y -= pTexture->m_size.cy + 5;
				pTexture->Render( &g_Neuz.m_2DRender, point );
			}
		}
	}
	//pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
#endif //__CSC_VER9_1

	if( pGuild && bSkip == FALSE )
	{
		point.x = (LONG)( vOut.x );
		point.y = (LONG)( vOut.y );
		MakeEven( point.x );	
		point.x -= pFont->GetTextExtent( pGuild->m_szGuild ).cx / 2;
		point.y -= 32;
		point.x -= 3;

#if __VER < 8 // __S8_PK
		KarmaProp* pKarmaProp = prj.GetKarmaProp(m_nSlaughter);
		if( pKarmaProp )
			dwColor = pKarmaProp->dwColor;
#endif // __VER < 8 // __S8_PK

		CWndWorld* pWndWorld = (CWndWorld*)g_WndMng.GetWndBase( APP_WORLD );				
		// ��帶���͸� ������ ��������� ���
		dwColor = 0xFFFFFFFF;
		if( pGuild->IsMaster(m_idPlayer))
			dwColor = 0xFFFFFF99;
		else
		if( bOtherColor )
			dwColor = dwNewColor;

		// ����Ĺ��� ���������̸鼭 ������� ��ȫ������ ���
		if( g_GuildCombatMng.m_nGCState >= CGuildCombat::MAINTENANCE_STATE )
		{
			if( pWndWorld && pWndWorld->GetGCStatusDefender(m_idPlayer) != -1 )
			{
				dwColor = 0xFFFEA8FF;
			}		
		}

		// ����Ʈ �÷��̾� ǥ�� - ��±���̰� ����Ʈ �÷��̾�� ����Ʈ �÷��̾ ǥ��
		// ��� ���� ���ΰ� �տ� ǥ������
		BOOL bRenderCombatBestLogo = FALSE;		
		BOOL bRenderCombatWinLogo  = FALSE;

		// �������� ������ �ʾ������� �������
		if( g_GuildCombatMng.m_nState == CGuildCombat::CLOSE_STATE )
		{
			if( g_GuildCombatMng.m_uBestPlayer == m_idPlayer )
				bRenderCombatBestLogo = TRUE;
			else
			if( g_GuildCombatMng.m_uWinGuildId == pGuild->GetGuildId() )
				bRenderCombatWinLogo = TRUE;
		}

		pFont->DrawText( (FLOAT)( point.x ), (FLOAT)( point.y ), dwColor, pGuild->m_szGuild );	
		
		if( pGuild->m_dwLogo != 0 )
		{
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );

			int nLogoSizey = pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].m_size.cy+5;
		
			point.x = (LONG)( vOut.x );
			point.y = (LONG)( vOut.y );
			
			MakeEven( point.x );	
			
			CSize cs1 = pFont->GetTextExtent( szName );
			CSize cs2 = pFont->GetTextExtent( pGuild->m_szGuild );
				
			if( cs1.cx > cs2.cx )
			{
				point.x -= cs1.cx / 2;
				point.y -= nLogoSizey;
			}
			else
			{
				point.x -= cs2.cx / 2;
				point.y -= nLogoSizey;
			}
			
			point.x -= pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].m_size.cx+2;
			pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].Render( &g_Neuz.m_2DRender, point, 255 );
			
			D3DDEVICE->SetRenderState( D3DRS_DESTBLEND ,D3DBLEND_INVSRCALPHA );			
			
			if( bRenderCombatBestLogo )
			{
				point.x -= pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].m_size.cx+2;
				pWndWorld->m_TexGuildBest.Render( &g_Neuz.m_2DRender, point, 255 );
			}	
			else
			if( bRenderCombatWinLogo )
			{
				point.x -= pWndWorld->m_pTextureLogo[pGuild->m_dwLogo-1].m_size.cx+2;
				pWndWorld->m_TexGuildWinner.Render( &g_Neuz.m_2DRender, point, 255 );
			}						
		}
		else
		{
			// ���ΰ� �����ȵ� ���������� ����� ����ϰ��
			if( bRenderCombatBestLogo || bRenderCombatWinLogo )
			{
				D3DDEVICE->SetRenderState( D3DRS_DESTBLEND ,D3DBLEND_INVSRCALPHA );
				int nLogoSizey = 30;
				
				point.x = (LONG)( vOut.x );
				point.y = (LONG)( vOut.y );
				
				MakeEven( point.x );			
				
				if( strlen(szName) > strlen(pGuild->m_szGuild) )
				{
					point.x -= pFont->GetTextExtent( szName ).cx / 2;
					point.y -= nLogoSizey;
				}
				else
				{
					point.x -= pFont->GetTextExtent( pGuild->m_szGuild ).cx / 2;
					point.y -= nLogoSizey;
				}
				
				point.x -= nLogoSizey;
				
				if( bRenderCombatBestLogo )
					pWndWorld->m_TexGuildBest.Render( &g_Neuz.m_2DRender, point, 255 );
				else
				if( bRenderCombatWinLogo )
					pWndWorld->m_TexGuildWinner.Render( &g_Neuz.m_2DRender, point, 255 );
			}				
		}
		

		if( IsPlayer() && !IsActiveMover() && g_Neuz.m_NeuzEnemy.IsPKing( GetId() ) )
		{
			if( (m_dwFlag & MVRF_WARMARK) == 0 )	// ���︶ũ�� ���� �ȶ��ִ� ���¸�
			{
				RenderFlag( ENEMY_PK );		// ���� ���� SFX
			}
		}
		else
#ifdef __CLIENT
		// ������ÿ� �Ӹ����� �ߴ� ������ũ
		if( g_eLocal.GetState( EVE_GUILDWAR ) == 1 )		// ����� ������ �����ΰ�.
		{
			if( m_idWar && g_pPlayer->m_idWar == m_idWar )	// ���� ���� ���￡ �������� ���ΰ�.
			{
				if( m_idGuild != g_pPlayer->m_idGuild )		// ���� �ٸ�����ΰ�(�����ΰ��� �̰ɷ� �ȵȴ�.)
				{
					if( (m_dwFlag & MVRF_WARMARK) == 0 )	// ���︶ũ�� ���� �ȶ��ִ� ���¸�
					{
						RenderFlag( ENEMY_WAR );
					}
				} 
			}
		}
#endif // CLIENT
	}
}

// ������������ ���� ǥ��
void CMover::RenderFlag( int nType )
{
	// ��������� sfx�� �Ӹ����� �����.
	CSfx *pSfx = CreateSfx( D3DDEVICE, XI_GEN_PVP_FLAG01, GetPos(), GetId(), D3DXVECTOR3(0,0,0), NULL_ID, -1 );
	if( pSfx )
	{
		pSfx->SetScale( D3DXVECTOR3(1.5f, 1.5f, 1.5f) );
		((CSfxDuelParty*)pSfx)->m_nType = nType;
		m_dwFlag |= MVRF_WARMARK;
	}
}
	
void CMover::RenderHP(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( !IsVisible() )
		return;
	
	if( IsMode( TRANSPARENT_MODE ) )
		return;

	if( g_Option.m_nPlayerName == FALSE && IsActiveMover() )
		return;
	
	if( g_Option.m_nOtherPlayerName == FALSE && !IsActiveMover() && IsPlayer() )
		return;

	if( g_WorldMng()->m_bViewHP == FALSE )
		return;

	// ����̰� �������̸� HP�� �ȱ׸���.
	if( IsAuthHigher( AUTH_GAMEMASTER ) == TRUE )
	{
		if( IsPlayer() && HasBuffByIk3(IK3_TEXT_DISGUISE) )
			return;
	}

#ifdef __QUIZ
	if( !IsActiveObj() && ( m_dwMode & QUIZ_RENDER_SKIP_MODE ) )
		return;
#endif // __QUIZ
	
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos, vPosHeight;
	D3DVIEWPORT9 vp;

	//��ȯ���� GetScrPos�� ��ġ�� ���Ѵ�.
	MoverProp* pMoverProp = GetProp();
	if( pMoverProp && ( pMoverProp->dwAI == AII_PET || pMoverProp->dwAI == AII_EGG ) )
		vPos = GetScrPos();
	else
		vPos = GetPos();

	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	vOut.y -= 34;
	int nGaugeWidth = 80;
	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
	MakeEven( p.x );

	BOOL bDbuff = FALSE;

	if( IsStateDbuff() )
		bDbuff = TRUE;

#if __VER < 9 // __S_9_ADD
	int nHitPercent = GetHitPointPercent( 100 );
	if( nHitPercent < CRITICAL_BERSERK_HP )
	{
		if( g_nRenderCnt & 1 )
		{
			g_Neuz.m_TextureHPGauge[0].RenderScal2(&(g_Neuz.m_2DRender), p, 200, 0.7f, 0.45f, 0xffffffff );		
		}
		else
		{
			g_Neuz.m_TextureHPGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		
		}
	}
	else
#endif // __S_9_ADD
	{
		g_Neuz.m_TextureHPGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		
	}
	
	int nWidth = g_Neuz.m_TextureHPGauge[1].m_size.cx * GetHitPoint() / GetMaxHitPoint();
	
	CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureHPGauge[1].m_size.cy );
	
	if( bDbuff )
		g_Neuz.m_TextureHPGauge[2].Render(&(g_Neuz.m_2DRender), p, p2, 200, 0.8f, 0.55f);		
	else
		g_Neuz.m_TextureHPGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 200, 0.8f, 0.55f);				

#if __VER >= 8 //__CSC_VER8_5
	if(m_pAngelFlag)
		RenderAngelStatus( pd3dDevice );
#endif //__CSC_VER8_5
}


#if __VER >= 11 // __SYS_COLLECTING
void CMover::RenderCltGauge(LPDIRECT3DDEVICE9 pd3dDevice)
{

	if( !IsVisible())	return;
	
	if(!IsCollecting()) return;

	if( g_Option.m_nOtherPlayerName == FALSE && !IsActiveMover() && IsPlayer() )
		return;

	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos, vPosHeight;
	D3DVIEWPORT9 vp;

	MoverProp* pMoverProp = GetProp();
	vPos = GetPos();

	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x  = 0;
	vPosHeight.z  = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	vOut.y -= 48;
	int nGaugeWidth = 80;
	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
	MakeEven( p.x );

	g_Neuz.m_TexCltGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 255, 0.8f, 0.55f);		
	
	int nWidth = g_Neuz.m_TexCltGauge[1].m_size.cx * m_nCltTime / m_nMaxCltTime;
	
	CPoint p2 = CPoint( nWidth, g_Neuz.m_TexCltGauge[1].m_size.cy );
	
	g_Neuz.m_TexCltGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 255, 0.8f, 0.55f);	

}
#endif

#if __VER >= 8 //__CSC_VER8_5
void CMover::RenderAngelStatus(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if(m_pAngel == NULL)
		return;
	
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos, vPosHeight;
	vPos = m_AngelPos;
	D3DVIEWPORT9 vp;
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	matWorld._41 = m_AngelPos.x;
	matWorld._42 = m_AngelPos.y;
	matWorld._43 = m_AngelPos.z;

	vPosHeight.y = 1.4f;
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	int nGaugeWidth = 40;
	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
	MakeEven( p.x );
	
	g_Neuz.m_TextureAngelGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.4f, 0.55f);
	
	int nAngel = 100;
#ifdef __BUFF_1107
	IBuff* pBuff	= m_buffs.GetBuffByIk3( IK3_ANGEL_BUFF );
	if( pBuff )
	{
		ItemProp* pItemProp = pBuff->GetProp();
		if( pItemProp )
			nAngel = (int)( (float)pItemProp->nAdjParamVal1 );
	}
#else	// __BUFF_1107
	LPSKILLINFLUENCE lpSkillIn = m_SkillState.GetItemBuf( IK3_ANGEL_BUFF );
	if( lpSkillIn )
	{
		ItemProp* pItemProp = prj.GetItemProp( lpSkillIn->wID );
		if( pItemProp )
			nAngel = (float)pItemProp->nAdjParamVal1;
	}
#endif	// __BUFF_1107
	if( nAngel <= 0 || 100 < nAngel  )
		nAngel = 100;
	
	EXPINTEGER maxExp = prj.m_aExpCharacter[m_nAngelLevel].nExp1 / 100 * nAngel;
	if( maxExp > 0)
	{
		int nWidth = (int)( g_Neuz.m_TextureAngelGauge[1].m_size.cx * m_nAngelExp / maxExp );
		CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureAngelGauge[1].m_size.cy );
		g_Neuz.m_TextureAngelGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 200, 0.4f, 0.55f);		
	}
}
#endif //__CSC_VER8_5

void CMover::RenderCasting(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) == FALSE )
		return;
	if( g_pPlayer->m_nReadyTime == 0 )
		return;
	if( !IsVisible() )
		return;
	
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
	D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	vOut.y -= 48;
	int nGaugeWidth = 80;

	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
		
	MakeEven( p.x );
	
	g_Neuz.m_TextureCastingGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		
	
	ItemProp* pItemProp = prj.GetItemProp( g_pPlayer->m_dwUseItemId );
	if( pItemProp )
	{
		DWORD dwTime = timeGetTime() - g_pPlayer->m_nReadyTime;
		int nWidth = (int)( g_Neuz.m_TextureHPGauge[1].m_size.cx * ( (float)dwTime / (float)pItemProp->dwSkillReadyType ) );
		if( 112 < nWidth )
			nWidth = 112;
		if( nWidth < 0 )
			nWidth = 0;

		CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureHPGauge[1].m_size.cy );
	
		g_Neuz.m_TextureCastingGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 255, 0.8f, 0.55f);		
	}
}

void CMover::RenderCtrlCasting(LPDIRECT3DDEVICE9 pd3dDevice)
{
//	if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) == FALSE )
//		return;
	if( g_pPlayer->m_dwCtrlReadyTime  == 0xffffffff )
		return;
	if( !IsVisible() )
		return;
	
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
	D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	vOut.y -= 48;
	int nGaugeWidth = 80;
	
	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
	
	MakeEven( p.x );
	
	g_Neuz.m_TextureCastingGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		
	
//	ItemProp* pItemProp = prj.GetItemProp( g_pPlayer->m_dwUseItemId );
//	if( pItemProp )
	{
		DWORD dwTime =  OPEN_COOLTIME - (g_pPlayer->m_dwCtrlReadyTime - timeGetTime()) ;
		int nWidth = (int)( g_Neuz.m_TextureHPGauge[1].m_size.cx * ((float)dwTime / OPEN_COOLTIME ) );
		if( 112 < nWidth )
			nWidth = 112;
		if( nWidth < 0 )
			nWidth = 0;
		
		CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureHPGauge[1].m_size.cy );
		
		g_Neuz.m_TextureCastingGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 255, 0.8f, 0.55f);		
	}
}

#if __VER >= 10 // __LEGEND	//	9�� ���½ý���	Neuz, World, Trans
void CMover::RenderSkillCasting(LPDIRECT3DDEVICE9 pd3dDevice)
{
//	if( g_pPlayer->IsStateMode( STATE_BASEMOTION_MODE ) == FALSE )
//		return;
	DWORD dwtmpTick = GetTickCount();
	DWORD dwTmpCastingEndTick = g_pPlayer->m_pActMover->GetCastingEndTick();
	if( dwTmpCastingEndTick < dwtmpTick )
		return;

	int dwTmpSkillID = g_pPlayer->m_pActMover->GetCastingSKillID();
	if( dwTmpSkillID != SI_KNT_HERO_DRAWING &&  dwTmpSkillID != SI_RIG_HERO_RETURN )
		return;

	if( !IsVisible() )
		return;

	DWORD dwTmpCastingTick = (DWORD)g_pPlayer->m_pActMover->GetCastingTick();

	
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// ���� ��ǥ�� ��ũ�� ��ǥ�� �������� �Ѵ�.
	D3DXVECTOR3 vOut, vPos = GetPos(), vPosHeight;
	D3DVIEWPORT9 vp;
	const BOUND_BOX* pBB = m_pModel->GetBBVector();
	
	pd3dDevice->GetViewport( &vp );
	vp.X = 0;
	vp.Y = 0;
	
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matScale );
	D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTrans );
	
	vPosHeight = pBB->m_vPos[0];
	vPosHeight.x = 0;
	vPosHeight.z = 0;
	
	D3DXVec3Project( &vOut, &vPosHeight, &vp, &GetWorld()->m_matProj,
		&GetWorld()->m_pCamera->m_matView, &matWorld);
	
	vOut.y -= 48;
	int nGaugeWidth = 80;
	
	CPoint point( (int)( vOut.x - nGaugeWidth / 2 ), (int)( vOut.y - 3 ) );
	CRect rect( point.x - 2, point.y - 2, point.x + nGaugeWidth + 2, point.y + 5 + 2);
	
	CPoint p = CPoint( rect.left, rect.top );
	
	MakeEven( p.x );
	
	g_Neuz.m_TextureCastingGauge[0].RenderScal(&(g_Neuz.m_2DRender), p, 200, 0.8f, 0.55f);		
	
//	ItemProp* pItemProp = prj.GetItemProp( g_pPlayer->m_dwUseItemId );
//	if( pItemProp )
	{
		DWORD dwTime =  dwTmpCastingTick - (dwTmpCastingEndTick - dwtmpTick) ;
		int nWidth = (int)( g_Neuz.m_TextureHPGauge[1].m_size.cx * ((float)dwTime / dwTmpCastingTick ) );
		if( 112 < nWidth )
			nWidth = 112;
		if( nWidth < 0 )
			nWidth = 0;
		
		CPoint p2 = CPoint( nWidth, g_Neuz.m_TextureHPGauge[1].m_size.cy );
		
		g_Neuz.m_TextureCastingGauge[1].Render(&(g_Neuz.m_2DRender), p, p2, 255, 0.8f, 0.55f);		
	}
}
#endif	//__LEGEND	//	9�� ���½ý���	Neuz, World, Trans


void CMover::RenderPVPCount(LPDIRECT3DDEVICE9 pd3dDevice)
{
	DWORD dwWidth = g_Neuz.m_d3dsdBackBuffer.Width / 2;
	DWORD dwHeight = g_Neuz.m_d3dsdBackBuffer.Height / 2;
	
	if( g_Neuz.m_dwPVPTime <= GetTickCount() && GetTickCount() <= g_Neuz.m_dwPVPTime + 1500 )
	{
		if( g_Neuz.m_nPVPCount == 0 )
			g_Neuz.m_nPVPCount = 10;
		
		float fScal = (float)( GetTickCount() - g_Neuz.m_dwPVPTime );
		fScal = fScal / 10.0f;
		int nAlpha = 180;
		if( 150 < nAlpha )
		if( fScal < 25 )
		{
			fScal = 1.0f;
		}
		else
		{
			fScal /= 10;
			fScal -= 1.5f;
			nAlpha	= (int)( nAlpha - (fScal * 10) );
		}

		if( 1.5f < fScal )
		{
			if( g_Neuz.m_nPVPCount == 0 )
				fScal = 1.5f;
			else if( 2.0f < fScal )
				fScal = 2.0f;
		}

		TRACE( "fScal : %f\n", fScal );

		CPoint p = CPoint( 
					(int)( dwWidth - ( g_Neuz.m_TexturePackPVP.m_ap2DTexture[g_Neuz.m_nPVPCount].m_size.cx * fScal / 2 ) ),
					(int)( dwHeight - ( g_Neuz.m_TexturePackPVP.m_ap2DTexture[g_Neuz.m_nPVPCount].m_size.cy * fScal / 2 ) - 50 )
					);
		 
		MakeEven( p.x );
		g_Neuz.m_TexturePackPVP.m_ap2DTexture[g_Neuz.m_nPVPCount].RenderScal(&(g_Neuz.m_2DRender), p, nAlpha, fScal, fScal);			
	}
	
	
}

#endif // not worldserver