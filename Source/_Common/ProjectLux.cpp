// Project.cpp: implementation of the CProject class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "defineItem.h"
#include "lang.h"

#include "Ship.h"
#ifdef __WORLDSERVER
#include "worldmng.h"
extern	CWorldMng	g_WorldMng;
#endif	// __WORLDSERVER

#include "guild.h"


#ifdef __WORLDSERVER
#include "guildquest.h"
#endif	// __WORLDSERVER

#define SYNTAX_ERROR		Error( "%s(%d) : MoverID=%d syntax error %s", szFileName, script.GetLineNum(), nVal, script.token );
#define UNIDENTIFY_ERROR	Error( "%s(%d) : MoverID=%d �������� ���� ������ error %s", szFileName, script.GetLineNum(), nVal, script.token );
#define CUSTOM_ERROR(A)		Error( "%s(%d) : MoverID=%d %s %s", szFileName, script.GetLineNum(), nVal, A, script.token );		

enum AICMD {
	AICMD_NONE = 0,
	AICMD_SCAN, 
	AICMD_ATTACK, 		// ���ݸ��.
	AICMD_ATK_HPCOND,	// hp��
	AICMD_ATK_LVCOND,	// lv��
	AICMD_RECOVERY,		// ȸ��
	AICMD_RANGEATTACK,	// ���Ÿ� ����.
	AICMD_KEEP_RANGEATTACK,	// �Ÿ����� ���Ÿ� ����.
	AICMD_SUMMON,		// ��ȯ
	AICMD_EVADE,		// ����
	AICMD_HELPER,		// �����û
	AICMD_BERSERK,		// ���л���
	AICMD_RANDOMTARGET,	// �����ѻ�뿡�� ����
	AICMD_LOOT,			// �����ϴ� ��.
};

// AI{}�� #SCAN ��.
BOOL CProject::LoadPropMoverEx_AI_SCAN( LPCTSTR szFileName, CScript &script, int nVal )
{
	AICMD nCommand = AICMD_NONE;

	script.GetToken();	// {
	if( script.token[0] != '{' )	// #SCAN �Ŀ� {�� �����°�.
	{
		Error( "%s(%d) : MoverID=%d SCAN ���ͺ� { ������.", szFileName, script.GetLineNum(), nVal );
		return FALSE;
	}

	MoverProp* pProp = GetMoverProp( nVal );
	
	while(1)
	{
		script.GetToken();		// ��ū �ϳ� ����,.
		if( script.token[0] == '}' )	// SCAN�� ��.
			break;

		if( script.tokenType == IDENTIFIER )		// ��ū�� �ĺ���.
		{
			if( nCommand == AICMD_SCAN )		// scan��ɾ ���Ծ���.
			{
				if( strcmpi( script.token, "job" ) == 0 )		// ��ĵ ���� 
					pProp->m_nScanJob = script.GetNumber();
				else 
				if( strcmpi( script.token, "range" ) == 0 )		// ��ĵ ���� 
					pProp->m_nAttackFirstRange = script.GetNumber();
				else 
				if( strcmpi( script.token, "quest" ) == 0 )	// ����Ʈ�� ���� �÷��̾� ���� 
					pProp->m_dwScanQuestId = script.GetNumber();
				else 
				if( strcmpi( script.token, "item" ) == 0 )	// �ش� �������� ���� �÷��̾� ���� 
					pProp->m_dwScanItemIdx = script.GetNumber();
				else
				if( strcmpi( script.token, "chao" ) == 0 )	// �ش� �������� ���� �÷��̾� ���� 
					pProp->m_nScanChao = script.GetNumber();
				else
				{
					// ���� �ڿ� ���� ��Ʈ���� �ƹ��͵� �ƴ϶�� ������ ���� 
					SYNTAX_ERROR;
					return FALSE;
				}
			}
			if( strcmpi( script.Token, "scan" ) == 0 )	// ��ɾ�.
			{
				if( nCommand )	// �̹� ���� ����� ���õǾ��µ� �� ���Դ�.
				{
					SYNTAX_ERROR;
					return FALSE;
				}
				nCommand =	AICMD_SCAN;	// scan���
				//pProp->m_nAttackFirstRange = 3;
			}
		}
	}

	return TRUE;
}

