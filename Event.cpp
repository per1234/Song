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
 * Event::insert - Insert a new event to the linked list
 * @e - the event to be inserted
 */
void Event::insert( Event *e) {
    // If event is the same, merge with this one
    if (e->getTime() == ticks) {
        merge(e);
    }
    // Place event between this and the next if it belongs
    else if (e->getTime() > ticks &&
        e->getTime() < next->getTime()) {
        
        e->next = next;
        e->prev = this;

        next->prev = e;
        next = e;
    }
    // Send down the line if it belongs further
    else if (e->getTime() > next->getTime()) {
        next->insert(e);
    }
}

/**
 * Event::move - move the event on the timeline. Takes care of merging
 * shifting in the linked list
 * @ticks - absolute position to move to
 */
void Event::move(int ticks) {

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
 * @e - the event to be merged into
 */
void Event::merge( Event* e) {

}

/**
 * Event::remove - remove a note from the list of notes in this event
 * @n - the note to be removed
 */
Note* Event::remove( Note* n) {
    // Iterate through notes to see if we have a match
    for (Note *note = notes; note != NULL; note = note->list) {
        if (note->note == n->note)
    }
}

/**
 * Event::remove - remove a CC from the list of CC's in this event
 * @cc - the CC to be removed
 */
CC* Event::remove( CC* cc) {

}

/**
 * Event::getTime - gets the time of this event in ticks
 */
int Event::getTime() {
    return ticks;
}