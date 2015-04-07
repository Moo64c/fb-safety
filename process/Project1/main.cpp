#include "tableStructs.h"
#include "DatabaseConnection.h"
#include <iostream>

using namespace std;

void testDatabaseConnection(){
	DatabaseConnection dc;
	dc.connect();
	dc.runRawQuery();
	rawEventEntry_t ev = dc.getNextRow();
	while (ev.userIdFrom != 0)
		dc.getNextRow();
}

int main(int argc, char **argv){
	string db;

	testDatabaseConnection();

	int i;
	cin >> i;
}