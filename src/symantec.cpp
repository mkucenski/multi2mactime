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

void processSymantec(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG("processSymantec()");

	local_time::local_date_time ldt = pTZCalc->createLocalTime(boost_lexical_cast_wrapper<string>(uiYear) + " " + string(*pstrData, 0, 19), "%Y %b %d %H:%M:%S%F") + posix_time::seconds(uiSkew);
	int32_t timeVal = getUnix32FromLocalTime(ldt);
	
	string strMsgType = findSubString(*pstrData, 23, "", ": ");
	string strMsg = findSubString(*pstrData, 23, ": ", "");

	string strBytes;
	string strSent = findSubString(*pstrData, 23, "sent=", " ");
	string strRcvd = findSubString(*pstrData, 23, "rcvd=", " ");
	if (strSent.length() || strRcvd.length()) {
			strBytes = strSent + "/" + strRcvd;
	}

	string strService = findSubString(findSubString(*pstrData, 23, "", ": "), 0, "", "[");
	string strSrc = findSubString(*pstrData, 23, "src=", "/");
	string strDst = findSubString(*pstrData, 23, "dst=", "/");

	//Output Values
	strFields[MULTI2MAC_DETAIL]	= strMsg;
	strFields[MULTI2MAC_TYPE]		= strMsgType;
	strFields[MULTI2MAC_LOG]		= "----symantec";
	strFields[MULTI2MAC_FROM]		= strSrc;
	strFields[MULTI2MAC_TO]			= strDst;
	strFields[MULTI2MAC_SIZE]		= strBytes;
	strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_BTIME]	= 
}

