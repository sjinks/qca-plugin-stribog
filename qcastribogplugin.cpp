#include <QtCore/QtPlugin>
#include "qcastribogplugin.h"
#include "qcastribogprovider.h"

QCAStribogPlugin::QCAStribogPlugin(QObject* parent)
	: QObject(parent), QCAPlugin()
{
}

QCAStribogPlugin::~QCAStribogPlugin(void)
{
}

QCA::Provider* QCAStribogPlugin::createProvider(void)
{
	return new QCAStribogProvider();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qca-stribog-plugin, QCAStribogPlugin)
#endif // QT_VERSION < 0x050000
