#ifndef Song_h
#define Song_h
#include "Event.h"

#include "Arduino.h"

// Number of patterns in a song
#define num_patterns 8

// Pattners hold event data. Events are implemented as a sequential 
// linked-list. Your MIDI code should iterate through the event list in here to
// get note and CC data.
typedef struct Pattern {
    char name;
    Event* events;
} Pattern;

// This is a song data structure for music-based arduino (or otherwise) projects.
// My goal with this library is to offer the finest granularity of control over
// musical parameters in a well-organized and useful way. Useful for sequencers
// and possibly other applications.
class Song {
  private:
    Pattern **patterns;
  public:
    Pattern**  chains;
    float tempo;
    float swing;

    Song();
    ~Song();
    Pattern* getPattern(int);
    void clearPattern(int);
};

#endif