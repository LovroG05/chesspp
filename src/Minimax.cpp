//
// Created by admin on 08/10/2024.
//

#include "../include/Minimax.h"

#include <limits>
#include <Utils.h>

#include "../include/ChessUtils.h"

std::map<char, int> piece_values = {
    {'p', 100},
    {'n', 300},
    {'b', 300},
    {'r', 500},
    {'q', 900},
    {'k', 20000}
};

int Minimax::evaluateMaterial(char board[8][8])
{
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char piece = board[i][j];
            if (piece != ' ')
            {
                int value = piece_values[std::tolower(piece)];
                score += value * std::islower(piece) ? -1 : 1;
            }
        }
    }

    return score;
}

// Check if a pawn is isolated (no pawn on adjacent files)
bool Minimax::isIsolatedPawn(const char board[8][8], int row, int col, bool isWhite) {
    char pawn = isWhite ? 'P' : 'p';

    // Check adjacent files (left and right columns)
    if (col > 0) {
        for (int i = 0; i < 8; ++i) {
            if (board[i][col - 1] == pawn) return false;
        }
    }
    if (col < 7) {
        for (int i = 0; i < 8; ++i) {
            if (board[i][col + 1] == pawn) return false;
        }
    }
    return true;
}

// Check if a pawn is doubled (another pawn in the same file)
bool Minimax::isDoubledPawn(const char board[8][8], int row, int col, bool isWhite) {
    char pawn = isWhite ? 'P' : 'p';

    for (int i = 0; i < 8; ++i) {
        if (i != row && board[i][col] == pawn) {
            return true;
        }
    }
    return false;
}

// Check if a pawn is a passed pawn (no enemy pawn blocking on the same or adjacent files)
bool Minimax::isPassedPawn(const char board[8][8], int row, int col, bool isWhite) {
    char enemyPawn = isWhite ? 'p' : 'P';

    int direction = isWhite ? -1 : 1;  // Move direction for white is up (-1), for black is down (+1)

    // Check the same file and adjacent files in front of the pawn
    for (int r = row + direction; r >= 0 && r < 8; r += direction) {
        if (board[r][col] == enemyPawn) return false; // Pawn in the same file
        if (col > 0 && board[r][col - 1] == enemyPawn) return false; // Pawn in the left adjacent file
        if (col < 7 && board[r][col + 1] == enemyPawn) return false; // Pawn in the right adjacent file
    }
    return true;
}

int Minimax::evaluatePawnStructure(char board[8][8], bool isWhite)
{
    int pawnStructureScore = 0;
    char pawn = isWhite ? 'P' : 'p';

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == pawn) {
                // Isolated pawn
                if (isIsolatedPawn(board, row, col, isWhite)) {
                    pawnStructureScore += ISOLATED_PAWN_PENALTY;
                }

                // Doubled pawn
                if (isDoubledPawn(board, row, col, isWhite)) {
                    pawnStructureScore += DOUBLED_PAWN_PENALTY;
                }

                // Passed pawn
                if (isPassedPawn(board, row, col, isWhite)) {
                    pawnStructureScore += PASSED_PAWN_BONUS;
                }

                // Backward pawn
                if (isBackwardPawn(board, row, col, isWhite)) {
                    pawnStructureScore += BACKWARD_PAWN_PENALTY;
                }

                // Pawn chain
                if (isPartOfPawnChain(board, row, col, isWhite)) {
                    pawnStructureScore += CHAIN_PAWN_BONUS;
                }
            }
        }
    }

    return pawnStructureScore;
}

// Check if a pawn is backward (cannot advance without being captured and has no pawn support)
bool Minimax::isBackwardPawn(const char board[8][8], int row, int col, bool isWhite) {
    char enemyPawn = isWhite ? 'p' : 'P';
    char pawn = isWhite ? 'P' : 'p';
    int direction = isWhite ? -1 : 1;

    // Check if it can be supported by a pawn on the adjacent files
    bool canBeSupported = false;
    if (col > 0 && board[row][col - 1] == pawn) canBeSupported = true;
    if (col < 7 && board[row][col + 1] == pawn) canBeSupported = true;

    // If the pawn cannot be supported and is blocked, it's backward
    if (!canBeSupported) {
        int newRow = row + direction;
        if (newRow >= 0 && newRow < 8 && board[newRow][col] == enemyPawn) {
            return true;
        }
    }
    return false;
}

