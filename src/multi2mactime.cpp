// Copyright 2007 Matthew A. Kucenski
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

#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iostream>
using namespace std;

#include "popt.h"

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libtimeUtils/src/timeUtils.h"
#include "libdelimText/src/textFile.h"
#include "libdelimText/src/textUtils.h"
#include "libdelimText/src/delimTextRow.h"
#include "misc/debugMsgs.h"
#include "misc/poptUtils.h"
#include "misc/tsk_mactime.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

//Sleuthkit TSK3.x body format
//0  |1        |2    |3     |4       |5       |6   |7    |8    |9    |10
//MD5|NAME     |INODE|PERMS |UID     |GID     |SIZE|ATIME|MTIME|CTIME|CRTIME

#define LOG2MACTIME_HASH	TSK3_MACTIME_MD5
#define LOG2MACTIME_DETAIL	TSK3_MACTIME_NAME
#define LOG2MACTIME_TYPE	TSK3_MACTIME_INODE
#define LOG2MACTIME_LOG		TSK3_MACTIME_PERMS
#define LOG2MACTIME_FROM	TSK3_MACTIME_UID
#define LOG2MACTIME_TO		TSK3_MACTIME_GID
#define LOG2MACTIME_SIZE	TSK3_MACTIME_SIZE
#define LOG2MACTIME_ATIME	TSK3_MACTIME_ATIME
#define LOG2MACTIME_MTIME	TSK3_MACTIME_MTIME
#define LOG2MACTIME_CTIME	TSK3_MACTIME_CTIME
#define LOG2MACTIME_CRTIME	TSK3_MACTIME_CRTIME

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

void processHirsch(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processHirsch()");
	//"	   Host Date/Time BETWEEN '2017-08-23 00:00:00' AND '2017-08-25 23:59:59'   ","<SITE>","All Events Log By Date","Print Time:","8/30/2017","12:07:07PM","Printed by:","<USER>","Sequence ID","Host Date/Time","Controller Date/Time","Description","Event ID","Address",285061,"8/25/2017  11:59:59PM","8/26/2017  12:00:00AM","Updating temporary users",8010,"\\XNET.001.0004.001.01","Page -1 of 1"
	//"	   Host Date/Time BETWEEN '2017-08-23 00:00:00' AND '2017-08-25 23:59:59'   "
	
	//"<SITE>"							0
	//"All Events Log By Date"		1
	//"Print Time:"					2
	//"8/30/2017"						3
	//"12:07:07PM"						4
	//"Printed by:"					5
	//"<USER>"							6
	//"Sequence ID"					7
	//"Host Date/Time"				8
	//"Controller Date/Time"		9
	//"Description"					10
	//"Event ID"						11
	//"Address"							12
	//285061								13
	//"8/25/2017  11:59:59PM"		14
	//"8/26/2017  12:00:00AM"		15
	//"Updating temporary users"	16
	//8010								17
	//"\\XNET.001.0004.001.01"		18
	//"Page -1 of 1"					19
	
	delimTextRow delimText(*pstrData, ',');
	string strDateTime = delimText.getField(14);
	string strDate = findSubString(strDateTime, 0, "", " ");
	string strTime = findSubString(findSubString(findSubString(strDateTime, 0, " ", ""), 0, " ", ""), 0, " ", ""); //time may have up to three leading spaces; dirty way to get rid of them...

	delimTextRow delimDate(strDate, '/');
	delimTextRow delimTime(strTime, ':');

		int32_t timeVal = -1; 
		if (strDate.length() && strTime.length()) {
			DEBUG_INFO(	strDate << " " << strTime << "\n" <<
							" month:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)) <<
							" day:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)) <<
							" year:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)) <<
							" hour:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0)) <<
							" minute:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)) <<
							" second:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)));
			u_int16_t uiMonth = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0));
			u_int16_t uiDay = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1));
			u_int16_t uiYear = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2));
			u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
			u_int16_t uiMin = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1));
			u_int16_t uiSec = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2));

			if (uiYear < 100) {
				uiYear += 2000;
			} //if (uiYear < 100) {

			if (	(1 <= uiMonth && uiMonth <= 12) &&
					(1 <= uiDay && uiDay <= 31) &&
					(1400 <= uiYear && uiYear <= 10000) &&
					(0 <= uiHour && uiHour <= 23) &&
					(0 <= uiMin && uiMin <= 60) &&
					(0 <= uiSec && uiSec <= 60)) {
				local_time::local_date_time ldt = pTZCalc->createLocalTime(uiMonth, uiDay, uiYear, uiHour, uiMin, uiSec) + posix_time::seconds(uiSkew);
				timeVal = getUnix32FromLocalTime(ldt);
			} else {
					  DEBUG_INFO("whoops");
				//ERROR
			} //if (	(1 <= uiMonth && uiMonth <= 12) &&
		}
	
	/* This was never actually completed...
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
	strFields[LOG2MACTIME_DETAIL]		= strURL;
	strFields[LOG2MACTIME_TYPE]		= strService;
	strFields[LOG2MACTIME_LOG]			= "fortg1k5";
	strFields[LOG2MACTIME_FROM]		= strSrc;
	strFields[LOG2MACTIME_TO]			= strDst;
	//strFields[LOG2MACTIME_SIZE]		= 
	strFields[LOG2MACTIME_ATIME]		= strTime;
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
	*/
}

