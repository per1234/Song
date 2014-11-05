#ifndef Song_h
#define Song_h
#include "Pattern.h"

#include "Arduino.h"

// Number of patterns in a song
#define num_patterns 8

// This is a song data structure for music-based arduino (or otherwise) projects.
// My goal with this library is to offer the finest granularity of control over
// musical parameters in a well-organized and useful way. Useful for sequencers
// and possibly other applications.
class Song {
  private:
    Pattern **patterns;
  public:
    float tempo;
    float swing;

    Song();
    ~Song();
    Pattern* getPattern(int);
};

#endif