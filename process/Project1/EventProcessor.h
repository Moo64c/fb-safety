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
		rawEventEntry_t getNewEvent();
		int calculateScore();
		int evaluateSeverity(int score);
};