// AI{}�� #BATTLE ��.
BOOL CProject::LoadPropMoverEx_AI_BATTLE( LPCTSTR szFileName, CScript &script, int nVal )
{
	AICMD nCommand = AICMD_NONE;

	script.GetToken();	// {
	if( script.token[0] != '{' )	// #BATTLE �Ŀ� {�� �����°�.
	{
		Error( "%s(%d) : MoverID=%d BATTLE ���ͺ� { ������.", szFileName, script.GetLineNum(), nVal );
		return FALSE;
	}

	MoverProp* pProp = GetMoverProp( nVal );
	
	while(1)
	{
		script.GetToken();		// ��ū �ϳ� ����,.
		if( script.token[0] == '}' )	// BATTLE�� ��.
			break;

		if( script.tokenType == IDENTIFIER )		// ��ū�� �ĺ���.
		{
			//---- Attack
			if( strcmpi( script.token, "Attack" ) == 0 )	// �Ϲ� ���ݸ��.
			{
				nCommand =	AICMD_ATTACK;	//	��� "����" ����.
				pProp->m_bMeleeAttack = 1;	// �������� AI�� �ִٴ°�.
			} else
			//--- cunning
			if( strcmpi( script.token, "cunning" ) == 0 )	// ���ݸ�ɽ� ���� ��
			{
				if( nCommand == 0 )		// cunning��ɾտ��� �����̵� ���� �;��Ѵ�.
				{
					SYNTAX_ERROR;
					return FALSE;
				} else
				if( nCommand == AICMD_ATTACK )		// Attack <xxx> cunning ���·� �Դ�.
				{
					script.GetToken();
					if( strcmpi( script.token, "low" ) == 0 )		// Ÿ���� ������ ������� ����
						pProp->m_nLvCond = 1;
					else if( strcmpi( script.token, "Sam" ) == 0 )	// Ÿ���� ������ �������.
						pProp->m_nLvCond = 2;
					else if( strcmpi( script.token, "Hi" ) == 0 )	// Ÿ���� ������ �������.
						pProp->m_nLvCond = 3;
					else
					{
						SYNTAX_ERROR;
						return FALSE;
					}
				}
			} else
			//--- Recovery
			if( strcmpi( script.token, "Recovery" ) == 0 )	// ȸ�� ���.
			{
				nCommand = AICMD_RECOVERY;		// ��� "ȸ��" ����.
				pProp->m_bRecvCond = 1;			//�ϴ��� ����Ʈ�� 1(������ ġ��)
				pProp->m_nRecvCondMe = 0;
				pProp->m_nRecvCondHow = 100;
				pProp->m_nRecvCondMP = 0;
			} else
			//--- �������. u=�ٸ��� m=����, a=�ٸ���/��
			if( strcmpi( script.token, "u" ) == 0 || 
				strcmpi( script.token, "m" ) == 0 || 
				strcmpi( script.token, "a" ) == 0 )	// �����
			{
				if( nCommand == 0 )		// u,m,a��ɾտ��� �����̵� ���� �;��Ѵ�.
				{
					SYNTAX_ERROR;
					return FALSE;
				} else
				if( nCommand == AICMD_RECOVERY )		// recovery xx xx xx (u,m,a) ���·� �Դ�.
				{
//					script.GetToken();
					if( strcmpi( script.token, "u" ) == 0 )		// ����� �ٸ���.
						pProp->m_bRecvCondWho = 1;
					else if( strcmpi( script.token, "m" ) == 0 )	// ����� ��
						pProp->m_bRecvCondWho = 2;
					else if( strcmpi( script.token, "a" ) == 0 )	// ����� �ٸ���,��
						pProp->m_bRecvCondWho = 3;

					pProp->m_bRecvCond = 2;			// �ϴ� �ɼ��� �پ����Ƿ� ����/������ ��� ����.
				}
			} else
			if( strcmpi( script.token, "RangeAttack" ) == 0 )	// ���Ÿ� ����
			{
				nCommand = AICMD_RANGEATTACK;
			} else
			if( strcmpi( script.token, "KeepRangeAttack" ) == 0 )	// �Ÿ����� ���Ÿ� ����
			{
				nCommand = AICMD_KEEP_RANGEATTACK;
			} else
			if( strcmpi( script.token, "Summon" ) == 0 )		// ��ȯ ���
			{
				nCommand = AICMD_SUMMON;
			} else
			if( strcmpi( script.token, "Evade" ) == 0 )			// ����
			{
				nCommand = AICMD_EVADE;
			} else
			if( strcmpi( script.token, "Helper" ) == 0 )		// �����û.
			{
				nCommand = AICMD_HELPER;
				pProp->m_tmUnitHelp = 0;			// ���ʸ��� �ѹ��� ���۸� �θ�����. 0�̸� �����ѹ���.
				pProp->m_nHelpRangeMul = 2;			// ����Ʈ�� �þ߹ݰ� 2�賻.
				pProp->m_bHelpWho = 1;
				pProp->m_nCallHelperMax = 5;
			} else
			// helper���� ���̴�..
			if( strcmpi( script.token, "all" ) == 0 ||
				strcmpi( script.token, "sam" ) == 0 )
			{
				if( nCommand == 0 ) // all/sam ��ɾտ��� �����̵� ���� �;� �Ѵ�.
				{
					SYNTAX_ERROR;
					return FALSE;
				} else
				if( nCommand == AICMD_HELPER )
				{
					if( strcmpi( script.token, "all" ) == 0 )			// �ƹ���.
						pProp->m_bHelpWho = 1;	
					else if( strcmpi( script.token, "sam" ) == 0 )		// ���� ����
						pProp->m_bHelpWho = 2;
				}
			} else
			if( strcmpi( script.token, "Berserk" ) == 0 )		// ����ȭ.
			{
				nCommand = AICMD_BERSERK;
			} else
			if( strcmpi( script.token, "Randomtarget" ) == 0 )	
			{
			} else
//			if( strcmpi( script.token, "Loot" ) == 0 )		// ����. ������ �޾�Ա�.
//			{
//				nCommand = AICMD_LOOT;
//				pProp->m_nLoot = 1;
//			} else
			{
				SYNTAX_ERROR;	// �̻��� ����� ������.
				return FALSE;
			}
		} else
		/////////// ��ū�� ����.
		if( script.tokenType == NUMBER )
		{
			if( nCommand == 0 )		// �ƹ� ��ɾ��� ���ڰ� ���Դ�.
			{
				SYNTAX_ERROR;
				return FALSE;
			} else
			if( nCommand == AICMD_ATTACK )		// ���ݸ���� ���Ծ���.
			{
				int nNum = atoi( script.token );		// �Ķ���Ͱ��� ���ڷ� ��ȯ.
				// nNum�� ���ü�� %;
				pProp->m_nHPCond = nNum;				// �������϶� Ÿ��HP�� nNum%�̸��̸� �����Ѵ�.
			} else
			if( nCommand == AICMD_RECOVERY )	// ȸ�����
			{
				int nNum = atoi( script.token );
				if( pProp->m_nRecvCondMe == 0 )		// ù��° �Ķ����(��HP�� ��%�����ΰ�?)
				{
					pProp->m_nRecvCondMe = nNum;
				} else
				if( pProp->m_nRecvCondHow == 100 )	// �ι�° �Ķ����(��%�� �����Ҳ���)
				{
					pProp->m_nRecvCondHow = nNum;
				} else
				if( pProp->m_nRecvCondMP == 0 )		// ��%�� ������ �Ҹ��Ҳ���.
				{
					pProp->m_nRecvCondMP = nNum;
				}
			} else
			if( nCommand == AICMD_RANGEATTACK || nCommand == AICMD_KEEP_RANGEATTACK )	// ���Ÿ� ���ݸ��
			{
				int nJob = MAX_JOB;
				int nRange	= atoi( script.token );			

				if( nCommand == AICMD_KEEP_RANGEATTACK )
				{
					nRange |= 0x80;		// KEEP_RANGE�� ���� 1��Ʈ ��.
					if( nJob >= MAX_JOB )		// ��� ����.
					{
						memset( pProp->m_bRangeAttack, nRange, sizeof(pProp->m_bRangeAttack) );		// ��������� ���������� ����.
					} else
					{
						if( nJob >0 || nJob < MAX_JOB )
							pProp->m_bRangeAttack[ nJob ] = (BYTE)nRange;		// �ش������� ���Ÿ� ���ݱ��̸� ����.
						else
							SYNTAX_ERROR;
					}
				} else
				{
					if( nJob >= MAX_JOB )		// ��� ����.
					{
						memset( pProp->m_bRangeAttack, nRange, sizeof(pProp->m_bRangeAttack) );		// ��������� ���������� ����.
					} else
					{
						if( nJob >0 || nJob < MAX_JOB )
							pProp->m_bRangeAttack[ nJob ] = (BYTE)nRange;		// �ش������� ���Ÿ� ���ݱ��̸� ����.
						else
							SYNTAX_ERROR;
					}
				}
			} else
			if( nCommand == AICMD_SUMMON )		// ��ȯ����� ���Ծ���.
			{
				pProp->m_nSummProb = atoi( script.token );	// ��ȯ �Ҷ� ����Ȯ��
				pProp->m_nSummNum  = script.GetNumber();		// �ѹ��� ����� ��ȯ�ϳ�.
				if( pProp->m_nSummNum > MAX_SUMMON )
				{
					CUSTOM_ERROR( "��ȯ�Ҽ� �ִ� ������ �ʹ� ����" );
#if __VER >= 12 // __NEW_SUMMON_RULE				
					pProp->m_nSummNum = MAX_SUMMON;
#endif // __NEW_SUMMON_RULE
				}

				script.GetToken();					// ���� ���̵�
				if( script.tokenType != NUMBER )
					UNIDENTIFY_ERROR;
				pProp->m_nSummID = atoi( script.token );
				if( GetMoverProp( pProp->m_nSummID ) == NULL )
					CUSTOM_ERROR( "������ ���� ���̵� ������Ƽ�� ����" );
			} else
			if( nCommand == AICMD_EVADE )	// ���� ����� ���Ծ���.
			{
				pProp->m_nRunawayHP = atoi( script.token );		// ��%������ ��������?
			} else
			if( nCommand == AICMD_HELPER )	// ���� ����� ���Ծ���.
			{
				int nNum = atoi( script.token );
				if( pProp->m_tmUnitHelp == 0 )		// �Ķ���Ͱ� ���� �������� �ʾҴٸ�..
					pProp->m_tmUnitHelp = (DWORD)nNum;		// ���ʸ��� ���۸� �θ��� �õ��� �Ҳ���.
				else if( pProp->m_nHelpRangeMul == 2 )		// �Ķ���Ͱ� ���� �������� �ʾҴٸ�..
					pProp->m_nHelpRangeMul = nNum;			// �þ߹��
			} else
			if( nCommand == AICMD_BERSERK )	// ����Ŀ ����� ���Ծ���.
			{
				pProp->m_nBerserkHP = atoi( script.token );		// ����Ŀ�� �Ǳ����� %. 0�̸� ����Ŀ����.
				pProp->m_fBerserkDmgMul = script.GetFloat();	// ����Ŀ�ŵǾ����� ������ ���.
				if( pProp->m_fBerserkDmgMul <= 0 || pProp->m_fBerserkDmgMul >= 20.0f )
				{
					CString str;
					str.Format( "Berserk����� ����������� ������ �̻��Ѱ� ������? %f", pProp->m_fBerserkDmgMul );
					CUSTOM_ERROR( (LPCTSTR)str );
				}
			}
		}
		
	}
	
	return TRUE;
}

