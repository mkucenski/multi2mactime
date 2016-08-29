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
#include "misc/debugMsgs.h"
#include "misc/poptUtils.h"
#include "misc/tsk_mactime.h"
#include "misc/boost_lexical_cast_wrapper.hpp"

//Sleuthkit TSK3.x body format
//0  |1        |2    |3     |4       |5       |6   |7    |8    |9    |10
//MD5|NAME     |INODE|PERMS |UID     |GID     |SIZE|ATIME|MTIME|CTIME|CRTIME
//Sleuthkit TSK2.x body format
//0  |1        |2     |3    |4       |5       |6    |7  |8     |9   |10     |11   |12      |13      |14      |15
//MD5|PATH/NAME|DEVICE|INODE|PERM-VAL|PERM-STR|LINKS|UID|GID   |RDEV|SIZE   |ATIME|MTIME   |CTIME   |BLK-SIZE|BLKS

void processSquidW3c(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields, string* strReferer) {
	DEBUG_INFO(PACKAGE << ":processSquidW3c()");
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

	string strTime = 	findSubString(*pstrData, 0, ":", ".");
	
	string strSrc = 	findSubString(*pstrData, 0, " c_ip=", " ") +
							"/" + findSubString(*pstrData, 0, " cs_ip=", " ") + ":" +
							findSubString(*pstrData, 0, " c_port=", " ");

	string strDst =	findSubString(*pstrData, 0, " r_ip=", " ") + ":" +
							findSubString(*pstrData, 0, " r_port=", " ");

	string strBytes = findSubString(*pstrData, 0, " sc_bytes=", " ");

	string strURL =	findSubString(*pstrData, 0, " cs_method=", " ") + " " +
							findSubString(*pstrData, 0, " c_uri=", " "); 

	strFields[TSK3_MACTIME_NAME]		= strURL;
	strFields[TSK3_MACTIME_SIZE]		= strBytes;
	strFields[TSK3_MACTIME_ATIME]		= strTime;

	//Find and passback the referal URL as a second entry for the timeline.
	string strURL2 = 	"REFERER " + findSubString(*pstrData, 0, " referer=", " ");

	strReferer[TSK3_MACTIME_NAME]		= strURL2;
	strReferer[TSK3_MACTIME_SIZE]		= strBytes;
	strReferer[TSK3_MACTIME_ATIME]	= strTime;
}

void processSymantec(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields) {
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

	//strFields[TSK3_MACTIME_MD5] 		= strService; //MD5 not output in default version of datatime/mactime (TODO where else can this data go?)
	strFields[TSK3_MACTIME_NAME]		= strMsg;
	strFields[TSK3_MACTIME_INODE]		= strMsgType;
	strFields[TSK3_MACTIME_UID]		= strSrc;
	strFields[TSK3_MACTIME_GID]		= strDst;
	strFields[TSK3_MACTIME_SIZE]		= strBytes;
	strFields[TSK3_MACTIME_ATIME]		= (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "");
	strFields[TSK3_MACTIME_MTIME]		= "";
	strFields[TSK3_MACTIME_CTIME]		= "";
	strFields[TSK3_MACTIME_CRTIME]	= "";
}

void processJuniper(string* pstrData, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields) {
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

	cout << "FWL|\"" << strMsg << "\"||" << strMsgType << "||" << strService << "||" << strSrc << "|" << strDst << "|\"" << *pstrData << "\"|" << strBytes << "|||" << (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "") << "||" << endl;
}

void processPIX(string* pstrData, u_int32_t uiSkew, timeZoneCalculator* pTZCalc, string* strFields) {
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
		
	cout << "FWL|\"" << strMsg << "\"||" << strMsgType << "||" << strService << "||" << strSrc << "|" << strDst << "|\"" << *pstrData << "\"|" << strBytes << "|||" << (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "") << "||" << endl;
}