// Check if a pawn is part of a pawn chain (supported diagonally by another pawn)
bool Minimax::isPartOfPawnChain(const char board[8][8], int row, int col, bool isWhite) {
    char pawn = isWhite ? 'P' : 'p';
    int direction = isWhite ? -1 : 1;

    if (col > 0 && row + direction >= 0 && row + direction < 8 && board[row + direction][col - 1] == pawn) {
        return true;
    }
    if (col < 7 && row + direction >= 0 && row + direction < 8 && board[row + direction][col + 1] == pawn) {
        return true;
    }
    return false;
}

// Check for open or half-open files near the king (file has no pawns of the same color)
bool Minimax::isOpenFileNearKing(const char board[8][8], int kingRow, int kingCol, bool isWhite) {
    char pawn = isWhite ? 'P' : 'p';

    // Check the king's file and adjacent files
    for (int fileOffset = -1; fileOffset <= 1; ++fileOffset) {
        int file = kingCol + fileOffset;
        if (file < 0 || file > 7) continue;  // Skip out of bounds

        bool pawnFound = false;
        for (int row = 0; row < 8; ++row) {
            if (board[row][file] == pawn) {
                pawnFound = true;
                break;
            }
        }

        if (!pawnFound) return true;  // No friendly pawns on the file = open file
    }

    return false;
}

// Check the pawn shield in front of the king (basic implementation for castling king safety)
int Minimax::evaluatePawnShield(const char board[8][8], int kingRow, int kingCol, bool isWhite) const
{
    int pawnShieldScore = 0;

    char pawn = isWhite ? 'P' : 'p';
    int direction = isWhite ? -1 : 1;

    // Check the three squares in front of the king (depends if it's castled or not)
    for (int fileOffset = -1; fileOffset <= 1; ++fileOffset) {
        int file = kingCol + fileOffset;
        if (file < 0 || file > 7) continue;  // Skip out of bounds

        int rowInFront = kingRow + direction;  // Row in front of the king
        if (rowInFront >= 0 && rowInFront < 8 && board[rowInFront][file] == pawn) {
            pawnShieldScore += KING_PAWN_SHIELD_BONUS;  // Pawn is protecting the king
        } else {
            pawnShieldScore += KING_MISSING_SHIELD_PAWN_PENALTY;  // Missing pawn in the shield
        }
    }

    return pawnShieldScore;
}

// Evaluate proximity of enemy pieces to the king
int Minimax::evaluateEnemyPieceProximity(const char board[8][8], int kingRow, int kingCol, bool isWhite) const {
    int proximityScore = 0;

    char enemyPawn = isWhite ? 'p' : 'P';
    char enemyKnight = isWhite ? 'n' : 'N';
    char enemyBishop = isWhite ? 'b' : 'B';
    char enemyRook = isWhite ? 'r' : 'R';
    char enemyQueen = isWhite ? 'q' : 'Q';

    // Scan the area around the king (3x3 grid)
    for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
        for (int colOffset = -1; colOffset <= 1; ++colOffset) {
            int newRow = kingRow + rowOffset;
            int newCol = kingCol + colOffset;

            if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7) continue;  // Skip out of bounds

            char piece = board[newRow][newCol];

            // Penalize for enemy pieces near the king
            if (piece == enemyPawn || piece == enemyKnight || piece == enemyBishop || piece == enemyRook || piece == enemyQueen) {
                proximityScore += KING_ENEMY_PROXIMITY_PENALTY;
            }
        }
    }

    return proximityScore;
}

// Check if the king has castled
bool Minimax::hasKingCastled(const char board[8][8], bool isWhite) {
    if (isWhite) {
        // White king castling check (assuming the rook is on h1 for kingside castling)
        return board[7][4] == 'K' && (board[7][5] == '.' && board[7][6] == '.' && board[7][7] == 'R');
    } else {
        // Black king castling check (assuming the rook is on h8 for kingside castling)
        return board[0][4] == 'k' && (board[0][5] == '.' && board[0][6] == '.' && board[0][7] == 'r');
    }
}

