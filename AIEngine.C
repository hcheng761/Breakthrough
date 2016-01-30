#include "AIEngine.H"

#include <utility>
#include <iostream>

#include <limits.h>
#include <stdlib.h>
#include <time.h>

AIEngine::AIEngine(Difficulty difficulty, char pieceType) { 
	this->difficulty = difficulty;
	this->pieceType = pieceType;
}

AIEngine::~AIEngine() {}

bool AIEngine::isValidPiece(int i, int j) {
	char piece = board->getIndex(i, j);
	
	// Case your piece is X
	if(pieceType == 'X') {
		// If the given position has an X piece, consider movements down
        if(piece == 'X') {
            // Diagonal left
			if(j > 0 && i < 7) {
				if (isValidMove(i + 1, j - 1, false))
                    return true;
            }
			
			// Diagonal right
			if (j < 7 && i < 7) {
				if (isValidMove(i + 1, j + 1, false))
                    return true;
            }
			
			// Forward
			if (i < 7) {
				if (isValidMove(i + 1, j, true))
					return true;
			}
            
			return false;
        }
    }
	
	// Case your piece is O
    else {
        // If the given position has an O piece, consider movements up
		if(piece == 'O') {
            // Diagonal left
			if(j > 0 && i > 0) {
				if (isValidMove(i - 1, j - 1, false))
                    return true;
            }
			
			// Diagonal right
			if (j < 7 && i > 0) {
				if (isValidMove(i - 1, j + 1, false))
                    return true;
            }
			
			// Forward
			if (i > 0) {
				if (isValidMove(i - 1, j, true))
					return true;
			}
            return false;
        }
    }
    return false;
}

bool AIEngine::isValidMove(int i, int j, bool forwards) {
	// Check for board bounds
	if (i > 7 || i < 0 || j > 7 || j < 0)
		return false;
	
	// If the new position is empty, there are no restrictions
	if (board->getIndex(i, j) == '\0')
		return true;
	
	// If the new position has an O piece
	else if(board->getIndex(i, j) == 'O') {
		// Being X, if O is in front, you can't move there
		// Otherwise, X eats O
		if(pieceType == 'X')
			return !forwards;
		// Being O, you can't move there
		else return false;
	}
	
	// If the new position has an X piece
	else {
		// Being O, if X is in front, you can't move there
		// Otherwise, O eats X
		if(pieceType == 'O')
			return !forwards;
		// Being X, you can't move there
		else return false;
	}
}

vector< pair<int,int> > AIEngine::getValidPieces() {
	vector< pair<int,int> > validPieces;
    
    // Traverse whole board and store all valid pieces in vector of pairs
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isValidPiece(i,j)) {
                pair<int,int> p(i,j);
                validPieces.push_back(p);
            }
        }
    }
	
	return validPieces;
}

vector< pair<int,int> > AIEngine::getValidMoves(pair<int,int> piece) {
	vector< pair<int,int> > validMoves;
	
	// For that piece, check all valid moves and store in vector of pairs
    int sX = get<0>(piece),
		sY = get<1>(piece);
    
	// Case piece type is X
    if(pieceType == 'X') {
		if(sY > 0 && sX < 7) {
			if (isValidMove(sX + 1, sY - 1, false)) {
                pair<int,int> p(sX + 1, sY - 1);
                validMoves.push_back(p);
            }
        }
		
		if (sY < 7 && sX < 7) {
			if (isValidMove(sX + 1, sY + 1, false)) {
                pair<int,int> p(sX + 1, sY + 1);
                validMoves.push_back(p);
            }
        }
		
		if (sX < 7) {
			if (isValidMove(sX + 1, sY, true)) {
				pair<int,int> p(sX + 1, sY);
				validMoves.push_back(p);
			}
		}
	}
	
	// Case piece type is O 
    else {
        if(sY > 0 && sX > 0) {
			if (isValidMove(sX - 1, sY - 1, false)) {
                pair<int,int> p(sX - 1,sY - 1) ;
                validMoves.push_back(p);
            }
        }
		
		if (sY < 7 && sX > 0) {
			if (isValidMove(sX - 1, sY + 1, false)) {
                pair<int,int> p(sX - 1,sY + 1);
                validMoves.push_back(p);
            }
        }
		
		if (sX > 0) {
			if (isValidMove(sX - 1, sY, true)) {
				pair<int, int> p(sX - 1, sY);
				validMoves.push_back(p);
			}
		}
    }
	
	return validMoves;
}

