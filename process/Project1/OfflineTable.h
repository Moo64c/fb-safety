#pragma once
#include "tableStructs.h"
#include "DatabaseConnection.h"
#include "my_global.h"

class OfflineTable {
public:
	OfflineTable(DatabaseConnection &connection);
	virtual ~OfflineTable();

protected:
	bool check(userData_t);
	void updateDB(userData_t);
	bool change(int curr, int prev);
};