#include "board/board_utils.h"
#include <vector>

void generatePawnMoves(const GameState& state, std::vector<Move>& moves);
void generateKnightMoves(const GameState &state, std::vector<Move> &moves);
void generateBishopMoves(const GameState& state, std::vector<Move>& moves);
void generateRookMoves(const GameState& state, std::vector<Move>& moves);
void generateQueenMoves(const GameState& state, std::vector<Move>& moves);
void generateKingMoves(const GameState& state, std::vector<Move>& moves);

bool isLegalMove(const GameState &state, const Move &move);

std::vector<Move> generateLegalMoves(const GameState& state) {
    std::vector<Move> pseudoLegalMoves;

    // Generate pawn moves
    generatePawnMoves(state, pseudoLegalMoves);

    // Generate knight moves
    generateKnightMoves(state, pseudoLegalMoves);

    // Generate bishop moves
    generateBishopMoves(state, pseudoLegalMoves);

    // Generate rook moves
    generateRookMoves(state, pseudoLegalMoves);

    // Generate queen moves
    generateQueenMoves(state, pseudoLegalMoves);

    // Generate king moves
    generateKingMoves(state, pseudoLegalMoves);

    std::vector<Move> legalMoves;
    for (const auto& move : pseudoLegalMoves) {
        if (isLegalMove(state, const_cast<Move&>(move))) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}