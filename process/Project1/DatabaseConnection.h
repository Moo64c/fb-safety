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
	userData_t getNextUserData();
	vector<corpusWord_t> getWords();

	int connect();
	int runRawQuery();

	void setHost(const char *_newHost) { host = _newHost; }
	void setUser(const char *_newUser) { user = _newUser; }
	void setPass(const char *_newPass) { pass = _newPass; }
	void setDB(const char *_newDB) { db = _newDB; }

	static const char* DEFAULT_HOST;
	static const char* DEFAULT_USER;
	static const char* DEFAULT_PASSWORD;
	static const char* DEFAULT_DATABASE;
	static const char* RAW_FACEBOOK_GET_NEW_ROWS_QUERY;
	static const char* GET_WORDS_QUERY;
	static const int NUMBER_OF_RAW_FIELDS;

protected:
	MYSQL *rawConn, *wordConn, *userConn;
	MYSQL_RES *raw_result;
	MYSQL_RES *word_result;
	MYSQL_RES *user_result;
	string host;
	string user;
	string pass;
	string db;
	vector<corpusWord_t> corpus;

	void initMySQLResult(MYSQL_RES *res);

};