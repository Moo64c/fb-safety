#include <string>
#include <ctime>

using namespace std;

enum category { PROFANITY_CATEGORY = 0, SUICIDAL_CATEGORY = 1, SEXUAL_CATEGORY = 2 , OTHER_CATEGORY = 3};
enum event_type { STATUS_EVENT_TYPE = 0, COMMENT_EVENT_TYPE = 1, PHOTO_EVENT_TYPE = 2, PRIVATE_MESSAGE_EVENT_TYPE = 3, 
	PRIVATE_MESSAGE_EVENT_TYPE = 4,
	};

struct corpusWord_t{
	string word;
	category cat;
	unsigned int score;
};

struct rawEntry_t {
	int userId;
	int eventId;
	event_type eventType;
	int likeAmount;
	int commentAmount;
	string data;
	time_t createdTime;
	time_t updatedTime;
};

struct processedEvent_t {
	int userId;
	int eventId;
	int severity;
	int popularity;
	category cat;
};