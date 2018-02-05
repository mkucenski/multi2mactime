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

#include "processor.h"

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libtimeUtils/src/timeUtils.h"
#include "libdelimText/src/textUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

void processCustomVPN_S1(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomVPN_S1()");

	//Date,Time,user,src_ip,dest_ip
	//2/12/17,15:59:31
	delimTextRow delimText(*pstrData, ',');
	string strDate = delimText.getField(0);
	if (strDate != "Date") {
		string strTime = delimText.getField(1);
		delimTextRow delimDate(strDate, '/');
		delimTextRow delimTime(strTime, ':');

		int32_t timeVal = -1; 
		if (strDate.length() && strTime.length()) {
			DEBUG_INFO(	strDate << " " << strTime << "\n" <<
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
					  DEBUG_INFO("whoops");
				//ERROR
			} //if (	(1 <= uiMonth && uiMonth <= 12) &&
		}
	
		//Output Values
		strFields[MULTI2MAC_DETAIL]	= delimText.getField(2);	//username
		//strFields[MULTI2MAC_TYPE]	= strService;
		strFields[MULTI2MAC_LOG]		= "-----vpn";
		strFields[MULTI2MAC_FROM]		= delimText.getField(3);	//src_ip
		strFields[MULTI2MAC_TO]			= delimText.getField(4);	//dst_ip
		//strFields[MULTI2MAC_SIZE]	= 
		strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
		//strFields[MULTI2MAC_MTIME]	= 
		//strFields[MULTI2MAC_CTIME]	= 
		//strFields[MULTI2MAC_CRTIME]	= 

	} //if (strDate != "Date") {
}

void processCustomFSEM(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomFSEM()");

	//Date (UTC)				IP							ed2k Hash									Filename
	// 8/1/2016 8:11:17 PM	107.77.172.33 :8004	54972296B9DD3CFE194FD3328827DD42	
	delimTextRow delimText(*pstrData, '\t');
	string strDateTime = delimText.getField(0);
	delimTextRow delimDateTime(strDateTime, ' ');
	delimTextRow delimDate(delimDateTime.getField(0), '/');
	delimTextRow delimTime(delimDateTime.getField(1), ':');

	int32_t timeVal = -1; 
	if (strDateTime.length()) {
		u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
		u_int16_t uiHourDbg = uiHour;
		if (delimDateTime.getField(2) == "PM") {
			if (uiHour != 12) {
				uiHour += 12;
			}
		} else {
			if (uiHour == 12) {
				uiHour = 0;
			}
		}
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		timeVal = getUnix32FromLocalTime(ldt);
	}

	string strIPPort = delimText.getField(1);
	string strIP = findSubString(strIPPort, 0, "", " ");
	string strPort = findSubString(strIPPort, 0, " :", "");

	//Output Values
	strFields[MULTI2MAC_DETAIL]		= delimText.getField(2);
	//strFields[MULTI2MAC_TYPE]		= strService;
	strFields[MULTI2MAC_LOG]			= "---emule";
	strFields[MULTI2MAC_FROM]		= strIP;
	//strFields[MULTI2MAC_TO]		= strDst;
	//strFields[MULTI2MAC_SIZE]		= 
	strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_CRTIME]	= 
}

void processCustomFSBT(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomFSBT()");

	//Date (UTC)	IP		Infohash		Severity		No. Of FOI (tab-separated, copy/paste from website)
	//"8/1/2017  8:06:25 AM"

	delimTextRow delimText(*pstrData, '\t');

	string strDateTime = delimText.getField(0);
	int32_t timeVal = -1; 
	if (strDateTime.length()) {
		delimTextRow delimDateTime(strDateTime, ' ');
		delimTextRow delimDate(delimDateTime.getField(0), '/');
		delimTextRow delimTime(delimDateTime.getField(1), ':');

		u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
		u_int16_t uiHourDbg = uiHour;
		if (delimDateTime.getField(2) == "PM") {
			if (uiHour != 12) {
				uiHour += 12;
			}
		} else {
			if (uiHour == 12) {
				uiHour = 0;
			}
		}
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		timeVal = getUnix32FromLocalTime(ldt);
	}

	string strIPPort = delimText.getField(1);
	string strIP = findSubString(strIPPort, 0, "", " ");
	string strPort = findSubString(strIPPort, 0, " :", "");

	//Output Values
	strFields[MULTI2MAC_DETAIL]		= delimText.getField(2);
	//strFields[MULTI2MAC_TYPE]		= strService;
	strFields[MULTI2MAC_LOG]			= "bittorre";
	strFields[MULTI2MAC_FROM]		= strIP;
	//strFields[MULTI2MAC_TO]		= strDst;
	strFields[MULTI2MAC_SIZE]		= delimText.getField(4);
	strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_CRTIME]	= 
}

