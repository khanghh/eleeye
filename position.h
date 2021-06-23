/*
position.h/position.cpp - Source Code for ElephantEye, Part III

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.3, Last Modified: Mar. 2012
Copyright (C) 2004-2012 www.xqbase.com

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

#include <string.h>
#include "base/base.h"
#include "pregen.h"

/* ElephantEyeԴ����ʹ�õ��������Ǻ�Լ����
 *
 * sq: �������(��������0��255������"pregen.cpp")
 * pc: �������(��������0��47������"position.cpp")
 * pt: �����������(��������0��6������"position.cpp")
 * mv: �ŷ�(��������0��65535������"position.cpp")
 * sd: ���ӷ�(������0�����췽��1�����ڷ�)
 * vl: �����ֵ(��������"-MATE_VALUE"��"MATE_VALUE"������"position.cpp")
 * (ע�������ĸ��Ǻſ���uc��dw�ȴ��������ļǺ����ʹ��)
 * pos: ����(PositionStruct���ͣ�����"position.h")
 * sms: λ�к�λ�е��ŷ�����Ԥ�ýṹ(����"pregen.h")
 * smv: λ�к�λ�е��ŷ��ж�Ԥ�ýṹ(����"pregen.h")
 */

#ifndef POSITION_H
#define POSITION_H

const int MAX_MOVE_NUM = 1024;  // ���������ɵĻع��ŷ���
const int MAX_GEN_MOVES = 128;  // ����������ŷ������й�������κξ��涼���ᳬ��120���ŷ�
const int DRAW_MOVES = 100;     // Ĭ�ϵĺ����ŷ�����ElephantEye�趨��50�غϼ�100������������Ӧ������������
const int REP_HASH_MASK = 4095; // �ж��ظ�����������û������ȣ���4096������

const int MATE_VALUE = 10000;           // ��߷�ֵ���������ķ�ֵ
const int BAN_VALUE = MATE_VALUE - 100; // �����и��ķ�ֵ�����ڸ�ֵ����д���û���(����"hash.cpp")
const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����
const int NULLOKAY_MARGIN = 200;        // ���Ųü����Բ������������ֵ�߽�
const int NULLSAFE_MARGIN = 400;        // ����ʹ�ÿ��Ųü���������������ֵ�߽�
const int DRAW_VALUE = 20;              // ����ʱ���صķ���(ȡ��ֵ)

const bool CHECK_LAZY = true;   // ͵����⽫��
const int CHECK_MULTI = 48;     // ������ӽ���

// ÿ�����������ͱ��
const int KING_TYPE = 0;
const int ADVISOR_TYPE = 1;
const int BISHOP_TYPE = 2;
const int KNIGHT_TYPE = 3;
const int ROOK_TYPE = 4;
const int CANNON_TYPE = 5;
const int PAWN_TYPE = 6;

// ÿ�������Ŀ�ʼ��źͽ������
const int KING_FROM = 0;
const int ADVISOR_FROM = 1;
const int ADVISOR_TO = 2;
const int BISHOP_FROM = 3;
const int BISHOP_TO = 4;
const int KNIGHT_FROM = 5;
const int KNIGHT_TO = 6;
const int ROOK_FROM = 7;
const int ROOK_TO = 8;
const int CANNON_FROM = 9;
const int CANNON_TO = 10;
const int PAWN_FROM = 11;
const int PAWN_TO = 15;

// ��������������λ
const int KING_BITPIECE = 1 << KING_FROM;
const int ADVISOR_BITPIECE = (1 << ADVISOR_FROM) | (1 << ADVISOR_TO);
const int BISHOP_BITPIECE = (1 << BISHOP_FROM) | (1 << BISHOP_TO);
const int KNIGHT_BITPIECE = (1 << KNIGHT_FROM) | (1 << KNIGHT_TO);
const int ROOK_BITPIECE = (1 << ROOK_FROM) | (1 << ROOK_TO);
const int CANNON_BITPIECE = (1 << CANNON_FROM) | (1 << CANNON_TO);
const int PAWN_BITPIECE = (1 << PAWN_FROM) | (1 << (PAWN_FROM + 1)) |
    (1 << (PAWN_FROM + 2)) | (1 << (PAWN_FROM + 3)) | (1 << PAWN_TO);
const int ATTACK_BITPIECE = KNIGHT_BITPIECE | ROOK_BITPIECE | CANNON_BITPIECE | PAWN_BITPIECE;

inline uint32_t BIT_PIECE(int pc) {
  return 1 << (pc - 16);
}

inline uint32_t WHITE_BITPIECE(int nBitPiece) {
  return nBitPiece;
}

