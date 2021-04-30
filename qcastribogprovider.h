#ifndef QCASTRIBOGPROVIDER_H
#define QCASTRIBOGPROVIDER_H

#include <qcaprovider.h>

class Q_DECL_HIDDEN QCAStribogProvider : public QCA::Provider {
public:
	virtual void init(void);
	virtual void deinit(void);
	virtual int version(void) const;
	virtual int qcaVersion(void) const;
	virtual QString name(void) const;
	virtual QStringList features(void) const;
	virtual QCA::Provider::Context* createContext(const QString& type);
};

#endif // QCASTRIBOGPROVIDER_H
