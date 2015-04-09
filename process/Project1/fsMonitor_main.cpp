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
	EventProcessor EP(&connection);
	EP.getNewEvent();
	printf("Calculating events' severy...\n");
	while (!(EP.newEvent.userIdTo.empty())) {
		int i_score = EP.calculateScore();
		EP.evaluateSeverity(i_score);
		EP.getNewEvent();
	}
	printf("Calculated events' severy.\n");
}

void offlineTable(DatabaseConnection connection)
{
	printf("analyzing data...\n");
	OfflineTable::OfflineTable(connection);
	printf("analyzed data. Done!");
}

int main (int argc, char **argv)
{
	const char* host = NULL;
	const char* user = NULL;
	const char* database = NULL;
	const char* password = NULL;

	for(int i=0; i < argc; i++) {
		if (strcmp("-h", *(argv+i))==0) {
			host = *(argv+i+1);
		}
		if (strcmp("-u", *(argv+i))==0) {
			user = *(argv+i+1);
		}
		if (strcmp("-d", *(argv+i))==0) {
			database = *(argv+i+1);
		}
		if (strcmp("-p", *(argv+i))==0) {
			password = *(argv+i+1);
		}

	}
	DatabaseConnection connection(host, user, password, database);
	printf("Connecting to database...\n");
	connection.connect();
	eventProcessor(connection);
	offlineTable(connection);
}