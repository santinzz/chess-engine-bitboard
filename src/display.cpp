#include <iostream>
#include <iomanip>
#include "board/board_utils.h"

void printBitboard(U64 bb) {
    for (int r = 7; r >= 0; --r) { // Loop ranks from 8 down to 1
        for (int f = 0; f < 8; ++f) { // Loop files from A to H
            Square sq = r * 8 + f;
            if ((bb >> sq) & 1) {
                std::cout << "1 "; // Or "X "
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

PieceType getPieceTypeAt(const Bitboard& board, Square sq) {
    U64 sq_bb = squareToBitboard(sq);
    if (board.pawns[WHITE] & sq_bb || board.pawns[BLACK] & sq_bb) return PAWN;
    if (board.knights[WHITE] & sq_bb || board.knights[BLACK] & sq_bb) return KNIGHT;
    if (board.bishops[WHITE] & sq_bb || board.bishops[BLACK] & sq_bb) return BISHOP;
    if (board.rooks[WHITE] & sq_bb || board.rooks[BLACK] & sq_bb) return ROOK;
    if (board.queens[WHITE] & sq_bb || board.queens[BLACK] & sq_bb) return QUEEN;
    if (board.kings[WHITE] & sq_bb || board.kings[BLACK] & sq_bb) return KING;
    return NONE;
}

Color getPieceColorAt(const Bitboard& board, Square sq) {
    U64 sq_bb = squareToBitboard(sq);
    if (board.byColor[WHITE] & sq_bb) return WHITE;
    if (board.byColor[BLACK] & sq_bb) return BLACK;
    return NO_COLOR; // Empty square
}

void printBoard(const GameState& state) {
    std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    for (int r = 7; r >= 0; --r) { // Iterate from rank 8 down to 1
        std::cout << r + 1 << " |"; // Print rank number
        for (int f = 0; f < 8; ++f) { // Iterate from file A to H
            Square sq = r * 8 + f;
            PieceType type = getPieceTypeAt(state.board, sq);
            Color color = getPieceColorAt(state.board, sq);

            if (type != NONE) {
                std::cout << " " << pieceToChar(type, color) << " |";
            } else {
                std::cout << "   |"; // Empty square
            }
        }
        std::cout << std::endl;
        std::cout << "  +---+---+---+---+---+---+---+---+" << std::endl;
    }
    std::cout << "    A   B   C   D   E   F   G   H" << std::endl;
    std::cout << "Side to move: " << (state.currentTurn == WHITE ? "White" : "Black") << std::endl;
    std::cout << "Castling Rights: ";
    if (state.castlingRights & CastlingRights::WK) std::cout << "K";
    if (state.castlingRights & CastlingRights::WQ) std::cout << "Q";
    if (state.castlingRights & CastlingRights::BK) std::cout << "k";
    if (state.castlingRights & CastlingRights::BQ) std::cout << "q";
    if (state.castlingRights == CastlingRights::NONE) std::cout << "-";
    std::cout << std::endl;
    std::cout << "En Passant Square: " << (state.enPassantSquare != NO_SQUARE ? squareToString(state.enPassantSquare) : "-") << std::endl;
    std::cout << "Halfmove Clock: " << state.halfMoveClock << std::endl;
    std::cout << "Fullmove Number: " << state.fullMoveNumber << std::endl;
    std::cout << "Zobrist Key (placeholder): " << std::hex << state.zobristKey << std::dec << std::endl;
    std::cout << std::endl;
}
