#include <algorithm>
#include <iostream>
#include "Game.H"

#define DEBUG

/*------------------------------------------------------------
 * GAME METHODS
 *------------------------------------------------------------*/
Game::Game() {
	parser = new GameParser(this);
	
	ai = NULL;
	
	prevBoard = NULL;
	curBoard = new Board();
	
	display = false;
	gameOver = false;
	
	// Default wait for user input 
	aiMode = false;
}

Game::~Game() {
	delete parser;
	
	if(prevBoard != NULL) delete prevBoard;
	delete curBoard;
	
	if(ai != NULL) delete ai;
}

bool Game::isAIOnly() { return aiMode == true; }

// Since this function is called by the parser, which checks for validity
// we can assume the string is valid here
void Game::setupAI(string diff, char pawnType) {
	// Interpret difficulty
	Difficulty difficulty;
	if(diff == "easy") 
		difficulty = Difficulty::EASY;
	else if(diff == "medium")
		difficulty = Difficulty::MEDIUM;
	else
		difficulty = Difficulty::HARD;
	
	// First time
	if(ai == NULL) {
		ai = new AIEngine(difficulty, pawnType);
		cout << "AI difficulty set up to " << difficulty << endl;
	} else {
		cout << "AI is already set up\n";
		return;
	}
}

void Game::saveBoard() {
	// Make temporary pointer to prevBoard to be deleted
	Board* tempBoard = prevBoard;
	
	// "Save" current state of the board in prevBoard
	// by making a copy
	prevBoard = new Board(curBoard->getContent());
	
	// Delete the previous state
	delete tempBoard;
}

string Game::getAiMove() {
	// Save board before modification
	saveBoard();
	
	// Retrieve result move
	string move = ai->smartMove(curBoard);
	return move; 
}

bool Game::moveIsValid(int sX, int sY, int dX, int dY) {
	// If player tries to move off the board (right side or bottom)
    if(( sX || sY || dX || dY ) > 7) {
#ifdef DEBUG
		cout << "Out of board bounds\n";
#endif
		return false;
	}
	
	//If a player tries to move off the board (left side or top)
    if(( sX || sY || dX || dY ) < 0) {
#ifdef DEBUG
		cout << "Out of board bounds\n";
#endif		
		return false;
	}
	
	// Get the piece type: 'X' or 'O'
	char piece = curBoard->getIndex(sX, sY);
	
	// Get the content of the location to be moved
	char space = curBoard->getIndex(dX, dY);
	
	// The user's default piece is always 'O'
	
	// If the selected location has no pieces
	if(piece == '\0') {
#ifdef DEBUG
		cout << "Empty space\n";
#endif		
		return false;
	}
	
	// If user tries to move piece that isn't theirs
	if(piece == 'X') {
#ifdef DEBUG
		cout << "Opponent's piece\n";
#endif	
		return false;
	}
	
	// If the space the player is moving to is not one of the three valid spaces: FWD, RIGHT, LEFT
    if(dX != sX && dX != (sX + 1) && dX != (sX - 1)) {
#ifdef DEBUG
		cout << "Invalid space\n";
#endif	
		return false;
	}
	
	// Remember the 'O' pieces are at the bottom and can only move up!
	// If the space the player is moving to is not up
	if(dY != (sY-1)) {
#ifdef DEBUG
		cout << "Invalid move\n";
#endif		
		return false;
	}
	
	// If the space the player is moving is forward and occupied
	if(dX == sX && space == 'X') {
#ifdef DEBUG
		cout << "Invalid move\n";
#endif			
		return false;
	}
	
	// If the space the player is moving to is occupied by a friend
	if(space == 'O') {
#ifdef DEBUG
		cout << "Occupied space\n";
#endif			
		return false;
	}
	
	return true;
}

bool Game::movePiece(int sX, int sY, int dX, int dY) {
	if(moveIsValid(sX, sY, dX, dY)) {
		// Save board before modification
		saveBoard();
		
		// Update board with movement
		curBoard->setIndex(sX, sY,'\0');
		curBoard->setIndex(dX, dY, 'O');
		
		// User has won if he reaches the top row
		if(dY == 0) return true;
		else return false;
		
	} else return false;
}

bool Game::undoMovement() {
	if (prevBoard == NULL) return false;
	
	Board* tempBoard = curBoard;
	
	curBoard = prevBoard;
	delete tempBoard;
	
	prevBoard = NULL;
	
	return true;
}

void Game::toggleDisplay() { 
	display = !display;
}

string Game::getDisplayState() {
	if(display == true) 
		return "DISPLAY ON\n";
	else 
		return "DISPLAY OFF\n";
}

void Game::displayBoard() {
	if(display) curBoard->draw();
}

bool Game::displayIsActive() {
	return display == true;
}

string Game::getBoardContent() {
	return curBoard->getContent();
}

string Game::getBoardFullContent() {
	return curBoard->getFullContent();
}

bool Game::isOver() { return gameOver; }

string Game::execute(string input) {
	return parser->parseInput(input);
}
/*------------------------------------------------------------
 * GAME PARSER METHODS
 *------------------------------------------------------------*/

Game::GameParser::GameParser(Game *game) {
	this->game = game;
};
Game::GameParser::~GameParser() {};

