#include "DatabaseConnection.h"


DatabaseConnection::DatabaseConnection()
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
