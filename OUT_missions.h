#pragma once
#include "./PriQueue.h"
#include "./mission.h"

class OUT_missions : public PriQueue<mission*>
{
public:
    // Remove mission with given ID from outgoing missions and return it.
    // This operation requires scanning the priority list; we'll rebuild the list excluding the mission.
    mission* AbortMission(int id) {
        // Extract all items
        struct Item { mission* m; int p; };
        std::vector<Item> items;
        mission* mm;
        int pr;
        while (this->dequeue(mm, pr)) {
            items.push_back({mm, pr});
        }

        mission* found = nullptr;
        for (auto &it : items) {
            if (it.m->getID() == id && found == nullptr) {
                found = it.m; // remove
            }
        }

        // Re-enqueue remaining
        for (auto &it : items) {
            if (it.m != found) this->enqueue(it.m, it.p);
        }

        return found;
    }
};