// AI{}�� #MOVE ��.
BOOL CProject::LoadPropMoverEx_AI_MOVE( LPCTSTR szFileName, CScript &script, int nVal )
{
	AICMD nCommand = AICMD_NONE;
	
	script.GetToken();	// {
	if( script.token[0] != '{' )	// #MOVE �Ŀ� {�� �����°�.
	{
		Error( "%s(%d) : MoverID=%d MOVE ���ͺ� { ������.", szFileName, script.GetLineNum(), nVal );
		return FALSE;
	}
	
	MoverProp* pProp = GetMoverProp( nVal );
	
	while(1)
	{
		script.GetToken();		// ��ū �ϳ� ����,.
		if( script.token[0] == '}' )	// MOVE�� ��.
			break;

		if( script.tokenType == IDENTIFIER )		// ��ū�� �ĺ���.
		{
			if( strcmpi( script.token, "Loot" ) == 0 )		// ����. ������ �޾�Ա�.
			{
				nCommand = AICMD_LOOT;
//				pProp->m_nLoot = 1;		// ˬ: �ӽ÷� ����
				pProp->m_nLoot	= 0;
			} else
			if( strcmpi( script.token, "d" ) == 0 )		// Loot��� �ɼ�
			{
//				pProp->m_nLoot = 2;		// 2�� d�ɼ�.
				pProp->m_nLoot	= 0;	// ˬ: �ӽ÷� ����
			} else
			{
				SYNTAX_ERROR;	// �̻��� ����� ������.
				return FALSE;
			}
		} else
		/////////// ��ū�� ����.
		if( script.tokenType == NUMBER )
		{
			if( nCommand == 0 )		// �ƹ� ��ɾ��� ���ڰ� ���Դ�.
			{
				SYNTAX_ERROR;
				return FALSE;
			}
			else if( nCommand == AICMD_LOOT )
			{
				if( script.tokenType != NUMBER )
					UNIDENTIFY_ERROR;
				pProp->m_nLootProb = atoi( script.token );
			}
		}
	}
	
	return TRUE;
}


