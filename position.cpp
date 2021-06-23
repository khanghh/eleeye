/*
position.h/position.cpp - Source Code for ElephantEye, Part III

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.0, Last Modified: Nov. 2007
Copyright (C) 2004-2007 www.elephantbase.net

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "base/base.h"
#include "pregen.h"
#include "position.h"

/* ElephantEyeԴ����ʹ�õ��������Ǻ�Լ����
 *
 * sq: �������(��������0��255������"pregen.cpp")
 * pc: �������(��������0��47������"position.cpp")
 * pt: �����������(��������0��6������"position.cpp")
 * mv: �ŷ�(��������0��65535������"position.cpp")
 * sd: ���ӷ�(������0�����췽��1�����ڷ�)
 * vl: �����ֵ(��������"-MATE_VALUE"��"MATE_VALUE"������"position.cpp")
 * (ע����������Ǻſ���uc��dw�ȴ��������ļǺ����ʹ��)
 * pos: ����(PositionStruct���ͣ�����"position.h")
 * sms: λ�к�λ�е��ŷ�����Ԥ�ýṹ(����"pregen.h")
 * smv: λ�к�λ�е��ŷ��ж�Ԥ�ýṹ(����"pregen.h")
 */

// ��ģ���漰���"PositionStruct"�ĳ�Ա����"this->"��ǳ����Է������

// ��ʼ�����FEN��
const char *const cszStartFen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";

// �������Ͷ�Ӧ�����ӷ���
const char *const cszPieceBytes = "KABNRCP";

/* ������Ŷ�Ӧ����������
 *
 * ElephantEye��������Ŵ�0��47������0��15���ã�16��31��ʾ���ӣ�32��47��ʾ���ӡ�
 * ÿ��������˳�������ǣ�˧�������������������ڱ���������(��ʿʿ��������������������������)
 * ��ʾ���ж������Ǻ�����"pc < 32"��������"pc >= 32"
 */
