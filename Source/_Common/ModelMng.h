#if !defined(AFX_MODELMNG_H__25FE7788_56AD_4D50_8D9E_E69C969DE0F9__INCLUDED_)
#define AFX_MODELMNG_H__25FE7788_56AD_4D50_8D9E_E69C969DE0F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DRender.h"

typedef struct tagMODELELEM
{
	DWORD m_dwType;
	DWORD m_dwIndex;
	TCHAR m_szName [48];
	int		m_nMax;
	TCHAR* m_apszMotion;
	DWORD m_dwModelType;
	TCHAR m_szPart[48]; 
	FLOAT m_fScale;
	LPDIRECT3DTEXTURE9 m_pTexture;	// 4����Ʈ ������ ���ؼ� �ű�.-xuzhu-
	int m_nTextureEx;			// �ؽ��� �� ��ȣ 0 ~ 7

	BYTE  m_bMark;
	BYTE  m_bFly      : 1;
	BYTE  m_dwDistant : 2;
	BYTE  m_bPick     : 1; // ĳ���� �̵��� ���콺 ��ŷ�� �ɸ��� ������Ʈ�ΰ�?
	BYTE  m_bUsed     : 1;
	BYTE  m_bTrans    : 1; // ĳ���͸� ���� �� �������� �Ǵ� ������Ʈ�ΰ�?
	BYTE  m_bShadow   : 1;
	BYTE  m_bReserved : 1;
	BYTE m_bRenderFlag : 1;

	TCHAR* GetMotion( int i ) 
	{ 
		if( i < 0 || i >= m_nMax )
		{
			Error( "tagMODELELEM : out of range %d", i );
			i = 0;
		}

		if( m_apszMotion ) 
			return &m_apszMotion[ i * 32 ]; 
		return NULL; 
	}

}* LPMODELELEM, MODELELEM;

typedef map< string, void* > CMapStrToPtr;
typedef CMapStrToPtr::value_type MapStrToPtrType;
typedef CMapStrToPtr::iterator MapStrToPtrItor;

class CModel; 

class CModelMng 
{
public:
	CMapStrToPtr m_mapFileToMesh;
	CFixedArray< tagMODELELEM > m_aaModelElem[ MAX_OBJTYPE ]; 

public:
	CModelMng();
	~CModelMng();

	void Free();	
	LPMODELELEM GetModelElem( DWORD dwType, DWORD dwIndex );
	void MakeBoneName( TCHAR* pszModelName, DWORD dwType, DWORD dwIndex );
	void MakeModelName( TCHAR* pszModelName, DWORD dwType, DWORD dwIndex );
	void MakeMotionName( TCHAR* pszMotionName, DWORD dwType, DWORD dwIndex, DWORD dwMotion );
	void MakePartsName( TCHAR* pszPartsName, LPCTSTR lpszRootName, DWORD dwIndex, int nSex = SEX_SEXLESS );

	BOOL    LoadMotion( CModel* pModel, DWORD dwType, DWORD dwIndex, DWORD dwMotion );
	CModel* LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, int nType, int nIndex, BOOL bParts = FALSE );
	CModel* LoadModel( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* lpszFileName, LPMODELELEM lpModelElem, int nType, BOOL bParts = FALSE ); //int nModelType, LPCTSTR lpszTexture = NULL );

	BOOL LoadScript( LPCTSTR lpszFileName );

	// dx �ʱ�ȭ ���� 
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
	HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELMNG_H__25FE7788_56AD_4D50_8D9E_E69C969DE0F9__INCLUDED_)






