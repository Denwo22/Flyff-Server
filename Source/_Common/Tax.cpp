#include "stdafx.h"
#include "tax.h"

#if __VER >= 12 // __TAX

#ifdef __DBSERVER
#include "dptrans.h"
#include "post.h"
#include "tlord.h"
#endif // __DBSERVER

#ifdef __WORLDSERVER
#include "User.h"
#include "DPDatabaseClient.h"
extern CDPDatabaseClient g_dpDBClient;
#include "DPCoreClient.h"
extern CDPCoreClient	g_DPCoreClient;
#endif // __WORLDSERVER

#include "defineText.h"
#include "guild.h"
extern CGuildMng g_GuildMng;

CTax::CTax(void)
{
	m_nMinTaxRate = 0;
	m_nMaxTaxRate = 0;
	
	m_mapTaxInfo.insert( make_pair(CONT_EAST, new __TAXINFO) );		// ���� ���� ��� - ����ᵵ ����
	m_mapTaxInfo.find( CONT_EAST )->second->mapTaxDetail.insert( make_pair( TAX_ADMISSION, new __TAXDETAIL ) ); 
	m_mapTaxInfo.insert( make_pair(CONT_WEST, new __TAXINFO) );		// ���� ���� ���
	m_mapTaxInfo.insert( make_pair(CONT_ALL, new __TAXINFO) );		// ���� - ����ᵵ ����
	m_mapTaxInfo.find( CONT_ALL )->second->mapTaxDetail.insert( make_pair( TAX_ADMISSION, new __TAXDETAIL ) );

#ifdef __DBSERVER
	m_nTaxSecretRoomRate = 0;
	m_nTaxLordRate = 0;
	m_nAdmissionSecretRoomRate = 0;
	m_nAdmissionLordRate = 0;
	m_nDBSaveCount = 1;
//	m_strChangedDate.clear();

	// DBController thread ����
	if( !m_taxDBController.CreateDbHandler( MIN(1) ) )	// 1�п� �ѹ� OnTimer()�� call�Ѵ�.
		Error( "CTax - m_Controller.CreateDbHandler()" );
#endif // __DBSERVER
}

CTax::~CTax(void)
{
	for( TAXINFOMAP::iterator it = m_mapTaxInfo.begin(); it!=m_mapTaxInfo.end(); it++ )
	{
		__TAXINFO* taxInfo = it->second;
		if( taxInfo )
		{
			for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
			{
				__TAXDETAIL* taxDetail = it2->second;
				if( taxDetail )
					safe_delete( taxDetail );
			}
			safe_delete( taxInfo );
		}
	}

	m_mapTaxInfo.clear();

#ifdef __DBSERVER
	m_taxDBController.CloseDbHandler();
#endif // __DBSERVER
#ifdef __OCCUPATION_SHOPITEM
	m_vecdwOccupationShopItem.clear();
#endif // __OCCUPATION_SHOPITEM
}


CTax* CTax::GetInstance( void )
{
	static CTax sTax;
	return &sTax;
}

__TAXINFO* CTax::GetTaxInfo( BYTE nContinent )	// �ش� ����� ���� ����
{
	if( m_mapTaxInfo.find( nContinent ) != m_mapTaxInfo.end() )
		return m_mapTaxInfo.find( nContinent )->second;
#ifndef __CLIENT
	else
		Error( "CTax::GetTaxInfo() - �߸��� ������� %x", nContinent );
#endif // __CLIENT

	return NULL;
}

#ifndef __DBSERVER
BYTE CTax::GetContinent( CMover* pMover )
{
	return CContinent::GetInstance()->GetArea( pMover );
}

float CTax::GetSalesTaxRate( BYTE nContinent )	// �ش� ����� �Ǹ� ����
{
	if( nContinent == CONT_NODATA )
		return 0.0f;

	__TAXINFO* taxInfo = GetTaxInfo( nContinent );
	if( !taxInfo )
		return 0.0f;

	int nTaxRate = taxInfo->mapTaxDetail.find( TAX_SALES )->second->nTaxRate;
	return static_cast<float>(nTaxRate) * 0.1f * 0.1f;
}

float CTax::GetSalesTaxRate( CMover* pMover )
{
	BYTE nContinent = GetContinent( pMover );

	return GetSalesTaxRate( nContinent );
}

float CTax::GetPurchaseTaxRate( BYTE nContinent )	// �ش� ����� ���� ����
{
	if( nContinent == CONT_NODATA )
		return 0.0f;

	__TAXINFO* taxInfo = GetTaxInfo( nContinent );
	if( !taxInfo )
		return 0.0f;

	int nTaxRate = taxInfo->mapTaxDetail.find( TAX_PURCHASE )->second->nTaxRate;
	return static_cast<float>(nTaxRate) * 0.1f * 0.1f;
}

