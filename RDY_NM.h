#pragma once
#include "./LinkedQueue.h"
#include "./mission.h"

class RDY_NM : public LinkedQueue<mission*>
{
public:
    // Remove mission with given ID from the ready normal queue and return it.
    // Returns nullptr if not found.
    mission* AbortMission(int id) {
        mission* m = nullptr;
        mission* found = nullptr;
        LinkedQueue<mission*> temp;

        while (!this->isEmpty()) {
            this->dequeue(m);
            if (m->getID() == id && found == nullptr) {
                found = m; // remove this mission
            }
            else {
                temp.enqueue(m);
            }
        }

        // restore remaining missions
        while (!temp.isEmpty()) {
            temp.dequeue(m);
            this->enqueue(m);
        }

        return found;
    }

};