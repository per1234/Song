#include "Arduino.h"

#include "Event.h"

/**
 * Event::Event - initialize a new event with a Note object
 * @ticks - where on the timeline this event occurs
 * @note  - The note (in MIDI note numbers) of the event
 * @length - The length of the note
 * @velocity - velocity of the note
 */
Event::Event( int ticks, int note, int length, int velocity) {
    this->ticks = ticks;
    Note *n = (Note*)malloc(sizeof(Note));

    n->note = note;
    n->length = length;
    n->velocity = velocity;
    n->list = NULL;

    this->notes = n;
}

/**
 * Event::Event - initialize a new event
 * @ticks  - where on the timeline this event occurs
 * @number - CC number
 * @value  - CC value
 * @interploate - If we should interpolate to the next CC
 */
Event::Event( int ticks, int number, int value, bool interpolate) {
    this->ticks = ticks;
    CC *cc = (CC*)malloc(sizeof(CC));

    cc->number = number;
    cc->value = value;
    cc->interpolate = interpolate;
    cc->list = NULL;

    this->ccs = cc;
}

/**
 * Event::~Event - Destroy the event. Lists should not be assumed to be 
 * destroyed here since their members may be referenced elsewhere.
 * However, this will link prev and next pointers to each other.
 */
Event::~Event() {
    if (prev != NULL)
        prev->next = next;
    if (next != NULL)
        next->prev = prev;
}

/** 
 * Event::addNote - Insert a new note to the list of Notes
 * @t        - the timing of the note in ticks
 * @note     - the note number (MIDI number)
 * @length   - the length of the note
 * @velocity - velocity of the note
 */
void Event::addNote( int t, int note, int length, int velocity) {
    // If event is the same, merge with this one
    if (t == ticks) {
        // Make the note
        Note* n = (Note*)malloc(sizeof(Note));
        n->note     = note;
        n->length   = length;
        n->velocity = velocity;

        // Place in list
        n->list = notes;
        notes   = n;
    }
    // Place event between this and the next if it belongs
    else if (t > ticks &&
        t < next->getTime()) {
        
        Event *e = new Event(t, note, length, velocity);

        e->next = next;
        e->prev = this;

        next->prev = e;
        next = e;
    }
    // Send down the line if it belongs further
    else if (t > next->getTime()) {
        next->addNote(t, note, length, velocity);
    }
}

/** 
 * Event::addCC - Insert a new CC to the list of CC's
 * @t           - the timing of the note in ticks
 * @number      - the CC number
 * @value       - the value of the control change
 * @interpolate - whether or not to interpolate to the next CC event
 */
void Event::addCC( int t, int number, int value, bool interpolate) {
    // If event is the same, merge with this one
    if (t == ticks) {
        // Make the CC
        CC* cc = (CC*)malloc(sizeof(CC));
        cc->number      = number;
        cc->value       = value;
        cc->interpolate = interpolate;

        // Place in list
        cc->list = ccs;
        ccs      = cc;
    }
    // Place event between this and the next if it belongs
    else if (t > ticks &&
        t < next->getTime()) {
        
        Event *e = new Event(t, number, value, interpolate);

        e->next = next;
        e->prev = this;

        next->prev = e;
        next = e;
    }
    // Send down the line if it belongs further
    else if (t > next->getTime()) {
        next->addCC(t, number, value, interpolate);
    }
}

/**
 * Event::move - move the event on the timeline. Takes care of merging
 * shifting in the linked list
 * @ticks - absolute position to move to
 */
void Event::move(int ticks) {
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
 * Event::getNext - gets the next event
 */
Event* Event::getNext() {
    return next;
}

/**
 * Event::getNotes - gets the note list
 */
Note* Event::getNotes() {
    return this->notes;
}

/**
 * Event::getCCs - gets the control change list
 */
CC* Event::getCCs() {
    return this->ccs;
}

/**
 * Event::merge - merge another event into this one
 * @e - the event to be merged
 */
void Event::merge( Event* e) {

}

/**
 * Event::removeNote - remove a note from the list of notes in this event
 * @n - the note to be removed
 */
Note* Event::removeNote( int n) {
    // Iterate through notes to see if we have a match
    for (Note *note = notes; note != NULL; note = note->list) {
        // Our first case is if the first note is the one we're looking for
        if (note->note == n) {
            Note *retNote = note;
            notes = retNote->list;
            return retNote;
        }
        // Our second is everything else
        else if (note->list != NULL) {
            if (note->list->note == n) {
                Note *retNote = note->list;
                note->list = retNote->list;
                return retNote;
            }
        }
    }

    return NULL;
}

/**
 * Event::removeCC - remove a CC from the list of CC's in this event
 * @n - the CC number to be removed
 */
CC* Event::removeCC( int n) {
    // Iterate through CCs to see if we have a match
    for (CC *cc = ccs; cc != NULL; cc = cc->list) {
        // Our first case is if the first cc is the one we're looking for
        if (cc->number == n) {
            CC *retCC = cc;
            ccs = retCC->list;
            return retCC;
        }
        // Our second is everything else
        else if (cc->list != NULL) {
            if (cc->list->number == n) {
                CC *retCC = cc->list;
                cc->list = retCC->list;
                return retCC;
            }
        }
    }

    return NULL;
}

/**
 * Event::getTime - gets the time of this event in ticks
 */
int Event::getTime() {
    return ticks;
}