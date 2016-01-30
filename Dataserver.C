/*-------------------------------------------------------------------------- 
 *	Description:
 *	This file implements a game server 
 *
 *	Authors: R. Bettati, Noemie Nakamura
 *  
 *  Date: October 2015
 *--------------------------------------------------------------------------*/

#include <cstring>
#include <iostream>
#include <iterator> 
#include <locale>
#include <sstream>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "NetworkReqChannel.H"
#include "Game.H"

#define MAX_MESSAGE 256
#define DEBUG

using namespace std;

/*--------------------------------------------------------------------------------
 *  GLOBAL VARIABLES
/*--------------------------------------------------------------------------------*/
const string password = "awesome";

string int2string(int number) {
   stringstream ss;     //create a stringstream
   ss << number;        //add number to the stream
   return ss.str();     //return a string with the contents of the stream
}

void clearBuffer(char* buffer, int size) {
	bzero(buffer, MAX_MESSAGE);
} 

#ifdef DEBUG
void printBuffer(char* buffer) {
	int size = strlen(buffer);
	
	cout << "buffer = {";
	
	for(int i = 0; i < size; i++) {
		cout << buffer[i];
		if (i != size-1) cout << ",";
	}
	cout << "}\n";
}
#endif

void correctBuffer(char* buffer) {
	int size = strlen(buffer);
	
	for(int i = 0; i < size; i++) {
		if (buffer[i] == '\n')
			buffer[i] = '\0';
	}
}

/*--------------------------------------------------------------------------------
 *  This function handles the game when it's two AIs playing against each other.
 *	The server configuration string is of the form:
 *
 *	AI-AI <server> <port> <pwd> <my_difficulty> <opponent_difficulty>
 *
 *	We can assume that the input string is correct, as it has passed through the
 *	game parser before. For the same reason, my_difficulty has already been set.
/*--------------------------------------------------------------------------------*/
void handleAIGame(string serverConfig, Game* game) {
	// TO-DO
	// Setup configurations for client-server
    istringstream buffer(serverConfig);
    istream_iterator<string> beg(buffer), end;
    vector<string> tokens(beg, end);
	
#ifdef DEBUG
	for(int i = 0; i < tokens.size(); i++)
		cout << tokens[i] << " / ";
	cout << endl;
#endif
	
	string hostName 	= tokens[1],
		   password 	= tokens[3],
		   opponentDiff = tokens[5];
	int portNum = stoi(tokens[2]);

		   // Open a new client channel and attempt to connect to external server
	NetworkRequestChannel *channel = new NetworkRequestChannel(hostName, portNum);
	
	// The server will request the password
    // string reply;
    // reply = channel->send_request(password);
    
	// // Loop until correct password
	// while(reply != "WELCOME\n") {
    // }
    
	// // Loop until game is over
    // while(reply != "GAME OVER\n") {
    // }
}

