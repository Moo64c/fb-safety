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
		processedEvent_t procceced_data;
		procceced_data.createdTime = 0;
		procceced_data.updatedTime = 0;
		procceced_data.userIdTo = data.userId;
		procceced_data.frequency = data.frequency;
		procceced_data.sticky = 1;
		procceced_data.title = "Weekly Status";
		if (check(data)) {
			procceced_data.severity = 5;
			procceced_data.alert = 1;
			procceced_data.body = "Unusual activity detected";
		}
		else {
			procceced_data.severity = 0;
			procceced_data.alert = 0;
			procceced_data.body = "Everything seems to be OK!";
		}
		connection.updateWebInterface(procceced_data);
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