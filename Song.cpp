#include "Event.h"

#include "Arduino.h"

#include "Song.h"

/**
 * Song::Song - Initialize memory space for patterns and set initial 
 * tempo/swing amounts.
 */
Song::Song() {
    
    tempo = 120.0f;
    swing = 0.0f;

    patterns = (Pattern**)malloc(sizeof(Pattern*) * num_patterns);
    for (int i = 0; i < num_patterns; i++) {
        patterns[i] = (Pattern*)malloc(sizeof(Pattern));
        patterns[i]->name = 'a' + i;
    }
}

/**
 * Song::~Song - Frees space for loading a new song
 */
Song::~Song() {
    for (int i = 0; i < num_patterns; i++) {
        delete patterns[i]->events;
        free( patterns[i]);
    }
    free( patterns);
}

/**
 * Song::getPattern - retrieves a pattern
 * @p - the pattern number
 */
Pattern* Song::getPattern(int p) {
    return patterns[p];
}

/**
 * Song::clearPattern - clears all events in a pattern
 * @p - the pattern number
 */
void Song::clearPattern(int p) {
    delete patterns[p]->events;
}