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

//#define _DEBUG_
#include "misc/debugMsgs.h"

#include <string>
using namespace std;

#include "libtimeUtils/src/timeZoneCalculator.h"
#include "misc/tsk_mactime.h"

//0		|1			|2			|3			|4		|5		|6		|7			|8			|9			|10
//Sleuthkit TSK3.x body format
//MD5		|NAME		|INODE	|PERMS	|UID	|GID	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME
//multi2mactime
//HASH	|DETAIL	|TYPE		|LOG-SRC	|FROM	|TO	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME

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

#endif /*MULTI2MACTIME_PROCESSOR_H_*/

