/*
evaluate.cpp - Source Code for ElephantEye, Part XI

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

#include "base/base.h"
#include "pregen.h"
#include "position.h"
#include "preeval.h"

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

// ͵�����۵ı߽�
const int EVAL_MARGIN1 = 160;
const int EVAL_MARGIN2 = 80;
const int EVAL_MARGIN3 = 40;
const int EVAL_MARGIN4 = 20;

// ��ģ��ֻ�漰��"PositionStruct"�е�"sdPlayer"��"ucpcSquares"��"ucsqPieces"��"wBitPiece"�ĸ���Ա����ʡ��ǰ���"this->"

/* ElephantEye�ľ����������ݹ�4��4����
 * 1. ����(ʿ)�йص��������͵����ۣ���"AdvisorShape()"��
 * 2. ������ǣ��˧(��)�򳵵����͵����ۣ���"StringHold()"��
 * 3. ��������Ե����ۣ���"RookMobility()"��
 * 4. ���ܵ��谭�����ۣ���"KnightTrap()"��
 */

// �����ǵ�һ���֣��������͵�����

/* ��(ʿ)����״���ھ������ۣ��ر����жϿ�ͷ�ڡ������ڵ��������ش����ã�Ϊ��ElephantEye����������״��
 * 1. ˧(��)��ԭλ��˫��(ʿ)���ڵ��ߣ���Ϊ1�ţ��������Ҫ�жϿ�ͷ�ں�������������
 * 2. ˧(��)��ԭλ��˫��(ʿ)����߰�Χ˧(��)����Ϊ2�ţ��������Ҫ�ж��ұߵĳ����ںͳ����ұߵ�˧(��)�ţ�
 * 3. ˧(��)��ԭλ��˫��(ʿ)���ұ߰�Χ˧(��)����Ϊ3�ţ��������Ҫ�ж���ߵĳ����ںͳ�����ߵ�˧(��)�ţ�
 * 4. �������������˧(��)����ԭλ��ȱ��(ʿ)�������0��
 * ע���ԡ����º��ϡ�����̶������̷�λ���涨���ҡ�
 */
const int WHITE_KING_BITFILE = 1 << (RANK_BOTTOM - RANK_TOP);
const int BLACK_KING_BITFILE = 1 << (RANK_TOP - RANK_TOP);
const int KING_BITRANK = 1 << (FILE_CENTER - FILE_LEFT);

const int SHAPE_NONE = 0;
const int SHAPE_CENTER = 1;
const int SHAPE_LEFT = 2;
const int SHAPE_RIGHT = 3;

