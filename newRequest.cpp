#include "./newRequest.h"
#include "./MarsStation.h"

void newRequest::Operate() {
	// handle new request operation
	mission* m = new mission(
		missionType,
		targetLocation,
		duration,
		missionID,
		day
	);

	station->insertMission(m, missionType);
}