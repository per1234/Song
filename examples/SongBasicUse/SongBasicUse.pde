#include "Song.h"

Song *song;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(2)) {
    Pattern* p = song->getPattern(0);
    printPattern(p);
    delay(500);
  }
  if (!digitalRead(3)) {
    Pattern* p = song->getPattern(1);
    printPattern(p); 
    delay(500);
  }
  if (!digitalRead(4)) {
    Pattern* p = song->getPattern(2);
    printPattern(p); 
    delay(500);
  }
  if (!digitalRead(5)) {
    createPatterns();
    //Pattern* p = song->getPattern(3);
    //printPattern(p); 
    delay(500);
  }
}

void createPatterns() {
  song = new Song();
  // Test basic out-of-order insertion
  Pattern *p = song->getPattern(0);
  p->addNote(0, 32, 0, 0);
  p->addNote(10, 30, 0, 0);
  p->addNote(5, 2, 0, 0);
  // Test moving note
  p->moveNote(5, 15, 2);
  // Test inserting at the beginning
  p = song->getPattern(1);
  p->addNote(10, 10, 0, 0);
  p->addNote(5, 5, 0, 0);
  p->addNote(1, 1, 0, 0);
  // Test removing
  p->removeNote(10, 10);
  p->removeNote(1, 1);
  // Test layered insertion
  p = song->getPattern(2);
  p->addNote(25, 10, 0, 0);
  p->addNote(10, 20, 0, 0);
  p->addNote(25, 11, 0, 0);
  // Test merging
  p->addNote(10, 20, 35, 0);
}

void printPattern(Pattern* p) {
  NoteEvent *ne = p->nextNote();
  while (ne != (NoteEvent*)0) {
    Note* n = ne->getNotes();
    while (n != (Note*)0) {
     Serial.print(n->note);
     Serial.print(" ");
     Serial.print(n->length);
     Serial.print(" ");
     Serial.println(n->velocity);
     n = n->list;
    }
    Serial.println();
    ne = p->nextNote();
  }
  p->reset();
}

int readSerialInput() {
 while (Serial.available() == 0) {
  int input = Serial.parseInt();
  while (Serial.available() > 0) {
   Serial.read(); 
  }
  return input;
 } 
}
