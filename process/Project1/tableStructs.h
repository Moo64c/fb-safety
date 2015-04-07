#ifndef PROCESSOR_DATA_TYPES
#define PROCESSOR_DATA_TYPES

#include <string>
#include <ctime>

using namespace std;

enum category { PROFANITY_CATEGORY = 1, SUICIDAL_CATEGORY = 2, SEXUAL_CATEGORY = 3 , OTHER_CATEGORY = 4};
enum event_type { STATUS_EVENT_TYPE = 1, COMMENT_EVENT_TYPE = 2, PHOTO_EVENT_TYPE = 3, 
	PRIVATE_MESSAGE_EVENT_TYPE = 4, LINK_EVENT_TYPE = 5};



struct corpusWord_t{
	string word;
	category cat;
	unsigned int score;
};

struct rawEventEntry_t {
	int userIdTo;
	int userIdFrom;
	int eventId;
	event_type eventType;
	int likeAmount;
	int commentAmount;
	string data;
	int row_id;
};

struct processedEvent_t {
	int userIdTo;
	int userIdFrom;
	int eventId;
	int severity;
	category cat;
	int row_id;
};


struct userData_t {
	int userId;
	int maxUserIdFrom;
	int numOfEvents;
	int prevAverageSeverity;
	int averageSeverity;
	int frequency;
};
rawEventEntry_t newEvent;
processedEvent_t event1;
#endif //PROCESSOR_DATA_TYPES