#include <cassert>
#include <cstring>
#include "stribogcontext.h"

StribogContext::StribogContext(QCA::Provider* provider, const QString& type)
	: QCA::HashContext(provider, type), m_ctx()
{
	uint bits = (QLatin1String("stribog-256") == type) ? 256 : 512;
	GOST34112012Init(&this->m_ctx, bits);
}

void StribogContext::clear(void)
{
	GOST34112012Init(&this->m_ctx, this->m_ctx.digest_size);
}

void StribogContext::update(const QCA::MemoryRegion& a)
{
	const unsigned char* data = reinterpret_cast<const unsigned char*>(a.data());
	std::size_t d             = reinterpret_cast<std::size_t>(data);
	std::size_t offset        = ((d + 15) & ~0x0F) - d;

	if (!offset) {
		GOST34112012Update(&this->m_ctx, data, a.size());
	}
	else {
		Q_DECL_ALIGN(16) unsigned char tmp[15];
		assert(offset < 16);

		memcpy(tmp, data, offset);
		GOST34112012Update(&this->m_ctx, tmp, offset);
		GOST34112012Update(&this->m_ctx, data + offset, a.size() - offset);
	}
}

QCA::MemoryRegion StribogContext::final(void)
{
	QCA::SecureArray a(this->m_ctx.digest_size >> 3);
	GOST34112012Final(&this->m_ctx, reinterpret_cast<unsigned char*>(a.data()));
	return a;
}

QCA::Provider::Context* StribogContext::clone(void) const
{
	return new StribogContext(*this);
}
