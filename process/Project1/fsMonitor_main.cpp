#include <string>
#include <ctime>
#include <iostream> 
#include <vector>
#include <regex>
#include "tableStructs.h"
#include "DatabaseConnection.h"
#include "EventProcessor.h"
#include "OfflineTable.h"
using namespace std;


void eventProcessor(DatabaseConnection connection)
{
	EventProcessor EP;
	EP.getNewEvent(connection);
	int i_score = EP.calculateScore();
	EP.evaluateSeverity(i_score);
}

void offlineTable(DatabaseConnection connection)
{
	OfflineTable::OfflineTable(connection);
}

int main (int argc, char **argv)
{
	const char* host = NULL;
	const char* user = NULL;
	const char* password = NULL;
	const char* database = NULL;

	for(int i=0; i< argc+1; i+2) {
		if (strcmp("-h", *(argv+i))==0) {
			host = *(argv+i+1);
		}
		if (strcmp("-u", *(argv+i))==0) {
			user = *(argv+i+1);
		}
		if (strcmp("-p", *(argv+i))==0) {
			password = *(argv+i+1);
		}
		if (strcmp("-d", *(argv+i))==0) {
			database = *(argv+i+1);
		}
	}
	DatabaseConnection::DatabaseConnection(host, user, password, database);
	DatabaseConnection connection;
	eventProcessor(connection);
	offlineTable(connection);
}