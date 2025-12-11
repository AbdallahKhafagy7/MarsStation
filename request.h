#pragma once
#include <iostream>
class MarsStation;

class request
{
protected:
	MarsStation* station;
	char type;
	int day;
	int missionID;
public:
	request(MarsStation* s, char reqType, int id, int d)
		: station(s), type(reqType), missionID(id), day(d) { }
	virtual void Operate() = 0;
	virtual ~request() {};

	int getDay() const { return day; }
	char getType() const { return type; }
	int getMissionID() const { return missionID; }
};