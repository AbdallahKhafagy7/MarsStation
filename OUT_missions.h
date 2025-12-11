#pragma once
#include "./PriQueue.h"
#include "./mission.h"

class OUT_missions : public PriQueue<mission*>
{
public:
    // Remove mission with given ID from outgoing missions and return it.
    // This operation requires scanning the priority list; we'll rebuild the list excluding the mission.
    mission* AbortMission(int id) {
        mission* mm;
        int pr;
        mission* found = nullptr;

        // Use a temporary PriQueue to hold non-matching items
        PriQueue<mission*> temp;

        while (this->dequeue(mm, pr)) {
            if (mm->getID() == id && found == nullptr) {
                found = mm; // skip this one
            } else {
                temp.enqueue(mm, pr);
            }
        }

        // Move items back to this priority queue preserving priority order
        while (temp.dequeue(mm, pr)) {
            this->enqueue(mm, pr);
        }

        return found;
    }
};