float CTax::GetPurchaseTaxRate( CMover* pMover )
{
	BYTE nContinent = GetContinent( pMover );

	return GetPurchaseTaxRate( nContinent );
}

// ���������� ������ �������ΰ�?
BOOL CTax::IsApplyTaxRate( CMover* pMover, CItemElem* pItemElem )
{
	if( !CContinent::IsValidObj( pMover ) || GetContinent( pMover ) == CONT_NODATA || !pItemElem )
		return FALSE;

	//if( pItemElem->GetProp()->dwItemKind3 == IK3_SCROLL )
	//	return FALSE;
	
	switch( pItemElem->m_dwItemId )
	{
		case II_SYS_SYS_SCR_PERIN :			// �丰
		case II_CHP_RED :					// ����Ĩ
		case II_SYS_SYS_SCR_AWAKE :			// ������ �η縶��
		case II_SYS_SYS_SCR_PETAWAKE :		// ������ ������ �η縶��
			return FALSE;
	}

	return TRUE;
}

#endif // !__DBSERVER

#ifdef __DBSERVER
void CTax::LoadScript()
{
	if( m_Lua.RunScript( "Tax.lua" ) != 0 )
	{
		Error( "Tax.lua Run Failed !!!" );
		ASSERT(0);
	}

	m_nMinTaxRate =					static_cast<int>( m_Lua.GetGlobalNumber( "MinTaxRate" ) );
	m_nMaxTaxRate =					static_cast<int>( m_Lua.GetGlobalNumber( "MaxTaxRate" ) );
	m_nTaxSecretRoomRate =			static_cast<int>( m_Lua.GetGlobalNumber( "TaxSecretRoomRate" ) );
	m_nTaxLordRate =				static_cast<int>( m_Lua.GetGlobalNumber( "TaxLordRate" ) );
	m_nAdmissionSecretRoomRate =	static_cast<int>( m_Lua.GetGlobalNumber( "AdmissionSecretRoomRate" ) );
	m_nAdmissionLordRate =			static_cast<int>( m_Lua.GetGlobalNumber( "AdmissionLordRate" ) );
	m_nDBSaveCount =				static_cast<int>( m_Lua.GetGlobalNumber( "DBSaveCount" ) );
	if( m_nDBSaveCount < 1 )
		m_nDBSaveCount = 1;
// #ifndef __MAINSERVER
// 	m_nDBSaveCount = 1;
// #endif // __MAINSERVER

#ifdef __OCCUPATION_SHOPITEM
	m_Lua.GetGloabal( "tOccupationShopItem" );
	m_Lua.PushNil();
	while( m_Lua.TableLoop( -2 ) )
	{
		m_vecdwOccupationShopItem.push_back( CScript::GetDefineNum( m_Lua.ToString( -1 ) ) );
		m_Lua.Pop( 1 );
	}
#endif // __OCCUPATION_SHOPITEM

	LoadTaxInfo( DPID_ALLPLAYERS );
}

void CTax::CheckChangeTime( BOOL bPay, BOOL bGameMaster )
{
	// ���� ���� �ð��� �˻��ؼ� ������ �����Ѵ�.
	// bPay�� FALSE�ΰ�� ���޵��� ���� ����̹Ƿ� �������� �����Ѵ�.
	// bGameMaster�� �⺻���� FALSE�̳� GM�� ������ ���� ������ ������ ��� �ð��� ������� �����Ѵ�.
	
	// ���ְ� ����Ǵ� ��(2���Ͽ� �ѹ�)�ΰ��...
	IElection* pElection	= CTLord::Instance()->GetElection();
	if( pElection->GetState() == IElection::ELECTION_STATE::eVote ) // ��ǥ���� ����
		return;	// ���� ���ְ� ������ ���� �ʾ����Ƿ� ���� TimeOut�� �ٽ� �˻��Ѵ�.
	
	if( m_Lua.GetLuaFunction( "CheckChangeTime" ) )
	{
		m_Lua.PushString( m_strChangedDate.c_str() );
		m_Lua.CallLuaFunction( 1, 2 );
		if( m_Lua.ToBool( -2 ) || bGameMaster )		// ������ ����� �ð��̸�...
		{
			// ���� ���� ������ ������ �̷������ ������� ���� �����Ѵ�.
			if( !bPay )
				m_taxDBController.PostRequest( QUERY_TAX_PAY, 0, 0, 0 );

			// ���� �Ǵ� ���� ����� ���ָ� �����Ѵ�.
			m_taxDBController.PostRequest( QUERY_TAX_SETNEXT_LORD, 0, 0, CTLord::Instance()->Get() );			
			
			// ���� �ð��� ���� �ð����� �����ϰ� ����� ������ �����Ѵ�.
			m_strChangedDate = m_Lua.ToString( -1 );
			m_taxDBController.PostRequest( QUERY_TAX_CHANGENEXT, 0, 0, 0 );
		}
	}
	m_Lua.Pop(0);
}