// propMoverEx.inc�� AI {} �� ó����.
BOOL CProject::LoadPropMoverEx_AI( LPCTSTR szFileName, CScript &script, int nVal )
{
	script.GetToken();		// {

	if( script.token[0] != '{' )
	{
		Error( "%s(%d) : MoverID=%d AI ������ { ����.", szFileName, script.GetLineNum(), nVal );
		return FALSE;
	}

	while(1)
	{
		script.GetToken();

		if( script.token[0] == '}' )		// �� ���������� ����.
			break;

		// ���� �� ����.
		if( script.token[0] == '#' )		// #???? �� ���;� ��.  #SCAN���� �Ѳ����� ������.
		{
//			script.GetToken();	// SCAN/BATTLE/MOVING
			if( strcmpi( script.Token, "#SCAN" ) == 0 )	// 
			{
				if( LoadPropMoverEx_AI_SCAN( szFileName, script, nVal ) == FALSE )
					return FALSE;
			} else
			if( strcmpi( script.Token, "#BATTLE" ) == 0 )	
			{
				if( LoadPropMoverEx_AI_BATTLE( szFileName, script, nVal ) == FALSE )
					return FALSE;
			} else
			if( strcmpi( script.Token, "#MOVE" ) == 0 )	
			{
				if( LoadPropMoverEx_AI_MOVE( szFileName, script, nVal ) == FALSE )
					return FALSE;
			} else
			{
				Error( "%s(%d) : MoverID=%d ���ͺ��̸��� �̻���(%s) ", szFileName, script.GetLineNum(), nVal, script.Token );
				return FALSE;
			}
		} else
		{
			// ���ͺ��� ����.
			Error( "%s(%d) : MoverID=%d ���ͺ�(<)�� ����.", szFileName, script.GetLineNum(), nVal );
			return FALSE;
		}
	}

	return TRUE;
}

