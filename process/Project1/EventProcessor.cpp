#include <string>
#include <ctime>
#include <iostream> 
#include <vector>
#include <regex>
#include "tableStructs.h"
#include "DatabaseConnection.h" 
#include "EventProcessor.h"

using namespace std;

	rawEventEntry_t EventProcessor::getNewEvent(DatabaseConnection connection)					
	{
		newEvent = connection.getNextRow();
	}
	int EventProcessor::calculateScore()						 
	{
		string data = newEvent.data;
		int score=0;
		vector<corpusWord_t> words = getWords();
		int size = words.size();

		for (int i=0; i<size+1; i++)
			{
				string word = words[i].word;
				std::size_t found = data.find(word);
				if (std::regex_match (data, std::regex("(.*)([^�-�]|^)"+word+"([^�-�]|$)(.*)") ) && newEvent.userIdTo!=newEvent.userIdFrom)			
				{
					 score += words[i].score;
					 event1.cat = words[i].cat;
				}
			}
		if (score>0)
		{
			if (5>newEvent.likeAmount+newEvent.commentAmount>1)
			{
				score+=1;
			}
			if (10>newEvent.likeAmount+newEvent.commentAmount>5)
			{
				score+=2;
			}
			if (newEvent.likeAmount+newEvent.commentAmount>10)
			{
				score+=4;
			}
		}
		return (score);
	}

	int EventProcessor::evaluateSeverity(int score)
	{
		if (3>score>0) {event1.severity = 1;}
		if (5>score>2) {event1.severity = 2;}
		if (8>score>4) {event1.severity = 3;}
		if (score>7)   {event1.severity = 4;}
		event1.eventId = newEvent.eventId;
		event1.userIdTo = newEvent.userIdTo;
		event1.userIdFrom = newEvent.userIdFrom;
	}
