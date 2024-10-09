//
// Created by admin on 08/10/2024.
//

#include "../include/ChessUtils.h"


void ChessUtils::printBoard(int color1, int color2, char board[8][8])
{
    std::cout << "   A     B     C     D     E     F     G     H\n\n";

    int HCHARSINCELL = 6;
    int VCHARSINCELL = 3;
    int CELLSINLINE = 8;
    int ROWS = 8;

    for (int row = 0; row < ROWS; row++)
    {
        for(int i = 0; i < VCHARSINCELL; i++) // vertical sublines of cells
        {
            for(int j = 0; j < CELLSINLINE; j++) // cells
            {
                for(int k = 0; k < HCHARSINCELL; k++) // chars in cells
                {
                    if (board[row][j] != ' ' && i == 1 && k == 2)
                    {
                        std::cout << board[row][j];
                    }
                    else if (j % 2 == 0)
                    {
                        std::cout << char((row%2 == 0) ? color2 : color1);
                    } else
                    {
                        std::cout << char((row%2 == 0) ? color1 : color2);
                    }
                }
            }
            if (i == 1)
            {
                std::cout << "  " << 8 - row;
            }
            std::cout << std::endl;
        }
    }
}

std::pair<int, int> ChessUtils::chessboardToArrCords(std::string strCoords)
{
    char xc = strCoords.substr(0, 1)[0];
    xc = std::toupper(xc);
    int x = xc - 'A';
    int y = 8 - std::stoi(strCoords.substr(1, 1));

    return std::make_pair(y, x);

}

void ChessUtils::move(char (&board)[8][8], std::pair<int, int> old, std::pair<int, int> newC,
          bool &isWhiteKingMoved, bool &isBlackKingMoved,
          bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
          bool &isBlackRookAMoved, bool &isBlackRookHMoved) {
    char piece = board[old.first][old.second];
    board[old.first][old.second] = ' '; // Clear the original square
    board[newC.first][newC.second] = piece; // Move the piece

    // Handle castling
    if (piece == 'K') { // White King
        isWhiteKingMoved = true; // King has moved
        if (newC.second == old.second + 2) { // Kingside castling
            board[newC.first][newC.second - 1] = 'R'; // Move rook to the left of the king
            board[old.first][7] = ' '; // Clear the rook's original position
            isWhiteRookHMoved = true; // Mark the rook as moved
        } else if (newC.second == old.second - 2) { // Queenside castling
            board[newC.first][newC.second + 1] = 'R'; // Move rook to the right of the king
            board[old.first][0] = ' '; // Clear the rook's original position
            isWhiteRookAMoved = true; // Mark the rook as moved
        }
    } else if (piece == 'k') { // Black King
        isBlackKingMoved = true; // King has moved
        if (newC.second == old.second + 2) { // Kingside castling
            board[newC.first][newC.second - 1] = 'r'; // Move rook to the left of the king
            board[old.first][7] = ' '; // Clear the rook's original position
            isBlackRookHMoved = true; // Mark the rook as moved
        } else if (newC.second == old.second - 2) { // Queenside castling
            board[newC.first][newC.second + 1] = 'r'; // Move rook to the right of the king
            board[old.first][0] = ' '; // Clear the rook's original position
            isBlackRookAMoved = true; // Mark the rook as moved
        }
    } else if (piece == 'R' || piece == 'r') {
        if (old.second == 0) {
            if (std::isupper(piece)) {
                isWhiteRookAMoved = true; // Mark white rook as moved
            } else {
                isBlackRookAMoved = true; // Mark black rook as moved
            }
        } else if (old.second == 7) {
            if (std::isupper(piece)) {
                isWhiteRookHMoved = true; // Mark white rook as moved
            } else {
                isBlackRookHMoved = true; // Mark black rook as moved
            }
        }
    }
}


