#include "mission.h"

mission::mission(char t, int loc, int dur, int id, int r)
	: type(t), targetLocation(loc), duration(dur), ID(id), readyDay(r)
{
	waitingDays = 0;
	executionDays = 0;
	assignedRover = nullptr;
}

int  mission::getID() const { return ID; }

int mission::getDuration() const { return duration; }
 int mission::getTargetLoc() const { return targetLocation; }
  char   mission::getType()  const { return type; }
  int mission::getRDay() const { return readyDay; }
   
void mission::setRover(rover* r ) { assignedRover = r; }
 rover* mission::getRover() const {   return assignedRover; }
 
ostream& operator<<(ostream& out, const mission& obj)
{
	out << obj.ID;
	return out;
}