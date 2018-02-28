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

void processFortiGate1K5(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG("processFortiGate1K5()");
	// "itime=1503697041","date=2017-08-25","time=15:37:21","devid=FG1K5D3I16804933","vd=root","type=""utm""","subtype=""webfilter""","action=""passthrough""","","","","","","","","","cat=52","catdesc=""Information Technology""","","","","","","","","","devname=FG1Kcopper","direction=""outgoing""","","dstintf=""port26""","dstintfrole=""undefined""","dstip=54.243.44.67","dstport=80","dtime=1503675441","","eventtype=""ftgd_allow""","","","hostname=""edge.simplereach.com""","","","","level=""notice""","logid=""0317013312""","logtime=1503697041","logver=56","method=""domain""","msg=""URL belongs to an allowed category in policy""","policyid=1","","","profile=""NTC_Web_CTA""","proto=6","rcvdbyte=0","","","referralurl=""http://www.cracked.com/pictofacts-766-28-things-you-completely-misunderstood-as-child-part-2/""","reqtype=""referral""","","sentbyte=1014","","service=""HTTP""","sessionid=18827768","","","srcintf=""port17""","srcintfrole=""undefined""","srcip=172.31.246.13","srcport=63661","","","","","","","url=""/t?pid=4f6a4e1ea782f30c41000002&title=28%20Things%20You%20Completely%20Misunderstood%20As%20A%20Child%2C%20Part%202&url=http://www.cracked.com/pictofacts-766-28-things-you-completely-misunderstood-as-child-part-2/&page_url=http://www.cracked.com/pictofacts-766-2
	
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
}

