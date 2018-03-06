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
#include "iefTypes.h"

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libtimeUtils/src/timeUtils.h"
#include "libdelimText/src/textUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

// TODO
// * RESOLVED Seems to be errors in LNK files and PDF files with finding valid dates that appear to be present.
// 	* Related to some entries having primary and some having secondary; only really an issue if it has neither.
// * The specific processors need to be made more consistent as follows:
// 	* Decisions related to what is or is not displayed should be left to main() for consistency across types.
// 	* Just pull whatever data you can find and return it.
// 	* For IEF, primary/secondary processing are identical; they should share code.
//		* WARNINGS should really not be reported here; only in main() -- ERROR only?

int32_t getIEFTime(string strTime, u_int32_t idArtifact, u_int32_t uiSkew, timeZoneCalculator* pTZCalc); 
bool getIEFFields(delimTextRow* p_delimText, delimTextRow* p_delimHeader, u_int32_t idArtifact, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields);

void processIEF(string* pstrData, string* pstrHeader, string* pstrFilename, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary) {
	DEBUG(*pstrFilename << ": processIEF(pstrData = '" << *pstrData << "')");

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');

	// TODO	This is far too quick and dirty...
	// 		Strip the filename of .xlsx and/or .csv to get down to just the overall artifact name
	string strFilename = ieraseSubString(*pstrFilename, ".xlsx");
	strFilename = ieraseSubString(strFilename, ".csv");
	
	u_int32_t idArtifact = getCode(strFilename, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
	if (idArtifact > 0) {
		getIEFFields(&delimText, &delimHeader, idArtifact + IEF_PRIMARY, uiSkew, pTZCalc, strFields);
		getIEFFields(&delimText, &delimHeader, idArtifact + IEF_SECONDARY, uiSkew, pTZCalc, strSecondary);
	} else {
		ERROR(*pstrFilename << ": processIEF() Unknown artifact (" << *pstrFilename << ")");
	}
}

int32_t getIEFTime(string strTime, u_int32_t idArtifact, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
	int32_t dtmTime = -1;

	if (!strTime.empty()) {
		if ((idArtifact & IEF_ARTIFACT_MASK)  == IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY) {
			dtmTime = getUnix32DateTimeFromString2(strTime, ' ', '-', ':', uiSkew, pTZCalc);
		} else {
			dtmTime = getUnix32DateTimeFromString(strTime, ' ', '/', ':', uiSkew, pTZCalc);
		}
		if (dtmTime <= 0) {
			ERROR("getIEFTime() Failed to convert non-empty string to time value (" << strTime << ")");
		}
	}

	return dtmTime;
}

bool getIEFFields(delimTextRow* p_delimText, delimTextRow* p_delimHeader, u_int32_t idArtifact, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields) {
	bool rv = false;

	if (p_delimText != NULL && p_delimHeader != NULL && pTZCalc != NULL && strFields != NULL) {
		int iBTimeColumn = p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_BTIME, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)));
		int iATimeColumn = p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_ATIME, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)));
		int iMTimeColumn = p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_MTIME, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)));
		int iCTimeColumn = p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_CTIME, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)));

		string strBTime = p_delimText->getValue(iBTimeColumn);
		string strATime = p_delimText->getValue(iATimeColumn);
		string strMTime = p_delimText->getValue(iMTimeColumn);
		string strCTime = p_delimText->getValue(iCTimeColumn);

		DEBUG("getIEFFields(): " << 	(idArtifact & IEF_PRIMARY_MASK == IEF_PRIMARY ? "PRIMARY: " : (idArtifact & IEF_PRIMARY_MASK == IEF_SECONDARY ? "SECONDARY: " : "TERTIARY: ")) <<	
							 					"strBTime(" << strBTime << ")(" << iBTimeColumn << ") " <<
							 					"strATime(" << strATime << ")(" << iATimeColumn << ") " <<
												"strMTime(" << strMTime << ")(" << iMTimeColumn << ") " <<
												"strCTime(" << strCTime << ")(" << iCTimeColumn << ")");

		int32_t dtmBTime = getIEFTime(strBTime, idArtifact, uiSkew, pTZCalc);
		int32_t dtmATime = getIEFTime(strATime, idArtifact, uiSkew, pTZCalc);
		int32_t dtmMTime = getIEFTime(strMTime, idArtifact, uiSkew, pTZCalc);
		int32_t dtmCTime = getIEFTime(strCTime, idArtifact, uiSkew, pTZCalc);
		
		string strDetails = stripQualifiers(p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_DETAIL, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)))), '"');
		string strDetail2 = stripQualifiers(p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_DETAIL2, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)))), '"');
		strDetails += (strDetail2 != "" ? " (" + strDetail2 + ")" : "");
		string strDetail3 = stripQualifiers(p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_DETAIL3, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS)))), '"');
		strDetails += (strDetail3 != "" ? " (" + strDetail3 + ")" : "");
	
		//Output Values
		strFields[MULTI2MAC_HASH]		= p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_HASH, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
		strFields[MULTI2MAC_DETAIL]	= strDetails;
		strFields[MULTI2MAC_TYPE]		= getDetails(idArtifact & IEF_ARTIFACT_MASK, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
		strFields[MULTI2MAC_LOG]		= "ief-" + getShort(idArtifact & IEF_ARTIFACT_MASK, IEF_ARTIFACTS, sizeof(IEF_ARTIFACTS));
		strFields[MULTI2MAC_FROM]		= p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_FROM, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
		strFields[MULTI2MAC_TO]			= p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_TO, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
		strFields[MULTI2MAC_SIZE]		= p_delimText->getValue(p_delimHeader->getColumnByValue(getMessage(idArtifact + IEF_SIZE, IEF_ARTIFACT_FIELDS, sizeof(IEF_ARTIFACT_FIELDS))));
		strFields[MULTI2MAC_ATIME]		= (dtmATime > 0 ? boost_lexical_cast_wrapper<string>(dtmATime) : "");
		strFields[MULTI2MAC_MTIME]		= (dtmMTime > 0 ? boost_lexical_cast_wrapper<string>(dtmMTime) : "");
		strFields[MULTI2MAC_CTIME]		= (dtmCTime > 0 ? boost_lexical_cast_wrapper<string>(dtmCTime) : "");
		strFields[MULTI2MAC_BTIME]		= (dtmBTime > 0 ? boost_lexical_cast_wrapper<string>(dtmBTime) : "");
	
		rv = true;
	} else {
		ERROR("getIEFFields(): Invalid pointer");
	}

	return rv;
}