string AIEngine::getMove(Board &preBoard, Board &postBoard) {
	string diff;
	int xO = -1;
	int yO = -1;
	int xM = -1;
	int yM = -1;
	if (pieceType == 'X')
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (xO == -1)
				{
					if (preBoard.getIndex(i, j) != postBoard.getIndex(i, j))
					{
						xO = i;
						yO = j;
					}
				}
				else
				{
					if (preBoard.getIndex(i, j) != postBoard.getIndex(i, j))
					{
						xM = i;
						yM = j;
					}
				}
			}
		}
	}
	else
	{
		for (int i = 7; i >= 0; i--)
		{
			for (int j = 7; j >= 0; j--)
			{
				if (xO != -1)
				{
					if (preBoard.getIndex(i, j) != postBoard.getIndex(i, j))
					{
						xO = i;
						yO = j;
					}
				}
				else
				{
					if (preBoard.getIndex(i, j) != postBoard.getIndex(i, j))
					{
						xM = i;
						yM = j;
					}
				}
			}
		}
	}
	
	return writeMove(xO, yO, xM, yM);
}

string AIEngine::writeMove(int sX, int sY, int dX, int dY) {
	char sColumn = getColumnLetter(sX), 
		 dColumn = getColumnLetter(dX); 
	
	string sRow = to_string(sY), 
		   dRow = to_string(dY);
		   
	return sColumn + sRow + " to " + dColumn + dRow + "\n";
}

string AIEngine::smartMove(Board* board) {
	this->board = board;
	
	if(difficulty == Difficulty::EASY)
        return randMove();
    else if(difficulty == Difficulty::MEDIUM)
        return miniMove(); 
    else
		return alphaMove();
}

string AIEngine::randMove() {
	srand(time(NULL));
    
	vector< pair<int,int> > validPieces = getValidPieces();
    
	// Randomly choose one of available pieces
    pair<int,int> piece = validPieces[rand() % validPieces.size()];   
    
	vector< pair<int,int> > validMoves = getValidMoves(piece);
    
    // Randomly choose one of available moves
    pair<int,int> move = validMoves[rand() % validMoves.size()];

    
	// Update the board
	int sX = get<0>(piece),
		sY = get<1>(piece);
	board->setIndex(sX, sY, '\0');
    
	int dX = get<0>(move), 
		dY = get<1>(move);

	if(pieceType == 'X')
        board->setIndex(dX, dY, 'X');
    if(pieceType == 'O')
        board->setIndex(dY, dY, 'O');
    
	// Convert move choice to string
	return writeMove(sX, sY, dX, dY);
}

string AIEngine::miniMove() {
	// Create a copy of the original board
	string content = board->getContent();
	Board preBoard(content);

	// Get best move
	board->score = 0;
    Board postBoard = bestMove(preBoard, pieceType, 2, true, pair<int, int>(0,0));

	// Set original board to resulting board
	// Set original board to resulting board
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++)
			board->setIndex(i, j, postBoard.getIndex(i, j));
	}
	
	// Get the selected move
	return getMove(preBoard, postBoard);
}

