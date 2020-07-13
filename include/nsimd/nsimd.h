/*

Copyright (c) 2020 Agenium Scale

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef NSIMD_H
#define NSIMD_H

/* clang-format off */

/* ------------------------------------------------------------------------- */
/* Compiler detection (order matters https://stackoverflow.com/a/28166605)   */

#if defined(_MSC_VER)
  #define NSIMD_IS_MSVC
#elif defined(__HIPCC__)
  #define NSIMD_IS_HIPCC
#elif defined(__NVCC__) || defined(__CUDACC__)
  #define NSIMD_IS_NVCC
#elif defined(__INTEL_COMPILER)
  #define NSIMD_IS_ICC
#elif defined(__clang__)
  #define NSIMD_IS_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
  #define NSIMD_IS_GCC
#endif

/* ------------------------------------------------------------------------- */
/* C standard detection */

#ifdef NSIMD_IS_MSVC
  #define NSIMD_C 1999
#else
  #ifdef __STDC_VERSION__
    #if __STDC_VERSION__ == 199901L
      #define NSIMD_C 1999
    #elif __STDC_VERSION__ >= 201112L
      #define NSIMD_C 2011
    #else
      #define NSIMD_C 1989
    #endif
  #else
    #define NSIMD_C 1989
  #endif
#endif

/* ------------------------------------------------------------------------- */
/* C++ standard detection */

#ifdef NSIMD_IS_MSVC
  #define NSIMD__cplusplus _MSVC_LANG
#else
  #ifdef __cplusplus
    #define NSIMD__cplusplus __cplusplus
  #else
    #define NSIMD__cplusplus 0
  #endif
#endif

#if NSIMD__cplusplus > 0 && NSIMD__cplusplus < 201103L
  #define NSIMD_CXX 1998
#elif NSIMD__cplusplus >= 201103L && NSIMD__cplusplus < 201402L
  #define NSIMD_CXX 2011
#elif NSIMD__cplusplus >= 201402L && NSIMD__cplusplus < 201703L
  #define NSIMD_CXX 2014
#elif NSIMD__cplusplus >= 201703L
  #define NSIMD_CXX 2017
#else
  #define NSIMD_CXX 0
#endif

/* ------------------------------------------------------------------------- */
/* Use of long long for GCC even in C89 and C++98. Note that for some reason */
/* the use of the __extension__ keyword does not prevent warning so we deal  */
/* with them now. We keep the __extension__ keyword in case.                 */

#if NSIMD_CXX < 2011 && NSIMD_C < 1999
  #define NSIMD_LONGLONG_IS_EXTENSION
#endif

#ifdef NSIMD_LONGLONG_IS_EXTENSION
  #if defined(NSIMD_IS_GCC)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wlong-long"
  #elif defined(NSIMD_IS_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wlong-long"
  #endif
#endif

typedef long long nsimd_longlong;
typedef unsigned long long nsimd_ulonglong;

#if NSIMD_CXX > 0
namespace nsimd {
  typedef long long longlong;
  typedef unsigned long long ulonglong;
} // namespace nsimd
#endif

#ifdef NSIMD_LONGLONG_IS_EXTENSION
  #if defined(NSIMD_IS_GCC)
    #pragma GCC diagnostic pop
  #elif defined(NSIMD_IS_CLANG)
    #pragma clang diagnostic pop
  #endif
#endif

/* ------------------------------------------------------------------------- */
/* Register size detection */

#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) ||         \
    defined(__amd64) || defined(_M_AMD64) || defined(__aarch64__) ||          \
    defined(_M_ARM64) || defined(__PPC64__)
  #define NSIMD_WORD_SIZE 64
#else
  #define NSIMD_WORD_SIZE 32
#endif

/* ------------------------------------------------------------------------- */
/* Architecture detection */

#if defined(i386) || defined(__i386__) || defined(__i486__) ||                \
    defined(__i586__) || defined(__i686__) || defined(__i386) ||              \
    defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) ||           \
    defined(__I86__) || defined(__INTEL__) || defined(__x86_64) ||            \
    defined(__x86_64__) || defined(__amd64__) || defined(__amd64) ||          \
    defined(_M_X64)
  #define NSIMD_X86
#elif defined(__arm__) || defined(__arm64) || defined(__thumb__) ||           \
    defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) ||             \
    defined(_M_ARM) || defined(_M_ARM64) || defined(__arch64__)
  #define NSIMD_ARM
#elif defined(__ppc__) || defined(__powerpc__) || defined(__PPC__)
  #define NSIMD_POWERPC
#else
  #define NSIMD_CPU
#endif

/* ------------------------------------------------------------------------- */
/* Microsoft DLL specifics */

#ifdef NSIMD_IS_MSVC
  #define NSIMD_DLLEXPORT __declspec(dllexport)
  #define NSIMD_DLLIMPORT __declspec(dllimport)
#else
  #define NSIMD_DLLEXPORT
  #define NSIMD_DLLIMPORT extern
#endif

/* ------------------------------------------------------------------------- */
/* DLL specifics when inside/outside the library */

#ifdef NSIMD_INSIDE
  #define NSIMD_DLLSPEC NSIMD_DLLEXPORT
#else
  #define NSIMD_DLLSPEC NSIMD_DLLIMPORT
#endif

/* ------------------------------------------------------------------------- */
/* inline in nsimd is ONLY useful for linkage */

#if NSIMD_CXX > 0 || NSIMD_C > 1989
  #if NSIMD_C > 0 && defined(NSIMD_IS_MSVC)
    #define NSIMD_INLINE static __inline
  #else
    #define NSIMD_INLINE static inline
  #endif
#else
  #if defined(NSIMD_IS_GCC) || defined(NSIMD_IS_CLANG)
    #define NSIMD_INLINE __extension__ __inline__
  #else
    #define NSIMD_INLINE
  #endif
#endif

/* ------------------------------------------------------------------------- */
/* Pre-processor */

#define NSIMD_PP_CAT_2_e(a, b) a##b
#define NSIMD_PP_CAT_2(a, b) NSIMD_PP_CAT_2_e(a, b)

