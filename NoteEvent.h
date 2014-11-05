#ifndef NoteEvent_h
#define NoteEvent_h
#include "Arduino.h"

typedef struct Note {
    int note;
    int length;
    int velocity;
    Note* list;
} Note;

// NoteEvent is used to define the Note contents of a pattern. An event may 
// have any number of CC's associated with it in the circumstance that 
// multiple events happen concurrently.
// A MIDI parser may iterate through events to queue up data to send.
class NoteEvent {
  private:
    int ticks;
    Note* notes;    // A stack implemented as a linked list
    NoteEvent *prev, *next;

    void  merge( NoteEvent*);  // merge two events if their timing overlaps
    Note* remove( int);
  public:
    NoteEvent( int, int, int, int);
    ~NoteEvent();
    NoteEvent* add( int, int, int, int);
    void move( int);
    NoteEvent* getNext();
    
    Note* getNotes();
    int   getTime();
};

#endif