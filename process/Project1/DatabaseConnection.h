#pragma once
#include "tableStructs.cpp"
#include 
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
};