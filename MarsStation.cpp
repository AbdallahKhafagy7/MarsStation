#include "MarsStation.h"
#include "newRequest.h"
#include "abortRequest.h"
#include "UI.h" 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <vector>
using namespace std;

void MarsStation::inputFile(const string filename) {
	ifstream file(filename);
	if (!file) {
		file.open(filename + ".txt");
		if (!file) { cout << "Error opening file!" << endl; return; }
	}

	string line;

	// number of rovers
	getline(file, line);
	istringstream iss1(line);
	iss1 >> numNM >> numPM >> numDM;

	// rover speeds
	getline(file, line);
	istringstream iss2(line);
	iss2 >> speedNM >> speedPM >> speedDM;

	// checkup info 
	getline(file, line);
	istringstream iss3(line);
	iss3 >> checkupTime >> checkDurNM >> checkDurPM >> checkDurDM;

	// --- CREATE ROVERS LOOP (Critical Logic) ---
	int idCounter = 1;
	for (int i = 0; i < numNM; ++i) Avail_NR.enqueue(new rover(idCounter++, 'N', speedNM, checkDurNM, checkupTime));
	for (int i = 0; i < numPM; ++i) Avail_PR.enqueue(new rover(idCounter++, 'P', speedPM, checkDurPM, checkupTime));
	for (int i = 0; i < numDM; ++i) Avail_DR.enqueue(new rover(idCounter++, 'D', speedDM, checkDurDM, checkupTime));

	// no.requests
	int req;
	file >> req;

	for (int i = 0; i < req; i++) {
		char type;
		file >> type;
		if (type == 'R') {
			int id, loc, dur, day;
			char mtype;
			// Correct Parsing Order
			file >> mtype >> day >> id >> loc >> dur;
			newRequest* p = new newRequest(this, type, id, day, mtype, loc, dur);
			Requests.enqueue(p);
		}
		else if (type == 'X') {
			int id, day;
			file >> day >> id;
			abortRequest* p = new abortRequest(this, type, id, day);
			Requests.enqueue(p);
		}
	}

	file.close();
}

void MarsStation::simulator() {
	UI ui;
	day = 1;
	srand(time(0));

	string file;
	cout << "Input File Name: ";
	cin >> file;

	inputFile(file);

	cout << "Simulation Starts..." << endl;

	while (day <= 50 || !Requests.isEmpty() || !EXEC_missions.isEmpty()) {

		handleRequests();

		// Calling YOUR function names
		moveCheckupToAvailable();
		moveingBackToDone();
		moveingExecToBack();
		moveingOutToExec();
		// Auto-abort polar ready missions before assigning
		autoAbortPolarReady();
		moveingReadyToOut();

		ui.printDay(day, &RDY_NM, &RDY_PM, &RDY_DM,
			&Avail_NR, &Avail_PR, &Avail_DR,
			&OUT_missions, &EXEC_missions, &BACK_missions, &DONE_missions);

		cout << "Press ENTER for next day...";
		cin.ignore(); cin.get();

		day++;
	}
}

void MarsStation::handleRequests() {
	while (!Requests.isEmpty()) {
		request* req;
		Requests.peek(req);
		if (req->getDay() == day) {
			Requests.dequeue(req);
			req->Operate();
			delete req;
		}
		else {
			break;
		}
	}
}

void MarsStation::insertMission(mission* m, char type) {
	if (type == 'N') RDY_NM.enqueue(m);
	else if (type == 'P') RDY_PM.enqueue(m);
	else if (type == 'D') RDY_DM.enqueue(m);
}

// =================================================
//  YOUR NAMING CONVENTION + PHASE 1.2 LOGIC
// =================================================

void MarsStation::checkUpTest()
{
	int Y = rand() % 100;
	// Rule: Only if Y < 70
	if (Y < 70)
	{
		rover* r;
		if (!checkup_NR.isEmpty() && checkup_NR.dequeue(r)) Avail_NR.enqueue(r);
		if (!checkup_PR.isEmpty() && checkup_PR.dequeue(r)) Avail_PR.enqueue(r);
		if (!checkup_DR.isEmpty() && checkup_DR.dequeue(r)) Avail_DR.enqueue(r);
	}
}

