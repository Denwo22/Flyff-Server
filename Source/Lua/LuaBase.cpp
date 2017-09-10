#include "stdafx.h"
#include "tools.h"
#include "LuaBase.h"

CLuaBase::CLuaBase()
{
	m_pLuaState = luaL_newstate();

	luaL_openlibs( m_pLuaState );

	RegisterFunction( "TRACE", _TRACE );
	RegisterFunction( "ERROR", _ERROR );
	RegisterFunction( "DoFile", _DOFILE );
}

CLuaBase::~CLuaBase()
{
	//if( m_pLuaState )
	//	lua_close( m_pLuaState );
}

int CLuaBase::_TRACE( lua_State* pLuaState )
{
	Error( "LUA - %s\n", lua_tostring(pLuaState, -1) );
	return NULL;
}

int CLuaBase::_ERROR( lua_State* pLuaState )
{
	Error( "LUA - %s", lua_tostring(pLuaState, -1) );
	return NULL;
}

int CLuaBase::_DOFILE( lua_State* pLuaState )
{
	lua_gettop( pLuaState );

	if( !lua_isstring( pLuaState, 1 ) )
	{
		Error( "%s(%d): %s", __FUNCTION__, __LINE__, lua_tostring( pLuaState, -1 ) );
		return NULL;
	}

	if( luaL_dofile( pLuaState, lua_tostring( pLuaState, 1 ) ) )
	{
		Error( "%s(%d): %s", __FUNCTION__, __LINE__, lua_tostring( pLuaState, -1 ) );
		return NULL;
	}
	return 1;
}

void CLuaBase::RegisterFunction( char* szFromLuaFunc, lua_CFunction ToCFunc )
{
	lua_register( m_pLuaState, szFromLuaFunc, ToCFunc );
}

int CLuaBase::RunScript( const char* szFileName )
{
	int nReturn = luaL_dofile( m_pLuaState, szFileName );
	if( nReturn != 0 )
		Error( ToString( -1 ) );
	return nReturn;
}

BOOL CLuaBase::GetLuaFunction( char* szFunc )
{
	lua_getglobal( m_pLuaState, szFunc );
	return lua_isfunction( m_pLuaState, -1 );
}

void CLuaBase::CallLuaFunction( int nElem, int nReturn )
{
	if( lua_pcall( m_pLuaState, nElem, nReturn, 0 ) != 0 )
	{
		Error( ToString( -1 ) );
		exit( 0 );
	}
}

double CLuaBase::GetGlobalNumber( char* szName )
{
	double fValue = 0;
	lua_getglobal( m_pLuaState, szName );
	if( lua_isnumber( m_pLuaState, -1 ) )
		fValue = ToNumber( -1 );
	Pop(1);

	return fValue;
}

const char* CLuaBase::GetGlobalString( char* szName )
{
	char* szValue = NULL;
	lua_getglobal( m_pLuaState, szName );
	if( lua_isstring( m_pLuaState, -1 ) )
		szValue = (char*)ToString( -1 );

	Pop( 1 );

	return szValue;
}

BOOL CLuaBase::GetGlobalBool( char* szName )
{
	BOOL bValue = FALSE;
	lua_getglobal( m_pLuaState, szName );
	if( lua_isboolean( m_pLuaState, -1 ) )
		bValue = ToBool( -1 );

	Pop( 1 );

	return bValue;
}

void CLuaBase::Pop( int nPop )
{  
	if( nPop == 0 )
		lua_settop( m_pLuaState, 0 );
	else
		lua_pop( m_pLuaState, nPop );
}

double CLuaBase::GetFieldToNumber( int nStackPos, char* szName )
{
	double fValue = 0;
	lua_getfield( m_pLuaState, nStackPos, szName );
	if( lua_isnumber( m_pLuaState, -1 ) )
		fValue = ToNumber( -1 );
	Pop( 1 );
	return fValue;
}

const char* CLuaBase::GetFieldToString( int nStackPos, char* szName )
{
	char* szValue = NULL;
	lua_getfield( m_pLuaState, nStackPos, szName );
	if( lua_isstring( m_pLuaState, -1 ) )
		szValue = (char*)ToString( -1 );
	Pop( 1 );
	
	return szValue;
}

BOOL CLuaBase::GetFieldToBool( int nStackPos, char* szName )
{
	int bValue = 0;
	lua_getfield( m_pLuaState, nStackPos, szName );
	if( lua_isboolean( m_pLuaState, -1 ) )
		bValue = ToBool( -1 );
	Pop( 1 );
	
	return (bValue == 0 ? FALSE : TRUE );
}
