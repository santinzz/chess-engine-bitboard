#include "board/board_utils.h"

U64 getSliderAttacks(Square fromSq, U64 blockers, PieceType type) {
  U64 attacks = 0ULL;
  int r = getRank(fromSq);
  int f = getFile(fromSq);

  if (type == ROOK || type == QUEEN) {
    for (int i = r + 1; i < 8; ++i) {
      Square targetSq = i * 8 + f;
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = r - 1; i >= 0; --i) {
      Square targetSq = i * 8 + f;
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = f + 1; i < 8; ++i) {
      Square targetSq = r * 8 + i;
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = f - 1; i >= 0; --i) {
      Square targetSq = r * 8 + i;
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
  }

  if (type == BISHOP || type == QUEEN) {
    for (int i = 1; r + i < 8 && f + i < 8; ++i) {
      Square targetSq = (r + i) * 8 + (f + i);
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = 1; r + i < 8 && f - i >= 0; ++i) {
      Square targetSq = (r + i) * 8 + (f - i);
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = 1; r - i >= 0 && f + i < 8; ++i) {
      Square targetSq = (r - i) * 8 + (f + i);
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
    for (int i = 1; r - i >= 0 && f - i >= 0; ++i) {
      Square targetSq = (r - i) * 8 + (f - i);
      attacks |= squareToBitboard(targetSq);
      if (blockers & squareToBitboard(targetSq)) break;
    }
  }

  return attacks;
}