#include "board/board_utils.h"

U64 RANK_ATTACKS[64];
U64 FILE_ATTACKS[64];
U64 DIAG_A8H1_ATTACKS[64];
U64 DIAG_A1H8_ATTACKS[64];
U64 ANTI_DIAG_MASKS[15];
U64 MAIN_DIAG_MASKS[15];
U64 KNIGHT_ATTACK_MASKS[64];
U64 KING_ATTACK_MASKS[64];

void initPieceMasks()
{
  initPawnMasks();
  for (int i = 0; i < 15; ++i)
  {
    ANTI_DIAG_MASKS[i] = 0ULL;
    MAIN_DIAG_MASKS[i] = 0ULL;
  }

  for (Square sq = A1; sq <= H8; ++sq)
  {
    U64 sq_bb = squareToBitboard(sq);

    KNIGHT_ATTACK_MASKS[sq] = 0ULL;
    KING_ATTACK_MASKS[sq] = 0ULL;

    int r = getRank(sq);
    int f = getFile(sq);

    int knight_dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int knight_df[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; ++i)
    {
      int new_r = r + knight_dr[i];
      int new_f = f + knight_df[i];

      if (new_r >= 0 && new_r < 8 && new_f >= 0 && new_f < 8)
      {
        KNIGHT_ATTACK_MASKS[sq] |= squareToBitboard(new_r * 8 + new_f);
      }
    }

    // King attacks
    int king_dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int king_df[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i)
    {
      int new_r = r + king_dr[i];
      int new_f = f + king_df[i];

      if (new_r >= 0 && new_r < 8 && new_f >= 0 && new_f < 8)
      {
        KING_ATTACK_MASKS[sq] |= squareToBitboard(new_r * 8 + new_f);
      }
    }

    RANK_ATTACKS[sq] = RANK_MASKS[getRank(sq)];
    FILE_ATTACKS[sq] = FILE_MASKS[getFile(sq)];

    MAIN_DIAG_MASKS[getMainDiagIndex(sq)] |= sq_bb;
    ANTI_DIAG_MASKS[getAntiDiagIndex(sq)] |= sq_bb;

    DIAG_A8H1_ATTACKS[sq] = MAIN_DIAG_MASKS[getMainDiagIndex(sq)];
    DIAG_A1H8_ATTACKS[sq] = ANTI_DIAG_MASKS[getAntiDiagIndex(sq)];
  }

  for (int i = A1; i <= H8; ++i)
  {
    DIAG_A8H1_ATTACKS[i] = MAIN_DIAG_MASKS[getMainDiagIndex(i)];
    DIAG_A1H8_ATTACKS[i] = ANTI_DIAG_MASKS[getAntiDiagIndex(i)];
  }
}