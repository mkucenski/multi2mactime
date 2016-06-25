#AM_CXXFLAGS = -I../../ $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

bin_PROGRAMS = datatime
datatime_SOURCES = datatime.cpp
datatime_CXXFLAGS = $(BOOST_CPPFLAGS) $(POPT_CFLAGS) -I../../
datatime_LDADD = $(BOOST_DATE_TIME_LIB) $(POPT_LIBS) ../../libtimeUtils/src/libtimeUtils.a ../../libdelimText/src/libdelimText.a