#define NSIMD_PP_CAT_3_e(a, b, c) a##b##c
#define NSIMD_PP_CAT_3(a, b, c) NSIMD_PP_CAT_3_e(a, b, c)

#define NSIMD_PP_CAT_4_e(a, b, c, d) a##b##c##d
#define NSIMD_PP_CAT_4(a, b, c, d) NSIMD_PP_CAT_4_e(a, b, c, d)

#define NSIMD_PP_CAT_5_e(a, b, c, d, e) a##b##c##d##e
#define NSIMD_PP_CAT_5(a, b, c, d, e) NSIMD_PP_CAT_5_e(a, b, c, d, e)

#define NSIMD_PP_CAT_6_e(a, b, c, d, e, f) a##b##c##d##e##f
#define NSIMD_PP_CAT_6(a, b, c, d, e, f) NSIMD_PP_CAT_6_e(a, b, c, d, e, f)

#define NSIMD_PP_EXPAND_e(a) a
#define NSIMD_PP_EXPAND(a) NSIMD_PP_EXPAND_e(a)

/* ------------------------------------------------------------------------- */
/* Detect architecture/SIMD */

/* Intel */

#if defined(SSE2) && !defined(NSIMD_SSE2)
  #define NSIMD_SSE2
#endif

#if defined(SSE42) && !defined(NSIMD_SSE42)
  #define NSIMD_SSE42
#endif

#if defined(AVX) && !defined(NSIMD_AVX)
  #define NSIMD_AVX
#endif

#if defined(AVX2) && !defined(NSIMD_AVX2)
  #define NSIMD_AVX2
#endif

#if defined(AVX512_KNL) && !defined(NSIMD_AVX512_KNL)
  #define NSIMD_AVX512_KNL
#endif

#if defined(AVX512_SKYLAKE) && !defined(NSIMD_AVX512_SKYLAKE)
  #define NSIMD_AVX512_SKYLAKE
#endif

#if defined(FP16) && !defined(NSIMD_FP16)
  #define NSIMD_FP16
#endif

#if defined(FMA) && !defined(NSIMD_FMA)
  #define NSIMD_FMA
#endif

/* ARM */

#if defined(NEON128) && !defined(NSIMD_NEON128)
  #define NSIMD_NEON128
#endif

#if defined(AARCH64) && !defined(NSIMD_AARCH64)
  #define NSIMD_AARCH64
#endif

#if defined(SVE) && !defined(NSIMD_SVE)
  #define NSIMD_SVE
  #define NSIMD_SVE_FAMILY
#endif

#if defined(SVE128) && !defined(NSIMD_SVE128)
  #define NSIMD_SVE128
  #define NSIMD_SVE_FAMILY
#endif

#if defined(SVE256) && !defined(NSIMD_SVE256)
  #define NSIMD_SVE256
  #define NSIMD_SVE_FAMILY
#endif

#if defined(SVE512) && !defined(NSIMD_SVE512)
  #define NSIMD_SVE512
  #define NSIMD_SVE_FAMILY
#endif

#if defined(SVE1024) && !defined(NSIMD_SVE1024)
  #define NSIMD_SVE1024
  #define NSIMD_SVE_FAMILY
#endif

#if defined(SVE2048) && !defined(NSIMD_SVE2048)
  #define NSIMD_SVE2048
  #define NSIMD_SVE_FAMILY
#endif

/* PPC */

#if (defined(POWER8) || defined(ALTIVEC)) && !defined(NSIMD_POWER8)
#define NSIMD_POWER8
#endif

#if defined(POWER7) && !defined(NSIMD_POWER7)
#define NSIMD_POWER7
#endif

/* CUDA */

#if defined(CUDA) && !defined(NSIMD_CUDA)
  #define NSIMD_CUDA
#endif

#if defined(NSIMD_CUDA) && (defined(NSIMD_IS_NVCC) || defined(NSIMD_IS_HIPCC))
  #define NSIMD_CUDA_COMPILING_FOR_DEVICE
#endif

/* ROCm */

#if defined(ROCM) && !defined(NSIMD_ROCM)
  #define NSIMD_ROCM
#endif

#if defined(NSIMD_ROCM) && defined(NSIMD_IS_HIPCC)
  #define NSIMD_ROCM_COMPILING_FOR_DEVICE
#endif

/* ------------------------------------------------------------------------- */
/* Set NSIMD_SIMD and NSIMD_PLATFORM macro, include the correct header. */

#if defined(NSIMD_SSE2)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD sse2
  #include <emmintrin.h>
  #if defined(NSIMD_FMA) || defined(NSIMD_FP16)
    #include <immintrin.h>
  #endif
  /* For some reason MSVC <= 2015 has intrinsics defined in another header */
  #ifdef NSIMD_IS_MSVC
    #include <intrin.h>
  #endif

#elif defined(NSIMD_SSE42)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD sse42
  #include <nmmintrin.h>
  #if defined(NSIMD_FMA) || defined(NSIMD_FP16)
    #include <immintrin.h>
  #endif
  /* For some reason MSVC <= 2015 has intrinsics defined in another header */
  #ifdef NSIMD_IS_MSVC
    #include <intrin.h>
  #endif

#elif defined(NSIMD_AVX)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD avx
  #include <immintrin.h>
  /* For some reason MSVC <= 2015 has intrinsics defined in another header */
  #ifdef NSIMD_IS_MSVC
    #include <intrin.h>
  #endif

#elif defined(NSIMD_AVX2)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD avx2
  #include <immintrin.h>
  /* For some reason MSVC <= 2015 has intrinsics defined in another header */
  #ifdef NSIMD_IS_MSVC
    #include <intrin.h>
  #endif

#elif defined(NSIMD_AVX512_KNL)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD avx512_knl
  #include <immintrin.h>

#elif defined(NSIMD_AVX512_SKYLAKE)

  #define NSIMD_PLATFORM x86
  #define NSIMD_SIMD avx512_skylake
  #include <immintrin.h>

#elif defined(NSIMD_NEON128)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD neon128
  #include <arm_neon.h>

#elif defined(NSIMD_AARCH64)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD aarch64
  #include <arm_neon.h>

