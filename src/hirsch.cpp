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
#include "libdelimText/src/textUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

void processHirsch(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG("processHirsch()");
	//"	   Host Date/Time BETWEEN '2017-08-23 00:00:00' AND '2017-08-25 23:59:59'   ","<SITE>","All Events Log By Date","Print Time:","8/30/2017","12:07:07PM","Printed by:","<USER>","Sequence ID","Host Date/Time","Controller Date/Time","Description","Event ID","Address",285061,"8/25/2017  11:59:59PM","8/26/2017  12:00:00AM","Updating temporary users",8010,"\\XNET.001.0004.001.01","Page -1 of 1"
	//"	   Host Date/Time BETWEEN '2017-08-23 00:00:00' AND '2017-08-25 23:59:59'   "
	
	//"<SITE>"							0
	//"All Events Log By Date"		1
	//"Print Time:"					2
	//"8/30/2017"						3
	//"12:07:07PM"						4
	//"Printed by:"					5
	//"<USER>"							6
	//"Sequence ID"					7
	//"Host Date/Time"				8
	//"Controller Date/Time"		9
	//"Description"					10
	//"Event ID"						11
	//"Address"							12
	//285061								13
	//"8/25/2017  11:59:59PM"		14
	//"8/26/2017  12:00:00AM"		15
	//"Updating temporary users"	16
	//8010								17
	//"\\XNET.001.0004.001.01"		18
	//"Page -1 of 1"					19
	
	delimTextRow delimText(*pstrData, ',');
	string strDateTime = delimText.getField(14);
	string strDate = findSubString(strDateTime, 0, "", " ");
	string strTime = findSubString(findSubString(findSubString(strDateTime, 0, " ", ""), 0, " ", ""), 0, " ", ""); //time may have up to three leading spaces; dirty way to get rid of them...

	delimTextRow delimDate(strDate, '/');
	delimTextRow delimTime(strTime, ':');

		int32_t timeVal = -1; 
		if (strDate.length() && strTime.length()) {
			DEBUG(	strDate << " " << strTime << "\n" <<
							" month:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)) <<
							" day:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)) <<
							" year:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)) <<
							" hour:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0)) <<
							" minute:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)) <<
							" second:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)));
			u_int16_t uiMonth = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0));
			u_int16_t uiDay = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1));
			u_int16_t uiYear = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2));
			u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
			u_int16_t uiMin = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1));
			u_int16_t uiSec = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2));

			if (uiYear < 100) {
				uiYear += 2000;
			} //if (uiYear < 100) {

			if (	(1 <= uiMonth && uiMonth <= 12) &&
					(1 <= uiDay && uiDay <= 31) &&
					(1400 <= uiYear && uiYear <= 10000) &&
					(0 <= uiHour && uiHour <= 23) &&
					(0 <= uiMin && uiMin <= 60) &&
					(0 <= uiSec && uiSec <= 60)) {
				local_time::local_date_time ldt = pTZCalc->createLocalTime(uiMonth, uiDay, uiYear, uiHour, uiMin, uiSec) + posix_time::seconds(uiSkew);
				timeVal = getUnix32FromLocalTime(ldt);
			} else {
					  DEBUG("whoops");
				//ERROR
			} //if (	(1 <= uiMonth && uiMonth <= 12) &&
		}
	
	/* This was never actually completed...
	string strTime =		findSubString(*pstrData, 0, "itime=", "\"");
	string strSrc = 		findSubString(*pstrData, 0, "srcip=", "\"") + ":" + 
			  					findSubString(*pstrData, 0, "srcport=", "\"");
	string strDst = 		findSubString(*pstrData, 0, "dstip=", "\"") + ":" + 
			  					findSubString(*pstrData, 0, "dstport=", "\"");
	string strURL = 		findSubString(*pstrData, 0, "referralurl=", ",");
	string strService =	findSubString(*pstrData, 0, "service=", "\"");		  					
	string strBytes = 	findSubString(*pstrData, 0, "sentbyte=", "\"") + "/" +
								findSubString(*pstrData, 0, "rcvdbyte=", "\"");

	//Output Values
	strFields[MULTI2MAC_DETAIL]	= strURL;
	strFields[MULTI2MAC_TYPE]		= strService;
	strFields[MULTI2MAC_LOG]		= "----fortg1k5";
	strFields[MULTI2MAC_FROM]		= strSrc;
	strFields[MULTI2MAC_TO]			= strDst;
	//strFields[MULTI2MAC_SIZE]	= 
	strFields[MULTI2MAC_ATIME]		= strTime;
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_BTIME]	= 
	*/
}

