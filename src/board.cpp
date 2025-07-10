#include <cassert>
#include <iostream>
#include "board/board_utils.h"

PieceType getPieceTypeAt(const Bitboard& board, Square sq);
Color getPieceColorAt(const Bitboard& board, Square sq);

void addPiece(Bitboard &board, PieceType type, Color color, Square sq) {
  U64 sq_bb = squareToBitboard(sq);

  board.byColor[color] |= sq_bb;
  board.byType[type] |= sq_bb;
  board.allPieces |= sq_bb;
  board.emptySquares &= ~sq_bb;

  switch (type) {
    case PAWN:
      board.pawns[color] |= sq_bb;
      break;
    case KNIGHT:
      board.knights[color] |= sq_bb;
      break;
    case BISHOP:
      board.bishops[color] |= sq_bb;
      break;
    case ROOK:
      board.rooks[color] |= sq_bb;
      break;
    case QUEEN:
      board.queens[color] |= sq_bb;
      break;
    case KING:
      board.kings[color] |= sq_bb;
      break;
    default:
      break;
  }
}

void removePiece(Bitboard &board, PieceType type, Color color, Square sq) {
  U64 sq_bb = squareToBitboard(sq);

  board.byColor[color] &= ~sq_bb;
  board.byType[type] &= ~sq_bb;
  board.allPieces &= ~sq_bb;
  board.emptySquares |= sq_bb;

  switch (type) {
    case PAWN:
      board.pawns[color] &= ~sq_bb;
      break;
    case KNIGHT:
      board.knights[color] &= ~sq_bb;
      break;
    case BISHOP:
      board.bishops[color] &= ~sq_bb;
      break;
    case ROOK:
      board.rooks[color] &= ~sq_bb;
      break;
    case QUEEN:
      board.queens[color] &= ~sq_bb;
      break;
    case KING:
      board.kings[color] &= ~sq_bb;
      break;
    default:
      break;
  }
}

void makeMove(GameState &state, const Move &move) {
  Color us = state.currentTurn;
  Color them = (us == WHITE) ? BLACK : WHITE;

  PieceType movingPieceType = getPieceTypeAt(state.board, move.from);
  Color movingPieceColor = getPieceColorAt(state.board, move.from);

  assert(movingPieceType != NONE && movingPieceColor == us);

  if (move.isCapture) {
    PieceType capturedPieceType = getPieceTypeAt(state.board, move.to);

    if (move.isEnPassant) {
      Square capturedPawnSq = (us == WHITE) ? move.to - 8 : move.to + 8;
      removePiece(state.board, PAWN, them, capturedPawnSq);
    } else {
      removePiece(state.board, capturedPieceType, them, move.to);
    }
  }

  removePiece(state.board, movingPieceType, us, move.from);

  if (move.promotionPieceType != NONE) {
    addPiece(state.board, move.promotionPieceType, us, move.to);
  } else {
    addPiece(state.board, movingPieceType, us, move.to);
  }

  if (movingPieceType == KING) {
    if (us == WHITE) {
      state.castlingRights &= ~(CastlingRights::WK | CastlingRights::WQ);
      state.whiteKingSquare = move.to;
    } else {
      state.castlingRights &= ~(CastlingRights::BK | CastlingRights::BQ);
      state.blackKingSquare = move.to;
    }
  }

  if (movingPieceType == ROOK) {
    if (move.from == H1) state.castlingRights &= ~CastlingRights::WK;
    if (move.from == A1) state.castlingRights &= ~CastlingRights::WQ;
    if (move.from == H8) state.castlingRights &= ~CastlingRights::BK;
    if (move.from == A8) state.castlingRights &= ~CastlingRights::BQ;
  }

  if (move.isCapture && getPieceTypeAt(state.board, move.to) == ROOK) {
    if (move.to == H1) state.castlingRights &= ~CastlingRights::WK;
    if (move.to == A1) state.castlingRights &= ~CastlingRights::WQ;
    if (move.to == H8) state.castlingRights &= ~CastlingRights::BK;
    if (move.to == A8) state.castlingRights &= ~CastlingRights::BQ;
  }

  if (move.isCastling) {
    if (move.to == G1) {  // White Kingside
      removePiece(state.board, ROOK, WHITE, H1);
      addPiece(state.board, ROOK, WHITE, F1);
    } else if (move.to == C1) {  // White Queenside
      removePiece(state.board, ROOK, WHITE, A1);
      addPiece(state.board, ROOK, WHITE, D1);
    } else if (move.to == G8) {  // Black Kingside
      removePiece(state.board, ROOK, BLACK, H8);
      addPiece(state.board, ROOK, BLACK, F8);
    } else if (move.to == C8) {  // Black Queenside
      removePiece(state.board, ROOK, BLACK, A8);
      addPiece(state.board, ROOK, BLACK, D8);
    }
  }

  if (movingPieceType == PAWN || move.isCapture) {
    state.halfMoveClock = 0;
  } else {
    state.halfMoveClock++;
  }

  if (us == BLACK) {
    state.fullMoveNumber++;
  }

  state.currentTurn = them;

  state.zobristKey =
      0ULL;  // Update Zobrist key here (not implemented in this snippet)
}

