#include "MarsStation.h"
#include "newRequest.h"
#include "abortRequest.h"
#include "UI.h" 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> 
#include <ctime>   
#include <iomanip> 
using namespace std;

void MarsStation::inputFile(const string filename) {
	ifstream file(filename);
	if (!file) {
		file.open(filename + ".txt");
		if (!file) { cout << "Error opening file!" << endl; return; }
	}

	string line;
	getline(file, line);
	istringstream iss1(line);
	iss1 >> numNM >> numPM >> numDM;

	getline(file, line);
	istringstream iss2(line);
	iss2 >> speedNM >> speedPM >> speedDM;

	getline(file, line);
	istringstream iss3(line);
	iss3 >> checkupTime >> checkDurNM >> checkDurPM >> checkDurDM;

	int idCounter = 1;
	for (int i = 0; i < numNM; ++i) Avail_NR.enqueue(new rover(idCounter++, 'N', speedNM, checkDurNM, checkupTime));
	for (int i = 0; i < numPM; ++i) Avail_PR.enqueue(new rover(idCounter++, 'P', speedPM, checkDurPM, checkupTime));
	for (int i = 0; i < numDM; ++i) Avail_DR.enqueue(new rover(idCounter++, 'D', speedDM, checkDurDM, checkupTime));

	int req;
	file >> req;

	for (int i = 0; i < req; i++) {
		char type;
		file >> type;
		if (type == 'R') {
			int id, loc, dur, day;
			char mtype;
			file >> mtype >> day >> id >> loc >> dur;

			newRequest* p = new newRequest(this, type, id, day, mtype, loc, dur);
			Requests.enqueue(p);

			if (mtype == 'N') totalMissions_N++;
			else if (mtype == 'P') totalMissions_P++;
			else if (mtype == 'D') totalMissions_D++;
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
	// 1. Initialize UI with pointer to station
	UI ui(this);

	day = 1;
	srand(time(0));

	// 2. UI handles mode selection internally in constructor

	string file;
	cout << "Input File Name: ";
	cin >> file;

	inputFile(file);

	// Optional: Check UI Mode via getter if you need to print start message differently
	// if(ui.getMode() == SILENT) ...

	cout << "Simulation Starts..." << endl;

	while (day <= 5000) {
		// Stop condition
		if (Requests.isEmpty() && EXEC_missions.isEmpty() &&
			RDY_NM.isEmpty() && RDY_PM.isEmpty() && RDY_DM.isEmpty() &&
			OUT_missions.isEmpty() && BACK_missions.isEmpty())
		{
			break;
		}

		handleRequests();
		moveCheckupToAvailable();
		moveingBackToDone();
		moveingExecToBack();
		moveingOutToExec();
		autoAbortPolarReady();
		moveingReadyToOut();

		// 3. Call UI Print with EXACTLY the 10 arguments it supports
		ui.printDay(day,
			&RDY_NM, &RDY_PM, &RDY_DM,
			&Avail_NR, &Avail_PR, &Avail_DR,
			&OUT_missions, &EXEC_missions, &BACK_missions,
			&DONE_missions);

		// Note: The UI class handles the "Press Enter" logic internally
		day++;
	}

	saveOutputFile();
	cout << "Output file generated." << endl;
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
		else break;
	}
}

void MarsStation::insertMission(mission* m, char type) {
	if (type == 'N') RDY_NM.enqueue(m);
	else if (type == 'P') RDY_PM.enqueue(m);
	else if (type == 'D') RDY_DM.enqueue(m);
}

void MarsStation::checkUpTest() {}

void MarsStation::moveCheckupToAvailable() {
	rover* r;

	auto checkList = [&](LinkedQueue<rover*>& list, LinkedQueue<rover*>& avail) {
		int count = list.getCount();
		while (count--) {
			list.dequeue(r);
			int end = r->getCheckupStartDay() + r->getCheckupDuration();
			if (end <= day) {
				r->setCheckupStartDay(0);
				avail.enqueue(r);
			}
			else {
				list.enqueue(r);
			}
		}
		};

	checkList(checkup_NR, Avail_NR);
	checkList(checkup_PR, Avail_PR);
	checkList(checkup_DR, Avail_DR);
}

void MarsStation::AbortMission(int missionID) {
	mission* m = RDY_NM.AbortMission(missionID);
	if (m) {
		Aborted_missions.enqueue(m);
		return;
	}
	m = OUT_missions.AbortMission(missionID);
	if (m) {
		rover* r = m->getRover();
		if (r) {
			int prob = rand() % 100;
			if (prob < 20) {
				r->setCheckupStartDay(day);
				if (r->getType() == 'N') checkup_NR.enqueue(r);
				else if (r->getType() == 'P') checkup_PR.enqueue(r);
				else checkup_DR.enqueue(r);
			}
			else {
				r->setCheckupStartDay(0);
				if (r->getType() == 'N') Avail_NR.enqueue(r);
				else if (r->getType() == 'P') Avail_PR.enqueue(r);
				else Avail_DR.enqueue(r);
			}
		}
		Aborted_missions.enqueue(m);
	}
}

void MarsStation::autoAbortPolarReady() {
	LinkedQueue<mission*> temp;
	mission* m;
	while (!RDY_PM.isEmpty()) {
		RDY_PM.dequeue(m);
		int wait = day - m->getRDay();
		if (wait > 2 * m->getDuration()) {
			Aborted_missions.enqueue(m);
			autoAbortedCount++;
		}
		else {
			temp.enqueue(m);
		}
	}
	while (!temp.isEmpty()) {
		temp.dequeue(m);
		RDY_PM.enqueue(m);
	}
}

void MarsStation::moveingReadyToOut() {
	mission* m;
	rover* r;

	while (RDY_PM.peek(m)) {
		r = NULL;
		if (!Avail_PR.isEmpty()) Avail_PR.dequeue(r);
		else if (!Avail_NR.isEmpty()) Avail_NR.dequeue(r);
		else if (!Avail_DR.isEmpty()) Avail_DR.dequeue(r);

		if (r) {
			RDY_PM.dequeue(m);
			m->setRover(r);

			int days = (m->getTargetLoc() + (r->getSpeed() * 25) - 1) / (r->getSpeed() * 25);
			m->setExecutionDays(days * 2 + m->getDuration());
			m->setWaitingDays(day - m->getRDay());

			OUT_missions.enqueue(m, -(day + days));
		}
		else break;
	}

	while (RDY_DM.peek(m)) {
		r = NULL;
		if (!Avail_DR.isEmpty()) Avail_DR.dequeue(r);
		if (r) {
			RDY_DM.dequeue(m);
			m->setRover(r);

			int days = (m->getTargetLoc() + (r->getSpeed() * 25) - 1) / (r->getSpeed() * 25);
			m->setExecutionDays(days * 2 + m->getDuration());
			m->setWaitingDays(day - m->getRDay());

			OUT_missions.enqueue(m, -(day + days));
		}
		else break;
	}

	while (RDY_NM.peek(m)) {
		r = NULL;
		if (!Avail_NR.isEmpty()) Avail_NR.dequeue(r);
		else if (!Avail_PR.isEmpty()) Avail_PR.dequeue(r);
		if (r) {
			RDY_NM.dequeue(m);
			m->setRover(r);

			int days = (m->getTargetLoc() + (r->getSpeed() * 25) - 1) / (r->getSpeed() * 25);
			m->setExecutionDays(days * 2 + m->getDuration());
			m->setWaitingDays(day - m->getRDay());

			OUT_missions.enqueue(m, -(day + days));
		}
		else break;
	}
}

void MarsStation::moveingOutToExec() {
	mission* m;
	int p;
	while (OUT_missions.peek(m, p)) {
		if (-p <= day) {
			OUT_missions.dequeue(m, p);
			EXEC_missions.enqueue(m, -(day + m->getDuration()));
		}
		else break;
	}
}

void MarsStation::moveingExecToBack() {
	mission* m;
	int p;
	while (EXEC_missions.peek(m, p)) {
		if (-p <= day) {
			EXEC_missions.dequeue(m, p);
			rover* r = m->getRover();
			int days = (m->getTargetLoc() + (r->getSpeed() * 25) - 1) / (r->getSpeed() * 25);
			BACK_missions.enqueue(m, -(day + days));
		}
		else break;
	}
}

void MarsStation::moveingBackToDone() {
	mission* m;
	int p;
	while (BACK_missions.peek(m, p)) {
		if (-p <= day) {
			BACK_missions.dequeue(m, p);
			DONE_missions.push(m);
			roverToAvailCheckup(m->getRover());
			m->setRover(nullptr);
		}
		else break;
	}
}

void MarsStation::roverToAvailCheckup(rover* r) {
	if (!r) return;
	r->incrementMissions();

	bool needsCheck = (r->getMissionsCompleted() >= r->getMissionsBeforeCheckup());
	LinkedQueue<rover*>* checkList = (r->getType() == 'N') ? &checkup_NR : (r->getType() == 'P') ? &checkup_PR : &checkup_DR;
	LinkedQueue<rover*>* availList = (r->getType() == 'N') ? &Avail_NR : (r->getType() == 'P') ? &Avail_PR : &Avail_DR;

	if (needsCheck) {
		r->setCheckupStartDay(day);
		r->resetMissions();
		checkList->enqueue(r);
	}
	else {
		r->setCheckupStartDay(0);
		availList->enqueue(r);
	}
}

void MarsStation::saveOutputFile() {
	ofstream file("output.txt");

	file << "Fday\tID\tRday\tWdays\tMDUR\tTdays" << endl;

	mission* m;
	int count = 0;
	int waitSum = 0, execSum = 0, durSum = 0;

	while (!DONE_missions.isEmpty()) {
		DONE_missions.pop(m);

		int fDay = m->getRDay() + m->getWaitingDays() + m->getExecutionDays();

		file << fDay << "\t" << m->getID() << "\t" << m->getRDay() << "\t"
			<< m->getWaitingDays() << "\t" << m->getDuration() << "\t"
			<< m->getExecutionDays() << endl;

		waitSum += m->getWaitingDays();
		execSum += m->getExecutionDays();
		durSum += m->getDuration();
		count++;
		delete m;
	}

	int abortedCount = Aborted_missions.getCount();
	int total = count + abortedCount;

	file << "......................................................." << endl;
	file << "Missions: " << total << " [N: " << totalMissions_N << ", P: " << totalMissions_P << ", D: " << totalMissions_D << "] "
		<< "[" << count << " DONE, " << abortedCount << " Aborted]" << endl;

	int totalRovers = numNM + numPM + numDM;
	file << "Rovers: " << totalRovers << " [N: " << numNM << ", P: " << numPM << ", D: " << numDM << "]" << endl;

	double avgWait = (count > 0) ? (double)waitSum / count : 0;
	double avgExec = (count > 0) ? (double)execSum / count : 0;
	double avgDur = (count > 0) ? (double)durSum / count : 0;

	file << fixed << setprecision(2);
	file << "Avg Wdays = " << avgWait << ", Avg MDUR = " << avgDur << ", Avg Tdays = " << avgExec << endl;

	double ratio = (avgDur > 0) ? (avgWait / avgDur) * 100 : 0;
	double autoRatio = (totalMissions_P > 0) ? ((double)autoAbortedCount / totalMissions_P) * 100 : 0;

	file << "% Avg Wdays / Avg MDUR = " << ratio << "%, Auto-aborted = " << autoRatio << "%" << endl;

	file.close();
}