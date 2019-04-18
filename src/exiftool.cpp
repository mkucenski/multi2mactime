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

void processExifTool(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary) {
	DEBUG("processExifTool(Data) " + *pstrData);
	DEBUG("processExifTool(Header) " + *pstrHeader);

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');

	//TODO exiftool generates a *lot* of different header values depending on the file type; this represents a quick/dirty first pass.
	//			It really probably needs to be written similar to IEF, but requires more research/time than I have right now.
	//
	//	RELEVANT HEADERS:
	//		Creator
	//		CreatorTool
	//		Description
	//		Subject
	//		Title
	//
	//		FileName
	//		Author
	//		Company
	//		FileSize
	//
	//		(M) ModifyDate
	//		(A)
	//		(C) MetadataDate
	//		(B) CreateDate (PDF CreationDate)
	//
	//TODO not ready to handle this...
	//		(M) SourceModified (PDF-ism in the format "D:...")
	//		(A) LastSaved (Another PDF-ism in the format "D:...")
	//		(C) Date (Seems to line up w/MetadataDate [at least in this one case that I'm seeing this field])
	//		(B) Created (PDF-ism in the format "D:...")
	//

	//0		|1						|2				|3				|4			|5				|6				|7				|8						|9					|10
	//Sleuthkit TSK3.x body format
	//MD5		|NAME					|INODE		|PERMS		|UID		|GID			|SIZE			|ATIME		|MTIME				|CTIME			|CRTIME
	//multi2mactime
	//HASH	|DETAIL				|TYPE			|LOG-SRC		|FROM		|TO			|SIZE			|ATIME		|MTIME				|CTIME			|CRTIME
	//exiftool (this file)
	//			|.................|"FileName"	|'exiftool'	|"Author"|"Company"	|				|				|"ModifyDate"		|"MetadataDate"|"CreateDate"
	//			|.................|"FileName"	|'exiftool'	|"Author"|"Company"	|				|"LastSaved"|"SourceModified"	|"Date"			|"Created"

	string strCreator 		= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Creator")), '"');
	string strCreatorTool 	= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("CreatorTool")), '"');
	string strDescription 	= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Description")), '"');
	string strSubject 		= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Subject")), '"');
	string strTitle 			= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Title")), '"');

	string strDetails;
	strDetails += (strTitle != "" ? "[" + strTitle + "] " : "");
	strDetails += (strSubject != "" ? "[" + strSubject + "] " : "");
	strDetails += (strDescription != "" ? "[" + strDescription + "] " : "");
	strDetails += (strCreator != "" ? "[" + strCreator + "] " : "");
	strDetails += (strCreatorTool != "" ? "[" + strCreatorTool + "] " : "");

	string strModified = delimText.getValue(delimHeader.getColumnByValue("ModifyDate"));
	string strChanged = delimText.getValue(delimHeader.getColumnByValue("MetadataDate"));
	string strBirthed = delimText.getValue(delimHeader.getColumnByValue("CreateDate"));
	int32_t mTimeVal = getUnix32DateTimeFromString2(strModified, ' ', ':', ':', uiSkew, pTZCalc);
	int32_t cTimeVal = getUnix32DateTimeFromString2(strChanged, ' ', ':', ':', uiSkew, pTZCalc);
	int32_t bTimeVal = getUnix32DateTimeFromString2(strBirthed, ' ', ':', ':', uiSkew, pTZCalc);

	//Output Values
	//strFields[MULTI2MAC_HASH]		=
	strFields[MULTI2MAC_DETAIL]		= strDetails;
	strFields[MULTI2MAC_TYPE]			= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("FileName")), '"');
	strFields[MULTI2MAC_LOG]			= "exiftool";
	strFields[MULTI2MAC_FROM]			= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Author")), '"');
	strFields[MULTI2MAC_TO]				= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("Company")), '"');
	//strFields[MULTI2MAC_SIZE]		= stripQualifiers(delimText.getValue(delimHeader.getColumnByValue("FileSize")), '"');
	//strFields[MULTI2MAC_ATIME]		=
	strFields[MULTI2MAC_MTIME]			= (mTimeVal > 0 ? boost_lexical_cast_wrapper<string>(mTimeVal) : "");
	strFields[MULTI2MAC_CTIME]			= (cTimeVal > 0 ? boost_lexical_cast_wrapper<string>(cTimeVal) : "");
	strFields[MULTI2MAC_BTIME]			= (bTimeVal > 0 ? boost_lexical_cast_wrapper<string>(bTimeVal) : "");
}

