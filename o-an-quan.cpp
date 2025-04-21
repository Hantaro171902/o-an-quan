#include <iostream>

/*
GAME RULE
	 _____________________________
	[    |   |   |   |   |   |    ]
	[    | 5 | 5 | 5 | 5 | 5 |    ] 
	[ 10 |===================| 10 ]
	[    | 5 | 5 | 5 | 5 | 5 |    ] 
	[____|___|___|___|___|___|____]
*/

using namespace std;

int board[12] = {5, 5, 5, 5, 5, 1,
				 5, 5, 5, 5, 5, 1};
				 
int valuePerStone[12] = {1, 1, 1, 1, 1, 10, 
						 1, 1, 1, 1, 1, 10};

int score[2] = {0, 0}; 	// Player 1: score[0], Player 2: score[1]

void printBoard(int board[12], int score[2]) {
	cout << "\t     | 10 |  9 |  8 |  7 |  6 |     \n";
    cout << "\t __________________________________\n";
	cout << "\t[    |    |    |    |    |    |    ]\n";

    // Top row: index 10 to 6
    cout << "\t[    |";
    for (int i = 10; i >= 6; i--) {
        cout << "  " << board[i] << " |";
    }
    cout << "    ]\n";

    // Quan row: pits 11 and 5 (both should be shown as value 10)
//    cout << "\t[ " << board[11] * valuePerStone[11] 
//     	 << " |========================| " 
//     	 << board[5] * valuePerStone[5] << " ]\n";

	auto calcQuanValue = [](int count) {
	    if (count == 0) return 0;
	    return 10 + (count - 1); // 10 for Quan, rest are 1-point stones
	};
	
	cout << "\t[ " << calcQuanValue(board[11]) 
	     << " |========================| " 
	     << calcQuanValue(board[5]) << " ]\n";
	

    // Bottom row: index 0 to 4
    cout << "\t[    |";
    for (int i = 0; i <= 4; i++) {
        cout << "  " << board[i] << " |";
    }
    cout << "    ]\n";
    
    cout << "\t[____|____|____|____|____|____|____]\n";
    cout << "\n";
    cout << "\t     |  1 |  2 |  3 |  4 |  5 |     \n";
    cout << "\n";

    // Show scores
    cout << "Score - Player 1: " << score[0]
         << " | Player 2: " << score[1] << "\n";
    cout << "========================================\n";
}



int getValidMove(int player) {
	int pit;
	while (true) {
		cout << "Player " << player << ", choose a pit (" 
     		 << (player == 1 ? "1-5" : "6-10") << "): ";

		cin >> pit;
		
		if (player == 1 && pit > 0 && pit <= 5 && board[pit] > 0) 
			return pit-1;
		if (player == 2 && pit > 6 && pit <= 11 && board[pit] > 0) 
			return pit-1;
			
		cout << "Invalid pit. Try again." << endl;
	}
}


bool makeMove(int pit, int player) {
    int stones = board[pit];
    board[pit] = 0;
    int i = pit;

    while (stones > 0) {
        i = (i + 1) % 12;
        
        // Skip opponent's big pit
        if ((player == 1 && i == 11) || (player == 2 && i == 5)) {
        	continue;
		}
        	
        board[i]++;
        stones--;
    }
    
    
    
    // Check if landed in your own stone
    if ((player == 1 && i == 5) || (player == 2 && i == 11)) {
    	return true;	// Get another turn
	}

    // Capture rule
    bool isPlayer1 = (player == 1);
    if (board[i] == 1) {
        if ((isPlayer1 && i >= 0 && i <= 4) || (!isPlayer1 && i >= 6 && i <= 10)) {
            int opposite = 11 - i;
            if (board[opposite] > 0) {
                // Determine score from opposite pit:
                int stoneValue = (opposite == 5 || opposite == 11) ? 10 : 1;
                int capturedStones = board[opposite];
                int totalScore = (capturedStones + 1) * stoneValue; // +1 for current pit

                score[player - 1] += totalScore;

                board[opposite] = 0;
                board[i] = 0;

                cout << "Player " << player << " captures from pit " << opposite
                     << " for " << totalScore << " points!\n";
            }
        }
    }
    
    return false;	// No extra turn
}

bool isGameOver() {
    bool player1Empty = true;
    bool player2Empty = true;

    for (int i = 0; i <= 4; ++i) {
        if (board[i] != 0) player1Empty = false;
    }
    for (int i = 6; i <= 10; ++i) {
        if (board[i] != 0) player2Empty = false;
    }

    return player1Empty || player2Empty;
}

void collectRemainStones() {
    for (int i = 0; i <= 4; ++i) {
        score[0] += board[i];
        board[i] = 0;
    }
    for (int i = 6; i <= 10; ++i) {
        score[1] += board[i];
        board[i] = 0;
    }
}

int main() {
	int currentPlayer = 1;
	
    while (true) {
        printBoard(board, score);

        switch (currentPlayer) {
            case 1: {
                int pit = getValidMove(1);
                bool extraTurn = makeMove(pit, 1);
                
                if (!extraTurn) {
                	cout << "Player 2 gets another turn!" << endl;  
				}
				else {
					currentPlayer = 1;
				}
				
                break;
            }
            case 2: {
            	int pit = getValidMove(2);
            	bool extraTurn = makeMove(pit, 2);
                
                if (!extraTurn) {
                	cout << "Player 2 gets another turn!" << endl;  
				}
				else {
					currentPlayer = 1;
				}
            
                break;
            }
        }
        
        
        if (isGameOver()) {
            cout << "Game over!\n";
            collectRemainStones();
            printBoard(board, score);
            if (score[0] > score[1]) {
                cout << "Player 1 wins!\n";
            } else if (score[1] > score[0]) {
                cout << "Player 2 wins!\n";
            } else {
                cout << "It's a tie!\n";
            }
            break;
        }
    }

	return 0;
}