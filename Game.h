#ifndef GAME_H
#define GAME_H

/*------------------------------------------------------------
 *	Description:
 *	This class implements the Breakthrough game rules.
 *
 *	Note: A game will always begin with the client, which will
 *	always have 'O' as its piece.
 *
 *	Author: Noemie Nakamura, David Cross
 *
 *	Date: October 2015
 *------------------------------------------------------------*/

#include <string>
#include <vector>

#include "Board.H"
#include "AIEngine.H"
// #include "AI.H"

class Game {
private:
	// NESTED CLASS
	class GameParser;
private:	
	// VARIABLES
	GameParser *parser;
	
	// AI *ai;
	AIEngine *ai;
	
	Board *prevBoard;	// Reference to previous state of the board
	Board *curBoard;	// Reference to current state of the board

	bool gameOver;
	bool display;
	bool aiMode;
	
	// METHODS
	// Sets up the AI difficulty once
	void setupAI(string diff, char pawnType);
	
	// Checks validity of a single movement
	bool moveIsValid(int sX, int sY, int dX, int dY);
	
	// Moves a (user) piece from (sX, sY) to (dX, dY)
	// Returns true if the game is over
	bool movePiece(int sX, int sY, int dX, int dY);
	
	// Undoes one single movement
	bool undoMovement();
	
	// "Saves" the curBoard to prevBoard
	void saveBoard();
	
	// Activates/deactivates the board display
	void toggleDisplay();
	
	// Returns ON or OFF for display state
	string getDisplayState();

public:
	Game();
	~Game();
	
	// Returns true if this is a AI-AI game
	bool isAIOnly();
	
	// AI makes a move according to its difficulty and the curBoard
	void aiMove();
	
	// Returns the AI's move according to its difficulty and the curBoard
	string getAiMove();
	
	// Displays the current board
	void displayBoard();
	
	// Returns a string with the board content
	string getBoardContent();
	
	// Returns a string with the whole board configuration
	string getBoardFullContent();
	
	// Check if anyone has won in the current state of the game
	bool isOver();
	
	// Returns true if the display is on
	bool displayIsActive();
	
	// Interprets user input commands and returns response
    string execute(string input);
	
};

/*------------------------------------------------------------
 *	Description:
 *	The GameParser class is implemented privately solely to
 *	interpret user input and correspondingly call game functions.
 *	Note that this parser is case insensitive!
 *
 *	Below is a list of the possible commands that can be input:
 *
 *	1. EXIT
 *	2. DISPLAY
 *	3. UNDO
 *	4. HUMAN-AI <difficulty>
 *		a) difficulty = { EASY, MEDIUM, HARD }
 *		Eg. human-ai medium
 *	5. AI-AI <server> <port> <pwd> <difficulty> <difficulty>
 *		a) server 	  = hostname
 *		b) port		  = port number
 *		c) pwd		  = password
 *		d) difficulty = { EASY, MEDIUM, HARD }
 *		Eg. ai-ai localhost 3467 pickles easy easy
 *	6. <s_column s_row> TO <d_column d_row>
 *	 	a) s_column = { a, b, c, d, e, f, g, h }
 *		b) s_row	= { 1, 2, 3, 4, 5, 6, 7, 8 }
 *		c) d_column = { a, b, c, d, e, f, g, h }
 *		d) d_row	= { 1, 2, 3, 4, 5, 6, 7, 8 } 
 *		Eg. c1 to c2
 *
 *	Author: Noemie Nakamura
 *	Date: October 2015
 *------------------------------------------------------------*/
class Game::GameParser {
private:
	// VARIABLES
	Game *game;
	vector<string> tokens;
	
	// METHODS
	// Checks if pointer can move
	bool isLastToken(int pointer);
	
	// Structures invalid message
	string writeInvalidMessage(string message);
	
	// Structures incomplete message
	string writeIncompleteMessage(string message);
	
	// Returns a vector of valid tokens
	vector<string> tokenize(string line);
	
	// Evaluates commands
	string evalCommand(int &pointer);
	
	// Evaluates movement input
	string evalMovement(int &pointer);
	
	// Evaluates player setup input
	string evalPlayer(int &pointer);
	
public:
	// Constructors
	GameParser(Game *game);
	~GameParser();
	
	// Interprets user input commands and returns response
	string parseInput(string input);
	
};

#endif