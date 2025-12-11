#pragma once
#include "./ArrayStack.h"
#include "./LinkedQueue.h"
#include "./PriQueue.h"
#include "./OUT_missions.h"
#include "./RDY_NM.h"
#include "./mission.h"
#include "./request.h"
#include "./rover.h"
#include "./UI.h" // Needed for printing

#include <fstream>
#include <sstream>
using namespace std;

class MarsStation {
private:
	int day = 0;

	// rovers numbers
	int numNM, numPM, numDM;

	// rovers speeds
	int speedNM, speedPM, speedDM;

	// number of missions before checkup
	int checkupTime;

	// checkup durations
	int checkDurNM, checkDurPM, checkDurDM;

	// request
	LinkedQueue<request*> Requests;

	// ready missions
	RDY_NM RDY_NM;
	LinkedQueue<mission*> RDY_PM, RDY_DM;

	// missions in different states
	OUT_missions OUT_missions;
	PriQueue<mission*> EXEC_missions;
	PriQueue<mission*> BACK_missions;
	LinkedQueue<mission*> Aborted_missions;
	ArrayStack<mission*> DONE_missions;

	// availible rovers
	LinkedQueue<rover*> Avail_NR, Avail_PR, Avail_DR;

	// checkup rovers
	LinkedQueue<rover*> checkup_NR, checkup_PR, checkup_DR;

public:
	//open input file and load rovers and requests
	void inputFile(const string filename);

	//simulator function
	void simulator();

	//insert mission (Required for Linker)
	void insertMission(mission* m, char type);

	//for each day
	//handle this day requests
	void handleRequests();

	//checkup -> available rovers (Your Name: checkUpTest)
	void checkUpTest();

	//back missions -> done (Your Name: moveingBackToDone)
	void moveingBackToDone();

	//assign ready missions to rovers (Your Name: moveingReadyToOut)
	void moveingReadyToOut();

	//missions from out -> exec (Your Name: moveingOutToExec)
	void moveingOutToExec();

	//missions from exec -> back (Your Name: moveingExecToBack)
	void moveingExecToBack();

	// Auto-abort ready polar missions if waiting time exceeds twice duration
	void autoAbortPolarReady();

	// Move rovers whose checkup ended to available lists
	void moveCheckupToAvailable();
};