Board AIEngine::bestMove(Board board, char curpawn, int stopper, bool once, pair<int, int> node) {
	char pawn = curpawn;
	Board current = board;
	
	/*CHECKS FOR VICTORY OR DEFEAT*/
	if (stopper <= 0)
		return board;

	if (board.checkVictory() == 1 && pawn != curpawn)
	{
		board.score -= 1000;
		return board;
	}
	if (board.checkVictory() == 1 && pawn == curpawn)
	{
		board.score += 1000;
		return board;
	}
	if (board.checkVictory() == 2 && pawn != curpawn)
	{
		board.score -= 1000;
		return board;
	}
	if (board.checkVictory() == 2 && pawn == curpawn)
	{
		board.score += 1000;
		return board;
	}

	/*GET THE NEXT PAWN*/
	char nextPawn;
	if (curpawn == 'X')
		nextPawn = 'O';
	else
		nextPawn = 'X';

	/*FIND AVAILABLE PAWNS (NODES) TO MOVE FOR THE CURRENT PAWN TYPE ONLY DONE ONCE*/
	if (once)
	{
		board.score = 0;
		int bestScore = INT_MIN;
		Board best;
		char savePawn = pawn;
		pawn = curpawn;
		vector<pair<int, int>> pieces;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (isValidPiece(i, j))
				{
					pieces.push_back(pair<int, int>(i, j));
				}
			}
		}
		pawn = savePawn;
		
		for (int i = 0; i < pieces.size(); i++)
		{
			node = pieces[i];
			Board temp = bestMove(board, curpawn, stopper, false, node);
			if (temp.score > bestScore)
			{
				best = temp;
				bestScore = best.score;
			}
		}
		return best;
	}

	/*Check on the possible moves for the one node pawn, and evaluate them DONE IF THE FUNCTION WAS CALLED RECURSIVELY*/
	if (!once)
	{
		current = board;
		char savePawn = pawn;
		pawn = curpawn;
		vector<pair<int, int>> validMoves = getValidMoves(node);
		
		pawn = savePawn;
		/*ERROR CHECKING*/
		/*cout << "Stopper: " << stopper << endl;
		cout << "Node: " << get<0>(node) << get<1>(node) << endl;
		cout << "Valid moves: ";
		for (int i = 0; i < validMoves.size(); i++)
		{
			cout << get<0>(validMoves[i]) << get<1>(validMoves[i]) << " ";
		}
		cout << endl;*/
		/*END OF ERROR CHECKING*/
		
		Board best;
		Board temp = board;
		Board temp2;
		int baseline;
		if (curpawn == pieceType)
			baseline = INT_MIN;
		else
			baseline = INT_MAX;

		for (int i = 0; i < validMoves.size(); i++) {
			temp = board;
			
			/*SCORING THE MOVE*/
			if (curpawn == pieceType) {
				if (temp.getIndex(get<0>(validMoves[i]), get<1>(validMoves[i])) == nextPawn)
					temp.score += 5;
				else
					temp.score += 1;
			}
			
			else {
				if (temp.getIndex(get<0>(validMoves[i]), get<1>(validMoves[i])) == pieceType)
					temp.score -= 4;
				else
					temp.score += 0;
			}
			temp.setIndex(get<0>(node), get<1>(node), '\0');
			temp.setIndex(get<0>(validMoves[i]), get<1>(validMoves[i]), curpawn);

			// IF AI WINS
			if (get<0>(validMoves[i]) == 7 && curpawn == pawn) {
				temp.score = INT_MAX;
				return temp;
			}
			
			if (curpawn == pieceType)
			{
				/*Maximize*/
				temp2 = bestMove(temp, nextPawn, stopper - 1, false, validMoves[i]);
				if (temp2.score > baseline)
				{
					baseline = temp2.score;
					best = temp;
					best.score = baseline;
				}
			}
			else
			{
				/*Minimize*/
				pawn = nextPawn;
				vector<pair<int, int>> pieces;
				for (int j = 0; j < BOARD_SIZE; j++)
				{
					for (int k = 0; k < BOARD_SIZE; k++)
					{
						if (isValidPiece(j, k))
						{
							pieces.push_back(pair<int, int>(j, k));
						}
					}
				}
				pawn = savePawn;
				for (int j = 0; j < pieces.size(); j++)
				{
					temp2 = bestMove(temp, nextPawn, stopper - 1, false, pieces[j]);
					if (temp2.score < baseline)
					{
						baseline = temp2.score;
						best = temp;
						best.score = baseline;
					}
				}
			}
		}
		return best;
	}

}

string AIEngine::alphaMove() {
	// Create a copy of the original board
	string content = board->getContent();
	Board preBoard(content);

	// Get best move
	board->score = 0;
    Board postBoard = alphaBeta(preBoard, pieceType, 4, true, pair<int, int>(0,0), INT_MIN, INT_MAX);
	
	// Set original board to resulting board
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++)
			board->setIndex(i, j, postBoard.getIndex(i, j));
	}
	
	// Get the selected move
	return getMove(preBoard, postBoard);
}

