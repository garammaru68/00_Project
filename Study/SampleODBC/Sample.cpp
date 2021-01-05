#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#pragma comment(lib, "odbc32.lib")

void main()
{
	setlocale(LC_ALL, "KOREAN");

	SQLHENV hEnv = SQL_NULL_HENV;
	SQLHENV hDbc = SQL_NULL_HDBC;
	SQLHENV hStmt = SQL_NULL_HSTMT;

	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	TCHAR InCon[256] = { 0, };
	TCHAR Dir[MAX_PATH];
	SQLSMALLINT cbCon;
	GetCurrentDirectory(MAX_PATH, Dir);
	wsprintf(InCon, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"), Dir, L"DemoGame.accdb");
	ret = SQLDriverConnect(hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbCon, SQL_DRIVER_NOPROMPT);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (ret != SQL_SUCCESS)
	{
		return;
	}
	{
		SQLTCHAR userid[30] = { 0, };
		SQLTCHAR userps[30] = { 0, };
		SQLINTEGER id;
		SQLLEN lID, lUserID, IUserPS;
		SQLBindCol(hStmt, 1, SQL_INTEGER, &id, 0, &lID);
	}


}