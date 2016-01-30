#include <string>
#include <iostream>

#include "Board.H"

using namespace std; 

/*--------------------
	   A B C D E F G H
	8 |X|X|X|X|X|X|X|X|
	7 |X|X|X|X|X|X|X|X|
	6 |_|_|_|_|_|_|_|_|
	5 |_|_|_|_|_|_|_|_|
	4 |_|_|_|_|_|_|_|_|
	3 |_|_|_|_|_|_|_|_|
	2 |O|O|O|O|O|O|O|O|
	1 |O|O|O|O|O|O|O|O|
/*--------------------*/

Board::Board() {
    //j is column indicator, i is row indicator
	vector<char> row;
	for(int i = 0; i < BOARD_SIZE; i++){
		row.push_back('\0');
	}
	for(int i = 0; i < BOARD_SIZE; i++){
		board.push_back(row);
	}
    
	for (int i = 0; i < BOARD_SIZE; i++) {
		
		if (i < 2) {
			for(int j = 0; j < BOARD_SIZE; j++)
            {
				board[i][j] = 'X';
            }
			
		} else if (i >= 2 && i < 6) {
			for(int j = 0; j < BOARD_SIZE; j++)
				board[i][j] = '\0';
		} else {
			for(int j = 0; j < BOARD_SIZE; j++)
				board[i][j] = 'O';
		}
	}
}

Board::Board(string content) {
	int k = 0;
	
	// Initialize vector
	vector<char> row;
	for(int i = 0; i < BOARD_SIZE; i++){
		row.push_back('\0');
	}
	for(int i = 0; i < BOARD_SIZE; i++){
		board.push_back(row);
	}
	
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) {
			char piece;
			if(content[k] == '1') piece = 'X';
			else if(content[k] == '2') piece = 'O';
			else piece = '\0';
			
			board[i][j] = piece;
			k++;
		}
	}
}

Board::~Board() { }

void Board::draw() {
	//print out top indices
    cout << "   A B C D E F G H" << endl; 
	
	for(int j = 0; j < BOARD_SIZE; j++) {
		//start with top row
		cout << j << " ";
		
		for(int i = 0; i < BOARD_SIZE; i++) { 
			//have each space print the line for the space before it
			cout << "|";
			if(board[j][i] == 'X') cout << "X";
			else if(board [j][i] == 'O') cout << "O";
			else cout << "_";
		}
		
		cout << "|" << endl;
	}
}

bool Board::isEmpty(int x, int y) {
	return board[y][x] == '\0';
}

string Board::getContent() {
	string content;
	
	for(int i = 0; i < BOARD_SIZE; i++) {
		for(int j = 0; j < BOARD_SIZE; j++) { 
			char c = board[i][j];
			
			if (c == 'X') content += '1';
			else if (c == 'O') content += '2';
			else content += '3';
		}
	}
	
	content += '\n';
	return content;
}

string Board::getFullContent() {
	string content;
	
	content = "   A B C D E F G H\n"; 
	
	for(int j = 0; j < BOARD_SIZE; j++) {
		//start with top row
		content += to_string(j) + " ";
		
		for(int i = 0; i < BOARD_SIZE; i++) { 
			//have each space print the line for the space before it
			content += "|";
			if(board[j][i] == 'X') content += "X";
			else if(board [j][i] == 'O') content += "O";
			else content += "_";
		}
		
		content += "|\n";
	}
	
	return content;
}

char Board::getIndex(int x, int y) {
	return board[y][x];
}

void Board::setIndex(int x, int y, char c){
	board[y][x] = c;
}

int Board::checkVictory()
{
	//Check if either player's pieces are all captured
	int Os = 0;
	int Xs = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == 'O')
			{
				++Os;
			}
			else if (board[i][j] == 'X')
			{
				++Xs;
			}
		}
	}
	if (Os == 0)
		{return 1;}	//X Wins
	else if (Xs == 0)
		{return 2;}	//O Wins
	
	
    //Check for X victory by iterating through the bottom row 
    for(int i=0; i<BOARD_SIZE; i++)
    {
        if(board[7][i]=='X')
        {
            return 1; //X wins!
        }
    }
    //Check for O victory by iterating through the top row
    for(int i=0; i<BOARD_SIZE; i++)
    {
        if(board[0][i]=='O')
        {
            return 2; //O wins!
        }
    }
	
	return 0; //No one wins!
}

Board& Board::operator=(const Board &b) {
	board = b.board;
	score = b.score;
}

/* PUBLIC HELPER FUNCTIONS */

int getColumnNum(char columnLetter) {
	char c = columnLetter;
	
	if (c == 'a')	
		return 0;
	else if (c == 'b')
		return 1;
	else if (c == 'c')
		return 2;
	else if (c == 'd')
		return 3;
	else if (c == 'e')
		return 4;
	else if (c == 'f')
		return 5;
	else if (c == 'g')
		return 6;
	else if (c == 'h')
		return 7;
	else
		return -1;
}

char getColumnLetter(int columnNum) {
	int c = columnNum;
	
	if (c == 0)	
		return 'a';
	else if (c == 1)
		return 'b';
	else if (c == 2)
		return 'c';
	else if (c == 3)
		return 'd';
	else if (c == 4)
		return 'e';
	else if (c == 5)
		return 'f';
	else if (c == 6)
		return 'g';
	else if (c == 7)
		return 'h';
	else
		return '\0';
}
