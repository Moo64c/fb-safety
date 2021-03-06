#include "DatabaseConnection.h"
#include <regex>
#include "time.h"
#include <iostream>
#include <algorithm>

const char* DatabaseConnection::DEFAULT_HOST = "localhost";
const char* DatabaseConnection::DEFAULT_USER = "root";
const char* DatabaseConnection::DEFAULT_PASSWORD = "qwerty";
const char* DatabaseConnection::DEFAULT_DATABASE = "fb_safety";
const char* DatabaseConnection::RAW_FACEBOOK_GET_NEW_ROWS_QUERY = "CALL `get_new_raw_data`();";
const char* DatabaseConnection::GET_WORDS_QUERY = "CALL `get_words`()";
const char* DatabaseConnection::GET_USER_DATA_QUERY = "call `get_user_processed_data_week`();";
const char* DatabaseConnection::UPDATE_RAW_TABLE_QUERY = "call update_raw_row(%d, %d, %d);";
const char* DatabaseConnection::UPDATE_WEB_INTERFACE_QUERY = "call update_web_interface(%d, %d, %s, %d, %d, %d, %d, %s, %s);";

DatabaseConnection::DatabaseConnection(const char *_host, const char *_user,
	const char *_pass, const char *_db, bool _verbosity) :
	rawConn(0),
	wordConn(0),
	userConn(0),
	raw_result(0),
	word_result(0),
	user_result(0),
	host(_host),
	user(_user),
	pass(_pass),
	db(_db),
	verbose(_verbosity)
{
	rawConn = mysql_init(NULL);
	wordConn = mysql_init(NULL);
	userConn = mysql_init(NULL);
	updateConn = mysql_init(NULL);
}

int DatabaseConnection::connect()
{
	if (mysql_real_connect(rawConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0) == NULL ||
		mysql_real_connect(updateConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0) == NULL)
	{
		mysql_close(rawConn);
		mysql_close(updateConn);
		return  99;
	}
	return 0;
}

int DatabaseConnection::runRawQuery()
{
	mysql_set_character_set(rawConn, "utf8mb4");
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

}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t res;
	res.userIdFrom;
	res.userIdTo;

	if (raw_result == 0)
		runRawQuery();
	MYSQL_ROW row = mysql_fetch_row(raw_result);


	if (row == 0)
	{
		return res;
	}

	char buffer[1600];
	sprintf_s(buffer, "%s", row[0]);
	res.userIdTo = buffer;

	sprintf_s(buffer, "%s", row[1]);
	res.userIdFrom = buffer;

	sprintf_s(buffer, "%s", row[2]);
	res.eventId = atoi(buffer);

	sprintf_s(buffer, "%s", row[3]);
	res.eventType = getEventType(buffer);

	sprintf_s(buffer, "%s", row[4]);
	res.likeAmount = atoi(buffer);

	sprintf_s(buffer, "%s", row[5]);
	res.commentAmount = atoi(buffer);

	sprintf_s(buffer, "%s", row[6]);
	res.data = buffer;

	sprintf_s(buffer, "%s", row[7]);
	res.row_id = atoi(buffer);

	sprintf_s(buffer, "%s", row[8]);
	res.createdTime = buffer;

	sprintf_s(buffer, "%s", row[9]);
	res.updatedTime = buffer;

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
	userData_t res;
	res.userId;
	if (user_result == 0)
	{
		runUsersQuery();
	}
	MYSQL_ROW row = mysql_fetch_row(user_result);

	if (row == 0)
	{
		return res;
	}

	char buffer[1600];
	sprintf_s(buffer, "%s", row[0]);
	res.userId = buffer;

	sprintf_s(buffer, "%s", row[1]);
	res.maxUserIdFrom = buffer;

	sprintf_s(buffer, "%s", row[6]);
	res.numOfEvents = atoi(buffer);

	sprintf_s(buffer, "%s", row[5]);
	int prevNumOfEvents = atoi(buffer) - res.numOfEvents;
	
	sprintf_s(buffer, "%s", row[4]);
	double sumSeverity = atof(buffer);

	sprintf_s(buffer, "%s", row[3]);
	double prevSumSeverity = atof(buffer) - sumSeverity;

	res.averageSeverity = (res.numOfEvents == 0) ? 0 : sumSeverity / res.numOfEvents;
	res.prevAverageSeverity = (prevNumOfEvents == 0) ? 0 : prevSumSeverity / prevNumOfEvents;

	return res;
}

void DatabaseConnection::runUsersQuery()
{
	mysql_real_connect(userConn, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0);
	mysql_query(userConn, GET_USER_DATA_QUERY);
	initMySQLResult(user_result);
	user_result = mysql_store_result(userConn);
}

void DatabaseConnection::updateRawDB(processedEvent_t processed)
{
	char buffer[200];
	sprintf_s(buffer, UPDATE_RAW_TABLE_QUERY, processed.row_id, processed.severity, processed.cat);
	mysql_query(updateConn, buffer);
	updateWebInterface(processed);
}

void DatabaseConnection::updateWebInterface(processedEvent_t processed)
{
	char buffer[2000];
	sprintf_s(buffer, UPDATE_WEB_INTERFACE_QUERY, processed.createdTime, processed.updatedTime, processed.userIdTo.c_str(),
		processed.severity, processed.frequency, processed.sticky, processed.alert,
		processed.title.c_str(), processed.body.c_str());
	mysql_query(updateConn, buffer);
}

event_type DatabaseConnection::getEventType(const char *eventText)
{
	if (strcmp(eventText, "STATUS_EVENT_TYPE") == 0)
		return STATUS_EVENT_TYPE;
	if (strcmp(eventText, "COMMENT_EVENT_TYPE") == 0)
		return COMMENT_EVENT_TYPE;
	if (strcmp(eventText, "PHOTO_EVENT_TYPE") == 0)
		return PHOTO_EVENT_TYPE;
	if (strcmp(eventText, "PRIVATE_MESSAGE_EVENT_TYPE") == 0)
		return PRIVATE_MESSAGE_EVENT_TYPE;
	if (strcmp(eventText, "LINK_EVENT_TYPE") == 0)
		return LINK_EVENT_TYPE;

	return OTHER_EVENT_TYPE;
}
