#include "DatabaseConnection.h"
#include <regex>
#include "time.h"

const char* DatabaseConnection::DEFAULT_HOST = "localhost";
const char* DatabaseConnection::DEFAULT_USER = "root";
const char* DatabaseConnection::DEFAULT_PASSWORD = "qwerty";
const char* DatabaseConnection::DEFAULT_DATABASE = "test";
const char* DatabaseConnection::RAW_FACEBOOK_GET_NEW_ROWS_QUERY = "CALL `test`.`get_new_raw_data`();";

DatabaseConnection::DatabaseConnection(const char *_host, const char *_user,
	const char *_pass, const char *_db) :
	con(0),
	result(0),
	host(_host),
	user(_user),
	pass(_pass),
	db(_db)
{
	con = mysql_init(NULL);
}

int DatabaseConnection::connect()
{
	if (mysql_real_connect(con, host.c_str(), user.c_str(), pass.c_str(),
		db.c_str(), 0, NULL, 0) == NULL)
	{
		mysql_close(con);
		return  mysql_errno(con);
	}
	return 0;
}

int DatabaseConnection::runRawQuery()
{
	int ans = mysql_query(con, RAW_FACEBOOK_GET_NEW_ROWS_QUERY);
	if (ans != 0)
		return ans;
	else
	{
		initResult();
		result = mysql_store_result(con);
	}
}

void DatabaseConnection::initResult()
{
	if (result != 0)
	{
		mysql_free_result(result);
		result = 0;
	}
}


DatabaseConnection::~DatabaseConnection()
{
	mysql_close(con);
}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t res;
	res.userIdFrom = 0;
	res.userIdTo = 0;

	if (result == 0)
		return res;
	MYSQL_ROW row = mysql_fetch_row(result);

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

