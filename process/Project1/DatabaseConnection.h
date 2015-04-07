#pragma once
#include "tableStructs.h"
#include "my_global.h"
#include "mysql.h"
#include <vector>

class DatabaseConnection
{
public:
	DatabaseConnection();
	virtual ~DatabaseConnection();

	rawEventEntry_t getNextRow();
	vector<corpusWord_t> getWords();

	void connect();
	void runRawQuery();

protected:
	void runProc(char *proc);
	MYSQL *con;
};