const int cnPieceTypes[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// ���ӵļ򵥷�ֵ��ֻ�ڼ򵥱Ƚ�ʱ���ο�
const int cnSimpleValues[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
  5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
};

// ������ܷ����ʵ��������ľ���(���ҶԳ�)
const uint8_t cucsqMirrorTab[256] = {
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
  0, 0, 0, 0x3b, 0x3a, 0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0, 0, 0, 0,
  0, 0, 0, 0x4b, 0x4a, 0x49, 0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0, 0, 0, 0,
  0, 0, 0, 0x5b, 0x5a, 0x59, 0x58, 0x57, 0x56, 0x55, 0x54, 0x53, 0, 0, 0, 0,
  0, 0, 0, 0x6b, 0x6a, 0x69, 0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0, 0, 0, 0,
  0, 0, 0, 0x7b, 0x7a, 0x79, 0x78, 0x77, 0x76, 0x75, 0x74, 0x73, 0, 0, 0, 0,
  0, 0, 0, 0x8b, 0x8a, 0x89, 0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0, 0, 0, 0,
  0, 0, 0, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96, 0x95, 0x94, 0x93, 0, 0, 0, 0,
  0, 0, 0, 0xab, 0xaa, 0xa9, 0xa8, 0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0, 0, 0, 0,
  0, 0, 0, 0xbb, 0xba, 0xb9, 0xb8, 0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 0, 0, 0, 0,
  0, 0, 0, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6, 0xc5, 0xc4, 0xc3, 0, 0, 0, 0,
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
  0, 0, 0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0, 0, 0, 0,
};

// FEN�������ӱ�ʶ��ע���������ֻ��ʶ���д��ĸ�������Сд��ĸʱ�����ȱ���ת��Ϊ��д
int FenPiece(int nArg) {
  switch (nArg) {
  case 'K':
    return 0;
  case 'A':
    return 1;
  case 'B':
  case 'E':
    return 2;
  case 'N':
  case 'H':
    return 3;
  case 'R':
    return 4;
  case 'C':
    return 5;
  case 'P':
    return 6;
  default:
    return 7;
  }
}

// ������һЩ���̴�������

// ��������������
void PositionStruct::AddPiece(int sq, int pc, bool bDel) {
  int pt;

  __ASSERT_SQUARE(sq);
  __ASSERT_PIECE(pc);
  if (bDel) {
    this->ucpcSquares[sq] = 0;
    this->ucsqPieces[pc] = 0;
  } else {
    this->ucpcSquares[sq] = pc;
    this->ucsqPieces[pc] = sq;
  }
  this->wBitRanks[RANK_Y(sq)] ^= PreGen.wBitRankMask[sq];
  this->wBitFiles[FILE_X(sq)] ^= PreGen.wBitFileMask[sq];
  __ASSERT_BITRANK(this->wBitRanks[RANK_Y(sq)]);
  __ASSERT_BITFILE(this->wBitRanks[FILE_X(sq)]);
  this->dwBitPiece ^= BIT_PIECE(pc);
  pt = PIECE_TYPE(pc);
  if (pc < 32) {
    if (bDel) {
      this->vlWhite -= PreEval.ucvlWhitePieces[pt][sq];
    } else {
      this->vlWhite += PreEval.ucvlWhitePieces[pt][sq];
    }
  } else {
    if (bDel) {
      this->vlBlack -= PreEval.ucvlBlackPieces[pt][sq];
    } else {
      this->vlBlack += PreEval.ucvlBlackPieces[pt][sq];
    }
    pt += 7;
  }
  __ASSERT_BOUND(0, pt, 13);
  this->zobr.Xor(PreGen.zobrTable[pt][sq]);
}

// �ƶ�����
int PositionStruct::MovePiece(int mv) {
  int sqSrc, sqDst, pcMoved, pcCaptured, pt;
  uint8_t *lpucvl;
  // �ƶ����Ӱ������¼������裺

  // 1. �õ��ƶ���������źͱ��Ե�������ţ�
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pcMoved = this->ucpcSquares[sqSrc];
  __ASSERT_SQUARE(sqSrc);
  __ASSERT_SQUARE(sqDst);
  __ASSERT_PIECE(pcMoved);
  pcCaptured = this->ucpcSquares[sqDst];
  if (pcCaptured == 0) {

    // 2. ���û�б��Ե����ӣ���ô����Ŀ����λ�к�λ�С�
    //    ���仰˵���б��Ե����ӣ�Ŀ����λ�к�λ�оͲ��ظ����ˡ�
    this->wBitRanks[RANK_Y(sqDst)] ^= PreGen.wBitRankMask[sqDst];
    this->wBitFiles[FILE_X(sqDst)] ^= PreGen.wBitFileMask[sqDst];
    __ASSERT_BITRANK(this->wBitRanks[RANK_Y(sqDst)]);
    __ASSERT_BITFILE(this->wBitRanks[FILE_X(sqDst)]);
  } else {

    __ASSERT_PIECE(pcCaptured);
    // 3. ����б��Ե����ӣ���ô��������λ����"ucsqPieces"�����������������
    //    ͬʱ����������ֵ��λ��λ�С�Zobrist��ֵ��У����
    this->ucsqPieces[pcCaptured] = 0;
    this->dwBitPiece ^= BIT_PIECE(pcCaptured);
    pt = PIECE_TYPE(pcCaptured);
    if (pcCaptured < 32) {
      this->vlWhite -= PreEval.ucvlWhitePieces[pt][sqDst];
    } else {
      this->vlBlack -= PreEval.ucvlBlackPieces[pt][sqDst];
      pt += 7;
    }
    __ASSERT_BOUND(0, pt, 13);
    this->zobr.Xor(PreGen.zobrTable[pt][sqDst]);
  }

  // 4. ��"ucpcSquares"��"ucsqPieces"�������ƶ����ӣ�ע�⡰����-������ϵ���顱�ƶ����ӵķ���
  //    ͬʱ����λ�С�λ�С�������ֵ��λ��λ�С�Zobrist��ֵ��У����
  this->ucpcSquares[sqSrc] = 0;
  this->ucpcSquares[sqDst] = pcMoved;
  this->ucsqPieces[pcMoved] = sqDst;
  this->wBitRanks[RANK_Y(sqSrc)] ^= PreGen.wBitRankMask[sqSrc];
  this->wBitFiles[FILE_X(sqSrc)] ^= PreGen.wBitFileMask[sqSrc];
  __ASSERT_BITRANK(this->wBitRanks[RANK_Y(sqSrc)]);
  __ASSERT_BITFILE(this->wBitRanks[FILE_X(sqSrc)]);
  pt = PIECE_TYPE(pcMoved);
  if (pcMoved < 32) {
    lpucvl = PreEval.ucvlWhitePieces[pt];
    this->vlWhite += lpucvl[sqDst] - lpucvl[sqSrc];
  } else {
    lpucvl = PreEval.ucvlBlackPieces[pt];
    this->vlBlack += lpucvl[sqDst] - lpucvl[sqSrc];
    pt += 7;
  }
  __ASSERT_BOUND(0, pt, 13);
  this->zobr.Xor(PreGen.zobrTable[pt][sqDst], PreGen.zobrTable[pt][sqSrc]);
  return pcCaptured;
}

// �����ƶ�����
void PositionStruct::UndoMovePiece(int mv, int pcCaptured) {
  int sqSrc, sqDst, pcMoved;
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pcMoved = this->ucpcSquares[sqDst];
  __ASSERT_SQUARE(sqSrc);
  __ASSERT_SQUARE(sqDst);
  __ASSERT_PIECE(pcMoved);
  this->ucpcSquares[sqSrc] = pcMoved;
  this->ucsqPieces[pcMoved] = sqSrc;
  this->wBitRanks[RANK_Y(sqSrc)] ^= PreGen.wBitRankMask[sqSrc];
  this->wBitFiles[FILE_X(sqSrc)] ^= PreGen.wBitFileMask[sqSrc];
  __ASSERT_BITRANK(this->wBitRanks[RANK_Y(sqSrc)]);
  __ASSERT_BITFILE(this->wBitRanks[FILE_X(sqSrc)]);
  if (pcCaptured > 0) {
    __ASSERT_PIECE(pcCaptured);
    this->ucpcSquares[sqDst] = pcCaptured;
    this->ucsqPieces[pcCaptured] = sqDst;
    this->dwBitPiece ^= BIT_PIECE(pcCaptured);
  } else {
    this->ucpcSquares[sqDst] = 0;
    this->wBitRanks[RANK_Y(sqDst)] ^= PreGen.wBitRankMask[sqDst];
    this->wBitFiles[FILE_X(sqDst)] ^= PreGen.wBitFileMask[sqDst];
    __ASSERT_BITRANK(this->wBitRanks[RANK_Y(sqDst)]);
    __ASSERT_BITFILE(this->wBitRanks[FILE_X(sqDst)]);
  }
}

// ����
int PositionStruct::Promote(int sq) {
  int pcCaptured, pcPromoted, pt;
  // ����������¼������裺

  // 1. �õ�����ǰ���ӵ���ţ�
  __ASSERT_SQUARE(sq);
  __ASSERT(CanPromote());
  __ASSERT(CAN_PROMOTE(sq));
  pcCaptured = this->ucpcSquares[sq];
  __ASSERT_PIECE(pcCaptured);
  pcPromoted = SIDE_TAG(this->sdPlayer) + Bsf(~this->wBitPiece[this->sdPlayer] & PAWN_BITPIECE);
  __ASSERT_PIECE(pcPromoted);
  __ASSERT(this->ucsqPieces[pcPromoted] == 0);

  // 2. ȥ������ǰ���ӣ�ͬʱ����������ֵ��Zobrist��ֵ��У����
  this->dwBitPiece ^= BIT_PIECE(pcPromoted) ^ BIT_PIECE(pcCaptured);
  this->ucsqPieces[pcCaptured] = 0;
  pt = PIECE_TYPE(pcCaptured);
  if (pcCaptured < 32) {
    this->vlWhite -= PreEval.ucvlWhitePieces[pt][sq];
  } else {
    this->vlBlack -= PreEval.ucvlBlackPieces[pt][sq];
    pt += 7;
  }
  __ASSERT_BOUND(0, pt, 13);
  this->zobr.Xor(PreGen.zobrTable[pt][sq]);

  // 3. ������������ӣ�ͬʱ����������ֵ��Zobrist��ֵ��У����
  this->ucpcSquares[sq] = pcPromoted;
  this->ucsqPieces[pcPromoted] = sq;
  pt = PIECE_TYPE(pcPromoted);
  if (pcPromoted < 32) {
    this->vlWhite += PreEval.ucvlWhitePieces[pt][sq];
  } else {
    this->vlBlack += PreEval.ucvlBlackPieces[pt][sq];
    pt += 7;
  }
  __ASSERT_BOUND(0, pt, 13);
  this->zobr.Xor(PreGen.zobrTable[pt][sq]);
  return pcCaptured;
}

// ��������
void PositionStruct::UndoPromote(int sq, int pcCaptured) {
  int pcPromoted;
  __ASSERT_SQUARE(sq);
  __ASSERT_PIECE(pcCaptured);
  pcPromoted = this->ucpcSquares[sq];
  __ASSERT(PIECE_TYPE(pcPromoted) == 6);
  this->ucsqPieces[pcPromoted] = 0;
  this->ucpcSquares[sq] = pcCaptured;
  this->ucsqPieces[pcCaptured] = sq;
  this->dwBitPiece ^= BIT_PIECE(pcPromoted) ^ BIT_PIECE(pcCaptured);
}

// ������һЩ���̴�������

// ������һЩ�ŷ���������

// ִ��һ���ŷ�
bool PositionStruct::MakeMove(int mv) {
  int sq, pcCaptured;
  uint32_t dwOldZobristKey;
  RollbackStruct *lprbs;

  // ����ﵽ����ŷ�������ô�ж�Ϊ�Ƿ��ŷ�
  if (this->nMoveNum == MAX_MOVE_NUM) {
    return false;
  }
  __ASSERT(this->nMoveNum < MAX_MOVE_NUM);
  // ִ��һ���ŷ�Ҫ�������¼������裺

  // 1. ����ԭ����Zobrist��ֵ
  dwOldZobristKey = this->zobr.dwKey;
  SaveStatus();

  // 2. �ƶ����ӣ���ס�Ե�����(����еĻ�)
  sq = SRC(mv);
  if (sq == DST(mv)) {
    pcCaptured = Promote(sq);
  } else {
    pcCaptured = MovePiece(mv);

    // 3. ����ƶ��󱻽����ˣ���ô�ŷ��ǷǷ��ģ��������ŷ�
    if (CheckedBy(CHECK_LAZY) > 0) {
      UndoMovePiece(mv, pcCaptured);
      Rollback();
      return false;
    }
  }

  // 4. �������ӷ�
  ChangeSide();

  // 5. ��ԭ����Zobrist��ֵ��¼������ظ��������û�����
  if (this->ucRepHash[dwOldZobristKey & REP_HASH_MASK] == 0) {
    this->ucRepHash[dwOldZobristKey & REP_HASH_MASK] = this->nMoveNum;
  }

  // 6. ���ŷ����浽��ʷ�ŷ��б��У�����ס�Ե����Ӻͽ���״̬
  lprbs = this->rbsList + this->nMoveNum;
  lprbs->mvs.wmv = mv;
  lprbs->mvs.ChkChs = CheckedBy();

  // 7. ���ú����ŷ���(������Ӧ��������)
  if (pcCaptured == 0) {
    if (lprbs->mvs.ChkChs == 0) {
      lprbs->mvs.ChkChs = -ChasedBy(mv);
    }
    if (LastMove().CptDrw == -100) {
      lprbs->mvs.CptDrw = -100;
    } else {
      lprbs->mvs.CptDrw = MIN((int) LastMove().CptDrw, 0) - (lprbs->mvs.ChkChs > 0 || LastMove().ChkChs > 0 ? 0 : 1);
    }
    __ASSERT_BOUND(-100, lprbs->mvs.CptDrw, 0);
  } else {
    lprbs->mvs.CptDrw = pcCaptured;
    __ASSERT_PIECE(pcCaptured);
  }
  this->nMoveNum ++;
  this->nDistance ++;

  return true;
}

// ����һ���ŷ�
void PositionStruct::UndoMakeMove(void) {
  int sq;
  RollbackStruct *lprbs;
  this->nMoveNum --;
  this->nDistance --;
  lprbs = this->rbsList + this->nMoveNum;
  sq = SRC(lprbs->mvs.wmv);
  if (sq == DST(lprbs->mvs.wmv)) {
    __ASSERT_BOUND(ADVISOR_TYPE, PIECE_TYPE(lprbs->mvs.CptDrw), BISHOP_TYPE);
    UndoPromote(sq, lprbs->mvs.CptDrw);
  } else {
    UndoMovePiece(lprbs->mvs.wmv, lprbs->mvs.CptDrw);
  }
  this->sdPlayer = OPP_SIDE(this->sdPlayer);
  Rollback();
  if (this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] == this->nMoveNum) {
    this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] = 0;
  }
  __ASSERT(this->nMoveNum > 0);
}

