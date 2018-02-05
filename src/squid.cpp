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

void processSquidW3c(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary) {
	DEBUG_INFO(PACKAGE << ":processSquidW3c()" << "[" << *pstrData << "]");
	// Squid has their own log format, but allow custom log formats; this one
	//	seems loosely based on the W3C specs: https://www.w3.org/TR/WDlogfile.html
	// With the exception of [logfile:] and [date/time;], the rest of the fields
	//	are space-delimited.
	//
	// squid.log-20150303.gz:
	//		1425312106.781;			//Date/Time (Unix Epoch)
	//		time_taken=132				//Time taken for transaction to complete in seconds
	//		dns=-							//DNS name
	//		c_ip=w20.x61.y38.z4		//Client IP address
	//		cs_ip=192.12.184.10		//Client-to-Server IP address
	//		r_ip=w08.x4.y2.z0			//Remote IP address
	//		r_port=80					//Remote port
	//		c_port=50034				//Client port
	//		s_action=TCP_MISS
	//		sc_status=200				//Server-to-Client status code
	//		l_err=-
	//		sc_bytes=61761				//Server-to-Client bytes transferred
	//		cs_method=GET				//Client-to-Server method
	//		c_uri="http://www..."	//Client URL
	//		content_type=image/png	//MIME type
	//		referer="http://www..."	//Referer
	//		user_agent="Mozilla..."	//Client user agent

	//TODO - This is a problem... some of these logs files come with the log file included in the data; others do not. Need a more robust way to handle both options.
	// string strTime = 		findSubString(*pstrData, 0, "", ".");
	string strTime = 		findSubString(*pstrData, 0, "", ".");
	DEBUG_INFO(strTime);
	
	string strSrc = 		findSubString(*pstrData, 0, " c_ip=", " ") +
								"/" + findSubString(*pstrData, 0, " cs_ip=", " ") + ":" +
								findSubString(*pstrData, 0, " c_port=", " ");

	string strDst =		findSubString(*pstrData, 0, " r_ip=", " ") + ":" +
								findSubString(*pstrData, 0, " r_port=", " ");

	string strBytes = 	findSubString(*pstrData, 0, " sc_bytes=", " ");

	string strMethod =	findSubString(*pstrData, 0, " cs_method=", " ");
	string strURI = 		findSubString(*pstrData, 0, " c_uri=", " "); 
	string strName = 		strMethod + " " + strURI;

	if (bNormalize) {
		strMethod = stripQualifiers(strMethod, '"');
		strURI = stripQualifiers(strURI, '"');
		strName = addQualifiers(strMethod + " " + strURI, '"');
	}

	//Output Values
	strFields[MULTI2MAC_DETAIL]	= strName;
	//strFields[MULTI2MAC_TYPE]	= strService;
	strFields[MULTI2MAC_LOG]		= "squidw3c";
	//strFields[MULTI2MAC_FROM]	= strSrc;
	//strFields[MULTI2MAC_TO]		= strDst;
	strFields[MULTI2MAC_SIZE]		= strBytes;
	strFields[MULTI2MAC_ATIME]		= strTime;
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_CRTIME]	= 
	
	//Find and passback the referal URL as a second entry for the timeline.
	string strURI2 = 		findSubString(*pstrData, 0, " referer=", " ");
	// Check to make sure referal URL is valid/useful before adding it to the timeline.
	if (strURI2 != "\"-\"") {
		string strName2 = 	"REFERER " + strURI2;

		if (bNormalize) {
			strURI2 = stripQualifiers(strURI2, '"');
			strName2 = addQualifiers("REFERER " + strURI2, '"');
		}

		//Output Values
		strSecondary[MULTI2MAC_DETAIL]	= strName2;
		//strSecondary[MULTI2MAC_TYPE]	= strService;
		strSecondary[MULTI2MAC_LOG]		= "squidw3c";
		//strSecondary[MULTI2MAC_FROM]	= strSrc;
		//strSecondary[MULTI2MAC_TO]		= strDst;
		strSecondary[MULTI2MAC_SIZE]		= strBytes;
		strSecondary[MULTI2MAC_ATIME]		= strTime;
		//strSecondary[MULTI2MAC_MTIME]	= 
		//strSecondary[MULTI2MAC_CTIME]	= 
		//strSecondary[MULTI2MAC_CRTIME]	= 
	}
}

