#ifndef GOST3411_2012_MMX_H
#define GOST3411_2012_MMX_H

#include <QtCore/QtGlobal>
#include "gost3411-2012.h"

Q_DECL_HIDDEN void GOST34112012Update_mmx(GOST34112012Context* ctx, const unsigned char* data, std::size_t len);
Q_DECL_HIDDEN void GOST34112012Final_mmx(GOST34112012Context* ctx, unsigned char* digest);

#endif /* GOST3411_2012_MMX_H */
