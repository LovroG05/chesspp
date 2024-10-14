//
// Created by admin on 09/10/2024.
//

#include "../include/Utils.h"

#include <fstream>

std::vector<std::vector<char>> Utils::boardToVectorBoard(char board[8][8]){
  std::vector<std::vector<char>> vecBoard(8, std::vector<char>(8));
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      vecBoard[i][j] = board[i][j];
    }
  }
  return vecBoard;
}

// Function to check if a square is within bounds
bool Utils::isValidSquare(int row, int col) {
  return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Check if a square is empty or occupied by an opponent
bool Utils::isEnemyPiece(char piece, bool isWhite) {
  return (isWhite && islower(piece)) || (!isWhite && isupper(piece));
}

// Check if a square is empty
bool Utils::isEmptySquare(char piece) {
  return piece == ' ';
}


// Function to convert a move to PGN
std::string Utils::convertMoveToPGN(const PGNMove& move) {
    std::string pgn;

    // Handle castling
    if (move.isCastling) {
        return (move.newPos.second == 6) ? "O-O" : "O-O-O"; // Kingside or Queenside castling
    }

    // Handle piece notation
    if (move.piece != 'P') { // Pawns do not have a letter in PGN
        pgn += move.piece; // Add piece notation (K, Q, R, B, N)
    }

    // Handle captures
    if (move.isCapture) {
        if (move.piece == 'P') {
            // For pawns, specify the file from which the pawn came
            pgn += static_cast<char>('a' + move.oldPos.second); // Convert column index to letter
            pgn += "x"; // Capture notation
        } else {
            pgn += "x"; // Capture notation
        }
    }

    // Append destination square
    pgn += static_cast<char>('a' + move.newPos.second); // Convert column index to letter
    pgn += std::to_string(8 - move.newPos.first); // Convert row index to number

    // Handle check and checkmate
    if (move.isCheck) {
        pgn += "+"; // Check notation
    }
    if (move.isCheckmate) {
        pgn += "#"; // Checkmate notation
    }

    return pgn;
}

// Function to detect moves based on board history
std::vector<PGNMove> Utils::detectMovesFromHistory(const std::vector<std::vector<std::vector<char>>>& history) {
    std::vector<PGNMove> moves;

    for (size_t i = 1; i < history.size(); ++i) {
        const auto& oldBoard = history[i - 1];
        const auto& newBoard = history[i];

        std::pair<int, int> oldPos{-1, -1};
        std::pair<int, int> newPos{-1, -1};
        char movedPiece = '\0';
        bool isCapture = false;

        // Detect the move
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (oldBoard[row][col] != newBoard[row][col]) {
                    // A piece has moved
                    movedPiece = std::toupper(oldBoard[row][col]); // Piece being moved
                    oldPos = {row, col}; // Old position
                    goto out;
                    // Check if it was a capture
                    /*if (oldBoard[row][col] != ' ') {
                        isCapture = true;
                    }*/
                }
            }
        }
    out:
        // Update newPos based on where the piece was moved
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if (std::toupper(newBoard[row][col]) == movedPiece && oldBoard[row][col] != newBoard[row][col]) {
                    newPos = {row, col}; // New position
                    if (oldBoard[row][col] != ' ') {
                        isCapture = true;
                    }
                    break;
                }
            }
        }

        // Check for castling
        bool isCastling = false;
        if (movedPiece == 'K' && (abs(newPos.second - oldPos.second) == 2)) {
            isCastling = true;
        }

        // Create a Move object
        moves.emplace_back(movedPiece, oldPos, newPos, isCapture, false, false, isCastling);
    }

    return moves;
}

// Function to convert a history of moves to PGN
std::vector<std::string> Utils::convertMoveHistoryToPGN(const std::vector<std::vector<std::vector<char>>>& history) {
    std::vector<PGNMove> moves = detectMovesFromHistory(history);
    std::vector<std::string> pgnMoves;

    for (const PGNMove& move : moves) {
        pgnMoves.push_back(convertMoveToPGN(move));
    }

    return pgnMoves;
}

// Function to write PGN moves to a .pgn file
void Utils::writePGNToFile(const std::string& filename, const std::vector<std::string>& pgnMoves) {
    std::ofstream pgnFile(filename);

    if (!pgnFile.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }

    // Write some PGN headers (optional, adjust as necessary)
    pgnFile << "[Event \"Casual Game\"]\n";
    pgnFile << "[Site \"?\"]\n";
    pgnFile << "[Date \"2024.10.14\"]\n";  // Use current date here
    pgnFile << "[Round \"1\"]\n";
    pgnFile << "[White \"Player1\"]\n";
    pgnFile << "[Black \"Player2\"]\n";
    pgnFile << "[Result \"*\"]\n";  // Default result
    pgnFile << "\n";  // Blank line between headers and moves

    // Track whether we're processing White's or Black's move
    int moveCounter = 1;
    for (size_t i = 0; i < pgnMoves.size(); ++i)
    {
        // Every pair of moves starts a new numbered move (e.g., "1. e4 e5")
        if (i % 2 == 0) {
            pgnFile << moveCounter++ << ". ";  // Start of a new move pair (e.g., "1. ")
        }

        // Write the move (White or Black)
        pgnFile << pgnMoves[i] << " ";

        // Optional: Add a newline every 6 moves to make it easier to read
        if (i > 0 && i % 12 == 0) {
            pgnFile << "\n";
        }
    }
}