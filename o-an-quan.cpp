#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // For std::setw
#include <sstream> // For std::stringstream
#include <cctype>  // For std::toupper
#include <limits>  // For std::numeric_limits
#include <numeric> // For std::accumulate (if needed)

using namespace std;

// ANSI color codes for terminal output
const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string CYAN    = "\033[36m";

const string BOLD    = "\033[1m";

// NEW: A struct to represent the state of a single pit
struct Pit {
    int smallStones = 0;
    bool hasMandarin = false;
};

// CHANGED: The board is now an array of Pit structs
Pit board[12];

int score[2] = {0, 0}; // Player 1: score[0], Player 2: score[1]

// NEW: Function to set up the board at the start of the game
void initializeBoard() {
    // Set up Player 1's small pits
    for (int i = 0; i <= 4; ++i) {
        board[i].smallStones = 5;
    }
    // Set up Player 2's small pits
    for (int i = 6; i <= 10; ++i) {
        board[i].smallStones = 5;
    }
    // Place the Mandarin stones in the Quan pits
    board[5].hasMandarin = true;
    board[11].hasMandarin = true;
}

void printBoard() {
    // Lambda to display the count of small stones in a pit
    auto displaySmallStones = [](int count) -> string {
        string color = (count > 0) ? CYAN : YELLOW;
        stringstream ss;
        ss << color << BOLD << setw(2) << count << RESET;
        return ss.str();
    };

    // Lambda to calculate the total point value of a Quan pit
    auto calcQuanValue = [](const Pit& quanPit) {
        int value = 0;
        if (quanPit.hasMandarin) {
            value += 10;
        }
        value += quanPit.smallStones;
        return value;
    };

    cout << YELLOW << R"(
       _______                       _______
      |       |    .---.-.-----.    |       |.--.--.---.-.-----.
      |   -   |    |  _  |     |    |   -  _||  |  |  _  |     |
      |_______|    |___._|__|__|    |_______||_____|___._|__|__|

    )" << RESET << endl;

    // --- Board Display ---
    cout << "\t      +------------------------+" << endl;
    cout << "\t  P2  | 11 | 10 |  9 |  8 |  7 |" << endl;
    cout << "\t -----+------------------------+-----" << endl;

    // Top row (P2): Shows the count of SMALL STONES
    cout << "\t[    ]|";
    for (int i = 10; i >= 6; i--) {
        cout << " " << displaySmallStones(board[i].smallStones) << " |";
    }
    cout << "[ " << BOLD << displaySmallStones(board[5].smallStones) << " ]" << endl;

    // Middle row: Shows the total calculated VALUE of Quan pits
    cout << "\t[ " << YELLOW << BOLD << setw(2) << calcQuanValue(board[11]) << RESET;
    cout << " ]|========================|[ "
         << YELLOW << BOLD << setw(2) << calcQuanValue(board[5]) << RESET << " ]" << endl;

    // Bottom row (P1): Shows the count of SMALL STONES
    cout << "\t[ " << BOLD << displaySmallStones(board[11].smallStones) << " ]|";
    for (int i = 0; i <= 4; i++) {
        cout << " " << displaySmallStones(board[i].smallStones) << " |";
    }
    cout << "[    ]" << endl;

    cout << "\t -----+------------------------+-----" << endl;
    cout << "\t  P1  |  1 |  2 |  3 |  4 |  5 |" << endl;
    cout << "\t      +------------------------+" << endl;
    cout << endl;

    // --- Score Display ---
    cout << BOLD << "[Score] Player 1: " << GREEN << score[0] << RESET
         << BOLD << " | Player 2: " << GREEN << score[1] << RESET << endl;
    cout << "========================================" << endl;
}

