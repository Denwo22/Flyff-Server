#ifndef _QUERY_H_
#define _QUERY_H_

#include <sql.h>
#include <sqlext.h>

struct QUERY_BINDINFO
{
	SQLSMALLINT targetType;
	SQLPOINTER	targetValue;
	SQLINTEGER	bufferLength;
	SQLINTEGER*	strLen_or_Ind;
};

class CQuery
{
public:
//	char *DBName, *DBId, *DBPass;	
	char DBName[256];
	char DBId[256];
	char DBPass[256];

	// �ִ� �÷���, BLOB ����� ����, NULL �ʵ尪
	enum { MAXCOL=256, BLOBBATCH=10000, CQUERYNULL=-100, CQUERYEOF=-101, 
		CQUERYNOCOL=-102, CQUERYERROR=-103 };
private:
	SQLHENV hEnv;							// ȯ�� �ڵ�
	SQLHDBC hDbc;							// ���� �ڵ�
	char*	Col[MAXCOL];					// ���ε��� �÷� ����

	int FindCol(char *name);				// �÷��� �̸����κ��� ��ȣ�� ã���ش�.

public:
	SQLHSTMT hStmt;							// ��� �ڵ�. ���� ����� ���� �����Ƿ� public���� ����
	SQLSMALLINT nCol;						// �÷� ����
	SQLCHAR ColName[MAXCOL][50];			// �÷��� �̸���
	SQLINTEGER lCol[MAXCOL];				// �÷��� ����/���� ����

	CQuery();								// ������
	~CQuery();								// �ı���:���� �ڵ��� �����Ѵ�.

	void PrintDiag( LPCTSTR szSQL, SQLSMALLINT type = SQL_HANDLE_DBC );						// ���� ���� ���
	BOOL Connect(int Type, char *ConStr, char *UID=NULL, const char *PWD=NULL);	// ������ �ҽ��� �����Ѵ�.
	void DisConnect();						// ������ �ҽ� ������ ���´�
	BOOL Exec(LPCTSTR szSQL);				// SQL���� �����Ѵ�.
	BOOL Exec(LPCTSTR szSQL, int nCount, QUERY_BINDINFO info[]);
	BOOL PrepareFetch();

	BOOL	Execute( LPCTSTR lpszFormat, ... );

	int ExecGetInt(LPCTSTR szSQL);			// SQL���� �����ϰ� ù��° �÷��� ���� ����
	void ExecGetStr(LPCTSTR szSQL, char *buf);		// SQL���� �����ϰ� ù��° �÷��� ���ڿ� ����
	BOOL Fetch();						// �� �� ��������
	void Clear();							// Ŀ�� ���� �� �� ���ε�
	int GetInt(int nCol);					// ������ �÷� �б�
	int GetInt(char *sCol);					// ������ �÷� �б�

	__int64		GetInt64( int nCol );
	__int64		GetInt64( char* sCol );

	EXPINTEGER	GetExpInteger( int nCol )	{ return GetInt64( nCol );	}
	EXPINTEGER	GetExpInteger(	char* sCol ) { return GetInt64( sCol ); 	}
	SERIALNUMBER	GetSerialNumber( int nCol )
		{
			return GetInt( nCol );
		}
	SERIALNUMBER	GetSerialNumber( char* sCol )
		{
			return GetInt( sCol );
		}

	float GetFloat(int nCol);					// �Ǽ��� �÷� �б�
	float GetFloat(char *sCol);				// �Ǽ��� �÷� �б�
	char GetChar(int nCol);					// �Ǽ��� �÷� �б�
	char GetChar(char *sCol);				// �Ǽ��� �÷� �б�
	void GetStr(int nCol, char *buf);		// ���ڿ��� �÷� �б�
	void GetStr(char *sCol, char *buf);		// ���ڿ��� �÷� �б�
	int ReadBlob(LPCTSTR szSQL, void *buf);
	void WriteBlob(LPCTSTR szSQL, void *buf, int size);
	BOOL BindParameter(SQLUSMALLINT parameterNumber,
                           SQLSMALLINT inputOutputType,
                           SQLSMALLINT valueType,
                           SQLSMALLINT parameterType,
                           SQLUINTEGER columnSize,
                           SQLSMALLINT decimalDigits,
                           SQLPOINTER  parameterValuePtr,
                           SQLINTEGER bufferLength,
                           SQLINTEGER *strLen_or_IndPtr);
	
	BOOL MoreResults( void );
	

	void WriteLogFile(char *strLog,...);
	static BOOL EnableConnectionPooling();
};
// TODO: ��� �и� 
// ��ȣȭ�� ��ū�� �ص��ؼ� �н������ ��´�.
// ��: ��ū - 3A08DB22
extern BOOL GetPWDFromToken( const char* szToken, char* szPWD );
#endif