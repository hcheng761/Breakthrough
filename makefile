CC = g++
CFLAGS = -std=c++11 -g

JCC = javac
JFLAGS = -g

all: Dataserver GUI
   
Player.o: Player.H Player.C
	$(CC) $(CFLAGS) -c Player.C

Board.o: Board.H Board.C
	$(CC) $(CFLAGS) -c Board.C

Game.o: Game.H Game.C
	$(CC) $(CFLAGS) -c Game.C

AIEngine.o: AIEngine.H AIEngine.C
	$(CC) $(CFLAGS) -c AIEngine.C	
	
NetworkReqChannel.o: NetworkReqChannel.H NetworkReqChannel.C
	$(CC) $(CFLAGS) -c NetworkReqChannel.C

Dataserver: Dataserver.C NetworkReqChannel.o Game.o Board.o AIEngine.o
	$(CC) $(CFLAGS) -o Dataserver Dataserver.C NetworkReqChannel.o Game.o Board.o AIEngine.o

GUI: GUI.java Client.java Cell.java
	$(JCC) $(JFLAGS) GUI.java Client.java Cell.java
		
clean:
	rm -f fifo* *.o *.class Dataserver
