#include <string>
#include <ctime>
#include "tableStructs.h"
#include <vector>
#include <iostream> 
#include <vector>
#include <regex>
#include "tableStructs.h"
#include "DatabaseConnection.h" 
#include <string>
#include <ctime>

using namespace std;

class EventProcessor
{
	public:
		EventProcessor(DatabaseConnection *connection);
		void getNewEvent();
		int calculateScore();
		void evaluateSeverity(int score);
		rawEventEntry_t newEvent;

    private:
        processedEvent_t event1;
		DatabaseConnection *connection;

};
