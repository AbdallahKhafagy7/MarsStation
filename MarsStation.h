#pragma once
#include "./ArrayStack.h"
#include "./LinkedQueue.h"
#include "./PriQueue.h"
#include "./OUT_missions.h"
#include "./RDY_NM.h"
#include "./mission.h"
#include "./request.h"
#include "./rover.h"
#include "./UI.h" 

#include <fstream>
#include <sstream>
using namespace std;

class MarsStation {
private:
	int day = 0;

	int numNM, numPM, numDM;
	int speedNM, speedPM, speedDM;
	int checkupTime;
	int checkDurNM, checkDurPM, checkDurDM;

	// Statistics Counters
	int totalMissions_N = 0;
	int totalMissions_P = 0;
	int totalMissions_D = 0;
	int autoAbortedCount = 0;

	// System Lists
	LinkedQueue<request*> Requests;

	RDY_NM RDY_NM;
	LinkedQueue<mission*> RDY_PM, RDY_DM;

	OUT_missions OUT_missions;
	PriQueue<mission*> EXEC_missions;
	PriQueue<mission*> BACK_missions;
	LinkedQueue<mission*> Aborted_missions;
	ArrayStack<mission*> DONE_missions;

	LinkedQueue<rover*> Avail_NR, Avail_PR, Avail_DR;
	LinkedQueue<rover*> checkup_NR, checkup_PR, checkup_DR;

public:
	void inputFile(const string filename);
	void simulator();
	void saveOutputFile();

	void insertMission(mission* m, char type);
	void handleRequests();

	void checkUpTest();
	void moveingBackToDone();
	void moveingReadyToOut();
	void moveingOutToExec();
	void moveingExecToBack();

	void autoAbortPolarReady();
	void moveCheckupToAvailable();

	void AbortMission(int missionID);
	void roverToAvailCheckup(rover* r);
};