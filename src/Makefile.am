AUTOMAKE_OPTIONS = foreign

SUBDIRS = traderflow

bin_PROGRAMS = zone
zone_SOURCES = \
	ZoneConfig.cc \
	TraderServer.cc \
	Main.cc 


zone_CPPFLAGS = -I$(top_srcdir) \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(ZERO_CPPFLAGS) \
		$(AIR_CPPFLAGS)


zone_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD) \
	$(ZERO_LIBADD) \
	$(AIR_LIBADD)


zone_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = ZoneConfig.hh \
		ZoneLog.hh \
		TraderServer.hh


EXTRA_DIST = env.sh zone.cfg log.cfg

zonedir = $(bindir)
zone_DATA = $(EXTRA_DIST)
