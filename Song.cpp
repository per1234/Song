#include "Arduino.h"

#include "Song.h"

/**
 * Song::Song - Initialize memory space for patterns and set initial 
 * tempo/swing amounts.
 */
Song::Song() {
    
    tempo = 120.0f;
    swing = 0.0f;

    patterns = (Pattern**)malloc(sizeof(Pattern*) * 8);

    for (int i = 0; i < num_patterns; i++) {
        patterns[i] = new Pattern();
        patterns[i]->name = 'a' + i;
    }
}

/**
 * Song::~Song - Frees space for loading a new song
 */
Song::~Song() {
    for (int i = 0; i < num_patterns; i++) {
        delete patterns[i];
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