bool ChessUtils::verifyMove(std::pair<int, int> old, std::pair<int, int> newC, char board[8][8],
                bool &isWhiteKingMoved, bool &isBlackKingMoved,
                bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool isWhite) {
    char piece = board[old.first][old.second];
    char target = board[newC.first][newC.second];

    // Check if the move is to an empty square or an enemy piece
    if ((std::isupper(piece) && std::isupper(target)) || (std::islower(piece) && std::islower(target)) && target != ' ') {
        return false; // Can't capture your own piece
    }

    // Castling conditions
    if ((piece == 'K' && !isWhiteKingMoved && (newC.first == old.first + 2 || newC.first == old.first - 2)) ||
        (piece == 'k' && !isBlackKingMoved && (newC.first == old.first + 2 || newC.first == old.first - 2))) {
        // Kingside castling
        if ((piece == 'K' && newC.first == old.first + 2 && !isWhiteRookHMoved) ||
            (piece == 'k' && newC.first == old.first + 2 && !isBlackRookHMoved)) {
            if (board[old.second][old.first + 1] == ' ' && board[old.second][old.first + 2] == ' ') {
                return true;
            }
        }
        // Queenside castling
        else if ((piece == 'K' && newC.first == old.first - 2 && !isWhiteRookAMoved) ||
                 (piece == 'k' && newC.first == old.first - 2 && !isBlackRookAMoved)) {
            if (board[old.second][old.first - 1] == ' ' && board[old.second][old.first - 2] == ' ') {
                return true;
            }
        }
    }


    switch (std::tolower(board[old.first][old.second]))
            {
            case 'p':
                // Handle white pawns
                    if (std::isupper(board[old.first][old.second])) {
                        // Forward move (1 or 2 squares on the first move)
                        if (old.second == newC.second && (old.first - newC.first == 1 || (old.first == 6 && newC.first == 4 && board[5][old.second] == ' '))  && board[newC.first][newC.second] == ' ')  {
                            return true;
                        }
                        // Diagonal capture
                        else if (std::abs(old.second - newC.second) == 1 && old.first - newC.first == 1 && board[newC.first][newC.second] != ' ') {
                            return true;
                        }
                    }
                // Handle black pawns
                    else {
                        if (old.second == newC.second && (newC.first - old.first == 1 || (old.first == 1 && newC.first == 3 && board[2][old.second] == ' ')) && board[newC.first][newC.second] == ' ') {
                            return true;
                        }
                        else if (std::abs(old.second - newC.second) == 1 && newC.first - old.first == 1 && board[newC.first][newC.second] != ' ') {
                            return true;
                        }
                    }
                break;

            case 'r':
                {
                    if (old.second == newC.second) {
                        // Vertical movement
                        int dir = (newC.first > old.first) ? 1 : -1;
                        for (int i = old.first + dir; i != newC.first; i += dir) {
                            if (board[i][old.second] != ' ') {
                                return false; // Path blocked
                            }
                        }
                        return true;
                    }
                    if (old.first == newC.first) {
                        // Horizontal movement
                        int dir = (newC.second > old.second) ? 1 : -1;
                        for (int i = old.second + dir; i != newC.second; i += dir) {
                            if (board[old.first][i] != ' ') {
                                return false; // Path blocked
                            }
                        }
                        return true;
                    }
                }
                break;

            case 'n':
                if ((std::abs(old.first - newC.first) == 2 && std::abs(old.second - newC.second) == 1) ||
                    (std::abs(old.first - newC.first) == 1 && std::abs(old.second - newC.second) == 2)) {
                    return true;
                    }
                break;

            case 'b':
                if (std::abs(old.first - newC.first) == std::abs(old.second - newC.second)) {
                    int dirY = (newC.first > old.first) ? 1 : -1;
                    int dirX = (newC.second > old.second) ? 1 : -1;
                    for (int i = old.first + dirY, j = old.second + dirX; i != newC.first && j != newC.second; i += dirY, j += dirX) {
                        if (board[j][i] != ' ') {
                            return false; // Path blocked
                        }
                    }
                    return true;
                }
                break;

            case 'q':
                // Use rook-like or bishop-like movement
                    if ((old.first == newC.first || old.second == newC.second) ||
                        (std::abs(old.first - newC.first) == std::abs(old.second - newC.second))) {
                        // Combine the logic for rook and bishop movement (similar path-checking)
                        return true;
                        }
                break;

            case 'k':
                if (std::abs(old.first - newC.first) <= 1 && std::abs(old.second - newC.second) <= 1) {
                    return true;
                }
                break;

            }
    return false;
}

std::pair<int, int> ChessUtils::findKing(char board[8][8], bool white)
{
    std::pair<int, int> king;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((board[i][j] == 'K' && white) || (board[i][j] == 'k' && !white)) {
                king = std::make_pair(i, j);
            }
        }
    }

    return king;
}

bool ChessUtils::checkExists(char board[8][8], bool isKingWhite, bool &isWhiteKingMoved, bool &isBlackKingMoved, bool &isWhiteRookAMoved, bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved) {
    std::pair<int, int> king;
    // Locate the king
    king = findKing(board, isKingWhite);

    //std::cout << "king on " << king.first << ' ' << king.second << '\n';

    // Check all opponent pieces to see if they can attack the king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char piece = board[i][j];
            if ((isKingWhite && std::islower(piece)) || (!isKingWhite && std::isupper(piece)) && piece != ' ') {
                std::pair<int, int> attacker = std::make_pair(i, j);
                //std::cout << "attacker on " << attacker.first << ' ' << attacker.second << ' ' << piece << '\n';
                if (verifyMove(attacker, king, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, !isKingWhite)) {
                    //std::cout << "king in check\n";
                    return true; // King is in check
                }
            }
        }
    }

    //std::cout << "king off\n";
    return false; // King is not in check
}



