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
 * CCEvent::move - move the CCEvent on the timeline. Takes care of merging
 * shifting in the linked list
 * @ticks - absolute position to move to
 */
void CCEvent::move(int ticks) {
    // move backward
    if (ticks < this->ticks) {
        if (ticks < next->ticks) {
            this->ticks = ticks;
        } else if (ticks == next->ticks) {
            next->merge(this);
        } else {
            // Iterate backward
        }
    } else if (ticks > this->ticks) {
        if (ticks > prev->ticks) {
            this->ticks = ticks;
        }
    }
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
 * CCEvent::merge - merge another CCEvent into this one
 * @e - the CCEvent to be merged
 */
void CCEvent::merge( CCEvent* e) {

}

/**
 * CCEvent::remove - remove a CC from the list of CC's in this CCEvent
 * @n - the CC number to be removed
 */
CC* CCEvent::remove( int n) {
    // Iterate through CCs to see if we have a match
    for (CC *cc = ccs; cc != 0; cc = cc->list) {
        // Our first case is if the first cc is the one we're looking for
        if (cc->number == n) {
            CC *retCC = cc;
            ccs = retCC->list;
            return retCC;
        }
        // Our second is everything else
        else if (cc->list != 0) {
            if (cc->list->number == n) {
                CC *retCC = cc->list;
                cc->list = retCC->list;
                return retCC;
            }
        }
    }

    return (CC*)0;
}

/**
 * CCEvent::getTime - gets the time of this CCEvent in ticks
 */
int CCEvent::getTime() {
    return ticks;
}