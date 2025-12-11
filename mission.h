#pragma once
#include <iostream>
#include "rover.h" // Required so Mission knows what a Rover is
using namespace std;

class mission
{
private:
	char type;
	int targetLocation;
	int duration;
	int ID;
	int readyDay;
	int waitingDays;
	int executionDays;

	// The Link: Tracks which rover is doing this mission
	rover* assignedRover;

public:
	mission(char t, int loc, int dur, int id, int r);

	int getID() const;
	int getDuration() const;
	int getTargetLoc() const;
	char getType() const;
	int getRDay() const;

	// Critical logic functions
	void setRover(rover* r);
	rover* getRover() const;

	friend ostream& operator<<(ostream& out, const mission& obj);
};