void CTax::SetChangeNextTax()	// ����� ������ ����
{
	//�� ����� ���ɱ��(CONT_���)�� ����(CONT_ALL)�� ��� �����Ѵ�.
	for( TAXINFOMAP::iterator it=m_mapTaxInfo.begin(); it!=m_mapTaxInfo.end(); it++ )
	{
		__TAXINFO* taxInfo = it->second;
		taxInfo->dwId = taxInfo->dwNextId;
		taxInfo->dwNextId = NULL_ID; // ���� �� �ʱ�ȭ
		
		if( taxInfo->bSetTaxRate )
		{
			if( it->first != CONT_ALL )		// ���ִ� ������ �������.
			{
				taxInfo->mapTaxDetail.find( TAX_SALES )->second->nTaxRate = taxInfo->mapTaxDetail.find( TAX_SALES )->second->nNextTaxRate;
				taxInfo->mapTaxDetail.find( TAX_PURCHASE )->second->nTaxRate = taxInfo->mapTaxDetail.find( TAX_PURCHASE )->second->nNextTaxRate;
			}
		}
		else	// ���� ������ ���� �ʾ��� ��� �⺻ ������ ����
		{
			taxInfo->bSetTaxRate = TRUE;	// ���̻� ���� ���� â�� �ȶߵ��� ������ ������ �����.
			if( it->first != CONT_ALL )		// ���ִ� ������ �������.
			{
				taxInfo->mapTaxDetail.find( TAX_SALES )->second->nTaxRate = m_nMinTaxRate;
				taxInfo->mapTaxDetail.find( TAX_PURCHASE )->second->nTaxRate = m_nMinTaxRate;
			}
		}
		
		// ������ ��� �ʱ�ȭ �Ѵ�.
		for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
		{
			__TAXDETAIL* taxDetail = it2->second;
			taxDetail->nNextTaxRate = 0;
			taxDetail->nTaxCount = 0;
			taxDetail->nTaxPerin = 0;
			taxDetail->nTaxGold = 0;
		}
	}
}

BOOL CTax::CheckPayTime()	// ���� ���� ���Ա��� �������� ������.
{
	// ���޽ð� �˻� -	���� �ð��� "��:��"�� �ݵ�� ��ġ�ؾ� �Ѵ�.(���� ���� ������ ����� ������ �� �ִ�)
	//					���� �ð��� ������ ������ �ȵȰ�� ������ �ٽ� �˻��ؼ� ���� �����Ѵ�.
	//					���� ������ ������ �ð��̸� bPay�� �����Ͽ� FALSE�� ��� �������� �����Ѵ�.
	BOOL bPay = FALSE;
	if( m_Lua.GetLuaFunction( "CheckPayTime" ) )
	{
		m_Lua.CallLuaFunction( 0, 1 );
		if( m_Lua.ToBool( -1 ) )
		{
			bPay = TRUE;
			m_taxDBController.PostRequest( QUERY_TAX_PAY, 0, 0, 0 );
		}
	}
	m_Lua.Pop(0);

	return bPay;
}

float CTax::GetEarningRate( BYTE nCont, BYTE nTaxKind )	// �� ���ݿ� ���� ���ֿ� ���ɱ���� ���ͷ�
{
	int nEarningRate = 0;
	switch( nTaxKind )
	{
	case TAX_ADMISSION :	// �����
		if( nCont == CONT_ALL )		// ������ ���
			nEarningRate = m_nAdmissionLordRate;
		else
			nEarningRate = m_nAdmissionSecretRoomRate;
		break;

	default :
		if( nCont == CONT_ALL )		// ������ ���
			nEarningRate = m_nTaxLordRate;
		else
			nEarningRate = m_nTaxSecretRoomRate;
		break;
	}

	return static_cast<float>(nEarningRate) * 0.1f * 0.1f;
}

void CTax::LoadTaxInfo( DWORD dpId )
{
	m_taxDBController.PostRequest( QUERY_TAX_LOAD, 0, 0, dpId );
}
#endif // __DBSERVER

