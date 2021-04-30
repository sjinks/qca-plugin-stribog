#ifndef STRIBOGCONTEXT_H
#define STRIBOGCONTEXT_H

#include <qcaprovider.h>
#include "gost3411-2012.h"

class Q_DECL_HIDDEN StribogContext : public QCA::HashContext {
public:
	StribogContext(QCA::Provider* provider, const QString& type);

	virtual void clear(void);
	virtual void update(const QCA::MemoryRegion& a);
	virtual QCA::MemoryRegion final(void);
	virtual QCA::Provider::Context* clone(void) const;

private:
	Q_DECL_ALIGN(16) GOST34112012Context m_ctx;
};

#endif // STRIBOGCONTEXT_H