int Minimax::evaluateKingSafety(const char board[8][8], bool isWhite) const
{
    int kingSafetyScore = 0;
    int kingRow, kingCol;

    // Find the king's position
    char king = isWhite ? 'K' : 'k';
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == king) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
    }

    // Bonus for castling
    if (hasKingCastled(board, isWhite)) {
        kingSafetyScore += KING_CASTLING_BONUS;
    }

    // Penalize for open or half-open files near the king
    if (isOpenFileNearKing(board, kingRow, kingCol, isWhite)) {
        kingSafetyScore += KING_OPEN_FILE_PENALTY;
    }

    // Evaluate the pawn shield (only applicable if the king is castled)
    kingSafetyScore += evaluatePawnShield(board, kingRow, kingCol, isWhite);

    // Penalize for enemy piece proximity
    kingSafetyScore += evaluateEnemyPieceProximity(board, kingRow, kingCol, isWhite);

    return kingSafetyScore;
}

// Evaluate activity for a knight
int Minimax::evaluateKnightActivity(const char board[8][8], int row, int col, bool isWhite) {
    int activityScore = 0;
    for (int i = 0; i < 8; ++i) {
        int newRow = row + knightMoves[i][0];
        int newCol = col + knightMoves[i][1];
        if (Utils::isValidSquare(newRow, newCol)) {
            char target = board[newRow][newCol];
            if (Utils::isEmptySquare(target) || Utils::isEnemyPiece(target, isWhite)) {
                activityScore += KNIGHT_ACTIVITY_VALUE;
            }
        }
    }
    return activityScore;
}

// Evaluate activity for a king
int Minimax::evaluateKingActivity(const char board[8][8], int row, int col, bool isWhite) {
    int activityScore = 0;
    for (int i = 0; i < 8; ++i) {
        int newRow = row + kingMoves[i][0];
        int newCol = col + kingMoves[i][1];
        if (Utils::isValidSquare(newRow, newCol)) {
            char target = board[newRow][newCol];
            if (Utils::isEmptySquare(target) || Utils::isEnemyPiece(target, isWhite)) {
                activityScore += KING_ACTIVITY_VALUE;
            }
        }
    }
    return activityScore;
}

// Evaluate activity for sliding pieces (bishop, rook, queen)
int Minimax::evaluateSlidingPieceActivity(const char board[8][8], int row, int col, const int directions[][2], int numDirections, int maxDistance, bool isWhite) {
    int activityScore = 0;
    for (int i = 0; i < numDirections; ++i) {
        for (int dist = 1; dist <= maxDistance; ++dist) {
            int newRow = row + dist * directions[i][0];
            int newCol = col + dist * directions[i][1];
            if (!Utils::isValidSquare(newRow, newCol)) break;

            char target = board[newRow][newCol];
            if (Utils::isEmptySquare(target)) {
                activityScore += BISHOP_ACTIVITY_VALUE; // Base value for all sliding pieces
            } else if (Utils::isEnemyPiece(target, isWhite)) {
                activityScore += BISHOP_ACTIVITY_VALUE * 2; // Attacking a piece is worth more
                break; // Cannot move further in this direction
            } else {
                break; // Blocked by own piece
            }
        }
    }
    return activityScore;
}

