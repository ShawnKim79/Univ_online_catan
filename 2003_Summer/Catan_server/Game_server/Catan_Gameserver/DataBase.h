// DataBase.h: interface for the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class CDataBase  
{
public:
	CDataBase();
	virtual ~CDataBase();

	void PrintDiag();
	void WriteBlob(LPCTSTR szSQL, void *buf, int size);
	int ReadBlob(LPCTSTR szSQL, void *buf);
	void GetStr(char *sCol, char *buf);
	void GetStr(int nCol, char *buf);
	int GetInt(char *sCol);
	int GetInt(int nCol);
	void Clear();
	SQLRETURN Fetch();
	void ExecGetStr(LPCTSTR szSQL, char *buf);
	int ExecGetInt(LPCTSTR szSQL);
	BOOL SQLQuery(LPCTSTR szSQL);
	BOOL Connect(int nType, char *ConStr, char *UID=NULL, char *PWD=NULL);
	Disconnect();

public:
	int GetRowCount();
	enum { MAXCOL = 100, BLOBBATCH = 10000, QUERYNULL = -100, QUERYEOF = -101, QUERYNOCOL = -102, QUERYERROR = -103 };
	SQLINTEGER		AffectCount;
	SQLHSTMT		hStmt;
	SQLSMALLINT		nCol;
	SQLCHAR			ColName[MAXCOL][50];
	SQLINTEGER		lCol[MAXCOL];


private:
	int FindCol(char *name);

private:
	SQLHENV		hEnv;
	SQLHDBC		hDbc;
	SQLRETURN	ret;
	char Col[MAXCOL][255];
};

#endif