#elif defined(NSIMD_SVE)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_SVE128)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve128
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_SVE256)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve256
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_SVE512)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve512
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_SVE1024)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve1024
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_SVE2048)

  #define NSIMD_PLATFORM arm
  #define NSIMD_SIMD sve2048
  #include <arm_neon.h>
  #include <arm_sve.h>

#elif defined(NSIMD_POWER7)

  #define NSIMD_PLATFORM ppc
  #define NSIMD_SIMD power7

  #ifdef NSIMD_IS_CLANG
  // New version of clang are spamming useless warning comming from their
  // altivec.h file
    #pragma clang diagnostic ignored "-Wc11-extensions"
    #pragma clang diagnostic ignored "-Wc++11-long-long"
  #endif

  #include <altivec.h>

  #ifdef bool
    #undef bool
  #endif
  #ifdef pixel
    #undef pixel
  #endif
  #ifdef vector
    #undef vector
  #endif

#else

  #ifdef NSIMD_CUDA_COMPILING_FOR_DEVICE
    #include <cuda_fp16.h>
  #endif

  #ifdef NSIMD_ROCM_COMPILING_FOR_DEVICE
    #include <hip/hip_runtime.h>
    #include <hip/hip_fp16.h>
  #endif

  #define NSIMD_SIMD cpu
  #define NSIMD_PLATFORM cpu

#endif

// ----------------------------------------------------------------------------
// For ARM SVE we need a special struct

#ifdef NSIMD_SVE
  #define NSIMD_STRUCT __sizeless_struct
#else
  #define NSIMD_STRUCT struct
#endif

/* ------------------------------------------------------------------------- */
/* Shorter typedefs for integers and their limits */

#if NSIMD_CXX > 0
  #include <climits>
#else
  #include <limits.h>
#endif

#ifdef NSIMD_IS_MSVC
  typedef unsigned __int8  u8;
  typedef signed   __int8  i8;
  typedef unsigned __int16 u16;
  typedef signed   __int16 i16;
  typedef unsigned __int32 u32;
  typedef signed   __int32 i32;
  typedef unsigned __int64 u64;
  typedef signed   __int64 i64;
#else
  typedef unsigned char  u8;
  typedef signed   char  i8;
  typedef unsigned short u16;
  typedef signed   short i16;
  typedef unsigned int   u32;
  typedef signed   int   i32;
  #if NSIMD_WORD_SIZE == 64
    typedef unsigned long u64;
    typedef signed long   i64;
  #else
    #if defined(NSIMD_IS_GCC) || defined(NSIMD_IS_CLANG)
      __extension__ typedef nsimd_ulonglong u64;
      __extension__ typedef nsimd_longlong i64;
    #else
      typedef unsigned long long u64;
      typedef signed long long   i64;
    #endif
  #endif
#endif

#define NSIMD_U8_MIN ((u8)0)
#define NSIMD_U8_MAX UCHAR_MAX
#define NSIMD_I8_MIN SCHAR_MIN
#define NSIMD_I8_MAX SCHAR_MAX
#define NSIMD_U16_MIN ((u16)0)
#define NSIMD_U16_MAX USHRT_MAX
#define NSIMD_I16_MIN SHRT_MIN
#define NSIMD_I16_MAX SHRT_MAX
#define NSIMD_U32_MIN ((u32)0)
#define NSIMD_U32_MAX UINT_MAX
#define NSIMD_I32_MIN INT_MIN
#define NSIMD_I32_MAX INT_MAX

#ifdef NSIMD_IS_MSVC
  #define NSIMD_U64_MIN ((u64)0)
  #define NSIMD_U64_MAX ULLONG_MAX
  #define NSIMD_I64_MIN LLONG_MIN
  #define NSIMD_I64_MAX LLONG_MAX
#else
  #if NSIMD_WORD_SIZE == 64
    #define NSIMD_U64_MIN ((u64)0)
    #define NSIMD_U64_MAX ULONG_MAX
    #define NSIMD_I64_MIN LONG_MIN
    #define NSIMD_I64_MAX LONG_MAX
  #else
    #define NSIMD_U64_MIN ((u64)0)
    #define NSIMD_U64_MAX (~((u64)0))
    #define NSIMD_I64_MIN ((i64)1 << 63)
    #define NSIMD_I64_MAX (~((i64)1 << 63))
  #endif
#endif

/* ------------------------------------------------------------------------- */
/* Shorter typedefs for floatting point types */

#if ((defined(NSIMD_NEON128) || defined(NSIMD_AARCH64)) &&                    \
     defined(NSIMD_FP16)) || defined(NSIMD_SVE_FAMILY)
  #define NSIMD_NATIVE_FP16
#endif

#ifdef NSIMD_NATIVE_FP16
  typedef __fp16 f16;
#elif defined(NSIMD_CUDA_COMPILING_FOR_DEVICE) || \
      defined(NSIMD_ROCM_COMPILING_FOR_DEVICE)
  typedef __half f16;
#else
  typedef struct { u16 u; } f16;
#endif

typedef float  f32;
typedef double f64;

/* ------------------------------------------------------------------------- */
/* Native register size (for now only 32 and 64 bits) types */

#if NSIMD_WORD_SIZE == 64
  typedef i64 nsimd_nat;
#else
  typedef i32 nsimd_nat;
#endif

#if NSIMD_CXX > 0
namespace nsimd {
typedef nsimd_nat nat;
} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* C++ traits for base types */

#if NSIMD_CXX > 0

