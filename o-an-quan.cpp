#include <iostream>
#include <vector>
#include <numeric> // For std::accumulate if needed later
#include <limits>
#include <cctype>
#include <iomanip>


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
	cout << "\t     | 11  | 10  |  9  |  8  |  7  |     \n";
    cout << "\t ________________________________________\n";
	cout << "\t[    |     |     |     |     |     |    ]\n";

    // Top row: index 10 to 6
    cout << "\t[    |";
    for (int i = 10; i >= 6; i--) {
        cout << "  " << board[i] << "  |";
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
	     << " |=============================| " 
	     << calcQuanValue(board[5]) << " ]\n";
	

    // Bottom row: index 0 to 4
    cout << "\t[    |";
    for (int i = 0; i <= 4; i++) {
        cout << "  " << board[i] << "  |";
    }
    cout << "    ]\n";
    
    cout << "\t[____|_____|_____|_____|_____|_____|____]\n";
    cout << "\n";
    cout << "\t     |  1  |  2  |  3  |  4  |  5  |     \n";
    cout << "\n";

    // Show scores
    cout << "Score - Player 1: " << score[0]
         << " | Player 2: " << score[1] << "\n";
    cout << "========================================\n";
}



int getValidMove(int player) {
	int pitChoice;
	int board_index;
	while (true) {
		cout << "Player " << player << ", choose a pit (" 
     		 << (player == 1 ? "1-5" : "7-11") << "): ";

		cin >> pitChoice;
		
//		if (player == 1 && pit > 0 && pit <= 5 && board[pit] > 0) 
//			return pit - 1;
//		if (player == 2 && pit > 6 && pit <= 11 && board[pit] > 0) 
//			return pit - 1;

		if (player == 1) {
            if (pitChoice >= 1 && pitChoice <= 5) {
                board_index = pitChoice - 1; // Convert 1-5 to 0-4
                if (board[board_index] > 0) {
                    return board_index;
                } else {
                    cout << "Invalid move: Pit " << pitChoice << " is empty. Try again." << endl;
                }
            } else {
                cout << "Invalid pit number for Player 1. Try again." << endl;
            }
        } else { // Player 2
            if (pitChoice >= 6 && pitChoice <= 10) {
                board_index = pitChoice - 1; 
                // The board layout is P1: 0-4, P1 Quan: 5, P2: 6-10, P2 Quan: 11
                // So player 2 input 6-10 corresponds to indices 6-10.
                if (board[board_index] > 0) {
                    return board_index;
                } else {
                    cout << "Invalid move: Pit " << pitChoice << " is empty. Try again." << endl;
                }
            } else {
                cout << "Invalid pit number for Player 2. Try again." << endl;
            }
        }
			
		// Clear input buffer if bad input occurred
        if (cin.fail()) {
        	cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input type. Please enter a number." << endl;
        }
	}
}


bool makeMove(int startPit, int player, int direction) {
	int currentPitIdx = startPit;
    int stonesToSow = board[currentPitIdx];
    
    if (stonesToSow == 0) {
        cout << "Error: Tried to move from an empty pit." << endl;
        return false; // Should not happen if getValidMove works correctly
    }
    
    cout << "Player " << player << " picks up " << stonesToSow << " stones from pit " << startPit +1 
		 << "in direction " << (direction == 1 ? "Right" : "Left") << "." << endl;
    board[currentPitIdx] = 0; // Pick up stones
    
    auto calculateNextIndex = [&](int currentIndex) {
    	return (currentIndex + direction + 12) % 12;
	};

    while (stonesToSow > 0) {
        while (stonesToSow > 0) {
            currentPitIdx = (currentPitIdx + 1) % 12; // Move to the next pit

            // Skip opponent's Quan
            if ((player == 1 && currentPitIdx == 11) || (player == 2 && currentPitIdx == 5)) {
                
                continue; // Skip sowing in opponent's Quan
            }

            board[currentPitIdx]++;
            stonesToSow--;
            // cout << "Sowed 1 stone in pit " << currentPitIdx + 1 << ". Remaining: " << stonesToSow << endl; 
        }
        
        int nextPitIndex = (currentPitIdx + 1) % 12;
        
        if (nextPitIndex != 5 && nextPitIndex != 11 && board[nextPitIndex] > 0) {
            cout << "--> Next pit (" << nextPitIndex + 1 << ") has " << board[nextPitIndex] << " stones. Picking them up to continue!" << endl;
            stonesToSow = board[nextPitIndex]; // Get stones for the next round of sowing
            board[nextPitIndex] = 0;          // Empty the pit
            currentPitIdx = nextPitIndex;   // Start sowing from the pit after this one
        } else {
            break;
        }
        
//        if ((player == 1 && currentPitIdx == 5) || (player == 2 && currentPitIdx == 11)) {
//        	cout << "Landed in own Quan pit " << currentPitIdx + 1 << ". Turn might continue based on specific rules (not implemented here), standard Mancala gives extra turn." << endl;
//		}
	}
    
    int pitToCheckForCapture = (currentPitIdx + 1) % 12;
    int pitAfterCaptureCheck = (pitToCheckForCapture + 1) % 12;

    if (pitToCheckForCapture != 5 && pitToCheckForCapture != 11 && board[pitToCheckForCapture] > 0 &&
        board[pitAfterCaptureCheck] == 0)
    {
        int capturedStones = board[pitToCheckForCapture];
      
        // Basic Capture: Capture only the stones in 'pitToCheckForCapture'
        cout << "Player " << player << " captures " << capturedStones << " stones from pit " << pitToCheckForCapture + 1 << " (followed by empty pit " << pitAfterCaptureCheck + 1 << ")" << endl;
        score[player - 1] += capturedStones;
        board[pitToCheckForCapture] = 0; // Remove captured stones

    }

    
/*    // Check if landed in your own stone
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
*/
    return false;	// No extra turn
}

