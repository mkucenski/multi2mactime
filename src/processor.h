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

#ifndef MULTI2MACTIME_PROCESSOR_H_
#define MULTI2MACTIME_PROCESSOR_H_

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "misc/tsk_mactime.h"

//0		|1			|2			|3			|4		|5		|6		|7			|8			|9			|10
//Sleuthkit TSK3.x body format
//MD5		|NAME		|INODE	|PERMS	|UID	|GID	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME
//multi2mactime
//HASH	|DETAIL	|TYPE		|LOG-SRC	|FROM	|TO	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME
//
//MODIFIED = file content changed
//ACCESSED
//CHANGED = file meta (MFT) data changed
//CREATED

#define MULTI2MAC_HASH		TSK3_MACTIME_MD5
#define MULTI2MAC_DETAIL	TSK3_MACTIME_NAME
#define MULTI2MAC_TYPE		TSK3_MACTIME_INODE
#define MULTI2MAC_LOG		TSK3_MACTIME_PERMS
#define MULTI2MAC_FROM		TSK3_MACTIME_UID
#define MULTI2MAC_TO			TSK3_MACTIME_GID
#define MULTI2MAC_SIZE		TSK3_MACTIME_SIZE
#define MULTI2MAC_ATIME		TSK3_MACTIME_ATIME
#define MULTI2MAC_MTIME		TSK3_MACTIME_MTIME
#define MULTI2MAC_CTIME		TSK3_MACTIME_CTIME
#define MULTI2MAC_BTIME		TSK3_MACTIME_CRTIME

void processExifTool(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary);
void processNotes(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processIEF(string* pstrData, string* pstrHeader, string* pstrFilename, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary);
void processGriffeyeCSV(string* pstrData, string* pstrHeader, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processHirsch(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processFortiGate1K5(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processSquidW3c(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields, string* strSecondary);
void processCustomVPN_S1(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processCustomFSEM(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processCustomFSBT(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processSymantec(string* pstrData, u_int16_t uiYear, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processJuniper(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);
void processPIX(string* pstrData, u_int32_t uiSkew, bool bNormalize, timeZoneCalculator* pTZCalc, string* strFields);

int32_t getUnix32FromStrings(string strMonth, string strDay, string strYear, string strHour, string strMinute, string strSecond, u_int32_t uiSkew, timeZoneCalculator* pTZCalc);
int32_t getUnix32DateTimeFromString(string strDateTime, char chSeparator, char chDateDelim, char chTimeDelim, u_int32_t uiSkew, timeZoneCalculator* pTZCalc);
int32_t getUnix32DateTimeFromString2(string strDateTime, char chSeparator, char chDateDelim, char chTimeDelim, u_int32_t uiSkew, timeZoneCalculator* pTZCalc);

#endif /*MULTI2MACTIME_PROCESSOR_H_*/

