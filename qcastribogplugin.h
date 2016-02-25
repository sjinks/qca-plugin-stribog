#ifndef QCASTRIBOGPLUGIN_H
#define QCASTRIBOGPLUGIN_H

#include <QtCrypto/qcaprovider.h>

class Q_DECL_HIDDEN QCAStribogPlugin : public QObject, public QCAPlugin {
	Q_OBJECT
	Q_INTERFACES(QCAPlugin)
#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "com.affinix.qca.Plugin/1.0")
#endif
public:
	QCAStribogPlugin(QObject *parent = 0);
	virtual ~QCAStribogPlugin(void);
	virtual QCA::Provider* createProvider(void);
};

#endif // QCASTRIBOGPLUGIN_H
