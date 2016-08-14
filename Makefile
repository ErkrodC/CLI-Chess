DESIGN = Chess

#libs
LIBS   = libGUIandAI.a
CC     = gcc
CFLAGS = -ansi -Wall -c
LFLAGS = -Wall
AR     = ar rc
RANLIB = ranlib

all:
	make Chess
	make test

Test.o: Chess.c Chess.h DataTypes.h
	$(CC) $(CFLAGS) -g -DDEBUG  Chess.c -o Test.o

AITest.o: AI.h AI.c DataTypes.h
	$(CC) $(CFLAGS) -g -DDEBUG AI.c -o AITest.o

#target to generate DIPs.o
GUITest.o: GUI.h GUI.c DataTypes.h
	$(CC) $(CFLAGS) -g -DDEBUG  GUI.c -o GUITest.o

#target to generate libfilter.a
libGUIandAITest.a: GUITest.o AITest.o
	$(AR) libGUIandAITest.a GUITest.o AITest.o
	$(RANLIB) libGUIandAITest.a


test: Test.o libGUIandAITest.a
	$(CC) $(LFLAGS) Test.o -lGUIandAITest -L. -o Test


#target to generate Chess.o
Chess.o: Chess.c Chess.h DataTypes.h
	$(CC) $(CFLAGS) Chess.c -o Chess.o 

#target to generate AI.o
AI.o: AI.h AI.c DataTypes.h
	$(CC) $(CFLAGS) AI.c -o AI.o

#target to generate GUI.o
GUI.o: GUI.h GUI.c DataTypes.h
	$(CC) $(CFLAGS)  GUI.c -o GUI.o

#target to generate ChessLog.o
ChessLog.o: ChessLog.c ChessLog.h
	$(CC) $(CFLAGS) ChessLog.c -o ChessLog.o

#target to generate libfilter.a
libGUIandAI.a: GUI.o AI.o ChessLog.o
	$(AR) libGUIandAI.a GUI.o AI.o ChessLog.o
	$(RANLIB) libGUIandAI.a

#target to generate PhotoLab
Chess: Chess.o libGUIandAI.a
	$(CC) $(LFLAGS) Chess.o -lGUIandAI -L. -o Chess

#target to clean the directory
clean:
	rm -f *.o $(DESIGN) $(LIBS)
	rm -f libGUIandAITest.a
	rm -f Test
	rm -f *.txt
