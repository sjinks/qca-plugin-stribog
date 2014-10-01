#pragma GCC target ("mmx")

#ifndef __MMX__
#define __MMX__
#endif

#include <cstring>
#include <mmintrin.h>
#include "tables.h"
#include "gost3411-2012-mmx.h"

static inline __m64 i64_to_m64(quint64 x)
{
#ifndef __x86_64__
	return _mm_setr_pi32(static_cast<quint32>(x), static_cast<quint32>(x >> 32));
#else
	return _mm_cvtsi64_m64(x);
#endif
}

static inline quint64 m64_to_i64(__m64 x)
{
#ifndef __x86_64__
	return reinterpret_cast<quint64>(x);
#else
	return _mm_cvtm64_si64(x);
#endif
}

static inline void xtranspose(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	__m64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
	__m64 tm0, tm1, tm2, tm3, tm4, tm5, tm6, tm7;

	/* load and XOR */
	mm0 = _mm_xor_si64(i64_to_m64(x->QWORD[0]), i64_to_m64(y->QWORD[0]));
	mm1 = _mm_xor_si64(i64_to_m64(x->QWORD[1]), i64_to_m64(y->QWORD[1]));
	mm2 = _mm_xor_si64(i64_to_m64(x->QWORD[2]), i64_to_m64(y->QWORD[2]));
	mm3 = _mm_xor_si64(i64_to_m64(x->QWORD[3]), i64_to_m64(y->QWORD[3]));
	mm4 = _mm_xor_si64(i64_to_m64(x->QWORD[4]), i64_to_m64(y->QWORD[4]));
	mm5 = _mm_xor_si64(i64_to_m64(x->QWORD[5]), i64_to_m64(y->QWORD[5]));
	mm6 = _mm_xor_si64(i64_to_m64(x->QWORD[6]), i64_to_m64(y->QWORD[6]));
	mm7 = _mm_xor_si64(i64_to_m64(x->QWORD[7]), i64_to_m64(y->QWORD[7]));

	/* transpose */
	tm0 = _mm_unpacklo_pi8(mm0, mm2);
	tm1 = _mm_unpackhi_pi8(mm0, mm2);
	tm2 = _mm_unpacklo_pi8(mm1, mm3);
	tm3 = _mm_unpackhi_pi8(mm1, mm3);
	tm4 = _mm_unpacklo_pi8(mm4, mm6);
	tm5 = _mm_unpackhi_pi8(mm4, mm6);
	tm6 = _mm_unpacklo_pi8(mm5, mm7);
	tm7 = _mm_unpackhi_pi8(mm5, mm7);

	mm0 = _mm_unpacklo_pi8(tm0, tm2);
	mm1 = _mm_unpackhi_pi8(tm0, tm2);
	mm2 = _mm_unpacklo_pi8(tm1, tm3);
	mm3 = _mm_unpackhi_pi8(tm1, tm3);
	mm4 = _mm_unpacklo_pi8(tm4, tm6);
	mm5 = _mm_unpackhi_pi8(tm4, tm6);
	mm6 = _mm_unpacklo_pi8(tm5, tm7);
	mm7 = _mm_unpackhi_pi8(tm5, tm7);

	tm2 = _mm_unpacklo_pi32(mm1, mm5);
	tm3 = _mm_unpackhi_pi32(mm1, mm5);
	tm0 = _mm_unpacklo_pi32(mm0, mm4);
	tm1 = _mm_unpackhi_pi32(mm0, mm4);
	mm4 = _mm_unpacklo_pi32(mm2, mm6);
	mm5 = _mm_unpackhi_pi32(mm2, mm6);
	mm6 = _mm_unpacklo_pi32(mm3, mm7);
	mm7 = _mm_unpackhi_pi32(mm3, mm7);
	mm0 = tm0;
	mm1 = tm1;
	mm2 = tm2;
	mm3 = tm3;

	/* unload */
	z->QWORD[0] = m64_to_i64(mm0);
	z->QWORD[1] = m64_to_i64(mm1);
	z->QWORD[2] = m64_to_i64(mm2);
	z->QWORD[3] = m64_to_i64(mm3);
	z->QWORD[4] = m64_to_i64(mm4);
	z->QWORD[5] = m64_to_i64(mm5);
	z->QWORD[6] = m64_to_i64(mm6);
	z->QWORD[7] = m64_to_i64(mm7);
}

static void XLPS(const union uint512_u* x, const union uint512_u* y, union uint512_u* data)
{
	uint i;
	quint8* p;
	Q_DECL_ALIGN(16) union uint512_u buf;

	xtranspose(x, y, &buf);
	p = buf.BYTE;

	for (i=0; i<8; ++i) {
		__m64 mm0;
		mm0 =                   i64_to_m64(Ax[0][*p]);  ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[1][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[2][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[3][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[4][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[5][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[6][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[7][*p])); ++p;
		data->QWORD[i] = m64_to_i64(mm0);
	}
}

static inline void X(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	z->QWORD[0] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[0]), i64_to_m64(y->QWORD[0])));
	z->QWORD[1] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[1]), i64_to_m64(y->QWORD[1])));
	z->QWORD[2] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[2]), i64_to_m64(y->QWORD[2])));
	z->QWORD[3] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[3]), i64_to_m64(y->QWORD[3])));
	z->QWORD[4] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[4]), i64_to_m64(y->QWORD[4])));
	z->QWORD[5] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[5]), i64_to_m64(y->QWORD[5])));
	z->QWORD[6] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[6]), i64_to_m64(y->QWORD[6])));
	z->QWORD[7] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[7]), i64_to_m64(y->QWORD[7])));
}

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
	uint i, CF, OF;

	CF = 0;
	for (i=0; i<8; ++i) {
		r->QWORD[i]  = x->QWORD[i] + y->QWORD[i];
		OF           = (r->QWORD[i] < y->QWORD[i]) ? 1 : 0;
		r->QWORD[i] += CF;
		CF           = OF;
	}
}

static void g(union uint512_u* h, const union uint512_u* N, const union uint512_u* m)
{
	Q_DECL_ALIGN(16) union uint512_u Ki, data;
	uint i;

	XLPS(h, N, &data);

	/* Starting E() */
	Ki = data;
	XLPS(&Ki, m, &data);

	for (i=0; i<11; ++i) {
		XLPS(&Ki, &C[i], &Ki);
		XLPS(&Ki, &data, &data);
	}

	XLPS(&Ki, &C[11], &Ki);
	X(&Ki, &data, &data);
	/* E() done */

	X(&data, h, &data);
	X(&data, m, h);
}

static inline void stage2(GOST34112012Context* CTX, const union uint512_u* data)
{
	g(&CTX->h, &CTX->N, data);

	add512(&CTX->N, &buffer512, &CTX->N);
	add512(&CTX->Sigma, data, &CTX->Sigma);
}


void GOST34112012Update_mmx(GOST34112012Context* ctx, const unsigned char* data, std::size_t len)
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

void GOST34112012Final_mmx(GOST34112012Context* ctx, unsigned char* digest)
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