/*--------------------------------------------------------------------------------
 *  Takes in a socket s which from which data will be read and answered back
/*--------------------------------------------------------------------------------*/
void* connection_handler(void* s) {
    int socket = *((int*) s);   // Get the server socket
    char buf[MAX_MESSAGE];      // A buffer to hold the reading from server socket
	
    // Create a new game
    Game* game = new Game();
	
	// Get correct password before user can start playing the game
	string request;
	do {
		clearBuffer(buf, MAX_MESSAGE);
		write(socket, "PASSWORD\n", strlen("PASSWORD\n"));
		
		// Read in the request from the Client and put it into string form.
		// During testing we know that 2 extra characters are always added. 
		// So we remove them and then finalize the request string.
		
		clearBuffer(buf, MAX_MESSAGE);
		read(socket, buf, MAX_MESSAGE);

		request = string(buf);
		buf[request.length()-1] = '\0';
		request = string(buf);
	
#ifdef DEBUG
        cout << "CLIENT <" << request << ">\n";
#endif

    } while (request != password);
	
	clearBuffer(buf, MAX_MESSAGE);
    write(socket, "WELCOME\n", strlen("WELCOME\n"));
    
	// Listen while a command isn't exit or someone hasn't won yet
	do {
		clearBuffer(buf, MAX_MESSAGE);
		read(socket, buf, MAX_MESSAGE);

		request = string(buf);
		buf[request.length()-1] = '\0';
		request = string(buf);
		
	#ifdef DEBUG
		cout << "CLIENT <" << request << ">\n";
	#endif
		
		// "Save" the board before request
		string preBoardContent;
		preBoardContent = game->getBoardContent();
		
		// Parse the user input and get response
		string response;
		response = game->execute(request);	

	#ifdef DEBUG
		cout << "SERVER <" << response << ">\n";
	#endif
		
		clearBuffer(buf, MAX_MESSAGE);
		write(socket, response.c_str(), strlen(response.c_str()));

		if(game->displayIsActive()) {
			/**************************************************
				SECTION TO SEND BOARD OVER TO CLIENT GUI
			 **************************************************/
			// Get the current board configuration 
			string boardContent;
			boardContent = game->getBoardContent();
		   
			// Send the board content over socket for GUI display 
			clearBuffer(buf, MAX_MESSAGE);
			write(socket, boardContent.c_str(), strlen(boardContent.c_str()));
			
		}
		
		// If the user requested for an AI-AI game, handle differently
		if(game->isAIOnly()) {
			clearBuffer(buf, MAX_MESSAGE);
			handleAIGame(request, game);
		}
		
		// Get the board after request 
		string posBoardContent;
		posBoardContent = game->getBoardContent();
		
		// If the board has changed, the player has made a movement
		// It means the AI should make a movement
		if(preBoardContent != posBoardContent) {
			//game->aiMove();
			string move = game->getAiMove();
			clearBuffer(buf, MAX_MESSAGE);
			write(socket, move.c_str(), strlen(move.c_str()));
			
			/**************************************************
				SECTION TO SEND BOARD OVER TO CLIENT GUI
			 **************************************************/
			// Get the current board configuration 
			string boardContent;
			boardContent = game->getBoardContent();
		   
			// Send the board content over socket for GUI display 
			clearBuffer(buf, MAX_MESSAGE);
			write(socket, boardContent.c_str(), strlen(boardContent.c_str()));
		}
		
		// Check for undo
		if(response == "UNDO OK\n") {
			/**************************************************
				SECTION TO SEND BOARD OVER TO CLIENT GUI
			 **************************************************/
			// Get the current board configuration 
			string boardContent;
			boardContent = game->getBoardContent();
		   
			// Send the board content over socket for GUI display 
			clearBuffer(buf, MAX_MESSAGE);
			write(socket, boardContent.c_str(), strlen(boardContent.c_str()));
		}
		
	} while (!game->isOver());

    // End the current game
	cout << "End of game\n";
	delete game;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    char c, *p, *b; 
    int pFlag = 0, bFlag = 0;
    int port_num, backlog;
    
    /**
     * Read arguments from command line: ./client 
     * -p <port number for data server>
     * -b <backlog of the server socket>
     **/
    
    while ((c = getopt (argc, argv, "p:b:")) != -1) {
        switch (c) {
        case 'p':
          pFlag = 1;
          p = optarg;
          port_num = atoi(p);
          break;
        case 'b':
          bFlag = 1;
          b = optarg;
          backlog = atoi(b);
          break;
        }
    }
    
    if(pFlag == 0 || bFlag == 0) {
        if(pFlag == 0)
            cerr << "Missing -p option." << endl;
        if(bFlag == 0)
            cerr << "Missing -b option." << endl;
      exit(EXIT_FAILURE);	       
    }
    
    NetworkRequestChannel(port_num, backlog, connection_handler);
    return 0;
}

