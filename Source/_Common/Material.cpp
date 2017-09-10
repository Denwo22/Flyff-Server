#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>

#include <stdio.h>
#include "material.h"
#include "..\_DirectX\dxutil.h"

#include "HwOption.h"
#include "path.h"
#include "vutil.h"
#include "xutil.h"


CTextureManager		g_TextureMng;

CTextureManager :: CTextureManager()
{
//	int		i;

	memset( m_pMaterial, 0, sizeof(m_pMaterial) );
//	for( i = 0; i < MAX_MATERIAL; i ++ )
//	{
//		m_pMaterial[i].m_bActive = FALSE;
//		m_pMaterial[i].strBitMapFileName[0] = 0;
//	}
	m_nMaxTexture = 0;
	//m_pd3dDevice = NULL;
}

CTextureManager :: ~CTextureManager()
{
	DeleteDeviceObjects();
//	m_pd3dDevice = NULL;
}
HRESULT CTextureManager::DeleteDeviceObjects()
{
	int		i;
	for( i = 0; i < MAX_MATERIAL; i ++ )
	{
		if( m_pMaterial[i].m_bActive )
			SAFE_RELEASE( m_pMaterial[i].m_pTexture );
		m_pMaterial[i].m_bActive = FALSE;
		m_pMaterial[i].strBitMapFileName[0] = 0;
	}
	m_nMaxTexture = 0;
	return  S_OK;
}	

// pTexture�� ����ϴ� ���͸����� ã�� �����Ѵ�.
// �����Ǿ� �ִ� �ؽ��Ķ�� ���ī���͸� ���� 1�ΰ͸� �����Ѵ�..
int CTextureManager::DeleteMaterial( LPDIRECT3DTEXTURE9 pTexture )
{
	int		i;

	if( pTexture == NULL )	return FALSE;
	if( m_nMaxTexture == 0 )	return FALSE;

	for( i = 0; i < MAX_MATERIAL; i ++ )
	{
		if( m_pMaterial[i].m_bActive )
		{
			if( m_pMaterial[i].m_pTexture == pTexture )		// pTexture�� ã�Ҵ�.
			{
				if( m_pMaterial[i].m_nUseCnt == 1 )			// �����Ȱ� �ƴϴ�(usecnt == 1)
				{
					SAFE_RELEASE( m_pMaterial[i].m_pTexture );	// ����.
					m_pMaterial[i].m_bActive = FALSE;			// �ؽ��� �����ڿ����� ����.
					m_pMaterial[i].strBitMapFileName[0] = 0;
					m_nMaxTexture --;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

MATERIAL*	CTextureManager :: AddMaterial( LPDIRECT3DDEVICE9 pd3dDevice, D3DMATERIAL9 *pMaterial, LPCTSTR strFileName, LPCTSTR szPath )
{
#ifdef	__WORLDSERVER
	return NULL;
#endif

	int		i;
	MATERIAL	*pMList = m_pMaterial;
	LPDIRECT3DTEXTURE9      pTexture = NULL;

	// �̹� �������� �˻�.
	for( i = 0; i < MAX_MATERIAL; i ++ )
	{
		if( pMList->m_bActive )
		{
			if( strcmpi(strFileName, pMList->strBitMapFileName) == 0 && pMList->m_pd3dDevice == pd3dDevice )	// �̹� ������ �ٽ� ���� ����.  ���� ���� -_-;;
			{
				pMList->m_nUseCnt ++;	// �̷̹ε��Ѱ� �����ϰ� �ִٸ� ī��Ʈ �ø�.
				return pMList;
			}
		}
		pMList ++;
	}
	pMList = NULL;

	CString strPath;
	if( szPath == NULL )
	{
	 #ifdef __WORLDSERVER
		strPath = MakePath( DIR_MODELTEX, strFileName );
	 #else
		if( g_Option.m_nTextureQuality == 0 )
			strPath = MakePath( DIR_MODELTEX, strFileName );
		else
		if( g_Option.m_nTextureQuality == 1 )
			strPath = MakePath( DIR_MODELTEXMID, strFileName );
		else
			strPath = MakePath( DIR_MODELTEXLOW, strFileName );
	 #endif // not worldserver
	}
	else
	{
		strPath = MakePath( szPath, strFileName );		// ��ΰ� �����Ǿ� ������ �װɾ���.
	}
	
	if( FAILED( LoadTextureFromRes( pd3dDevice, strPath, 
			  D3DX_DEFAULT, D3DX_DEFAULT, 4, 0, D3DFMT_UNKNOWN, //D3DFMT_A4R4G4B4, 
			  D3DPOOL_MANAGED,  D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR              , 
			   D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR               , 0x00000000, NULL, NULL, &pTexture ) ) )

	{
		if( !IsEmpty(strFileName) )
			Error( "%s texture bitmap", strPath );
	}

	// �� ������ �ִ��� �˻�.
	pMList = m_pMaterial;
	for( i = 0; i < MAX_MATERIAL; i ++ )
	{
		if( pMList->m_bActive == FALSE )	break;
		pMList++;
	}
	if( i >= MAX_MATERIAL )
	{
		LPCTSTR szErr = Error( "CTextureManager::AddMaterial : �ؽ��� ������ �Ѿ��." );
		//ADDERRORMSG( szErr );
		return NULL;
	}

	pMList->m_bActive = TRUE;
#ifndef __CHERRY
	pMaterial->Ambient.r = 1;
	pMaterial->Ambient.g = 1;
	pMaterial->Ambient.b = 1;
	pMaterial->Diffuse.r = 1;
	pMaterial->Diffuse.g = 1;
	pMaterial->Diffuse.b = 1;
	pMaterial->Specular.r = 1;
	pMaterial->Specular.g = 1;
	pMaterial->Specular.b = 1;
	pMaterial->Emissive.r = 0;
	pMaterial->Emissive.g = 0;
	pMaterial->Emissive.b = 0;
	pMaterial->Power = 0.0f;
#endif
	pMList->m_Material = *pMaterial;
//	memcpy( &pMList->m_Material, pMaterial, sizeof(D3DMATERIAL9) );				// ���͸��󳻿� ī��
//	memcpy( pMList->strBitMapFileName, strFileName, strlen(strFileName) );		// �ؽ��� ���ϸ� ī��
#ifdef	_XDEBUG
	if( strlen(strFileName)+1 > 32 )
		Error( "CTextureManager::AddMaterial() : %s�� ���̰� �ʹ� ���", strFilename );
#endif
	strcpy( pMList->strBitMapFileName, strFileName );		// �ؽ��� ���ϸ� ī��
	pMList->m_pTexture = pTexture;
	pMList->m_pd3dDevice = pd3dDevice;
	pMList->m_nUseCnt = 1;	// ó�� ��ϵȰ��̱ⶫ�� 1���� ����.
	m_nMaxTexture ++;


	return pMList;
}
D3DMATERIAL9* CTextureManager::GetMaterial( LPDIRECT3DDEVICE9 m_pd3dDevice, int nIdx )
{
	return &m_pMaterial[ nIdx ].m_Material; 
}
LPDIRECT3DTEXTURE9	CTextureManager::GetTexture( LPDIRECT3DDEVICE9 m_pd3dDevice, int nIdx )
{
	return m_pMaterial[ nIdx ].m_pTexture; 
}

