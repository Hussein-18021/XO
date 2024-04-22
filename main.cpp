#include <iostream>
#include <vector>
#include <random> // For random number generation

// Constants for board size
const int BOARD_SIZE = 3;
const int MAX_DEPTH = 7; // Increased depth limit

// Function prototypes
void displayBoard(const std::vector<std::vector<char>>& board);
bool isBoardFull(const std::vector<std::vector<char>>& board);
char checkWinner(const std::vector<std::vector<char>>& board);
void findBestMove(std::vector<std::vector<char>>& board);
int minimax(std::vector<std::vector<char>>& board, int depth, int alpha, int beta, bool isMaximizingPlayer); // Added alpha-beta pruning parameters
int evaluateBoard(const std::vector<std::vector<char>>& board); // Add prototype for evaluateBoard
void handlePlayerMove(std::vector<std::vector<char>>& board, char currentPlayer);
void twoPlayersMode();
void playerVsAIMode();


// Function to display the Tic-Tac-Toe board
void displayBoard(const std::vector<std::vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// Function to check if the board is full
bool isBoardFull(const std::vector<std::vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

// Function to check if there's a winner
char checkWinner(const std::vector<std::vector<char>>& board) {
    // Check rows, columns, and diagonals for wins
    for (int i = 0; i < BOARD_SIZE; ++i) {
        // Check rows
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return board[i][0];

        // Check columns
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return board[0][i];
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return board[0][0];

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
        return board[0][2];

    // No winner
    return ' ';
}

// Function to find the best move for AI
void findBestMove(std::vector<std::vector<char>>& board) {
    int bestScore = -1000;
    int bestRow = -1;
    int bestCol = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = 'X';
                int score = minimax(board, MAX_DEPTH, -1000, 1000, false); // Depth limit: MAX_DEPTH, Added alpha-beta pruning parameters
                board[i][j] = ' ';

                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    board[bestRow][bestCol] = 'X'; // Make the best move
}

// Minimax algorithm with depth limit
int minimax(std::vector<std::vector<char>>& board, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || checkWinner(board) != ' ') {
        return evaluateBoard(board);
    }

    if (isMaximizingPlayer) {
        int maxScore = -1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    int score = minimax(board, depth - 1, alpha, beta, false);
                    board[i][j] = ' ';
                    maxScore = std::max(maxScore, score);
                    alpha = std::max(alpha, score);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return maxScore;
    } else {
        int minScore = 1000;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    int score = minimax(board, depth - 1, alpha, beta, true);
                    board[i][j] = ' ';
                    minScore = std::min(minScore, score);
                    beta = std::min(beta, score);
                    if (beta <= alpha)
                        break;
                }
            }
        }
        return minScore;
    }
}

// Evaluation function
int evaluateBoard(const std::vector<std::vector<char>>& board) {
    int score = 0;

    // Check rows, columns, and diagonals for potential wins or threats
    for (int i = 0; i < BOARD_SIZE; ++i) {
        // Rows
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'X')
                score += 100; // Favorable position for AI
            else if (board[i][0] == 'O')
                score -= 100; // Unfavorable position for opponent
        }
        // Columns
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'X')
                score += 100; // Favorable position for AI
            else if (board[0][i] == 'O')
                score -= 100; // Unfavorable position for opponent
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X')
            score += 100; // Favorable position for AI
        else if (board[0][0] == 'O')
            score -= 100; // Unfavorable position for opponent
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X')
            score += 100; // Favorable position for AI
        else if (board[0][2] == 'O')
            score -= 100; // Unfavorable position for opponent
    }

    // Check for potential wins or threats in rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        // Rows
        if (board[i][0] == 'X' || board[i][2] == 'X')
            score += 10; // Favorable position for AI
        else if (board[i][0] == 'O' || board[i][2] == 'O')
            score -= 10; // Unfavorable position for opponent

        // Columns
        if (board[0][i] == 'X' || board[2][i] == 'X')
            score += 10; // Favorable position for AI
        else if (board[0][i] == 'O' || board[2][i] == 'O')
            score -= 10; // Unfavorable position for opponent
    }

    // Check for potential wins or threats in diagonals
    if (board[0][0] == 'X' || board[2][2] == 'X')
        score += 10; // Favorable position for AI
    else if (board[0][0] == 'O' || board[2][2] == 'O')
        score -= 10; // Unfavorable position for opponent

    if (board[0][2] == 'X' || board[2][0] == 'X')
        score += 10; // Favorable position for AI
    else if (board[0][2] == 'O' || board[2][0] == 'O')
        score -= 10; // Unfavorable position for opponent

    // Add more evaluation criteria as needed

    return score;
}

void handlePlayerMove(std::vector<std::vector<char>>& board, char currentPlayer) {
    int row, col;
    std::cout << "Player " << currentPlayer << ", enter row (0-" << BOARD_SIZE - 1 << ") and column (0-" << BOARD_SIZE - 1 << ") for your move: ";
    std::cin >> row >> col;

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ') {
        std::cout << "Invalid move! Try again." << std::endl;
        handlePlayerMove(board, currentPlayer); // Recurse to get a valid move
    } else {
        board[row][col] = currentPlayer; // Place player's symbol
    }
}

void twoPlayersMode() {
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' '));

    char currentPlayer = 'X'; // Player 1 starts

    // Game loop
    while (!isBoardFull(board)) {
        displayBoard(board);
        handlePlayerMove(board, currentPlayer);

        if (checkWinner(board) == 'X') {
            displayBoard(board);
            std::cout << "Player 1 wins!" << std::endl;
            break; // Exit the loop if player 1 wins
        }

        if (checkWinner(board) == 'O') {
            displayBoard(board);
            std::cout << "Player 2 wins!" << std::endl;
            break; // Exit the loop if player 2 wins
        }

        if (isBoardFull(board)) {
            displayBoard(board);
            std::cout << "It's a tie!" << std::endl;
            break; // Exit the loop if the board is full
        }

        // Switch player
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

// Function for player vs AI mode
// Function for player vs AI mode
void playerVsAIMode() {
    std::vector<std::vector<char>> board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' '));

    // Game loop
    while (!isBoardFull(board)) {
        displayBoard(board);
        handlePlayerMove(board, 'O'); // Player's move
        if (checkWinner(board) == 'O') {
            displayBoard(board);
            std::cout << "You win!" << std::endl;
            break; // Exit the loop if the player wins
        }

        if (isBoardFull(board)) {
            std::cout << "It's a tie!" << std::endl;
            break; // Exit the loop if the board is full
        }

        findBestMove(board); // AI's move

        if (checkWinner(board) == 'X') {
            displayBoard(board);
            std::cout << "AI wins!" << std::endl;
            break; // Exit the loop if the AI wins
        }
    }
}


// Main menu function
void mainMenu() {
    int choice;
    std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
    std::cout << "Choose a mode:" << std::endl;
    std::cout << "1. Two Players Mode" << std::endl;
    std::cout << "2. Player vs AI Mode" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            twoPlayersMode();
            break;
        case 2:
            playerVsAIMode();
            break;
        default:
            std::cout << "Invalid choice. Please choose again." << std::endl;
            mainMenu();
            break;
    }
}

// Main function
int main() {
     mainMenu();
    return 0;
}
