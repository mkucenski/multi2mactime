#AM_CXXFLAGS = -I../../ $(BOOST_CPPFLAGS)
#AM_LDFLAGS = $(BOOST_LDFLAGS)

bin_PROGRAMS = multi2mactime
multi2mactime_SOURCES = multi2mactime.cpp processor.cpp custom.cpp fortigate.cpp griffeye.cpp ief.cpp hirsch.cpp juniper.cpp pix.cpp processor.h squid.cpp symantec.cpp ../../misc/errMsgs.cpp
multi2mactime_CXXFLAGS = $(POPT_CFLAGS) -I../../../
multi2mactime_LDADD = $(POPT_LIBS) ../../../libtimeUtils/build/src/libtimeUtils.a ../../../libdelimText/build/src/libdelimText.a

