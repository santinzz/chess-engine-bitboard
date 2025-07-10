#include "types/types.h"

const U64 RANK_MASKS[8] = {
  0xFFULL, // RANK_1
  0xFF00ULL << 8, // RANK_2
  0xFF0000ULL << 16, // RANK_3
  0xFF000000ULL << 24, // RANK_4
  0xFF00000000ULL << 32, // RANK_5
  0xFF0000000000ULL << 40, // RANK_6
  0xFF000000000000ULL << 48, // RANK_7
  0xFF00000000000000ULL << 56, // RANK_8
};

const U64 FILE_MASKS[8] = {
  0x0101010101010101ULL, // FILE_A
  0x0202020202020202ULL << 1, // FILE_B
  0x0404040404040404ULL << 2, // FILE_C
  0x0808080808080808ULL << 3, // FILE_D
  0x1010101010101010ULL << 4, // FILE_E
  0x2020202020202020ULL << 5, // FILE_F
  0x4040404040404040ULL << 6, // FILE_G
  0x8080808080808080ULL << 7, // FILE_H
};

U64 PAWN_PUSH_MASKS[2][64];
U64 PAWN_ATTACK_MASKS[2][64];

void initPawnMasks() {
  for (Square sq = A1; sq <= H8; ++sq) {
    U64 sq_bb = squareToBitboard(sq);

    PAWN_PUSH_MASKS[WHITE][sq] = 0ULL;
    PAWN_ATTACK_MASKS[WHITE][sq] = 0ULL;

    if (getRank(sq) < 7) {
      PAWN_PUSH_MASKS[WHITE][sq] |= (sq_bb << 8); // Push forward
    }
    if (getRank(sq) == 1) {
      PAWN_PUSH_MASKS[WHITE][sq] |= (sq_bb << 16); // Double push from rank 2
    }
    if (getFile(sq) > 0 && getRank(sq) < 7) {
      PAWN_ATTACK_MASKS[WHITE][sq] |= (sq_bb << 7); // Attack left
    }
    if (getFile(sq) < 7 && getRank(sq) < 7) {
      PAWN_ATTACK_MASKS[WHITE][sq] |= (sq_bb << 9); // Attack right
    }

    PAWN_PUSH_MASKS[BLACK][sq] = 0ULL;
    PAWN_ATTACK_MASKS[BLACK][sq] = 0ULL;

    if (getRank(sq) > 0) {
      PAWN_PUSH_MASKS[BLACK][sq] |= (sq_bb >> 8); // Push forward
    }
    if (getRank(sq) == 6) {
      PAWN_PUSH_MASKS[BLACK][sq] |= (sq_bb >> 16); // Double push from rank 7
    }
    if (getFile(sq) > 0 && getRank(sq) > 0) {
      PAWN_ATTACK_MASKS[BLACK][sq] |= (sq_bb >> 9); // Attack left
    }
    if (getFile(sq) < 7 && getRank(sq) > 0) {
      PAWN_ATTACK_MASKS[BLACK][sq] |= (sq_bb >> 7); // Attack right
    }
  }
}
