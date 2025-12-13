#pragma once

#include <iostream>
#include "LinkedQueue.h"
#include "PriQueue.h"
#include "ArrayStack.h"
#include "mission.h"
#include "rover.h"
#include "MarsStation.h"

using namespace std;

enum UIMode { INTERACTIVE, SILENT };

class UI
{
private:
	UIMode mode;
	MarsStation* station;

public:
	UI(MarsStation* s);

	// Return currently selected mode
	UIMode getMode() const;

	void printDay(int day,
		LinkedQueue<mission*>* r_nm, LinkedQueue<mission*>* r_pm, LinkedQueue<mission*>* r_dm,
		LinkedQueue<rover*>* a_nr, LinkedQueue<rover*>* a_pr, LinkedQueue<rover*>* a_dr,
		PriQueue<mission*>* out, PriQueue<mission*>* exec, PriQueue<mission*>* back,
		ArrayStack<mission*>* done);
};