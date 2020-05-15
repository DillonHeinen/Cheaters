To use this program, follow the list of instructions below.

1. Navigate to directory containing source code and makefile.
2. Type "make" into shell.
3. This will create the cheaters executable that can be run by typing:
   "./plagiarismCatcher document_directory chunk_size similarity_threshold
   Substitute the final three fields above

This will compare the similarities between the different documents in the directory and tell you if there are any suspicious cases where cheating may have occurred.

As of this time, there do not appear to be any bugs with this current implementation. Everything should be in working order with each document set size.

The names of the files are:
main.cpp
cheaters.h
cheaters.cpp