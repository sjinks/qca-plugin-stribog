#include <QtCore/QCoreApplication>
#include <QtTest/QTest>
#include <QtCrypto>

class StribogTests : public QObject {
	Q_OBJECT
private Q_SLOTS:
	void initTestCase(void)
	{
		if (!QCA::isSupported("stribog-256") || !QCA::isSupported("stribog-512")) {
			QSKIP(
				"qca-plugin-stribog is not loaded"
#if QT_VERSION < 0x050000
				, SkipAll
#endif
			);
		}
	}

	void test_Vector1(void)
	{
		QCA::SecureArray vec("012345678901234567890123456789012345678901234567890123456789012");
		QString actual256   = QCA::Hash(QLatin1String("stribog-256")).hashToString(vec);
		QString actual512   = QCA::Hash(QLatin1String("stribog-512")).hashToString(vec);
		QString expected256 = QLatin1String("9d151eefd8590b89daa6ba6cb74af9275dd051026bb149a452fd84e5e57b5500");
		QString expected512 = QLatin1String("1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48");

		QCOMPARE(actual256, expected256);
		QCOMPARE(actual512, expected512);
	}

	void test_Vector2(void)
	{
		QCA::SecureArray vec(QByteArray::fromBase64("0eUg4uXy8OgsINHy8Ojh7uboIOLt8/boLCDi5f7y+iDxIOzu8P8g8fLw5evg7Ogg7eAg9fDg4fD7/yDv6/rq+yDI4+7w5eL7"));
		QString actual256   = QCA::Hash(QLatin1String("stribog-256")).hashToString(vec);
		QString actual512   = QCA::Hash(QLatin1String("stribog-512")).hashToString(vec);
		QString expected256 = QLatin1String("9dd2fe4e90409e5da87f53976d7405b0c0cac628fc669a741d50063c557e8f50");
		QString expected512 = QLatin1String("1e88e62226bfca6f9994f1f2d51569e0daf8475a3b0fe61a5300eee46d961376035fe83549ada2b8620fcd7c496ce5b33f0cb9dddc2b6460143b03dabac9fb28");

		QCOMPARE(actual256, expected256);
		QCOMPARE(actual512, expected512);
	}

	void test_Vector1Incremental(void)
	{
		QByteArray vec("012345678901234567890123456789012345678901234567890123456789012");
		QCA::Hash h256(QLatin1String("stribog-256"));
		QCA::Hash h512(QLatin1String("stribog-512"));

		for (int i=0; i<vec.size(); ++i) {
			QCA::SecureArray part = vec.mid(i, 1);
			h256.update(part);
			h512.update(part);
		}

		QCA::SecureArray result256 = h256.final();
		QCA::SecureArray result512 = h512.final();

		QString actual256   = QCA::arrayToHex(result256.toByteArray());
		QString actual512   = QCA::arrayToHex(result512.toByteArray());
		QString expected256 = QLatin1String("9d151eefd8590b89daa6ba6cb74af9275dd051026bb149a452fd84e5e57b5500");
		QString expected512 = QLatin1String("1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48");

		QCOMPARE(actual256, expected256);
		QCOMPARE(actual512, expected512);
	}

	void test_Vector2Incremental(void)
	{
		QByteArray vec = QByteArray::fromBase64("0eUg4uXy8OgsINHy8Ojh7uboIOLt8/boLCDi5f7y+iDxIOzu8P8g8fLw5evg7Ogg7eAg9fDg4fD7/yDv6/rq+yDI4+7w5eL7");

		QCA::Hash h256(QLatin1String("stribog-256"));
		QCA::Hash h512(QLatin1String("stribog-512"));

		for (int i=0; i<vec.size(); i+=32) {
			QCA::SecureArray part = vec.mid(i, 32);
			h256.update(part);
			h512.update(part);
		}

		QCA::SecureArray result256 = h256.final();
		QCA::SecureArray result512 = h512.final();

		QString actual256   = QCA::arrayToHex(result256.toByteArray());
		QString actual512   = QCA::arrayToHex(result512.toByteArray());
		QString expected256 = QLatin1String("9dd2fe4e90409e5da87f53976d7405b0c0cac628fc669a741d50063c557e8f50");
		QString expected512 = QLatin1String("1e88e62226bfca6f9994f1f2d51569e0daf8475a3b0fe61a5300eee46d961376035fe83549ada2b8620fcd7c496ce5b33f0cb9dddc2b6460143b03dabac9fb28");

		QCOMPARE(actual256, expected256);
		QCOMPARE(actual512, expected512);
	}
};

#include "hashtest.moc"

int main(int argc, char **argv)
{
	QCA::Initializer init;
	QCoreApplication app(argc, argv);
	StribogTests t;

	return QTest::qExec(&t, argc, argv);
}