void processFortiGate1K5(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processFortiGate1K5()");
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
	strFields[LOG2MACTIME_DETAIL]		= strURL;
	strFields[LOG2MACTIME_TYPE]		= strService;
	strFields[LOG2MACTIME_LOG]			= "fortg1k5";
	strFields[LOG2MACTIME_FROM]		= strSrc;
	strFields[LOG2MACTIME_TO]			= strDst;
	//strFields[LOG2MACTIME_SIZE]		= 
	strFields[LOG2MACTIME_ATIME]		= strTime;
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

void processSquidW3c(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary) {
	DEBUG_INFO(PACKAGE << ":processSquidW3c()" << "[" << *pstrData << "]");
	// Squid has their own log format, but allow custom log formats; this one
	//	seems loosely based on the W3C specs: https://www.w3.org/TR/WDlogfile.html
	// With the exception of [logfile:] and [date/time;], the rest of the fields
	//	are space-delimited.
	//
	// squid.log-20150303.gz:
	//		1425312106.781;			//Date/Time (Unix Epoch)
	//		time_taken=132				//Time taken for transaction to complete in seconds
	//		dns=-							//DNS name
	//		c_ip=w20.x61.y38.z4		//Client IP address
	//		cs_ip=192.12.184.10		//Client-to-Server IP address
	//		r_ip=w08.x4.y2.z0			//Remote IP address
	//		r_port=80					//Remote port
	//		c_port=50034				//Client port
	//		s_action=TCP_MISS
	//		sc_status=200				//Server-to-Client status code
	//		l_err=-
	//		sc_bytes=61761				//Server-to-Client bytes transferred
	//		cs_method=GET				//Client-to-Server method
	//		c_uri="http://www..."	//Client URL
	//		content_type=image/png	//MIME type
	//		referer="http://www..."	//Referer
	//		user_agent="Mozilla..."	//Client user agent

	//TODO - This is a problem... some of these logs files come with the log file included in the data; others do not. Need a more robust way to handle both options.
	// string strTime = 		findSubString(*pstrData, 0, "", ".");
	string strTime = 		findSubString(*pstrData, 0, "", ".");
	DEBUG_INFO(strTime);
	
	string strSrc = 		findSubString(*pstrData, 0, " c_ip=", " ") +
								"/" + findSubString(*pstrData, 0, " cs_ip=", " ") + ":" +
								findSubString(*pstrData, 0, " c_port=", " ");

	string strDst =		findSubString(*pstrData, 0, " r_ip=", " ") + ":" +
								findSubString(*pstrData, 0, " r_port=", " ");

	string strBytes = 	findSubString(*pstrData, 0, " sc_bytes=", " ");

	string strMethod =	findSubString(*pstrData, 0, " cs_method=", " ");
	string strURI = 		findSubString(*pstrData, 0, " c_uri=", " "); 
	string strName = 		strMethod + " " + strURI;

	if (bNormalize) {
		strMethod = stripQualifiers(strMethod, '"');
		strURI = stripQualifiers(strURI, '"');
		strName = addQualifiers(strMethod + " " + strURI, '"');
	}

	//Output Values
	strFields[LOG2MACTIME_DETAIL]		= strName;
	//strFields[LOG2MACTIME_TYPE]		= strService;
	strFields[LOG2MACTIME_LOG]			= "squidw3c";
	//strFields[LOG2MACTIME_FROM]		= strSrc;
	//strFields[LOG2MACTIME_TO]		= strDst;
	strFields[LOG2MACTIME_SIZE]		= strBytes;
	strFields[LOG2MACTIME_ATIME]		= strTime;
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
	
	//Find and passback the referal URL as a second entry for the timeline.
	string strURI2 = 		findSubString(*pstrData, 0, " referer=", " ");
	// Check to make sure referal URL is valid/useful before adding it to the timeline.
	if (strURI2 != "\"-\"") {
		string strName2 = 	"REFERER " + strURI2;

		if (bNormalize) {
			strURI2 = stripQualifiers(strURI2, '"');
			strName2 = addQualifiers("REFERER " + strURI2, '"');
		}

		//Output Values
		strSecondary[LOG2MACTIME_DETAIL]	= strName2;
		//strSecondary[LOG2MACTIME_TYPE]	= strService;
		strSecondary[LOG2MACTIME_LOG]		= "squidw3c";
		//strSecondary[LOG2MACTIME_FROM]	= strSrc;
		//strSecondary[LOG2MACTIME_TO]		= strDst;
		strSecondary[LOG2MACTIME_SIZE]		= strBytes;
		strSecondary[LOG2MACTIME_ATIME]		= strTime;
		//strSecondary[LOG2MACTIME_MTIME]	= 
		//strSecondary[LOG2MACTIME_CTIME]	= 
		//strSecondary[LOG2MACTIME_CRTIME]	= 
	}
}

void processCustomVPN_S1(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomVPN_S1()");

	//Date,Time,user,src_ip,dest_ip
	//2/12/17,15:59:31
	delimTextRow delimText(*pstrData, ',');
	string strDate = delimText.getField(0);
	if (strDate != "Date") {
		string strTime = delimText.getField(1);
		delimTextRow delimDate(strDate, '/');
		delimTextRow delimTime(strTime, ':');

		int32_t timeVal = -1; 
		if (strDate.length() && strTime.length()) {
			DEBUG_INFO(	strDate << " " << strTime << "\n" <<
							" month:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)) <<
							" day:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)) <<
							" year:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)) <<
							" hour:"		<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0)) <<
							" minute:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)) <<
							" second:"	<< boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)));
			u_int16_t uiMonth = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0));
			u_int16_t uiDay = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1));
			u_int16_t uiYear = boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2));
			u_int16_t uiHour = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(0));
			u_int16_t uiMin = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1));
			u_int16_t uiSec = boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2));

			if (uiYear < 100) {
				uiYear += 2000;
			} //if (uiYear < 100) {

			if (	(1 <= uiMonth && uiMonth <= 12) &&
					(1 <= uiDay && uiDay <= 31) &&
					(1400 <= uiYear && uiYear <= 10000) &&
					(0 <= uiHour && uiHour <= 23) &&
					(0 <= uiMin && uiMin <= 60) &&
					(0 <= uiSec && uiSec <= 60)) {
				local_time::local_date_time ldt = pTZCalc->createLocalTime(uiMonth, uiDay, uiYear, uiHour, uiMin, uiSec) + posix_time::seconds(uiSkew);
				timeVal = getUnix32FromLocalTime(ldt);
			} else {
					  DEBUG_INFO("whoops");
				//ERROR
			} //if (	(1 <= uiMonth && uiMonth <= 12) &&
		}
	
		//Output Values
		strFields[LOG2MACTIME_DETAIL]		= delimText.getField(2);	//username
		//strFields[LOG2MACTIME_TYPE]		= strService;
		strFields[LOG2MACTIME_LOG]			= "-----vpn";
		strFields[LOG2MACTIME_FROM]		= delimText.getField(3);	//src_ip
		strFields[LOG2MACTIME_TO]			= delimText.getField(4);	//dst_ip
		//strFields[LOG2MACTIME_SIZE]		= 
		strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
		//strFields[LOG2MACTIME_MTIME]	= 
		//strFields[LOG2MACTIME_CTIME]	= 
		//strFields[LOG2MACTIME_CRTIME]	= 

	} //if (strDate != "Date") {
}

