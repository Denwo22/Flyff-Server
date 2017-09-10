#pragma once

extern "C"
{
#include "..\_Lua\include\lua.h"
#include "..\_Lua\include\lualib.h"
#include "..\_Lua\include\lauxlib.h"
}

class CLuaBase
{
public:
	CLuaBase();
	virtual ~CLuaBase();

	static int	_TRACE( lua_State* pLuaState );
	static int	_ERROR( lua_State* pLuaState );
	static int	_DOFILE( lua_State* pLuaState );

	void	RegisterFunction( char* szFromLuaFunc, lua_CFunction ToCFunc );
	int		RunScript( const char* szFileName );
public:
	inline	bool	IsNil( int nStackPos )					{ return lua_isnil( m_pLuaState, nStackPos ); }
	inline	bool	IsTable( int nStackPos )				{ return lua_istable( m_pLuaState, nStackPos ); }
	inline	bool	IsString( int nStackPos )				{ return lua_isstring( m_pLuaState, nStackPos ); }
public:
	inline	void	PushNumber( double nValue )				{ lua_pushnumber( m_pLuaState, nValue ); }
	inline	void	PushString( const char* szValue )		{ lua_pushstring( m_pLuaState, szValue ); }
	inline	void	PushPointer( void* p )					{ lua_pushlightuserdata( m_pLuaState, p ); }
	inline	void	PushBool( int bValue )					{ lua_pushboolean( m_pLuaState, bValue ); }
	inline	void	PushNil()								{ lua_pushnil( m_pLuaState ); }
	inline	void	Pop( int nPop );
public:
	inline	double		ToNumber( int nStackPos )			{ return lua_tonumber( m_pLuaState, nStackPos ); }
	inline	const char*	ToString( int nStackPos )			{ return lua_tostring( m_pLuaState, nStackPos ); }
	inline	bool		ToBool( int nStackPos )				{ return static_cast< BOOL >( lua_toboolean( m_pLuaState, nStackPos ) ); }
	inline	int			TableLoop( int nStackPos )			{ return lua_next( m_pLuaState, nStackPos ); }
public:
	void		GetGloabal( char* szName )					{ lua_getglobal( m_pLuaState, szName ); }
	double		GetGlobalNumber( char* szName );
	const char* GetGlobalString( char* szName );
	BOOL		GetGlobalBool( char* szName );
	void		GetField( int nStackPos, char* szName )		{ lua_getfield( m_pLuaState, nStackPos, szName ); }
	double		GetFieldToNumber( int nStackPos, char* szName );
	const char* GetFieldToString( int nStackPos, char* szName );
	BOOL		GetFieldToBool( int nStackPos, char* szName );

	BOOL		GetLuaFunction( char* szFunc );
	void		CallLuaFunction( int nElem, int nReturn );
public:
	inline	lua_State*	GetLuaState() { return m_pLuaState; }

	inline	void	GetGlobal( _In_ char* szName )			{ lua_getglobal( m_pLuaState, szName ); }
	inline size_t	GetRawLen( int nStackPos = -1 )			{ return lua_rawlen( m_pLuaState, nStackPos ); }
	inline void		GetTable( int nStackPos )				{ lua_gettable( m_pLuaState, nStackPos ); }
	inline void		GetRawI( int nStackPos, int n )			{ lua_rawgeti( m_pLuaState, nStackPos, n ); }
private:
	lua_State*	m_pLuaState;
};
