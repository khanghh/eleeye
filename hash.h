/*
hash.h/hash.cpp - Source Code for ElephantEye, Part V

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.31, Last Modified: May 2012
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
#include "position.h"

#ifndef HASH_H
#define HASH_H

// �û�����־��ֻ����"RecordHash()"������
const int HASH_BETA = 1;
const int HASH_ALPHA = 2;
const int HASH_PV = HASH_ALPHA | HASH_BETA;

const int HASH_LAYERS = 2;   // �û����Ĳ���
const int NULL_DEPTH = 2;    // ���Ųü������

// �û����ṹ���û�����Ϣ��������ZobristУ�����м䣬���Է�ֹ��ȡ��ͻ
struct HashStruct {
  uint32_t dwZobristLock0;           // ZobristУ��������һ����
  uint16_t wmv;                      // ����ŷ�
  uint8_t ucAlphaDepth, ucBetaDepth; // ���(�ϱ߽���±߽�)
  int16_t svlAlpha, svlBeta;         // ��ֵ(�ϱ߽���±߽�)
  uint32_t dwZobristLock1;           // ZobristУ�������ڶ�����
}; // hsh

// �û�����Ϣ
extern int nHashMask;              // �û����Ĵ�С
extern HashStruct *hshItems;       // �û�����ָ�룬ElephantEye���ö����û���
#ifdef HASH_QUIESC
  extern HashStruct *hshItemsQ;
#endif

inline void ClearHash(void) {         // ����û���
  memset(hshItems, 0, (nHashMask + 1) * sizeof(HashStruct));
#ifdef HASH_QUIESC
  memset(hshItemsQ, 0, (nHashMask + 1) * sizeof(HashStruct));
#endif
}

inline void NewHash(int nHashScale) { // �����û�������С�� 2^nHashScale �ֽ�
  nHashMask = ((1 << nHashScale) / sizeof(HashStruct)) - 1;
  hshItems = new HashStruct[nHashMask + 1];
#ifdef HASH_QUIESC
  hshItemsQ = new HashStruct[nHashMask + 1];
#endif
  ClearHash();
}

inline void DelHash(void) {           // �ͷ��û���
  delete[] hshItems;
#ifdef HASH_QUIESC
  delete[] hshItemsQ;
#endif
}

// �ж��û����Ƿ���Ͼ���(Zobrist���Ƿ����)
inline bool HASH_POS_EQUAL(const HashStruct &hsh, const PositionStruct &pos) {
  return hsh.dwZobristLock0 == pos.zobr.dwLock0 && hsh.dwZobristLock1 == pos.zobr.dwLock1;
}

// ������Ͳ�����ȡ�û�����(����һ�����ã����Զ��丳ֵ)
inline HashStruct &HASH_ITEM(const PositionStruct &pos, int nLayer) {
  return hshItems[(pos.zobr.dwKey + nLayer) & nHashMask];
}

// �û����Ĺ�������
void RecordHash(const PositionStruct &pos, int nFlag, int vl, int nDepth, int mv);                    // �洢�û���������Ϣ
int ProbeHash(const PositionStruct &pos, int vlAlpha, int vlBeta, int nDepth, bool bNoNull, int &mv); // ��ȡ�û���������Ϣ
#ifdef HASH_QUIESC
  void RecordHashQ(const PositionStruct &pos, int vlBeta, int vlAlpha); // �洢�û���������Ϣ(��̬����)
  int ProbeHashQ(const PositionStruct &pos, int vlAlpha, int vlBeta);   // ��ȡ�û���������Ϣ(��̬����)
#endif

#ifndef CCHESS_A3800
  // UCCI֧�� - ���Hash���еľ�����Ϣ
  bool PopHash(const PositionStruct &pos);
#endif

#endif
