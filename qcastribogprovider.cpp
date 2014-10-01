#include "qcastribogprovider.h"
#include "stribogcontext.h"

void QCAStribogProvider::init(void)
{
}

void QCAStribogProvider::deinit(void)
{
}

int QCAStribogProvider::version(void) const
{
	return 0x010000;
}

int QCAStribogProvider::qcaVersion(void) const
{
	return QCA_VERSION;
}

QString QCAStribogProvider::name(void) const
{
	return QLatin1String("qca-stribog");
}

QStringList QCAStribogProvider::features(void) const
{
	QStringList features;
	features.append(QLatin1String("stribog-256"));
	features.append(QLatin1String("stribog-512"));
	return features;
}

QCA::Provider::Context* QCAStribogProvider::createContext(const QString& type)
{
	if (type == QLatin1String("stribog-256") || type == QLatin1String("stribog-512")) {
		return new StribogContext(this, type);
	}

	return 0;
}
