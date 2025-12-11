#pragma once
#include <iostream>
#include "./request.h"

class abortRequest : public request {
private:

public:
	abortRequest(MarsStation* s, char reqType, int id, int day)
		: request(s, reqType, id, day) {
	}

	void Operate() override {
		if (station) {
			station->AbortMission(missionID);
		}
	}
};