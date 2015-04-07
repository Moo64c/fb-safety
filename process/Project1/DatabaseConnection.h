#pragma once
#include "tableStructs.h"
#include "my_global.h"
#include "mysql.h"
#include <vector>

using namespace std;

class DatabaseConnection
{
public:
	DatabaseConnection(const char *_host = DEFAULT_HOST, const char *_user = DEFAULT_USER,
		const char *_pass = DEFAULT_PASSWORD, const char *_db = DEFAULT_DATABASE);
	virtual ~DatabaseConnection();

	rawEventEntry_t getNextRow();
	vector<corpusWord_t> getWords();

	void connect();
	void runRawQuery();

	void setHost(const char *_newHost) { host = _newHost; }
	void setUser(const char *_newUser) { user = _newUser; }
	void setPass(const char *_newPass) { pass = _newPass; }
	void setDB(const char *_newDB) { db = _newDB; }

	static const char* DEFAULT_HOST;
	static const char* DEFAULT_USER;
	static const char* DEFAULT_PASSWORD;
	static const char* DEFAULT_DATABASE;

protected:
	void runProc(char *proc);
	MYSQL *con;
	string host;
	string user;
	string pass;
	string db;
};