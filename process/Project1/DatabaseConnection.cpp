#include "DatabaseConnection.h"
#include <regex>
#include "time.h"
#include <iostream>

const char* DatabaseConnection::DEFAULT_HOST = "localhost";
const char* DatabaseConnection::DEFAULT_USER = "root";
const char* DatabaseConnection::DEFAULT_PASSWORD = "qwerty";
const char* DatabaseConnection::DEFAULT_DATABASE = "test";
const char* DatabaseConnection::RAW_FACEBOOK_GET_NEW_ROWS_QUERY = "CALL `test`.`get_new_raw_data`();";
const char* DatabaseConnection::GET_WORDS_QUERY = "CALL `test`.`get_words`()";

DatabaseConnection::DatabaseConnection(const char *_host, const char *_user,
	const char *_pass, const char *_db) :
	rawConn(0),
	wordConn(0),
	userConn(0),
	raw_result(0),
	word_result(0),
	user_result(0),
	host(_host),
	user(_user),
	pass(_pass),
	db(_db)
{
	rawConn = mysql_init(NULL);
	wordConn = mysql_init(NULL);
	userConn = mysql_init(NULL);
}

int DatabaseConnection::connect()
{
	if (mysql_real_connect(rawConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0) == NULL ||
		mysql_real_connect(userConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0) == NULL)
	{
		mysql_close(rawConn);
		mysql_close(wordConn);
		mysql_close(userConn);
		return  99;
	}
	return 0;
}

int DatabaseConnection::runRawQuery()
{
	int ans = mysql_query(rawConn, RAW_FACEBOOK_GET_NEW_ROWS_QUERY);
	if (ans != 0)
		return ans;
	else
	{
		initMySQLResult(raw_result);
		raw_result = mysql_store_result(rawConn);
		return ans;
	}
}

void DatabaseConnection::initMySQLResult(MYSQL_RES *res)
{
	if (res != 0)
	{
		mysql_free_result(res);
		res = 0;
	}
}


DatabaseConnection::~DatabaseConnection()
{
	/*mysql_close(rawConn);
	mysql_close(userConn);
	initMySQLResult(raw_result);
	initMySQLResult(word_result);
	initMySQLResult(user_result);*/
}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t res;
	res.userIdFrom = 0;
	res.userIdTo = 0;

	if (raw_result == 0)
		return res;
	MYSQL_ROW row = mysql_fetch_row(raw_result);

	if (row == 0)
	{
		return res;
	}

	char buffer[200];
	sprintf_s(buffer, "%s", row[0]);
	res.userIdTo = atoi(buffer);

	sprintf_s(buffer, "%s", row[1]);
	res.userIdFrom = atoi(buffer);

	sprintf_s(buffer, "%s", row[2]);
	res.eventId = atoi(buffer);

	sprintf_s(buffer, "%s", row[3]);
	res.eventType = (event_type) atoi(buffer);

	sprintf_s(buffer, "%s", row[4]);
	res.likeAmount = atoi(buffer);

	sprintf_s(buffer, "%s", row[5]);
	res.commentAmount = atoi(buffer);

	sprintf_s(buffer, "%s", row[6]);
	res.data = buffer;

	sprintf_s(buffer, "%s", row[7]);
	res.row_id = atoi(buffer);

	return res;
}

vector<corpusWord_t> DatabaseConnection::getWords()
{
	if (corpus.size() > 0)
		return corpus;

	initMySQLResult(word_result);
	mysql_real_connect(wordConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0);
	mysql_set_character_set(wordConn, "utf8mb4");
	mysql_query(wordConn, GET_WORDS_QUERY);
	word_result = mysql_store_result(wordConn);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(word_result)))
	{
		corpusWord_t corpus_word;
		char buffer[200];
		sprintf_s(buffer, "%s", row[0]);
		corpus_word.cat = (category) atoi(buffer);

		sprintf_s(buffer, "%s", row[1]);
		corpus_word.word = buffer;

		sprintf_s(buffer, "%s", row[2]);
		corpus_word.score = atoi(buffer);

		corpus.push_back(corpus_word);
	}

	mysql_close(wordConn);
	return corpus;
}

userData_t DatabaseConnection::getNextUserData()
{
	userData_t ans = {};
	return ans;
}

