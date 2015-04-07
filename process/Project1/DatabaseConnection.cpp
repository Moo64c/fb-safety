#include "DatabaseConnection.h"


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


DatabaseConnection::~DatabaseConnection()
{

}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t t;
	return t;
}
