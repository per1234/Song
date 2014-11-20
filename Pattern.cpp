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
 *                     through the list of notes. Sets currentNote to that note.
 * @t - tick count
 */
NoteEvent* Pattern::gotoNote(int t) {
    NoteEvent* n = getNote(t);
    currentNote = n;
    return n;
}

/**
 * Pattern::getNote - grabs the next note after t ticks. Works by iterating
 *                    through the list of notes
 * @t - tick count
 */
NoteEvent* Pattern::getNote(int t) {
    for (NoteEvent* n = notes; n->getNext() != NULL; n = n->getNext()) {
        if (n->getTime() < t && n->getNext()->getTime() >= t) {
            return n->getNext();
        }
    }
}

/**
 * Pattern::getNote - grabs the next note n after t ticks. Works by iterating
 *                    through the list of notes
 * @t - tick count
 * @n - the note number
 */
Note* Pattern::getNote(int t, int n) {
    for (NoteEvent* ne = notes; ne->getNext() != NULL; ne = ne->getNext()) {
        if (ne->getTime() < t && ne->getNext()->getTime() >= t) {
            // Found our t. Let's get our n
            ne = ne->getNext();
            for (Note* note = ne->getNotes(); note != NULL; note = note->list)
                if (n == note->note)
                    return note;
                else
                    return (Note*)0;
        }
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
 * Pattern::moveNote - Moves the specified note from t0 to tF. gotoNote should
 *                     be called immediately after this
 * @t0 - initial t value of note
 * @tF - final t value of note
 * @n  - the note
 */
void Pattern::moveNote( int t0, int tF, int n) {
    // Get the note structure we'll be moving
    Note* note = getNote(t0, n);
    int l = note->length;
    int v = note->velocity;
    // Remove it from the list
    removeNote(t0, n);
    // Re-insert it
    addNote(tF, n, l, v);
}


/**
 * Pattern::nextCC - gets the next CC. Updates iterator
 */
CCEvent* Pattern::nextCC() {
    if (currentCC == (CCEvent*)0) {
        return currentCC;
    }
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
    CCEvent* cc = getCC(t);
    currentCC = cc;
    return cc;
}

/**
 * Pattern::getCC - grabs the next CC after t ticks. Works by iterating
 *                  through the list of CCs
 * @t - tick count
 */
CCEvent* Pattern::getCC(int t) {
    for (CCEvent* cc = ccs; cc->getNext() != NULL; cc = cc->getNext()) {
        if (cc->getTime() < t && cc->getNext()->getTime() >= t) {
            currentCC = cc->getNext();
            return cc->getNext();
        }
    }
}

/**
 * Pattern::getCC - grabs the next CC after t ticks. Works by iterating
 *                  through the list of CCs
 * @t - tick count
 * @c - the CC number
 */
CC* Pattern::getCC(int t, int c) {
    for (CCEvent* ce = ccs; ce->getNext() != NULL; ce = ce->getNext()) {
        if (ce->getTime() < t && ce->getNext()->getTime() >= t) {
            // Found our t. Let's get our c
            ce = ce->getNext();
            for (CC* cc = ce->getCCs(); cc != NULL; cc = cc->list)
                if (c == cc->number)
                    return cc;
                else
                    return (CC*)0;
        }
    }
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
    }
    else {
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
void Pattern::removeCC( int ticks, int cc) {
    if (ccs == 0)
        return;
    else
        ccs = ccs->remove(ticks, cc);

    currentCC = ccs;
}

/**
 * Pattern::moveCC - Moves the specified CC from t0 to tF. gotoCC should
 *                   be called immediately after this
 * @t0 - initial t value of note
 * @tF - final t value of note
 * @c  - the CC
 */
void Pattern::moveCC( int t0, int tF, int c) {
    // Get the note structure we'll be moving
    CC* cc = getCC(t0, c);
    int  v = cc->value;
    bool i = cc->interpolate;
    // Remove it from the list
    removeCC(t0, c);
    // Re-insert it
    addCC(tF, c, v, i);
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