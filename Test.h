#pragma once
#include <iostream>

#include "mission.h"
#include "rover.h"

#include "LinkedQueue.h"
#include "ArrayStack.h"
#include "PriQueue.h"

using namespace std;

void testLinkedQueue();
void testStack();
void testPriQueue();

void testLinkedQueue()
{
	cout << "\n------------ TESTING LINKED QUEUE (DM) -------------\n";

	LinkedQueue<mission*> Q;
	mission* pMission = nullptr;

	cout << "Enqueuing 10 DM" << endl;

	for (int i = 1; i <= 10; i++)
	{
		// Using mission(char type, int loc, int dur, int id, int readyDay)
		pMission = new mission('D', 100, 10, i, 1);
		Q.enqueue(pMission);
	}

	// Print (will print each mission via operator<<)
	cout << "Printing Queue:" << endl;
	Q.print();
	cout << "Count in Queue: " << Q.getCount() << endl;

	// Dequeue
	cout << "Dequeuing elements: " << endl;
	while (Q.dequeue(pMission))
	{
		cout << "Dequeued Mission ID: " << *pMission << " (Type: D)" << endl;
		delete pMission;
	}
}

void testStack()
{
	cout << "\n------------ TESTING ARRAY STACK (PM) ------------\n";

	ArrayStack<mission*> S;
	mission* pMission = nullptr;

	cout << "Pushing 10 'PM' " << endl;

	for (int i = 1; i <= 10; i++)
	{
		pMission = new mission('P', 200, 15, i, 2);
		S.push(pMission);
	}

	cout << "Printing Stack: " << endl;
	S.print();

	cout << "Popping elements: " << endl;
	while (S.pop(pMission))
	{
		cout << "Popped Mission ID: " << *pMission << " (Type: P)" << endl;
		delete pMission;
	}
}


void testPriQueue()
{
	cout << "\n------------ TESTING PRIORITY QUEUE (NM) -------\n";

	PriQueue<mission*> PQ;
	mission* pMission = nullptr;

	cout << "Enqueuing 10 'NM' " << endl;

	for (int i = 1; i <= 10; i++)
	{
		pMission = new mission('N', 300, 50, i, 3);

		int priority = pMission->getDuration();

		PQ.enqueue(pMission, priority);
		cout << "Enqueued Mission " << i << " (N) with Priority " << priority << endl;
	}

	cout << "\nPrinting Priority Queue:" << endl;
	PQ.print();

	cout << "Dequeuing elements" << endl;

	int pri;
	while (PQ.dequeue(pMission, pri))
	{
		cout << "Dequeued Mission ID: " << *pMission << " Priority: " << pri << endl;
		delete pMission;
	}
}