// ִ��һ������
void PositionStruct::NullMove(void) {
  __ASSERT(this->nMoveNum < MAX_MOVE_NUM);
  if (this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] == 0) {
    this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] = this->nMoveNum;
  }
  SaveStatus();
  ChangeSide();
  this->rbsList[nMoveNum].mvs.dwmv = 0; // wmv, Chk, CptDrw, ChkChs = 0
  this->nMoveNum ++;
  this->nDistance ++;
}

// ����һ������
void PositionStruct::UndoNullMove(void) {
  this->nMoveNum --;
  this->nDistance --;
  this->sdPlayer = OPP_SIDE(this->sdPlayer);
  Rollback();
  if (this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] == this->nMoveNum) {
    this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] = 0;
  }
  __ASSERT(this->nMoveNum > 0);
}

// ������һЩ�ŷ���������

// ������һЩ���洦������

// FEN��ʶ��
void PositionStruct::FromFen(const char *szFen) {
  int i, j, k;
  int pcWhite[7];
  int pcBlack[7];
  const char *lpFen;
  // FEN����ʶ��������¼������裺
  // 1. ��ʼ�����������
  pcWhite[0] = SIDE_TAG(0) + KING_FROM;
  pcWhite[1] = SIDE_TAG(0) + ADVISOR_FROM;
  pcWhite[2] = SIDE_TAG(0) + BISHOP_FROM;
  pcWhite[3] = SIDE_TAG(0) + KNIGHT_FROM;
  pcWhite[4] = SIDE_TAG(0) + ROOK_FROM;
  pcWhite[5] = SIDE_TAG(0) + CANNON_FROM;
  pcWhite[6] = SIDE_TAG(0) + PAWN_FROM;
  for (i = 0; i < 7; i ++) {
    pcBlack[i] = pcWhite[i] + 16;
  }
  /* ����"pcWhite[7]"��"pcBlack[7]"�ֱ�����췽�ͺڷ�ÿ�����ּ���ռ�е���ţ�
   * ��"pcWhite[7]"Ϊ������������16��31���δ�����˧�������������������ڱ�������������
   * �������Ӧ����"pcWhite[7] = {16, 17, 19, 21, 23, 25, 27}"��ÿ����һ�����ӣ����������1��
   * ���������������Ӷ��������(�������ӵڶ���˧���ͱ������)��������ǰҪ���߽���
   */
  ClearBoard();
  lpFen = szFen;
  if (*lpFen == '\0') {
    SetIrrev();
    return;
  }
  // 2. ��ȡ�����ϵ�����
  i = RANK_TOP;
  j = FILE_LEFT;
  while (*lpFen != ' ') {
    if (*lpFen == '/') {
      j = FILE_LEFT;
      i ++;
      if (i > RANK_BOTTOM) {
        break;
      }
    } else if (*lpFen >= '1' && *lpFen <= '9') {
      j += (*lpFen - '0');
    } else if (*lpFen >= 'A' && *lpFen <= 'Z') {
      if (j <= FILE_RIGHT) {
        k = FenPiece(*lpFen);
        if (k < 7) {
          if (pcWhite[k] < 32) {
            if (this->ucsqPieces[pcWhite[k]] == 0) {
              AddPiece(COORD_XY(j, i), pcWhite[k]);
              pcWhite[k] ++;
            }
          }
        }
        j ++;
      }
    } else if (*lpFen >= 'a' && *lpFen <= 'z') {
      if (j <= FILE_RIGHT) {
        k = FenPiece(*lpFen + 'A' - 'a');
        if (k < 7) {
          if (pcBlack[k] < 48) {
            if (this->ucsqPieces[pcBlack[k]] == 0) {
              AddPiece(COORD_XY(j, i), pcBlack[k]);
              pcBlack[k] ++;
            }
          }
        }
        j ++;
      }
    }
    lpFen ++;
    if (*lpFen == '\0') {
      SetIrrev();
      return;
    }
  }
  lpFen ++;
  // 3. ȷ���ֵ��ķ���
  if (*lpFen == 'b') {
    ChangeSide();
  }
  // 4. �Ѿ�����ɡ������桱
  SetIrrev();
}

