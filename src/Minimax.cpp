//
// Created by admin on 08/10/2024.
//

#include "../include/Minimax.h"

#include <limits>
#include "../include/ChessUtils.h"

std::map<char, int> piece_values = {
    {'p', 1},
    {'n', 3},
    {'b', 3},
    {'r', 5},
    {'q', 9},
    {'k', 0}
};

int Minimax::evaluate(char board[8][8])
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
