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


DatabaseConnection::~DatabaseConnection()
{

}

rawEventEntry_t DatabaseConnection::getNextRow()
{
	rawEventEntry_t t;
	return t;
}
