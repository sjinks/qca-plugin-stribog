#ifndef GOST3411_2012_H
#define GOST3411_2012_H

#include <QtCore/QtGlobal>

union Q_DECL_ALIGN(16) uint512_u {
	quint64 QWORD[8];
	quint8 BYTE[64];
};

typedef struct Q_DECL_ALIGN(16) GOST34112012Context {
	Q_DECL_ALIGN(16) union uint512_u buffer;
	Q_DECL_ALIGN(16) union uint512_u hash;
	Q_DECL_ALIGN(16) union uint512_u h;
	Q_DECL_ALIGN(16) union uint512_u N;
	Q_DECL_ALIGN(16) union uint512_u Sigma;
	std::size_t bufsize;
	unsigned int digest_size;
} GOST34112012Context;

Q_DECL_HIDDEN void GOST34112012Init(GOST34112012Context* ctx, const unsigned int digest_size);
Q_DECL_HIDDEN extern void GOST34112012Update(GOST34112012Context* ctx, const unsigned char* data, std::size_t len);
Q_DECL_HIDDEN extern void GOST34112012Final(GOST34112012Context* ctx, unsigned char* digest);

#endif /* GOST3411_2012_H */
