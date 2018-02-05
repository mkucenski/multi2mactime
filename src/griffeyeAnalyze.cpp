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

void processGriffeyeCSV(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processGriffeyeCSV()");
	//1	Binary Copies	3
	//2	Bookmarks	
	//3	Can File Be Opened?	True
	//4	Category	Child Exploitive (non-CAM) / Age Difficult
	//5	Created Date	1/10/2009 8:16:10 PM
	//6	Database Match Conflict	
	//7	DB: Default Database	-1
	//8	DB: NSRL	-1
	//9	DB: PhotoDNA	-1
	//10	DB: ProjectVIC	-1
	//11	DB: ProjectVIC PhotoDNA	2
	//12	Deleted?	True
	//13	Directory Path	"..."
	//14	Distributed	Distributed
	//15	Exif Serial Number	
	//16	Exif: Author	
	//17	Exif: City	
	//18	Exif: Comment	"..."
	//19	Exif: CreateDate	
	//20	Exif: ImageUniqueId	
	//21	Exif: Make	
	//22	Exif: Model	
	//23	Exif: Software	
	//24	Exif: Title	
	//25	File Extension	.jpg
	//26	File Location	...
	//27	File Name	...
	//28	File Size	37590
	//29	File Type	Image
	//30	Has Exif/GPS Informtion	Relevant Exif
	//31	Has Sound	False
	//32	Height	450
	//33	Identified	Not Identified
	//34	Initial Category	2
	//35	Is Animated	False
	//36	Last Accessed	12/29/2009 12:10:18 AM
	//37	Last Write Time	2/4/2008 6:12:15 AM
	//38	Latitude	0
	//39	Length	
	//40	Longitude	0
	//41	MD5	...
	//42	Mime Type	image/jpeg
	//43	Nudity Level	11
	//44	Overwritten?	False
	//45	PhotoDNA	...
	//46	Physical Location	
	//47	Representative	False
	//48	Series Name	US/Pink wall
	//49	SHA-1	...
	//50	SHA-256	...
	//51	SHA-384	...
	//52	SHA-512	...
	//53	Source ID	...
	//54	Tags	#...
	//55	Unallocated	True
	//56	User Comment	
	//57	Visual Copies	6
	//58	Visual Group	25787
	//59	Width	600

	delimTextRow delimText(*pstrData, ',', '"');
	delimTextRow delimHeader(*pstrHeader, ',', '"');

	//5	Created Date	1/10/2009 8:16:10 PM
	//36	Last Accessed	12/29/2009 12:10:18 AM
	//37	Last Write Time	2/4/2008 6:12:15 AM
	string strCreated = delimText.getValue(delimHeader.getColumnByValue("Created Date"));
	string strAccessed = delimText.getValue(delimHeader.getColumnByValue("Last Accessed"));
	string strModified = delimText.getValue(delimHeader.getColumnByValue("Last Write Time"));

	int32_t crTimeVal = -1; 
	if (strCreated.length()) {
		delimTextRow delimDateTime(strCreated, ' ');
		delimTextRow delimDate(delimDateTime.getField(0), '/');
		delimTextRow delimTime(delimDateTime.getField(1), ':');

		u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
		u_int16_t uiHourDbg = uiHour;
		if (delimDateTime.getField(2) == "PM") {
			if (uiHour != 12) {
				uiHour += 12;
			}
		} else {
			if (uiHour == 12) {
				uiHour = 0;
			}
		}
		try {
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		crTimeVal = getUnix32FromLocalTime(ldt);
		} catch (...) {
			return;
		}
	}
	
	int32_t aTimeVal = -1; 
	if (strAccessed.length()) {
		delimTextRow delimDateTime(strAccessed, ' ');
		delimTextRow delimDate(delimDateTime.getField(0), '/');
		delimTextRow delimTime(delimDateTime.getField(1), ':');

		u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
		u_int16_t uiHourDbg = uiHour;
		if (delimDateTime.getField(2) == "PM") {
			if (uiHour != 12) {
				uiHour += 12;
			}
		} else {
			if (uiHour == 12) {
				uiHour = 0;
			}
		}
		try {
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		aTimeVal = getUnix32FromLocalTime(ldt);
		} catch (...) {
			return;
		}
	}

	int32_t mTimeVal = -1; 
	if (strModified.length()) {
		delimTextRow delimDateTime(strModified, ' ');
		delimTextRow delimDate(delimDateTime.getField(0), '/');
		delimTextRow delimTime(delimDateTime.getField(1), ':');

		u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
		u_int16_t uiHourDbg = uiHour;
		if (delimDateTime.getField(2) == "PM") {
			if (uiHour != 12) {
				uiHour += 12;
			}
		} else {
			if (uiHour == 12) {
				uiHour = 0;
			}
		}
		try {
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		mTimeVal = getUnix32FromLocalTime(ldt);
		} catch (...) {
			return;
		}
	}

	//Output Values
	strFields[TSK3_MACTIME_MD5]		= delimText.getValue(delimHeader.getColumnByValue("MD5"));
	strFields[TSK3_MACTIME_NAME]		= delimText.getValue(delimHeader.getColumnByValue("Directory Path")) + "\\" + delimText.getValue(delimHeader.getColumnByValue("File Name"));
	//strFields[TSK3_MACTIME_INODE]		= 
	strFields[TSK3_MACTIME_PERMS]		= "griffeye";
	//strFields[TSK3_MACTIME_UID]		= 
	//strFields[TSK3_MACTIME_GID]		= 
	strFields[TSK3_MACTIME_SIZE]		= delimText.getValue(delimHeader.getColumnByValue("File Size"));
	strFields[TSK3_MACTIME_ATIME]		= (aTimeVal > 0 ? boost_lexical_cast_wrapper<string>(aTimeVal) : "");
	strFields[TSK3_MACTIME_MTIME]		= (mTimeVal > 0 ? boost_lexical_cast_wrapper<string>(mTimeVal) : "");
	//strFields[TSK3_MACTIME_CTIME]		= 
	strFields[TSK3_MACTIME_CRTIME]	= (crTimeVal > 0 ? boost_lexical_cast_wrapper<string>(crTimeVal) : "");
}

