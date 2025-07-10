#include <iostream>
#include "board/board_utils.h"
#include <vector>
#include <algorithm>

void initializeStartingBoard(GameState &state);

extern const U64 RANK_MASKS[8];
extern const U64 FILE_MASKS[8];

void printBoard(const GameState &state);
void generatePawnMoves(const GameState &state, std::vector<Move> &moves);

extern U64 PAWN_PUSH_MASKS[2][64];
extern U64 PAWN_ATTACK_MASKS[2][64];

std::vector<Move> generateLegalMoves(const GameState& state);

int main() {
  initPieceMasks();
   GameState gameState;
   initializeStartingBoard(gameState);

    std::cout << "Initial Board State:" << std::endl;
    printBoard(gameState);

    std::vector<Move> legalMoves = generateLegalMoves(gameState); // <--- Use this now!

    std::cout << "Legal Moves for " << (gameState.currentTurn == WHITE ? "White" : "Black") << " (" << legalMoves.size() << "):" << std::endl;

    // Optional: Sort moves for consistent output
    std::sort(legalMoves.begin(), legalMoves.end(), [](const Move& a, const Move& b) {
        if (a.from != b.from) {
            return a.from < b.from;
        }
        return a.to < b.to;
    });

    for (const auto& move : legalMoves) {
        std::cout << "  " << squareToString(move.from) << squareToString(move.to);
        if (move.promotionPieceType != NONE) {
            std::cout << pieceToChar(move.promotionPieceType, gameState.currentTurn);
        }
        if (move.isCapture) {
            std::cout << " (C)";
        }
        if (move.isCastling) {
            std::cout << " (O-O)";
        }
        if (move.isEnPassant) {
            std::cout << " (EP)";
        }
        std::cout << std::endl;
    }

    return 0;
}