namespace nsimd {

template <typename T> struct traits {};

// 8-bits

template <> struct traits<i8> {
  typedef i8 itype;
  typedef u8 utype;
};

template <> struct traits<u8> {
  typedef i8 itype;
  typedef u8 utype;
};

// 16-bits

template <> struct traits<i16> {
  typedef i16 itype;
  typedef u16 utype;
  typedef f16 ftype;
};

template <> struct traits<u16> {
  typedef i16 itype;
  typedef u16 utype;
  typedef f16 ftype;
};

template <> struct traits<f16> {
  typedef i16 itype;
  typedef u16 utype;
  typedef f16 ftype;
};

// 32-bits

template <> struct traits<i32> {
  typedef i32 itype;
  typedef u32 utype;
  typedef f32 ftype;
};

template <> struct traits<u32> {
  typedef i32 itype;
  typedef u32 utype;
  typedef f32 ftype;
};

template <> struct traits<f32> {
  typedef i32 itype;
  typedef u32 utype;
  typedef f32 ftype;
};

// 64-bits

template <> struct traits<i64> {
  typedef i64 itype;
  typedef u64 utype;
  typedef f64 ftype;
};

template <> struct traits<u64> {
  typedef i64 itype;
  typedef u64 utype;
  typedef f64 ftype;
};

template <> struct traits<f64> {
  typedef i64 itype;
  typedef u64 utype;
  typedef f64 ftype;
};

} // namespace nsimd

#endif

/* ------------------------------------------------------------------------- */
/* Set if denormalized float are set to 0                                    */

#ifdef NSIMD_NEON128
  #define NSIMD_DNZ_FLUSH_TO_ZERO
#endif

/* clang-format on */

/* ------------------------------------------------------------------------- */
/* POPCNT: GCC and Clang have intrinsics */

#if defined(NSIMD_IS_GCC) || defined(NSIMD_IS_CLANG)

NSIMD_INLINE int nsimd_popcnt32_(u32 a) { return __builtin_popcount(a); }

NSIMD_INLINE int nsimd_popcnt64_(u64 a) {
#if __SIZEOF_LONG__ == 4
  return __builtin_popcountl((u32)(a & 0xFFFFFFFF)) +
         __builtin_popcountl((u32)(a >> 32));
#else
  return __builtin_popcountl(a);
#endif
}

/* ------------------------------------------------------------------------- */
/* POPCNT: MSVC has also an intrinsic for that */

#elif defined(NSIMD_IS_MSVC)

#include <intrin.h>

NSIMD_INLINE int nsimd_popcnt32_(u32 a) { return (int)__popcnt(a); }

NSIMD_INLINE int nsimd_popcnt64_(u64 a) { return (int)__popcnt64(a); }

/* ------------------------------------------------------------------------- */
/* POPCNT: Default naive implementation */

#else

NSIMD_INLINE int nsimd_popcnt32_(u32 a) {
  int i, ret = 0;
  for (i = 0; i < 32; i++) {
    ret += (int)((a >> i) & 1);
  }
  return ret;
}

NSIMD_INLINE int nsimd_popcnt64_(u64 a) {
  int i, ret = 0;
  for (i = 0; i < 64; i++) {
    ret += (int)((a >> i) & 1);
  }
  return ret;
}

#endif

/* ------------------------------------------------------------------------- */
/* Macro to automatically include function depending on detected
   platform/SIMD */

#define NSIMD_AUTO_INCLUDE(path) <nsimd/NSIMD_PLATFORM/NSIMD_SIMD/path>

/* ------------------------------------------------------------------------- */
/* Standard includes */

/* clang-format off */

#if NSIMD_CXX > 0
  #include <cerrno>
  #include <cstdlib>
#else
  #include <errno.h>
  #include <stdlib.h>
#endif

/* clang-format on */

/* ------------------------------------------------------------------------- */
/* Now includes detected SIMD types */

#if NSIMD_CXX > 0

namespace nsimd {
template <typename T, typename SimdExt> struct simd_traits {};
} // namespace nsimd

#endif

#include NSIMD_AUTO_INCLUDE(types.h)

/* ------------------------------------------------------------------------- */
/* Macro/typedefs for SIMD infos */

#define vec(T) NSIMD_PP_CAT_4(nsimd_, NSIMD_SIMD, _v, T)
#define vecl(T) NSIMD_PP_CAT_4(nsimd_, NSIMD_SIMD, _vl, T)

#define vecx2(T) NSIMD_PP_CAT_5(nsimd_, NSIMD_SIMD, _v, T, x2)
#define vecx3(T) NSIMD_PP_CAT_5(nsimd_, NSIMD_SIMD, _v, T, x3)
#define vecx4(T) NSIMD_PP_CAT_5(nsimd_, NSIMD_SIMD, _v, T, x4)

typedef vec(i8) vi8;
typedef vec(u8) vu8;
typedef vec(i16) vi16;
typedef vec(u16) vu16;
typedef vec(i32) vi32;
typedef vec(u32) vu32;
typedef vec(i64) vi64;
typedef vec(u64) vu64;
typedef vec(f16) vf16;
typedef vec(f32) vf32;
typedef vec(f64) vf64;

typedef vecx2(i8) vi8x2;
typedef vecx2(u8) vu8x2;
typedef vecx2(i16) vi16x2;
typedef vecx2(u16) vu16x2;
typedef vecx2(i32) vi32x2;
typedef vecx2(u32) vu32x2;
typedef vecx2(i64) vi64x2;
typedef vecx2(u64) vu64x2;
typedef vecx2(f16) vf16x2;
typedef vecx2(f32) vf32x2;
typedef vecx2(f64) vf64x2;

typedef vecx3(i8) vi8x3;
typedef vecx3(u8) vu8x3;
typedef vecx3(i16) vi16x3;
typedef vecx3(u16) vu16x3;
typedef vecx3(i32) vi32x3;
typedef vecx3(u32) vu32x3;
typedef vecx3(i64) vi64x3;
typedef vecx3(u64) vu64x3;
typedef vecx3(f16) vf16x3;
typedef vecx3(f32) vf32x3;
typedef vecx3(f64) vf64x3;

typedef vecx4(i8) vi8x4;
typedef vecx4(u8) vu8x4;
typedef vecx4(i16) vi16x4;
typedef vecx4(u16) vu16x4;
typedef vecx4(i32) vi32x4;
typedef vecx4(u32) vu32x4;
typedef vecx4(i64) vi64x4;
typedef vecx4(u64) vu64x4;
typedef vecx4(f16) vf16x4;
typedef vecx4(f32) vf32x4;
typedef vecx4(f64) vf64x4;

