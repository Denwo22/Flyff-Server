#ifndef __SFXBASE_H
#define __SFXBASE_H

class CModel;

#define DEGREETORADIAN(x) ((2.0f*D3DX_PI*(x))/(360.0f))

class CSfxTexture;
class CSfxMeshMng;
class CSfxMng; 
class CSfxObjMng;
extern CSfxMng g_SfxMng; // SFX ���� ������
extern CSfxObjMng g_SfxObjMng; // ���� ������ SFX ������Ʈ ������ (���ӿ����� CModelMng�� ����ϹǷ� �ʿ����)
extern CSfxTexture g_SfxTex; // SFX�� ���Ǵ� �ؽ��ĸ� �����ϴ� ������
extern CSfxMeshMng g_SfxMeshMng; // SFX�� ���Ǵ� �޽��� �����ϴ� ������

enum SFXPARTTYPE {
	SFXPARTTYPE_BILL=1,
	SFXPARTTYPE_PARTICLE=2,
	SFXPARTTYPE_MESH=3,
	SFXPARTTYPE_CUSTOMMESH=4,
};
enum SFXPARTBILLTYPE {
	SFXPARTBILLTYPE_BILL=1,
	SFXPARTBILLTYPE_BOTTOM=2,
	SFXPARTBILLTYPE_POLE=3,
	SFXPARTBILLTYPE_NORMAL=4,
};
enum SFXPARTALPHATYPE {
	SFXPARTALPHATYPE_BLEND=1,
	SFXPARTALPHATYPE_GLOW=2,
};

//
// �ϳ��� SFX�� SFX������ ������. 
// 
//
//
//
//

#define D3DFVF_D3DSFXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
#ifndef DIR_SFX
	#define DIR_SFX     _T( "SFX\\"  )
#endif 

struct D3DSFXVERTEX // SFX�� ���Ǵ� ���ؽ� ����
{
	D3DXVECTOR3 p;
	FLOAT       tu1, tv1;
};

struct SfxKeyFrame
{
	WORD nFrame;       // Ű �������� ��ġ (������)
	D3DXVECTOR3 vPos;  // ��ġ
	D3DXVECTOR3 vPosRotate; // ���� ������ �߽����� �� �࿡ ���� ��ġȸ��
	D3DXVECTOR3 vScale; // ũ��
	D3DXVECTOR3 vRotate; // ȸ��
	int nAlpha;
}; // Ű������ ����ü

struct Particle
{
	WORD nFrame;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSpeed;
	D3DXVECTOR3 vScale;
//#ifdef __ATEST
	D3DXVECTOR3 vRotation;
	D3DXVECTOR3 vScaleStart;
	D3DXVECTOR3 vScaleEnd;
	D3DXVECTOR3 vScaleSpeed;
	BOOL        bSwScal;
//#endif	
}; // ��ƼŬ ����ü

class CSfxPart
{
public:
//#ifdef __ATEST
	CString m_strName;  // ���̾� �̸�...
//#endif
	CString m_strTex; // �ؽ��� ���ϸ�
	SFXPARTTYPE m_nType; // �� ��Ʈ�� ���ϴ� ��Ʈ�ΰ�
	SFXPARTBILLTYPE m_nBillType; // �� ��Ʈ�� �������ΰ� �ٴ��ΰ�
	SFXPARTALPHATYPE m_nAlphaType; // ������ �뵵
	WORD m_nTexFrame; // �ؽ��� �ִϸ��̼� ������ �� (���)
	WORD m_nTexLoop;  // �ؽ��� �ִϸ��̼� �ѹ��� ���µ��� �ɸ��� ������ �� (�ð�)
	BOOL m_bUseing;

	CPtrArray m_apKeyFrames; // Ű������ �迭
	SfxKeyFrame* Key(BYTE nIndex) { // Ű������
		if(nIndex>=m_apKeyFrames.GetSize()) return NULL;
		return (SfxKeyFrame*)(m_apKeyFrames[nIndex]);
	}
	SfxKeyFrame* KeyByFrame(WORD nFrame) { // Ư�� �������� Ű������
		SfxKeyFrame* pKey;
		for(int i=0;i<m_apKeyFrames.GetSize();i++) {
			pKey=Key((BYTE)i);
			if(pKey->nFrame==nFrame) return pKey;
		}
		return NULL;
	}

