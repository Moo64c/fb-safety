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
		void getNewEvent(DatabaseConnection &connection);
		int calculateScore();
		void evaluateSeverity(int score);
      
    private:
        rawEventEntry_t newEvent;
        processedEvent_t event1;
		DatabaseConnection connection;

};
