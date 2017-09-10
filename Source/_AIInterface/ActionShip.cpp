#include "stdafx.h"
#include "ActionShip.h"
#include "..\_Common\Ship.h"

#ifdef __CLIENT
#include "DPClient.h"
extern	CDPClient	g_DPlay;
#endif


CActionShip::CActionShip()
{
	Init();
}

CActionShip::~CActionShip()
{
	Destroy();
}

void CActionShip::Init( void )
{
	m_pShip = NULL;
}

void CActionShip::Destroy( void )
{
	// �ı��ڵ�.

	Init();
}

//
//
//
int	CActionShip::Process( void )
{
	CShip *pShip = m_pShip;
	
	if( GetState() == 0 )
	{
		SendActMsg( OBJMSG_STAND );
	}

	if( GetState() & OBJSTA_MOVE_ALL )
		ProcessState( GetState() & OBJSTA_MOVE_ALL );

	if( GetState() & OBJSTA_TURN_ALL )	// ��/�� ��
		ProcessState( GetState() & OBJSTA_TURN_ALL );
	
	if( GetState() & OBJSTA_LOOK_ALL )	// �� ��/�ϰ�
		ProcessState( GetState() & OBJSTA_LOOK_ALL );

	if( GetState() & OBJSTA_ATK_ALL )	// ����
		ProcessState( GetState() & OBJSTA_ATK_ALL );

	if( GetState() & OBJSTA_DMG_ALL )	// �ǰ�.
		ProcessState( GetState() & OBJSTA_DMG_ALL );

	if( GetTurnState() == 0 )		// ���� �������� ������ ���°�.
	{
		if( pShip->m_fAccAng > 0 )		// �������� ���ϰ� �־���.
		{
			pShip->m_fAccAng -= 0.002f;
			if( m_pShip->m_fAccAng < 0 )
				m_pShip->m_fAccAng = 0;
		} else
		if( pShip->m_fAccAng < 0 )
		{
			pShip->m_fAccAng += 0.002f;
			if( m_pShip->m_fAccAng > 0 )
				m_pShip->m_fAccAng = 0;
		}
		if( m_pShip->m_fAccAng )
			m_pShip->SetAngle( m_pShip->GetAngle() + m_pShip->m_fAccAng );
	}

	return TRUE;
}

#define AE_SUCCESS	 1		// ����� ���������� ��������.
#define AE_ALREADY	 0		// �̹� �������̹Ƿ� �ٽ� �������� ����.
#define AE_FAIL		-1		// ����� �����ϱ⿡ ������ ��Ȳ�� �ƴϾ �� ������.

//
// ������Ʈ���� ������ ����� ������ ���ʼ����� �����ϰ� ���¸� ��ȯ��Ŵ.
// return : #define AE_xxxx ����.
int	CActionShip::SendActMsg( OBJMSG dwMsg, int nParam1, int nParam2, int nParam3 )
{
	switch( dwMsg )
	{
	case OBJMSG_STAND:	// �����·� �־��.
		if( GetMoveState() == OBJSTA_STAND )	return AE_ALREADY;		// �̹� �������̹Ƿ� ����.
		SetMoveState( OBJSTA_STAND );		// ��� ���·� ��ȯ.
		break;
	case OBJMSG_STOP:	// �����.
		if( GetMoveState() == 0 )	return AE_ALREADY;		// �̹� ���������Ƿ� ����.
		ResetState( OBJSTA_MOVE_ALL );		// �̵����¸� ����
		SetMoveState( OBJSTA_STAND );
		break;
	case OBJMSG_FORWARD:	// ������ �����ض�.
		if( GetMoveState() == OBJSTA_FMOVE )	return AE_ALREADY;	// �̹� ���������̸� ����.
		SetMoveState( OBJSTA_FMOVE );	// ���� ���·� ��ȯ.
		break;
	
	case OBJMSG_LTURN:	// ��ȸ���� �����ض�.
		if( GetTurnState() == OBJSTA_LTURN )	return AE_ALREADY;	// �̹� ��������.
		SetTurnState( OBJSTA_LTURN );
		break;
	case OBJMSG_RTURN:	// ��ȸ���� �����ض�.
		if( GetTurnState() == OBJSTA_RTURN )	return AE_ALREADY;	// �̹� ����������.
		SetTurnState( OBJSTA_RTURN );
		break;
	case OBJMSG_STOP_TURN:	// ȸ���� ����.
		if( GetTurnState() == 0 )	return AE_ALREADY;	// �̹� ��������.
		ResetState( OBJSTA_TURN_ALL );	// �ϻ��� ����.
		break;

	case OBJMSG_LOOKUP:		// ����� �ض�.
		if( GetLookState() == OBJSTA_LOOKUP )	return AE_ALREADY;	// �̹� �����.
		SetLookState( OBJSTA_LOOKUP );
		break;
	case OBJMSG_LOOKDOWN:	// ���ϰ�.
		if( GetLookState() == OBJSTA_LOOKDOWN )	return AE_ALREADY;	// �̹� �ϰ���
		SetLookState( OBJSTA_LOOKDOWN );
		break;
	case OBJMSG_STOP_LOOK:	// ���/�ϰ� �����.
		if( GetLookState() == 0 )	return AE_ALREADY;	// �̹� ������.
		ResetState( OBJSTA_LOOK_ALL );	// ���/�ϰ� ���� ����
		break;
	}
#ifdef __CLIENT
	if( m_pShip->GetMover()->IsActiveMover() )
		g_DPlay.SendShipActMsg( m_pShip, dwMsg, nParam1, nParam2 );
#endif // CLIENT	
	return 1;
}

//
// ������Ʈ�� ���� ���¸� ������
//
void CActionShip::ProcessState( DWORD dwState, float fSpeed )
{
	CShip *pShip = m_pShip;
	FLOAT fAng = pShip->GetAngle();
	
	switch( dwState )
	{
	case OBJSTA_STAND:	// �����
		pShip->m_vAcc = D3DXVECTOR3( 0, 0, 0 );		// �����¿��� �������� ����.
		break;
	case OBJSTA_FMOVE:	// ������
		AngleToVectorXZ( &pShip->m_vAcc, fAng, 0.001f );		// fAng�������� �����¹߻�.
		break;
	case OBJSTA_LTURN:	// ��������
		pShip->m_fAccAng += 0.01f;
		if( pShip->m_fAccAng > 0.1f )
			pShip->m_fAccAng = 0.1f;
		fAng += pShip->m_fAccAng;
		pShip->SetAngle( fAng );
		break;
	case OBJSTA_RTURN:	// ��������.
		pShip->m_fAccAng -= 0.01f;
		if( pShip->m_fAccAng < -0.1f )
			pShip->m_fAccAng = -0.1f;
		fAng += pShip->m_fAccAng;
		pShip->SetAngle( fAng );
		break;
	case OBJSTA_LOOKUP:		// �� �����
		pShip->m_vAcc.y += 0.0001f;
		break;
	case OBJSTA_LOOKDOWN:	// �� �ϰ���
		pShip->m_vAcc.y -= 0.0001f;
		break;

	}
}