void MarsStation::moveingBackToDone()
{
	mission* m;
	int p;
	if (BACK_missions.dequeue(m, p))
	{
		DONE_missions.push(m);

		// Get Rover Link
		rover* r = m->getRover();
		if (r != nullptr)
		{
			int x = rand() % 100;
			char type = r->getType();

			// Rule: X < 20 -> Checkup
			if (x < 20) {
				// set checkup start day and send to checkup queue
				r->setCheckupStartDay(day);
				if (type == 'N') checkup_NR.enqueue(r);
				else if (type == 'P') checkup_PR.enqueue(r);
				else if (type == 'D') checkup_DR.enqueue(r);
			}
			else {
				// return to available and clear checkup start
				r->setCheckupStartDay(0);
				if (type == 'N') Avail_NR.enqueue(r);
				else if (type == 'P') Avail_PR.enqueue(r);
				else if (type == 'D') Avail_DR.enqueue(r);
			}
		}
	}
}


void MarsStation::moveingOutToExec()
{
	mission* m;
	int priority;
	while (OUT_missions.peek(m, priority)) 
	{
				int arrivalDay = -priority;
				if (arrivalDay <= day) 
				{
						OUT_missions.dequeue(m, priority);
			int executionEndDay = day + m->getDuration();
			EXEC_missions.enqueue(m, -executionEndDay);
		        }
		else {			break;
		}
	}
}



// Move rovers whose checkup ended to available lists
void MarsStation::moveCheckupToAvailable()
{
	rover* r;
	LinkedQueue<rover*> tempQ;

	// Process normal rovers
	while (!checkup_NR.isEmpty()) {
		checkup_NR.dequeue(r);
		int start = r->getCheckupStartDay();
		int end = 0;
		if (start > 0) end = start + r->getCheckupDuration();
		if (start > 0 && end <= day) {
			r->setCheckupStartDay(0);
			Avail_NR.enqueue(r);
		}
		else {
			tempQ.enqueue(r);
		}
	}
	while (!tempQ.isEmpty()) {
		tempQ.dequeue(r);
		checkup_NR.enqueue(r);
	}

	// Process polar rovers
	while (!checkup_PR.isEmpty()) {
		checkup_PR.dequeue(r);
		int start = r->getCheckupStartDay();
		int end = 0;
		if (start > 0) end = start + r->getCheckupDuration();
		if (start > 0 && end <= day) {
			r->setCheckupStartDay(0);
			Avail_PR.enqueue(r);
		}
		else {
			tempQ.enqueue(r);
		}
	}
	while (!tempQ.isEmpty()) {
		tempQ.dequeue(r);
		checkup_PR.enqueue(r);
	}

	// Process digging rovers
	while (!checkup_DR.isEmpty()) {
		checkup_DR.dequeue(r);
		int start = r->getCheckupStartDay();
		int end = 0;
		if (start > 0) end = start + r->getCheckupDuration();
		if (start > 0 && end <= day) {
			r->setCheckupStartDay(0);
			Avail_DR.enqueue(r);
		}
		else {
			tempQ.enqueue(r);
		}
	}
	while (!tempQ.isEmpty()) {
		tempQ.dequeue(r);
		checkup_DR.enqueue(r);
	}
}

// Abort a normal mission by ID (called from abortRequest)
void MarsStation::AbortMission(int missionID)
{
    // Try to abort from ready normal list
    mission* m = RDY_NM.AbortMission(missionID);
    if (m != nullptr) {
        // found in ready list -> move to aborted
        Aborted_missions.enqueue(m);
        return;
    }

    // Try to abort from outgoing missions
    m = OUT_missions.AbortMission(missionID);
    if (m != nullptr) {
        // detach rover and send it to checkup/available immediately
        rover* r = m->getRover();
        if (r != nullptr) {
            int x = rand() % 100;
            char type = r->getType();
            // Simplified: send to checkup with probability 20%, else available
            if (x < 20) {
                r->setCheckupStartDay(day);
                if (type == 'N') checkup_NR.enqueue(r);
                else if (type == 'P') checkup_PR.enqueue(r);
                else if (type == 'D') checkup_DR.enqueue(r);
            }
            else {
                r->setCheckupStartDay(0);
                if (type == 'N') Avail_NR.enqueue(r);
                else if (type == 'P') Avail_PR.enqueue(r);
                else if (type == 'D') Avail_DR.enqueue(r);
            }
        }

        // move mission to aborted list
        Aborted_missions.enqueue(m);
        return;
    }

    // Otherwise cannot abort; ignore
}