int PositionStruct::AdvisorShape(void) const {
  int pcCannon, pcRook, sq, sqAdv1, sqAdv2, x, y, nShape;
  int vlWhitePenalty, vlBlackPenalty;
  SlideMaskStruct *lpsms;
  vlWhitePenalty = vlBlackPenalty = 0;
  if ((this->wBitPiece[0] & ADVISOR_BITPIECE) == ADVISOR_BITPIECE) {
    if (this->ucsqPieces[SIDE_TAG(0) + KING_FROM] == 0xc7) {
      sqAdv1 = this->ucsqPieces[SIDE_TAG(0) + ADVISOR_FROM];
      sqAdv2 = this->ucsqPieces[SIDE_TAG(0) + ADVISOR_TO];
      if (false) {
      } else if (sqAdv1 == 0xc6) { // �췽һ������������
        nShape = (sqAdv2 == 0xc8 ? SHAPE_CENTER : sqAdv2 == 0xb7 ? SHAPE_LEFT : SHAPE_NONE);
      } else if (sqAdv1 == 0xc8) { // �췽һ�������Ҳ����
        nShape = (sqAdv2 == 0xc6 ? SHAPE_CENTER : sqAdv2 == 0xb7 ? SHAPE_RIGHT : SHAPE_NONE);
      } else if (sqAdv1 == 0xb7) { // �췽һ�����ڻ���
        nShape = (sqAdv2 == 0xc6 ? SHAPE_LEFT : sqAdv2 == 0xc8 ? SHAPE_RIGHT : SHAPE_NONE);
      } else {
        nShape = SHAPE_NONE;
      }
      switch (nShape) {
      case SHAPE_NONE:
        break;
      case SHAPE_CENTER:
        for (pcCannon = SIDE_TAG(1) + CANNON_FROM; pcCannon <= SIDE_TAG(1) + CANNON_TO; pcCannon ++) {
          sq = this->ucsqPieces[pcCannon];
          if (sq != 0) {
            x = FILE_X(sq);
            if (x == FILE_CENTER) {
              y = RANK_Y(sq);
              lpsms = this->FileMaskPtr(x, y);
              if ((lpsms->wRookCap & WHITE_KING_BITFILE) != 0) {
                // �����ͷ�ڵ���в
                vlWhitePenalty += PreEvalEx.vlHollowThreat[RANK_FLIP(y)];
              } else if ((lpsms->wSuperCap & WHITE_KING_BITFILE) != 0 &&
                  (this->ucpcSquares[0xb7] == 21 || this->ucpcSquares[0xb7] == 22)) {
                // ������������������в
                vlWhitePenalty += PreEvalEx.vlCentralThreat[RANK_FLIP(y)];
              }
            }
          }
        }
        break;
      case SHAPE_LEFT:
      case SHAPE_RIGHT:
        for (pcCannon = SIDE_TAG(1) + CANNON_FROM; pcCannon <= SIDE_TAG(1) + CANNON_TO; pcCannon ++) {
          sq = this->ucsqPieces[pcCannon];
          if (sq != 0) {
            x = FILE_X(sq);
            y = RANK_Y(sq);
            if (x == FILE_CENTER) {
              if ((this->FileMaskPtr(x, y)->wSuperCap & WHITE_KING_BITFILE) != 0) {
                // ����һ�����ڵ���в��˧(��)�ű��Է����ƵĻ��ж��ⷣ��
                vlWhitePenalty += (PreEvalEx.vlCentralThreat[RANK_FLIP(y)] >> 2) +
                    (this->Protected(1, nShape == SHAPE_LEFT ? 0xc8 : 0xc6) ? 20 : 0);
                // ������ڵ��߱���˧(��)����������ķ��֣�
                for (pcRook = SIDE_TAG(0) + ROOK_FROM; pcRook <= SIDE_TAG(0) + ROOK_TO; pcRook ++) {
                  sq = this->ucsqPieces[pcRook];
                  if (sq != 0) {
                    y = RANK_Y(sq);
                    if (y == RANK_BOTTOM) {
                      x = FILE_X(sq);
                      if ((this->RankMaskPtr(x, y)->wRookCap & KING_BITRANK) != 0) {
                        vlWhitePenalty += 80;
                      }
                    }
                  }
                }
              }
            } else if (y == RANK_BOTTOM) {
              if ((this->RankMaskPtr(x, y)->wRookCap & KING_BITRANK) != 0) {
                // ��������ڵ���в
                vlWhitePenalty += PreEvalEx.vlWhiteBottomThreat[x];
              }
            }
          }
        }
        break;
      default:
        break;
      }
    } else if (this->ucsqPieces[SIDE_TAG(0) + KING_FROM] == 0xb7) {
      // ��˫��(ʿ)�����ı�˧(��)ռ�죬Ҫ����
      vlWhitePenalty += 20;
    }
  } else {
    if ((this->wBitPiece[1] & ROOK_BITPIECE) == ROOK_BITPIECE) {
      // ȱ��(ʿ)��˫�����з���
      vlWhitePenalty += PreEvalEx.vlWhiteAdvisorLeakage;
    }
  }
  if ((this->wBitPiece[1] & ADVISOR_BITPIECE) == ADVISOR_BITPIECE) {
    if (this->ucsqPieces[SIDE_TAG(1) + KING_FROM] == 0x37) {
      sqAdv1 = this->ucsqPieces[SIDE_TAG(1) + ADVISOR_FROM];
      sqAdv2 = this->ucsqPieces[SIDE_TAG(1) + ADVISOR_TO];
      if (false) {
      } else if (sqAdv1 == 0x36) { // �ڷ�һ��ʿ��������
        nShape = (sqAdv2 == 0x38 ? SHAPE_CENTER : sqAdv2 == 0x47 ? SHAPE_LEFT : SHAPE_NONE);
      } else if (sqAdv1 == 0x38) { // �ڷ�һ��ʿ���Ҳ����
        nShape = (sqAdv2 == 0x36 ? SHAPE_CENTER : sqAdv2 == 0x47 ? SHAPE_RIGHT : SHAPE_NONE);
      } else if (sqAdv1 == 0x47) { // �ڷ�һ��ʿ�ڻ���
        nShape = (sqAdv2 == 0x36 ? SHAPE_LEFT : sqAdv2 == 0x38 ? SHAPE_RIGHT : SHAPE_NONE);
      } else {
        nShape = SHAPE_NONE;
      }
      switch (nShape) {
      case SHAPE_NONE:
        break;
      case SHAPE_CENTER:
        for (pcCannon = SIDE_TAG(0) + CANNON_FROM; pcCannon <= SIDE_TAG(0) + CANNON_TO; pcCannon ++) {
          sq = this->ucsqPieces[pcCannon];
          if (sq != 0) {
            x = FILE_X(sq);
            if (x == FILE_CENTER) {
              y = RANK_Y(sq);
              lpsms = this->FileMaskPtr(x, y);
              if ((lpsms->wRookCap & BLACK_KING_BITFILE) != 0) {
                // �����ͷ�ڵ���в
                vlBlackPenalty += PreEvalEx.vlHollowThreat[y];
              } else if ((lpsms->wSuperCap & BLACK_KING_BITFILE) != 0 &&
                  (this->ucpcSquares[0x47] == 37 || this->ucpcSquares[0x47] == 38)) {
                // ������������������в
                vlBlackPenalty += PreEvalEx.vlCentralThreat[y];
              }
            }
          }
        }
        break;
      case SHAPE_LEFT:
      case SHAPE_RIGHT:
        for (pcCannon = SIDE_TAG(0) + CANNON_FROM; pcCannon <= SIDE_TAG(0) + CANNON_TO; pcCannon ++) {
          sq = this->ucsqPieces[pcCannon];
          if (sq != 0) {
            x = FILE_X(sq);
            y = RANK_Y(sq);
            if (x == FILE_CENTER) {
              if ((this->FileMaskPtr(x, y)->wSuperCap & BLACK_KING_BITFILE) != 0) {
                // ����һ�����ڵ���в��˧(��)�ű��Է����ƵĻ��ж��ⷣ��
                vlBlackPenalty += (PreEvalEx.vlCentralThreat[y] >> 2) +
                    (this->Protected(0, nShape == SHAPE_LEFT ? 0x38 : 0x36) ? 20 : 0);
                // ������ڵ��߱���˧(��)����������ķ��֣�
                for (pcRook = SIDE_TAG(1) + ROOK_FROM; pcRook <= SIDE_TAG(1) + ROOK_TO; pcRook ++) {
                  sq = this->ucsqPieces[pcRook];
                  if (sq != 0) {
                    y = RANK_Y(sq);
                    if (y == RANK_TOP) {
                      x = FILE_X(sq);
                      if ((this->RankMaskPtr(x, y)->wRookCap & KING_BITRANK) != 0) {
                        vlBlackPenalty += 80;
                      }
                    }
                  }
                }
              }
            } else if (y == RANK_TOP) {
              if ((this->RankMaskPtr(x, y)->wRookCap & KING_BITRANK) != 0) {
                // ��������ڵ���в
                vlBlackPenalty += PreEvalEx.vlBlackBottomThreat[x];
              }
            }
          }
        }
        break;
      default:
        break;
      }
    } else if (this->ucsqPieces[SIDE_TAG(1) + KING_FROM] == 0x47) {
      // ��˫��(ʿ)�����ı�˧(��)ռ�죬Ҫ����
      vlBlackPenalty += 20;
    }
  } else {
    if ((this->wBitPiece[0] & ROOK_BITPIECE) == ROOK_BITPIECE) {
      // ȱ��(ʿ)��˫�����з���
      vlBlackPenalty += PreEvalEx.vlBlackAdvisorLeakage;
    }
  }
  return SIDE_VALUE(this->sdPlayer, vlBlackPenalty - vlWhitePenalty);
}

