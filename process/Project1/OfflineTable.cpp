#include "tableStructs.h"
#include "OfflineTable.h"
#include "DatabaseConnection.h"

using namespace std;

int min_diff = 2;
double sev_threshold = 1.2;
double freq_threshold = 5;

OfflineTable::OfflineTable(DatabaseConnection &connection) {
	userData_t data = connection.getNextUserData();

	while (data.userId != 0) {         //userID == 0 : row is NULL
		processedEvent_t processed;
		processed.createdTime = 0;
		processed.updatedTime = 0;
		processed.userIdTo = data.userId;
		processed.frequency = data.frequency;
		processed.sticky = 1;
		processed.title = "Weekly Status";
		if (check(data)) {
			processed.severity = 4;
			processed.alert = 1;
			processed.body = "Unusual activity detected";
		}
		else {
			processed.severity = 5;
			processed.alert = 0;
			processed.body = "Everything seems to be OK!";
		}
		connection.updateWebInterface(processed);
		data = connection.getNextUserData();
	}
}
bool OfflineTable::check(userData_t data) {
	if (change(data.averageSeverity, data.prevAverageSeverity) || (data.frequency > freq_threshold))
		return true;
	return false;
}

bool OfflineTable::change(double curr, double prev) {
	if ((prev == 0) || (curr - prev < min_diff) || ((float)curr / (float)prev < sev_threshold))
		return false;
	return true;
}

OfflineTable::~OfflineTable()
{

}

 
void OfflineTable::updateDB(struct userData_t)
{

}