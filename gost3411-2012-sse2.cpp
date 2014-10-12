#pragma GCC target ("sse2,mmx")

#ifndef __MMX__
#define __MMX__ 1
#endif

#ifndef __SSE__
#define __SSE__ 1
#endif

#ifndef __SSE2__
#define __SSE2__ 1
#endif

#include <cstring>
#include <mmintrin.h>
#include <emmintrin.h>
#include "tables.h"
#include "gost3411-2012-sse2.h"

#if defined(__GNUC_PREREQ) && __GNUC_PREREQ(4,8)
#	define TARGET_SSE2
#else
#	define TARGET_SSE2 __attribute__((target("sse2")))
#endif

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
	uint i, CF;

	CF = 0;
	for (i=0; i<8; ++i) {
		quint64 a   = x->QWORD[i];
		quint64 b   = y->QWORD[i];
		quint64 sum = a + b + CF;
		CF          = ((sum < b) ? 1 : ((sum > b) ? 0 : CF));
		r->QWORD[i] = sum;
	}
}

#if __i386__

template<unsigned int row>
TARGET_SSE2 static inline __m128i extract(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	quint16 ax;
	__m64 mm0, mm1;

	ax  = _mm_extract_epi16(xmm0, row + 0);
	mm0 = reinterpret_cast<__m64>(Ax[0][static_cast<quint8>(ax)]);
	mm1 = reinterpret_cast<__m64>(Ax[0][static_cast<quint8>(ax >> 8)]);

	ax  = _mm_extract_epi16(xmm0, row + 4);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[1][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[1][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, row + 0);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[2][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[2][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, row + 4);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[3][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[3][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, row + 0);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[4][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[4][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, row + 4);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[5][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[5][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, row + 0);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[6][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[6][static_cast<quint8>(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, row + 4);
	mm0 = _mm_xor_si64(mm0, reinterpret_cast<__m64>(Ax[7][static_cast<quint8>(ax)]));
	mm1 = _mm_xor_si64(mm1, reinterpret_cast<__m64>(Ax[7][static_cast<quint8>(ax >> 8)]));

	return _mm_set_epi64(mm1, mm0);
}

#else

template<unsigned int row>
TARGET_SSE2 static inline __m128i extract(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	quint64 r0, r1;
	quint16 ax;

	ax  = _mm_extract_epi16(xmm0, row);
	r0  = Ax[0][static_cast<quint8>(ax)];
	r1  = Ax[0][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm0, row + 4);
	r0 ^= Ax[1][static_cast<quint8>(ax)];
	r1 ^= Ax[1][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, row);
	r0 ^= Ax[2][static_cast<quint8>(ax)];
	r1 ^= Ax[2][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, row + 4);
	r0 ^= Ax[3][static_cast<quint8>(ax)];
	r1 ^= Ax[3][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, row);
	r0 ^= Ax[4][static_cast<quint8>(ax)];
	r1 ^= Ax[4][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, row + 4);
	r0 ^= Ax[5][static_cast<quint8>(ax)];
	r1 ^= Ax[5][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, row);
	r0 ^= Ax[6][static_cast<quint8>(ax)];
	r1 ^= Ax[6][static_cast<quint8>(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, row + 4);
	r0 ^= Ax[7][static_cast<quint8>(ax)];
	r1 ^= Ax[7][static_cast<quint8>(ax >> 8)];

	return _mm_set_epi64x(r1, r0);
}
#endif

TARGET_SSE2 static inline void g(union uint512_u* h, const union uint512_u* N, const union uint512_u* m)
{
	__m128i xmm0, xmm2, xmm4, xmm6;
	__m128i xmm1, xmm3, xmm5, xmm7;
	__m128i tmm0, tmm1, tmm2, tmm3;
	uint i;
	const __m128i *pN = reinterpret_cast<const __m128i*>(N);
	const __m128i *pM = reinterpret_cast<const __m128i*>(m);
	 __m128i      *pH = reinterpret_cast<__m128i*>(h);
	const __m128i* p;

	xmm0 = _mm_load_si128(&pN[0]);
	xmm2 = _mm_load_si128(&pN[1]);
	xmm4 = _mm_load_si128(&pN[2]);
	xmm6 = _mm_load_si128(&pN[3]);

	/* XLPS128M(h, xmm0, xmm2, xmm4, xmm6); */
	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pH[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pH[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pH[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pH[3]));

	tmm0 = extract<0>(xmm0, xmm2, xmm4, xmm6);
	tmm1 = extract<1>(xmm0, xmm2, xmm4, xmm6);
	tmm2 = extract<2>(xmm0, xmm2, xmm4, xmm6);
	tmm3 = extract<3>(xmm0, xmm2, xmm4, xmm6);

	xmm0 = tmm0;
	xmm2 = tmm1;
	xmm4 = tmm2;
	xmm6 = tmm3;
	/**/

	xmm1 = _mm_load_si128(&pM[0]);
	xmm3 = _mm_load_si128(&pM[1]);
	xmm5 = _mm_load_si128(&pM[2]);
	xmm7 = _mm_load_si128(&pM[3]);

	/* XLPS128R */
	xmm1 = _mm_xor_si128(xmm1, xmm0);
	xmm3 = _mm_xor_si128(xmm3, xmm2);
	xmm5 = _mm_xor_si128(xmm5, xmm4);
	xmm7 = _mm_xor_si128(xmm7, xmm6);

	tmm0 = extract<0>(xmm1, xmm3, xmm5, xmm7);
	tmm1 = extract<1>(xmm1, xmm3, xmm5, xmm7);
	tmm2 = extract<2>(xmm1, xmm3, xmm5, xmm7);
	tmm3 = extract<3>(xmm1, xmm3, xmm5, xmm7);

	xmm1 = tmm0;
	xmm3 = tmm1;
	xmm5 = tmm2;
	xmm7 = tmm3;
	/* end of XLPS128R */

	for (i=0; i<11; ++i) {
		/* XLPS128M(&C[i], xmm0, xmm2, xmm4, xmm6); */
		p = reinterpret_cast<const __m128i*>(&C[i]);
		xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&p[0]));
		xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&p[1]));
		xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&p[2]));
		xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&p[3]));

		tmm0 = extract<0>(xmm0, xmm2, xmm4, xmm6);
		tmm1 = extract<1>(xmm0, xmm2, xmm4, xmm6);
		tmm2 = extract<2>(xmm0, xmm2, xmm4, xmm6);
		tmm3 = extract<3>(xmm0, xmm2, xmm4, xmm6);

		xmm0 = tmm0;
		xmm2 = tmm1;
		xmm4 = tmm2;
		xmm6 = tmm3;
		/**/

		/* XLPS128R */
		xmm1 = _mm_xor_si128(xmm1, xmm0);
		xmm3 = _mm_xor_si128(xmm3, xmm2);
		xmm5 = _mm_xor_si128(xmm5, xmm4);
		xmm7 = _mm_xor_si128(xmm7, xmm6);

		tmm0 = extract<0>(xmm1, xmm3, xmm5, xmm7);
		tmm1 = extract<1>(xmm1, xmm3, xmm5, xmm7);
		tmm2 = extract<2>(xmm1, xmm3, xmm5, xmm7);
		tmm3 = extract<3>(xmm1, xmm3, xmm5, xmm7);

		xmm1 = tmm0;
		xmm3 = tmm1;
		xmm5 = tmm2;
		xmm7 = tmm3;
		/* end of XLPS128R */
	}

	/*XLPS128M(&C[11], xmm0, xmm2, xmm4, xmm6);*/
	p = reinterpret_cast<const __m128i*>(&C[11]);
	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&p[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&p[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&p[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&p[3]));

	tmm0 = extract<0>(xmm0, xmm2, xmm4, xmm6);
	tmm1 = extract<1>(xmm0, xmm2, xmm4, xmm6);
	tmm2 = extract<2>(xmm0, xmm2, xmm4, xmm6);
	tmm3 = extract<3>(xmm0, xmm2, xmm4, xmm6);

	xmm0 = tmm0;
	xmm2 = tmm1;
	xmm4 = tmm2;
	xmm6 = tmm3;

	xmm0 = _mm_xor_si128(xmm0, xmm1);
	xmm2 = _mm_xor_si128(xmm2, xmm3);
	xmm4 = _mm_xor_si128(xmm4, xmm5);
	xmm6 = _mm_xor_si128(xmm6, xmm7);

	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pH[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pH[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pH[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pH[3]));

	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pM[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pM[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pM[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pM[3]));

	_mm_store_si128(&pH[0], xmm0);
	_mm_store_si128(&pH[1], xmm2);
	_mm_store_si128(&pH[2], xmm4);
	_mm_store_si128(&pH[3], xmm6);
}

static inline void stage2(GOST34112012Context* CTX, const union uint512_u* data)
{
	g(&CTX->h, &CTX->N, data);

	add512(&CTX->N, &buffer512, &CTX->N);
	add512(&CTX->Sigma, data, &CTX->Sigma);
}

void GOST34112012Update_sse2(GOST34112012Context* ctx, const unsigned char* data, std::size_t len)
{
	std::size_t chunksize;
	const union uint512_u* d = reinterpret_cast<const union uint512_u*>(data);

	while (len > 63 && ctx->bufsize == 0) {
		stage2(ctx, d);

		data += 64;
		len  -= 64;
	}

	while (len) {
		chunksize = 64 - ctx->bufsize;
		if (chunksize > len) {
			chunksize = len;
		}

		std::memcpy(&ctx->buffer.BYTE[ctx->bufsize], data, chunksize);

		ctx->bufsize += chunksize;
		len          -= chunksize;
		data         += chunksize;

		if (ctx->bufsize == 64) {
			stage2(ctx, &ctx->buffer);
			ctx->bufsize = 0;
		}
	}

	_mm_empty();
}

void GOST34112012Final_sse2(GOST34112012Context* ctx, unsigned char* digest)
{
	Q_DECL_ALIGN(16) union uint512_u buf;

	std::memset(&ctx->buffer.BYTE[ctx->bufsize], 0, sizeof(ctx->buffer) - ctx->bufsize);

	buf.QWORD[0] = ctx->bufsize << 3;
	std::memset(buf.BYTE + sizeof(buf.QWORD[0]), 0, sizeof(buf) - sizeof(buf.QWORD[0]));

	if (ctx->bufsize <= 63) {
		ctx->buffer.BYTE[ctx->bufsize] = 1;
		std::memset(ctx->buffer.BYTE + ctx->bufsize + 1, 0, sizeof(ctx->buffer) - ctx->bufsize + 1);
	}

	g(&ctx->h, &ctx->N, &ctx->buffer);

	add512(&ctx->N, &buf, &ctx->N);
	add512(&ctx->Sigma, &ctx->buffer, &ctx->Sigma);

	g(&ctx->h, &buffer0, &ctx->N);
	g(&ctx->h, &buffer0, &ctx->Sigma);

	std::memcpy(&ctx->hash, &ctx->h, sizeof(ctx->hash));

	if (ctx->digest_size == 256) {
		std::memcpy(digest, &(ctx->hash.QWORD[4]), 32);
	}
	else {
		std::memcpy(digest, &(ctx->hash.QWORD[0]), 64);
	}

	std::memset(ctx, 0, sizeof(GOST34112012Context));
	_mm_empty();
}
