#include <algorithm>
#include <iostream>
#include <Minimax.h>
#include <string>

#include "include/ChessUtils.h"

#define WHITE_SQUARE 0xDB
#define BLACK_SQUARE 0xFF



int main() {
    char board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    bool white_turn = true;
    bool isWhiteKingMoved = false;
    bool isWhiteRookAMoved = false; // Rook on A1
    bool isWhiteRookHMoved = false; // Rook on H1
    bool isBlackKingMoved = false;
    bool isBlackRookAMoved = false; // Rook on A8
    bool isBlackRookHMoved = false; // Rook on H8

    std::cout << "Welcome to Chess!\nThe coordinates are formatted like 'B4'!\n";


    while (true) {
        std::cout << std::endl;
        ChessUtils().printBoard(WHITE_SQUARE, BLACK_SQUARE, board);


        // In the main loop, check for checkmate or stalemate:
        if (ChessUtils().checkExists(board, white_turn, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved)) {
            if (ChessUtils().canKingMove(white_turn, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved) || ChessUtils().canOthersSaveKing(white_turn, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved)) {
                std::cout << "Check on " << (white_turn ? "white" : "black") << std::endl;
            } else {
                std::cout << "Checkmate on " << (white_turn ? "white" : "black") << std::endl;
                break; // Checkmate ends the game
            }
        }

        std::pair<int, int> old;
        std::pair<int, int> newC;

        if (white_turn)
        {
            std::string start_location, destination;
            std::cout << (white_turn ? "White" : "Black") << " player's turn\n";
            std::cout << "Select a figure start location: ";
            std::cin >> start_location;
            std::cout << "Select destination square: ";
            std::cin >> destination;

            old = ChessUtils().chessboardToArrCords(start_location);
            newC = ChessUtils().chessboardToArrCords(destination);
        } else
        {
            Move m = Minimax().findBestMove(board, 20, isWhiteKingMoved, isBlackKingMoved,
                isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved,white_turn);

            old = m.oldC;
            newC = m.newC;

            //std::cout << "AI move: " << old.first << " " << old.second << " goes to " << newC.first << " " << newC.second << std::endl;
        }






        // Ensure valid turn
        if (std::isupper(board[old.first][old.second]) == white_turn) {
            // Check if move is valid (including castling)
            if (ChessUtils().verifyMove(old, newC, board,
                           isWhiteKingMoved, isBlackKingMoved,
                           isWhiteRookAMoved, isWhiteRookHMoved,
                           isBlackRookAMoved, isBlackRookHMoved, white_turn)) {
                ChessUtils().move(board, old, newC,
                     isWhiteKingMoved, isBlackKingMoved,
                     isWhiteRookAMoved, isWhiteRookHMoved,
                     isBlackRookAMoved, isBlackRookHMoved);
            } else {
                std::cout << "Invalid move. Try again.\n";
                continue;
            }
        } else {
            std::cout << "Invalid move or not your turn. Try again.\n";
            continue;
        }
        white_turn = !white_turn;  // Switch turns
    }
    return 0;
}