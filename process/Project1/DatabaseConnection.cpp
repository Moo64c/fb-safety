#include "DatabaseConnection.h"

const char* DatabaseConnection::DEFAULT_HOST = "localhost";
const char* DatabaseConnection::DEFAULT_USER = "root";
const char* DatabaseConnection::DEFAULT_PASSWORD = "qwerty";
const char* DatabaseConnection::DEFAULT_DATABASE = "test";
const char* DatabaseConnection::RAW_FACEBOOK_GET_NEW_ROWS_QUERY = "SELECT 1;";

DatabaseConnection::DatabaseConnection(const char *_host, const char *_user,
	const char *_pass, const char *_db) :
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
	return mysql_query(con, RAW_FACEBOOK_GET_NEW_ROWS_QUERY);
}


DatabaseConnection::~DatabaseConnection()
{
	mysql_close(con);
}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t t;
	return t;
}