bool ChessUtils::canKingMove(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved, bool &isWhiteRookAMoved, bool &isWhiteRookHMoved, bool &isBlackRookAMoved, bool &isBlackRookHMoved) {
    std::pair<int, int> kingPosition;

    // Find the king
    kingPosition = findKing(board, white);

    //std::cout << "king on " << kingPosition.first << ' ' << kingPosition.second << '\n';


    // Check all adjacent squares
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;  // Skip current square

            int ni = kingPosition.first + di;
            int nj = kingPosition.second + dj;

            // Ensure the new position is within bounds
            if (ni >= 0 && ni < 8 && nj >= 0 && nj < 8) {
                // Ensure the square is either empty or contains an enemy piece
                if (board[ni][nj] == ' ' || (white && std::islower(board[ni][nj])) || (!white && std::isupper(board[ni][nj]))) {
                    //std::cout << "possible position on " << ni << ' ' << nj << '\n';
                    // Temporarily move the king to that square and check if still in check
                    char tempBoard[8][8];
                    std::copy(&board[0][0], &board[0][0] + 8 * 8, &tempBoard[0][0]);

                    tempBoard[kingPosition.first][kingPosition.second] = ' ';  // Remove the king from its current position
                    tempBoard[ni][nj] = white ? 'K' : 'k';  // Move the king to the new square

                    //std::cout << "TEMP BOARD\n\n\n";
                    //printBoard(WHITE_SQUARE, BLACK_SQUARE, tempBoard);
                    //std::cout << '\n\n\n';

                    if (!checkExists(tempBoard, white, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved)) {
                        //std::cout << "king can move\n";
                        return true;  // The king can move here safely
                    }
                }
            }
        }
    }

    return false;  // The king cannot move
}



bool ChessUtils::canOthersSaveKing(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
                       bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                       bool &isBlackRookAMoved, bool &isBlackRookHMoved) {
    /*std::pair<int, int> king;
    // Locate the king
    king = findKing(board, white);*/

    // Simulate all friendly piece moves to try and remove the check
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char piece = board[i][j];
            if ((white && std::isupper(piece)) || (!white && std::islower(piece))) {
                std::pair<int, int> attacker = std::make_pair(i, j);
                // Test each possible move for this piece
                for (int ni = 0; ni < 8; ni++) {
                    for (int nj = 0; nj < 8; nj++) {
                        std::pair<int, int> destination = std::make_pair(ni, nj);
                        if (verifyMove(attacker, destination, board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, white)) {
                            char tempBoard[8][8];
                            // Simulate the move
                            memcpy(tempBoard, board, sizeof(tempBoard));
                            move(tempBoard, attacker, destination, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved);

                            // Check if the move solves the check
                            if (!checkExists(tempBoard, white, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved)) {
                                return true; // The move saves the king
                            }
                        }
                    }
                }
            }
        }
    }
    return false; // No piece can save the king
}



bool ChessUtils::canMove(bool white, char board[8][8], bool &isWhiteKingMoved, bool &isBlackKingMoved,
                bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                bool &isBlackRookAMoved, bool &isBlackRookHMoved)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != ' ' && (std::isupper(board[i][j]) && white || std::islower(board[i][j]) && !white))
            {
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if (verifyMove(std::make_pair(i, j), std::make_pair(k, l), board, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved, white))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ChessUtils::solvesCheck(bool white, char board[8][8], std::pair<int, int> old, std::pair<int, int> newC, bool &isWhiteKingMoved, bool &isBlackKingMoved,
                bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                bool &isBlackRookAMoved, bool &isBlackRookHMoved)
{
    char tempBoard[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] != ' ')
            {
                tempBoard[i][j] = board[i][j];
            } else
            {
                tempBoard[i][j] = ' ';
            }
        }
    }
    move(tempBoard, old, newC,
                           isWhiteKingMoved, isBlackKingMoved,
                           isWhiteRookAMoved, isWhiteRookHMoved,
                           isBlackRookAMoved, isBlackRookHMoved);

    return checkExists(tempBoard, white,
                           isWhiteKingMoved, isBlackKingMoved,
                           isWhiteRookAMoved, isWhiteRookHMoved,
                           isBlackRookAMoved, isBlackRookHMoved);
}