typedef vecl(i8) vli8;
typedef vecl(u8) vlu8;
typedef vecl(i16) vli16;
typedef vecl(u16) vlu16;
typedef vecl(i32) vli32;
typedef vecl(u32) vlu32;
typedef vecl(i64) vli64;
typedef vecl(u64) vlu64;
typedef vecl(f16) vlf16;
typedef vecl(f32) vlf32;
typedef vecl(f64) vlf64;

#define vec_a(T, simd_ext) NSIMD_PP_CAT_4(nsimd_, simd_ext, _v, T)
#define vecl_a(T, simd_ext) NSIMD_PP_CAT_4(nsimd_, simd_ext, _vl, T)

#if NSIMD_CXX > 0

namespace nsimd {

/* Alignment tags */
struct aligned {};
struct unaligned {};

#if NSIMD_CXX >= 2011

template <typename T>
using simd_vector = typename simd_traits<T, NSIMD_SIMD>::simd_vector;

template <typename T>
using simd_vectorl = typename simd_traits<T, NSIMD_SIMD>::simd_vectorl;

#endif

} // namespace nsimd

#endif

/* clang-format off */

#if defined(NSIMD_X86)
  #define NSIMD_MAX_ALIGNMENT 64
#elif defined(NSIMD_ARM)
#define NSIMD_MAX_ALIGNMENT 256
#elif defined(NSIMD_POWERPC)
#define NSIMD_MAX_ALIGNMENT 64
#else
  #define NSIMD_MAX_ALIGNMENT 16
#endif

/* clang-format on */

#define NSIMD_NB_REGISTERS NSIMD_PP_CAT_3(NSIMD_, NSIMD_SIMD, _NB_REGISTERS)

#define NSIMD_MAX_LEN_BIT 2048

#define NSIMD_MAX_LEN_i8 (NSIMD_MAX_LEN_BIT / 8)
#define NSIMD_MAX_LEN_u8 (NSIMD_MAX_LEN_BIT / 8)
#define NSIMD_MAX_LEN_i16 (NSIMD_MAX_LEN_BIT / 16)
#define NSIMD_MAX_LEN_u16 (NSIMD_MAX_LEN_BIT / 16)
#define NSIMD_MAX_LEN_f16 (NSIMD_MAX_LEN_BIT / 16)
#define NSIMD_MAX_LEN_i32 (NSIMD_MAX_LEN_BIT / 32)
#define NSIMD_MAX_LEN_u32 (NSIMD_MAX_LEN_BIT / 32)
#define NSIMD_MAX_LEN_f32 (NSIMD_MAX_LEN_BIT / 32)
#define NSIMD_MAX_LEN_i64 (NSIMD_MAX_LEN_BIT / 64)
#define NSIMD_MAX_LEN_u64 (NSIMD_MAX_LEN_BIT / 64)
#define NSIMD_MAX_LEN_f64 (NSIMD_MAX_LEN_BIT / 64)

#define NSIMD_MAX_LEN_e(typ) NSIMD_MAX_LEN_##typ
#define NSIMD_MAX_LEN(typ) NSIMD_MAX_LEN_e(typ)

#if NSIMD_CXX > 0
namespace nsimd {

template <typename T> struct max_len_t {};

template <> struct max_len_t<i8> {
  static const int value = NSIMD_MAX_LEN_BIT / 8;
};
template <> struct max_len_t<u8> {
  static const int value = NSIMD_MAX_LEN_BIT / 8;
};
template <> struct max_len_t<i16> {
  static const int value = NSIMD_MAX_LEN_BIT / 16;
};
template <> struct max_len_t<u16> {
  static const int value = NSIMD_MAX_LEN_BIT / 16;
};
template <> struct max_len_t<f16> {
  static const int value = NSIMD_MAX_LEN_BIT / 16;
};
template <> struct max_len_t<i32> {
  static const int value = NSIMD_MAX_LEN_BIT / 32;
};
template <> struct max_len_t<u32> {
  static const int value = NSIMD_MAX_LEN_BIT / 32;
};
template <> struct max_len_t<f32> {
  static const int value = NSIMD_MAX_LEN_BIT / 32;
};
template <> struct max_len_t<i64> {
  static const int value = NSIMD_MAX_LEN_BIT / 64;
};
template <> struct max_len_t<u64> {
  static const int value = NSIMD_MAX_LEN_BIT / 64;
};
template <> struct max_len_t<f64> {
  static const int value = NSIMD_MAX_LEN_BIT / 64;
};

#if NSIMD_CXX >= 2014
template <typename T> constexpr int max_len = max_len_t<T>::value;
#endif

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* Memory functions */

/* clang-format off */

#if NSIMD_CXX > 0
  #include <cstddef>
  #include <new>
#endif

/* clang-format on */

/* ------------------------------------------------------------------------- */

#if NSIMD_CXX > 0
extern "C" {
#endif

NSIMD_DLLSPEC void *nsimd_aligned_alloc(nsimd_nat);
NSIMD_DLLSPEC void nsimd_aligned_free(void *);

#if NSIMD_CXX > 0
} // extern "C"
#endif

/* ------------------------------------------------------------------------- */
/* C++ templated functions */

#if NSIMD_CXX > 0
namespace nsimd {

NSIMD_DLLSPEC void *aligned_alloc(nsimd_nat);
NSIMD_DLLSPEC void aligned_free(void *);

template <typename T> T *aligned_alloc_for(nsimd_nat n) {
  return (T *)aligned_alloc(n * (nsimd_nat)sizeof(T));
}

template <typename T> void aligned_free_for(void *ptr) {
  return aligned_free((T *)ptr);
}

template <typename T> class scoped_aligned_mem_helper {
protected:
  scoped_aligned_mem_helper(T *const ptr) : m_ptr(ptr) {
    if (NULL == m_ptr) {
      throw std::bad_alloc();
    }
  }

  static void swap(T **lhs, T **rhs) {
    T *temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
  }

public:
  T *release() {
    T *ret_ptr = m_ptr;
    m_ptr = NULL;
    return ret_ptr;
  }

  void reset(T *new_ptr) {
    T *old_ptr = m_ptr;
    m_ptr = new_ptr;
    if (NULL != old_ptr) {
      aligned_free(old_ptr);
    }
  }

  T *get() const { return m_ptr; }

protected:
  T *m_ptr;

private:
  scoped_aligned_mem_helper(const scoped_aligned_mem_helper &other) {
    (void)other;
  }

  scoped_aligned_mem_helper &operator=(const scoped_aligned_mem_helper &rhs) {
    (void)rhs;
  }
};

template <typename T, nat MemAlignedSizeBytes = NSIMD_MAX_LEN_BIT / 8>
class scoped_aligned_mem : public scoped_aligned_mem_helper<T> {
public:
  scoped_aligned_mem()
      : scoped_aligned_mem_helper<T>((T *)aligned_alloc(MemAlignedSizeBytes)) {}