// ����FEN��
void PositionStruct::ToFen(char *szFen) const {
  int i, j, k, pc;
  char *lpFen;

  lpFen = szFen;
  for (i = RANK_TOP; i <= RANK_BOTTOM; i ++) {
    k = 0;
    for (j = FILE_LEFT; j <= FILE_RIGHT; j ++) {
      pc = this->ucpcSquares[COORD_XY(j, i)];
      if (pc != 0) {
        if (k > 0) {
          *lpFen = k + '0';
          lpFen ++;
          k = 0;
        }
        *lpFen = PIECE_BYTE(PIECE_TYPE(pc)) + (pc < 32 ? 0 : 'a' - 'A');
        lpFen ++;
      } else {
        k ++;
      }
    }
    if (k > 0) {
      *lpFen = k + '0';
      lpFen ++;
    }
    *lpFen = '/';
    lpFen ++;
  }
  *(lpFen - 1) = ' '; // �����һ��'/'�滻��' '
  *lpFen = (this->sdPlayer == 0 ? 'w' : 'b');
  lpFen ++;
  *lpFen = '\0';
}

// ���澵��
void PositionStruct::Mirror(void) {
  int i, sq, nMoveNumSave;
  uint16_t wmvList[MAX_MOVE_NUM];
  uint8_t ucsqList[32];
  // ���澵����Ҫ�����²������ν��У�

  // 1. ��¼������ʷ�ŷ�
  nMoveNumSave = this->nMoveNum;
  for (i = 1; i < nMoveNumSave; i ++) {
    wmvList[i] = this->rbsList[i].mvs.wmv;
  }

  // 2. ���������ŷ�
  for (i = 1; i < nMoveNumSave; i ++) {
    UndoMakeMove();
  }

  // 3. ���������Ӵ����������ߣ�λ�ü�¼��"ucsqList"���飻
  for (i = 16; i < 48; i ++) {
    sq = this->ucsqPieces[i];
    ucsqList[i - 16] = sq;
    if (sq != 0) {
      AddPiece(sq, i, DEL_PIECE);
    }
  }

  // 4. �����ߵ����Ӱ��վ���λ�����·ŵ������ϣ�
  for (i = 16; i < 48; i ++) {
    sq = ucsqList[i - 16];
    if (sq != 0) {
      AddPiece(SQUARE_MIRROR(sq), i);
    }
  }

  // 6. ��ԭ�����ŷ�
  SetIrrev();
  for (i = 1; i < nMoveNumSave; i ++) {
    MakeMove(MOVE_MIRROR(wmvList[i]));
  }
}

