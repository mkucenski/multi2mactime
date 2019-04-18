// Copyright 2019 Matthew A. Kucenski
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

void processNotes(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG("processNotes(Data) " + *pstrData);
	DEBUG("processNotes(Header) " + *pstrHeader);

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');
	// Header Format/Fields: "Date/Time,Artifact,Details,Source,From,To,Notes"

	// Rob Lee (SANS Instructor) uses a four-column format for writing his timeline notes:
	//		1. Date/time
	//		2. Artifact (message, email, url, log, etc.)
	//		3. What happened? (who, what, when, where, why, how, etc.?)
	//		4. Source
	//
	// I'm going to add a few fields:
	//		5. From (email address, phone number, ip address, etc.)
	//		6. To (email address, phone number, ip address, etc.)
	//		7. Notes (analyst notes, opinions, theories, etc. about what this entry means)

	//0		|1						|2				|3			|4			|5		|6		|7		|8			|9			|10
	//Sleuthkit TSK3.x body format
	//MD5		|NAME					|INODE		|PERMS	|UID		|GID	|SIZE	|ATIME|MTIME	|CTIME	|CRTIME
	//multi2mactime
	//HASH	|DETAIL				|TYPE			|LOG-SRC	|FROM		|TO	|SIZE	|ATIME|MTIME	|CTIME	|CRTIME
	//notes (this file)
	//			|"Details"/"Notes"|"Artifact"	|"Source"|"From"	|"To"	|		|		|			|			|"Date/Time"
	

	string strBirthed = delimText.getValue(delimHeader.getColumnByValue("Date/Time"));
	int32_t bTimeVal = getUnix32DateTimeFromString(strBirthed, ' ', '/', ':', uiSkew, pTZCalc);

	string strDetails = stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Details")), '"');
	string strNotes 	= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Notes")), '"');
	strDetails += (strNotes != "" ? " [" + strNotes + "]" : "");

	//Output Values
	//strFields[MULTI2MAC_HASH]	=
	strFields[MULTI2MAC_DETAIL]	= strDetails;
	strFields[MULTI2MAC_TYPE]		= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Artifact")), '"');
	strFields[MULTI2MAC_LOG]		= "notes-" + stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Source")), '"');
	strFields[MULTI2MAC_FROM]		= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("From")), '"');
	strFields[MULTI2MAC_TO]			= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("To")), '"');
	//strFields[MULTI2MAC_SIZE]	=
	//strFields[MULTI2MAC_ATIME]	=
	//strFields[MULTI2MAC_MTIME]	=
	//strFields[MULTI2MAC_CTIME]	=
	strFields[MULTI2MAC_BTIME]		= (bTimeVal > 0 ? boost_lexical_cast_wrapper<string>(bTimeVal) : "");
}

