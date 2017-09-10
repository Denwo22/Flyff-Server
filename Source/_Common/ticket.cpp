#include "stdafx.h"
#include "ticket.h"

CTicketProperty::CTicketProperty()
{
}

CTicketProperty::~CTicketProperty()
{
	m_mapTicketProp.clear();
}

BOOL	CTicketProperty::IsTarget( DWORD dwWorldId )
{
	for( map<DWORD, TicketProp>::iterator i = m_mapTicketProp.begin(); i != m_mapTicketProp.end(); ++i )
	{
		if( i->second.dwWorldId == dwWorldId )
			return TRUE;
	}
	return FALSE;
}

TicketProp*	CTicketProperty::GetTicketProp( DWORD dwItemId )
{
	map<DWORD, TicketProp>::iterator i	= m_mapTicketProp.find( dwItemId );
	if( i != m_mapTicketProp.end() )
		return &i->second;
	return NULL;
}

CTicketProperty*	CTicketProperty::GetInstance( void )
{
	static	CTicketProperty sTicketProperty;
	return &sTicketProperty;
}

BOOL	CTicketProperty::LoadScript()
{
	CScript s;
	if( s.Load( "ticket.inc" ) == FALSE )
		return FALSE;

	DWORD dwItemId	= s.GetNumber();
	while( s.tok != FINISHED )
	{
		TicketProp	tp;
		tp.dwWorldId	= s.GetNumber();
		tp.vPos.x	= s.GetFloat();
		tp.vPos.y	= s.GetFloat();
		tp.vPos.z	= s.GetFloat();
		bool b	= m_mapTicketProp.insert( map<DWORD, TicketProp>::value_type( dwItemId, tp ) ).second;
		ASSERT( b );
		dwItemId	= s.GetNumber();
	}
#ifdef __AZRIA_1023
	return m_lp.LoadScript();
#endif	// __AZRIA_1023
}

#ifdef __AZRIA_1023
CLayerProperty::CLayerProperty()
{
}

CLayerProperty::~CLayerProperty()
{
}

BOOL CLayerProperty::LoadScript()
{
	CScript s;
	if( s.Load( "layer.inc" ) == FALSE )
		return FALSE;
	DWORD dwWorldId	= s.GetNumber();
	while( s.tok != FINISHED )
	{
		LayerStruct ls;
		ls.dwWorldId	= dwWorldId;
		ls.nExpand	= s.GetNumber();
		m_vLayers.push_back( ls );
		dwWorldId	= s.GetNumber();
	}
	return TRUE;
}

int CLayerProperty::GetExpanedLayer( DWORD dwWorldId )
{
	for( VLS::iterator i = m_vLayers.begin(); i != m_vLayers.end(); ++i )
		if( ( *i ).dwWorldId == dwWorldId )
			return ( *i ).nExpand;
	return 0;
}
#endif	// __AZRIA_1023