// ������һЩ���洦������

// ������һЩ�ŷ�������

// �ŷ������Լ�⣬�����ڡ�ɱ���ŷ����ļ����
bool PositionStruct::LegalMove(int mv) const {
  int sqSrc, sqDst, sqPin, pcMoved, pcCaptured, x, y, nSideTag;
  // �ŷ������Լ��������¼������裺

  // 1. ���Ҫ�ߵ����Ƿ����
  nSideTag = SIDE_TAG(this->sdPlayer);
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pcMoved = this->ucpcSquares[sqSrc];
  if ((pcMoved & nSideTag) == 0) {
    return false;
  }
  __ASSERT_SQUARE(sqSrc);
  __ASSERT_SQUARE(sqDst);
  __ASSERT_PIECE(pcMoved);

  // 2. ���Ե������Ƿ�Ϊ�Է�����(����г��Ӳ���û������Ļ�)
  pcCaptured = this->ucpcSquares[sqDst];
  if (sqSrc != sqDst && (pcCaptured & nSideTag) != 0) {
    return false;
  }
  __ASSERT_BOUND(0, PIECE_INDEX(pcMoved), 15);
  switch (PIECE_INDEX(pcMoved)) {

  // 3. �����˧(��)����(ʿ)�����ȿ��Ƿ��ھŹ��ڣ��ٿ��Ƿ��Ǻ���λ��
  case KING_FROM:
    return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);
  case ADVISOR_FROM:
  case ADVISOR_TO:
    if (sqSrc == sqDst) {
      // �������䣬�ڵ��߲��ұ�(��)��ȫʱ���ſ�����
      return CAN_PROMOTE(sqSrc) && CanPromote();
    } else {
      return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);
    }

  // 4. �������(��)�����ȿ��Ƿ���ӣ��ٿ��Ƿ��Ǻ���λ�ƣ������û�б�������
  case BISHOP_FROM:
  case BISHOP_TO:
    if (sqSrc == sqDst) {
      // �������䣬�ڵ��߲��ұ�(��)��ȫʱ���ſ�����
      return CAN_PROMOTE(sqSrc) && CanPromote();
    } else {
      return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) && this->ucpcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
    }

  // 5. ������������ȿ����Ƿ��Ǻ���λ�ƣ��ٿ���û�б�������
  case KNIGHT_FROM:
  case KNIGHT_TO:
    sqPin = KNIGHT_PIN(sqSrc, sqDst);
    return sqPin != sqSrc && this->ucpcSquares[sqPin] == 0;

  // 6. ����ǳ������ȿ��Ǻ����ƶ����������ƶ����ٶ�ȡλ�л�λ�е��ŷ�Ԥ��������
  case ROOK_FROM:
  case ROOK_TO:
    x = FILE_X(sqSrc);
    y = RANK_Y(sqSrc);
    if (x == FILE_X(sqDst)) {
      if (pcCaptured == 0) {
        return (FileMaskPtr(x, y)->wNonCap & PreGen.wBitFileMask[sqDst]) != 0;
      } else {
        return (FileMaskPtr(x, y)->wRookCap & PreGen.wBitFileMask[sqDst]) != 0;
      }
    } else if (y == RANK_Y(sqDst)) {
      if (pcCaptured == 0) {
        return (RankMaskPtr(x, y)->wNonCap & PreGen.wBitRankMask[sqDst]) != 0;
      } else {
        return (RankMaskPtr(x, y)->wRookCap & PreGen.wBitRankMask[sqDst]) != 0;
      }
    } else {
      return false;
    }

  // 7. ������ڣ��ж������ͳ�һ��
  case CANNON_FROM:
  case CANNON_TO:
    x = FILE_X(sqSrc);
    y = RANK_Y(sqSrc);
    if (x == FILE_X(sqDst)) {
      if (pcCaptured == 0) {
        return (FileMaskPtr(x, y)->wNonCap & PreGen.wBitFileMask[sqDst]) != 0;
      } else {
        return (FileMaskPtr(x, y)->wCannonCap & PreGen.wBitFileMask[sqDst]) != 0;
      }
    } else if (y == RANK_Y(sqDst)) {
      if (pcCaptured == 0) {
        return (RankMaskPtr(x, y)->wNonCap & PreGen.wBitRankMask[sqDst]) != 0;
      } else {
        return (RankMaskPtr(x, y)->wCannonCap & PreGen.wBitRankMask[sqDst]) != 0;
      }
    } else {
      return false;
    }

  // 8. ����Ǳ�(��)���򰴺췽�ͺڷ����������
  default:
    if (AWAY_HALF(sqDst, this->sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) {
      return true;
    } else {
      return sqDst == SQUARE_FORWARD(sqSrc, this->sdPlayer);
    }
  }
}

