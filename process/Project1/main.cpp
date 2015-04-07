#include "tableStructs.h"
#include "DatabaseConnection.h"
#include <iostream>

using namespace std;

void testDatabaseConnection(){
	DatabaseConnection dc;
	printf("%d\n", dc.connect());
	printf("%d", dc.runRawQuery());
}

int main(){
	testDatabaseConnection();

	int i;
	cin >> i;
}