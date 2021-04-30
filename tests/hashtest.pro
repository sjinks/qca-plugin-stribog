QT       = core testlib
CONFIG  += console crypto debug
CONFIG  -= app_bundle
TARGET   = tst_stribog
TEMPLATE = app
SOURCES += hashtest.cpp

lessThan(QT_MAJOR_VERSION, 5) {
	CONFIG += qtestlib
}
else {
	DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
}