	CSfxPart();
	virtual ~CSfxPart();
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // ����
#endif
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) ); // ����

	void DeleteAllKeyFrame();
	void AddKeyFrame(WORD nFrame); // Ű������ �߰�
	void DeleteKeyFrame(WORD nFrame); // Ű������ ����
	SfxKeyFrame* GetPrevKey(WORD nFrame); // �־��� �����Ӻ��� ���� Ű�������� ����´�
	SfxKeyFrame* GetNextKey(WORD nFrame, BOOL bSkip=TRUE); // �־��� �����Ӻ��� ���� Ű�������� ����´�
	virtual void Load(CResFile& file); // �ε�
	virtual void OldLoad(CResFile& file); // �������� �ε�
	virtual void Load2(CResFile& file); 
	virtual void Load3(CResFile& file) {}; // �ֽŹ���
	
	void GetKey(WORD nFrame,SfxKeyFrame* pKey); // Ư���������� ���������̼ǵ� Ű���� ����´�

	void AdjustKeyFrame(WORD nFrame, SfxKeyFrame& key); // Ư�� �������� Ű�������� ������ ����
};

class CSfxPartBill : public CSfxPart
{
public:
	CSfxPartBill();
	~CSfxPartBill() {}
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void OldLoad(CResFile& file);
};

class CSfxPartParticle: public CSfxPart
{
public:
	WORD m_nParticleCreate;     // ������Ű�� ����(������)
	WORD m_nParticleCreateNum;     // ������Ű�� ����(������)

	// ��ƼŬ �� ���� ����������Ŭ
	WORD m_nParticleFrameAppear; // ��Ÿ���µ� �ɸ��� �ð�(������)
	WORD m_nParticleFrameKeep; // �����ð�(������)
	WORD m_nParticleFrameDisappear; // ������µ� �ɸ��� �ð�(������)

	/// ���� �ʱ�ġ�� �������� �ּҰ� ������ �������̴�.
	FLOAT m_fParticleStartPosVar; // ������ġ XZ���� (�ݰ�) �������� ��ġ�� �߽����� XZ���󿡼� �� ����ŭ ������ ������ ��ġ���� ����
	FLOAT m_fParticleStartPosVarY; // ������ġ Y ����. �������� Y��ǥ���� �� �� ������ ������ ��ġ���� ����
	FLOAT m_fParticleYLow; // �����ӵ� �ʱ�ġ ������
	FLOAT m_fParticleYHigh; // �����ӵ� �ʱ�ġ �ִ밪
	FLOAT m_fParticleXZLow; // ����ӵ� �ʱ�ġ ������
	FLOAT m_fParticleXZHigh; // ����ӵ� �ʱ�ġ �ִ밪

	D3DXVECTOR3 m_vParticleAccel;  // ���ӵ� ����... �����Ӹ��� �ӵ��� �������� ��

	D3DXVECTOR3 m_vScale; // �ʱ� ũ��
	D3DXVECTOR3 m_vScaleSpeed; // �����Ӹ��� ũ�⿡ �������� ��
	D3DXVECTOR3 m_vRotation; 
	D3DXVECTOR3 m_vRotationLow; 
	D3DXVECTOR3 m_vRotationHigh; 

//#ifdef __ATEST
	BOOL		m_bRepeatScal;
	FLOAT		m_fScalSpeedXLow;
	FLOAT		m_fScalSpeedXHigh;
	FLOAT		m_fScalSpeedYLow;
	FLOAT		m_fScalSpeedYHigh;
	FLOAT		m_fScalSpeedZLow;
	FLOAT		m_fScalSpeedZHigh;
	D3DXVECTOR3 m_vScaleSpeed2;
	D3DXVECTOR3 m_vScaleEnd;
//#endif
	
	
	BOOL        m_bRepeat;
	
	
	CSfxPartParticle();
	~CSfxPartParticle();

#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  ); // ��ƼŬ�� ��� �� �����Լ��� �����Ϳ����� ���ȴ�. �ٸ� ��Ʈ�� ����, ������ ��� ���.
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file);
	virtual void OldLoad(CResFile& file);
};
class CSfxPartMesh: public CSfxPart
{
public:
	CSfxPartMesh();
	~CSfxPartMesh();
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file) {};
	virtual void OldLoad(CResFile& file);
};
class CSfxPartCustomMesh: public CSfxPart
{
public:
	int m_nPoints;
	CSfxPartCustomMesh();
	~CSfxPartCustomMesh();
#ifndef __WORLDSERVER
	virtual void Render( D3DXVECTOR3 vPos, WORD nFrame, FLOAT fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
#endif
	virtual void Render2( D3DXVECTOR3 vPos, WORD nFrame, D3DXVECTOR3 fAngle, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f )  );
	virtual void Load(CResFile& file);
	virtual void Load2(CResFile& file);
	virtual void Load3(CResFile& file) {};
	virtual void OldLoad(CResFile& file);
};

