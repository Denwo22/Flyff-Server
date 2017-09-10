// Continent.cpp: implementation of the CContinent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Continent.h"
#include "defineText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#if __VER >= 12 // __SECRET_ROOM

CContinent::CContinent()
{
	Init();
}

CContinent::~CContinent()
{

}

CContinent* CContinent::GetInstance( void )
{
	static CContinent sContinent;
	return &sContinent;	
}

void CContinent::Init()
{
	m_MapCont.clear();
	vector<CPoint> vecMap;

#if __VER >= 15 // __USING_CONTINENT_DATA// 대륙 경계정보 Data연결중 
	// 세금이나 비밀의 방에 관여하는 대륙은 마드리갈에만(메인월드) 존재하며 앞으로도로 그럴꺼라고 가정한다.
	const char* szWorldName = "WdMadrigal";				
	CString strDir = MakePath( DIR_WORLD, szWorldName );
	CString strFullFileName = strDir + CString( "\\" ) + CString(szWorldName) + CString(".wld.cnt");

	CScript script;
	if( script.Load( strFullFileName ) == FALSE )
		return;

	int id = 0;
	BOOL bTown = FALSE;

	do{
		script.GetToken( );

		if( script.Token == _T( "Continent" ) )
		{
			script.GetToken();
			if( script.Token == _T( "BEGIN" ) )
			{
				vecMap.clear( );
			}
			else if( script.Token == _T( "END" ) )
			{
				if( !vecMap.empty() )
					vecMap.push_back( vecMap[ 0 ] );

				pair< map< BYTE, vector<CPoint> >::iterator, BOOL > rst;

				// gmpbigsun(100409): bTown이 TRUE라면 이구역은 마을이라는 의미고, 현재는 Client 맵관련 정보를 위해 사용되지만
				// Server에서도 충분히 사용가능하고, 비용이 아주 적기때문에 풀어둠.
				rst = ( bTown ? m_MapContTown.insert( make_pair( id, vecMap ) ) : m_MapCont.insert( make_pair( id, vecMap ) ) ) ;
	
				assert( rst.second );

				bTown = FALSE;		//reset town
			}
		}
		else
		if( script.Token == _T( "C_id" ) )
		{
			id = script.GetNumber( );
		}
		else
		if( script.Token == _T( "VERTEX" ) )
		{
			float x = script.GetFloat();
			float y = script.GetFloat();
			float z = script.GetFloat();
			vecMap.push_back( CPoint( (int)( x ), (int)( z ) ) );
		}	
		else
		if( script.Token == _T( "TOWN" ) )
		{
			bTown = script.GetNumber( );
		}
		else 
		if( script.Token == _T( "C_useRealData" ) )
		{
			BOOL bReal = script.GetNumber( );
			if( !bReal )
			{
				//클라전용 비쥬얼변화 데이터로만 쓰인다. 클라는 월드 로드시 해당 대륙만 자동로드됨.
				do{
					script.GetToken( );
				}while( script.Token != "END" );		//한대륙 pass 
			}
		} 

	}while( script.tok != FINISHED );

#else	
	// 플라리스
	vecMap.clear();
	vecMap.push_back( CPoint(6159, 3407) );
	vecMap.push_back( CPoint(6350, 3825) );
	vecMap.push_back( CPoint(6736, 4649) );
	vecMap.push_back( CPoint(7246, 4570) );
	vecMap.push_back( CPoint(8258, 5022) );
	vecMap.push_back( CPoint(8213, 4367) );
	vecMap.push_back( CPoint(8021, 4037) );
	vecMap.push_back( CPoint(8017, 3496) );
	vecMap.push_back( CPoint(7245, 2685) );
	vecMap.push_back( CPoint(6783, 2631) );
	vecMap.push_back( CPoint(6366, 2922) );
	vecMap.push_back( CPoint(6159, 3407) );
	
	m_MapCont.insert( make_pair(CONT_FLARIS, vecMap) );
	
	// 세인트모닝
	vecMap.clear();
	vecMap.push_back( CPoint(7253, 2684) );
	vecMap.push_back( CPoint(8023, 3496) );
	vecMap.push_back( CPoint(8029, 4039) );
	vecMap.push_back( CPoint(8217, 4365) );
	vecMap.push_back( CPoint(8609, 4187) );
	vecMap.push_back( CPoint(9489, 3047) );
	vecMap.push_back( CPoint(9289, 2130) );
	vecMap.push_back( CPoint(8765, 1615) );
	vecMap.push_back( CPoint(7855, 1554) );
	vecMap.push_back( CPoint(7506, 1870) );
	vecMap.push_back( CPoint(7253, 2684) );
	
	m_MapCont.insert( make_pair(CONT_SAINTMORNING, vecMap) );
	
	// 리시스정원
	vecMap.clear();
	vecMap.push_back( CPoint(9123, 3339) );
	vecMap.push_back( CPoint(8922, 3740) );
	vecMap.push_back( CPoint(8607, 4127) );
	vecMap.push_back( CPoint(8285, 4397) );
	vecMap.push_back( CPoint(8316, 4748) );
	vecMap.push_back( CPoint(8833, 4762) );
	vecMap.push_back( CPoint(9612, 4992) );
	vecMap.push_back( CPoint(10436,4396) );
	vecMap.push_back( CPoint(9441, 3171) );
	vecMap.push_back( CPoint(9123, 3339) );
	
	m_MapCont.insert( make_pair(CONT_RICIS, vecMap) );
	
	// 다콘1,2
	vecMap.clear();
	vecMap.push_back( CPoint(4858, 2781) );
	vecMap.push_back( CPoint(4561, 3613) );
	vecMap.push_back( CPoint(4051, 3835) );
	vecMap.push_back( CPoint(3796, 3790) );
	vecMap.push_back( CPoint(3574, 4200) );
	vecMap.push_back( CPoint(3455, 4479) );
	vecMap.push_back( CPoint(3072, 4563) );
	vecMap.push_back( CPoint(2833, 5237) );
	vecMap.push_back( CPoint(3572, 5185) );
	vecMap.push_back( CPoint(4863, 4778) );
	vecMap.push_back( CPoint(6756, 4875) );
	vecMap.push_back( CPoint(6558, 4367) );
	vecMap.push_back( CPoint(6387, 3826) );
	vecMap.push_back( CPoint(6102, 3298) );
	vecMap.push_back( CPoint(5795, 2727) );
	vecMap.push_back( CPoint(4858, 2781) );
	
	m_MapCont.insert( make_pair(CONT_DARKON12, vecMap) );
	
	// 다콘3
	vecMap.clear();
	vecMap.push_back( CPoint(2834, 5241) );
	vecMap.push_back( CPoint(3069, 4562) );
	vecMap.push_back( CPoint(3451, 4476) );
	vecMap.push_back( CPoint(3567, 4196) );
	vecMap.push_back( CPoint(3792, 3791) );
	vecMap.push_back( CPoint(4049, 3736) );
	vecMap.push_back( CPoint(4555, 3615) );
	vecMap.push_back( CPoint(4854, 2775) );
	vecMap.push_back( CPoint(3395, 1291) );
	vecMap.push_back( CPoint(1291, 2425) );
	vecMap.push_back( CPoint(1867, 3867) );
	vecMap.push_back( CPoint(1946, 4694) );
	vecMap.push_back( CPoint(2834, 5241) );
	
	m_MapCont.insert( make_pair(CONT_DARKON3, vecMap) );

#if __VER >= 14 // __NEW_CONTINENT
	// 하르모닌
	vecMap.clear();
	vecMap.push_back( CPoint(6037, 4875) );
	vecMap.push_back( CPoint(6484, 5060) );
	vecMap.push_back( CPoint(7800, 5600) );
	vecMap.push_back( CPoint(7990, 5938) );
	vecMap.push_back( CPoint(7940, 6100) );
	vecMap.push_back( CPoint(7562, 6384) );
	vecMap.push_back( CPoint(7170, 6616) );
	vecMap.push_back( CPoint(6897, 6631) );
	vecMap.push_back( CPoint(6561, 6768) );
	vecMap.push_back( CPoint(5681, 6777) );
	vecMap.push_back( CPoint(5544, 6379) );
	vecMap.push_back( CPoint(5317, 6079) );
	vecMap.push_back( CPoint(5200, 5900) );
	vecMap.push_back( CPoint(5231, 4875) );
	
	m_MapCont.insert( make_pair(CONT_HARMONIN, vecMap) );
#endif // __NEW_CONTINENT

#endif

}

