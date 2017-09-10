#ifndef __GUILDRENDER_H__
#define __GUILDRENDER_H__

#define MAX_GUILD_TEXTURE	256			// ������ �ѱ���(256�� ������ �ִ������ ��ü����)

// 64k�� ���� �ʵ��� ����.
struct GUILD_TEXTURE
{
	DWORD	idGuild;						// ��� ���̵�
	LPDIRECT3DTEXTURE9	lpCloakTexture;		// ���� �ؽ���
	//LPDIRECT3DTEXTURE9	lpIconTexture;	// ICON �ؽ���
	DWORD	tmLoad;							// �޸𸮿� �ε��� �ð�. �ð��� ������ ������.
};

class CGuildTexture
{
	int m_nMax;
	GUILD_TEXTURE	m_Texture[ MAX_GUILD_TEXTURE ];		// �ؽ��� �޸� Ǯ.  �����ս� �������� ����Ʈ ������ �ٲ���.
public:
	CGuildTexture();
	~CGuildTexture();
	
	// ����ȣ�� �޾Ƽ� CT+����ȣ.JPG������ �ε��Ѵ�. �ε��� �ؽ�ó�����͸� ����.
	LPDIRECT3DTEXTURE9	LoadGuildTexture( DWORD idGuild );		
};

extern CGuildTexture	g_GuildTexture;


#endif // __GUILDRENDER_H__