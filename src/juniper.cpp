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

void processJuniper(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	int32_t timeVal = -1; 
	string strTime = findSubString(*pstrData, 34, "start_time=\"", "\" ");
	if (strTime.length()) {
		local_time::local_date_time ldt = pTZCalc->createLocalTime(strTime, "%Y-%m-%d %H:%M:%S") + posix_time::seconds(uiSkew);
		timeVal = getUnix32FromLocalTime(ldt);
	}
	string strMsg = findSubString(*pstrData, 34, "[", "");
	string strMsgType = findSubString(strMsg, 34, "]", ": ");
	strMsg = findSubString(strMsg, 0, ": ", "");
	
	string strSrc = findSubString(*pstrData, 34, "src=", " ");
	if (!strSrc.length()) {
		strSrc = findSubString(*pstrData, 34, " from ", "/");
	}
	string strDst = findSubString(*pstrData, 34, "dst=", " ");
	if (!strDst.length()) {
		strDst = findSubString(*pstrData, 34, " to ", "/");
	}
	
	string strService = findSubString(*pstrData, 34, "service=", " ");
	
	string strBytes;
	string strSent = findSubString(*pstrData, 34, "sent=", " ");
	string strRcvd = findSubString(*pstrData, 34, "rcvd=", " ");
	if (strSent.length() || strRcvd.length()) {
			strBytes = strSent + "/" + strRcvd;
	}

	//Output Values
	strFields[MULTI2MAC_DETAIL]	= strMsg;
	strFields[MULTI2MAC_TYPE]		= strMsgType + ":" + strService;
	strFields[MULTI2MAC_LOG]		= "-juniper";
	strFields[MULTI2MAC_FROM]		= strSrc;
	strFields[MULTI2MAC_TO]			= strDst;
	strFields[MULTI2MAC_SIZE]		= strBytes;
	strFields[MULTI2MAC_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[MULTI2MAC_MTIME]	= 
	//strFields[MULTI2MAC_CTIME]	= 
	//strFields[MULTI2MAC_CRTIME]	= 
}