Board AIEngine::alphaBeta(Board board, char curpawn, int stopper, bool once, pair<int, int> node, int a, int b)	//1 = max, 0 = min
{
	int alpha = a;
	int beta = b;
	char pawn = curpawn;
	Board current = board;
	/*CHECKS FOR VICTORY OR DEFEAT*/
	if (stopper <= 0)
		return board;

	if (board.checkVictory() == 1 && pawn != curpawn)
	{
		board.score -= 1000;
		return board;
	}
	if (board.checkVictory() == 1 && pawn == curpawn)
	{
		board.score += 1000;
		return board;
	}
	if (board.checkVictory() == 2 && pawn != curpawn)
	{
		board.score -= 1000;
		return board;
	}
	if (board.checkVictory() == 2 && pawn == curpawn)
	{
		board.score += 1000;
		return board;
	}

	/*GET THE NEXT PAWN*/
	char nextPawn;
	if (curpawn == 'X')
		nextPawn = 'O';
	else
		nextPawn = 'X';

	/*FIND AVAILABLE PAWNS (NODES) TO MOVE FOR THE CURRENT PAWN TYPE ONLY DONE ONCE*/
	if (once)
	{
		board.score = 0;
		int bestScore = INT_MIN;
		Board best;
		char savePawn = pawn;
		pawn = curpawn;
		vector<pair<int, int>> peices;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (isValidPiece(i, j))
				{
					peices.push_back(pair<int, int>(i, j));
				}
			}
		}
		pawn = savePawn;

		for (int i = 0; i < peices.size(); i++)
		{
			node = peices[i];
			Board temp = bestMove(board, curpawn, stopper, false, node);
			if (temp.score > bestScore)
			{
				best = temp;
				bestScore = best.score;
			}
		}
		return best;
	}

	/*Check on the possible moves for the one node pawn, and evaluate them DONE IF THE FUNCTION WAS CALLED RECURSIVELY*/
	if (!once)
	{
		current = board;
		char savePawn = pawn;
		pawn = curpawn;
		vector<pair<int, int>> validMoves = getValidMoves(node);

		pawn = savePawn;
		/*ERROR CHECKING*/
		/*cout << "Stopper: " << stopper << endl;
		cout << "Node: " << get<0>(node) << get<1>(node) << endl;
		cout << "Valid moves: ";
		for (int i = 0; i < validMoves.size(); i++)
		{
		cout << get<0>(validMoves[i]) << get<1>(validMoves[i]) << " ";
		}
		cout << endl;*/
		/*END OF ERROR CHECKING*/

		Board best;
		Board temp = board;
		Board temp2;
		int baseline;
		if (curpawn == pieceType)
		{
			baseline = INT_MIN;
		}
		else
		{
			baseline = INT_MAX;
		}
		int v;
		for (int i = 0; i < validMoves.size(); i++)
		{
			temp = board;
			/*SCORING THE MOVE*/
			if (curpawn == pieceType)
			{
				if (temp.getIndex(get<0>(validMoves[i]), get<1>(validMoves[i])) == nextPawn)
				{
					temp.score += 5;
				}
				else
				{
					temp.score += 1;
				}
			}
			else
			{
				if (temp.getIndex(get<0>(validMoves[i]), get<1>(validMoves[i])) == pieceType)
				{
					temp.score -= 4;
				}
				else
				{
					temp.score += 0;
				}
			}
			temp.setIndex(get<0>(node), get<1>(node), '\0');
			temp.setIndex(get<0>(validMoves[i]), get<1>(validMoves[i]), curpawn);

			//IF AI WINS
			if (get<0>(validMoves[i]) == 7 && curpawn == pawn)
			{
				temp.score = INT_MAX;
				return temp;
			}

			if (curpawn == pieceType)
			{
				/*Maximize*/
				temp2 = alphaBeta(temp, nextPawn, stopper - 1, false, validMoves[i], alpha, beta);
			/*	if (temp2.score > baseline)
				{
					baseline = temp2.score;
					best = temp;
					best.score = baseline;
				}*/
				if (temp2.score > baseline)
					v = temp2.score;
				else
					v = baseline;

				if (alpha < v)
					alpha = alpha;
				else
					alpha = v;

				if (beta <= alpha)
					return temp;
			}
			else
			{
				/*Minimize*/
				pawn = nextPawn;
				vector<pair<int, int>> peices;
				for (int j = 0; j < 8; j++)
				{
					for (int k = 0; k < 8; k++)
					{
						if (isValidPiece(j, k))
						{
							peices.push_back(pair<int, int>(j, k));
						}
					}
				}
				pawn = savePawn;
				for (int j = 0; j < peices.size(); j++)
				{
					temp2 = alphaBeta(temp, nextPawn, stopper - 1, false, peices[j], alpha, beta);
					/*if (temp2.score < baseline)
					{
						baseline = temp2.score;
						best = temp;
						best.score = baseline;
					}*/
					if (temp2.score < baseline)
						v = temp2.score;
					else
						v = baseline;

					if (beta > v)
						beta = beta;
					else
						beta = v;

					if (beta <= alpha)
						return temp;
				}
			}
		}
		return best;
	}
}

