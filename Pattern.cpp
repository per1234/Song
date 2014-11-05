#include "Pattern.h"

#include "Arduino.h"

#include "NoteEvent.h"
#include "CCEvent.h"

/**
 * Pattern::Pattern - Initialize a new Pattern. Also initializes the Event linked list
 */
Pattern::Pattern() {
    notes = (NoteEvent*)0;
    currentNote = notes;
    ccs = (CCEvent*)0;
    currentCC = ccs;
    follow = this;
}

/**
 * Pattern::~Pattern - Destroy this pattern. Deletes the lists it contains
 */
Pattern::~Pattern() {
    clear();
}

/**
 * Pattern::nextNote() - gets the next 
 */
NoteEvent* Pattern::nextNote() {
    if (currentNote == (NoteEvent*)0) {
        return currentNote;
    }
    NoteEvent* note = currentNote;
    currentNote = currentNote->getNext();
    return note;
}

/**
 * Pattern::nextCC - gets the next CC. Updates iterator
 */
CCEvent* Pattern::nextCC() {
    CCEvent* cc = currentCC;
    currentCC = cc->getNext();
    return cc;
}

/**
 * Pattern::addNote - Add a new note to a pattern
 * @ticks    - the timing of the note
 * @note     - the note number (MIDI number) to add
 * @length   - the length of the note
 * @velocity - the velocity of the note
 */
void Pattern::addNote( int ticks, int note, int length, int velocity) {
    if (notes == 0) {
        notes = new NoteEvent(ticks, note, length, velocity);
        currentNote = notes;
    }
    else {
        NoteEvent* currentStart = notes;
        notes = notes->add(ticks, note, length, velocity);
        if (currentStart == currentNote)
            reset();
    }
}

/**
 * Pattern::addEvent - Add a new CC to a pattern
 * @cc - the CC to add
 * @ticks - the timing of the CC
 */
void Pattern::addCC( int ticks, int number, int value, bool interpolate) {
    if (ccs == 0) {
        ccs = new CCEvent(ticks, number, value, interpolate);
        currentCC = ccs;
    } else {
        CCEvent* currentStart = ccs;
        ccs->add(ticks, number, value, interpolate);
        if (currentStart = currentCC)
            reset();
    }
}

/**
 * Pattern::setFollow - Sets the follow action of this pattern. 
 *                      Default is itself (looping)
 * @next - The next pattern that follows this one
 */
void Pattern::setFollow( Pattern *next) {
    follow = next;
}

/**
 * Pattern::reset - resets the pattern to the beginning
 */
void Pattern::reset() {
    currentNote = notes;
    currentCC = ccs;
}

/**
 * Pattern::clear - reinitializes the events in this pattern
 */
void Pattern::clear() {
    NoteEvent *n = notes;
    while (n != 0) {
        NoteEvent *next = n->getNext();
        delete n;
        n = next;
    }
    CCEvent *cc = ccs;
    while (cc != 0) {
        CCEvent *next = cc->getNext();
        delete cc;
        cc = next;
    }
    reset();
}