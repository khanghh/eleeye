/*
movesort.h/movesort.cpp - Source Code for ElephantEye, Part VII

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.11, Last Modified: Dec. 2007
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

#ifndef MOVESORT_H
#define MOVESORT_H

#include <string.h>
#include "base/base.h"
#include "position.h"

const int LIMIT_DEPTH = 64;       // �����ļ������
const int SORT_VALUE_MAX = 65535; // �ŷ��������ֵ

extern const int FIBONACCI_LIST[32];

// "nHistory"ֻ��"movesort.cpp"һ��ģ����ʹ��
extern int nHistory[65536]; // ��ʷ��

// �ŷ�˳������ɽ׶�(����"NextFull()"����)
const int PHASE_HASH = 0;
const int PHASE_GEN_CAP = 1;
const int PHASE_GOODCAP = 2;
const int PHASE_KILLER1 = 3;
const int PHASE_KILLER2 = 4;
const int PHASE_GEN_NONCAP = 5;
const int PHASE_REST = 6;

const bool NEXT_ALL = true;    // �ŷ�˳����"MoveSortStruct::NextQuiesc()"ѡ��
const bool ROOT_UNIQUE = true; // �ŷ�˳����"MoveSortStruct::ResetRoot()"ѡ��

// �ŷ����нṹ
struct MoveSortStruct {
  int nPhase, nMoveIndex, nMoveNum;
  int mvHash, mvKiller1, mvKiller2;
  MoveStruct mvs[MAX_GEN_MOVES];

  void SetHistory(void); // ������ʷ�����ŷ��б���ֵ
  void ShellSort(void);  // �ŷ��������
  // �õĳ����ŷ�(����û���ŷ�������������ʷ����ɱ���ŷ���)
  bool GoodCap(const PositionStruct &pos, int mv) {
    return mv == 0 || nPhase == PHASE_GOODCAP || (nPhase < PHASE_GOODCAP && pos.GoodCap(mv));
  }

  // ��̬�������ŷ�˳�����
  void InitAll(const PositionStruct &pos) {
    nMoveIndex = 0;
    nMoveNum = pos.GenAllMoves(mvs);
    SetHistory();
    ShellSort();
  }
  void InitQuiesc(const PositionStruct &pos) {
    nMoveIndex = 0;
    nMoveNum = pos.GenCapMoves(mvs);
    ShellSort();
  }
  void InitQuiesc2(const PositionStruct &pos) {
    nMoveNum += pos.GenNonCapMoves(mvs);
    SetHistory();
    ShellSort();
  }
  int NextQuiesc(bool bNextAll = false) {
    if (nMoveIndex < nMoveNum && (bNextAll || mvs[nMoveIndex].wvl > 0)) {
      nMoveIndex ++;
      return mvs[nMoveIndex - 1].wmv;
    } else {
      return 0;
    }
  }

  // ��ȫ�������ŷ�˳�����
  void InitFull(const PositionStruct &pos, int mv, const uint16_t *lpwmvKiller) {
    nPhase = PHASE_HASH;
    mvHash = mv;
    mvKiller1 = lpwmvKiller[0];
    mvKiller2 = lpwmvKiller[1];
  }
  int InitEvade(PositionStruct &pos, int mv, const uint16_t *lpwmvKiller);
  int NextFull(const PositionStruct &pos);

  // ������ŷ�˳�����
  void InitRoot(const PositionStruct &pos, int nBanMoves, const uint16_t *lpwmvBanList);
  void ResetRoot(bool bUnique = false) {
    nMoveIndex = 0;
    ShellSort();
    nMoveIndex = (bUnique ? 1 : 0);
  }
  int NextRoot(void) {
    if (nMoveIndex < nMoveNum) {
      nMoveIndex ++;
      return mvs[nMoveIndex - 1].wmv;
    } else {
      return 0;
    }
  }
  void UpdateRoot(int mv);
};

// �����ʷ��
inline void ClearHistory(void) {
  memset(nHistory, 0, sizeof(int[65536]));
}

// ���ɱ���ŷ���
inline void ClearKiller(uint16_t (*lpwmvKiller)[2]) {
  memset(lpwmvKiller, 0, LIMIT_DEPTH * sizeof(uint16_t[2]));
}

// ����ɱ���ŷ���
inline void CopyKiller(uint16_t (*lpwmvDst)[2], const uint16_t (*lpwmvSrc)[2]) {
  memcpy(lpwmvDst, lpwmvSrc, LIMIT_DEPTH * sizeof(uint16_t[2]));
}
     
/* �ҵ�����ŷ�ʱ��ȡ�Ĵ�ʩ
 *
 * ��ʷ�������������������¼���ѡ��
 * 1. ƽ����ϵ(n^2)��
 * 2. ָ����ϵ(2^n)��
 * 3. Fibonacci���У�
 * 4. ���ϼ����������ϣ����磺n^2 + 2^n���ȵȡ�
 * ElephantEyeʹ���ͳ��ƽ����ϵ��
 */
inline void SetBestMove(int mv, int nDepth, uint16_t *lpwmvKiller) {
  nHistory[mv] += SQR(nDepth);
  if (lpwmvKiller[0] != mv) {
    lpwmvKiller[1] = lpwmvKiller[0];
    lpwmvKiller[0] = mv;
  }
}

#endif
