AM_CXXFLAGS = -I../../../ $(POPT_CFLAGS)
AM_LDFLAGS = $(POPT_LIBS)

bin_PROGRAMS = multi2mactime
multi2mactime_SOURCES = multi2mactime.cpp processor.cpp custom.cpp fortigate.cpp griffeye.cpp ief.cpp hirsch.cpp juniper.cpp pix.cpp squid.cpp symantec.cpp ../../misc/errMsgs.cpp
multi2mactime_LDADD = ../../../libtimeUtils/build/src/libtimeUtils.a ../../../libdelimText/build/src/libdelimText.a

