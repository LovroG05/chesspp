#define CATCH_CONFIG_MAIN
#include <fstream>

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

TEST_CASE("Convert moves to PGN", "[convertMoveToPGN]") {
    SECTION("Regular move without capture") {
        PGNMove move('P', {6, 4}, {5, 4}, false, false, false);
        REQUIRE(Utils::convertMoveToPGN(move) == "e3");
    }

    SECTION("Regular move with capture") {
        PGNMove move('P', {6, 4}, {5, 5}, true, false, false);
        REQUIRE(Utils::convertMoveToPGN(move) == "exf3");
    }

    SECTION("Kingside castling") {
        PGNMove move('K', {7, 4}, {7, 6}, false, false, false, false, true);
        REQUIRE(Utils::convertMoveToPGN(move) == "O-O");
    }

    SECTION("Queenside castling") {
        PGNMove move('K', {7, 4}, {7, 2}, false, false, false, false, true);
        REQUIRE(Utils::convertMoveToPGN(move) == "O-O-O");
    }

    SECTION("Move that puts opponent in check") {
        PGNMove move('Q', {0, 3}, {4, 3}, false, true, false);
        REQUIRE(Utils::convertMoveToPGN(move) == "Qd4+");
    }

    SECTION("Move that results in checkmate") {
        PGNMove move('Q', {0, 3}, {4, 3}, false, false, true);
        REQUIRE(Utils::convertMoveToPGN(move) == "Qd4#");
    }
}

TEST_CASE("Detect moves from history", "[detectMovesFromHistory]") {
    std::vector<std::vector<std::vector<char>>> history = {
        {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        },
        {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', ' ', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'p', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        }
    };

    SECTION("Detect simple move") {
        std::vector<PGNMove> moves = Utils::detectMovesFromHistory(history);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].piece == 'P');
        REQUIRE(moves[0].oldPos.first == 1);
        REQUIRE(moves[0].oldPos.second == 4);
        REQUIRE(moves[0].newPos.first == 3);
        REQUIRE(moves[0].newPos.second == 4);
        REQUIRE(!moves[0].isCapture);
    }

    SECTION("Detect capture move") {
        std::vector<std::vector<std::vector<char>>> captureHistory = {
            {
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', 'p', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', 'P', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'P', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
            },
            {
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
                {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'P', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
            },
        };

        std::vector<PGNMove> moves = Utils::detectMovesFromHistory(captureHistory);
        REQUIRE(moves.size() == 1);
        REQUIRE(moves[0].piece == 'P');
        REQUIRE(moves[0].oldPos.first == 3);
        REQUIRE(moves[0].oldPos.second == 1);
        REQUIRE(moves[0].newPos.first == 4);
        REQUIRE(moves[0].newPos.second == 2);
        REQUIRE(moves[0].isCapture); // This should be true if it was a capture
    }
}

TEST_CASE("Writing PGN moves to file", "[pgn]") {
    // Create a vector of strings to simulate a game in PGN format
    std::vector<std::string> pgnMoves = {
        "e4",  "e5",         // 1. e4 e5
        "Nf3", "Nc6",        // 2. Nf3 Nc6
        "Bb5", "a6",         // 3. Bb5 a6
        "O-O", "O-O"         // 4. O-O O-O (castling example)
    };

    // Specify the PGN output file
    std::string pgnFilename = "tests.pgn";

    // Call the function to write the PGN file
    Utils::writePGNToFile(pgnFilename, pgnMoves);

    // Open and check the PGN file content
    std::ifstream pgnFile(pgnFilename);
    REQUIRE(pgnFile.is_open());

    std::string pgnContent((std::istreambuf_iterator<char>(pgnFile)),
                            std::istreambuf_iterator<char>());

    // Check that the moves are written in correct PGN format
    REQUIRE(pgnContent.find("1. e4 e5") != std::string::npos);  // 1st move pair
    REQUIRE(pgnContent.find("2. Nf3 Nc6") != std::string::npos); // 2nd move pair
    REQUIRE(pgnContent.find("3. Bb5 a6") != std::string::npos);  // 3rd move pair
    REQUIRE(pgnContent.find("4. O-O O-O") != std::string::npos); // Castling

    // Close the file
    pgnFile.close();

    // Clean up the test file (optional)
    std::remove(pgnFilename.c_str());
}

/*TEST_CASE("Write PGN to file", "[writePGNToFile]") {
    // Create a mock PGN move list for testing
    std::vector<std::string> pgnMoves = {"e4", "e5", "Nf3", "Nc6", "Bb5", "a6", "Ba4", "d6", "c3"};

    // Write to a file
    Utils::writePGNToFile("test.pgn", pgnMoves);

    // Check if the file was created and has the expected content
    std::ifstream inFile("test.pgn");
    REQUIRE(inFile.is_open());

    std::string line;
    int lineCount = 0;

    while (std::getline(inFile, line)) {
        lineCount++;
        // Check the expected output lines
        if (lineCount == 1) {
            REQUIRE(line == "[Event \"Chess Game\"]");
        }
        else if (lineCount == 2) {
            REQUIRE(line == "[Site \"Online\"]");
        }
        else if (lineCount == 3) {
            REQUIRE(line == "[Date \"2024.10.14\"]");
        }
        else if (lineCount == 4) {
            REQUIRE(line == "[Round \"1\"]");
        }
        else if (lineCount == 5) {
            REQUIRE(line == "[White \"Player1\"]");
        }
        else if (lineCount == 6) {
            REQUIRE(line == "[Black \"Player2\"]");
        }
        else if (lineCount == 7) {
            REQUIRE(line == "[Result \"*\"]");
        }
        else if (lineCount > 8) {
            REQUIRE(line == pgnMoves[lineCount - 8]); // Ensure PGN moves match
        }
    }
    inFile.close();
}*/