bool isKingInCheck(Color kingColor, const GameState &state) {
  Square kingSquare =
      (kingColor == WHITE) ? state.whiteKingSquare : state.blackKingSquare;
  Color opponentColor = (kingColor == WHITE) ? BLACK : WHITE;
  return isSquareAttacked(state, kingSquare, opponentColor);
}

void initializeStartingBoard(GameState &state) {
  state.board = Bitboard();

  addPiece(state.board, ROOK, WHITE, A1);
  addPiece(state.board, ROOK, WHITE, H1);
  addPiece(state.board, KNIGHT, WHITE, B1);
  addPiece(state.board, KNIGHT, WHITE, G1);
  addPiece(state.board, BISHOP, WHITE, C1);
  addPiece(state.board, BISHOP, WHITE, F1);
  addPiece(state.board, QUEEN, WHITE, D1);
  addPiece(state.board, KING, WHITE, E1);

  addPiece(state.board, PAWN, WHITE, A2);
  addPiece(state.board, PAWN, WHITE, B2);
  addPiece(state.board, PAWN, WHITE, C2);
  addPiece(state.board, PAWN, WHITE, D2);
  addPiece(state.board, PAWN, WHITE, E2);
  addPiece(state.board, PAWN, WHITE, F2);
  addPiece(state.board, PAWN, WHITE, G2);
  addPiece(state.board, PAWN, WHITE, H2);

  addPiece(state.board, PAWN, BLACK, A7);
  addPiece(state.board, PAWN, BLACK, B7);
  addPiece(state.board, PAWN, BLACK, C7);
  addPiece(state.board, PAWN, BLACK, D7);
  addPiece(state.board, PAWN, BLACK, E7);
  addPiece(state.board, PAWN, BLACK, F7);
  addPiece(state.board, PAWN, BLACK, G7);
  addPiece(state.board, PAWN, BLACK, H7);

  addPiece(state.board, ROOK, BLACK, A8);
  addPiece(state.board, ROOK, BLACK, H8);
  addPiece(state.board, KNIGHT, BLACK, B8);
  addPiece(state.board, KNIGHT, BLACK, G8);
  addPiece(state.board, BISHOP, BLACK, C8);
  addPiece(state.board, BISHOP, BLACK, F8);
  addPiece(state.board, QUEEN, BLACK, D8);
  addPiece(state.board, KING, BLACK, E8);

  state.currentTurn = WHITE;
  state.halfMoveClock = 0;
  state.fullMoveNumber = 1;
  state.castlingRights = CastlingRights::ALL;
  state.enPassantSquare = NO_SQUARE;
  state.zobristKey = 0ULL;
  state.whiteKingSquare = E1;
  state.blackKingSquare = E8;
}

bool isSquareAttacked(const GameState &state, Square sq, Color attackingColor) {
  U64 allPieces = state.board.allPieces;

  if (attackingColor == WHITE) {
    if (sq >= 9 && PAWN_ATTACK_MASKS[BLACK][sq] & state.board.pawns[WHITE]) {
      return true;
    }
  } else {
    if (sq <= 54 && PAWN_ATTACK_MASKS[WHITE][sq] & state.board.pawns[BLACK]) {
      return true;
    }
  }

  if ((KNIGHT_ATTACK_MASKS[sq] & state.board.knights[attackingColor]) != 0ULL) {
    return true;
  }

  if ((KING_ATTACK_MASKS[sq] & state.board.kings[attackingColor]) != 0ULL) {
    return true;
  }

  U64 rook_queen_attacks_from_sq = getSliderAttacks(sq, allPieces, ROOK);
  if ((rook_queen_attacks_from_sq & (state.board.rooks[attackingColor] |
                                     state.board.queens[attackingColor])) !=
      0ULL) {
    return true;
  }

  U64 bishop_queen_attacks_from_sq = getSliderAttacks(sq, allPieces, BISHOP);
  if ((bishop_queen_attacks_from_sq & (state.board.bishops[attackingColor] |
                                       state.board.queens[attackingColor])) !=
      0ULL) {
    return true;
  }

  return false;
}

bool isLegalMove(const GameState &state, const Move &move) {
  Color us = state.currentTurn;
  Color them = (us == WHITE) ? BLACK : WHITE;

  if (move.isCastling) {
    Square kingSq = (us == WHITE) ? E1 : E8;
    Square kingPassSq = NO_SQUARE;
    Square kingEndSq = NO_SQUARE;

    if (us == WHITE) {
      if (move.to == G1) {
        kingPassSq = F1;
        kingEndSq = G1;
      } else if (move.to == C1) {
        kingPassSq = D1;
        kingEndSq = C1;
      }
    } else {
      if (move.to == G8) {
        kingPassSq = F8;
        kingEndSq = G8;
      } else if (move.to == C8) {
        kingPassSq = D8;
        kingEndSq = C8;
      }
    }

    if (isSquareAttacked(state, kingSq, them)) {
      return false;
    }
    if (isSquareAttacked(state, kingPassSq, them)) {
      return false;
    }
    if (isSquareAttacked(state, kingEndSq, them)) {
      return false;
    } 
  }

  GameState tempState = state;
  makeMove(tempState, move);

  if (isKingInCheck(us, tempState)) {
    return false;
  }

  return true;
}