// Auto-abort ready polar missions whose waiting time > 2 * duration
void MarsStation::autoAbortPolarReady()
{
	// We need to iterate through RDY_PM queue and remove missions that meet condition.
	// Since LinkedQueue doesn't provide direct iteration or removal by predicate,
	// we'll dequeue all items and re-enqueue those that don't get aborted.

	LinkedQueue<mission*> tempQueue;
	mission* m;

	while (!RDY_PM.isEmpty()) {
		RDY_PM.dequeue(m);
		int waiting;
		if (m->getRDay() == 0) {
			waiting = 0;
		}
		else {
			waiting = day - m->getRDay();
		}
		// If waitingDays field exists and is maintained, prefer using it. Otherwise calculate.
		// Condition: waiting time > 2 * mission duration
		if (waiting > 2 * m->getDuration()) {
			// move to aborted list
			Aborted_missions.enqueue(m);
		}
		else {
			tempQueue.enqueue(m);
		}
	}

	// restore remaining missions
	while (!tempQueue.isEmpty()) {
		tempQueue.dequeue(m);
		RDY_PM.enqueue(m);
	}
}
void MarsStation::moveingReadyToOut()
{
	mission* m1;
	rover* r1;

	// the polar missions
	// Polar , Normal ,  Digging 
	while (RDY_PM.peek(m1)) //while exist ready polar mission
	{
		r1 = NULL; 

		if ( !Avail_PR.isEmpty()  )  {
			Avail_PR.dequeue( r1) ; //   polar elAwl
		}
		else if (!Avail_NR.isEmpty())
		{
			Avail_NR.dequeue(r1); // MAFESH POLAR AROH L normaL
	}
		else if (!Avail_DR.isEmpty()) 
		{
			Avail_DR.dequeue(r1); //  digging
		}

		// LW LAQA ROVER FADY 
		if (r1 != NULL)
		{
			RDY_PM.dequeue( m1); 
			m1->setRover(r1 ); 
			int distance = m1-> getTargetLoc() ;
			
			int speed = r1->getSpeed() ;
			int dailyDistance = speed * 25;
			int trvelDays = (distance + dailyDistance - 1) / dailyDistance;
			int arrivaDay = day + trvelDays ;
			OUT_missions.enqueue(m1, -arrivaDay);
		}
		else 
		{break;}
	}
	// digging
//digging only
	while (  RDY_DM.peek(m1) )
	{
		r1 = nullptr;
		if (!Avail_DR.isEmpty()) 
		{
			Avail_DR.dequeue(r1);
		}

		if (r1 != nullptr)
		{
			RDY_DM.dequeue(m1);
			m1->setRover(r1);
			int distance = m1->getTargetLoc();
			int speed = r1->getSpeed();
			int dailyDistance = speed * 25;

			int travelDays = (distance + dailyDistance - 1) / dailyDistance;

			int arrivalDay = day + travelDays;
			OUT_missions.enqueue(m1, -arrivalDay);
		}
		else {
			break;
		}
	}
	//normal
	// normal , polar 
	// ---------------------------------------------------------
	while (RDY_NM.peek(m1))
	{
		r1 = nullptr;
		if (!Avail_NR.isEmpty()) 
		{
			Avail_NR.dequeue(r1); 
		}
		else if (!Avail_PR.isEmpty()) 
		{
			Avail_PR.dequeue(r1); 
		}

		if (r1 != nullptr)
		{
			RDY_NM.dequeue(m1);
			m1->setRover(r1);
			int distance = m1->getTargetLoc();
			int speed = r1->getSpeed();
			int dailyDistance = speed * 25;
			int travelDays = (distance + dailyDistance - 1) / dailyDistance;
			int arrivalDay = day + travelDays;
			OUT_missions.enqueue(m1, -arrivalDay);
		}
		else 
		{ 	break; }
	}
}



void MarsStation::moveingExecToBack()
{
	mission* m;
	int priority;

	while (EXEC_missions.peek(m, priority)) {
		int finishDay = -priority;
		if (finishDay <= day) {
			EXEC_missions.dequeue(m, priority);
			rover* r = m->getRover();
			int dist = m->getTargetLoc();     
			int speed = r->getSpeed();        
			int dailyDist = speed * 25;       
			int travelDays = (dist + dailyDist - 1) / dailyDist;
			int backArrivalDay = day + travelDays;
			BACK_missions.enqueue(m, -backArrivalDay);
		}
		else {
			break;		}
	}
}