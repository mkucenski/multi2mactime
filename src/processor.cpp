// Copyright 2018 Matthew A. Kucenski
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//#define _DEBUG_
#include "misc/debugMsgs.h"
#include "misc/errMsgs.h"

#include "processor.h"

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libtimeUtils/src/timeUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

// TODO Unix32 is unable to handle dates past the year 2038...

int32_t getUnix32DateTimeFromString2(string strDateTime, char chSeparator, char chDateDelim, char chTimeDelim, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
	DEBUG("getUnix32DateTimeFromString2() " << strDateTime);
	// Sample2:	2017-05-16 16:17:09

	int32_t rv = -1; 

	if (strDateTime.length()) {
		DEBUG("getUnix32DateTimeFromString2() valid length");
		delimTextRow delimDateTime(strDateTime, chSeparator);
		DEBUG(delimDateTime.getData());
		delimTextRow delimDate(delimDateTime.getField(0), chDateDelim);
		DEBUG(delimDate.getData());
		delimTextRow delimTime(delimDateTime.getField(1), chTimeDelim);
		DEBUG(delimTime.getData());

		// TODO	The only reason for this function is to handle IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY for which IEF only stores a local (non-UTC) time in a weird format.
		// 		Since I do not yet have a way to handle intermingling of UTC/local times -- strip out the time value and use only the date.
		// getUnix32FromStrings(delimDate.getField(1), delimDate.getField(2), delimDate.getField(0), delimTime.getField(0), delimTime.getField(1), delimTime.getField(2), uiSkew, pTZCalc);
		rv = getUnix32FromStrings(delimDate.getField(1), delimDate.getField(2), delimDate.getField(0), to_string(0), to_string(0), to_string(0), uiSkew, pTZCalc);
	}

	return rv;
}


int32_t getUnix32DateTimeFromString(string strDateTime, char chSeparator, char chDateDelim, char chTimeDelim, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
	DEBUG("getUnix32DateTimeFromString() " << strDateTime);
	// Sample:	1/10/2009 8:16:10 PM
	// The idea here is that there is a common separator between the date, time, and AM/PM fields. There are
	// also alternate separators between the month/day/year and the hour/minute/second fields.

	int32_t rv = -1; 

	if (strDateTime.length()) {
		delimTextRow delimDateTime(strDateTime, chSeparator);
		delimTextRow delimDate(delimDateTime.getField(0), chDateDelim);
		delimTextRow delimTime(delimDateTime.getField(1), chTimeDelim);

		u_int16_t uiHour = 0;
		string strMinute = "0";
		string strSecond = "0";
		if (delimTime.getDataLength() > 0) {
			uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
			if (delimDateTime.getField(2) == "PM") {
				uiHour = (uiHour != 12 ? uiHour + 12 : uiHour);
			} else {
				uiHour = (uiHour == 12 ? 0 : uiHour);
			}
			strMinute = delimTime.getField(1);
			strSecond = delimTime.getField(2);
		}
		rv = getUnix32FromStrings(delimDate.getField(0), delimDate.getField(1), delimDate.getField(2), to_string(uiHour), strMinute, strSecond, uiSkew, pTZCalc);
	}

	return rv;
}

int32_t getUnix32FromStrings(string strMonth, string strDay, string strYear, string strHour, string strMinute, string strSecond, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
	DEBUG("getUnix32FromStrings() " << strMonth << "-" << strDay << "-" << strYear << " " << strHour << ":" << strMinute << ":" << strSecond << ")"); 
	int32_t rv = -1;

	try {
		boost::local_time::local_date_time ldt(boost::local_time::not_a_date_time);
		if (pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(strMonth),
							 					boost_lexical_cast_wrapper<u_int16_t>(strDay),
												boost_lexical_cast_wrapper<u_int16_t>(strYear),
												boost_lexical_cast_wrapper<u_int16_t>(strHour),
												boost_lexical_cast_wrapper<u_int16_t>(strMinute),
												boost_lexical_cast_wrapper<u_int16_t>(strSecond),
												&ldt)) {
			rv = getUnix32FromLocalTime(ldt + boost::posix_time::seconds(uiSkew));
		} else {
			ERROR("getUnix32FromStrings() Unable to createLocalTime(" << strMonth << "-" << strDay << "-" << strYear << " " << strHour << ":" << strMinute << ":" << strSecond << ")");
		}
	} catch (...) {
		ERROR("getUnix32FromStrings() Caught exception converting string (" << strMonth << "-" << strDay << "-" << strYear << " " << strHour << ":" << strMinute << ":" << strSecond << ")");
	}

	return rv;
}