void CTax::SetNextSecretRoomGuild( BYTE nCont, DWORD dwGuildId )	// ���� ���ɱ�� ����
{
#ifdef __WORLDSERVER
	g_dpDBClient.SendSecretRoomWinGuild( nCont, dwGuildId );
#endif // __WORLDSERVER

#ifdef __DBSERVER
	__TAXINFO* taxInfo = GetTaxInfo( nCont );
	if( !taxInfo )
		return;

	if( dwGuildId == NULL_ID )	// ���ɱ�尡 ���� ��� ������ ������ ������ �Ѵ�.
		taxInfo->bSetTaxRate = TRUE;
	else
		taxInfo->bSetTaxRate = FALSE;
	// ���� ID��� �� ���� �ʱ�ȭ
	taxInfo->dwNextId = dwGuildId;
	for( TAXDETAILMAP::iterator it=taxInfo->mapTaxDetail.begin(); it!=taxInfo->mapTaxDetail.end(); it++ )
		it->second->nNextTaxRate = 0;
#endif // __DBSERVER
}

void CTax::SetNextLord( DWORD dwIdPlayer )	// ���� ���� ����
{
#ifdef __WORLDSERVER
	g_dpDBClient.SendLord( dwIdPlayer );
#endif // __WORLDSERVER

#ifdef __DBSERVER
	__TAXINFO* taxInfo = GetTaxInfo( CONT_ALL );
	if( !taxInfo )
		return;
	
	taxInfo->bSetTaxRate = TRUE;
	// ���� ID��� �� ���� �ʱ�ȭ
	taxInfo->dwNextId = dwIdPlayer;
	for( TAXDETAILMAP::iterator it=taxInfo->mapTaxDetail.begin(); it!=taxInfo->mapTaxDetail.end(); it++ )
		it->second->nNextTaxRate = 0; // ���ִ� ������ ���ǹ� ������ �ʱ�ȭ...
#endif // __DBSERVER
}

void CTax::SetNextTaxRate( BYTE nCont, int nSalesTaxRate, int nPurchaseTaxRate )	// ���� ������ �����Ѵ�.
{
#ifdef __WORLDSERVER
	if( nSalesTaxRate < m_nMinTaxRate || nSalesTaxRate > m_nMaxTaxRate
		|| nPurchaseTaxRate < m_nMinTaxRate || nPurchaseTaxRate > m_nMaxTaxRate )
		return;	// �ּ�, �ִ� ���� ����� �׳� ����...

	g_dpDBClient.SendTaxRate( nCont, nSalesTaxRate, nPurchaseTaxRate );
#endif // __WORLDSERVER
#ifdef __DBSERVER
	__TAXINFO* taxInfo = GetTaxInfo( nCont );
	if( !taxInfo )
		return;
	
	taxInfo->bSetTaxRate = TRUE;
	taxInfo->mapTaxDetail.find( TAX_SALES )->second->nNextTaxRate = nSalesTaxRate;
	taxInfo->mapTaxDetail.find( TAX_PURCHASE )->second->nNextTaxRate = nPurchaseTaxRate;
#endif // __DBSERVER
}

void CTax::SetApplyTaxRateNow()		// GM������� ���� ������ �� ���
{
#ifdef __WORLDSERVER
	g_dpDBClient.SendApplyTaxRateNow();
#endif // __WORLDSERVER
#ifdef __DBSERVER
	//m_taxDBController.PostRequest( QUERY_TAX_CHANGENEXT, 0, 0, 0 );
	CheckChangeTime( FALSE, TRUE );	// FALSE - ������ �ȵǾ���, TRUE - GM������� ���� ���� �����Ѵ�.
#endif // __DBSERVER
}

void CTax::Serialize( CAr & ar )	// ������ �ʿ��� ��� ������ �����Ѵ�.
{
	if( ar.IsStoring() )
	{
		ar << m_mapTaxInfo.size();
		for( TAXINFOMAP::iterator it=m_mapTaxInfo.begin(); it!=m_mapTaxInfo.end(); it++ )
		{
			__TAXINFO* taxInfo = it->second;
			ar << it->first;

			ar << taxInfo->dwId;
#ifdef __DBSERVER
			ar << taxInfo->bSetTaxRate;
			ar << taxInfo->dwNextId;
#endif // __DBSERVER
			ar << taxInfo->mapTaxDetail.size();
			for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
			{
				__TAXDETAIL* taxDetail = it2->second;
				ar << it2->first;

				ar << taxDetail->nTaxRate;
			}
		}
	}
	else
	{
		int nSize, nSize2;
		BYTE nCont, nTaxKind;
				
		ar >> nSize;
		for( int i=0; i<nSize; i++ )
		{
			ar >> nCont;
			__TAXINFO* taxInfo = GetTaxInfo( nCont );
			ar >> taxInfo->dwId;
#ifdef __WORLDSERVER
			ar >> taxInfo->bSetTaxRate;
			ar >> taxInfo->dwNextId;
#endif // __WORLDSERVER
			ar >> nSize2;
			for( int j=0; j<nSize2; j++ )
			{
				ar >> nTaxKind;
				__TAXDETAIL* taxDetail = taxInfo->mapTaxDetail.find( nTaxKind )->second;
			
				ar >> taxDetail->nTaxRate;
			}
		}
	}
}