// SFX�� ����. �ε��ϸ� SFX���� �����ص״ٰ� ���ӻ� ������Ʈ�� ���� ����.
class CSfxBase
{
public:
	BOOL    LoadMerge();
	CString m_strName; // sfx�� �̸�. ���ϸ��� Ȯ���ڸ� ������ ��

	CPtrArray m_apParts; // ��Ʈ�� �迭
	CSfxPart* Part(BYTE nIndex) { // ������ ��Ʈ�� �����͸� �����´�. ������ ����� NULL�� ����
		if(m_apParts.GetSize()>nIndex) return (CSfxPart*)(m_apParts[nIndex]);
		return NULL;
	}

	CSfxBase();
	~CSfxBase();

	CSfxPart* AddPart(SFXPARTTYPE nType); // ��Ʈ �߰�
	void DeletePart(BYTE nIndex); // ��Ʈ ����
	void AdjustPart(BYTE nIndex); // ��Ʈ ����

	BOOL Load(void); // �ε�
};

// CSfxBase ������
class CSfxMng
{
public:
	static LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d ����̽� ������
	static LPDIRECT3DVERTEXBUFFER9 m_pSfxVB; // �� ����� ���ؽ� ����

	FLOAT m_fScale; // sfx�� ũ��

	CPtrArray m_apSfxBase; // SfxBase�� �迭

	CSfxMng() {	m_fScale=0.5f; }
	~CSfxMng();

	void AddSfxBase(CSfxBase* pSfxBase); // SfxBase�� �߰�
	void DeleteSfxBase(BYTE nIndex); // ������ SfxBase�� ����
	CSfxBase* GetSfxBase(BYTE nIndex); // ������ SfxBase�� �����͸� ����´�.
	CSfxBase* GetSfxBase(CString strSfxName); // SfxBase�� �̸����� ������ SfxBase�� �����͸� ����´�.

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

// ���ӻ� ������Ʈ���� SFX�� ����� ���� CModel ������Ʈ
class CSfxModel : public CModel
{
public:

	WORD m_nSfxBase; //
	CSfxBase* m_pSfxBase;

	D3DXMATRIX  m_matScale;
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vRotate;
	D3DXVECTOR3 m_vScale;
	WORD m_nCurFrame;

	CPtrArray m_apParticles;

