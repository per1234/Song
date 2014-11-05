#ifndef Event_h
#define Event_h
#include "Arduino.h"

typedef struct Note {
    int note;
    int length;
    int velocity;
    Note* list;
} Note;

typedef struct CC {
    int number;       // Which CC
    int value;        // Assigned Value
    bool interpolate; // Should this CC be interpolated?
    CC* list;
} CC;

// Event is used to define the contents of a pattern. They can
// contain Notes or CC's. An event may have any number of Notes
// and CC's associated with it in the circumstance that multiple
// events happen concurrnetly.
// A MIDI parser may iterate through events to queue up data to 
// send.
class Event {
  private:
    int ticks;
    Note* notes;    // A stack implemented as a linked list
    CC* ccs;        // Also a stack implemented as a linked list
    Event *prev, *next;

    void  merge(  Event*);  // merge two events if their timing overlaps
    Note* removeNote( int);
    CC*   removeCC( int);
  public:
    Event( int, int, int, int);
    Event( int, int, int, bool);
    ~Event();
    void addNote( int, int, int, int);
    void addCC( int, int, int, bool);
    void move( int);
    Event* getNext();
    
    Note* getNotes();
    CC*   getCCs();
    int   getTime();
};

#endif