BOOL CTax::AddTax( BYTE nCont, int nTax, BYTE nTaxKind )	// �Ǹ�, ����, ����ῡ ���� ������ �����Ѵ�.
{
	BOOL bUpdateToDB = FALSE;
#ifdef __WORLDSERVER
	g_dpDBClient.SendAddTax( nCont, nTax, nTaxKind );
#endif // __WORLDSERVER

#ifdef __DBSERVER
	for( TAXINFOMAP::iterator it=m_mapTaxInfo.begin(); it!=m_mapTaxInfo.end(); it++ )
	{
#if __VER >= 15 // __USING_CONTINENT_DATA
		if( it->first == CONT_ALL || it->first == nCont )
#else // __USING_CONTINENT_DATA
		if( it->first & nCont )
#endif // __USING_CONTINENT_DATA
		{
			__TAXINFO* taxInfo = GetTaxInfo( it->first );
			if( !taxInfo || taxInfo->dwId == NULL_ID )
				continue;
			if( taxInfo->mapTaxDetail.find( nTaxKind ) == taxInfo->mapTaxDetail.end() )
				continue;

			__TAXDETAIL* taxDetail = taxInfo->mapTaxDetail.find( nTaxKind )->second;
			if( taxDetail )
			{
				taxDetail->nTaxCount++;
				taxDetail->nTaxGold += static_cast<int>( static_cast<float>(nTax) * GetEarningRate( nCont, nTaxKind ) );
				int nTaxTemp = taxDetail->nTaxGold;
				if( nTaxTemp >= 100000000 )
				{
					taxDetail->nTaxPerin += nTaxTemp/PERIN_VALUE;
					taxDetail->nTaxGold = nTaxTemp%PERIN_VALUE;
				}
				// m_nDBSaveCount���� ä������ ������ DB�� �������� �ʴ´�.
				if( (taxDetail->nTaxCount % m_nDBSaveCount) == 0 )
					bUpdateToDB = TRUE;
			}
		}
	}
#endif // __DBSERVER
	return bUpdateToDB;
}

#ifdef __WORLDSERVER
void CTax::SendSetTaxRateOpenWnd( BYTE nCont, DWORD dwGuildId )		// ���ɱ�忡�� ���� ����â�� ������ �Ѵ�.
{
	CGuild* pGuild = g_GuildMng.GetGuild( dwGuildId );
	if( pGuild )
	{
		CUser* pUserTemp = (CUser*)prj.GetUserByID( pGuild->m_idMaster );
		if( IsValidObj( pUserTemp ) )
			pUserTemp->AddTaxSetTaxRateOpenWnd( nCont );
	}
}

void CTax::SendNoSetTaxRateOpenWnd( CUser* pUser )	// ���� ������ ���� ������� ���ӽ� ���� ����â�� ������...
{
	CGuild* pGuild = pUser->GetGuild();
	if( pGuild )
	{
		for( TAXINFOMAP::iterator it=m_mapTaxInfo.begin(); it!=m_mapTaxInfo.end(); it++ )
		{
			if( it->first != CONT_ALL && it->second->dwNextId == pGuild->GetGuildId()
				&& !it->second->bSetTaxRate && pGuild->IsMaster( pUser->m_idPlayer ) )
			{
				pUser->AddTaxSetTaxRateOpenWnd( it->first );
				return;
			}
		}
	}
}

#ifdef __OCCUPATION_SHOPITEM
BOOL CTax::IsOccupationGuildMember( CMover* pMover )
{
	if( GetContinent( pMover ) == CONT_NODATA )
		return FALSE;

	__TAXINFO* pTaxInfo = GetTaxInfo( GetContinent( pMover ) );
	if( pTaxInfo && pTaxInfo->dwId == pMover->m_idGuild )
		return TRUE;

	return FALSE;
}

BOOL CTax::IsOccupationShopItem( DWORD dwItemId )
{
	for( size_t i=0; i<m_vecdwOccupationShopItem.size(); i++ )
	{
		if( m_vecdwOccupationShopItem[i] == dwItemId )
			return TRUE;
	}
	
	return FALSE;
}
#endif // __OCCUPATION_SHOPITEM
#endif // __WORLDSERVER


#ifdef __DBSERVER
///////////////////////////////////////////////////////////////////////
///// CTaxDBController ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
CTaxDBController::CTaxDBController( void )
:
	m_nTimes( 0 ),
	m_bLoadTaxInfo( FALSE )
{
}

