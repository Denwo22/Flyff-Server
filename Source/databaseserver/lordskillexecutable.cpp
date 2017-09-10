#include "stdafx.h"

#if __VER >= 12 // __LORD

#include "tlord.h"
#include "lordskillexecutable.h"
#include "dptrans.h"
#include "definetext.h"

CLordSkillComponentODBC::CLordSkillComponentODBC()
{
}

CLordSkillComponentODBC::~CLordSkillComponentODBC()
{
}

void CLordSkillComponentODBC::Execute( u_long idPlayer, u_long idTarget, VOID* pParam )
{	// Ʈ���� ������ ���� ��ų ����
	int nRet	= 0;
	if( GetTick() > 0 )		// ���� ��� �ð��� ���������� �Ұ�
		nRet	= TID_GAME_LORD_SKILL_USE_E005;
	if( !nRet )
	{	// ���� ��� �ð� ���� ���� ��û
		CLController* pController	= reinterpret_cast<CLController*>( pParam );
		if( !pController->UpdateLordSkillTick( this, GetCooltime() ) )
			nRet	= TID_GAME_LORD_SKILL_USE_E006;
	}
	if( !nRet )		// ���� �����ϸ� ���� ��� �ð� ����
		Use();
	// ���� ������ ��� ����
	CDPTrans::GetInstance()->SendLordSkillUse( idPlayer, idTarget, GetId(), nRet );
}

#endif	// __LORD