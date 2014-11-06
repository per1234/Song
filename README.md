Song
====

Arduino library for keeping track of song data

Provides Song data structure that includes 8 patterns. Each pattern may contain
any number of CC or Note events. This was created with the intention that you
will be using MIDI, but it could be used for other music control paradigms
as well.

Inside each Pattern is a linked list of NoteEvent and CCEvent objects. Each of
these contains a stack of any number of Note and CC structs. Memory management
may get out of control so you may want to impose some limitations.

If Note data is inserted to a list where there is already a Note with the same
number and timing, its velocity and length data will be overwritten and no
new Note will be added.

Timing is measured in 'ticks'. This allows you to define your PPQ in your 
application.