CTaxDBController::~CTaxDBController( void )
{
}

void CTaxDBController::Handler( LPDB_OVERLAPPED_PLUS pov, DWORD dwCompletionKey )
{
	switch( pov->nQueryMode )
	{
		case QUERY_TAX_LOAD :
			{
				if( !m_bLoadTaxInfo )	// DB���� �ε��� �ѹ���...
				{
					LoadTaxInfo();
					m_bLoadTaxInfo = TRUE;
				}
				CDPTrans::GetInstance()->SendTaxInfo( dwCompletionKey, TRUE );
			}
			break;

		case QUERY_TAX_PAY :	
			{
				PayTaxToPost();
				UpdateAllToDB(); // ��� ����� ���� ������ �����Ѵ�.
			}
			break;
		
		case QUERY_TAX_CHANGENEXT :
			{
				CTax::GetInstance()->SetChangeNextTax();
				CDPTrans::GetInstance()->SendTaxInfo( DPID_ALLPLAYERS, FALSE, TRUE ); // WorldServer�� ����
				InsertToDB();		// ����� ������ DB�� �߰� 
			}
			break;

		
		case QUERY_TAX_SETNEXT_SECRETROOMWINGUILD :
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nCont;
				DWORD dwGuildId;
				ar >> nCont >> dwGuildId;
				CTax::GetInstance()->SetNextSecretRoomGuild( nCont, dwGuildId );
				CDPTrans::GetInstance()->SendTaxInfo( DPID_ALLPLAYERS );
				UpdateToDB( nCont );
			}
			break;
		
		case QUERY_TAX_SETNEXT_LORD :
			{
				if( dwCompletionKey == 0 )
				{
					CAr ar( pov->lpBuf, pov->uBufSize );
					DWORD dwIdPlayer;
					ar >> dwIdPlayer;
					CTax::GetInstance()->SetNextLord( dwIdPlayer );
				}
				else
					CTax::GetInstance()->SetNextLord( dwCompletionKey );

				CDPTrans::GetInstance()->SendTaxInfo( DPID_ALLPLAYERS );
				UpdateToDB( CONT_ALL );
			}
			break;

		case QUERY_TAX_SETNEXT_TAXRATE :
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nCont;
				int nSalesTaxRate, nPurchaseTaxRate;
				ar >> nCont;
				ar >> nSalesTaxRate >> nPurchaseTaxRate;
				CTax::GetInstance()->SetNextTaxRate( nCont, nSalesTaxRate, nPurchaseTaxRate );
				CDPTrans::GetInstance()->SendTaxInfo( DPID_ALLPLAYERS );
				UpdateToDB( nCont );
			}
			break;

		case QUERY_TAX_ADDTAX :
			{
				CAr ar( pov->lpBuf, pov->uBufSize );
				BYTE nCont, nTaxKind;
				int nTax;
				ar >> nCont >> nTax >> nTaxKind;
				if( CTax::GetInstance()->AddTax( nCont, nTax, nTaxKind ) )
				{
 					//UpdateToDB( nCont );
 					//UpdateToDB( CONT_ALL );
					UpdateAllToDB();
				}
			}
			break;
	}	// switch
}

void CTaxDBController::OnTimer()
{
	BOOL bPay = CTax::GetInstance()->CheckPayTime();
	CTax::GetInstance()->CheckChangeTime( bPay );
}

