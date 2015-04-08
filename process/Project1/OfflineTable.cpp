//offline table

#include "tableStructs.h"
#include "OfflineTable.h"

using namespace std;

int min_diff = 2;
double sev_threshold = 1.2;
double freq_threshold = 5;

OfflineTable::OfflineTable(DatabaseConnection &connection) {
	userData_t data = connection.getNextUserData();
	while (data.userId != 0) {         //userID == 0 : row is NULL
		if (check(data))
			updateDB (data);
		data = connection.getNextUserData();
	}
}
bool OfflineTable::check(userData_t data) {
	if (change(data.averageSeverity, data.prevAverageSeverity) || (data.frequency > freq_threshold))
		return true;
	return false;
}

bool OfflineTable::change(int curr, int prev) {
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