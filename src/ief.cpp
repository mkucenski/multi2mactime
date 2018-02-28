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

#define _DEBUG_
#include "misc/debugMsgs.h"
#include "misc/errMsgs.h"

#include "processor.h"
#include "iefTypes.h"

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libtimeUtils/src/timeUtils.h"
#include "libdelimText/src/textUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

int32_t getIEFTime(u_int32_t idArtifactFieldCode, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, delimTextRow* p_delimText, delimTextRow* p_delimHeader); 

void processIEF(string* pstrData, string* pstrHeader, string* pstrFilename, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary) {
	DEBUG(*pstrFilename << ": processIEF(pstrData = '" << *pstrData << "')");

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');

	// Strip the filename of .xlsx and/or .csv to get down to just the overall artifact name
	string strFilename = ieraseSubString(*pstrFilename, ".xlsx");
	strFilename = ieraseSubString(strFilename, ".csv");
	
	u_int32_t idArtifact = getCode(strFilename, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
	if (idArtifact > 0) {
		int32_t dtmBTime = getIEFTime(idArtifact + IEF_PRIMARY + IEF_BTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
		int32_t dtmATime = getIEFTime(idArtifact + IEF_PRIMARY + IEF_ATIME, uiSkew, pTZCalc, &delimText, &delimHeader);
		int32_t dtmMTime = getIEFTime(idArtifact + IEF_PRIMARY + IEF_MTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
		int32_t dtmCTime = getIEFTime(idArtifact + IEF_PRIMARY + IEF_CTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
		
		// There needs to be at least one valid time value before anything else makes sense.
		if (dtmBTime > 0 || dtmATime > 0 || dtmMTime > 0 || dtmCTime > 0) {
			string strDetails = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_DETAIL, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			string strDetail2 = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_DETAIL2, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strDetails += (strDetail2 != "" ? " (" + strDetail2 + ")" : "");
			string strDetail3 = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_DETAIL3, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strDetails += (strDetail3 != "" ? " (" + strDetail3 + ")" : "");

			//Output Values
			strFields[MULTI2MAC_HASH]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_HASH, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strFields[MULTI2MAC_DETAIL]	= strDetails;
			strFields[MULTI2MAC_TYPE]		= getDetails(idArtifact, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
			strFields[MULTI2MAC_LOG]		= "ief-" + getShort(idArtifact, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
			strFields[MULTI2MAC_FROM]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_FROM, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strFields[MULTI2MAC_TO]			= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_TO, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strFields[MULTI2MAC_SIZE]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_PRIMARY + IEF_SIZE, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
			strFields[MULTI2MAC_ATIME]		= (dtmATime > 0 ? boost_lexical_cast_wrapper<string>(dtmATime) : "");
			strFields[MULTI2MAC_MTIME]		= (dtmMTime > 0 ? boost_lexical_cast_wrapper<string>(dtmMTime) : "");
			strFields[MULTI2MAC_CTIME]		= (dtmCTime > 0 ? boost_lexical_cast_wrapper<string>(dtmCTime) : "");
			strFields[MULTI2MAC_BTIME]		= (dtmBTime > 0 ? boost_lexical_cast_wrapper<string>(dtmBTime) : "");

			//Secondary entries within a single line
			dtmBTime = getIEFTime(idArtifact + IEF_SECONDARY + IEF_BTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
			dtmATime = getIEFTime(idArtifact + IEF_SECONDARY + IEF_ATIME, uiSkew, pTZCalc, &delimText, &delimHeader);
			dtmMTime = getIEFTime(idArtifact + IEF_SECONDARY + IEF_MTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
			dtmCTime = getIEFTime(idArtifact + IEF_SECONDARY + IEF_CTIME, uiSkew, pTZCalc, &delimText, &delimHeader);
			if (dtmBTime > 0 || dtmATime > 0 || dtmMTime > 0 || dtmCTime > 0) {
				strDetails = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY+ IEF_DETAIL, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strDetail2 = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_DETAIL2, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strDetails += (strDetail2 != "" ? " (" + strDetail2 + ")" : "");
				strDetail3 = delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_DETAIL3, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strDetails += (strDetail3 != "" ? " (" + strDetail3 + ")" : "");

				strSecondary[MULTI2MAC_HASH]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_HASH, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strSecondary[MULTI2MAC_DETAIL]	= strDetails;
				strSecondary[MULTI2MAC_TYPE]		= getDetails(idArtifact, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
				strSecondary[MULTI2MAC_LOG]		= "ief-" + getShort(idArtifact, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
				strSecondary[MULTI2MAC_FROM]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_FROM, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strSecondary[MULTI2MAC_TO]			= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_TO, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strSecondary[MULTI2MAC_SIZE]		= delimText.getValue(delimHeader.getColumnByValue(getMessage(idArtifact + IEF_SECONDARY + IEF_SIZE, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
				strSecondary[MULTI2MAC_ATIME]		= (dtmATime > 0 ? boost_lexical_cast_wrapper<string>(dtmATime) : "");
				strSecondary[MULTI2MAC_MTIME]		= (dtmMTime > 0 ? boost_lexical_cast_wrapper<string>(dtmMTime) : "");
				strSecondary[MULTI2MAC_CTIME]		= (dtmCTime > 0 ? boost_lexical_cast_wrapper<string>(dtmCTime) : "");
				strSecondary[MULTI2MAC_BTIME]		= (dtmBTime > 0 ? boost_lexical_cast_wrapper<string>(dtmBTime) : "");
			}
		} else {
			WARNING(*pstrFilename << ": processIEF() No valid time values found (" << *pstrData << ")");
		}
	} else {
		ERROR(*pstrFilename << ": processIEF() Unknown artifact (" << *pstrFilename << ")");
	}
}

int32_t getIEFTime(u_int32_t idArtifactFieldCode, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, delimTextRow* p_delimText, delimTextRow* p_delimHeader) {
	int32_t dtmTime = 0;

	DEBUG("getIEFTime() idArtifactFieldCode = " << idArtifactFieldCode << "(" << (idArtifactFieldCode & IEF_ARTIFACT_MASK) << ")");
	string strTime = p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifactFieldCode, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
	if (!strTime.empty()) {
		if ((idArtifactFieldCode & IEF_ARTIFACT_MASK) == IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY) {
			DEBUG("getIEFTime() This should only get called while processing IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY.");
			dtmTime = getUnix32DateTimeFromString2(strTime, ' ', '-', ':', uiSkew, pTZCalc);
		} else {
			dtmTime = getUnix32DateTimeFromString(strTime, ' ', '/', ':', uiSkew, pTZCalc);
		}
		if (dtmTime <= 0) {
			ERROR("getIEFTime() Failed to convert non-empty string to time value (" << strTime << ")");
			dtmTime = -1;
		}
	} else {
		DEBUG("getIEFTime() strTime empty (" << p_delimText->getData() << ")");
	}

	return dtmTime;
}

