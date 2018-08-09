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

// #define _DEBUG_
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

void processGriffeyeCSV(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG("processGriffeyeCSV()");

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');

	string strBirthed = delimText.getValue(delimHeader.getColumnByValue("Created Date"));
	string strAccessed = delimText.getValue(delimHeader.getColumnByValue("Last Accessed"));
	string strModified = delimText.getValue(delimHeader.getColumnByValue("Last Write Time"));
	string strCreated = delimText.getValue(delimHeader.getColumnByValue("Exif: CreateDate"));
	DEBUG("processGriffeyeCSV() (B) " << strBirthed << "; (A) " << strAccessed << "; (M) " << strModified << "; (C) " << strCreated);

	int32_t bTimeVal = getUnix32DateTimeFromString(strBirthed, ' ', '/', ':', uiSkew, pTZCalc);
	int32_t aTimeVal = getUnix32DateTimeFromString(strAccessed, ' ', '/', ':', uiSkew, pTZCalc);
	int32_t mTimeVal = getUnix32DateTimeFromString(strModified, ' ', '/', ':', uiSkew, pTZCalc);
	int32_t cTimeVal = getUnix32DateTimeFromString(strCreated, ' ', '/', ':', uiSkew, pTZCalc);

	// There needs to be at least one valid time value before anything else makes sense.
	if (bTimeVal > 0 || aTimeVal > 0 || mTimeVal > 0 || cTimeVal > 0) {
		//Output Values
		strFields[MULTI2MAC_HASH]		= delimText.getValue(delimHeader.getColumnByValue("MD5"));
		// TODO How to handle slashes in file listings--when trying to correlate/compare between MCT records from different sources (e.g. Griffeye to TSK), you have to compensate...
		strFields[MULTI2MAC_DETAIL]	= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Directory Path")), '"') + "\\" + stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("File Name")), '"');
		strFields[MULTI2MAC_TYPE]		= string("cat") + delimText.getValue(delimHeader.getColumnByValue("Category"));
		strFields[MULTI2MAC_LOG]		= "griffeye";
		//strFields[MULTI2MAC_FROM]	= 
		//strFields[MULTI2MAC_TO]		= 
		strFields[MULTI2MAC_SIZE]		= delimText.getValue(delimHeader.getColumnByValue("File Size"));
		strFields[MULTI2MAC_ATIME]		= (aTimeVal > 0 ? boost_lexical_cast_wrapper<string>(aTimeVal) : "");
		strFields[MULTI2MAC_MTIME]		= (mTimeVal > 0 ? boost_lexical_cast_wrapper<string>(mTimeVal) : "");
		strFields[MULTI2MAC_CTIME]		= (cTimeVal > 0 ? boost_lexical_cast_wrapper<string>(cTimeVal) : "");
		strFields[MULTI2MAC_BTIME]		= (bTimeVal > 0 ? boost_lexical_cast_wrapper<string>(bTimeVal) : "");
	} else {
		WARNING("processGriffeyeCSV() No valid time values found (" << *pstrData << ")");
	}
}

