# qca-plugin-stribog

GOST R 34.11-2012 (Stribog) Hash Function plugin for QCA

## Introduction

This extension provides implementation of GOST R 34.11-2012 256 and 512 bit hash functions to be used with QCA (Qt Cryptographic Architecture).

The implementation of the hash function (`gost3411-2012-*.cpp`) is based on the implementation by [Alexey Degtyarev](https://www.streebog.net/en/), see `LICENSE.gost`.
The changes made to Alexey's code:
  * C code was rewritten to C++, added Qt-specific things;
  * the MMX implementation was almost completely rewritten in order not to use SSE instructions;
  * optimization of 64-bit version of `extract()` — gained extra 0.7 MB/s;
  * the generic implementation was optimized by removing unnecessary `memcpy` calls;
  * inline functions instead of macros;
  * GOST context was modified to avoid unnecessary casts;
  * the implementation (generic/MMX/SSE) is chosen at runtime depending on CPU capabilities (GCC 4.6 or newer is required for that).

## Installing/Configuring

```bash
qmake
make
sudo make install
```
