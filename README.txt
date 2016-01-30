Description
----------------------------------------------------------------

In our program, the server is responsible for handling the game
mechanics. Therefore, it must include Game.H and provide the
game play according to the user input, which will come from the
client.

The client side is initially being run using telnet. 
In a final version, the client will be represented in a Java GUI.

There are two possible game modes: HUMAN-AI and AI-AI*.
In the second case, the server connects to another server, thus
acting as a client too.

Client-server compilation
----------------------------------------------------------------
Follow these instructions:

Observation: Run server in Linux (linux.cse.tamu.edu)*	
1.	Open two separate terminals.
2.	Run the makefile in one of the terminals.
3.	In one terminal, run the server by typing:
	
	./Dataserver -p <portnumber> -b 10
	
	Choose a high port number, such as 3333.
	We're using backlog 10 as default.
	
Observation: Run client GUI in Unix.*	
4.	In the other terminal, run the client by typing:
	
	1. java GUI
	2. A window will pop up requiring hostname and portnumber.
		- Use the above server (linux.cse.tamu.edu)
		- Use the same port number that you chose for the server.

When the program starts to run, it will request a password.
The local server password is "awesome".

*Notes: Please contact members if you encounter problems
----------------------------------------------------------------
1. Socket disparities
Due to server reading disparities in different computers, it is
possible that user input reading is incorrect. This is an issue
which could not be resolved, even with teaching assistant and
peer teacher evaluation.
Different systems and different compilers gave different readings.

Due to this problem, the AI-AI game was affected and it was decided
that this feature would be disabled.

2. The Unix system does not recognize 'bzero' used in the Dataserver,
so we require to compile Dataserver in the Linux server.

3. The Linux system does not support XMing, so we require to compile
the GUI in the Unix server.

4. Other
The undo movement feature was also discarded in the final version.