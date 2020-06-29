[![Build Status](https://travis-ci.org/agenium-scale/nsimd.svg?branch=master)](https://travis-ci.org/agenium-scale/nsimd)

Documentation can be found [here](https://agenium-scale.github.io/nsimd/).

# What is NSIMD?

At its core, NSIMD is a vectorization library that abstracts [SIMD
programming](<https://en.wikipedia.org/wiki/SIMD>). It was designed to exploit
the maximum power of processors at a low development cost. NSIMD comes with
modules. As of now two of them adds support for GPUs to NSIMD. The
direction that NSIMD is taking is to provide several programming paradigms
to address different problems and to allow a wider support of architectures.
With two of its modules NSIMD provides three programming paradigms:

- Imperative programming provided by NSIMD core that supports a lots of
  CPU/SIMD extensions.
- Expressions templates provided by the TET1D module that supports all
  architectures from NSIMD core and adds support for NVIDIA and AMD GPUs.
- Single Program Multiple Data provided by the SPMD module that supports all
  architectures from NSIMD core and adds support for NVIDIA and AMD GPUs.

## Supported architectures

|              | CPU | SSE2 | SSE42 | AVX | AVX2 | AVX512\_KNL | AVX512\_SKYLAKE | NEON128 | AARCH64 | SVE | FIXED\_SVE | CUDA | ROCm |
|:-------------|:---:|:----:|:-----:|:---:|:----:|:-----------:|:---------------:|:-------:|:-------:|:---:|:----------:|:----:|:----:|
| NSIMD core   |  Y  |  Y   |   Y   |  Y  |   Y  |      Y      |       Y         |   Y     |    Y    |  Y  |      Y     |  N   |  N   |
| SPMD module  |  Y  |  Y   |   Y   |  Y  |   Y  |      Y      |       Y         |   Y     |    Y    |  Y  |      Y     |  Y   |  Y   |
| TET1D module |  Y  |  Y   |   Y   |  Y  |   Y  |      Y      |       Y         |   Y     |    Y    |  Y  |      Y     |  Y   |  Y   |

Note on Arm SIMD extensions: NEON128 means NEON on ARMv7 and earlier, AARCH64
means NEON on ARMv8 and later. FIXED\_SVE means the support of SVE where
the size of register is known at compile time such as for GCC.

## How it works?

To achieve maximum performance, NSIMD mainly relies on the inline optimization
pass of the compiler. Therefore using any mainstream compiler such as GCC,
Clang, MSVC, XL C/C++, ICC and others with NSIMD will give you a zero-cost SIMD
abstraction library.

To allow inlining, a lot of code is placed in header files. *Small* functions
such as addition, multiplication, square root, etc, are all present in header
files whereas big functions such as I/O are put in source files that are
compiled as a `.so`/`.dll` library.

NSIMD provides C89, C++98, C++11 and C++14 APIs. All APIs allow writing generic
code. For the C API this is achieved through a thin layer of macros; for the
C++ APIs it is achieved using templates and function overloading. The C++ API
is split in two. The first part is a C-like API with only function calls and
direct type definitions for SIMD types while the second one provides operator
overloading, higher level type definitions that allows unrolling. C++11, C++14
APIs add for instance templated type definitions and templated constants.

Binary compatibility is guaranteed by the fact that only a C ABI is exposed.
The C++ API only wraps the C calls.

## Supported compilers

NSIMD is tested with GCC, Clang, MSVC, NVCC, HIPCC and ARMClang. As a C89 and a
C++98 API are provided, other compilers should work fine. Old compiler versions
should work as long as they support the targeted SIMD extension. For instance,
NSIMD can compile on SSE 4.2 code with MSVC 2010.

# Build the library

The support for CMake has been dropped as it has several flaws:
- too slow especially on Windows,
- inability to use several compilers at once,
- inability to have a portable build system,
- very poor support for portable compilation flags,
- ...

## Dependencies

Generating C/C++ files is done by the Python3 code contained in the `egg`.
Python should be installed by default on any Linux distro. On Windows it comes
with the latest versions of Visual Studio on Windows
(<https://visualstudio.microsoft.com/vs/community/>), you can also download and
install it directly from <https://www.python.org/>.

The Python code can call `clang-format` to properly format all generated C/C++
source. On Linux you can install it via your package manager. On Windows you
can use the official binary at <https://llvm.org/builds/>.

Testing the library requires the Google Test library that can be found at
<https://github.com/google/googletest> and the MPFR library that can be found at
<https://www.mpfr.org/>.

Benchmarking the library requires Google Benchmark version 1.3 that can be
found at <https://github.com/google/benchmark> plus all the other SIMD
libraries used for comparison:
- MIPP (<https://github.com/aff3ct/MIPP>)
- Sleef (<https://sleef.org/>)

Compiling the library requires a C++14 compiler. Any recent version of GCC,
Clang and MSVC will do. Note that the produced library and header files for the
end-user are C89, C++98, C++11 compatible. Note that C/C++ files are generated
by a bunch of Python scripts and they must be executed first before running
cmake.

## Build for Linux

```bash
bash scripts/build.sh for simd_ext1/.../simd_extN with comp1/.../compN
```

For each combination a directory `build-simd_ext-comp` will be created and
will contain the library. Supported SIMD extension are:

- sse2
- sse42
- avx
- avx2
- avx512\_knl
- avx512\_skylake
- neon128
- aarch64
- sve
- sve128
- sve256
- sve512
- sve1024
- sve2048
- cuda
- rocm

Supported compiler are:

- gcc
- clang
- icc
- armclang
- cl
- nvcc
- hipcc

Note that certain combination of SIMD extension/compilers are not supported
such as aarch64 with icc, or avx512\_skylake with nvcc.

## Build on Windows

Make sure you are typing in a Visual Studio prompt. The command is almost the
same as for Linux with the same constraints on the pairs SIMD
extension/compilers.

```batch
scripts\build.bat for simd_ext1/.../simd_extN with comp1/.../compN
```

# Philosophy

The library aims to provide a portable zero-cost abstraction over SIMD vendor
intrinsics disregarding the underlying SIMD vector length.

NSIMD was designed following as closely as possible the following guidelines:

- Do not aim for a fully IEEE compliant library, rely on intrinsics, errors
  induced by non compliance are small and acceptable.
- Correctness primes over speed.
- Emulate with tricks and intrinsic integer arithmetic when not available.
- Use common names as found in common computation libraries.
- Do not hide SIMD registers, one variable (of a type such as `nsimd::pack`)
  matches one register.
- Keep the code simple to allow the compiler to perform as many optimizations
  as possible.

You may wrap intrinsics that require compile time knowledge of the underlying
vector length but this should be done with caution.

Wrapping intrinsics that do not exist for all types is difficult and may
require casting or emulation. For instance, 8 bit integer vector multiplication
using SSE2 does not exist. We can either process each pair of integers
individually or we can cast the 8 bit vectors to 16 bit vectors, do the
multiplication and cast them back to 8 bit vectors. In the second case,
chaining operations will generate many unwanted casts.

To avoid hiding important details to the user, overloads of operators involving
scalars and SIMD vectors are not provided by default. Those can be included
explicitely to emphasize the fact that using expressions like `scalar + vector`
might incur an optimization penalty.

The use of `nsimd::pack` may not be portable to ARM SVE and therefore must be
included manually. ARM SVE registers can only be stored in sizeless strucs
(`__sizeless_struct`). This feature (as of 2019/04/05) is only supported by the
ARM compiler. We do not know whether other compilers will use the same keyword
or paradigm to support SVE intrinsics.

# Contributing

The wrapping of intrinsics, the writing of test and bench files are tedious and
repetitive tasks. Most of those are generated using Python scripts that can be
found in `egg`.

- Intrinsics that do not require to known the vector length can be wrapped and
  will be accepted with no problem.
- Intrinsics that do require the vector length at compile time can be wrapped
  but it is up to the maintainer to accept it.
- Use `clang-format` when writing C or C++ code.
- The `.cpp` files are written in C++14.
- The headers files must be compatible with C89 (when possible otherwise
  C99), C++98, C++11 and C++14.

Please see [CONTRIBUTE.md](CONTRIBUTE.md) for more details.

# LICENSE

Copyright (c) 2020 Agenium Scale

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
