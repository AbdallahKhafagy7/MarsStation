#include "UI.h"
#include "MarsStation.h"
#include <iostream>

using namespace std;

void UI::printDay(int day,
    LinkedQueue<mission*>* r_nm, LinkedQueue<mission*>* r_pm, LinkedQueue<mission*>* r_dm,
    LinkedQueue<rover*>* a_nr, LinkedQueue<rover*>* a_pr, LinkedQueue<rover*>* a_dr,
    PriQueue<mission*>* out, PriQueue<mission*>* exec, PriQueue<mission*>* back,
    ArrayStack<mission*>* done)
{
    // Silent mode: do not print per-day output
    if (mode == SILENT) return;

    cout << "Current Day: " << day << endl;

    cout << "==============   Ready List(s)   ==============\n";
    cout << (r_nm->getCount() + r_pm->getCount() + r_dm->getCount()) << " Missions: ";
    cout << "NMs["; r_nm->print(); cout << "] PMs["; r_pm->print(); cout << "] DMs["; r_dm->print(); cout << "]" << endl;

    cout << "==============   Available Rovers List(s)   ==============\n";
    cout << (a_nr->getCount() + a_pr->getCount() + a_dr->getCount()) << " Rovers: ";
    cout << "NRs["; a_nr->print(); cout << "] PRs["; a_pr->print(); cout << "] DRs["; a_dr->print(); cout << "]" << endl;

    cout << "==============   OUT List   ==============\n";
    cout << out->getCount() << " Missions/Rovers: "; out->print(); cout << endl;

    cout << "==============   EXEC List   ==============\n";
    cout << exec->getCount() << " Missions/Rovers: "; exec->print(); cout << endl;

    cout << "==============   BACK List   ==============\n";
    cout << back->getCount() << " Missions/Rovers: "; back->print(); cout << endl;

    cout << "==============   Done List   ==============\n";
    cout << done->getCount() << " Missions: "; done->print(); cout << endl;

    cout << "--------------------------------------------------" << endl;

    if (mode == INTERACTIVE) {
        cout << "Press ENTER Key To Display Next Day\n";
        cin.get();
    }
}

UI::UI(MarsStation* s)
    : mode(INTERACTIVE), station(s)
{
    cout << "Choose Mode\n Type I For INTERACTIVE Mode \n Type S For SILENT Mode\n";
    char input;
    cin >> input;

    while (input != 'I' && input != 'S')
    {
        cout << "\nEnter I or S \n";
        cin >> input;
    }

    switch (input)
    {
    case 'I':
        mode = INTERACTIVE;
        break;
    case 'S':
        mode = SILENT;
        break;
    default:
        mode = INTERACTIVE;
        break;
    }
}

UIMode UI::getMode() const
{
    return mode;
}