inline uint32_t BLACK_BITPIECE(int nBitPiece) {
  return nBitPiece << 16;
}

inline uint32_t BOTH_BITPIECE(int nBitPiece) {
  return nBitPiece + (nBitPiece << 16);
}

// "RepStatus()"���ص��ظ����λ
const int REP_NONE = 0;
const int REP_DRAW = 1;
const int REP_LOSS = 3;
const int REP_WIN = 5;

/* ElephantEye�ĺܶ�����ж��õ�"SIDE_TAG()"��������췽��Ϊ16���ڷ���Ϊ32��
 * ��"SIDE_TAG() + i"���Է����ѡ�����ӵ����ͣ�"i"��0��15�����ǣ�
 * ˧�������������������ڱ���������(��ʿʿ��������������������������)
 * ����"i"ȡ"KNIGHT_FROM"��"KNIGHT_TO"�����ʾ���μ����������λ��
 */
inline int SIDE_TAG(int sd) {
  return 16 + (sd << 4);
}

inline int OPP_SIDE_TAG(int sd) {
  return 32 - (sd << 4);
}

inline int SIDE_VALUE(int sd, int vl) {
  return (sd == 0 ? vl : -vl);
}

inline int PIECE_INDEX(int pc) {
  return pc & 15;
}

extern const char *const cszStartFen;     // ��ʼ�����FEN��
extern const char *const cszPieceBytes;   // �������Ͷ�Ӧ�����ӷ���
extern const int cnPieceTypes[48];        // ������Ŷ�Ӧ����������
extern const int cnSimpleValues[48];      // ���ӵļ򵥷�ֵ
extern const uint8_t cucsqMirrorTab[256]; // ����ľ���(���ҶԳ�)����

inline char PIECE_BYTE(int pt) {
  return cszPieceBytes[pt];
}

inline int PIECE_TYPE(int pc) {
  return cnPieceTypes[pc];
}

inline int SIMPLE_VALUE(int pc) {
  return cnSimpleValues[pc];
}

inline uint8_t SQUARE_MIRROR(int sq) {
  return cucsqMirrorTab[sq];
}

// FEN�������ӱ�ʶ
int FenPiece(int Arg);

// �����ŷ��ṹ
union MoveStruct {
  uint32_t dwmv;           // ���������ṹ��
  struct {
    uint16_t wmv, wvl;     // �ŷ��ͷ�ֵ
  };
  struct {
    uint8_t Src, Dst;      // ��ʼ���Ŀ���
    int8_t CptDrw, ChkChs; // ������(+)/�����ŷ���(-)��������(+)/��׽��(-)
  };
}; // mvs

// �ŷ��ṹ
inline int SRC(int mv) { // �õ��ŷ������
  return mv & 255;
}

inline int DST(int mv) { // �õ��ŷ����յ�
  return mv >> 8;
}

inline int MOVE(int sqSrc, int sqDst) {   // �������յ�õ��ŷ�
  return sqSrc + (sqDst << 8);
}

inline uint32_t MOVE_COORD(int mv) {      // ���ŷ�ת�����ַ���
  union {
    char c[4];
    uint32_t dw;
  } Ret;
  Ret.c[0] = FILE_X(SRC(mv)) - FILE_LEFT + 'a';
  Ret.c[1] = '9' - RANK_Y(SRC(mv)) + RANK_TOP;
  Ret.c[2] = FILE_X(DST(mv)) - FILE_LEFT + 'a';
  Ret.c[3] = '9' - RANK_Y(DST(mv)) + RANK_TOP;
  // ��������ŷ��ĺ�����
  __ASSERT_BOUND('a', Ret.c[0], 'i');
  __ASSERT_BOUND('0', Ret.c[1], '9');
  __ASSERT_BOUND('a', Ret.c[2], 'i');
  __ASSERT_BOUND('0', Ret.c[3], '9');
  return Ret.dw;
}

inline int COORD_MOVE(uint32_t dwMoveStr) { // ���ַ���ת�����ŷ�
  int sqSrc, sqDst;
  char *lpArgPtr;
  lpArgPtr = (char *) &dwMoveStr;
  sqSrc = COORD_XY(lpArgPtr[0] - 'a' + FILE_LEFT, '9' - lpArgPtr[1] + RANK_TOP);
  sqDst = COORD_XY(lpArgPtr[2] - 'a' + FILE_LEFT, '9' - lpArgPtr[3] + RANK_TOP);
  // �������ŷ��ĺ����Բ�������
  // __ASSERT_SQUARE(sqSrc);
  // __ASSERT_SQUARE(sqDst);
  return (IN_BOARD(sqSrc) && IN_BOARD(sqDst) ? MOVE(sqSrc, sqDst) : 0);
}

