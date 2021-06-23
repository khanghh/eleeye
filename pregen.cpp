/* 
pregen.h/pregen.cpp - Source Code for ElephantEye, Part II

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

#include <string.h>
#include "base/base.h"
#include "pregen.h"

const bool cbcInBoard[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const bool cbcInFort[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const bool cbcCanPromote[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int8_t ccLegalSpanTab[512] = {
                       0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

const int8_t ccKnightPinTab[512] = {
                               0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0
};

PreGenStruct PreGen;
PreEvalStruct PreEval;

// ���ĸ����������ж����ӵ����ӷ�������Ϊ�����ǣ�sqDst = sqSrc + cnKnightMoveTab[i]
static const int cnKingMoveTab[4]    = {-0x10, -0x01, +0x01, +0x10};
static const int cnAdvisorMoveTab[4] = {-0x11, -0x0f, +0x0f, +0x11};
static const int cnBishopMoveTab[4]  = {-0x22, -0x1e, +0x1e, +0x22};
static const int cnKnightMoveTab[8]  = {-0x21, -0x1f, -0x12, -0x0e, +0x0e, +0x12, +0x1f, +0x21};

void PreGenInit(void) {
  int i, j, k, n, sqSrc, sqDst;
  RC4Struct rc4;
  SlideMoveStruct smv;
  SlideMaskStruct sms;

  // ���ȳ�ʼ��Zobrist��ֵ��
  rc4.InitZero();
  PreGen.zobrPlayer.InitRC4(rc4);
  for (i = 0; i < 14; i ++) {
    for (j = 0; j < 256; j ++) {
      PreGen.zobrTable[i][j].InitRC4(rc4);
    }
  }

  // Ȼ���ʼ������λ�к�����λ��
  // ע��λ�к�λ�в��������������λ�����Ծͻ�Ƶ��ʹ��"+/- RANK_TOP/FILE_LEFT"
  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
    if (IN_BOARD(sqSrc)) {
      PreGen.wBitRankMask[sqSrc] = 1 << (FILE_X(sqSrc) - FILE_LEFT);
      PreGen.wBitFileMask[sqSrc] = 1 << (RANK_Y(sqSrc) - RANK_TOP);
    } else {
      PreGen.wBitRankMask[sqSrc] = 0;
      PreGen.wBitFileMask[sqSrc] = 0;
    }
  }

  // Ȼ�����ɳ��ں����Ԥ������(�����Ӧ�ò���"pregen.h")
  for (i = 0; i < 9; i ++) {
    for (j = 0; j < 512; j ++) {
      // ��ʼ�������ڡ�λ�С��ĳ����ڵ��ŷ�Ԥ�������飬�������¼������裺
      // 1. ��ʼ����ʱ����"SlideMoveTab"������û���ŷ�������ʼ�����
      smv.ucNonCap[0] = smv.ucNonCap[1] = smv.ucRookCap[0] = smv.ucRookCap[1] =
      smv.ucCannonCap[0] = smv.ucCannonCap[1] = smv.ucSuperCap[0] = smv.ucSuperCap[1] = i + FILE_LEFT;
      sms.wNonCap = sms.wRookCap = sms.wCannonCap = sms.wSuperCap = 0;
      // ��ʾ������"pregen.h"��...[0]��ʾ���һ�������ƶ������ƶ�����[0]����֮��Ȼ
      // 2. ���������ƶ���Ŀ������...[0]��
      for (k = i + 1; k <= 8; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucRookCap[0] = FILE_DISP(k + FILE_LEFT);
          sms.wRookCap |= 1 << k;
          break;
        }
        smv.ucNonCap[0] = FILE_DISP(k + FILE_LEFT);
        sms.wNonCap |= 1 << k;
      }
      for (k ++; k <= 8; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucCannonCap[0] = FILE_DISP(k + FILE_LEFT);
          sms.wCannonCap |= 1 << k;
          break;
        }
      }
      for (k ++; k <= 8; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucSuperCap[0] = FILE_DISP(k + FILE_LEFT);
          sms.wSuperCap |= 1 << k;
          break;
        }
      }
      // 3. ���������ƶ���Ŀ������...[1]
      for (k = i - 1; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucRookCap[1] = FILE_DISP(k + FILE_LEFT);
          sms.wRookCap |= 1 << k;
          break;
        }
        smv.ucNonCap[1] = FILE_DISP(k + FILE_LEFT);
        sms.wNonCap |= 1 << k;
      }
      for (k --; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucCannonCap[1] = FILE_DISP(k + FILE_LEFT);
          sms.wCannonCap |= 1 << k;
          break;
        }
      }
      for (k --; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucSuperCap[1] = FILE_DISP(k + FILE_LEFT);
          sms.wSuperCap |= 1 << k;
          break;
        }
      }
      // 4. Ϊ"smv"��"sms"��ֵ������
      __ASSERT_BOUND_2(3, smv.ucNonCap[1], smv.ucNonCap[0], 11);
      __ASSERT_BOUND_2(3, smv.ucRookCap[1], smv.ucRookCap[0], 11);
      __ASSERT_BOUND_2(3, smv.ucCannonCap[1], smv.ucCannonCap[0], 11);
      __ASSERT_BOUND_2(3, smv.ucSuperCap[1], smv.ucSuperCap[0], 11);
      __ASSERT_BITRANK(sms.wNonCap);
      __ASSERT_BITRANK(sms.wRookCap);
      __ASSERT_BITRANK(sms.wCannonCap);
      __ASSERT_BITRANK(sms.wSuperCap);
      // 5. ����ʱ����"smv"��"sms"�������ŷ�Ԥ����������
      PreGen.smvRankMoveTab[i][j] = smv;
      PreGen.smsRankMaskTab[i][j] = sms;
    }
  }

  // Ȼ�����ɳ��������Ԥ������(�����Ӧ�ò���"pregen.h")
  for (i = 0; i < 10; i ++) {
    for (j = 0; j < 1024; j ++) {
      // ��ʼ�������ڡ�λ�С��ĳ����ڵ��ŷ�Ԥ�������飬�������¼������裺
      // 1. ��ʼ����ʱ����"smv"������û���ŷ�������ʼ�����
      smv.ucNonCap[0] = smv.ucNonCap[1] = smv.ucRookCap[0] = smv.ucRookCap[1] =
      smv.ucCannonCap[0] = smv.ucCannonCap[1] = smv.ucSuperCap[0] = smv.ucSuperCap[1] = (i + RANK_TOP) * 16;
      sms.wNonCap = sms.wRookCap = sms.wCannonCap = sms.wSuperCap = 0;
      // 2. ���������ƶ���Ŀ������...[0]
      for (k = i + 1; k <= 9; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucRookCap[0] = RANK_DISP(k + RANK_TOP);
          sms.wRookCap |= 1 << k;
          break;
        }
        smv.ucNonCap[0] = RANK_DISP(k + RANK_TOP);
        sms.wNonCap |= 1 << k;
      }
      for (k ++; k <= 9; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucCannonCap[0] = RANK_DISP(k + RANK_TOP);
          sms.wCannonCap |= 1 << k;
          break;
        }
      }
      for (k ++; k <= 9; k ++) {
        if ((j & (1 << k)) != 0) {
          smv.ucSuperCap[0] = RANK_DISP(k + RANK_TOP);
          sms.wSuperCap |= 1 << k;
          break;
        }
      }
      // 3. ���������ƶ���Ŀ������...[1]
      for (k = i - 1; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucRookCap[1] = RANK_DISP(k + RANK_TOP);
          sms.wRookCap |= 1 << k;
          break;
        }
        smv.ucNonCap[1] = RANK_DISP(k + RANK_TOP);
        sms.wNonCap |= 1 << k;
      }
      for (k --; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucCannonCap[1] = RANK_DISP(k + RANK_TOP);
          sms.wCannonCap |= 1 << k;
          break;
        }
      }
      for (k --; k >= 0; k --) {
        if ((j & (1 << k)) != 0) {
          smv.ucSuperCap[1] = RANK_DISP(k + RANK_TOP);
          sms.wSuperCap |= 1 << k;
          break;
        }
      }
      // 4. Ϊ"smv"��"sms"��ֵ������
      __ASSERT_BOUND_2(3, smv.ucNonCap[1] >> 4, smv.ucNonCap[0] >> 4, 12);
      __ASSERT_BOUND_2(3, smv.ucRookCap[1] >> 4, smv.ucRookCap[0] >> 4, 12);
      __ASSERT_BOUND_2(3, smv.ucCannonCap[1] >> 4, smv.ucCannonCap[0] >> 4, 12);
      __ASSERT_BOUND_2(3, smv.ucSuperCap[1] >> 4, smv.ucSuperCap[0] >> 4, 12);
      __ASSERT_BITFILE(sms.wNonCap);
      __ASSERT_BITFILE(sms.wRookCap);
      __ASSERT_BITFILE(sms.wCannonCap);
      __ASSERT_BITFILE(sms.wSuperCap);
      // 5. ����ʱ����"smv"��"sms"�������ŷ�Ԥ����������
      PreGen.smvFileMoveTab[i][j] = smv;
      PreGen.smsFileMaskTab[i][j] = sms;
    }
  }

  // �����������ŷ�Ԥ�������飬��ͬ����Ԥ������
  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
    if (IN_BOARD(sqSrc)) {
      // ����˧(��)���ŷ�Ԥ��������
      n = 0;
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + cnKingMoveTab[i];
        if (IN_FORT(sqDst)) {
          PreGen.ucsqKingMoves[sqSrc][n] = sqDst;
          n ++;
        }
      }
      __ASSERT(n <= 4);
      PreGen.ucsqKingMoves[sqSrc][n] = 0;
      // ������(ʿ)���ŷ�Ԥ��������
      n = 0;
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + cnAdvisorMoveTab[i];
        if (IN_FORT(sqDst)) {
          PreGen.ucsqAdvisorMoves[sqSrc][n] = sqDst;
          n ++;
        }
      }
      __ASSERT(n <= 4);
      PreGen.ucsqAdvisorMoves[sqSrc][n] = 0;
      // ������(��)���ŷ�Ԥ�������飬������������
      n = 0;
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + cnBishopMoveTab[i];
        if (IN_BOARD(sqDst) && SAME_HALF(sqSrc, sqDst)) {
          PreGen.ucsqBishopMoves[sqSrc][n] = sqDst;
          PreGen.ucsqBishopPins[sqSrc][n] = BISHOP_PIN(sqSrc, sqDst);
          n ++;
        }
      }
      __ASSERT(n <= 4);
      PreGen.ucsqBishopMoves[sqSrc][n] = 0;
      // ���������ŷ�Ԥ�������飬������������
      n = 0;
      for (i = 0; i < 8; i ++) {
        sqDst = sqSrc + cnKnightMoveTab[i];
        if (IN_BOARD(sqDst)) {
          PreGen.ucsqKnightMoves[sqSrc][n] = sqDst;
          PreGen.ucsqKnightPins[sqSrc][n] = KNIGHT_PIN(sqSrc, sqDst);
          n ++;
        }
      }
      __ASSERT(n <= 8);
      PreGen.ucsqKnightMoves[sqSrc][n] = 0;
      // ���ɱ�(��)���ŷ�Ԥ��������
      for (i = 0; i < 2; i ++) {
        n = 0;
        sqDst = SQUARE_FORWARD(sqSrc, i);
        sqDst = sqSrc + (i == 0 ? -16 : 16);
        if (IN_BOARD(sqDst)) {
          PreGen.ucsqPawnMoves[i][sqSrc][n] = sqDst;
          n ++;
        }
        if (AWAY_HALF(sqSrc, i)) {
          for (j = -1; j <= 1; j += 2) {
            sqDst = sqSrc + j;
            if (IN_BOARD(sqDst)) {
              PreGen.ucsqPawnMoves[i][sqSrc][n] = sqDst;
              n ++;
            }
          }
        }
        __ASSERT(n <= 3);
        PreGen.ucsqPawnMoves[i][sqSrc][n] = 0;
      }
    }
  }

  // �����վ���Ԥ���۽ṹ
  memset(&PreEval, 0, sizeof(PreEvalStruct));
  PreEval.bPromotion = false; // ȱʡ�ǲ����������
}
