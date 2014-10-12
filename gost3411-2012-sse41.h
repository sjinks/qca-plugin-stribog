#ifndef GOST34112012SSE41_H
#define GOST34112012SSE41_H

#include <QtCore/QtGlobal>
#include "gost3411-2012.h"

Q_DECL_HIDDEN void GOST34112012Update_sse41(GOST34112012Context* ctx, const unsigned char* data, std::size_t len);
Q_DECL_HIDDEN void GOST34112012Final_sse41(GOST34112012Context* ctx, unsigned char* digest);

#endif // GOST34112012SSE41_H
