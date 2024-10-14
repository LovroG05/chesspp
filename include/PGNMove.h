//
// Created by admin on 14/10/2024.
//

#ifndef PGNMOVE_H
#define PGNMOVE_H

class PGNMove
{
public:
    char piece;                    // Piece being moved ('K', 'Q', 'R', 'B', 'N', 'P')
    std::pair<int, int> oldPos;   // Old position (row, column)
    std::pair<int, int> newPos;   // New position (row, column)
    bool isCapture;                // Whether the move is a capture
    bool isCheck;                  // Whether the move puts the opponent in check
    bool isCheckmate;              // Whether the move results in checkmate
    char promotionPiece;           // Piece to promote to, if applicable
    bool isCastling;               // Whether the move is a castling move

    PGNMove(char p, std::pair<int, int> oldPos, std::pair<int, int> newPos,
         bool capture, bool check, bool checkmate,
         char promo = '\0', bool castling = false)
        : piece(p), oldPos(oldPos), newPos(newPos),
          isCapture(capture), isCheck(check),
          isCheckmate(checkmate),
          promotionPiece(promo), isCastling(castling) {}
};

#endif //PGNMOVE_H
