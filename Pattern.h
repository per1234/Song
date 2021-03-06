#ifndef Pattern_h
#define Pattern_h
#include "NoteEvent.h"
#include "CCEvent.h"

#include "Arduino.h"

// Patterns hold event data. Events are implemented as a sequential 
// linked-list. Your MIDI code should iterate through the event list in here to
// get note and CC data.
class Pattern {
  private:
    NoteEvent* notes;
    NoteEvent* currentNote;
    CCEvent*   ccs;
    CCEvent*   currentCC;
    Pattern*   follow;
  public:
    char name;

    Pattern();
    ~Pattern();

    NoteEvent* nextNote();
    NoteEvent* gotoNote(int);
    NoteEvent* getNote(int);
    Note*      getNote(int, int);
    
    void addNote( int, int, int, int);
    void removeNote(int, int);
    void moveNote( int, int, int);

    CCEvent* nextCC();
    CCEvent* gotoCC(int);
    CCEvent* getCC(int);
    CC*      getCC(int, int);

    void addCC( int, int, int, bool);
    void removeCC(int, int);
    void moveCC( int, int, int);

    void setFollow(Pattern*);
    void reset();
    void clear();
};

#endif