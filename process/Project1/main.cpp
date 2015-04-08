#include "tableStructs.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <vector>

using namespace std;

void testDatabaseConnection(){
	DatabaseConnection dc;
	dc.connect();
	vector<corpusWord_t> vec = dc.getWords();
	for (int i = 0; i < vec.size(); i++)
	{
		printf("(%s, %d, %d)\n", vec[i].word.c_str(), vec[i].cat, vec[i].score);
	}
}

int main(int argc, char **argv){
	string db;

	testDatabaseConnection();

	int i;
	cin >> i;
}