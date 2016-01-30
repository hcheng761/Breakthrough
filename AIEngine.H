#ifndef AI_ENG_H
#define AI_ENG_H

/*------------------------------------------------------------
 *	Description:
 *	The AI class implements three different difficulties of
 *	AI, basically using one of the following logic:
 *	1. Random choice
 *	2. Min-max algorithm & alpha-beta pruning
 *
 *	Authors:	
 *				David Cross
 *				Howard Cheng,
 *				John Pickering, 
 *				Noemie Nakamura
 *
 *	Date: October 2015
 *------------------------------------------------------------*/

#include <string>
#include <vector>
#include <utility>

#include "Board.H"

enum Difficulty { EASY, MEDIUM, HARD };

class AIEngine {

private:
    char pieceType;			// Should be either X or O
	Difficulty difficulty;	// Difficulty determines algorithm choice
	Board* board;			// Reference to a board
    
	// A valid piece is determined given a row and column
	// Returns true if the piece belongs to the player and
	// the piece can move
    bool isValidPiece(int i, int j);
    
	// A valid move is determined given a row and column and
	// a indication of forward move (which changes semantics depending
	// on the piece)
	// Returns true if the move is legal within board bounds
    bool isValidMove(int i, int j, bool forwards);
	
	// Returns a set of valid pieces for the current board configuration
	vector< pair<int,int> > getValidPieces();
	
	// Given a piece, returns the set of valid moves for that piece
	vector< pair<int,int> > getValidMoves(pair<int,int> piece);
	
	// Identifies the move made given a previous and post state of
	// boards. Returns the move in a string form.
	string getMove(Board &preBoard, Board &postBoard);
	
	// Given the pair integer positions (source and destination), writes
	// the move in the format:
	// <source_column><souce_row> to <dest_column><dest_row><newline>
	string writeMove(int sX, int sY, int dX, int dY);
	
	// Easy AI uses a random movement algorithm
	// Returns the selected movement
	string randMove();
	
	// Medium AI uses MIN-MAX algorithm
	// Wrapper function. Returns the selected movement
	string miniMove();
	// Recursive function to identify best move using MIN-MAX
	Board bestMove(Board board, char pawnType, int stopper, bool once, pair<int, int> node);
	
	// Medium AI uses alpha-beta pruning algorithm
	// Wrapper function. Returns the selected movement
	string alphaMove();
	Board alphaBeta(Board board, char curpawn, int stopper, bool once, pair<int, int> node, int a, int b);

public:
	AIEngine(Difficulty difficulty, char pieceType);
    ~AIEngine();
	
	string smartMove(Board* board);
};

#endif