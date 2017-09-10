#ifndef __SKYBOX_H_2002_5_14
#define __SKYBOX_H_2002_5_14

#include <D3D9.h>
#include <basetsd.h> 

class CWorld;

#define D3DFVF_D3DSKYBOXVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_D3DSUNVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct D3DSKYBOXVERTEX
{
	D3DXVECTOR3 p;
	DWORD       c;
	FLOAT       tu1, tv1;
};

struct D3DSUNVERTEX
{
	D3DXVECTOR3 p;
	DWORD       c;
	FLOAT       tu1, tv1;
};

//#define WEATHER_NONE 0 
//#define WEATHER_SLOW 1 
//#define WEATHER_RAIN 2 
class CSkyBox
{
	CTimer m_timerWeather;
	int m_nWeather;
	void	DrawRain(LPDIRECT3DDEVICE9 pd3dDevice);
	void	DrawSnow(LPDIRECT3DDEVICE9 pd3dDevice);
	
public:
	enum SKY_TYPE
	{
		SKY_SIDE,
		SKY_CLOUD,
		SKY_MOON,
		SKY_SUN,
	};

	// ���� �ؽ���
	LPDIRECT3DTEXTURE9 m_pMoonTexture;
	LPDIRECT3DTEXTURE9 m_pSkyBoxTexture;
	LPDIRECT3DTEXTURE9 m_pSkyBoxTexture2;
	LPDIRECT3DTEXTURE9 m_pSkyBoxTexture3;
	LPDIRECT3DTEXTURE9 m_pCloudTexture;
	LPDIRECT3DTEXTURE9 m_pCloudTexture2;
	LPDIRECT3DTEXTURE9 m_pCloudTexture3;
	LPDIRECT3DTEXTURE9 m_pSunTexture;
	LPDIRECT3DTEXTURE9 m_pSunTexture2;
	LPDIRECT3DTEXTURE9 m_pSnowTexture;
	LPDIRECT3DTEXTURE9 m_pLensTexture[8];
	LPDIRECT3DVERTEXBUFFER9 m_pTopSkyBoxVB;
	LPDIRECT3DVERTEXBUFFER9 m_pSideSkyBoxVB;
	LPDIRECT3DVERTEXBUFFER9 m_pSunVB;
	LPDIRECT3DVERTEXBUFFER9 m_pLensFlareVB[8];
	LPDIRECT3DVERTEXBUFFER9 m_pRainVB;
	LPDIRECT3DVERTEXBUFFER9 m_pSnowVB;
	LPDIRECT3DVERTEXBUFFER9 m_pStarVB;
	CWorld*        m_pWorld       ; // �ڽ��� ���� ������ �����͸� ���´�. 

	D3DXVECTOR3 m_vVelocity[2000]; // �������� �ӵ� 
	D3DXVECTOR3 m_vFall[2000]; // �������� ��ġ �迭
	//D3DXVECTOR3 m_vSnow[2000]; // �����̿� ��ġ �迭
//	D3DXVECTOR3 m_vStar[1000];

	D3DXMATRIX m_matStar[2000]; // ���� ��ġ �迭
	DWORD m_nFall; // ���� ����� ����
//	DWORD m_nSnow; // ���� ������ ����
//	DWORD m_nStar; // ���� �� ����

	BOOL m_bOldSkyBox;

	float m_fCloudx,m_fCloudy; // �귯���� ������ ��ǥ
	float m_fSunAngle; // ���� �¾��� ����

#if __VER >= 15 // __BS_CHANGING_ENVIR
	BOOL m_bLockWeather; //gmpbigsun :  ���� ����flag (��������� ������ ������ ��� �����ε� ������ �ʴ´�)
#endif

	CSkyBox();
	~CSkyBox();

#ifdef __ENVIRONMENT_EFFECT
	static	CSkyBox*	GetInstance();
#endif // __ENVIRONMENT_EFFECT

	void SetWeather( int nWeather, BOOL bOnOff );		

	void InitFall();

	void Free();
	void SetVertices();
	void SetStarVertices();

	// ó�� �� ��� 
	void    Process();
	void    Render(CWorld* pWorld, LPDIRECT3DDEVICE9 pd3dDevice);
	void	DrawLensFlare(LPDIRECT3DDEVICE9 pd3dDevice);
	void    RenderFall( LPDIRECT3DDEVICE9 pd3dDevice );
		
	BOOL CheckSun(LPDIRECT3DDEVICE9 pd3dDevice);

	int GetWeather( )	{ return m_nWeather; }

	IDirect3DTexture9* GetCurrSkyTexture( SKY_TYPE eType );

	// Direct3D ���� ������Ʈ �ʱ�ȭ�� ����, �缳�� ���� 
	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();
};

#endif