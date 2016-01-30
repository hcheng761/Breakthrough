#ifndef BOARD_H
#define BOARD_H

#include <vector>

#define BOARD_SIZE 8

using namespace std;

class Board {
private:
	// The board representation
	vector< vector<char> > board;

public:
	Board();
	// Constructs a board from the content of another
	// Assumes correct content size BOARD_SIZE * BOARD_SIZE
	Board(string content);
	~Board();

	// Prints the current state of the board
	void draw();

	// Returns true if a position is empty
	bool isEmpty(int x, int y);
	
	// Returns a string with the board content
	// Here X = 1, O = 2 and empty = 3
	string getContent();
	
	// Returns a string with the whole board configuration
	string getFullContent();
	
	// Get the type of a piece at index column x, row y
	// Returns X, O or empty
	char getIndex(int x, int y);
	
	//Set the piece at index column x, row y to type c
	void setIndex(int x, int y, char c);
	
	// For AI algorithms
	int score;
	int checkVictory();
	
	// Operators
	Board& operator= (const Board &b);
};

int getColumnNum(char columnLetter);
char getColumnLetter(int columnNum);

#endif