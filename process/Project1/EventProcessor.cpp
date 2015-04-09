#include <string>
#include <ctime>
#include <iostream> 
#include <vector>
#include <regex>
#include "tableStructs.h"
#include "DatabaseConnection.h" 
#include "EventProcessor.h"

using namespace std;

	EventProcessor::EventProcessor(DatabaseConnection *_connection):
		connection(_connection)
	{

	}

	void EventProcessor::getNewEvent()
	{
		newEvent = connection->getNextRow();
	}
	int EventProcessor::calculateScore()						 
	{
		string data = newEvent.data;
		int score=0;
		vector<corpusWord_t> words = connection->getWords();
		int size = (int) words.size();

		for (int i=0; i<size; i++)
			{
				string word = words[i].word;
				if (std::regex_match(data, std::regex("(.*)([é]|[ä]|[^à-ú]|^)" + word + "([^à-ú]|$)(.*)")) && newEvent.userIdTo != newEvent.userIdFrom && std::regex_match(data, std::regex("(.*)([^a-z]|^)" + word + "([^a-z]|$)(.*)")))
				{
					 score += words[i].score;
					 event1.cat = words[i].cat;
					 event1.body = "Expration found: " + words[i].word;
				}
			}
		if (score>0)
		{
			if (5>newEvent.likeAmount+newEvent.commentAmount && newEvent.likeAmount+newEvent.commentAmount>1)
			{
				score+=1;
			}
			if (10>newEvent.likeAmount+newEvent.commentAmount && newEvent.likeAmount+newEvent.commentAmount>5)
			{
				score+=2;
			}
			if (newEvent.likeAmount+newEvent.commentAmount>10)
			{
				score+=3;
			}
		}
		return (score);
	}

	void EventProcessor::evaluateSeverity(int score)
	{
		if (3>score && score>0) {event1.severity = 1;}
		if (5>score && score>2) {event1.severity = 2;}
		if (8>score && score>4) {event1.severity = 3;}
		if (score>7)   {event1.severity = 4;}
		event1.row_id = newEvent.row_id;
		event1.userIdTo = newEvent.userIdTo;
		event1.row_id = newEvent.row_id;
		event1.sticky = 0;
		event1.createdTime = newEvent.createdTime;
		event1.frequency = 0;
		string type1 = Type(event1.cat);
		event1.title = "Event Type: " + type1;
		if (event1.severity < 3){
			event1.body = "";
		}
		connection->updateRawDB(event1);
	}

	string EventProcessor::Type(int typeNum)
	{
		string type;
		if (typeNum == 1) {
			type == "PROFANITY";
		}
		if (typeNum == 2) {
			type == "SUICIDAL";
		}
		if (typeNum == 3) {
			type == "SEXUAL";
		}
		if (typeNum == 4){
			type == "OTHER";
		}
		return(type);
	}