// �������
int PositionStruct::CheckedBy(bool bLazy) const {
  int pcCheckedBy, i, sqSrc, sqDst, sqPin, pc, x, y, nOppSideTag;
  SlideMaskStruct *lpsmsRank, *lpsmsFile;

  pcCheckedBy = 0;
  nOppSideTag = OPP_SIDE_TAG(this->sdPlayer);
  // �����жϰ������¼��������ݣ�

  // 1. �ж�˧(��)�Ƿ���������
  sqSrc = this->ucsqPieces[SIDE_TAG(this->sdPlayer)];
  if (sqSrc == 0) {
    return 0;
  }
  __ASSERT_SQUARE(sqSrc);

  // 2. ���˧(��)���ڸ��ӵ�λ�к�λ��
  x = FILE_X(sqSrc);
  y = RANK_Y(sqSrc);
  lpsmsRank = RankMaskPtr(x, y);
  lpsmsFile = FileMaskPtr(x, y);

  // 3. �ж��Ƿ�˧����
  sqDst = this->ucsqPieces[nOppSideTag + KING_FROM];
  if (sqDst != 0) {
    __ASSERT_SQUARE(sqDst);
    if (x == FILE_X(sqDst) && (lpsmsFile->wRookCap & PreGen.wBitFileMask[sqDst]) != 0) {
      return CHECK_MULTI;
    }
  }

  // 4. �ж��Ƿ�������
  for (i = KNIGHT_FROM; i <= KNIGHT_TO; i ++) {
    sqDst = this->ucsqPieces[nOppSideTag + i];
    if (sqDst != 0) {
      __ASSERT_SQUARE(sqDst);
      sqPin = KNIGHT_PIN(sqDst, sqSrc); // ע�⣬sqSrc��sqDst�Ƿ��ģ�
      if (sqPin != sqDst && this->ucpcSquares[sqPin] == 0) {
        if (bLazy || pcCheckedBy > 0) {
          return CHECK_MULTI;
        }
        pcCheckedBy = nOppSideTag + i;
        __ASSERT_PIECE(pcCheckedBy);
      }
    }
  }

  // 5. �ж��Ƿ񱻳�������˧����
  for (i = ROOK_FROM; i <= ROOK_TO; i ++) {
    sqDst = this->ucsqPieces[nOppSideTag + i];
    if (sqDst != 0) {
      __ASSERT_SQUARE(sqDst);
      if (x == FILE_X(sqDst)) {
        if ((lpsmsFile->wRookCap & PreGen.wBitFileMask[sqDst]) != 0) {
          if (bLazy || pcCheckedBy > 0) {
            return CHECK_MULTI;
          }
          pcCheckedBy = nOppSideTag + i;
          __ASSERT_PIECE(pcCheckedBy);
        }
      } else if (y == RANK_Y(sqDst)) {
        if ((lpsmsRank->wRookCap & PreGen.wBitRankMask[sqDst]) != 0) {
          if (bLazy || pcCheckedBy > 0) {
            return CHECK_MULTI;
          }
          pcCheckedBy = nOppSideTag + i;
          __ASSERT_PIECE(pcCheckedBy);
        }
      }
    }
  }

  // 6. �ж��Ƿ��ڽ���
  for (i = CANNON_FROM; i <= CANNON_TO; i ++) {
    sqDst = this->ucsqPieces[nOppSideTag + i];
    if (sqDst != 0) {
      __ASSERT_SQUARE(sqDst);
      if (x == FILE_X(sqDst)) {
        if ((lpsmsFile->wCannonCap & PreGen.wBitFileMask[sqDst]) != 0) {
          if (bLazy || pcCheckedBy > 0) {
            return CHECK_MULTI;
          }
          pcCheckedBy = nOppSideTag + i;
          __ASSERT_PIECE(pcCheckedBy);
        }
      } else if (y == RANK_Y(sqDst)) {
        if ((lpsmsRank->wCannonCap & PreGen.wBitRankMask[sqDst]) != 0) {
          if (bLazy || pcCheckedBy > 0) {
            return CHECK_MULTI;
          }
          pcCheckedBy = nOppSideTag + i;
          __ASSERT_PIECE(pcCheckedBy);
        }
      }
    }
  }

  // 7. �ж��Ƿ񱻱�(��)����
  for (sqDst = sqSrc - 1; sqDst <= sqSrc + 1; sqDst += 2) {
    // ���˧(��)�ڱ���(ElephantEye����)����ô���Բ�����
    // __ASSERT_SQUARE(sqDst);
    pc = this->ucpcSquares[sqDst];
    if ((pc & nOppSideTag) != 0 && PIECE_INDEX(pc) >= PAWN_FROM) {
      if (bLazy || pcCheckedBy > 0) {
        return CHECK_MULTI;
      }
      pcCheckedBy = nOppSideTag + i;
      __ASSERT_PIECE(pcCheckedBy);
    }
  }
  pc = this->ucpcSquares[SQUARE_FORWARD(sqSrc, this->sdPlayer)];
  if ((pc & nOppSideTag) != 0 && PIECE_INDEX(pc) >= PAWN_FROM) {
    if (bLazy || pcCheckedBy > 0) {
      return CHECK_MULTI;
    }
    pcCheckedBy = nOppSideTag + i;
    __ASSERT_PIECE(pcCheckedBy);
  }
  return pcCheckedBy;
}