bool Game::GameParser::isLastToken(int pointer) {
	if(pointer == tokens.size() - 1) return true;
	else return false;
}

string Game::GameParser::writeInvalidMessage(string message) {
	return "Invalid error: " + message + "\n";
}

string Game::GameParser::writeIncompleteMessage(string message) {
	return "Incomplete error: " + message + "\n";
}

/** 
 * HELPER FUNCTION: Converts all string characters to lowercase
 */
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),::tolower);
	return s;
}

string Game::GameParser::parseInput(string input) {
	// Transform string to lowercase
	input = toLower(input);
	
	// Separate the input into tokens
	tokens = tokenize(input);
	
	// Begin evaluating from first token
	int pointer = 0;
	return evalCommand(pointer);
}

vector<string> Game::GameParser::tokenize(string line) {
	vector<string> tokens;
    string accumulator = "";
    
    if (line.empty()) return tokens;
    
    for ( ; ; ) {
        switch(line[0])	{
        case ' ':
			if (accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			break;
        
        case '\n':
            if(accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			break;
        
        default:
            accumulator += line.substr(0,1);
			break;
		}
		
		line.erase(0,1);

		if(line.size() == 0) {
			if(accumulator.size() != 0) 
				tokens.push_back(accumulator);
			break;
		}
    }
    
    return tokens;
}

string Game::GameParser::evalCommand(int &pointer) {
	string token = tokens[pointer];
	
	// Check for exit
	if (token == "exit") {
		game->gameOver = true;		
		return "GAME OVER\n";
	} 
	// Check for display
	else if (token == "display") {
		game->toggleDisplay();
		return game->getDisplayState();
	} 
	
	// Check for undo
	else if (token == "undo") {
		bool result = game->undoMovement();
		if(result == true) return "UNDO OK\n";
		else return "Cannot undo\n";
	}
	
	// Check for player types
	else if (token == "human-ai" || token == "ai-ai") {
		return evalPlayer(pointer);
	}
	
	// Check for movement: column and row origin
	else if (token.length() == 2) { 
		return evalMovement(pointer);
	}
	
	// Any other input is not interpretable
	else return "Unknown command\n";
}

string Game::GameParser::evalPlayer(int &pointer) {
	string token = tokens[pointer];
	
	// If game is of type HUMAN-AI
	if (token == "human-ai") {
		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("AI difficulty");
		
		pointer++;
		string diff = tokens[pointer];
		
		// Check ai difficulty
		if(diff != "easy" && diff != "medium" && diff != "hard")
			return writeInvalidMessage("AI difficulty");
					
		// If all input is correct, setup the AI difficulty
		game->aiMode = false;
		game->setupAI(diff, 'X');
		return "PLAYERS OK\n";
		
	}
	
	// If game is of type AI-AI
	else if (token == "ai-ai") {
		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("Server name");
		
		pointer++;
		string name = tokens[pointer];
		
		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("Server port");
		
		pointer++;
		string port = tokens[pointer];
		
		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("Server password");
		
		pointer++;
		string password = tokens[pointer];	

		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("My difficulty");
		
		pointer++;
		string myDiff = tokens[pointer];	

		// Check if pointer can move
		if(isLastToken(pointer))
			return writeIncompleteMessage("Opponent difficulty");
		
		pointer++;
		string opponentDiff = tokens[pointer];
		
		// If all input is correct, setup the local AI difficulty
		game->aiMode = true;
		game->setupAI(myDiff, 'O');		
		return "PLAYERS OK\n";
	} 
	
	// Any other input is not interpretable
	else {
		return writeInvalidMessage("Player type");
	} 
}

string Game::GameParser::evalMovement(int &pointer) {
	string token = tokens[pointer];
	
	// Source column and row
	char sColumn = token[0], sRow = token[1];
	int sColumnNum, sRowNum;
	
	// Check if source column is letter
	if(isalpha(sColumn)) {
		sColumnNum = getColumnNum(sColumn);
		
	} else return writeInvalidMessage("Source column");
	
	// Check if second character is number
	if(isdigit(sRow)) {
		sRowNum = sRow - '0'; 
		
	} else return writeInvalidMessage("Source row");
	
	
	// Check if pointer can move
	if(isLastToken(pointer))
		return writeIncompleteMessage("Movement");
		
	pointer++;
	token = tokens[pointer];
	
	// Check if token is "to"
	if (token != "to") 
		return writeInvalidMessage("Movement");
	
	// Check if pointer can move
	if(isLastToken(pointer))
		return writeIncompleteMessage("Movement");
		
	pointer++;
	token = tokens[pointer];
	
	// Check that tokens size is 2
	if(token.size() != 2) return writeInvalidMessage("Destination");
	
	// Destination column and row
	char dColumn = token[0], dRow = token[1];
	int dColumnNum, dRowNum;
	
	// Check if destination column is number
	if(isalpha(dColumn)) {
		dColumnNum = getColumnNum(dColumn);
		
	} else return writeInvalidMessage("Destination column");
					
	// Check if destination row is number
	if(isdigit(dRow)) {
		dRowNum = dRow - '0'; 
		
	} else return writeInvalidMessage("Destination row");
	
	// If input is right, update board with movement
	game->gameOver = game->movePiece(sColumnNum, sRowNum, dColumnNum, dRowNum);
		
	return "MOVE OK\n";
}

