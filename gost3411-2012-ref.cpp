#include <QtCore/QtGlobal>
#include <cstring>
#include "gost3411-2012-ref.h"
#include "tables.h"

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
static inline quint64 bswap64(quint64 x)
{
#ifdef __GNUC__
	return __builtin_bswap64(x);
#else
	return
		((x & 0xFF00000000000000ULL) >> 56) |
		((x & 0x00FF000000000000ULL) >> 40) |
		((x & 0x0000FF0000000000ULL) >> 24) |
		((x & 0x000000FF00000000ULL) >>  8) |
		((x & 0x00000000FF000000ULL) <<  8) |
		((x & 0x0000000000FF0000ULL) << 24) |
		((x & 0x000000000000FF00ULL) << 40) |
		((x & 0x00000000000000FFULL) << 56)
	;
#endif
}
#endif

static void XLPS(const union uint512_u* x, const union uint512_u* y, union uint512_u* data)
{
	quint64 r0, r1, r2, r3, r4, r5, r6, r7;
	int i;

	r0 = x->QWORD[0] ^ y->QWORD[0];
	r1 = x->QWORD[1] ^ y->QWORD[1];
	r2 = x->QWORD[2] ^ y->QWORD[2];
	r3 = x->QWORD[3] ^ y->QWORD[3];
	r4 = x->QWORD[4] ^ y->QWORD[4];
	r5 = x->QWORD[5] ^ y->QWORD[5];
	r6 = x->QWORD[6] ^ y->QWORD[6];
	r7 = x->QWORD[7] ^ y->QWORD[7];

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	for (i=0; i<8; ++i) {
		data->QWORD[i]  = Ax[0][static_cast<quint8>(r0 >> (i << 3))];
		data->QWORD[i] ^= Ax[1][static_cast<quint8>(r1 >> (i << 3))];
		data->QWORD[i] ^= Ax[2][static_cast<quint8>(r2 >> (i << 3))];
		data->QWORD[i] ^= Ax[3][static_cast<quint8>(r3 >> (i << 3))];
		data->QWORD[i] ^= Ax[4][static_cast<quint8>(r4 >> (i << 3))];
		data->QWORD[i] ^= Ax[5][static_cast<quint8>(r5 >> (i << 3))];
		data->QWORD[i] ^= Ax[6][static_cast<quint8>(r6 >> (i << 3))];
		data->QWORD[i] ^= Ax[7][static_cast<quint8>(r7 >> (i << 3))];
	}
#else
	for (i=7; i>=0; --i) {
		data->QWORD[7-i]  = Ax[0][static_cast<quint8>(r0 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[1][static_cast<quint8>(r1 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[2][static_cast<quint8>(r2 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[3][static_cast<quint8>(r3 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[4][static_cast<quint8>(r4 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[5][static_cast<quint8>(r5 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[6][static_cast<quint8>(r6 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[7][static_cast<quint8>(r7 >> (i << 3))];
	}
#endif
}

static inline void X(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	z->QWORD[0] = x->QWORD[0] ^ y->QWORD[0];
	z->QWORD[1] = x->QWORD[1] ^ y->QWORD[1];
	z->QWORD[2] = x->QWORD[2] ^ y->QWORD[2];
	z->QWORD[3] = x->QWORD[3] ^ y->QWORD[3];
	z->QWORD[4] = x->QWORD[4] ^ y->QWORD[4];
	z->QWORD[5] = x->QWORD[5] ^ y->QWORD[5];
	z->QWORD[6] = x->QWORD[6] ^ y->QWORD[6];
	z->QWORD[7] = x->QWORD[7] ^ y->QWORD[7];
}

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	int i, CF, OF;

	CF = 0;
	for (i=0; i<8; ++i) {
		r->QWORD[i]  = x->QWORD[i] + y->QWORD[i];
		OF           = (r->QWORD[i] < y->QWORD[i]) ? 1 : 0;
		r->QWORD[i] += CF;
		CF           = OF;
	}
#else
	const unsigned char *xp, *yp;
	unsigned char *rp;
	unsigned int i;
	int buf;

	xp = x->BYTE;
	yp = y->BYTE;
	rp = r->BYTE;

	buf = 0;
	for (i=0; i<64; ++i) {
		buf   = xp[i] + yp[i] + (buf >> 8);
		rp[i] = static_cast<quint8>(buf);
	}
#endif
}

static void g(union uint512_u* h, const union uint512_u* N, const union uint512_u* m)
{
	Q_DECL_ALIGN(16) union uint512_u Ki, data;
	int i;

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


void GOST34112012Update_ref(GOST34112012Context* ctx, const unsigned char* data, std::size_t len)
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

		memcpy(&ctx->buffer.BYTE[ctx->bufsize], data, chunksize);

		ctx->bufsize += chunksize;
		len          -= chunksize;
		data         += chunksize;

		if (ctx->bufsize == 64) {
			stage2(ctx, &ctx->buffer);
			ctx->bufsize = 0;
		}
	}
}

void GOST34112012Final_ref(GOST34112012Context* ctx, unsigned char* digest)
{
	Q_DECL_ALIGN(16) union uint512_u buf;

	std::memset(&ctx->buffer.BYTE[ctx->bufsize], 0, sizeof(ctx->buffer) - ctx->bufsize);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
	buf.QWORD[0] = ctx->bufsize << 3;
#else
	buf.QWORD[0] = bswap64(ctx->bufsize << 3);
#endif
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
}
