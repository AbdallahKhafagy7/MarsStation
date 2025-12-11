#include "UI.h"

void UI::printDay(int day,
	LinkedQueue<mission*>* r_nm, LinkedQueue<mission*>* r_pm, LinkedQueue<mission*>* r_dm,
	LinkedQueue<rover*>* a_nr, LinkedQueue<rover*>* a_pr, LinkedQueue<rover*>* a_dr,
	PriQueue<mission*>* out, PriQueue<mission*>* exec, PriQueue<mission*>* back,
	ArrayStack<mission*>* done)
{
	cout << "Current Day: " << day << endl;

	cout << "  Waiting Missions (N/P/D): "
		<< r_nm->getCount() << " / " << r_pm->getCount() << " / " << r_dm->getCount() << endl;

	cout << "  Available Rovers (N/P/D): "
		<< a_nr->getCount() << " / " << a_pr->getCount() << " / " << a_dr->getCount() << endl;

	cout << "  In-Service: "
		<< "OUT: " << out->getCount()
		<< " | EXEC: " << exec->getCount()
		<< " | BACK: " << back->getCount() << endl;

	cout << "  Finished: " << done->getCount() << endl;
	cout << "--------------------------------------------------" << endl;
}