void CTaxDBController::PayTaxToPost()
{
	CTax* tax = CTax::GetInstance();
	CQuery* pQuery = GetQueryObject();
	for( TAXINFOMAP::iterator it=tax->m_mapTaxInfo.begin(); it!=tax->m_mapTaxInfo.end(); it++ )
	{
		__TAXINFO* taxInfo = it->second;

		DWORD dwId = taxInfo->dwId;

		if( ( it->first != CONT_ALL ) && ( dwId != NULL_ID ) )
		{
			char szQuery[QUERY_SIZE] = {0,};
			sprintf( szQuery, "TAX_INFO_STR 'S3', '%02d', 0, 0, '%07d', '0', 0, '0'", g_appInfo.dwSys, dwId );
			if( pQuery->Exec( szQuery ) )
			{ 
				if( pQuery->Fetch() )
					dwId = static_cast<DWORD>( pQuery->GetInt( "m_idPlayer" ) <= CQuery::CQUERYNULL ? NULL_ID : pQuery->GetInt( "m_idPlayer" ) );
				else
					dwId = NULL_ID;
			}
			else
			{
				WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery );
				dwId = NULL_ID;
			}
		}

		if( dwId == NULL_ID )
			continue;

		for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
		{
			__TAXDETAIL* taxDetail = it2->second;

			CMail* pMail = new CMail;
			pMail->m_tmCreate	= ::time_null();
			pMail->m_idSender	= 0;	//
			pMail->m_nGold		= taxDetail->nTaxGold;
			switch( it2->first )
			{
				case TAX_SALES :
					sprintf( pMail->m_szTitle, prj.GetText(TID_GAME_TAX_PAY_SALES_TITLE),
												CContinent::GetInstance()->GetContinentName(it->first) );
					break;
				case TAX_PURCHASE :
					sprintf( pMail->m_szTitle, prj.GetText(TID_GAME_TAX_PAY_PURCHASE_TITLE),
												CContinent::GetInstance()->GetContinentName(it->first) );
					break;
				case TAX_ADMISSION :
					sprintf( pMail->m_szTitle, prj.GetText(TID_GAME_TAX_PAY_ADMISSION_TITLE),
												CContinent::GetInstance()->GetContinentName(it->first) );
					break;
			} // switch
			char strDate[100]={0,};
			CTime cTime = CTime::GetCurrentTime() - CTimeSpan( 1, 0, 0, 0 );
			sprintf( strDate, prj.GetText( TID_GAME_TAX_PAY_DATE ), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay() );
			sprintf( pMail->m_szText, prj.GetText( TID_GAME_TAX_PAY_INFO ), strDate, taxDetail->nTaxCount, taxDetail->nTaxPerin, taxDetail->nTaxGold );
			if( taxDetail->nTaxPerin )
			{
				pMail->m_pItemElem	= new CItemElem;
				pMail->m_pItemElem->m_dwItemId	= II_SYS_SYS_SCR_PERIN;
				pMail->m_pItemElem->m_nItemNum	= static_cast<short>( taxDetail->nTaxPerin );
				pMail->m_pItemElem->SetSerialNumber( xRand() );
			}

			post::Post( dwId, pMail, pQuery );
			
			taxDetail->nTaxCount = 0;
			taxDetail->nTaxGold = 0;
			taxDetail->nTaxPerin = 0;
		} // for it2
	}	// for it
}


void CTaxDBController::LoadTaxInfo()
{
	CTax* tax = CTax::GetInstance();
	CQuery* pQuery = GetQueryObject();

	char szQuery[QUERY_SIZE] = {0,};
	// �ε��� ������
	sprintf( szQuery, "TAX_INFO_STR 'S1', '%02d', 0, 0, '0', '0', 0, '0'", g_appInfo.dwSys );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
	if( pQuery->Fetch() )
		m_nTimes = pQuery->GetInt( "nTimes" );
	
	if( m_nTimes <= 0 ) // DB�� �ƹ��͵� ������� �������
	{
		// ���� �ð��� �����´�.
		if( tax->m_Lua.GetLuaFunction( "GetNowDate" ) )
		{
			tax->m_Lua.CallLuaFunction( 0, 1 );
			tax->m_strChangedDate = tax->m_Lua.ToString( -1 );
		}
		tax->m_Lua.Pop(0);

		InsertToDB();	// dummy��(�ʱⰪ)�� insert �Ѵ�.
		return;
	}

	for( TAXINFOMAP::iterator it=tax->m_mapTaxInfo.begin(); it!=tax->m_mapTaxInfo.end(); it++ )
	{
		// DB�� ����� row�� ������ ������ �о�´�.
		BYTE nContinent = it->first;
		__TAXINFO* taxInfo = it->second;
		
		sprintf( szQuery, "TAX_INFO_STR 'S2', '%02d', %d, %d, '0', '0', 0, '0'", g_appInfo.dwSys, m_nTimes, nContinent );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
		while( pQuery->Fetch() )
		{
			taxInfo->dwId = static_cast<DWORD>( pQuery->GetInt( "dwID" ) );
			if( taxInfo->dwId == 0 ) taxInfo->dwId = NULL_ID;
			taxInfo->dwNextId = static_cast<DWORD>( pQuery->GetInt( "dwNextID" ) );
			if( taxInfo->dwNextId == 0 ) taxInfo->dwNextId = NULL_ID;
			taxInfo->bSetTaxRate = static_cast<BOOL>( pQuery->GetInt( "bSetTaxRate" ) );
			char szDate[16] = {0,}; pQuery->GetStr( "change_time", szDate );
			tax->m_strChangedDate = szDate;
		}
	
		// �� ����
		sprintf( szQuery, "TAX_DETAIL_STR 'S2', '%02d', %d, %d, 0, 0, 0, 0, 0, 0", g_appInfo.dwSys, m_nTimes, nContinent );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
		while( pQuery->Fetch() )
		{
			BYTE nTaxKind = static_cast<BYTE>( pQuery->GetInt( "nTaxKind" ) );
			if( taxInfo->mapTaxDetail.find(nTaxKind) == taxInfo->mapTaxDetail.end() )
			{
				WriteLog( "LoadTaxInfo() - taxDetail is wrong, nContinent = %d, nTaxKind = %d", nContinent, nTaxKind );
				return;
			}

			__TAXDETAIL* taxDetail = taxInfo->mapTaxDetail.find(nTaxKind)->second;
			taxDetail->nTaxRate = pQuery->GetInt( "nTaxRate" );
			taxDetail->nTaxCount = pQuery->GetInt( "nTaxCount" );
			taxDetail->nTaxGold = pQuery->GetInt( "nTaxGold" );
			taxDetail->nTaxPerin = pQuery->GetInt( "nTaxPerin" );
			taxDetail->nNextTaxRate = pQuery->GetInt( "nNextTaxRate" );
		}
	}
}


