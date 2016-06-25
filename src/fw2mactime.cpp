#define APPNAME "fw2mactime"
#define VERSION "20070319"

//Normal mactime for files
//"    |FILENAME | |INODE   | |PERMISSIONS| |UID       |GID       |       |SIZE     |ATIME  |MTIME  |CTIME    | | "

//Mactime for event records
//"REC#|EVENTFILE| |EVENTID | |SOURCE     | |SID       |COMPUTER  |       |TYPE     |       |WRITTEN|GENERATED| | "

//Mactime for firewall records
//"FWL |MESSAGE  | |MSG-TYPE| |SERVICE    | |SRCIP     |DESTIP    |RAWDATA|SENT/RECV|       |       |GENERATED| | "

#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iostream>
using namespace std;

#include "popt.h"

#include "libTimeUtils/timeZoneCalculator.h"
#include "libTimeUtils/timeUtils.h"
#include "libdelimText/textFile.h"
#include "libdelimText/textUtils.h"
#include "Misc/debugMsgs.h"
#include "Misc/poptUtils.h"
#include "Misc/boost_lexical_cast_wrapper.hpp"

void processSymantec(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
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

	cout << "FWL|\"" << strMsg << "\"||" << strMsgType << "||" << strService << "||" << strSrc << "|" << strDst << "|\"" << *pstrData << "\"|" << strBytes << "|||" << (timeVal > 0 ? boost_lexical_cast_wrapper<string>(timeVal) : "") << "||" << endl;
}

void processJuniper(string* pstrData, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
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

void processPIX(string* pstrData, u_int32_t uiSkew, timeZoneCalculator* pTZCalc) {
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
	string strFirewallType = "symantec";
	u_int16_t uiYear = posix_time::second_clock::local_time().date().year();
	timeZoneCalculator tzcalc;
	u_int32_t uiSkew = 0;

	struct poptOption optionsTable[] = {
		{"firewall",	'f',	POPT_ARG_STRING,	NULL,	10,	"Format for firewall log(s).",	"firewall"},
		{"year",			'y',	POPT_ARG_INT,		NULL,	20,	"Some firewall logs do not store the year in each entry.  Defaults to the current year.",	"year"},
		{"timezone", 	'z',	POPT_ARG_STRING,	NULL,	30,	"POSIX timezone string (e.g. 'EST-5EDT,M4.1.0,M10.1.0' or 'GMT-5') indicating which zone the firewall logs are using. Defaults to GMT.", "zone"},
		{"skew",		 	0,		POPT_ARG_INT,		NULL,	40,	"Adjust time values by given seconds.", "seconds"},
		{"version",	 			0,		POPT_ARG_NONE,		NULL,	100,	"Display version.", NULL},
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
				version(APPNAME, VERSION);
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
			while (txtFileObj.getNextRow(&strData)) {
				if (strFirewallType == "symantec") {
					processSymantec(&strData, uiYear, uiSkew, &tzcalc);
				} else if (strFirewallType == "ipfw") {
					cout << "Not Yet Implemented" << endl;
				} else if (strFirewallType == "pf") {
					cout << "Not Yet Implemented" << endl;
				} else if (strFirewallType == "pix") {
					processPIX(&strData, uiSkew, &tzcalc);
				} else if (strFirewallType == "juniper") {
					processJuniper(&strData, uiSkew, &tzcalc);
				} else {
					cout << "Unknown Firewall Type" << endl;
				}
			}
		} else {
		}
	}	//for (vector<string>::iterator it = arguments.filenameVector.begin(); it != arguments.filenameVector.end(); it++) {

	exit(rv);	
}	//int main(int argc, const char** argv) {