	CSfxModel();
	~CSfxModel();
	void DeleteAll(void);
	void RemovePart( int nIndex );
	void SetSfx(CSfxBase* pSfxBase);
	void SetSfx(DWORD dwIndex);	
	void SetSfx(LPCTSTR strSfxName);
#ifndef __WORLDSERVER	
	virtual BOOL Render( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	BOOL RenderZ( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles( D3DXVECTOR3 vPos,WORD nFrame,FLOAT fAngle,CSfxPartParticle* pPartParticle,CPtrArray* pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
#endif
	virtual BOOL Render2( LPDIRECT3DDEVICE9 pd3dDevice, const D3DXMATRIX* pmWorld = NULL );
	void RenderParticles2( D3DXVECTOR3 vPos,WORD nFrame,D3DXVECTOR3 fAngle,CSfxPartParticle* pPartParticle,CPtrArray* pParticles, D3DXVECTOR3 vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	//void Render(void);
	BOOL Process(void);
	BOOL SetFrame( int nFrame );
#ifdef __CLIENT
#ifndef __VM_0820
#ifndef __MEM_TRACE
	static MemPooler<CSfxModel>*	m_pPool;
	void*	operator new( size_t nSize )	{	return CSfxModel::m_pPool->Alloc();	}
	void*	operator new( size_t nSize, LPCSTR lpszFileName, int nLine )	{	return CSfxModel::m_pPool->Alloc();	}
	void	operator delete( void* lpMem )	{	CSfxModel::m_pPool->Free( (CSfxModel*)lpMem );	}
	void	operator delete( void* lpMem, LPCSTR lpszFileName, int nLine )	{	CSfxModel::m_pPool->Free( (CSfxModel*)lpMem );	}
#endif	// __MEM_TRACE
#endif	// __VM_0820
#endif	// __CLIENT
};

// ���ӻ� ���� SFX������Ʈ���� ������. (������ ���µ� �ٸ��ɷ� �����ϹǷ� �����δ� �Ⱦ���)
class CSfxObjMng
{
public:
	CPtrArray m_apSfxObj; // ���ӻ� SFX������Ʈ���� �迭

	CSfxObjMng();
	~CSfxObjMng();
	void AddObj(CSfxBase *pSfxBase,D3DXVECTOR3 vPos=D3DXVECTOR3(0,0,0),D3DXVECTOR3 vRotate=D3DXVECTOR3(0,0,0)); // 
	void Process(void);
#ifndef __WORLDSERVER
	void Render(void);
#endif
	void RemoveAll(void); // ������Ҹ� ���� �ı��Ѵ�
};

// SFX������ ���Ǵ� �ؽ��� ������
class CSfxTexture
{
public:
	CMapStringToPtr m_apTexture; // ��Ʈ������ �ؽ��� �����͸� �����ϴ� �ؽ����̺�

	CSfxTexture();
	~CSfxTexture();

	LPDIRECT3DTEXTURE9 AddTex(CString str); // �ؽ��� �߰�
	void DeleteTex(CString str); // �ؽ��� ����
	LPDIRECT3DTEXTURE9 Tex(CString str); //  ������ �̸��� �ؽ��� �����͸� �����ش�
	void DeleteAll(void); // ��� �ؽ��� ����
};

class CSfxMeshMng
{
public:
	LPDIRECT3DDEVICE9 m_pd3dDevice; // d3d����̽� ������
	CMapStringToPtr m_apMesh; // ��Ʈ������ �޽� �����͸� �����ϴ� �ؽ����̺�

	CSfxMeshMng();
	~CSfxMeshMng();

	CModelObject* AddMesh(CString str);// �޽� �߰�
	void DeleteMesh(CString str); // �޽� ����
	CModelObject* Mesh(CString str); //  ������ �̸��� �޽� �����͸� �����ش�
	void DeleteAll(void); // ���� ����

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
};

#ifdef __BS_EFFECT_LUA

void open_lua_sfx();
void close_lua_sfx();

void run_lua_sfx( int nState, OBJID caller, const char* szMoverName );

//for lua glue
struct lua_State;
static int call_sfx( lua_State* L );
static BOOL stop_sfx( OBJID caller );

// ��ü update 
struct SfxModelSet
{
	SfxModelSet( ) : _idMaster(0), _pModel(NULL), _bLoop(FALSE), _nMaxFrame(0), _nState(0) { }
	SfxModelSet( const OBJID idMaster, const char* szSfxName, const char* szBoneName, BOOL bLoop, int _nState );
	
	~SfxModelSet( );

	BOOL Update( );
	BOOL Render( LPDIRECT3DDEVICE9 pd3dDevice );
	
	OBJID _idMaster;
	char _szBone[ 64 ];
	char _szFileName[ 64 ];
	int _nMaxFrame;
	BOOL _bLoop;
	CSfxModel* _pModel;

	int _nState;		//���� : ������϶� �ߵ��Ǵ� ȿ������

};

typedef vector< SfxModelSet* >					SMSVector;				//SMS : SfxModelSet
typedef map< OBJID, SMSVector >					SfxModelSetContainer;
typedef SfxModelSetContainer::iterator			SfxModelSetIter;

//gmpbigsun(100128 ) : Lua���� ȣ���� sfx( ��� �Ӽ��� data�� �����ϴ� ) ������
// bone �� ��ũ�Ǿ� loop�Ǵ� ���常 �����Ѵ�. 
class CSfxModelMng 
{
public:
	CSfxModelMng( );
	~CSfxModelMng( );

	BOOL AddData( SfxModelSet* pData, BOOL bChecked );				
	BOOL SubData( OBJID objID );						//�ش� ������Ʈ�� ��� lua���� sfx���� 
	BOOL SubData( OBJID objID, const int nState );		//�ش� ������Ʈ�� ��� ���¿� �ش��ϴ� sfx����
	BOOL SubData( OBJID objID, const char* szBone );	//�ش� ������Ʈ�� �ش� ���� ��ũ�� ��� sfx����
	
	void Update( );
	void Render( LPDIRECT3DDEVICE9 pd3dDevice );

	BOOL IsFull( OBJID objID, const char* szSfx, const char* szBone );	//optimized 

	static CSfxModelMng* GetThis();
	static void Free();

public:
	map< DWORD, int > _cWaitingObj;

private:
	static CSfxModelMng* _pThis;

	SfxModelSetContainer	_cDatas;
};


#endif //__BS_EFFECT_LUA

#endif
