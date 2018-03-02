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

void processPIX(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	int32_t timeVal = -1;
	int32_t uiPIXPos = pstrData->find("%PIX", 16);
	if (uiPIXPos > 0) {
		string strTime = string(*pstrData, uiPIXPos - 22, 20);  //Find "%PIX" and then backup 22 characters to get the PIX generated time, not the receiving syslog time
		if (strTime.length()) {
				  boost::local_time::local_date_time ldt = pTZCalc->createLocalTime(strTime, "%b %d %Y %H:%M:%S") + boost::posix_time::seconds(uiSkew);
			timeVal = getUnix32FromLocalTime(ldt);
		}
	}
	
	string strMsg = findSubString(findSubString(*pstrData, 16, ": ", ""), 0, ": ", "");
	string strMsgType = findSubString(*pstrData, 16, ": ", ": ");
	string strCode = findSubString(findSubString(strMsgType, 0, "%PIX-", ""), 0, "-", "");
	string strService;
	string strSrc;
	string strDst;
	string strBytes;
	
	if (strCode.length()) {
		u_int32_t uiCode = boost_lexical_cast_wrapper<u_int32_t>(strCode);	//Extract and convert the PIX message code
		
		//ODOT	I should come up with a way to define a .conf file for each type of log file to be able to dynamically configure
		//			which pieces of each entry get saved as src, dest, etc.
		//NOTE	I was originally truncating strMsg at 25 characters.  That would have meant that searching for an IP address might
		//			miss data if that IP was in the truncated portion and a specific parser for the event type was not found below.
		//			Since I am no longer truncating the message, searching for an IP address will always find it in the message even
		//			if it is not specifically parsed below.
		
		switch (uiCode) {
			case 106015:
				strSrc = findSubString(*pstrData, 16, " from ", "/");
				strDst = findSubString(*pstrData, 16, " to ", "/");
				break;

			case 302010:
				break;

			case 305009:
			case 305010:
				strSrc = findSubString(findSubString(*pstrData, 16, " from ", " to "), 0, ":", "/");
				strDst = findSubString(findSubString(*pstrData, 16, " to ", ""), 0, ":", "/");
				break;

			case 302013:
			case 302014:
			case 302015:
			case 302016:
				strSrc = findSubString(findSubString(*pstrData, 16, " for ", " to "), 0, ":", "/");
				strDst = findSubString(findSubString(*pstrData, 16, " to ", ""), 0, ":", "/");
				break;

			case 304001:
				strSrc = findSubString(findSubString(*pstrData, 16, "%PIX", ""), 0, ": ", " Accessed URL ");
				strDst = findSubString(*pstrData, 16, "Accessed URL ", ":");
				break;

			case 106023:
			case 305005:
			case 305006:
				strSrc = findSubString(findSubString(*pstrData, 16, " src ", " dst "), 0, ":", "/");
				strDst = findSubString(findSubString(*pstrData, 16, " dst ", ""), 0, ":", "/");
				break;

			case 609001:
			case 609002:
				strSrc = findSubString(findSubString(*pstrData, 16, " local-host ", ""), 0, ":", " ");
				break;
				
			case 303002:
				strSrc = findSubString(*pstrData, 16, "303002:  ", " Retrieved");
				strDst = findSubString(*pstrData, 16, " Retrieved ", ":");
				break;
				
			case 108002:
				strSrc = findSubString(*pstrData, 16, " in ", " data:");
				strDst = findSubString(*pstrData, 16, ": out ", " in ");
				break;

			default:
				break;
		}
	}
		
	//Output Values
	strFields[MULTI2MAC_DETAIL]	= strMsg;
	strFields[MULTI2MAC_TYPE]		= strMsgType + ":" + strService;
	strFields[MULTI2MAC_LOG]		= "---------pix";
	strFields[MULTI2MAC_FROM]		= strSrc;
	strFields[MULTI2MAC_TO]			= strDst;
	strFields[MULTI2MAC_SIZE]		= strBytes;
	strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_BTIME]	= 
}

