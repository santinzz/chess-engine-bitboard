#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#pragma once

#include <iostream>
#include <cstdint>
#include "types/types.h"
#include <vector>


void printBitboard(const U64 bb);

inline int countSetBits(U64 bb) {
  return __builtin_popcountll(bb);
}

inline Square getLSB(U64 bb) {
  return static_cast<Square>(__builtin_ctzll(bb));
}
inline void clearLSB(U64 &bb) {
  bb &= (bb - 1);
}

inline Square popLSB(U64 &bb) {
  Square lsb = getLSB(bb);
  clearLSB(bb);
  return lsb;
}

inline Square getMSB(U64 bb) {
  if (bb == 0) return NO_SQUARE;

  return static_cast<Square>(63 - __builtin_clzll(bb));
}

inline const char* squareToString(Square sq) {
  static const char* squareNames[] = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
    "NO_SQUARE"
  };

  if (sq > A1 && sq <= H8) {
    return squareNames[sq];
  }
  return squareNames[NO_SQUARE];
}

inline char pieceToChar(PieceType type, Color color) {
  char pieceChar;
  switch (type) {
    case PAWN: pieceChar = 'p'; break;
    case KNIGHT: pieceChar = 'n'; break;
    case BISHOP: pieceChar = 'b'; break;
    case ROOK: pieceChar = 'r'; break;
    case QUEEN: pieceChar = 'q'; break;
    case KING: pieceChar = 'k'; break;
    default: pieceChar = '.'; break;
  }
  return (color == WHITE) ? static_cast<char>(toupper(pieceChar)) : pieceChar;
}

void printBoard(const GameState &state);

inline bool isEmpty(const GameState &state, Square sq)
{
  return (state.board.emptySquares & squareToBitboard(sq));
}

// Function to remove a piece
void removePiece(Bitboard& board, PieceType type, Color color, Square sq);
// Function to get piece type at a square (already exists in board.cpp, but declare here)
PieceType getPieceTypeAt(const Bitboard& board, Square sq);
// Function to get piece color at a square (already exists in board.cpp, but declare here)
Color getPieceColorAt(const Bitboard& board, Square sq);

// The core makeMove function
void makeMove(GameState& state, const Move& move);

// --- Legality Check Helpers ---
bool isKingInCheck(Color kingColor, const GameState& state);

bool isSquareAttacked(const GameState &state, Square sq, Color attackingColor);
bool isLegalMove(const GameState &state, const Move &move);

#endif // BOARD_UTILS_H