int main(int argc, const char** argv) {
	int rv = EXIT_FAILURE;

	textFile txtFileObj;
	vector<string> filenameVector;
	string strFirewallType = "squidw3c";
	u_int16_t uiYear = posix_time::second_clock::local_time().date().year();
	timeZoneCalculator tzcalc;
	u_int32_t uiSkew = 0;

	struct poptOption optionsTable[] = {
		{"firewall",	'f',	POPT_ARG_STRING,	NULL,	10,	"Format for firewall log(s).", "firewall"},
		{"year",			'y',	POPT_ARG_INT,		NULL,	20,	"Some firewall logs do not store the year in each entry.  Defaults to the current year.",	"year"},
		{"timezone", 	'z',	POPT_ARG_STRING,	NULL,	30,	"POSIX timezone string (e.g. 'EST-5EDT,M4.1.0,M10.1.0' or 'GMT-5') indicating which zone the firewall logs are using. Defaults to GMT.", "zone"},
		{"skew",			 0,		POPT_ARG_INT,		NULL,	40,	"Adjust time values by given seconds.", "seconds"},
		{"version",		 0,		POPT_ARG_NONE,		NULL,	100,	"Display version.", NULL},
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
				strFirewallType = poptGetOptArg(optCon);
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
			string strReferer[11];

			while (txtFileObj.getNextRow(&strData)) {
				if (strFirewallType == "squidw3c") {

					strFields[TSK3_MACTIME_PERMS] = "squidw3c";
					strReferer[TSK3_MACTIME_PERMS] = "squidw3c";
	 				processSquidW3c(&strData, uiYear, uiSkew, &tzcalc, strFields, strReferer);

				} else if (strFirewallType == "symantec") {

					strFields[TSK3_MACTIME_PERMS] = "symantec";
	 				processSymantec(&strData, uiYear, uiSkew, &tzcalc, strFields);

				} else if (strFirewallType == "ipfw") {

					strFields[TSK3_MACTIME_PERMS] = "----ipfw";
					cout << "Not Yet Implemented" << endl;

				} else if (strFirewallType == "pf") {

					strFields[TSK3_MACTIME_PERMS] = "------pf";
					cout << "Not Yet Implemented" << endl;

				} else if (strFirewallType == "pix") {

					strFields[TSK3_MACTIME_PERMS] = "-----pix";
					processPIX(&strData, uiSkew, &tzcalc, strFields);

				} else if (strFirewallType == "juniper") {

					strFields[TSK3_MACTIME_PERMS] = "-juniper";
					processJuniper(&strData, uiSkew, &tzcalc, strFields);

				} else {

					strFields[TSK3_MACTIME_PERMS] = "-unknown";
					cout << "Unknown Firewall Type" << endl;

				}

				//TSK 3.0+: MD5|name|inode|mode_as_string|UID|GID|size|atime|mtime|ctime|crtime
				cout 	<< strFields[TSK3_MACTIME_MD5] << "|"					//0  MD5
						<< strFields[TSK3_MACTIME_NAME] << "|"					//1  name
						<< strFields[TSK3_MACTIME_INODE] << "|"				//2  inode
						<< "fwl-" << strFields[TSK3_MACTIME_PERMS] << "|"	//3  mode_as_string;
						<< strFields[TSK3_MACTIME_UID] << "|"					//4  UID
						<< strFields[TSK3_MACTIME_GID] << "|"					//5  GID
						<< strFields[TSK3_MACTIME_SIZE] << "|" 				//6  size
						<< strFields[TSK3_MACTIME_ATIME] << "|"				//7  atime
						<< strFields[TSK3_MACTIME_MTIME] << "|"				//8  mtime
						<< strFields[TSK3_MACTIME_CTIME] << "|"				//9  ctime
						<< strFields[TSK3_MACTIME_CRTIME]						//10 crtime
						<< "\n";
				if (strReferer[TSK3_MACTIME_NAME] != "") {
					cout 	<< strReferer[TSK3_MACTIME_MD5] << "|"					//0  MD5
							<< strReferer[TSK3_MACTIME_NAME] << "|"				//1  name
							<< strReferer[TSK3_MACTIME_INODE] << "|"				//2  inode
							<< "fwl-" << strFields[TSK3_MACTIME_PERMS] << "|"	//3  mode_as_string;
							<< strReferer[TSK3_MACTIME_UID] << "|"					//4  UID
							<< strReferer[TSK3_MACTIME_GID] << "|"					//5  GID
							<< strReferer[TSK3_MACTIME_SIZE] << "|" 				//6  size
							<< strReferer[TSK3_MACTIME_ATIME] << "|"				//7  atime
							<< strReferer[TSK3_MACTIME_MTIME] << "|"				//8  mtime
							<< strReferer[TSK3_MACTIME_CTIME] << "|"				//9  ctime
							<< strReferer[TSK3_MACTIME_CRTIME]						//10 crtime
							<< "\n";
				}
			}
		} else {
		}
	}	//for (vector<string>::iterator it = arguments.filenameVector.begin(); it != arguments.filenameVector.end(); it++) {

	exit(rv);	
}	//int main(int argc, const char** argv) {
