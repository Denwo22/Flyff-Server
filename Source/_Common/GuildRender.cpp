#include "stdafx.h"

#include "GuildRender.h"


#ifndef __WORLDSERVER
#ifdef __XCLOAK

//
//
//
CGuildTexture	g_GuildTexture;

//
//
//
CGuildTexture::CGuildTexture()
{
	m_nMax = 0;
	memset( m_Texture, 0, sizeof(m_Texture) );
//	m_pTextures = NULL;		// ���� ��ġ ����.
//	m_pFree = NULL;
}

CGuildTexture::~CGuildTexture()
{
	// DeleteDevicesObject();		// �ؽ��� ����̽� ����
	m_nMax = 0;
	memset( m_Texture, 0, sizeof(m_Texture) );
//	m_pTextures = NULL;		// ���� ��ġ ����.
//	m_pFree = NULL;
}

//
//
//
LPDIRECT3DTEXTURE9 CGuildTexture::LoadGuildTexture( DWORD idGuild )
{
#ifndef _DEBUG
	return NULL;		// ������ �ö󰥶� �� �� ����.
#endif
	int	i;
	GUILD_TEXTURE *pList = m_Texture, *pNode;
	MATERIAL	*pMtrl;
	D3DMATERIAL9	mMaterial;
	char	szName[128];

	memset( szName, 0, sizeof(szName) );

	for( i = 0; i < MAX_GUILD_TEXTURE; i ++ )
	{
		pNode = pList++;
		if( pNode->idGuild == 0 )	continue;
		if( pNode->idGuild == idGuild )		// �̹� �ε��Ѱ� ������ �ϸ� 
			return pNode->lpCloakTexture;
	}

	pList = m_Texture;
	for( i = 0; i < MAX_GUILD_TEXTURE; i ++ )
	{
		pNode = pList++;
		if( pNode->idGuild )		continue;
		// idGuild �� 0�ΰ͸� ����ִ� ����.
		pNode->idGuild = idGuild;

		// CT + ����ȣ + .jpg �� �ؽ��� �̸� ����/�ε�
		sprintf( szName, "CT%06d.jpg", idGuild );
		pMtrl = g_TextureMng.AddMaterial( D3DDEVICE, &mMaterial, szName, DIR_LOGO );

		pNode->lpCloakTexture = pMtrl->m_pTexture;
		pNode->tmLoad = timeGetTime();			// �ε��� �ð� ���.
		return pNode->lpCloakTexture;
	}

	LPCTSTR szErr = Error( "CGuildTexture::LoadGuildTexture : ������� ����" );
	//ADDERRORMSG( szErr );

	return NULL;
}






/*
	. ������ �����ؽ��� jpg�� Ȩ�ǿ� ���.
	. �ؽ��ĸ� CTxxx.jpg ���·� ��ȯ �� �ҽ��������� ������ ������ ���.  - �����ϵ��� ���߿� merge�ȴ�.
	. xxx�� �����̵�� 1:1�����ȴ� �����̵��� ������ �����ȴ�.
	. "CT" + �����̵� + ".jpg"�� ���·� ������ �ε��ȴ�.
	. ���������� DB���� ���� �����ϴ� ����� ��ȣ����Ʈ�� �ؽ�Ʈ ���Ϸ� �޾Ƽ� �̸� ���� ������ ���� ���ϵ� ���������ش�.
	. ��� ����� ������ �����ϸ� "����"�� �ϸ� ���� ���ҵǰ� �ΰ������� ���� ��¥���䰡 �κ��� ����.
	. �� ����� Ÿ�ο��� �ټ��� �ִ�. �׷��� ��尡 �����Ǹ� ���䵵 ��¥����� ���ƿ´�.

*/






#endif
#endif // not WORLDSERVER