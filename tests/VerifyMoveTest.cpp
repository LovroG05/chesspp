#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/ChessUtils.h"
#include "../include/Utils.h"

// Sample chessboard for testing
char sampleBoard[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

// Helper variables for king and rook movement tracking
bool isWhiteKingMoved = false;
bool isBlackKingMoved = false;
bool isWhiteRookAMoved = false;
bool isWhiteRookHMoved = false;
bool isBlackRookAMoved = false;
bool isBlackRookHMoved = false;

TEST_CASE("Chessboard coordinates are correctly converted to array indices", "[chessboardToArrCords]") {
    auto coords = ChessUtils::chessboardToArrCords("A1");
    REQUIRE(coords.first == 7);
    REQUIRE(coords.second == 0);

    coords = ChessUtils::chessboardToArrCords("H8");
    REQUIRE(coords.first == 0);
    REQUIRE(coords.second == 7);

    coords = ChessUtils::chessboardToArrCords("D5");
    REQUIRE(coords.first == 3);
    REQUIRE(coords.second == 3);
}

// Test: ChessUtils::move
TEST_CASE("Pieces move correctly on the board", "[move]") {
    char board[8][8];
    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Move white pawn from E2 to E4
    ChessUtils::move(board, {6, 4}, {4, 4}, isWhiteKingMoved, isBlackKingMoved, isWhiteRookAMoved, isWhiteRookHMoved, isBlackRookAMoved, isBlackRookHMoved);

    REQUIRE(board[4][4] == 'P');  // Pawn should be at E4 now
    REQUIRE(board[6][4] == ' ');  // E2 should be empty
}


TEST_CASE("Check Exists Functionality", "[checkExistsTest]") {
    char board[8][8];

    // Initialize all movement flags
    bool isWhiteKingMoved = false;
    bool isBlackKingMoved = false;
    bool isWhiteRookAMoved = false;
    bool isWhiteRookHMoved = false;
    bool isBlackRookAMoved = false;
    bool isBlackRookHMoved = false;

    SECTION("White King is in check by a black rook") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][4] = 'r';  // Black Rook at e2

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("White King is not in check") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][5] = 'n';  // Black Knight at f2

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == false);
    }

    SECTION("White King is in check by a black bishop") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[2][2] = 'b';  // Black Bishop at c3

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("White King is in check by a black queen") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[0][7] = 'q';  // Black Queen at h1

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("White King is in check by a black knight") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][6] = 'n';  // Black Knight at f2

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("King is in check by multiple pieces") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][4] = 'r';  // Black Rook at e2
        board[2][2] = 'b';  // Black Bishop at c3

        bool checkOnWhite = false; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true); // King is in check
    }

    SECTION("Check is blocked by pawn") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[0][5] = 'P';  // Black Rook at e2
        board[0][7] = 'q';  // Black Bishop at c3

        bool checkOnWhite = true; // No initial check
        bool checkOnBlack = false; // No initial check

        REQUIRE(ChessUtils::checkExists(board, true, isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == false); // King is in check
    }
}

// Test: ChessUtils::verifyMove
TEST_CASE("Valid and invalid moves for all pieces are verified", "[verifyMove]") {
    char board[8][8];
    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: White Pawn Move (E2 to E4)
    bool validMove = ChessUtils::verifyMove(
        {6, 4}, {4, 4}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == true);

    // Test: Black Pawn Capture (C5 to D4)
    board[3][3] = 'P'; // White pawn on D4
    board[2][4] = 'p'; // Black pawn on C5
    validMove = ChessUtils::verifyMove(
        {2, 4}, {3, 3}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        false, false, false // Black's turn, no checks
    );
    REQUIRE(validMove == true);

    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: Invalid Pawn Move (E2 to E5)
    validMove = ChessUtils::verifyMove(
        {6, 4}, {3, 4}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == false);

    // Test: Knight Move (B1 to C3)
    validMove = ChessUtils::verifyMove(
        {7, 1}, {5, 2}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == true);

    // Test: Invalid Knight Move (B1 to B2)
    validMove = ChessUtils::verifyMove(
        {7, 1}, {6, 1}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == false);

    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: Bishop Move (C1 to F4)
    board[7][2] = 'B'; // Place white bishop on C1
    board[6][3] = ' ';
    validMove = ChessUtils::verifyMove(
        {7, 2}, {4, 5}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == true);

    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: Invalid Bishop Move (C1 to F4 but a pawn is in the way)
    board[6][3] = 'P';
    validMove = ChessUtils::verifyMove(
        {7, 2}, {4, 5}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == false);

    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: Move that resolves check (King moves from E1 to F1)
    board[7][4] = 'K'; // White king on E1
    board[5][4] = 'q'; // Black queen putting the white king in check
    board[6][5] = ' ';
    board[7][5] = ' ';
    validMove = ChessUtils::verifyMove(
        {7, 4}, {7, 5}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, true, false // White's turn, white in check
    );
    REQUIRE(validMove == true);

    memcpy(board, sampleBoard, sizeof(sampleBoard));

    // Test: Move that does not resolve check (King moves to D1)
    validMove = ChessUtils::verifyMove(
        {7, 4}, {7, 3}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, true, false // White's turn, white in check
    );
    REQUIRE(validMove == false);

    // Test: Invalid King move (King tries to capture a piece on E1)
    validMove = ChessUtils::verifyMove(
        {7, 4}, {7, 3}, board,
        isWhiteKingMoved, isBlackKingMoved,
        isWhiteRookAMoved, isWhiteRookHMoved,
        isBlackRookAMoved, isBlackRookHMoved,
        true, false, false // White's turn, no checks
    );
    REQUIRE(validMove == false);
}

