#pragma once
#include <iostream>
#include "./request.h"
#include "./mission.h"

class MarsStation; // forward declaration

class newRequest : public request {
private:
	char missionType;
	int targetLocation;
	int duration;

public:
	newRequest(MarsStation* s, char reqType, int id, int day, char t, int loc, int dur)
		: request(s, reqType,id, day),
		missionType(t),
		targetLocation(loc),
		duration(dur) {}

	void Operate() override; // implemented in newRequest.cpp
};