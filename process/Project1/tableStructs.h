#ifndef PROCESSOR_DATA_TYPES
#define PROCESSOR_DATA_TYPES

#include <string>
#include <ctime>

using namespace std;

enum category { PROFANITY_CATEGORY = 1, SUICIDAL_CATEGORY = 2, SEXUAL_CATEGORY = 3 , OTHER_CATEGORY = 4};
enum event_type { OTHER_EVENT_TYPE = 0, STATUS_EVENT_TYPE = 1, COMMENT_EVENT_TYPE = 2, PHOTO_EVENT_TYPE = 3, 
	PRIVATE_MESSAGE_EVENT_TYPE = 4, LINK_EVENT_TYPE = 5};



struct corpusWord_t{
	string word;
	category cat;
	unsigned int score;
};

struct rawEventEntry_t {
	string userIdTo;
	string userIdFrom;
	string eventId;
	event_type eventType;
	int likeAmount;
	int commentAmount;
	string data;
	int row_id;
	string updatedTime;
	string createdTime;
};

struct processedEvent_t {
	string updatedTime;
	string createdTime;
	string userIdTo;
	int severity;
	int frequency;
	int sticky;//0 or 1
	int alert;//0 or 1
	int row_id;
	category cat;
	string title;
	string body;
};


struct userData_t {
	string userId;
	string maxUserIdFrom;
	int numOfEvents;
	double prevAverageSeverity;
	double averageSeverity;
	int frequency;
};
#endif //PROCESSOR_DATA_TYPES