// Wrapper function to evaluate all piece activity
int Minimax::evaluateAllPieceActivity(const char board[8][8]) {
    int totalActivityScore = 0;

    // Direction arrays for sliding pieces (bishop, rook, queen)
    const int bishopDirections[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    const int rookDirections[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
    const int queenDirections[8][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    // Loop through the board and evaluate each piece's activity
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            char piece = board[row][col];
            if (piece == '.') continue; // Skip empty squares

            bool isWhite = isupper(piece); // True for white pieces

            switch (tolower(piece)) {
                case 'n': // Knight
                    totalActivityScore += evaluateKnightActivity(board, row, col, isWhite);
                    break;
                case 'b': // Bishop
                    totalActivityScore += evaluateSlidingPieceActivity(board, row, col, bishopDirections, 4, 8, isWhite);
                    break;
                case 'r': // Rook
                    totalActivityScore += evaluateSlidingPieceActivity(board, row, col, rookDirections, 4, 8, isWhite);
                    break;
                case 'q': // Queen
                    totalActivityScore += evaluateSlidingPieceActivity(board, row, col, queenDirections, 8, 8, isWhite);
                    break;
                case 'k': // King
                    totalActivityScore += evaluateKingActivity(board, row, col, isWhite);
                    break;
                case 'p': // Pawn (simplified pawn activity)
                    if (isWhite) {
                        if (Utils::isValidSquare(row - 1, col) && Utils::isEmptySquare(board[row - 1][col])) {
                            totalActivityScore += PAWN_ACTIVITY_VALUE; // White pawn moving forward
                        }
                    } else {
                        if (Utils::isValidSquare(row + 1, col) && Utils::isEmptySquare(board[row + 1][col])) {
                            totalActivityScore -= PAWN_ACTIVITY_VALUE; // Black pawn moving forward
                        }
                    }
                    break;
            }
        }
    }

    return totalActivityScore;
}

int Minimax::evaluate(char board[8][8])
{
    int score = 0;
    int materialScore = evaluateMaterial(board);

    int whitePawnStructureScore = evaluatePawnStructure(board, true);
    int blackPawnStructureScore = evaluatePawnStructure(board, false);

    int whiteKingSafetyScore = evaluateKingSafety(board, true);
    int blackKingSafetyScore = evaluateKingSafety(board, false);

    int activityScore = evaluateAllPieceActivity(board);


    score = materialScore
        + whitePawnStructureScore - blackPawnStructureScore
        + whiteKingSafetyScore - blackKingSafetyScore
        + activityScore;
    return score;
}

// WARNING: THE MINIMAX ALGORITHM FUNCTION ALTERS THE BOARD ARRAY! ALWAYS PASS A TEMPBOARD
float Minimax::minimaxAB(char board[8][8], int depth, float alpha, float beta, bool isMaximisingPlayer, bool &isWhiteKingMoved, bool &isBlackKingMoved,
                       bool &isWhiteRookAMoved, bool &isWhiteRookHMoved,
                       bool &isBlackRookAMoved, bool &isBlackRookHMoved, bool &isWhite)
{
    char tempBoard[8][8];
    std::copy(&board[0][0], &board[0][0] + 8 * 8, &tempBoard[0][0]);
    if (depth == 0 &&
        ChessUtils().canKingMove(isWhite, tempBoard, isWhiteKingMoved, isBlackKingMoved,
            isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved) &&
        ChessUtils().canOthersSaveKing(isWhite, tempBoard, isWhiteKingMoved,
            isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved)) // TODO also check for game end
    {
        return evaluate(tempBoard);
    }

    if (isMaximisingPlayer)
    {
        float max_eval = -std::numeric_limits<float>::infinity();
        // for each valid move (NEVEM A SAM CURRENT PLAYER - ZVEDI CURRENT PLAYER - NEKAKO VALIDIREJ MOVE)
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (tempBoard[i][j] != ' ' && (isWhite && std::isupper(tempBoard[i][j]) || (!isWhite && std::islower(tempBoard[i][j]))))
                {
                    char piece = tempBoard[i][j];
                    bool pieceIsWhite = std::isupper(piece);

                    for (int k = 0; k < 8; k++)
                    {
                        for (int l = 0; l < 8; l++)
                        {
                            if ((isWhite && std::islower(tempBoard[k][l]) || !isWhite && std::isupper(tempBoard[k][l])) && ChessUtils().verifyMove(std::make_pair(i, j),
                                std::make_pair(k, l), tempBoard, isWhiteKingMoved,
                                isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved,
                                isBlackRookAMoved, isBlackRookHMoved, isWhite))
                            {
                                ChessUtils().move(tempBoard, std::make_pair(i, j),
                                std::make_pair(k, l), isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved);
                                max_eval = std::max(max_eval, minimaxAB(tempBoard, depth-1, alpha, beta, false, isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved, isWhite));
                                ChessUtils().move(tempBoard, std::make_pair(k, l), std::make_pair(i, j), isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved);
                                alpha = std::max(alpha, max_eval);
                                if (beta <= alpha)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        return max_eval;
    } else
    {
        float min_eval = std::numeric_limits<float>::infinity();
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (tempBoard[i][j] != ' ' && (isWhite && std::isupper(tempBoard[i][j]) || (!isWhite && std::islower(tempBoard[i][j]))))
                {
                    char piece = tempBoard[i][j];
                    bool pieceIsWhite = std::isupper(piece);

                    for (int k = 0; k < 8; k++)
                    {
                        for (int l = 0; l < 8; l++)
                        {
                            if ((isWhite && std::islower(tempBoard[k][l]) || !isWhite && std::isupper(tempBoard[k][l])) && ChessUtils().verifyMove(std::make_pair(i, j),
                                std::make_pair(k, l), tempBoard, isWhiteKingMoved,
                                isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved,
                                isBlackRookAMoved, isBlackRookHMoved, isWhite))
                            {
                                ChessUtils().move(tempBoard, std::make_pair(i, j),
                                std::make_pair(k, l), isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved);
                                min_eval = std::min(min_eval, minimaxAB(tempBoard, depth-1, alpha, beta, true, isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved, isWhite));
                                ChessUtils().move(tempBoard, std::make_pair(k, l), std::make_pair(i, j), isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved);
                                alpha = std::min(beta, min_eval);
                                if (beta <= alpha)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        return min_eval;
    }
}


Move Minimax::findBestMove(char board[8][8], int depth, bool isWhiteKingMoved, bool isBlackKingMoved,
                       bool isWhiteRookAMoved, bool isWhiteRookHMoved,
                       bool isBlackRookAMoved, bool isBlackRookHMoved, bool isWhite)
{
    char tempBoard[8][8];
    std::copy(&board[0][0], &board[0][0] + 8 * 8, &tempBoard[0][0]);
    Move bestMove;

    float max_eval = -std::numeric_limits<float>::infinity();
    float alpha = -std::numeric_limits<float>::infinity();
    float beta = std::numeric_limits<float>::infinity();

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (tempBoard[i][j] != ' ' && (isWhite && std::isupper(tempBoard[i][j]) || (!isWhite && std::islower(tempBoard[i][j]))))
            {
                char piece = tempBoard[i][j];
                for (int k = 0; k < 8; k++)
                {
                    for (int l = 0; l < 8; l++)
                    {
                        if (ChessUtils().verifyMove(std::make_pair(i, j),
                                std::make_pair(k, l), board, isWhiteKingMoved,
                                isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved,
                                isBlackRookAMoved, isBlackRookHMoved, isWhite))
                        {
                            ChessUtils().move(tempBoard, std::make_pair(i, j),
                                std::make_pair(k, l), isWhiteKingMoved, isBlackKingMoved,
                                   isWhiteRookAMoved, isWhiteRookHMoved,
                                   isBlackRookAMoved, isBlackRookHMoved);
                            float eval = minimaxAB(tempBoard, depth-1, alpha, beta, true, isWhiteKingMoved, isBlackKingMoved,
                               isWhiteRookAMoved, isWhiteRookHMoved,
                               isBlackRookAMoved, isBlackRookHMoved, isWhite);
                            ChessUtils().move(tempBoard, std::make_pair(k, l), std::make_pair(i, j), isWhiteKingMoved, isBlackKingMoved,
                               isWhiteRookAMoved, isWhiteRookHMoved,
                               isBlackRookAMoved, isBlackRookHMoved);
                            if (eval >= max_eval)
                            {
                                max_eval = eval;
                                bestMove.newC = std::make_pair(k, l);
                                bestMove.oldC = std::make_pair(i, j);

                                /*std::cout << "new best move: " << bestMove.oldC.first
                                << ", " << bestMove.oldC.second
                                << " to " << bestMove.newC.first << ", " << bestMove.newC.second
                                << " figure " << tempBoard[bestMove.oldC.first][bestMove.oldC.second]
                                << " eval: " << eval
                                << std::endl;*/
                            }
                        }
                    }
                }
            }
        }
    }
    return bestMove;
}