bool isGameOver() {
    bool player1Empty = true;
    bool player2Empty = true;

	if (board[5] == 0 && board[11] == 0) {
        cout << "Game Over: Both Quan pits are empty." << endl;
        return true;
    }
    
    // Another end condition: All *small* pits are empty (stones are only in Quans)
    bool allSmallPitsEmpty = true;
    for(int i=0; i<=4; ++i) if(board[i] != 0) allSmallPitsEmpty = false;
    for(int i=6; i<=10; ++i) if(board[i] != 0) allSmallPitsEmpty = false;

    if (allSmallPitsEmpty) {
        cout << "Game Over: All small pits are empty." << endl;
        return true;
    }

//    for (int i = 0; i <= 4; ++i) {
//        if (board[i] != 0) player1Empty = false;
//    }
//    for (int i = 6; i <= 10; ++i) {
//        if (board[i] != 0) player2Empty = false;
//    }
//
//    return player1Empty || player2Empty;
	return false;
}

void collectRemainStones() {
	cout << "Collecting remaining small pit stones..." << endl;
    int p1_collected = 0;
    int p2_collected = 0;
	
	// Player 1's remaining small pit stones go to Player 1
    for (int i = 0; i <= 4; ++i) {
        score[0] += board[i];
        board[i] = 0;
    }
    if (p1_collected > 0) cout << "Player 1 collects " << p1_collected << " remaining stones." << endl;
    score[0] += p1_collected;
    
    
    // Player 2's remaining small pit stones go to Player 2
    for (int i = 6; i <= 10; ++i) {
        p2_collected += board[i];
        board[i] = 0;
    }
    if (p2_collected > 0) cout << "Player 2 collects " << p2_collected << " remaining stones." << endl;
    score[1] += p2_collected;
}

int main() {
	int currentPlayer = 1;
	
    while (true) {
        printBoard(board, score);
        
        if (isGameOver()) {
            cout << "Game over!\n";
            collectRemainStones(); // Collect small pit stones
           
            cout << "Final Board State (before calculating winner):" << endl;
            printBoard(board, score);

            // Determine winner based on final score
            cout << "Final Score -> Player 1: " << score[0] << " | Player 2: " << score[1] << endl;
            if (score[0] > score[1]) {
                cout << "Player 1 wins!\n";
            } else if (score[1] > score[0]) {
                cout << "Player 2 wins!\n";
            } else {
                cout << "It's a tie!\n";
            }
            break; // Exit game loop
        }
        
        // Check if current player has any valid moves
        bool canMove = false;
        int startPit = (currentPlayer == 1) ? 0 : 6;
        int endPit = (currentPlayer == 1) ? 4 : 10;
        for (int i = startPit; i <= endPit; ++i) {
            if (board[i] > 0) {
                canMove = true;
                break;
            }
        }

        if (!canMove) {
            cout << "Player " << currentPlayer << " has no valid moves! Skipping turn." << endl;
             // In O An Quan, if a player has no stones, they might borrow points/stones - complex rule.
             // Simple approach: skip turn. If both have no moves -> game over (covered by isGameOver).
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            continue; // Skip to next player's turn
        }
        
        int pitIndex = getValidMove(currentPlayer);	// Take place player's choice
        
        // --- Player's move direction ---
        int direction;
        char directionChoice;
        while (true) {
        	// printBoard(board, score)
        	cout << "Choose direction to sow " << pitIndex + 1 << " (L: Left | R: Right): ";
        	cin >> directionChoice;
        	
        	if (directionChoice == 'L') {
        		direction = -1;
        		break;
			} else if (directionChoice == 'R') {
				direction = 1;
				break;
			} else {
				cout << "Invalid choice ! Please choose 'L' or 'R'. " << endl;
				// clear cached when chose wrong
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}        
        
        
        bool extraTurn = makeMove(pitIndex, currentPlayer, direction); // makeMove now handles the continuation internally

		if (!extraTurn) {
            
             currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch player
        } else {
            cout << "Player " << currentPlayer << " gets another turn!" << endl;
            // Keep currentPlayer the same
        }
     
/*        switch (currentPlayer) {
		    case 1: {
		        int pit = getValidMove(1);
		        bool extraTurn = makeMove(pit, 1);
		
		        if (!extraTurn) {
		            currentPlayer = 2; // ? Switch to Player 2
		        } else {
		            cout << "Player 1 gets another turn!" << endl;
		        }
		        break;
		    }
		    case 2: {
		        int pit = getValidMove(2);
		        bool extraTurn = makeMove(pit, 2);
		
		        if (!extraTurn) {
		            currentPlayer = 1; // ? Switch to Player 1
		        } else {
		            cout << "Player 2 gets another turn!" << endl;
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
*/	
	}

	return 0;
}