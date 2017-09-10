#ifndef PATCH_H
#define PATCH_H

class CLandscape;


// ��ġ Ŭ����
// ���彺�������� �����ϴ� ������ Ÿ��

class CPatch
{
protected:
	FLOAT *m_pHeightMap;			// ���� �� (�� ��ġ�� ���� �ּ�)

	BOOL m_bDirty;					// ���ؽ� ���۸� ������ �ʿ䰡 ���� ��� TRUE�� ��Ʈ

	BOOL m_bVisible;				// �ø��� ���
	D3DXVECTOR3  m_avBounds[8];		// �ø��� ����� �ٿ�� �ڽ� ����
	D3DXPLANE    m_aplaneBounds[6];	// �ø��� ����� �ٿ�� �ڽ� ���

	D3DXVECTOR3 m_vCenter;          // �ø��� ����� �� ��ġ�� �߽�

public:
	int m_nWorldX,m_nWorldY;		// �� ��ġ�� ���ϴ� ������ǥ

	int m_nLevel;           // �� ��ġ�� LOD ����
	int m_nTopLevel;        // �� ��ġ ��ܺ��� LOD ����
	int m_nLeftLevel;       // �� ��ġ �´ܺ��� LOD ����
	int m_nRightLevel;      // �� ��ġ ��ܺ��� LOD ����
	int m_nBottomLevel;     // �� ��ġ �ϴܺ��� LOD ����

	CPatch() { m_pHeightMap = NULL; m_nBottomLevel = m_nRightLevel = m_nLeftLevel = m_nTopLevel = m_nLevel = 0; }
	~CPatch();

//	void SetExPatch(BOOL bEnable); // �̰� ���� �Ⱦ���. ���� ��� �߰��ߴٰ� ��������.

	void Init( int heightX, int heightY, int worldX, int worldY, FLOAT *hMap ); // ��ġ �ʱ�ȭ. (��ü ���忡���� ��ġ�� �� ��ġ�� ����� ���� ���� �ּҸ� �����Ѵ�.)
	BOOL isVisibile( ) { return m_bVisible; } // �ø��� ����� �����ش�.
	BOOL isDirty( ) { return m_bDirty; } // ����Ʈ���� ���� ���� ����� ��� ���ؽ� ����, ����Ʈ���� �����Ѵ�.
	void SetDirty( BOOL bDirty ) { m_bDirty=bDirty; }
	void Render(LPDIRECT3DDEVICE9 pd3dDevice,int X,int Y);

	void CalculateBound();
	void Cull();

	void CalculateLevel();
};

#endif