void processCustomFSEM(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomFSEM()");

	//Date (UTC)				IP							ed2k Hash									Filename
	// 8/1/2016 8:11:17 PM	107.77.172.33 :8004	54972296B9DD3CFE194FD3328827DD42	
	delimTextRow delimText(*pstrData, '\t');
	string strDateTime = delimText.getField(0);
	delimTextRow delimDateTime(strDateTime, ' ');
	delimTextRow delimDate(delimDateTime.getField(0), '/');
	delimTextRow delimTime(delimDateTime.getField(1), ':');

	int32_t timeVal = -1; 
	if (strDateTime.length()) {
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
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		timeVal = getUnix32FromLocalTime(ldt);
	}

	string strIPPort = delimText.getField(1);
	string strIP = findSubString(strIPPort, 0, "", " ");
	string strPort = findSubString(strIPPort, 0, " :", "");

	//Output Values
	strFields[LOG2MACTIME_DETAIL]		= delimText.getField(2);
	//strFields[LOG2MACTIME_TYPE]		= strService;
	strFields[LOG2MACTIME_LOG]			= "---emule";
	strFields[LOG2MACTIME_FROM]		= strIP;
	//strFields[LOG2MACTIME_TO]		= strDst;
	//strFields[LOG2MACTIME_SIZE]		= 
	strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

void processCustomFSBT(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processCustomFSBT()");

	//Date (UTC)	IP		Infohash		Severity		No. Of FOI (tab-separated, copy/paste from website)
	//"8/1/2017  8:06:25 AM"

	delimTextRow delimText(*pstrData, '\t');

	string strDateTime = delimText.getField(0);
	int32_t timeVal = -1; 
	if (strDateTime.length()) {
		delimTextRow delimDateTime(strDateTime, ' ');
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
		local_time::local_date_time ldt = pTZCalc->createLocalTime(	boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(0)),	//month
							 															boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(1)),	//day
																						boost_lexical_cast_wrapper<u_int16_t>(delimDate.getField(2)),	//year
																						uiHour, 																			//hour
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(1)),	//minute
																						boost_lexical_cast_wrapper<u_int16_t>(delimTime.getField(2)))	//second
																						+ posix_time::seconds(uiSkew);
		timeVal = getUnix32FromLocalTime(ldt);
	}

	string strIPPort = delimText.getField(1);
	string strIP = findSubString(strIPPort, 0, "", " ");
	string strPort = findSubString(strIPPort, 0, " :", "");

	//Output Values
	strFields[LOG2MACTIME_DETAIL]		= delimText.getField(2);
	//strFields[LOG2MACTIME_TYPE]		= strService;
	strFields[LOG2MACTIME_LOG]			= "bittorre";
	strFields[LOG2MACTIME_FROM]		= strIP;
	//strFields[LOG2MACTIME_TO]		= strDst;
	strFields[LOG2MACTIME_SIZE]		= delimText.getField(4);
	strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