void CTaxDBController::InsertToDB()
{
	CQuery* pQuery = GetQueryObject();

	char szQuery[QUERY_SIZE] = {0,};
	
	m_nTimes++;
	CTax* pTax = CTax::GetInstance();
	for( TAXINFOMAP::iterator it=pTax->m_mapTaxInfo.begin(); it!=pTax->m_mapTaxInfo.end(); it++ )
	{
		__TAXINFO* taxInfo = it->second;
		DWORD dwTempId = 0, dwTempNextId = 0;
		if( taxInfo->dwId != NULL_ID ) dwTempId = taxInfo->dwId;
		if( taxInfo->dwNextId != NULL_ID ) dwTempNextId = taxInfo->dwNextId;
		sprintf( szQuery, "TAX_INFO_STR 'I1', '%02d', %d, %d, '%07d', '%07d', %d, '%s'",
				g_appInfo.dwSys, m_nTimes, it->first, dwTempId, dwTempNextId, taxInfo->bSetTaxRate, pTax->m_strChangedDate.c_str() );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
				
		for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
		{
			__TAXDETAIL* taxDetail = it2->second;
			sprintf( szQuery, "TAX_DETAIL_STR 'I1', '%02d', %d, %d, %d, %d, %d, %d, %d, %d",
					g_appInfo.dwSys, m_nTimes, it->first, it2->first, taxDetail->nTaxRate, taxDetail->nTaxCount,
					taxDetail->nTaxGold, taxDetail->nTaxPerin, taxDetail->nNextTaxRate );
			if( pQuery->Exec( szQuery ) == FALSE )
			{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
		}
	}
}

void CTaxDBController::UpdateToDB( BYTE nContinent )
{
	CQuery* pQuery = GetQueryObject();

	char szQuery[QUERY_SIZE] = {0,};

	__TAXINFO* taxInfo = CTax::GetInstance()->GetTaxInfo( nContinent );
	if( !taxInfo )
	{
		WriteLog( "UpdateToDB() - taxInfo is NULL, nContinent = %x", nContinent );
		return;
	}

	DWORD dwTempId = 0, dwTempNextId = 0;
	if( taxInfo->dwId != NULL_ID ) dwTempId = taxInfo->dwId;
	if( taxInfo->dwNextId != NULL_ID ) dwTempNextId = taxInfo->dwNextId;
	sprintf( szQuery, "TAX_INFO_STR 'U1', '%02d', %d, %d, '%07d', '%07d', %d, '0'",
		g_appInfo.dwSys, m_nTimes, nContinent, dwTempId, dwTempNextId, taxInfo->bSetTaxRate );
	if( pQuery->Exec( szQuery ) == FALSE )
	{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }

	for( TAXDETAILMAP::iterator it2=taxInfo->mapTaxDetail.begin(); it2!=taxInfo->mapTaxDetail.end(); it2++ )
	{
		__TAXDETAIL* taxDetail = it2->second;
		sprintf( szQuery, "TAX_DETAIL_STR 'U1', '%02d', %d, %d, %d, %d, %d, %d, %d, %d",
			g_appInfo.dwSys, m_nTimes, nContinent, it2->first, taxDetail->nTaxRate, taxDetail->nTaxCount,
			taxDetail->nTaxGold, taxDetail->nTaxPerin, taxDetail->nNextTaxRate );
		if( pQuery->Exec( szQuery ) == FALSE )
		{ WriteLog( "%s, %d\t%s", __FILE__, __LINE__, szQuery ); return; }
	}
}

void CTaxDBController::UpdateAllToDB()
{
	CTax* tax = CTax::GetInstance();
	for( TAXINFOMAP::iterator it=tax->m_mapTaxInfo.begin(); it!=tax->m_mapTaxInfo.end(); it++ )
		UpdateToDB( it->first );
}
#endif // __DBSERVER

#endif // TAX