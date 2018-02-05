#AM_CXXFLAGS = -I../../ $(BOOST_CPPFLAGS)
#AM_LDFLAGS = $(BOOST_LDFLAGS)

bin_PROGRAMS = multi2mactime
multi2mactime_SOURCES = multi2mactime.cpp custom.cpp fortigate.cpp griffeye.cpp hirsch.cpp juniper.cpp pix.cpp processor.h squid.cpp symantec.cpp
multi2mactime_CXXFLAGS = $(POPT_CFLAGS) -I../../../
multi2mactime_LDADD = $(POPT_LIBS) ../../../libtimeUtils/src/libtimeUtils.a ../../../libdelimText/src/libdelimText.a

