#include <assert.h>
#include <sys/timeb.h>

#ifndef BASE_H
#define BASE_H

#define NoPopLeafInfo

#ifdef _MSC_VER
  typedef signed   __int64  int64_t; // ll
  typedef unsigned __int64 uint64_t; // qw
  typedef signed   __int32  int32_t; // l
  typedef unsigned __int32 uint32_t; // dw
  typedef signed   __int16  int16_t; // s
  typedef unsigned __int16 uint16_t; // w
  typedef signed   __int8   int8_t;  // c
  typedef unsigned __int8  uint8_t;  // uc
  #define FORMAT_I64 "I64"
#else
  #include <stdint.h>
  #define FORMAT_I64 "ll"
#endif

#define __ASSERT(a) assert(a)
#define __ASSERT_BOUND(a, b, c) assert((a) <= (b) && (b) <= (c))
#define __ASSERT_BOUND_2(a, b, c, d) assert((a) <= (b) && (b) <= (c) && (c) <= (d))

inline bool EQV(bool bArg1, bool bArg2) {
  return bArg1 ? bArg2 : !bArg2;
}

inline bool XOR(bool bArg1, bool bArg2) {
  return bArg1 ? !bArg2 : bArg2;
}

template <typename T> inline T MIN(T Arg1, T Arg2) {
  return Arg1 < Arg2 ? Arg1 : Arg2;
}

template <typename T> inline T MAX(T Arg1, T Arg2) {
  return Arg1 > Arg2 ? Arg1 : Arg2;
}

template <typename T> inline T ABS(T Arg) {
  return Arg < 0 ? -Arg : Arg;
}

template <typename T> inline T SQR(T Arg) {
  return Arg * Arg;
}

template <typename T> inline void SWAP(T &Arg1, T &Arg2) {
  T Temp;
  Temp = Arg1;
  Arg1 = Arg2;
  Arg2 = Temp;
}

inline int PopCnt8(uint8_t uc) {
  int n;
  n = ((uc >> 1) & 0x55) + (uc & 0x55);
  n = ((n >> 2) & 0x33) + (n & 0x33);
  return (n >> 4) + (n & 0x0f);
}

inline int PopCnt16(uint16_t w) {
  int n;
  n = ((w >> 1) & 0x5555) + (w & 0x5555);
  n = ((n >> 2) & 0x3333) + (n & 0x3333);
  n = ((n >> 4) & 0x0f0f) + (n & 0x0f0f);
  return (n >> 8) + (n & 0x00ff); 
}

inline int PopCnt32(uint32_t dw) {
  int n;
  n = ((dw >> 1) & 0x55555555) + (dw & 0x55555555);
  n = ((n >> 2) & 0x33333333) + (n & 0x33333333);
  n = ((n >> 4) & 0x0f0f0f0f) + (n & 0x0f0f0f0f);
  n = ((n >> 8) & 0x00ff00ff) + (n & 0x00ff00ff);
  return (n >> 16) + (n & 0x0000ffff);
}

inline int64_t GetTime() {
  timeb tb;
  ftime(&tb);
  return (int64_t) tb.time * 1000 + tb.millitm;
}

inline uint32_t LOW_LONG(uint64_t Operand) {
	return (uint32_t)Operand;
}

inline uint32_t HIGH_LONG(uint64_t Operand) {
	return (uint32_t)(Operand >> 32);
}

inline uint64_t MAKE_LONG_LONG(uint32_t LowLong, uint32_t HighLong) {
	return (uint64_t)LowLong | ((uint64_t)HighLong << 32);
}

static int cnBitScanTable[64] = {
  32,  0,  1, 12,  2,  6, -1, 13,  3, -1,  7, -1, -1, -1, -1, 14,
  10,  4, -1, -1,  8, -1, -1, 25, -1, -1, -1, -1, -1, 21, 27, 15,
  31, 11,  5, -1, -1, -1, -1, -1,  9, -1, -1, 24, -1, -1, 20, 26,
  30, -1, -1, -1, -1, 23, -1, 19, 29, -1, 22, 18, 28, 17, 16, -1
};

inline int BitScan(uint32_t Operand) {
	uint32_t dw = (Operand << 4) + Operand;
	dw += dw << 6;
	dw = (dw << 16) - dw;
	return cnBitScanTable[dw >> 26];
}

//inline int Bsf(uint32_t Operand) {
	//return BitScan(Operand & -Operand);
//}

inline int Bsr(uint32_t Operand) {
	uint32_t dw = Operand | (Operand >> 1);
	dw |= dw >> 2;
	dw |= dw >> 4;
	dw |= dw >> 8;
	dw |= dw >> 16;
	return BitScan(dw - (dw >> 1));
}

#endif
