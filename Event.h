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

class Event {
  private:
    int ticks;
    Note* notes;
    CC* ccs;
    Event *prev, *next;

    void  merge(  Event*);  // merge two events if their timing overlaps
    Note* remove( Note*);
    CC*   remove( CC*);
  public:
    Event( int, int, int, int);
    Event( int, int, int, bool);
    ~Event();
    void insert( Event*);
    void move( int);
    
    Note* getNotes();
    CC*   getCCs();
    int   getTime();
};

#endif