// �ж��Ƿ񱻽���
bool PositionStruct::IsMate(void) {
  int i, nGenNum;
  MoveStruct mvsGen[MAX_GEN_MOVES];
  nGenNum = GenCapMoves(mvsGen);
  for (i = 0; i < nGenNum; i ++) {
    if (MakeMove(mvsGen[i].wmv)) {
      UndoMakeMove();
      return false;
    }
  }
  // �ŷ����ɷ������������������Խ�Լʱ��
  nGenNum = GenNonCapMoves(mvsGen);
  for (i = 0; i < nGenNum; i ++) {
    if (MakeMove(mvsGen[i].wmv)) {
      UndoMakeMove();
      return false;
    }
  }
  return true;
}

// ���ý���״̬λ
inline void SetPerpCheck(uint32_t &dwPerpCheck, int nChkChs) {
  if (nChkChs == 0) {
    dwPerpCheck = 0;
  } else if (nChkChs > 0) {
    dwPerpCheck &= 0x10000;
  } else {
    dwPerpCheck &= (1 << -nChkChs);
  }
}

// �ظ�������
int PositionStruct::RepStatus(int nRecur) const {
  // ����"nRecur"ָ�ظ���������������ȡ1���������Ч��(Ĭ��ֵ)������㴦ȡ3����Ӧ����
  int sd;
  uint32_t dwPerpCheck, dwOppPerpCheck;
  const RollbackStruct *lprbs;
  /* �ظ�������������¼������裺
   *
   * 1. �����жϼ���ظ��������û������Ƿ�����е�ǰ���棬���û�п��ܣ����ò����ж���
   *    �û���"ucRepHash"��ElephantEye��һ����ɫ������ÿִ��һ���ŷ�ʱ���ͻ����û������м�¼�µ�ǰ��"nMoveNum"
   *    ����û������Ѿ������������棬�Ͳ��ظ����ˣ�����"MakeMove()"����
   *    ��˳����ŷ�ʱ��ֻҪ�����û������ֵ�Ƿ���ڵ�ǰ��"nMoveNum"������������ո���
   *    ��������ڵ�ǰ��"nMoveNum"����˵��֮ǰ���о���ռ������û����������ո������"position.h"�е�"UndoMakeMove()"����
   */
  if (this->ucRepHash[this->zobr.dwKey & REP_HASH_MASK] == 0) {
    return REP_NONE;
  }

  // 2. ��ʼ��
  sd = OPP_SIDE(this->sdPlayer);
  dwPerpCheck = dwOppPerpCheck = 0x1ffff;
  lprbs = this->rbsList + this->nMoveNum - 1;

  // 3. �����һ���ŷ�������ǿ��Ż�����ŷ����Ͳ��������ظ���
  while (lprbs->mvs.wmv != 0 && lprbs->mvs.CptDrw <= 0) {
    __ASSERT(lprbs >= this->rbsList);

    // 4. �ж�˫���ĳ��򼶱�0��ʾ�޳���0xffff��ʾ��׽��0x10000��ʾ����
    if (sd == this->sdPlayer) {
      SetPerpCheck(dwPerpCheck, lprbs->mvs.ChkChs);

      // 5. Ѱ���ظ����棬����ظ������ﵽԤ���������򷵻��ظ��Ǻ�
      if (lprbs->zobr.dwLock0 == this->zobr.dwLock0 && lprbs->zobr.dwLock1 == this->zobr.dwLock1) {
        nRecur --;
        if (nRecur == 0) {
          dwPerpCheck = ((dwPerpCheck & 0xffff) == 0 ? dwPerpCheck : 0xffff);
          dwOppPerpCheck = ((dwOppPerpCheck & 0xffff) == 0 ? dwOppPerpCheck : 0xffff);
          return dwPerpCheck > dwOppPerpCheck ? REP_LOSS : dwPerpCheck < dwOppPerpCheck ? REP_WIN : REP_DRAW;
        }
      }

    } else {
      SetPerpCheck(dwOppPerpCheck, lprbs->mvs.ChkChs);
    }

    sd = OPP_SIDE(sd);
    lprbs --;
  }
  return REP_NONE;
}

// ������һЩ�ŷ�������
