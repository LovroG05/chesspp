#include <algorithm>
#include <iostream>
#include <Minimax.h>
#include <string>
#include <vector>

#include <ChessUtils.h>
#include <Utils.h>

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

    std::vector<std::vector<std::vector<char>>> history(0, std::vector<std::vector<char>>(8, std::vector<char>(8, ' ')));

    bool white_turn = true;
    bool isWhiteKingMoved = false;
    bool isWhiteRookAMoved = false; // Rook on A1
    bool isWhiteRookHMoved = false; // Rook on H1
    bool isBlackKingMoved = false;
    bool isBlackRookAMoved = false; // Rook on A8
    bool isBlackRookHMoved = false; // Rook on H8
    bool checkOnBlack = false;
    bool checkOnWhite = false;

    std::cout << "Welcome to Chess!\nThe coordinates are formatted like 'B4'!\n";


    int movesFor50Moverule = 0;

    while (true) {
        std::cout << std::endl;
        ChessUtils::printBoard(WHITE_SQUARE, BLACK_SQUARE, board);


        // In the main loop, check for checkmate or stalemate:
        if (ChessUtils::checkExists(board, white_turn, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, checkOnWhite, checkOnBlack)) {
            if (ChessUtils::canKingMove(white_turn, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, checkOnWhite, checkOnBlack)
                || ChessUtils::canOthersSaveKing(white_turn, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, checkOnWhite, checkOnBlack)) {
                std::cout << "Check on " << (white_turn ? "white" : "black") << std::endl;
                if (!white_turn)
                {
                    checkOnBlack = true;
                } else
                {
                    checkOnWhite = true;
                }
            } else {
                std::cout << "Checkmate on " << (white_turn ? "white" : "black") << std::endl;
                break; // Checkmate ends the game
            }
        }

        if (movesFor50Moverule == 50)
        {
            std::cout<< "50 move rule reached - DRAW" << std::endl;
            break;
        }

        if (std::count(history.begin(), history.end(), Utils::boardToVectorBoard(board)) >= 3)
        {
            std::cout << "3 repeated moves - DRAW" << std::endl;
            break;
        }

        std::pair<int, int> old;
        std::pair<int, int> newC;

        /*if (white_turn)
        {
            std::string start_location, destination;
            std::cout << (white_turn ? "White" : "Black") << " player's turn\n";
            std::cout << "Select a figure start location: ";
            std::cin >> start_location;
            std::cout << "Select destination square: ";
            std::cin >> destination;

            old = ChessUtils::chessboardToArrCords(start_location);
            newC = ChessUtils::chessboardToArrCords(destination);
        } else
        {
            Move m = Minimax().findBestMove(board, 20, isWhiteKingMoved, isBlackKingMoved,
                isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved,white_turn, checkOnWhite, checkOnBlack);

            old = m.oldC;
            newC = m.newC;

            //std::cout << "AI move: " << old.first << " " << old.second << " goes to " << newC.first << " " << newC.second << std::endl;
        }*/

        Move m = Minimax().findBestMove(board, 20, isWhiteKingMoved, isBlackKingMoved,
                isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved,white_turn, checkOnWhite, checkOnBlack);

        old = m.oldC;
        newC = m.newC;

        // Ensure valid turn
        if (std::isupper(board[old.first][old.second]) == white_turn) {
            // Check if move is valid (including castling)
            // TODO WTF IS GOING ON HERE EVEN DEBUGGAR IS NOT WORK
            if (((white_turn && checkOnWhite) || (!white_turn && checkOnBlack))
                && !ChessUtils::solvesCheck(white_turn, board, old, newC, isWhiteKingMoved, isBlackKingMoved,
                                       isWhiteRookAMoved, isWhiteRookHMoved,
                                       isBlackRookAMoved, isBlackRookHMoved, checkOnWhite, checkOnBlack))
            {
                std::cout << "First solve the check on " << (white_turn ? "white" : "black") << std::endl;
                std::cout << "attempted move: " << old.first << ", " << old.second << " to " << newC.first << ", " << newC.second << std::endl;
                continue;
            }
            if (ChessUtils::verifyMove(old, newC, board,
                                       isWhiteKingMoved, isBlackKingMoved,
                                       isWhiteRookAMoved, isWhiteRookHMoved,
                                       isBlackRookAMoved, isBlackRookHMoved, white_turn, checkOnWhite, checkOnBlack)) {
                if (std::tolower(board[old.first][old.second]) == 'p' || board[newC.first][newC.second] != ' ')
                {
                    movesFor50Moverule = 0;
                } else
                {
                    movesFor50Moverule++;
                }

                ChessUtils::move(board, old, newC,
                     isWhiteKingMoved, isBlackKingMoved,
                     isWhiteRookAMoved, isWhiteRookHMoved,
                     isBlackRookAMoved, isBlackRookHMoved);

                std::vector<std::vector<char>> boardV = Utils::boardToVectorBoard(board);

                history.resize(history.size() + 1);
                history[history.size()-1] = boardV;

            } else {
                std::cout << "Invalid move. Try again.\n";
                continue;
            }
        } else {
            std::cout << "Invalid move or not your turn. Try again.\n";
            continue;
        }
        white_turn = !white_turn;  // Switch turns
        checkOnBlack = false;
        checkOnWhite = false;
    }
    return 0;
}