void processSymantec(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	DEBUG_INFO(PACKAGE << ":processSymantec()");

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
	strFields[LOG2MACTIME_DETAIL]		= strMsg;
	strFields[LOG2MACTIME_TYPE]		= strMsgType;
	strFields[LOG2MACTIME_LOG]			= "symantec";
	strFields[LOG2MACTIME_FROM]		= strSrc;
	strFields[LOG2MACTIME_TO]			= strDst;
	strFields[LOG2MACTIME_SIZE]		= strBytes;
	strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

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
	strFields[LOG2MACTIME_DETAIL]		= strMsg;
	strFields[LOG2MACTIME_TYPE]		= strMsgType + ":" + strService;
	strFields[LOG2MACTIME_LOG]			= "-juniper";
	strFields[LOG2MACTIME_FROM]		= strSrc;
	strFields[LOG2MACTIME_TO]			= strDst;
	strFields[LOG2MACTIME_SIZE]		= strBytes;
	strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

void processPIX(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields) {
	int32_t timeVal = -1;
	int32_t uiPIXPos = pstrData->find("%PIX", 16);
	if (uiPIXPos > 0) {
		string strTime = string(*pstrData, uiPIXPos - 22, 20);  //Find "%PIX" and then backup 22 characters to get the PIX generated time, not the receiving syslog time
		if (strTime.length()) {
			local_time::local_date_time ldt = pTZCalc->createLocalTime(strTime, "%b %d %Y %H:%M:%S") + posix_time::seconds(uiSkew);
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
	strFields[LOG2MACTIME_DETAIL]		= strMsg;
	strFields[LOG2MACTIME_TYPE]		= strMsgType + ":" + strService;
	strFields[LOG2MACTIME_LOG]			= "-----pix";
	strFields[LOG2MACTIME_FROM]		= strSrc;
	strFields[LOG2MACTIME_TO]			= strDst;
	strFields[LOG2MACTIME_SIZE]		= strBytes;
	strFields[LOG2MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	//strFields[LOG2MACTIME_MTIME]	= 
	//strFields[LOG2MACTIME_CTIME]	= 
	//strFields[LOG2MACTIME_CRTIME]	= 
}

int main(int argc, const char** argv) {
	int rv = EXIT_FAILURE;

	textFile txtFileObj;
	vector<string> filenameVector;
	string strType = "";
	string strCustom1 = "";
	string strCustom2 = "";
	u_int16_t uiYear = posix_time::second_clock::local_time().date().year();
	timeZoneCalculator tzcalc;
	u_int32_t uiSkew = 0;
	bool bNormalize = false;
	bool bHTMLDecode = false;

	struct poptOption optionsTable[] = {
		{"type",	't',	POPT_ARG_STRING,	NULL,	10,	"Format for data.", "type"},
		{"year",			'y',	POPT_ARG_INT,		NULL,	20,	"Some logs do not store the year in each entry.  Defaults to the current year.",	"year"},
		{"timezone", 	'z',	POPT_ARG_STRING,	NULL,	30,	"POSIX timezone string (e.g. 'EST-5EDT,M4.1.0,M10.1.0' or 'GMT-5') indicating which zone the logs are using. Defaults to GMT.", "zone"},
		{"skew",			's',	POPT_ARG_INT,		NULL,	40,	"Adjust time values by given seconds.", "seconds"},
		{"normalize",	'n',	POPT_ARG_NONE,		NULL,	50,	"Attempt to clean/normalize input data based on known issues with various types of data. Use w/CAUTION and check stderr for results!"},
		//{"html-decode",'h',	POPT_ARG_NONE,		NULL, 60, 	"Execute multipass decoding of HTML encoded strings. Provides easier readability of URLs w/in URLs."},
		{"custom1",		 0,	POPT_ARG_STRING,	NULL,	70,	"Custom value applicable to certain types of data.", "custom1"},
		{"custom2",		 0,	POPT_ARG_STRING,	NULL,	80,	"Custom value applicable to certain types of data.", "custom2"},
		{"version",		 0,	POPT_ARG_NONE,		NULL,	100,	"Display version.", NULL},
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	poptContext optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
	poptSetOtherOptionHelp(optCon, "[options] <filename> [<filename>] ...");

	if (argc < 1) {
		poptPrintUsage(optCon, stderr, 0);
		exit(EXIT_FAILURE);
	}

	string strTmp;
	int iOption = poptGetNextOpt(optCon);
	while (iOption >= 0) {
		switch (iOption) {
			case 10:
				strType = poptGetOptArg(optCon);
				break;
			case 20:
				uiYear = strtol(poptGetOptArg(optCon), NULL, 10);
				break;
			case 30:
				if (tzcalc.setTimeZone(poptGetOptArg(optCon)) >= 0) {
				} else {
					usage(optCon, "Invalid time zone string", "e.g. 'EST-5EDT,M4.1.0,M10.1.0' or 'GMT-5'");
					exit(EXIT_FAILURE);
				}
				break;
			case 40:
				uiSkew = strtol(poptGetOptArg(optCon), NULL, 10);
				break;
			case 50:
				bNormalize = true;
				break;
			case 60:
				bHTMLDecode = true;
				break;
			case 70:
				strCustom1 = poptGetOptArg(optCon);
				break;
			case 80:
				strCustom2 = poptGetOptArg(optCon);
				break;
			case 100:
				version(PACKAGE, VERSION);
				exit(EXIT_SUCCESS);
				break;
		}
		iOption = poptGetNextOpt(optCon);
	}

	if (iOption != -1) {
		usage(optCon, poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(iOption));
		exit(EXIT_FAILURE);
	}
	
	const char* cstrFilename = poptGetArg(optCon);
	while (cstrFilename) {
		filenameVector.push_back(cstrFilename);
		cstrFilename = poptGetArg(optCon);
	}
	
	if (filenameVector.size() < 1) {
		filenameVector.push_back("");		//If no files are given, an empty filename will cause libDelimText::textFile to read from stdin
	}

	for (vector<string>::iterator it = filenameVector.begin(); it != filenameVector.end(); it++) {
		if (txtFileObj.open(*it)) {

			string strData;
			string strFields[11];
			string strSecondary[11];

			string strHeader = txtFileObj.getFirstRow();

			while (txtFileObj.getNextRow(&strData)) {
				if (strType == "squidw3c") {
	 				processSquidW3c(&strData, uiYear, uiSkew, bNormalize, &tzcalc, strFields, strSecondary);
				} else if (strType == "symantec") {
	 				processSymantec(&strData, uiYear, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "ipfw") {
					strFields[LOG2MACTIME_LOG] = "----ipfw";
					strFields[LOG2MACTIME_DETAIL] = "Not Yet Implemented";
				} else if (strType == "pf") {
					strFields[LOG2MACTIME_LOG] = "------pf";
					strFields[LOG2MACTIME_DETAIL] = "Not Yet Implemented";
				} else if (strType == "pix") {
					processPIX(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "juniper") {
					processJuniper(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "custfsbt") {
					processCustomFSBT(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "custfsem") {
					processCustomFSEM(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "cusvpns1") {
					processCustomVPN_S1(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "fortg1k5") {
					processFortiGate1K5(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "hirsch") {
					processHirsch(&strData, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "griffeye") {
					processGriffeyeCSV(&strData, &strHeader, uiSkew, bNormalize, &tzcalc, strFields);
				} else {
					strFields[LOG2MACTIME_LOG] = "-unknown";
					strFields[LOG2MACTIME_DETAIL] = "Unknown Type";
				}

				// Output final mactime format
				cout 					<< strFields[LOG2MACTIME_HASH]	<< "|"
										<< strFields[LOG2MACTIME_DETAIL]	<< "|"
										<< strFields[LOG2MACTIME_TYPE]	<< "|"
						<< "log-"	<< strFields[LOG2MACTIME_LOG]		<< "|"
										<< strFields[LOG2MACTIME_FROM]	<< "|"
										<< strFields[LOG2MACTIME_TO]		<< "|"
										<< strFields[LOG2MACTIME_SIZE]	<< "|"
										<< strFields[LOG2MACTIME_ATIME]	<< "|"
										<< strFields[LOG2MACTIME_MTIME]	<< "|"
										<< strFields[LOG2MACTIME_CTIME]	<< "|"
										<< strFields[LOG2MACTIME_CRTIME]	<< "\n";

				// If secondary records created, output them in mactime format also
				if (strSecondary[LOG2MACTIME_DETAIL] != "") {
					cout 					<< strSecondary[LOG2MACTIME_HASH]		<< "|"
											<< strSecondary[LOG2MACTIME_DETAIL]	<< "|"
											<< strSecondary[LOG2MACTIME_TYPE]		<< "|"
							<< "log-"	<< strSecondary[LOG2MACTIME_LOG]		<< "|"
											<< strSecondary[LOG2MACTIME_FROM]		<< "|"
											<< strSecondary[LOG2MACTIME_TO]			<< "|"
											<< strSecondary[LOG2MACTIME_SIZE]		<< "|"
											<< strSecondary[LOG2MACTIME_ATIME]		<< "|"
											<< strSecondary[LOG2MACTIME_MTIME]		<< "|"
											<< strSecondary[LOG2MACTIME_CTIME]		<< "|"
											<< strSecondary[LOG2MACTIME_CRTIME]	<< "\n";
				} //if (strSecondary[LOG2MACTIME_DETAIL] != "") {
			}
		} else {
		}
	}	//for (vector<string>::iterator it = arguments.filenameVector.begin(); it != arguments.filenameVector.end(); it++) {

	exit(rv);	
}	//int main(int argc, const char** argv) {