  ~scoped_aligned_mem() {
    if (NULL != this->m_ptr) {
      aligned_free(this->m_ptr);
    }
  }

  void swap(scoped_aligned_mem<T, MemAlignedSizeBytes> *rhs) {
    scoped_aligned_mem_helper<T>::swap(&(this->m_ptr), &(rhs->m_ptr));
  }

  static void free(T **ptr) {
    aligned_free(*ptr);
    *ptr = NULL;
  }
  static const nat num_elems = MemAlignedSizeBytes / sizeof(T);
};

template <typename T, nat NumElems>
class scoped_aligned_mem_for : public scoped_aligned_mem_helper<T> {
public:
  scoped_aligned_mem_for()
      : scoped_aligned_mem_helper<T>(aligned_alloc_for<T>(NumElems)) {}

  ~scoped_aligned_mem_for() {
    if (NULL != this->m_ptr) {
      aligned_free_for<T>(this->m_ptr);
    }
  }

  void swap(scoped_aligned_mem_for<T, NumElems> *rhs) {
    scoped_aligned_mem_helper<T>::swap(&(this->m_ptr), &(rhs->m_ptr));
  }

  static void free(T **ptr) {
    aligned_free_for<T>(*ptr);
    *ptr = NULL;
  }
  static const nat num_elems = NumElems;
};

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* C++ <11 allocator */

#if NSIMD_CXX > 0 && NSIMD_CXX < 2011
namespace nsimd {

template <typename T> class allocator {
public:
  typedef T value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

public:
  template <typename U> struct rebind { typedef allocator<U> other; };

public:
  allocator() {}
  ~allocator() {}
  allocator(allocator const &) {}

  template <typename U> inline explicit allocator(allocator<U> const &) {}

  pointer address(reference r) { return &r; }
  const_pointer address(const_reference r) { return &r; }

  pointer allocate(size_type n) {
    return reinterpret_cast<pointer>(aligned_alloc_for<T>((nsimd_nat)n));
  }

  pointer allocate(size_type n, const void *) { return allocate(n); }

  void deallocate(pointer p, size_type) { aligned_free_for<T>(p); }

  size_type max_size() const { return size_type(-1) / sizeof(T); }

  void construct(pointer p, const T &t) { new (p) T(t); }
  void destroy(pointer p) { p->~T(); }

  bool operator==(allocator const &) { return true; }
  bool operator!=(allocator const &a) { return !operator==(a); }
};

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* C++ >=11 allocator */

#if NSIMD_CXX >= 2011
namespace nsimd {

template <typename T> struct allocator {
  using value_type = T;

  allocator() = default;

  template <typename S> allocator(allocator<S> const &) {}

  T *allocate(std::size_t n) {
    if (n > std::size_t(-1) / sizeof(T)) {
      throw std::bad_alloc();
    }
    T *ptr = aligned_alloc_for<T>((nsimd_nat)n);
    if (ptr != NULL) {
      return ptr;
    }
    throw std::bad_alloc();
  }

  void deallocate(T *ptr, std::size_t) { nsimd::aligned_free(ptr); }
};

template <class T, class S>
bool operator==(allocator<T> const &, allocator<S> const &) {
  return true;
}

template <class T, class S>
bool operator!=(allocator<T> const &, allocator<S> const &) {
  return false;
}

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* Conversion functions f16 <---> f32 for C */

#if NSIMD_CXX > 0
extern "C" {
#endif

NSIMD_DLLSPEC u16 nsimd_f32_to_u16(f32);
NSIMD_DLLSPEC f32 nsimd_u16_to_f32(u16);

#ifdef NSIMD_NATIVE_FP16
NSIMD_INLINE f16 nsimd_f32_to_f16(f32 a) { return (f16)a; }
NSIMD_INLINE f32 nsimd_f16_to_f32(f16 a) { return (f32)a; }
#elif defined(NSIMD_CUDA_COMPILING_FOR_DEVICE) || \
      defined(NSIMD_ROCM_COMPILING_FOR_DEVICE)
inline f16 nsimd_f32_to_f16(f32 a) {
  return __float2half(a);
}
inline f32 nsimd_f16_to_f32(f16 a) {
  return __half2float(a);
}
#else
NSIMD_DLLSPEC f16 nsimd_f32_to_f16(f32);
NSIMD_DLLSPEC f32 nsimd_f16_to_f32(f16);
#endif

#if NSIMD_CXX > 0
} // extern "C"
#endif

/* ------------------------------------------------------------------------- */
/* Conversion functions f16 <---> f32 for C++ */

#if NSIMD_CXX > 0
namespace nsimd {
NSIMD_DLLSPEC u16 f32_to_u16(f32);
NSIMD_DLLSPEC f32 u16_to_f32(u16);
#ifdef NSIMD_NATIVE_FP16
NSIMD_INLINE f16 f32_to_f16(f32 a) { return (f16)a; }
NSIMD_INLINE f32 f16_to_f32(f16 a) { return (f32)a; }
#else
NSIMD_DLLSPEC f16 f32_to_f16(f32);
NSIMD_DLLSPEC f32 f16_to_f32(f16);
#endif
} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* General conversion for C++ */

#if NSIMD_CXX > 0
namespace nsimd {

template <typename T, typename S> struct to_helper {
  static T to(T, S value) { return (T)value; }
};

template <> struct to_helper<f16, f16> {
  static f16 to(f16, f16 value) { return value; }
};

template <typename S> struct to_helper<f16, S>{
  static f16 to(f16, S value) { return nsimd_f32_to_f16((f32)value); }
};

template <typename T> struct to_helper<T, f16> {
  static T to(T, f16 value) { return (T)nsimd_f16_to_f32(value); }
};

template <typename T, typename S> T to(S value) {
  return to_helper<T, S>::to(T(), value);
}

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* SIMD-related functions */

/* clang-format off */

#if defined(NSIMD_IS_MSVC)
  /* We do not want MSVC to warn us about unary minus on an unsigned type.
     It is well defined in standards: unsigned arithmetic is done modulo
     2^n. */
  #pragma warning(push)
  #pragma warning(disable : 4146)
#elif defined(NSIMD_IS_CLANG) && NSIMD_CXX < 2011
  /* When compiling with Clang with C++98 or C++03, some Intel intrinsics are
     implemented as macros which contain long long but long long are not
     standard. To get rid of a lot of warning we push the corresponding
     warning here. */
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc++11-long-long"
#endif

#include <nsimd/functions.h>

#if defined(NSIMD_IS_MSVC)
  #pragma warning(pop)
#elif defined(NSIMD_IS_CLANG) && NSIMD_CXX < 2011
  #pragma clang diagnostic pop
#endif

/* clang-format on */

/* ------------------------------------------------------------------------- */
/* If_else cannot be auto-generated */

#define vif_else(a0, a1, a2, typel, type)                                     \
  NSIMD_PP_CAT_4(nsimd_if_else1_, NSIMD_SIMD, _, type)                        \
  (NSIMD_PP_CAT_6(nsimd_vreinterpretl_, NSIMD_SIMD, _, type, _, typel)(a0),   \
   a1, a2)

#define vif_else_e(a0, a1, a2, typel, type, simd_ext)                         \
  NSIMD_PP_CAT_4(nsimd_if_else1_, simd_ext, _, type)                          \
  (NSIMD_PP_CAT_6(nsimd_vreinterpretl_, simd_ext, _, type, _, typel)(a0), a1, \
   a2)

#if NSIMD_CXX > 0
namespace nsimd {

template <typename A0, typename A1, typename A2, typename L, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vector if_else(A0 a0, A1 a1, A2 a2,
                                                         L, T) {
  return if_else1(reinterpretl(a0, L(), T(), NSIMD_SIMD()), a1, a2, T(),
                  NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename L, typename T,
          typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vector if_else(A0 a0, A1 a1, A2 a2, L,
                                                      T, SimdExt) {
  return if_else1(reinterpretl(a0, L(), T(), SimdExt()), a1, a2, T(),
                  SimdExt());
}

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* Loads/stores can be parametrized/templated by the alignment */

#define NSIMD_ALIGNED a
#define NSIMD_UNALIGNED u

#define vload(a0, type, alignment)                                            \
  NSIMD_PP_CAT_6(nsimd_load, alignment, _, NSIMD_SIMD, _, type)(a0)

#define vload_e(a0, type, simd_ext, alignment)                                \
  NSIMD_PP_CAT_6(nsimd_load, alignment, _, simd_ext, _, type)(a0)

#define vload2(a0, type, alignment)                                           \
  NSIMD_PP_CAT_6(nsimd_load2, alignment, _, NSIMD_SIMD, _, type)(a0)

#define vload2_e(a0, type, simd_ext, alignment)                               \
  NSIMD_PP_CAT_6(nsimd_load2, alignment, _, simd_ext, _, type)(a0)

#define vload3(a0, type, alignment)                                           \
  NSIMD_PP_CAT_6(nsimd_load3, alignment, _, NSIMD_SIMD, _, type)(a0)

#define vload3_e(a0, type, simd_ext, alignment)                               \
  NSIMD_PP_CAT_6(nsimd_load3, alignment, _, simd_ext, _, type)(a0)

#define vload4(a0, type, alignment)                                           \
  NSIMD_PP_CAT_6(nsimd_load4, alignment, _, NSIMD_SIMD, _, type)(a0)

#define vload4_e(a0, type, simd_ext, alignment)                               \
  NSIMD_PP_CAT_6(nsimd_load4, alignment, _, simd_ext, _, type)(a0)

#define vloadl(a0, type, alignment)                                           \
  NSIMD_PP_CAT_6(nsimd_loadl, alignment_, NSIMD_SIMD, _, type)(a0)

#define vloadl_e(a0, type, simd_ext, alignment)                               \
  NSIMD_PP_CAT_6(nsimd_loadl, alignment_, simd_ext, _, type)(a0)

#define vstore(a0, a1, type, alignment)                                       \
  NSIMD_PP_CAT_6(nsimd_store, alignment, _, NSIMD_SIMD, _, type)(a0, a1)

#define vstore_e(a0, a1, type, simd_ext, alignment)                           \
  NSIMD_PP_CAT_6(nsimd_store, alignment, _, simd_ext, _, type)(a0, a1)

#define vstore2(a0, a1, a2, type, alignment)                                  \
  NSIMD_PP_CAT_4(nsimd_store2, alignment, _, NSIMD_SIMD, _, type)(a0, a1, a2)

#define vstore2_e(a0, a1, a2, type, simd_ext, alignment)                      \
  NSIMD_PP_CAT_4(nsimd_store2, alignment, _, simd_ext, _, type)(a0, a1, a2)

#define vstore3(a0, a1, a2, a3, type, alignment)                              \
  NSIMD_PP_CAT_4(nsimd_store3, alignment, _, NSIMD_SIMD, _, type)             \
  (a0, a1, a2, a3)

#define vstore3_e(a0, a1, a2, a3, type, simd_ext, alignment)                  \
  NSIMD_PP_CAT_4(nsimd_store3, alignment, _, simd_ext, _, type)(a0, a1, a2, a3)

#define vstore4(a0, a1, a2, a3, a4, type, alignment)                          \
  NSIMD_PP_CAT_4(nsimd_store3, alignment, _, NSIMD_SIMD, _, type)             \
  (a0, a1, a2, a3, a4)

#define vstore4_e(a0, a1, a2, a3, a4, type, simd_ext, alignment)              \
  NSIMD_PP_CAT_4(nsimd_store3, alignment, _, simd_ext, _, type)               \
  (a0, a1, a2, a3, a4)

#define vstorel(a0, a1, type, alignment)                                      \
  NSIMD_PP_CAT_6(nsimd_storel, alignment, _, NSIMD_SIMD, _, type)(a0, a1)

#define vstorel_e(a0, a1, type, simd_ext, alignment)                          \
  NSIMD_PP_CAT_6(nsimd_storel, alignment, _, simd_ext, _, type)(a0, a1)

#if NSIMD_CXX > 0
namespace nsimd {

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vector load(A0 a0, T, aligned) {
  return loada(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vector load(A0 a0, T, unaligned) {
  return loadu(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vector load(A0 a0, T, SimdExt,
                                                   aligned) {
  return loada(a0, T(), SimdExt());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vector load(A0 a0, T, SimdExt,
                                                   unaligned) {
  return loadu(a0, T(), SimdExt());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx2 load2(A0 a0, T, aligned) {
  return load2a(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx2 load2(A0 a0, T, unaligned) {
  return load2u(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx2 load2(A0 a0, T, SimdExt,
                                                      aligned) {
  return load2a(a0, T(), SimdExt());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx2 load2(A0 a0, T, SimdExt,
                                                      unaligned) {
  return load2u(a0, T(), SimdExt());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx3 load3(A0 a0, T, aligned) {
  return load3a(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx3 load3(A0 a0, T, unaligned) {
  return load3u(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx3 load3(A0 a0, T, SimdExt,
                                                      aligned) {
  return load3a(a0, T(), SimdExt());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx3 load3(A0 a0, T, SimdExt,
                                                      unaligned) {
  return load3u(a0, T(), SimdExt());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx4 load4(A0 a0, T, aligned) {
  return load4a(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorx4 load4(A0 a0, T, unaligned) {
  return load4u(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx4 load4(A0 a0, T, SimdExt,
                                                      aligned) {
  return load4a(a0, T(), SimdExt());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorx4 load4(A0 a0, T, SimdExt,
                                                      unaligned) {
  return load4u(a0, T(), SimdExt());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorl loadlu(A0 a0, T, aligned) {
  return loadla(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T>
typename simd_traits<T, NSIMD_SIMD>::simd_vectorl loadlu(A0 a0, T, unaligned) {
  return loadlu(a0, T(), NSIMD_SIMD());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorl loadlu(A0 a0, T, SimdExt,
                                                      aligned) {
  return loadla(a0, T(), SimdExt());
}

template <typename A0, typename T, typename SimdExt>
typename simd_traits<T, SimdExt>::simd_vectorl loadlu(A0 a0, T, SimdExt,
                                                      unaligned) {
  return loadlu(a0, T(), SimdExt());
}

template <typename A0, typename A1, typename T>
void store(A0 a0, A1 a1, T, aligned) {
  storea(a0, a1, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename T>
void store(A0 a0, A1 a1, T, unaligned) {
  storeu(a0, a1, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename T, typename SimdExt>
void store(A0 a0, A1 a1, T, SimdExt, aligned) {
  storea(a0, a1, T(), SimdExt());
}

template <typename A0, typename A1, typename T, typename SimdExt>
void store(A0 a0, A1 a1, T, SimdExt, unaligned) {
  storeu(a0, a1, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename T>
void store2(A0 a0, A1 a1, A2 a2, T, aligned) {
  store2a(a0, a1, a2, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename T>
void store2(A0 a0, A1 a1, A2 a2, T, unaligned) {
  store2u(a0, a1, a2, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename T, typename SimdExt>
void store2(A0 a0, A1 a1, A2 a2, T, SimdExt, aligned) {
  store2a(a0, a1, a2, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename T, typename SimdExt>
void store2(A0 a0, A1 a1, A2 a2, T, SimdExt, unaligned) {
  store2u(a0, a1, a2, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename A3, typename T>
void store3(A0 a0, A1 a1, A2 a2, A3 a3, T, aligned) {
  store3a(a0, a1, a2, a3, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename A3, typename T>
void store3(A0 a0, A1 a1, A2 a2, A3 a3, T, unaligned) {
  store3u(a0, a1, a2, a3, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename A3, typename T,
          typename SimdExt>
void store3(A0 a0, A1 a1, A2 a2, A3 a3, T, SimdExt, aligned) {
  store3a(a0, a1, a2, a3, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename A3, typename T,
          typename SimdExt>
void store3(A0 a0, A1 a1, A2 a2, A3 a3, T, SimdExt, unaligned) {
  store3u(a0, a1, a2, a3, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename A3, typename A4,
          typename T>
void store4(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, T, aligned) {
  store4a(a0, a1, a2, a3, a4, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename A3, typename A4,
          typename T>
void store4(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, T, unaligned) {
  store4u(a0, a1, a2, a3, a4, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename A2, typename A3, typename A4,
          typename T, typename SimdExt>
void store4(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, T, SimdExt, aligned) {
  store4a(a0, a1, a2, a3, a4, T(), SimdExt());
}

template <typename A0, typename A1, typename A2, typename A3, typename A4,
          typename T, typename SimdExt>
void store4(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, T, SimdExt, unaligned) {
  store4u(a0, a1, a2, a3, a4, T(), SimdExt());
}

template <typename A0, typename A1, typename T>
void storel(A0 a0, A1 a1, T, aligned) {
  storela(a0, a1, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename T>
void storel(A0 a0, A1 a1, T, unaligned) {
  storelu(a0, a1, T(), NSIMD_SIMD());
}

template <typename A0, typename A1, typename T, typename SimdExt>
void storel(A0 a0, A1 a1, T, SimdExt, aligned) {
  storela(a0, a1, T(), SimdExt());
}

template <typename A0, typename A1, typename T, typename SimdExt>
void storel(A0 a0, A1 a1, T, SimdExt, unaligned) {
  storelu(a0, a1, T(), SimdExt());
}

} // namespace nsimd
#endif

/* ------------------------------------------------------------------------- */
/* Scalar utilisties */

#include <nsimd/scalar_utilities.h>

/* ------------------------------------------------------------------------- */

#endif
