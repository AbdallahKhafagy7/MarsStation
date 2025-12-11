#include "rover.h"

rover::rover(int id, char t, int s, int checkDur, int mLimit)
{
	ID = id;
	type = t;
	speed = s;
	checkupDuration = checkDur;
	missionsBeforeCheckup = mLimit;
	missionsCompleted = 0;
	checkupStartDay = 0;
}

int rover::getID() const { return ID; }
char rover::getType() const { return type; }
int rover::getSpeed() const { return speed; }
int rover::getCheckupDuration() const { return checkupDuration; }
int rover::getMissionsBeforeCheckup() const { return missionsBeforeCheckup; }
int rover::getMissionsCompleted() const { return missionsCompleted; }

void rover::incrementMissions() { missionsCompleted++; }
void rover::resetMissions() { missionsCompleted = 0; }

void rover::setCheckupStartDay(int d) { checkupStartDay = d; }
int rover::getCheckupStartDay() const { return checkupStartDay; }

ostream& operator<<(ostream& out, const rover& obj)
{
	out << obj.ID;
	return out;
}