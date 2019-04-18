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

// #define _DEBUG_
#include "misc/debugMsgs.h"
#include "misc/errMsgs.h"

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "popt.h"
#include "misc/poptUtils.h"
#include "misc/errMsgs.h"
#include "processor.h"

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "libdelimText/src/textFile.h"

int main(int argc, const char** argv) {
	int rv = EXIT_FAILURE;

	textFile txtFileObj;
	vector<string> filenameVector;
	string strType = "";
	string strCustom1 = "";
	string strCustom2 = "";
	u_int16_t uiYear = boost::posix_time::second_clock::local_time().date().year();
	timeZoneCalculator tzcalc;
	u_int32_t uiSkew = 0;
	bool bNormalize = false;
	bool bHTMLDecode = false;
	string strLog;

	struct poptOption optionsTable[] = {
		{"type",			't',	POPT_ARG_STRING,	NULL,	10,	"Format for data.", "type"},
		{"year",			'y',	POPT_ARG_INT,		NULL,	20,	"Some logs do not store the year in each entry.  Defaults to the current year.",	"year"},
		{"timezone", 	'z',	POPT_ARG_STRING,	NULL,	30,	"POSIX timezone string (e.g. 'EST-5EDT,M4.1.0,M10.1.0' or 'GMT-5') indicating which zone the logs are using. Defaults to GMT.", "zone"},
		{"skew",			's',	POPT_ARG_INT,		NULL,	40,	"Adjust time values by given seconds.", "seconds"},
		{"normalize",	'n',	POPT_ARG_NONE,		NULL,	50,	"Attempt to clean/normalize input data based on known issues with various types of data. Use w/CAUTION and check stderr for results!"},
		{"log",			'l',	POPT_ARG_STRING,	NULL,	55,	"Log errors/warnings to file.", "log"},
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
			case 55:
				strLog = poptGetOptArg(optCon);
				logOpen(strLog);
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

			// For these types, we know there is a leading header row and we use that row to figure out what should be read.
			string strHeader;
			if (strType == "griffeye" || strType == "ief" || strType == "notes" || strType == "exiftool") {
				strHeader = txtFileObj.getNextRow();
				DEBUG("strHeader: " << strHeader);
			}	  

			while (txtFileObj.getNextRow(&strData)) {
				DEBUG("strData: " << strData);

				if (strType == "squidw3c") {
	 				processSquidW3c(&strData, uiYear, uiSkew, bNormalize, &tzcalc, strFields, strSecondary);
				} else if (strType == "symantec") {
	 				processSymantec(&strData, uiYear, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "ipfw") {
					strFields[MULTI2MAC_LOG] = "--------ipfw";
					strFields[MULTI2MAC_DETAIL] = "Not Yet Implemented";
				} else if (strType == "pf") {
					strFields[MULTI2MAC_LOG] = "----------pf";
					strFields[MULTI2MAC_DETAIL] = "Not Yet Implemented";
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
				} else if (strType == "ief") {
					processIEF(&strData, &strHeader, &*it, uiSkew, bNormalize, &tzcalc, strFields, strSecondary);
				} else if (strType == "notes") {
					processNotes(&strData, &strHeader, uiSkew, bNormalize, &tzcalc, strFields);
				} else if (strType == "exiftool") {
					processExifTool(&strData, &strHeader, uiSkew, bNormalize, &tzcalc, strFields, strSecondary);
				} else {
					strFields[MULTI2MAC_LOG] = "-----unknown";
					strFields[MULTI2MAC_DETAIL] = "Unknown Type";
				}

				if (strFields[MULTI2MAC_DETAIL].length() > 0 ) {
					// Do some rudimentary cleanup on the data; since '|' is a field delimiter, it cannot be in the final output.
					replace(strFields[MULTI2MAC_HASH].begin(), strFields[MULTI2MAC_HASH].end(), '|', '-');
					replace(strFields[MULTI2MAC_DETAIL].begin(), strFields[MULTI2MAC_DETAIL].end(), '|', '-');
					replace(strFields[MULTI2MAC_TYPE].begin(), strFields[MULTI2MAC_TYPE].end(), '|', '-');
					replace(strFields[MULTI2MAC_LOG].begin(), strFields[MULTI2MAC_LOG].end(), '|', '-');
					replace(strFields[MULTI2MAC_FROM].begin(), strFields[MULTI2MAC_FROM].end(), '|', '-');
					replace(strFields[MULTI2MAC_TO].begin(), strFields[MULTI2MAC_TO].end(), '|', '-');
	
					// Output final mactime format
					cout 					<< strFields[MULTI2MAC_HASH]		<< "|"
											<< strFields[MULTI2MAC_DETAIL]	<< "|"
											<< strFields[MULTI2MAC_TYPE]		<< "|"
											<< strFields[MULTI2MAC_LOG]		<< "|"
											<< strFields[MULTI2MAC_FROM]		<< "|"
											<< strFields[MULTI2MAC_TO]			<< "|"
											<< strFields[MULTI2MAC_SIZE]		<< "|"
											<< strFields[MULTI2MAC_ATIME]		<< "|"
											<< strFields[MULTI2MAC_MTIME]		<< "|"
											<< strFields[MULTI2MAC_CTIME]		<< "|"
											<< strFields[MULTI2MAC_BTIME]		<< "\n";
				} // if (strFields[MULTI2MAC_DETAIL].length() > 0 ) {
	
				// If secondary records created, output them in mactime format also
				if (strSecondary[MULTI2MAC_DETAIL].length() > 0) {
					// Do some rudimentary cleanup on the data; since '|' is a field delimiter, it cannot be in the final output.
					replace(strSecondary[MULTI2MAC_HASH].begin(), strSecondary[MULTI2MAC_HASH].end(), '|', '-');
					replace(strSecondary[MULTI2MAC_DETAIL].begin(), strSecondary[MULTI2MAC_DETAIL].end(), '|', '-');
					replace(strSecondary[MULTI2MAC_TYPE].begin(), strSecondary[MULTI2MAC_TYPE].end(), '|', '-');
					replace(strSecondary[MULTI2MAC_LOG].begin(), strSecondary[MULTI2MAC_LOG].end(), '|', '-');
					replace(strSecondary[MULTI2MAC_FROM].begin(), strSecondary[MULTI2MAC_FROM].end(), '|', '-');
					replace(strSecondary[MULTI2MAC_TO].begin(), strSecondary[MULTI2MAC_TO].end(), '|', '-');
	
					cout 				<< strSecondary[MULTI2MAC_HASH]		<< "|"
										<< strSecondary[MULTI2MAC_DETAIL]	<< "|"
										<< strSecondary[MULTI2MAC_TYPE]		<< "|"
										<< strSecondary[MULTI2MAC_LOG]		<< "|"
										<< strSecondary[MULTI2MAC_FROM]		<< "|"
										<< strSecondary[MULTI2MAC_TO]			<< "|"
										<< strSecondary[MULTI2MAC_SIZE]		<< "|"
										<< strSecondary[MULTI2MAC_ATIME]		<< "|"
										<< strSecondary[MULTI2MAC_MTIME]		<< "|"
										<< strSecondary[MULTI2MAC_CTIME]		<< "|"
										<< strSecondary[MULTI2MAC_BTIME]		<< "\n";
				} // if (strSecondary[MULTI2MAC_DETAIL].length() > 0) {

				// Clear out values for the next line
				for (int i=0; i<11; i++) {
					strFields[i] = "";
					strSecondary[i] = "";
				}
			}
		} else {
			ERROR(*it << ": Unable to open file");
		} // if (txtFileObj.open(*it)) { 
	}	// for (vector<string>::iterator it = arguments.filenameVector.begin(); it != arguments.filenameVector.end(); it++) {

	if (strLog != "") {
		logClose();
	}

	exit(rv);	
}	// int main(int argc, const char** argv) {

