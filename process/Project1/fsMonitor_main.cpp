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
	while (EP.newEvent.userIdTo != 0) {
		printf("Calculating event score...\n");
		int i_score = EP.calculateScore();
		cout << "score calculated:" << i_score << "\n";
		printf("Determining severity...\n");
		EP.evaluateSeverity(i_score);
		EP.getNewEvent();
	}
}

void offlineTable(DatabaseConnection connection)
{
	OfflineTable::OfflineTable(connection);
	printf("analyzing data...\n");
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
	int i;
	cin >> i;
}