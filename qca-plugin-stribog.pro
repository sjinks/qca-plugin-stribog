QT      += core
QT      -= gui
TARGET   = qca-plugin-stribog
TEMPLATE = lib
CONFIG  += plugin crypto bsymbolic_functions hide_symbols no_plugin_name_prefix

SOURCES += \
	qcastribogprovider.cpp \
	qcastribogplugin.cpp \
	stribogcontext.cpp \
	gost3411-2012-mmx.cpp \
	gost3411-2012-ref.cpp \
	gost3411-2012-sse2.cpp \
	gost3411-2012.cpp \
	tables.cpp \
	gost3411-2012-sse41.cpp

HEADERS += \
	qcastribogplugin.h \
	qcastribogprovider.h \
	stribogcontext.h \
	gost3411-2012-mmx.h \
	gost3411-2012-ref.h \
	gost3411-2012-sse2.h \
	gost3411-2012.h \
	tables.h \
	gost3411-2012-sse41.h

*-g++* {
	system("echo '$$LITERAL_HASH include <cpuid.h>' | g++ -c -xc++ - -o /dev/null > /dev/null 2>&1") {
		DEFINES += HAVE_CPUID_H
	}
}

target.path = $$[QT_INSTALL_PLUGINS]/crypto
INSTALLS   += target
