#ifndef CCEvent_h
#define CCEvent_h
#include "Arduino.h"

typedef struct CC {
    int number;       // Which CC
    int value;        // Assigned Value
    bool interpolate; // Should this CC be interpolated?
    CC* list;
} CC;

// CCEvent is used to define the CC contents of a pattern. An event may have 
// any number of CC's associated with it in the circumstance that multiple 
// events happen concurrently.
// A MIDI parser may iterate through events to queue up data to send.
class CCEvent {
  private:
    int ticks;
    CC* ccs;        // Also a stack implemented as a linked list
    CCEvent *prev, *next;

    void merge(CCEvent*);
  public:
    CCEvent( int, int, int, bool);
    ~CCEvent();
    CCEvent* add( int, int, int, bool);
    CCEvent* remove(int, int);
    void move( int);
    CCEvent* getNext();
    
    CC*   getCCs();
    int   getTime();
};

#endif