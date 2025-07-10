#ifndef TYPES_H
#define TYPES_H

#pragma once

#include <cstdint>

using U64 = uint64_t;

using Square = int;
using File = int;
using Rank = int;

enum PieceType
{
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NONE
};

enum Color
{
  WHITE,
  BLACK,
  NO_COLOR
};

struct Piece
{
  PieceType type;
  Color color;

  Piece(PieceType t = PieceType::NONE, Color c = NO_COLOR) : type(t), color(c) {}
};

enum SquareName {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQUARE
};

namespace CastlingRights {
    const U64 WK = 1; // White Kingside
    const U64 WQ = 2; // White Queenside
    const U64 BK = 4; // Black Kingside
    const U64 BQ = 8; // Black Queenside
    const U64 ALL = WK | WQ | BK | BQ;
    const U64 NONE = 0;
}

struct Bitboard
{
  U64 byColor[2]; // 0: white, 1: black
  U64 byType[6];

  U64 pawns[2]; // 0: white, 1: black
  U64 knights[2];
  U64 bishops[2];
  U64 rooks[2];
  U64 queens[2];
  U64 kings[2];

  U64 allPieces;
  U64 emptySquares;

  Bitboard() {
    for (int i = 0; i < 2; ++i) byColor[i] = 0ULL;
    for (int i = 0; i < 6; ++i) byType[i] = 0ULL;
    for (int i = 0; i < 2; ++i) pawns[i] = 0ULL;
    for (int i = 0; i < 2; ++i) knights[i] = 0ULL;
    for (int i = 0; i < 2; ++i) bishops[i] = 0ULL;
    for (int i = 0; i < 2; ++i) rooks[i] = 0ULL;
    for (int i = 0; i < 2; ++i) queens[i] = 0ULL;
    for (int i = 0; i < 2; ++i) kings[i] = 0ULL;
    allPieces = 0ULL;
    emptySquares = ~0ULL;
  }
};

struct Move
{
  Square from;
  Square to;
  PieceType promotionPieceType;
  bool isCapture;
  bool isEnPassant;
  bool isCastling;

  Move() : from(0), to(0), promotionPieceType(NONE),
           isCapture(false), isEnPassant(false), isCastling(false) {}

  Move(Square f, Square t, PieceType p = NONE,
       bool capture = false, bool enPassant = false, bool castling = false)
      : from(f), to(t), promotionPieceType(p),
        isCapture(capture), isEnPassant(enPassant), isCastling(castling) {}
};

struct GameState {
  Bitboard board;
  Color currentTurn;
  int halfMoveClock;
  int fullMoveNumber;
  U64 castlingRights;
  Square enPassantSquare;
  U64 zobristKey;
  Square whiteKingSquare;
  Square blackKingSquare;

  GameState(): currentTurn(WHITE), halfMoveClock(0), fullMoveNumber(1),
              castlingRights(CastlingRights::ALL), enPassantSquare(NO_SQUARE),
              zobristKey(0ULL), whiteKingSquare(NO_SQUARE), blackKingSquare(NO_SQUARE) {}
};

// Define castling rights flags

const U64 RANK_1 = 0xFFULL; // 0b000...00011111111
const U64 RANK_2 = RANK_1 << 8;
const U64 RANK_3 = RANK_1 << 16;
const U64 RANK_4 = RANK_1 << 24;
const U64 RANK_5 = RANK_1 << 32;
const U64 RANK_6 = RANK_1 << 40;
const U64 RANK_7 = RANK_1 << 48;
const U64 RANK_8 = RANK_1 << 56;

const U64 FILE_A = 0x0101010101010101ULL;
const U64 FILE_B = FILE_A << 1;
const U64 FILE_C = FILE_A << 2;
const U64 FILE_D = FILE_A << 3;
const U64 FILE_E = FILE_A << 4;
const U64 FILE_F = FILE_A << 5;
const U64 FILE_G = FILE_A << 6;
const U64 FILE_H = FILE_A << 7;

inline U64 squareToBitboard(Square square) {
    return 1ULL << square;
}

inline Rank getRank(Square square) {
    return square / 8;
}

inline File getFile(Square square) {
    return square % 8;
}

inline int getAntiDiagIndex(Square sq) {
  return getRank(sq) + getFile(sq);
}

inline int getMainDiagIndex(Square sq) {
  return getRank(sq) - getFile(sq) + 7; // Adjust to fit in 0-14 range
}

U64 getSliderAttacks(Square fromSq, U64 blockers, PieceType type);

extern const U64 RANK_MASKS[8];
extern const U64 FILE_MASKS[8];
extern U64 PAWN_PUSH_MASKS[2][64];  // [color][square] -> bitboard of possible pushes
extern U64 PAWN_ATTACK_MASKS[2][64]; // [color][square] -> bitboard of possible attacks
extern U64 KNIGHT_ATTACK_MASKS[64];
extern U64 KING_ATTACK_MASKS[64];
extern U64 RANK_ATTACKS[64];
extern U64 FILE_ATTACKS[64];
extern U64 DIAG_A8H1_ATTACKS[64];
extern U64 DIAG_A1H8_ATTACKS[64];

void initPieceMasks();
void initPawnMasks();

#endif // TYPES_H