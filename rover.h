#pragma once
#include <iostream>
using namespace std;

class rover
{
private:
	int ID;
	char type;
	int speed;
	int checkupDuration;
	int missionsBeforeCheckup;
	int missionsCompleted;
	int checkupStartDay; // day when checkup started (0 = not in checkup)

public:
	// Constructor must match the 5 values read in MarsStation::inputFile
	rover(int id, char t, int s, int checkDur, int mLimit);

	int getID() const;
	char getType() const;
	int getSpeed() const;
	int getCheckupDuration() const;
	int getMissionsBeforeCheckup() const;
	int getMissionsCompleted() const;

	void incrementMissions();
	void resetMissions();

	// checkup start day accessors
	void setCheckupStartDay(int d);
	int getCheckupStartDay() const;

	friend ostream& operator<<(ostream& out, const rover& obj);
};