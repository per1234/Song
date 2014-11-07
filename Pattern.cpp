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
 * Pattern::gotoNote - grabs the next note after t ticks. Works by iterating
 *                     through the list of notes
 * @t - tick count
 */
NoteEvent* Pattern::gotoNote(int t) {
    for (NoteEvent* n = notes; n->getNext() != NULL; n = n->getNext()) {
        if (n->getTime() < t && n->getNext()->getTime() >= t)
            return n->getNext();
    }
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
 * Pattern::gotoCC - grabs the next CC after t ticks. Works by iterating
 *                   through the list of CC's
 * @t - tick count
 */
CCEvent* Pattern::gotoCC(int t) {
    for (CCEvent* cc = ccs; cc->getNext() != NULL; cc = cc->getNext()) {
        if (cc->getTime() < t && cc->getNext()->getTime() >= t)
            return cc->getNext();
    }
}

/**
 * Pattern::addNote - Add a new note to a pattern.
 *                    Beware - after calling this, nextNote will return the
 *                    first note in the list. It is recommended that you call 
 *                    'gotoNote' immediately after calling this.
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
    }
    currentNote = notes;
}

/**
 * Pattern::removeNote - Removes the specified note from the pattern.
 *                       Resets the note iterator. It is recommended that you
 *                       call 'gotoNote' immediately after calling this.
 * @ticks - the time that the note occurs
 * @note  - the note number
 */
void Pattern::removeNote( int ticks, int note) {
    if (notes == 0)
        return;
    else
        notes = notes->remove(ticks, note);

    currentNote = notes;
}

/**
 * Pattern::addEvent - Add a new CC to a pattern.
 *                     Beware - after calling this, nextCC will return the
 *                     first note in the list. It is recommended that you call
 *                     'gotoCC' immediately after calling this.
 * @ticks       - the timing of the CC
 * @number      - the CC number to add
 * @value       - the CC value to add
 * @interpolate - whether this CC interpolates or not
 */
void Pattern::addCC( int ticks, int number, int value, bool interpolate) {
    if (ccs == 0) {
        ccs = new CCEvent(ticks, number, value, interpolate);
        currentCC = ccs;
    } else {
        CCEvent* currentStart = ccs;
        ccs->add(ticks, number, value, interpolate);
    }
    currentCC = ccs;
}

/**
 * Pattern::removeCC - Removes the specified CC from the pattern.
 *                     Resets the cc iterator. It is recommended that you
 *                     call 'gotoCC' immediately after calling this.
 * @ticks - the time of the CC to remove
 * @cc    - the CC number to remove
 */
Pattern::removeCC( int ticks, int cc) {
    if (ccs == 0)
        return;
    else
        ccs = ccs->remove(ticks, cc);

    currentCC = ccs;
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