#pragma once
#include <iostream>
#include "rover.h" 

using namespace std;

class mission
{
private:
	char type;
	int targetLocation;
	int duration;
	int ID;
	int readyDay;

	// Phase 2 Statistics
	int waitingDays;
	int executionDays;

	rover* assignedRover;

public:
	mission(char t, int loc, int dur, int id, int r);

	int getID() const;
	int getDuration() const;
	int getTargetLoc() const;
	char getType() const;
	int getRDay() const;

	void setRover(rover* r);
	rover* getRover() const;

	// Statistics Setters/Getters
	void setWaitingDays(int d);
	void setExecutionDays(int d);
	int getWaitingDays() const;
	int getExecutionDays() const;

	friend ostream& operator<<(ostream& out, const mission& obj);
};