inline int MOVE_MIRROR(int mv) {          // ���ŷ�������
  return MOVE(SQUARE_MIRROR(SRC(mv)), SQUARE_MIRROR(DST(mv)));
}

// �ع��ṹ
struct RollbackStruct {
  ZobristStruct zobr;   // Zobrist
  int vlWhite, vlBlack; // �췽�ͺڷ���������ֵ
  MoveStruct mvs;       // �ŷ�
}; // rbs

const bool DEL_PIECE = true; // ����"PositionStruct::AddPiece()"��ѡ��

// ����ṹ
struct PositionStruct {
  // ������Ա
  int sdPlayer;             // �ֵ��ķ��ߣ�0��ʾ�췽��1��ʾ�ڷ�
  uint8_t ucpcSquares[256]; // ÿ�����ӷŵ����ӣ�0��ʾû������
  uint8_t ucsqPieces[48];   // ÿ�����ӷŵ�λ�ã�0��ʾ����
  ZobristStruct zobr;       // Zobrist

  // λ�ṹ��Ա��������ǿ���̵Ĵ���
  union {
    uint32_t dwBitPiece;    // 32λ������λ��0��31λ���α�ʾ���Ϊ16��47�������Ƿ���������
    uint16_t wBitPiece[2];  // ��ֳ�����
  };
  uint16_t wBitRanks[16];   // λ�����飬ע���÷���"wBitRanks[RANK_Y(sq)]"
  uint16_t wBitFiles[16];   // λ�����飬ע���÷���"wBitFiles[FILE_X(sq)]"

  // ������������
  int vlWhite, vlBlack;   // �췽�ͺڷ���������ֵ

  // �ع��ŷ����������ѭ������
  int nMoveNum, nDistance;              // �ع��ŷ������������
  RollbackStruct rbsList[MAX_MOVE_NUM]; // �ع��б�
  uint8_t ucRepHash[REP_HASH_MASK + 1]; // �ж��ظ�����������û���

  // ��ȡ�ŷ�Ԥ������Ϣ
  SlideMoveStruct *RankMovePtr(int x, int y) const {
    return PreGen.smvRankMoveTab[x - FILE_LEFT] + wBitRanks[y];
  }
  SlideMoveStruct *FileMovePtr(int x, int y) const {
    return PreGen.smvFileMoveTab[y - RANK_TOP] + wBitFiles[x];
  }
  SlideMaskStruct *RankMaskPtr(int x, int y) const {
    return PreGen.smsRankMaskTab[x - FILE_LEFT] + wBitRanks[y];
  }
  SlideMaskStruct *FileMaskPtr(int x, int y) const {
    return PreGen.smsFileMaskTab[y - RANK_TOP] + wBitFiles[x];
  }

  // ���̴�������
  void ClearBoard(void) { // ���̳�ʼ��
    sdPlayer = 0;
    memset(ucpcSquares, 0, 256);
    memset(ucsqPieces, 0, 48);
    zobr.InitZero();
    dwBitPiece = 0;
    memset(wBitRanks, 0, 16 * sizeof(uint16_t));
    memset(wBitFiles, 0, 16 * sizeof(uint16_t));
    vlWhite = vlBlack = 0;
    // "ClearBoard()"�����������"SetIrrev()"������ʼ��������Ա
  }
  void ChangeSide(void) { // �������巽
    sdPlayer = OPP_SIDE(sdPlayer);
    zobr.Xor(PreGen.zobrPlayer);
  }
  void SaveStatus(void) { // ����״̬
    RollbackStruct *lprbs;
    lprbs = rbsList + nMoveNum;
    lprbs->zobr = zobr;
    lprbs->vlWhite = vlWhite;
    lprbs->vlBlack = vlBlack;
  }
  void Rollback(void) {   // �ع�
    RollbackStruct *lprbs;
    lprbs = rbsList + nMoveNum;
    zobr = lprbs->zobr;
    vlWhite = lprbs->vlWhite;
    vlBlack = lprbs->vlBlack;
  }
  void AddPiece(int sq, int pc, bool bDel = false); // ��������������
  int MovePiece(int mv);                            // �ƶ�����
  void UndoMovePiece(int mv, int pcCaptured);       // �����ƶ�����
  int Promote(int sq);                              // ����
  void UndoPromote(int sq, int pcCaptured);         // ��������

  // �ŷ���������
  bool MakeMove(int mv);   // ִ��һ���ŷ�
  void UndoMakeMove(void); // ����һ���ŷ�
  void NullMove(void);     // ִ��һ������
  void UndoNullMove(void); // ����һ������
  void SetIrrev(void) {    // �Ѿ�����ɡ������桱��������ع��ŷ�
    rbsList[0].mvs.dwmv = 0; // wmv, Chk, CptDrw, ChkChs = 0
    rbsList[0].mvs.ChkChs = CheckedBy();
    nMoveNum = 1;
    nDistance = 0;
    memset(ucRepHash, 0, REP_HASH_MASK + 1);
  }

  // ���洦������
  void FromFen(const char *szFen); // FEN��ʶ��
  void ToFen(char *szFen) const;   // ����FEN��
  void Mirror(void);               // ���澵��

  // �ŷ�������
  bool GoodCap(int mv) const {     // �õĳ����ŷ���⣬�������ŷ�����¼����ʷ����ɱ���ŷ�����
    int pcMoved, pcCaptured;
    pcCaptured = ucpcSquares[DST(mv)];
    if (pcCaptured == 0) {
      return false;
    }
    if (!Protected(OPP_SIDE(sdPlayer), DST(mv))) {
      return true;
    }
    pcMoved = ucpcSquares[SRC(mv)];
    return SIMPLE_VALUE(pcCaptured) > SIMPLE_VALUE(pcMoved);
  }
  bool LegalMove(int mv) const;            // �ŷ������Լ�⣬�����ڡ�ɱ���ŷ����ļ����
  int CheckedBy(bool bLazy = false) const; // ���ĸ��ӽ���
  bool IsMate(void);                       // �ж����ѱ�����
  MoveStruct LastMove(void) const {        // ǰһ���ŷ������ŷ������˾���Ľ���״̬
    return rbsList[nMoveNum - 1].mvs;
  }
  bool CanPromote(void) const {            // �ж��Ƿ�������
    return (wBitPiece[sdPlayer] & PAWN_BITPIECE) != PAWN_BITPIECE && LastMove().ChkChs <= 0;
  }
  bool NullOkay(void) const {              // ����ʹ�ÿ��Ųü�������
    return (sdPlayer == 0 ? vlWhite : vlBlack) > NULLOKAY_MARGIN;
  }
  bool NullSafe(void) const {              // ���Ųü����Բ����������
    return (sdPlayer == 0 ? vlWhite : vlBlack) > NULLSAFE_MARGIN;
  }
  bool IsDraw(void) const {                // �����ж�
    return (!PreEval.bPromotion && (dwBitPiece & BOTH_BITPIECE(ATTACK_BITPIECE)) == 0) ||
        -LastMove().CptDrw >= DRAW_MOVES || nMoveNum == MAX_MOVE_NUM;
  }
  int RepStatus(int nRecur = 1) const;     // �ظ�������
  int DrawValue(void) const {              // ����ķ�ֵ
    return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
  }
  int RepValue(int vlRep) const {          // �ظ�����ķ�ֵ
    // return vlRep == REP_LOSS ? nDistance - MATE_VALUE : vlRep == REP_WIN ? MATE_VALUE - nDistance : DrawValue();
    // �����и��ķ�ֵ������BAN_VALUE����д���û���(����"hash.cpp")
    return vlRep == REP_LOSS ? nDistance - BAN_VALUE : vlRep == REP_WIN ? BAN_VALUE - nDistance : DrawValue();
  }
  int Material(void) const {               // ����ƽ�⣬��������Ȩ����
    return SIDE_VALUE(sdPlayer, vlWhite - vlBlack) + PreEval.vlAdvanced;
  }

  // �ŷ����ɹ��̣�������Щ���̴������ر�����԰����Ƕ�������"genmoves.cpp"��
  bool Protected(int sd, int sqSrc, int sqExcept = 0) const; // ���ӱ����ж�
  int ChasedBy(int mv) const;                                // ׽�ĸ���
  int MvvLva(int sqDst, int pcCaptured, int nLva) const;     // ����MVV(LVA)ֵ
  int GenCapMoves(MoveStruct *lpmvs) const;                  // �����ŷ�������
  int GenNonCapMoves(MoveStruct *lpmvs) const;               // �������ŷ�������
  int GenAllMoves(MoveStruct *lpmvs) const {                 // ȫ���ŷ�������
    int nCapNum;
    nCapNum = GenCapMoves(lpmvs);
    return nCapNum + GenNonCapMoves(lpmvs + nCapNum);
  }

  // �ŷ����ɹ��̣�������Щ���̴������ر�����԰����Ƕ�������"preeval.cpp"��"evaluate.cpp"��
  void PreEvaluate(void);
  int AdvisorShape(void) const;
  int StringHold(void) const;
  int RookMobility(void) const;
  int KnightTrap(void) const;
  int Evaluate(int vlAlpha, int vlBeta) const;
}; // pos

#endif