int getValidMove(int player) {
    int pitChoice;
    int board_index;
    while (true) {
        cout << BOLD << "Player " << player << ", choose a pit ("
             << (player == 1 ? "1-5" : "7-11") << "): " << RESET;

        cin >> pitChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Please enter a number." << RESET << endl;
            continue;
        }

        int minPit = (player == 1) ? 1 : 7;
        int maxPit = (player == 1) ? 5 : 11;

        if (pitChoice >= minPit && pitChoice <= maxPit) {
            board_index = pitChoice - 1; // Map 1-5 to 0-4 and 7-11 to 6-10
            // CHANGED: A move is only valid if there are small stones to pick up
            if (board[board_index].smallStones > 0) {
                return board_index;
            } else {
                cout << YELLOW << "Invalid move: Pit " << pitChoice << " has no small stones. Try again." << RESET << endl;
            }
        } else {
            cout << YELLOW << "Invalid pit number for Player " << player << ". Try again." << RESET << endl;
        }
    }
}

// Helper function to calculate the score value of a captured pit
int calculateCaptureScore(int pitIndex) {
    int scoreVal = board[pitIndex].smallStones;
    if (board[pitIndex].hasMandarin) scoreVal += 10;
    return scoreVal;
}

bool makeMove(int startPit, int player, int direction) {
    int stones = board[startPit].smallStones;
    board[startPit].smallStones = 0; // pick up
    int currentPit = startPit;

    cout << "Player " << player << " picks up " << stones 
         << " stones from pit " << startPit + 1 
         << " going " << (direction == 1 ? "Right" : "Left") << ".\n";

    auto nextIndex = [&](int idx) { return (idx + direction + 12) % 12; };

    // initial sow
    while (stones > 0) {
        currentPit = nextIndex(currentPit);
        board[currentPit].smallStones++; // sow: always add a small stone, even into Quan
        stones--;
    }

    cout << "--> Last stone landed in pit " << currentPit + 1 << ".\n";

        // --- add debug info right at loop start ---
        while (true) {
            int nextPit  = nextIndex(currentPit); // immediate next pit
            int afterNext = nextIndex(nextPit);    // the pit after that
    
            // DEBUG: print current, next, afterNext and their small stones + hasMandarin
            // cerr << "[DBG] current=" << currentPit+1
            //      << " next=" << nextPit+1 << "(s=" << board[nextPit].smallStones
            //      << ",m=" << board[nextPit].hasMandarin << ")"
            //      << " afterNext=" << afterNext+1 << "(s=" << board[afterNext].smallStones
            //      << ",m=" << board[afterNext].hasMandarin << ")\n";
    
            // CASE 1: if next pit is Quan -> turn ends (Quan is not removed by passing)
            if (nextPit == 5 || nextPit == 11) {
                cout << "--> Next pit is a Quan. Turn ends.\n";
                break;
            }
    
            // CASE 2: next two pits empty and afterNext has no mandarin -> turn ends
            if (board[nextPit].smallStones == 0 &&
                board[afterNext].smallStones == 0 &&
                !board[afterNext].hasMandarin) 
            {
                cout << "--> Next two pits are empty. Turn ends.\n";
                break;
            }
    
            // CASE 3: capture: next pit empty but afterNext has pieces (or mandarin)
            if (board[nextPit].smallStones == 0) {
                if (board[afterNext].smallStones > 0 || board[afterNext].hasMandarin) {
                    int capturedScore = calculateCaptureScore(afterNext);
                    cout << GREEN << "--> Capture from pit " << afterNext + 1 
                         << " worth " << capturedScore << " points!\n" << RESET;
                    score[player - 1] += capturedScore;
    
                    // SAFEGUARD: only REMOVE mandarin if afterNext actually had a mandarin
                    if (board[afterNext].hasMandarin) {
                        cerr << "[DBG] Mandarin at pit " << afterNext + 1 << " is being captured and removed.\n";
                        board[afterNext].hasMandarin = false;
                    }
                    board[afterNext].smallStones = 0;
                } else {
                    cout << "--> Nothing to capture. Turn ends.\n";
                }
                break;
            }
    

        // CASE 4: continue (rái quân) — the next pit has small stones, pick them up
        cout << "--> Continuing by picking up " << board[nextPit].smallStones 
             << " stones from pit " << nextPit + 1 << ".\n";

        stones = board[nextPit].smallStones;
        board[nextPit].smallStones = 0;
        currentPit = nextPit;

        // sow the picked stones and loop back
        while (stones > 0) {
            currentPit = nextIndex(currentPit);
            board[currentPit].smallStones++;
            stones--;
        }

        cout << "--> Last stone landed in pit " << currentPit + 1 << ".\n";
        // now loop to re-evaluate nextPit/afterNext for this new currentPit
    }

    return false;
}

bool isGameOver() {
    bool noMandarin = (!board[5].hasMandarin && !board[11].hasMandarin);

    bool noStones = true;
    for (int i = 0; i < 12; i++) {
        if (board[i].smallStones > 0) {
            noStones = false;
            break;
        }
    }
    return noMandarin || noStones;
}


void collectRemainStones() {
    cout << "Collecting remaining small stones..." << endl;
    // At the end of the game, any remaining small stones on the board are forfeited.
    // In many rule variants, they are not added to any player's score.
    // If you want to add them to the player on whose side they are, you can add that logic here.
    // For now, we just clear them.
    for(int i = 0; i < 12; ++i) {
        board[i].smallStones = 0;
    }
    cout << "All remaining small stones have been cleared from the board." << endl;
}

int main() {
    initializeBoard(); // NEW: Call the function to set up the board
    int currentPlayer = 1;

    while (true) {
        printBoard();

        if (isGameOver()) {
            cout << BOLD << "Game Over!" << RESET << endl;
            collectRemainStones();

            cout << "\nFinal Board State:" << endl;
            printBoard();

            cout << BOLD << "Final Score -> Player 1: " << score[0] << " | Player 2: " << score[1] << RESET << endl;
            if (score[0] > score[1]) {
                cout << BOLD << GREEN << "Player 1 wins!" << RESET << endl;
            } else if (score[1] > score[0]) {
                cout << BOLD << GREEN << "Player 2 wins!" << RESET << endl;
            } else {
                cout << BOLD << YELLOW << "It's a tie!" << RESET << endl;
            }
            break;
        }

        bool canMove = false;
        int startRange = (currentPlayer == 1) ? 0 : 6;
        int endRange = (currentPlayer == 1) ? 4 : 10;
        for (int i = startRange; i <= endRange; i++) {
            // CHANGED: Check for smallStones to see if a move is possible
            if (board[i].smallStones > 0) {
                canMove = true;
                break;
            }
        }

        if (!canMove) {
            cout << YELLOW << "Player " << currentPlayer << " has no valid moves! Forfeiting remaining stones." << RESET << endl;
            // A player with no moves must "feed" their stones to the other player
            int forfeitedStones = 0;
            for(int i = startRange; i <= endRange; ++i) {
                forfeitedStones += board[i].smallStones;
                board[i].smallStones = 0;
            }
            if (forfeitedStones > 0) {
                score[((currentPlayer % 2))] += forfeitedStones;
                cout << "Player " << ((currentPlayer % 2) + 1) << " receives " << forfeitedStones << " forfeited stones." << endl;
            }
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            continue;
        }

        int pitIdx = getValidMove(currentPlayer);
        
        int direction;
        char directionChoice;
        while (true) {
            cout << BOLD << "Choose direction from pit " << pitIdx + 1 << " (L: Left | R: Right): " << RESET;
            cin >> directionChoice;
            directionChoice = toupper(directionChoice);

            if (directionChoice == 'L' || directionChoice == 'R') {
                direction = (directionChoice == 'R') ? 1 : -1;
                break;
            } else {
                cout << RED << "Invalid choice! Please choose 'L' or 'R'." << RESET << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        makeMove(pitIdx, currentPlayer, direction);
        
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        cout << "\n----------------------------------------\n" << endl;
    }

    return 0;
}