// �����ǵ�һ���֣��������͵�����

// �����ǵڶ����֣�ǣ�Ƶ�����

// ������"cnValuableStringPieces"���ж�ǣ���Ƿ��м�ֵ
// ����0�����Ƕ��ڳ���˵�ģ�ǣ��������(��ǣ��)���м�ֵ������1�����Ƕ�������˵ֻ��ǣ�������м�ֵ
static const int cnValuableStringPieces[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0
};

// "ccvlStringValueTab"������"KNIGHT_PIN_TAB"�ĳ�����(����"pregen.h")������ǣ�Ƽ�ֵ
// �м��Ӻͱ�ǣ���ӵľ���Խ����ǣ�Ƶļ�ֵ��Խ��
static const char ccvlStringValueTab[512] = {
                               0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
  12, 16, 20, 24, 28, 32, 36,  0, 36, 32, 28, 24, 20, 16, 12,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0
};

// ������ǣ��˧(��)�򳵵����͵�����
int PositionStruct::StringHold(void) const {
  int sd, i, j, nDir, sqSrc, sqDst, sqStr;
  int x, y, nSideTag, nOppSideTag;
  int vlString[2];
  SlideMoveStruct *lpsmv;

  for (sd = 0; sd < 2; sd ++) {
    vlString[sd] = 0;
    nSideTag = SIDE_TAG(sd);
    nOppSideTag = OPP_SIDE_TAG(sd);
    // �����ó���ǣ�Ƶ����
    for (i = ROOK_FROM; i <= ROOK_TO; i ++) {
      sqSrc = this->ucsqPieces[nSideTag + i];
      if (sqSrc != 0) {
        __ASSERT_SQUARE(sqSrc);
        // ����ǣ��Ŀ����˧(��)�����
        sqDst = this->ucsqPieces[nOppSideTag + KING_FROM];
        if (sqDst != 0) {
          __ASSERT_SQUARE(sqDst);
          x = FILE_X(sqSrc);
          y = RANK_Y(sqSrc);
          if (x == FILE_X(sqDst)) {
            lpsmv = this->FileMovePtr(x, y);
            nDir = (sqSrc < sqDst ? 0 : 1);
            // ��������ڵĳԷ�(���ó����ڵ��ŷ�)�ܳԵ�Ŀ����"sqDst"��ǣ�ƾͳ����ˣ���ͬ
            if (sqDst == lpsmv->ucCannonCap[nDir] + FILE_DISP(x)) {
              // ��ǣ����"sqStr"�ǳ�(��)�����ܳԵ������ӣ���ͬ
              sqStr = lpsmv->ucRookCap[nDir] + FILE_DISP(x);
              __ASSERT_SQUARE(sqStr);
              // ��ǣ���ӱ����ǶԷ����ӣ���ͬ
              if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                // �����ǣ�������м�ֵ�ģ����ұ�ǣ����û�б���(��Ŀ���ӱ�������)����ôǣ�����м�ֵ�ģ���ͬ
                if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 0 &&
                    !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                  vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                }
              }
            }
          } else if (y == RANK_Y(sqDst)) {
            lpsmv = this->RankMovePtr(x, y);
            nDir = (sqSrc < sqDst ? 0 : 1);
            if (sqDst == lpsmv->ucCannonCap[nDir] + RANK_DISP(y)) {
              sqStr = lpsmv->ucRookCap[nDir] + RANK_DISP(y);
              __ASSERT_SQUARE(sqStr);
              if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 0 &&
                    !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                  vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                }
              }
            }
          }
        }
        // ����ǣ��Ŀ���ǳ������
        for (j = ROOK_FROM; j <= ROOK_TO; j ++) {
          sqDst = this->ucsqPieces[nOppSideTag + j];
          if (sqDst != 0) {
            __ASSERT_SQUARE(sqDst);
            x = FILE_X(sqSrc);
            y = RANK_Y(sqSrc);
            if (x == FILE_X(sqDst)) {
              lpsmv = this->FileMovePtr(x, y);
              nDir = (sqSrc < sqDst ? 0 : 1);
              if (sqDst == lpsmv->ucCannonCap[nDir] + FILE_DISP(x)) {
                sqStr = lpsmv->ucRookCap[nDir] + FILE_DISP(x);
                __ASSERT_SQUARE(sqStr);
                if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                  // Ŀ�����ǳ�����ͬ��˧(��)��Ҫ��Ҳû�б���ʱ����ǣ�Ƽ�ֵ����ͬ
                  if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 0 &&
                      !this->Protected(OPP_SIDE(sd), sqDst) && !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                    vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                  }
                }
              }
            } else if (y == RANK_Y(sqDst)) {
              lpsmv = this->RankMovePtr(x, y);
              nDir = (sqSrc < sqDst ? 0 : 1);
              if (sqDst == lpsmv->ucCannonCap[nDir] + RANK_DISP(y)) {
                sqStr = lpsmv->ucRookCap[nDir] + RANK_DISP(y);
                __ASSERT_SQUARE(sqStr);
                if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                  if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 0 &&
                      !this->Protected(OPP_SIDE(sd), sqDst) && !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                    vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                  }
                }
              }
            }
          }
        }
      }
    }

    // ����������ǣ�Ƶ����
    for (i = CANNON_FROM; i <= CANNON_TO; i ++) {
      sqSrc = this->ucsqPieces[nSideTag + i];
      if (sqSrc != 0) {
        __ASSERT_SQUARE(sqSrc);
        // ����ǣ��Ŀ����˧(��)�����
        sqDst = this->ucsqPieces[nOppSideTag + KING_FROM];
        if (sqDst != 0) {
          __ASSERT_SQUARE(sqDst);
          x = FILE_X(sqSrc);
          y = RANK_Y(sqSrc);
          if (x == FILE_X(sqDst)) {
            lpsmv = this->FileMovePtr(x, y);
            nDir = (sqSrc < sqDst ? 0 : 1);
            if (sqDst == lpsmv->ucSuperCap[nDir] + FILE_DISP(x)) {
              sqStr = lpsmv->ucCannonCap[nDir] + FILE_DISP(x);
              __ASSERT_SQUARE(sqStr);
              if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 1 &&
                    !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                  vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                }
              }
            }
          } else if (y == RANK_Y(sqDst)) {
            lpsmv = this->RankMovePtr(x, y);
            nDir = (sqSrc < sqDst ? 0 : 1);
            if (sqDst == lpsmv->ucSuperCap[nDir] + RANK_DISP(y)) {
              sqStr = lpsmv->ucCannonCap[nDir] + RANK_DISP(y);
              __ASSERT_SQUARE(sqStr);
              if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 1 &&
                    !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                  vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                }
              }
            }
          }
        }
        // ����ǣ��Ŀ���ǳ������
        for (j = ROOK_FROM; j <= ROOK_TO; j ++) {
          sqDst = this->ucsqPieces[nOppSideTag + j];
          if (sqDst != 0) {
            __ASSERT_SQUARE(sqDst);
            x = FILE_X(sqSrc);
            y = RANK_Y(sqSrc);
            if (x == FILE_X(sqDst)) {
              lpsmv = this->FileMovePtr(x, y);
              nDir = (sqSrc < sqDst ? 0 : 1);
              if (sqDst == lpsmv->ucSuperCap[nDir] + FILE_DISP(x)) {
                sqStr = lpsmv->ucCannonCap[nDir] + FILE_DISP(x);
                __ASSERT_SQUARE(sqStr);
                if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                  if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 1 &&
                      !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                    vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                  }
                }
              }
            } else if (y == RANK_Y(sqDst)) {
              lpsmv = this->RankMovePtr(x, y);
              nDir = (sqSrc < sqDst ? 0 : 1);
              if (sqDst == lpsmv->ucSuperCap[nDir] + RANK_DISP(y)) {
                sqStr = lpsmv->ucCannonCap[nDir] + RANK_DISP(y);
                __ASSERT_SQUARE(sqStr);
                if ((this->ucpcSquares[sqStr] & nOppSideTag) != 0) {
                  if (cnValuableStringPieces[this->ucpcSquares[sqStr]] > 1 &&
                      !this->Protected(OPP_SIDE(sd), sqStr, sqDst)) {
                    vlString[sd] += ccvlStringValueTab[sqDst - sqStr + 256];
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return SIDE_VALUE(this->sdPlayer, vlString[0] - vlString[1]);
}

// �����ǵڶ����֣�ǣ�Ƶ�����

// �����ǵ������֣���������Ե�����

int PositionStruct::RookMobility(void) const {
  int sd, i, sqSrc, nSideTag, x, y;
  int vlRookMobility[2];
  for (sd = 0; sd < 2; sd ++) {
    vlRookMobility[sd] = 0;
    nSideTag = SIDE_TAG(sd);
    for (i = ROOK_FROM; i <= ROOK_TO; i ++) {
      sqSrc = this->ucsqPieces[nSideTag + i];
      if (sqSrc != 0) {
        __ASSERT_SQUARE(sqSrc);
        x = FILE_X(sqSrc);
        y = RANK_Y(sqSrc);
        vlRookMobility[sd] += PreEvalEx.cPopCnt16[this->RankMaskPtr(x, y)->wNonCap] +
            PreEvalEx.cPopCnt16[this->FileMaskPtr(x, y)->wNonCap];
      }
    }
    __ASSERT(vlRookMobility[sd] <= 34);
  }
  return SIDE_VALUE(this->sdPlayer, vlRookMobility[0] - vlRookMobility[1]) >> 1;
}

// �����ǵ������֣���������Ե�����

// �����ǵ��Ĳ��֣����ܵ��谭������

// ������"cbcEdgeSquares"�����˲���������λ�ã��������̱�Ե����������λ�õ������ǻ���
static const bool cbcEdgeSquares[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int PositionStruct::KnightTrap(void) const {
  int sd, i, sqSrc, sqDst, nSideTag, nMovable;
  uint8_t *lpucsqDst, *lpucsqPin;
  int vlKnightTraps[2];

  for (sd = 0; sd < 2; sd ++) {
    vlKnightTraps[sd] = 0;
    nSideTag = SIDE_TAG(sd);
    // �����������ߵ�λ�ã��ߵ����̱�Ե�ϣ������ߵ��Է��Ŀ��Ƹ񣬶������ų�
    for (i = KNIGHT_FROM; i <= KNIGHT_TO; i ++) {
      sqSrc = this->ucsqPieces[nSideTag + i];
      if (sqSrc != 0) {
        __ASSERT_SQUARE(sqSrc);
        nMovable = 0;
        lpucsqDst = PreGen.ucsqKnightMoves[sqSrc];
        lpucsqPin = PreGen.ucsqKnightPins[sqSrc];
        sqDst = *lpucsqDst;
        while (sqDst != 0) {
          __ASSERT_SQUARE(sqDst);
          // ���µ��ж�������"genmoves.cpp"�е��ŷ����������ų����ߵ����̱�Ե���ߵ��Է����Ƹ���ŷ�
          if (!cbcEdgeSquares[sqDst] && this->ucpcSquares[sqDst] == 0 &&
              this->ucpcSquares[*lpucsqPin] == 0 && !this->Protected(OPP_SIDE(sd), sqDst)) {
            nMovable ++;
            if (nMovable > 1) {
              break;
            }
          }
          lpucsqDst ++;
          sqDst = *lpucsqDst;
          lpucsqPin ++;
        }
        // û�кõ��ŷ���������10�ַ��֣�ֻ��һ���õ��ŷ���������5�ַ���
        if (nMovable == 0) {
          vlKnightTraps[sd] += 10;
        } else if (nMovable == 1) {
          vlKnightTraps[sd] += 5;
        }
      }
      __ASSERT(vlKnightTraps[sd] <= 20);
    }
  }
  return SIDE_VALUE(this->sdPlayer, vlKnightTraps[1] - vlKnightTraps[0]);
}

// �����ǵ��Ĳ��֣����ܵ��谭������

// �������۹���
int PositionStruct::Evaluate(int vlAlpha, int vlBeta) const {
  int vl;
  // ͵���ľ������ۺ��������¼�����Σ�

  // 1. �ļ�͵������(����͵������)��ֻ��������ƽ�⣻
  vl = this->Material();
  if (vl + EVAL_MARGIN1 <= vlAlpha) {
    return vl + EVAL_MARGIN1;
  } else if (vl - EVAL_MARGIN1 >= vlBeta) {
    return vl - EVAL_MARGIN1;
  }

  // 2. ����͵�����ۣ������������ͣ�
  vl += this->AdvisorShape();
  if (vl + EVAL_MARGIN2 <= vlAlpha) {
    return vl + EVAL_MARGIN2;
  } else if (vl - EVAL_MARGIN2 >= vlBeta) {
    return vl - EVAL_MARGIN2;
  }

  // 3. ����͵�����ۣ�����ǣ�ƣ�
  vl += this->StringHold();
  if (vl + EVAL_MARGIN3 <= vlAlpha) {
    return vl + EVAL_MARGIN3;
  } else if (vl - EVAL_MARGIN3 >= vlBeta) {
    return vl - EVAL_MARGIN3;
  }

  // 4. һ��͵�����ۣ�������������ԣ�
  vl += this->RookMobility();
  if (vl + EVAL_MARGIN4 <= vlAlpha) {
    return vl + EVAL_MARGIN4;
  } else if (vl - EVAL_MARGIN4 >= vlBeta) {
    return vl - EVAL_MARGIN4;
  }

  // 5. �㼶͵������(��ȫ����)�����������谭��
  return vl + this->KnightTrap();
}
