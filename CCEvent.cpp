#include "Arduino.h"

#include "CCEvent.h"

/**
 * CCEvent::Event - initialize a new CC CCEvent
 * @ticks  - where on the timeline this CCEvent occurs
 * @number - CC number
 * @value  - CC value
 * @interploate - If we should interpolate to the next CC
 */
CCEvent::CCEvent( int ticks, int number, int value, bool interpolate) {
    this->ticks = ticks;
    CC *cc = (CC*)malloc(sizeof(CC));

    cc->number = number;
    cc->value = value;
    cc->interpolate = interpolate;
    cc->list = (CC*)0;

    this->ccs = cc;
    next = (CCEvent*)0;
}

/**
 * CCEvent::~CCEvent - Destroy the CCEvent. Lists should not be assumed to be 
 * destroyed here since their members may be referenced elsewhere.
 * However, this will link prev and next pointers to each other.
 */
CCEvent::~CCEvent() {
    if (prev != 0)
        prev->next = next;
    if (next != 0)
        next->prev = prev;
}

/** 
 * CCEvent::add   - Insert a new CC to the list of CC's. Returns the start of
 *                  the CC list after insertion.
 * @t           - the timing of the note in ticks
 * @number      - the CC number
 * @value       - the value of the control change
 * @interpolate - whether or not to interpolate to the next CC CCEvent
 */
CCEvent* CCEvent::add( int t, int number, int value, bool interpolate) {
    // If we're inserting before the first CC in our list
    if (t < ticks) {
        CCEvent *cc = new CCEvent(t, number, value, interpolate);

        cc->next = this;
        cc->prev = (CCEvent*)0;

        prev = cc;

        return cc;
    }
    // If timing is the same, add to this list
    else if (t == ticks) {
        // check if CC already exists in this list (keyed by CC number)
        for (CC *member = ccs; member != NULL; member = member->list) {
            // Update the CC data
            if (member->number == number) {
                member->value = value;
                member->interpolate = interpolate;
                return this;
            }
        }

        // Make the CC
        CC* cc = (CC*)malloc(sizeof(CC));
        cc->number      = number;
        cc->value       = value;
        cc->interpolate = interpolate;

        // Place at top of stack
        cc->list = ccs;
        ccs      = cc;
    }
    // Place CCEvent between this and the next if it belongs
    else if (t > ticks && t < next->getTime()) {
        
        CCEvent *e = new CCEvent(t, number, value, interpolate);

        e->next = next;
        e->prev = this;

        next->prev = e;
        next = e;
    }
    // Send down the line if it belongs further
    else if (t > ticks && next != NULL) {
        next->add(t, number, value, interpolate);
    }
    // Or if we're at the end of the list, insert it
    else if (t > ticks) {
        CCEvent *e = new CCEvent(t, number, value, interpolate);

        e->prev = this;
        next = e;
    }

    return this;
}

/**
 * CCEvent::getNext - gets the next CCEvent
 */
CCEvent* CCEvent::getNext() {
    return next;
}

/**
 * CCEvent::getCCs - gets the control change list
 */
CC* CCEvent::getCCs() {
    return this->ccs;
}

/**
 * CCEvent::remove - remove a CC from the list of CC's in this CCEvent
 * @n - the CC number to be removed
 */
CCEvent* CCEvent::remove( int t, int number) {
    // Is it this time?
    if (t == ticks) {
        // iterate through cc list to see if we have a match
        for (CC* cc = ccs; cc != NULL; cc = cc->list) {
            // This should only happen for the first cc in the list
            if (cc->number == number) {
                ccs = cc->list;
                free(cc);

                // If this event is empty, delete this event.
                if (ccs == 0) {
                    CCEvent* returnCC = next;
                    delete this;
                    return returnCC;
                }
            }
            // This should happen for the rest of the notes
            else if (cc->list != 0 && cc->list->number == number) {
                CC* deleteMe = cc->list;
                cc->list = cc->list->list;
                free(deleteMe);
                return this;
            }
        }
    } else if (t > ticks) {
        next = next->remove(t, number);
    }
    return this;
}

/**
 * CCEvent::getTime - gets the time of this CCEvent in ticks
 */
int CCEvent::getTime() {
    return ticks;
}