BOOL CContinent::Point_In_Poly( vector<CPoint> vecMap, CPoint test_point )
{
	int counter = 0;
	int i;
	double xinters;
	CPoint p1,p2;
	
	p1 = vecMap[0];
	for( i=1;i<=(int)( vecMap.size() );i++ )
	{
		p2 = vecMap[i % vecMap.size()];
		
		if (test_point.y > min_cont(p1.y,p2.y))
		{
			if (test_point.y <= max_cont(p1.y,p2.y))
			{
				if (test_point.x <= max_cont(p1.x,p2.x))
				{
					if (p1.y != p2.y)
					{
						xinters = (test_point.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						
						if (p1.x == p2.x || test_point.x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0)
		return FALSE;
	else
		return TRUE;
}

#ifndef __DBSERVER
BYTE CContinent::GetContinent( CMover* pMover )
{
	if( !CContinent::IsValidObj( pMover ) )
		return CONT_NODATA;

	switch( pMover->GetWorld()->m_dwWorldID )
	{
		case WI_WORLD_MADRIGAL:	return GetContinent( pMover->GetPos() );
		case WI_WORLD_ARENA:	return CONT_DARKON12;	// 아레나는 다콘 1, 2 지구에 포함
		case WI_WORLD_GUILDWAR : return CONT_FLARIS;	// 길드대전장은 플라리스
	}

	return CONT_NODATA;
}

BYTE CContinent::GetArea( CMover* pMover )
{
	if( !CContinent::IsValidObj( pMover ) )
		return CONT_NODATA;


	return GetArea( GetContinent(pMover) );	
}

BOOL CContinent::IsValidObj( CMover* pMover )
{
	return ( ::IsValidObj( pMover ) && pMover->GetWorld() );
}

#endif // !__DBSERVER

BYTE CContinent::GetContinent( const D3DXVECTOR3& vPos )
{
	CPoint  cpoint;
	vector<CPoint>	vecPoint;

	cpoint.x    = (long)( vPos.x );
	cpoint.y    = (long)( vPos.z );

	for( map< BYTE, vector<CPoint> >::iterator iter = m_MapCont.begin(); iter != m_MapCont.end(); ++iter )
	{
		vecPoint  = iter->second;	
		if( Point_In_Poly( vecPoint, cpoint ) )
			return iter->first;
	}

	return CONT_NODATA;
}

BYTE CContinent::GetTown( const D3DXVECTOR3& vPos )
{
	CPoint  cpoint;
	vector<CPoint>	vecPoint;

	cpoint.x    = (long)( vPos.x );
	cpoint.y    = (long)( vPos.z );

	for( map< BYTE, vector<CPoint> >::iterator iter = m_MapContTown.begin(); iter != m_MapContTown.end(); ++iter )
	{
		vecPoint  = iter->second;	
		if( Point_In_Poly( vecPoint, cpoint ) )
			return iter->first;
	}

	return CONT_NODATA;
}

BYTE CContinent::GetArea( BYTE nCont )
{
#if __VER >= 15 // __USING_CONTINENT_DATA
	if( nCont == CONT_NODATA )
		return CONT_NODATA;
	else if( nCont <= CONT_EAST )
		return CONT_EAST;
	else if( nCont >= CONT_WEST )
		return CONT_WEST;

	return CONT_NODATA;
#else // __USING_CONTINENT_DATA
	if( nCont & CONT_EAST )
		return CONT_EAST;
	else if( nCont & CONT_WEST )
		return CONT_WEST;
	else
		return CONT_NODATA;
#endif // __USING_CONTINENT_DATA
}

CString CContinent::GetContinentName( BYTE nCont )
{
	CString strTemp;
	
	switch( nCont )
	{
		case CONT_FLARIS :
			strTemp = prj.GetText( TID_GAME_CONT_FLARIS );
			break;
		case CONT_SAINTMORNING :
			strTemp = prj.GetText( TID_GAME_CONT_SAINTMORNING );
			break;
		case CONT_DARKON12 :
			strTemp = prj.GetText( TID_GAME_CONT_DARKON12 );
			break;
		case CONT_DARKON3 :
			strTemp = prj.GetText( TID_GAME_CONT_DARKON3 );
			break;
		case CONT_RICIS :
			strTemp = prj.GetText( TID_GAME_CONT_RICIS );
			break;
		
		case CONT_EAST :
			strTemp = prj.GetText( TID_GAME_CONT_EAST );
			break;
		case CONT_WEST :
			strTemp = prj.GetText( TID_GAME_CONT_WEST );
			break;

		case CONT_ALL :
			strTemp = prj.GetText( TID_GAME_CONT_ALL );
			break;
	}

	return strTemp;
}

#ifdef __WORLDSERVER
D3DXVECTOR3 CContinent::GetRevivalPos( BYTE nContinent )
{
	D3DXVECTOR3 vPos;
	
	switch( nContinent )
	{
		case CONT_NODATA :			vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
		case CONT_FLARIS :			vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
		case CONT_SAINTMORNING :	vPos = D3DXVECTOR3( 8470, 0, 3635 );	break;
		case CONT_DARKON12 :		vPos = D3DXVECTOR3( 3808, 0, 4455 );	break;
		case CONT_DARKON3 :			vPos = D3DXVECTOR3( 3808, 0, 4455 );	break;
		case CONT_RICIS :			vPos = D3DXVECTOR3( 8470, 0, 3635 );	break;
		case CONT_EAST :			vPos = D3DXVECTOR3( 6959, 0, 3217 );	break;
		case CONT_WEST :			vPos = D3DXVECTOR3( 3824, 0, 4456 );	break;
		
		default :					vPos = D3DXVECTOR3( 6968, 0, 3328 );	break;
	}
	
	return vPos;
}
#endif // __WORLDSERVER

#if __VER >= 14 // __NEW_CONTINENT
#ifdef __CLIENT
int	CContinent::GetMapNo( CMover* pMover )
{
	if( IsValidObj( pMover ) && pMover->GetWorld()->GetID() == WI_WORLD_MADRIGAL )
		return GetMapNo( pMover->GetPos() );
	return 100;
}

int	CContinent::GetMapNo( const D3DXVECTOR3& vPos )
{
	BYTE nCont = GetContinent( vPos );
	switch( nCont )
	{
		case CONT_FLARIS :			return 5;
		case CONT_SAINTMORNING :	return 4;
		case CONT_DARKON12 :		return 1;
		case CONT_DARKON3 :			return 2;
		case CONT_RICIS :			return 3;
		case CONT_HARMONIN :		return 6;
#if __VER >= 15 // __NEW_CONTINENT15
		case CONT_ESTIA :			return 7;
#endif // __NEW_CONTINENT15
		default:					return 100;
	}
}
#endif // __CLIENT
#endif // __NEW_CONTINENT

#endif // __SECRET_ROOM

#if __VER >= 15 // __BS_CHANGING_ENVIR
int CContinent::GetContinentIndex( const char* continentName )
{
	if( strcmp( "Flaris", continentName ) == 0 )
	{
		return CONT_FLARIS;
	}else 
	if( strcmp( "Saint", continentName ) == 0 )
	{
		return CONT_SAINTMORNING;
	}else
	if( strcmp( "Ricis", continentName ) == 0 )
	{
		return CONT_RICIS;
	}else
	if( strcmp( "Darkon12", continentName ) == 0 )
	{
		return CONT_DARKON12;
	}
	else
	if( strcmp( "Darkon3", continentName ) == 0 )
	{
		return CONT_DARKON3;
	}
	else
	if( strcmp( "Harmonin", continentName ) == 0 )
	{
		return CONT_HARMONIN;
	}

	return CONT_NODATA;
}
#endif