#include <iostream>

/*
GAME RULE
	 _____________________________
	[	 |	 |   |   |   |   |    ]
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
	cout << "\t     |  1 |  2 |  3 |  4 |  5 |     \n";
    cout << "\t __________________________________\n";
	cout << "\t[    |    |    |    |    |    |    ]\n";

    // Top row: index 10 to 6
    cout << "\t[    |";
    for (int i = 10; i >= 6; i--) {
        cout << "  " << board[i] << " |";
    }
    cout << "    ]\n";

    // Quan row: pits 11 and 5 (both should be shown as value 10)
    cout << "\t[ " << board[11] * 10 << " |========================| " << board[5] * 10 << " ]\n";

    // Bottom row: index 0 to 4
    cout << "\t[    |";
    for (int i = 0; i <= 4; i++) {
        cout << "  " << board[i] << " |";
    }
    cout << "    ]\n";
    
    cout << "\t[____|____|____|____|____|____|____]\n";
    cout << "\t     | 10 |  9 |  8 |  7 |  6 |     \n";

    // Show scores
    cout << "Score - Player 1: " << score[0]
         << " | Player 2: " << score[1] << "\n";
    cout << "========================================\n";
}



int getValidMove(int player) {
	int pit;
	while (true) {
		cout << "Player " << player << ", choose a pit (" 
     		 << (player == 1 ? "0-5" : "6-11") << "): ";

		cin >> pit;
		
		if (player == 1 && pit >= 0 && pit <= 5 && board[pit] > 0) 
			return pit;
		if (player == 2 && pit >= 6 && pit <= 11 && board[pit] > 0) 
			return pit;
			
		cout << "Invalid pit. Try again." << endl;
	}
}


void makeMove(int pit, int player) {
    int stones = board[pit];             // number of stones to move
    int stoneValue = valuePerStone[pit]; // value per stone
    board[pit] = 0;

    int i = pit;
    while (stones > 0) {
        i = (i + 1) % 12;
        board[i]++; // Drop 1 stone
        stones--;
    }

    // Capture logic (if last stone lands in empty pit on your side)
    bool isPlayer1 = (player == 1);
    if (board[i] == 1 && valuePerStone[i] == 1) { // Only capture from regular pits
        if ((isPlayer1 && i >= 0 && i <= 4) || (!isPlayer1 && i >= 6 && i <= 10)) {
            int opposite = 11 - i;
            if (board[opposite] > 0 && valuePerStone[opposite] == 1) {
                int captured = (board[opposite] + board[i]) * valuePerStone[i];
                score[player - 1] += captured;
                board[opposite] = 0;
                board[i] = 0;
                cout << "Player " << player << " captures " << captured << " points!\n";
            }
        }
    }
}

int main() {
	int currentPlayer = 1;
	
    while (true) {
        printBoard(board, score);

        switch (currentPlayer) {
            case 1: {
                int pit = getValidMove(1);
                makeMove(pit, 1);
                currentPlayer = 2;
                break;
            }
            case 2: {
                int pit = getValidMove(2);
                makeMove(pit, 2);
                currentPlayer = 1;
                break;
            }
        }
    }

	return 0;
}