TEST_CASE("Solves Check Functionality") {
    char board[8][8];

    // Initialize all movement flags
    bool isWhiteKingMoved = false;
    bool isBlackKingMoved = false;
    bool isWhiteRookAMoved = false;
    bool isWhiteRookHMoved = false;
    bool isBlackRookAMoved = false;
    bool isBlackRookHMoved = false;

    SECTION("White King is in check, move resolves the check") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[0][7] = 'q';  // Black Queen at h1

        std::pair<int, int> old = {0, 4};  // Current position of the White King
        std::pair<int, int> newC = {1, 4}; // Move the king to e2

        bool checkOnWhite = true; // King is in check
        bool checkOnBlack = false; // No initial check on black

        REQUIRE(ChessUtils::solvesCheck(true, board, old, newC,
                                          isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("White King is in check, move does not resolve the check") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[0][7] = 'q';  // Black Queen at h1

        std::pair<int, int> old = {0, 4};  // Current position of the White King
        std::pair<int, int> newC = {0, 5}; // Move the king to f1 (still in check)

        bool checkOnWhite = true; // King is in check
        bool checkOnBlack = false; // No initial check on black

        REQUIRE(ChessUtils::solvesCheck(true, board, old, newC,
                                          isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == false);
    }

    SECTION("White King moves into a check position") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][5] = 'r';  // Black Rook at e2

        std::pair<int, int> old = {0, 4};  // Current position of the White King
        std::pair<int, int> newC = {1, 4}; // Move the king to e2 (now in check)

        bool checkOnWhite = false; // King not in check initially
        bool checkOnBlack = false; // No initial check on black

        REQUIRE(ChessUtils::solvesCheck(true, board, old, newC,
                                          isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == false);
    }

    SECTION("Move captures the piece that checks the King") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[1][4] = 'q';  // Black Queen at e2

        std::pair<int, int> old = {0, 4};  // Current position of the White King
        std::pair<int, int> newC = {1, 4}; // Move the King to e2 and capture the Queen

        bool checkOnWhite = true; // King is in check
        bool checkOnBlack = false; // No initial check on black

        // Here we assume that capturing a piece will solve the check
        REQUIRE(ChessUtils::solvesCheck(true, board, old, newC,
                                          isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }

    SECTION("Move blocks the check") {
        memset(board, ' ', sizeof(board));  // Clear the board
        board[0][4] = 'K';  // White King at e1
        board[0][7] = 'q';  // Black Queen at h1
        board[1][5] = 'P';  // White Pawn at f1

        std::pair<int, int> old = {1, 5};  // Current position of the White King
        std::pair<int, int> newC = {0, 5}; // Move the King to e2

        bool checkOnWhite = true; // King is in check
        bool checkOnBlack = false; // No initial check on black

        // Here we assume that moving to e2 does not solve the check since the king is still in the same line as the black queen
        REQUIRE(ChessUtils::solvesCheck(true, board, old, newC,
                                          isWhiteKingMoved, isBlackKingMoved,
                                          isWhiteRookAMoved, isWhiteRookHMoved,
                                          isBlackRookAMoved, isBlackRookHMoved,
                                          checkOnWhite, checkOnBlack) == true);
    }
}