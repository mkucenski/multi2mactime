// Copyright 2018 Matthew A. Kucenski
//
// Licensed under the Apache License, Version 2.0 (the"License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an"AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _IEFTYPES_H_
#define _IEFTYPES_H_

#include"misc/coded-message.h"

// Magnet's IEF software allows you to export artifacts found during an examination in XLSX format. By 
// converting these files into CSV (e.g. w/ py-xlsx2csv), timeline analysis can be conducted in
// conjunction with other datasets. However, individual artifact spreadsheets have various headers/fields
// as well as associated meanings. This file attempts to pre-define those fields and how they might
// relate to other activity in a timeline analysis.
//
// It works as follows:
// 1) Lookup a unique code assigned to an individual artifact file (based on the name of the file).
// 2) Using that code (e.g. IEF_ARTIFACT_GOOGLE_SEARCHES) + specific type code (e.g. 
// IEF_PRIMARY + IEF_ATIME) you can lookup the header name for the column you should access.
//
// So, looking up <IEF_ARTIFACT_GOOGLE_SEARCHES + IEF IEF_PRIMARY + IEF_ATIME> in <IEF_ARTIFACT_FIELDS> 
// should return: "Date/Time - (UTC) (MM/dd/yyyy)"
//
// With that header string name, you should be able to find the field number and pull the individual
// values out of the delimited CSV data. NOTE: I am not saving specific field numbers within these tables
// since it is entirely possible Magnet may decide to reorder the fields on export. Forcing an additional
// 'find' function based on the header name will hopefully help bypass such an event.
//
// You can also pull out three details fields that can be used to build the timeline event message.
//
// While the coded-message.h functionality was originally designed to assist in deconstructing binary
// fields in various types of data files, it also provides a general table lookup functionality. They are
// unused at this time, but the "short" and "details" fields of the tables could be used to pre-define 
// additional information/coding.

#define IEF_PRIMARY_MASK	0x00F00
#define IEF_PRIMARY			0x00100
#define IEF_SECONDARY		0x00200
#define IEF_TERTIARY			0x00300

#define IEF_TYPE_MASK		0x000FF
#define IEF_HASH				0x00010
#define IEF_TYPE				0x00020		//12 characters overlaid on the inode field
#define IEF_LOG				0x00030		//12 characters overlaid on the permissions field
#define IEF_FROM				0x00040		// 8 characters
#define IEF_TO					0x00050		// 8 characters
#define IEF_SIZE				0x00060		//10 characters
#define IEF_ATIME				0x00070 	//Last time the item was accessed/visited.
#define IEF_ATIME2			IEF_ATIME + 0x00001
#define IEF_ATIME3			IEF_ATIME + 0x00002
#define IEF_ATIME4			IEF_ATIME + 0x00003
#define IEF_ATIME5			IEF_ATIME + 0x00004
#define IEF_ATIME6			IEF_ATIME + 0x00005
#define IEF_ATIME7			IEF_ATIME + 0x00006
#define IEF_ATIME8			IEF_ATIME + 0x00007
#define IEF_MTIME				0x00080 	// Last time the item itself was modified.
#define IEF_CTIME				0x00090		// Metadata about the item was changed and/or changes related to something more abstract (e.g. an expiration date).
#define IEF_BTIME				0x000A0		// Item itself was created [birthed].
#define IEF_DETAIL			0x000B0
#define IEF_DETAIL2			IEF_DETAIL + 0x00001
#define IEF_DETAIL3			IEF_DETAIL + 0x00002

#define IEF_ARTIFACT_MASK																0xFF000
#define IEF_ARTIFACT_ARES_SEARCH_KEYWORDS											0x01000
#define IEF_ARTIFACT_CARVED_VIDEO													0x02000
#define IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES									0x03000
#define IEF_ARTIFACT_CHROME_AUTOFILL												0x04000
#define IEF_ARTIFACT_CHROME_CACHE_RECORDS											0x05000
#define IEF_ARTIFACT_CHROME_COOKIES													0x06000
#define IEF_ARTIFACT_CHROME_CURRENT_SESSION										0x07000
#define IEF_ARTIFACT_CHROME_CURRENT_TABS											0x08000
#define IEF_ARTIFACT_CHROME_DOWNLOADS												0x09000
#define IEF_ARTIFACT_CHROME_FAVICONS												0x0A000
#define IEF_ARTIFACT_CHROME_KEYWORD_SEARCH_TERMS								0x0B000
#define IEF_ARTIFACT_CHROME_LOGINS													0x0C000
#define IEF_ARTIFACT_CHROME_SHORTCUTS												0x0D000
#define IEF_ARTIFACT_CHROME_TOP_SITES												0x0E000
#define IEF_ARTIFACT_CHROME_WEB_HISTORY											0x0F000
#define IEF_ARTIFACT_CHROME_WEB_VISITS												0x10000
#define IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS			0x11000
#define IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY	0x12000
#define IEF_ARTIFACT_CLASSIFIEDS_URLS												0x13000
#define IEF_ARTIFACT_CLOUD_SERVICES_URLS											0x14000
#define IEF_ARTIFACT_CRAIGSLIST_ADS													0x15000
#define IEF_ARTIFACT_DATING_SITES_URLS												0x16000
#define IEF_ARTIFACT_ENCRYPTED_FILES												0x17000
#define IEF_ARTIFACT_FACEBOOK_CHAT													0x18000
#define IEF_ARTIFACT_FACEBOOK_PAGES													0x19000
#define IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS			0x1A000
#define IEF_ARTIFACT_FACEBOOK_URLS													0x1B000
#define IEF_ARTIFACT_FILE_SYSTEM_INFORMATION										0x1C000
#define IEF_ARTIFACT_FIREFOX_BOOKMARKS												0x1D000
#define IEF_ARTIFACT_FIREFOX_CACHE_RECORDS										0x1E000
#define IEF_ARTIFACT_FIREFOX_COOKIES												0x1F000
#define IEF_ARTIFACT_FIREFOX_DOWNLOADS												0x20000
#define IEF_ARTIFACT_FIREFOX_FAVICONS												0x21000
#define IEF_ARTIFACT_FIREFOX_FORMHISTORY											0x22000
#define IEF_ARTIFACT_FIREFOX_INPUT_HISTORY										0x23000
#define IEF_ARTIFACT_FIREFOX_SESSIONSTORE_ARTIFACTS							0x24000
#define IEF_ARTIFACT_FIREFOX_WEB_HISTORY											0x25000
#define IEF_ARTIFACT_FIREFOX_WEB_VISITS											0x26000
#define IEF_ARTIFACT_FLASH_COOKIES													0x27000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED			0x28000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES					0x29000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED				0x2A000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES						0x2B000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED				0x2C000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES							0x2D000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED								0x2E000
#define IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS										0x2F000
#define IEF_ARTIFACT_GOOGLE_MAPS_QUERIES											0x30000
#define IEF_ARTIFACT_GOOGLE_MAPS_TILES												0x31000
#define IEF_ARTIFACT_GOOGLE_MAPS														0x32000
#define IEF_ARTIFACT_GOOGLE_SEARCHES												0x33000
#define IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS									0x34000
#define IEF_ARTIFACT_IDENTIFIERS														0x35000
#define IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS								0x36000
#define IEF_ARTIFACT_INSTALLED_PROGRAMS											0x37000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT							0x38000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES							0x39000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY			0x3A000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DEPENDENCY_ENTRIES			0x3B000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS						0x3C000
#define IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY					0x3D000
#define IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS							0x3E000
#define IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES								0x3F000
#define IEF_ARTIFACT_INTERNET_EXPLORER_TYPED_URLS								0x40000
#define IEF_ARTIFACT_JUMP_LISTS														0x41000
#define IEF_ARTIFACT_KEYWORD_SEARCHES												0x42000
#define IEF_ARTIFACT_LNK_FILES														0x43000
#define IEF_ARTIFACT_MSN_PLUS															0x44000
#define IEF_ARTIFACT_MALWARE_PHISHING_URLS										0x45000
#define IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY								0x46000
#define IEF_ARTIFACT_NETWORK_PROFILES												0x47000
#define IEF_ARTIFACT_NETWORK_SHARE_INFORMATION									0x48000
#define IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES										0x49000
#define IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION								0x4A000
#define IEF_ARTIFACT_PDF_DOCUMENTS													0x4B000
#define IEF_ARTIFACT_PARSED_SEARCH_QUERIES										0x4C000
#define IEF_ARTIFACT_PICTURES															0x4D000
#define IEF_ARTIFACT_PORNOGRAPHY_URLS												0x4E000
#define IEF_ARTIFACT_POTENTIAL_BROWSER_ACTIVITY									0x4F000
#define IEF_ARTIFACT_POWERPOINT_DOCUMENTS											0x50000
#define IEF_ARTIFACT_QQ																	0x51000
#define IEF_ARTIFACT_RTF_DOCUMENTS													0x52000
#define IEF_ARTIFACT_REBUILT_WEBPAGES												0x53000
#define IEF_ARTIFACT_RECYCLE_BIN														0x54000
#define IEF_ARTIFACT_SAFARI_HISTORY													0x55000
#define IEF_ARTIFACT_SECOND_LIFE														0x56000
#define IEF_ARTIFACT_SHELLBAGS														0x57000
#define IEF_ARTIFACT_SHIPPING_SITE_URLS											0x58000
#define IEF_ARTIFACT_SOCIAL_MEDIA_URLS												0x59000
#define IEF_ARTIFACT_STARTUP_ITEMS													0x5A000
#define IEF_ARTIFACT_TAX_SITE_URLS													0x5B000
#define IEF_ARTIFACT_TEXT_DOCUMENTS													0x5C000
#define IEF_ARTIFACT_TIMEZONE_INFORMATION											0x5D000
#define IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS									0x5E000
#define IEF_ARTIFACT_TORRENT_URLS													0x5F000
#define IEF_ARTIFACT_USB_DEVICES														0x60000
#define IEF_ARTIFACT_USER_ACCOUNTS													0x61000
#define IEF_ARTIFACT_USERASSIST														0x62000
#define IEF_ARTIFACT_WEB_CHAT_URLS													0x63000
#define IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES								0x64000
#define IEF_ARTIFACT_WINDOWS_EVENT_LOGS											0x65000
#define IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN									0x66000
#define IEF_ARTIFACT_WINDOWS_PREFETCH_FILES										0x67000
#define IEF_ARTIFACT_WORD_DOCUMENTS													0x68000
#define IEF_ARTIFACT_YAHOO_NON_ENCRYPTED_CHAT									0x69000
#define IEF_ARTIFACT_ZOOM_CHAT_MESSAGES											0x6A000
//#define IEF_ARTIFACT_OUTLOOK_EMAILS													0x6B000	//TODO multi-line CSVs are not processing correctly
#define IEF_ARTIFACT_OUTLOOK_APPOINTMENTS											0x6C000
#define IEF_ARTIFACT_OUTLOOK_CONTACTS												0x6D000
//#define IEF_ARTIFACT_EML_EMAILS														0x6E000	//TODO multi-line CSVs are not processing correctly
#define IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS								0x6F000
#define IEF_ARTIFACT_EXCEL_DOCUMENTS												0X70000
#define IEF_ARTIFACT_FACEBOOK_PICTURES												0x71000
#define IEF_ARTIFACT_YAHOO_EMAILS													0x72000

static coded_message_t IEF_ARTIFACTS[] = {
//	strMessage															idCode																			strDetails(12)		strShort(8)
	{"Ares Search Keywords",										IEF_ARTIFACT_ARES_SEARCH_KEYWORDS,										"search",			"ares"},
	{"Carved Video",													IEF_ARTIFACT_CARVED_VIDEO,													"video",				"carved"},
	{"Chrome Autofill Profiles",									IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES,									"autofill",			"chrome"},
	{"Chrome Autofill",												IEF_ARTIFACT_CHROME_AUTOFILL,												"autofill",			"chrome"},
	{"Chrome Cache Records",										IEF_ARTIFACT_CHROME_CACHE_RECORDS,										"cache",				"chrome"},
	{"Chrome Cookies",												IEF_ARTIFACT_CHROME_COOKIES,												"cookie",			"chrome"},
	{"Chrome Current Session",										IEF_ARTIFACT_CHROME_CURRENT_SESSION,									"session",			"chrome"},
	{"Chrome Current Tabs",											IEF_ARTIFACT_CHROME_CURRENT_TABS,										"tab",				"chrome"},
	{"Chrome Downloads",												IEF_ARTIFACT_CHROME_DOWNLOADS,											"download",			"chrome"},
	{"Chrome FavIcons",												IEF_ARTIFACT_CHROME_FAVICONS,												"favicon",			"chrome"},
	{"Chrome Keyword Search Terms",								IEF_ARTIFACT_CHROME_KEYWORD_SEARCH_TERMS,								"search",			"chrome"},
	{"Chrome Logins",													IEF_ARTIFACT_CHROME_LOGINS,												"login",				"chrome"},
	{"Chrome Shortcuts",												IEF_ARTIFACT_CHROME_SHORTCUTS,											"shortcut",			"chrome"},
	{"Chrome Top Sites",												IEF_ARTIFACT_CHROME_TOP_SITES,											"top-site",			"chrome"},
	{"Chrome Web History",											IEF_ARTIFACT_CHROME_WEB_HISTORY,											"history",			"chrome"},
	{"Chrome Web Visits",											IEF_ARTIFACT_CHROME_WEB_VISITS,											"visit",				"chrome"},
	{"Chrome-360 Safe Browser Carved Session-Tabs",			IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS,		"carved-tab",		"chrome"},
	{"Chrome-360 Safe Browser-Opera Carved Web History",	IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY,	"carved-hist",		"chrome"},
	{"Classifieds URLs",												IEF_ARTIFACT_CLASSIFIEDS_URLS,											"classifieds",		"url"},
	{"Cloud Services URLs",											IEF_ARTIFACT_CLOUD_SERVICES_URLS,										"cloud",				"url"},
	{"Craigslist Ads",												IEF_ARTIFACT_CRAIGSLIST_ADS,												"craigslist",		"url"},
	{"Dating Sites URLs",											IEF_ARTIFACT_DATING_SITES_URLS,											"dating",			"url"},
	{"Encrypted Files",												IEF_ARTIFACT_ENCRYPTED_FILES,												"encrypted",		"file"},
	{"Facebook Chat",													IEF_ARTIFACT_FACEBOOK_CHAT,												"facebook",			"chat"},
	{"Facebook Pages",												IEF_ARTIFACT_FACEBOOK_PAGES,												"facebook-pge",	"url"},
	{"Facebook Status Updates-Wall Posts-Comments",			IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS,		"facebook-wll",	"url"},
	{"Facebook URLs",													IEF_ARTIFACT_FACEBOOK_URLS,												"facebook",			"url"},
	{"File System Information",									IEF_ARTIFACT_FILE_SYSTEM_INFORMATION,									"info",				"filesys"},
	{"Firefox Bookmarks",											IEF_ARTIFACT_FIREFOX_BOOKMARKS,											"bookmark",			"firefox"},
	{"Firefox Cache Records",										IEF_ARTIFACT_FIREFOX_CACHE_RECORDS,										"cache",				"firefox"},
	{"Firefox Cookies",												IEF_ARTIFACT_FIREFOX_COOKIES,												"cookie",			"firefox"},
	{"Firefox Downloads",											IEF_ARTIFACT_FIREFOX_DOWNLOADS,											"download",			"firefox"},
	{"Firefox FavIcons",												IEF_ARTIFACT_FIREFOX_FAVICONS,											"favicon",			"firefox"},
	{"Firefox FormHistory",											IEF_ARTIFACT_FIREFOX_FORMHISTORY,										"form",				"firefox"},
	{"Firefox Input History",										IEF_ARTIFACT_FIREFOX_INPUT_HISTORY,										"input",				"firefox"},
	{"Firefox SessionStore Artifacts",							IEF_ARTIFACT_FIREFOX_SESSIONSTORE_ARTIFACTS,							"session",			"firefox"},
	{"Firefox Web History",											IEF_ARTIFACT_FIREFOX_WEB_HISTORY,										"history",			"firefox"},
	{"Firefox Web Visits",											IEF_ARTIFACT_FIREFOX_WEB_VISITS,											"visit",				"firefox"},
	{"Flash Cookies",													IEF_ARTIFACT_FLASH_COOKIES,												"cookie",			"flash"},
	{"Google Analytics First Visit Cookies Carved",			IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED,		"cookie",			"google"},
	{"Google Analytics First Visit Cookies",					IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES,					"cookie",			"google"},
	{"Google Analytics Referral Cookies Carved",				IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED,			"cookie",			"google"},
	{"Google Analytics Referral Cookies",						IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES,						"cookie",			"google"},
	{"Google Analytics Session Cookies Carved",				IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED,				"cookie",			"google"},
	{"Google Analytics Session Cookies",						IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES,						"cookie",			"google"},
	{"Google Analytics URLs Carved",								IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED,							"analytics",		"google"},
	{"Google Analytics URLs",										IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS,										"analytics",		"google"},
	{"Google Maps Queries",											IEF_ARTIFACT_GOOGLE_MAPS_QUERIES,										"search",			"google"},
	{"Google Maps Tiles",											IEF_ARTIFACT_GOOGLE_MAPS_TILES,											"map-tile",			"google"},
	{"Google Maps",													IEF_ARTIFACT_GOOGLE_MAPS,													"map",				"google"},
	{"Google Searches",												IEF_ARTIFACT_GOOGLE_SEARCHES,												"search",			"google"},
	{"IE InPrivate-Recovery URLs",								IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS,								"private",			"iexplore"},
	{"Identifiers",													IEF_ARTIFACT_IDENTIFIERS,													"identity",			"identity"},
	{"Installed Microsoft Programs",								IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS,							"installed-ms",	"os"},
	{"Installed Programs",											IEF_ARTIFACT_INSTALLED_PROGRAMS,											"installed",		"os"},
	{"Internet Explorer 10-11 Content",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT,						"content",			"iexplore"},
	{"Internet Explorer 10-11 Cookies",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES,						"cookie",			"iexplore"},
	{"Internet Explorer 10-11 Daily-Weekly History",		IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY,		"history",			"iexplore"},
	{"Internet Explorer 10-11 Dependency Entries",			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DEPENDENCY_ENTRIES,			"entry",				"iexplore"},
	{"Internet Explorer 10-11 Downloads",						IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS,						"download",			"iexplore"},
	{"Internet Explorer 10-11 Main History",					IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY,					"history",			"iexplore"},
	{"Internet Explorer Cache Records",							IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS,						"cache",				"iexplore"},
	{"Internet Explorer Favorites",								IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES,								"favorite",			"iexplore"},
	{"Internet Explorer Typed URLs",								IEF_ARTIFACT_INTERNET_EXPLORER_TYPED_URLS,							"typed-url",		"iexplore"},
	{"Jump Lists",														IEF_ARTIFACT_JUMP_LISTS,													"jump-list",		"file"},
	{"Keyword Searches",												IEF_ARTIFACT_KEYWORD_SEARCHES,											"search",			"url"},
	{"LNK Files",														IEF_ARTIFACT_LNK_FILES,														"lnk",				"file"},
	{"MSN Plus!",														IEF_ARTIFACT_MSN_PLUS,														"msn",				"url"},
	{"Malware-Phishing URLs",										IEF_ARTIFACT_MALWARE_PHISHING_URLS,										"malware",			"url"},
	{"Network Interfaces (Registry)",							IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,								"registry",			"network"},
	{"Network Profiles",												IEF_ARTIFACT_NETWORK_PROFILES,											"profile",			"network"},
	{"Network Share Information",									IEF_ARTIFACT_NETWORK_SHARE_INFORMATION,								"share-info",		"network"},
	{"Opera Autofill Profiles",									IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES,									"autofill",			"opera"},
	{"Operating System Information",								IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION,							"info",				"os"},
	{"PDF Documents",													IEF_ARTIFACT_PDF_DOCUMENTS,												"pdf",				"document"},
	{"Parsed Search Queries",										IEF_ARTIFACT_PARSED_SEARCH_QUERIES,										"search",			"url"},
	{"Pictures",														IEF_ARTIFACT_PICTURES,														"picture",			"picture"},
	{"Pornography URLs",												IEF_ARTIFACT_PORNOGRAPHY_URLS,											"pornography",		"url"},
	{"Potential Browser Activity",								IEF_ARTIFACT_POTENTIAL_BROWSER_ACTIVITY,								"potential",		"url"},
	{"PowerPoint Documents",										IEF_ARTIFACT_POWERPOINT_DOCUMENTS,										"ppt",				"document"},
	{"QQ",																IEF_ARTIFACT_QQ,																"qq",					"chat"},
	{"RTF Documents",													IEF_ARTIFACT_RTF_DOCUMENTS,												"rtf",				"document"},
	{"Rebuilt Webpages",												IEF_ARTIFACT_REBUILT_WEBPAGES,											"rebuilt",			"url"},
	{"Recycle Bin",													IEF_ARTIFACT_RECYCLE_BIN,													"recycle-bin",		"os"},
	{"Safari History",												IEF_ARTIFACT_SAFARI_HISTORY,												"history",			"safari"},
	{"Second Life",													IEF_ARTIFACT_SECOND_LIFE,													"second-life",		"games"},
	{"Shellbags",														IEF_ARTIFACT_SHELLBAGS,														"shellbag",			"os"},
	{"Shipping Site URLs",											IEF_ARTIFACT_SHIPPING_SITE_URLS,											"shipping",			"url"},
	{"Social Media URLs",											IEF_ARTIFACT_SOCIAL_MEDIA_URLS,											"social-media",	"url"},
	{"Startup Items",													IEF_ARTIFACT_STARTUP_ITEMS,												"startup-item",	"os"},
	{"Tax Site URLs",													IEF_ARTIFACT_TAX_SITE_URLS,												"tax-site",			"url"},
	{"Text Documents",												IEF_ARTIFACT_TEXT_DOCUMENTS,												"text",				"document"},
	{"Timezone Information",										IEF_ARTIFACT_TIMEZONE_INFORMATION,										"timezone",			"os"},
	{"Torrent Active Transfers",									IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS,									"transfer",			"torrent"},
	{"Torrent URLs",													IEF_ARTIFACT_TORRENT_URLS,													"url",				"torrent"},
	{"USB Devices",													IEF_ARTIFACT_USB_DEVICES,													"device",			"usb"},
	{"User Accounts",													IEF_ARTIFACT_USER_ACCOUNTS,												"account",			"user"},
	{"UserAssist",														IEF_ARTIFACT_USERASSIST,													"assist",			"user"},
	{"Web Chat URLs",													IEF_ARTIFACT_WEB_CHAT_URLS,												"web",				"chat"},
	{"Windows 8-10 Prefetch Files",								IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES,								"prefetch",			"file"},
	{"Windows Event Logs",											IEF_ARTIFACT_WINDOWS_EVENT_LOGS,											"EVT",				"log"},
	{"Windows Live Messenger - MSN",								IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN,								"log",				"chat"},
	{"Windows Prefetch Files",										IEF_ARTIFACT_WINDOWS_PREFETCH_FILES,									"prefetch",			"file"},
	{"Word Documents",												IEF_ARTIFACT_WORD_DOCUMENTS,												"word",				"document"},
	{"Yahoo! Non-Encrypted Chat",									IEF_ARTIFACT_YAHOO_NON_ENCRYPTED_CHAT,									"yahoo",				"chat"},
	{"Zoom Chat Messages",											IEF_ARTIFACT_ZOOM_CHAT_MESSAGES,											"zoom",				"chat"},
	////{"Outlook Emails",												IEF_ARTIFACT_OUTLOOK_EMAILS,												"outlook",			"email"},
	{"Outlook Appointments",										IEF_ARTIFACT_OUTLOOK_APPOINTMENTS,										"outlook",			"appointm"},
	{"Outlook Contacts",												IEF_ARTIFACT_OUTLOOK_CONTACTS,											"outlook",			"contact"},
	////{"EML(X) Files",													IEF_ARTIFACT_EML_EMAILS,													"eml",				"email"},
	{"Encryption - Anti-forensics Tools",						IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS,							"encr-antifor",	"software"},
	{"Excel Documents",												IEF_ARTIFACT_EXCEL_DOCUMENTS,												"excel",				"document"},
	{"Facebook Pictures",											IEF_ARTIFACT_FACEBOOK_PICTURES,											"facebook",			"picture"},
	{"Yahoo! Webmail",												IEF_ARTIFACT_YAHOO_EMAILS,													"yahoo",				"email"}
};

static coded_message_t IEF_ARTIFACT_FIELDS[] = {
//	strMessage																		idCode																			strDetails	strShort
	//{"Search Keyword",															IEF_ARTIFACT_ARES_SEARCH_KEYWORDS, "", ""},

	//{"Category",																	IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	//{"Container Format",														IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	//{"Content Format",															IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_CARVED_VIDEO + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Image",																		IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	{"MD5 Hash",																	IEF_ARTIFACT_CARVED_VIDEO + IEF_PRIMARY + IEF_HASH, "", ""},
	//{"SHA1 Hash",																IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	//{"Saved Video Size (Bytes)",											IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	//{"Skin Tone Percentage",													IEF_ARTIFACT_CARVED_VIDEO, "", ""},
	//{"_Video",																	IEF_ARTIFACT_CARVED_VIDEO, "", ""},

	//{"Address Line 1",															IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"Address Line 2",															IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"City",																		IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"Company",																	IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"Country",																	IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	{"Date Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Email",																		IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Name",																			IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Number",																	IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"State",																		IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},
	//{"Zipcode",																	IEF_ARTIFACT_CHROME_AUTOFILL_PROFILES, "", ""},

	//{"Count",																		IEF_ARTIFACT_CHROME_AUTOFILL, "", ""},
	{"Date Created Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_AUTOFILL + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Name",																			IEF_ARTIFACT_CHROME_AUTOFILL + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Value",																		IEF_ARTIFACT_CHROME_AUTOFILL + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Content Size (Bytes)",													IEF_ARTIFACT_CHROME_CACHE_RECORDS + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Content",																	IEF_ARTIFACT_CHROME_CACHE_RECORDS, "", ""},
	//{"File Type",																IEF_ARTIFACT_CHROME_CACHE_RECORDS, "", ""},
	{"First Visited Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_CHROME_CACHE_RECORDS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Image",																		IEF_ARTIFACT_CHROME_CACHE_RECORDS, "", ""},
	{"Last Synced Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CACHE_RECORDS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CACHE_RECORDS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_CACHE_RECORDS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Accessed Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Expiration Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Host",																			IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Name",																			IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Path",																		IEF_ARTIFACT_CHROME_COOKIES, "", ""},
	{"Value",																		IEF_ARTIFACT_CHROME_COOKIES + IEF_PRIMARY + IEF_DETAIL3, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CURRENT_SESSION + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CURRENT_SESSION + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Redirect URL",																IEF_ARTIFACT_CHROME_CURRENT_SESSION + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_CURRENT_SESSION + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_CURRENT_SESSION + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_CHROME_CURRENT_SESSION, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CURRENT_TABS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_CURRENT_TABS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Redirect URL",																IEF_ARTIFACT_CHROME_CURRENT_TABS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_CURRENT_TABS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_CURRENT_TABS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_CHROME_CURRENT_TABS, "", ""},

	//{"Bytes Downloaded",														IEF_ARTIFACT_CHROME_DOWNLOADS, "", ""},
	{"Download Source",															IEF_ARTIFACT_CHROME_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"End Time Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_CHROME_DOWNLOADS + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"File Name",																IEF_ARTIFACT_CHROME_DOWNLOADS, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_CHROME_DOWNLOADS + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Opened By User",															IEF_ARTIFACT_CHROME_DOWNLOADS, "", ""},
	//{"Saved To",																	IEF_ARTIFACT_CHROME_DOWNLOADS, "", ""},
	{"Start Time Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_DOWNLOADS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"State",																		IEF_ARTIFACT_CHROME_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Icon URL",																	IEF_ARTIFACT_CHROME_FAVICONS, "", ""},
	//{"Icon",																		IEF_ARTIFACT_CHROME_FAVICONS, "", ""},
	{"Last Updated Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_FAVICONS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Page URL",																	IEF_ARTIFACT_CHROME_FAVICONS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Keyword Search Term",													IEF_ARTIFACT_CHROME_KEYWORD_SEARCH_TERMS, "", ""},
	//{"URL",																		IEF_ARTIFACT_CHROME_KEYWORD_SEARCH_TERMS, "", ""},

	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_CHROME_LOGINS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Password",																	IEF_ARTIFACT_CHROME_LOGINS + IEF_PRIMARY + IEF_TO, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_LOGINS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"User Name",																	IEF_ARTIFACT_CHROME_LOGINS + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Last Access Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Access Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Original Search Query",													IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_SECONDARY + IEF_DETAIL2, "", ""},
	{"Search Term",																IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"Times Used",																IEF_ARTIFACT_CHROME_SHORTCUTS, "", ""},
	//{"Transition Type",														IEF_ARTIFACT_CHROME_SHORTCUTS, "", ""},
	//{"Type",																		IEF_ARTIFACT_CHROME_SHORTCUTS, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Web Page Title",															IEF_ARTIFACT_CHROME_SHORTCUTS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Last Updated Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_TOP_SITES + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"Thumbnail",																IEF_ARTIFACT_CHROME_TOP_SITES, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_TOP_SITES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_TOP_SITES + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_WEB_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Typed Count",																IEF_ARTIFACT_CHROME_WEB_HISTORY, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_CHROME_WEB_HISTORY, "", ""},

	{"Date Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_WEB_VISITS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_WEB_VISITS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Transition Type",														IEF_ARTIFACT_CHROME_WEB_VISITS, "", ""},
	//{"Typed Count",																IEF_ARTIFACT_CHROME_WEB_VISITS, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_WEB_VISITS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Source",															IEF_ARTIFACT_CHROME_WEB_VISITS, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Redirect URL",																IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_CARVED_SESSION_TABS, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Title",																		IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Typed Count",																IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY, "", ""},
	{"URL",																			IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Visit Count",																IEF_ARTIFACT_CHROME_360_SAFE_BROWSER_OPERA_CARVED_WEB_HISTORY, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_CLASSIFIEDS_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_CLASSIFIEDS_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_CLASSIFIEDS_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CLASSIFIEDS_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_CLOUD_SERVICES_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_CLOUD_SERVICES_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_CLOUD_SERVICES_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_CLOUD_SERVICES_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Fragment",																	IEF_ARTIFACT_CRAIGSLIST_ADS, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_DATING_SITES_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_DATING_SITES_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_DATING_SITES_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_DATING_SITES_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Entropy Value",															IEF_ARTIFACT_ENCRYPTED_FILES, "", ""},
	{"File Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_ENCRYPTED_FILES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"File Created Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_ENCRYPTED_FILES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_ENCRYPTED_FILES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_ENCRYPTED_FILES + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Filename",																	IEF_ARTIFACT_ENCRYPTED_FILES + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Additional Information",												IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	//{"Coordinates",																IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	//{"Downloaded Receiver Image",											IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	//{"Downloaded Sender Image",												IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	//{"Message ID",																IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	{"Message Sent Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FACEBOOK_CHAT + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Message",																		IEF_ARTIFACT_FACEBOOK_CHAT + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Profile ID",																IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	{"Receiver ID(s)",															IEF_ARTIFACT_FACEBOOK_CHAT + IEF_PRIMARY + IEF_TO, "", ""},
	//{"Receiver Name(s)",														IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	{"Sender ID",																	IEF_ARTIFACT_FACEBOOK_CHAT + IEF_PRIMARY + IEF_FROM, "", ""},
	//{"Sender Name",																IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},
	//{"Sender Offline",															IEF_ARTIFACT_FACEBOOK_CHAT, "", ""},

	//{"Fragment",																	IEF_ARTIFACT_FACEBOOK_PAGES, "", ""},

	//{"Downloaded Receiver Image",											IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS, "", ""},
	//{"Downloaded Sender Image",												IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS, "", ""},
	{"Posted Date/Time - (UTC) (MM/dd/yyyy)",								IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Receiver ID",																IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS + IEF_PRIMARY + IEF_TO, "", ""},
	//{"Receiver Name",															IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS, "", ""},
	{"Sender ID",																	IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS + IEF_PRIMARY + IEF_FROM, "", ""},
	//{"Sender Name",																IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS, "", ""},
	{"Status Update / Wall Post / Comment",								IEF_ARTIFACT_FACEBOOK_STATUS_UPDATES_WALL_POSTS_COMMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_FACEBOOK_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Potential Activity",														IEF_ARTIFACT_FACEBOOK_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_FACEBOOK_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Allocated Area (Bytes)",												IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Bytes per sector",														IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Drive Type",																IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"File System",																IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Free Clusters",															IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Full Volume Serial Number",											IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Id",																			IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Sectors per cluster",													IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Total Capacity (Bytes)",												IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Total Clusters",															IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Total Sectors",															IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Unallocated Area (Bytes)",											IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Volume Name",																IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Volume Offset (Bytes)",												IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},
	//{"Volume Serial Number",													IEF_ARTIFACT_FILE_SYSTEM_INFORMATION, "", ""},

	//{"Bookmark Type",															IEF_ARTIFACT_FIREFOX_BOOKMARKS, "", ""},
	{"Date Added Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_BOOKMARKS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_FIREFOX_BOOKMARKS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Title",																		IEF_ARTIFACT_FIREFOX_BOOKMARKS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_FIREFOX_BOOKMARKS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Content Size (Bytes)",													IEF_ARTIFACT_FIREFOX_CACHE_RECORDS + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Content",																	IEF_ARTIFACT_FIREFOX_CACHE_RECORDS, "", ""},
	{"Date Created Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_CACHE_RECORDS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Image",																		IEF_ARTIFACT_FIREFOX_CACHE_RECORDS, "", ""},
	//{"MIME Type",																IEF_ARTIFACT_FIREFOX_CACHE_RECORDS, "", ""},
	{"URL",																			IEF_ARTIFACT_FIREFOX_CACHE_RECORDS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Accessed Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_FIREFOX_COOKIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_FIREFOX_COOKIES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Expiration Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_COOKIES + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Host",																			IEF_ARTIFACT_FIREFOX_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Name",																			IEF_ARTIFACT_FIREFOX_COOKIES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Path",																		IEF_ARTIFACT_FIREFOX_COOKIES, "", ""},
	//{"Value",																		IEF_ARTIFACT_FIREFOX_COOKIES, "", ""},

	//{"Bytes Downloaded",														IEF_ARTIFACT_FIREFOX_DOWNLOADS, "", ""},
	{"Download Source",															IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"End Date/Time - (UTC) (MM/dd/yyyy)",									IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"File Name",																IEF_ARTIFACT_FIREFOX_DOWNLOADS, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Referrer",																	IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Saved To",																	IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Start Date/Time - (UTC) (MM/dd/yyyy)",								IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Start Date/Time - (UTC) (MM/dd/yyyy)",								IEF_ARTIFACT_FIREFOX_DOWNLOADS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"State",																		IEF_ARTIFACT_FIREFOX_DOWNLOADS, "", ""},
	//{"Temp Path",																IEF_ARTIFACT_FIREFOX_DOWNLOADS, "", ""},

	//{"Icon",																		IEF_ARTIFACT_FIREFOX_FAVICONS, "", ""},
	//{"URL",																		IEF_ARTIFACT_FIREFOX_FAVICONS, "", ""},

	{"Field Name",																	IEF_ARTIFACT_FIREFOX_FORMHISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"First Used Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_FORMHISTORY + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"ID",																			IEF_ARTIFACT_FIREFOX_FORMHISTORY, "", ""},
	{"Last Used Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_FIREFOX_FORMHISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Times Used",																IEF_ARTIFACT_FIREFOX_FORMHISTORY, "", ""},
	{"Value",																		IEF_ARTIFACT_FIREFOX_FORMHISTORY + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"ID",																			IEF_ARTIFACT_FIREFOX_INPUT_HISTORY, "", ""},
	//{"Input",																		IEF_ARTIFACT_FIREFOX_INPUT_HISTORY, "", ""},
	//{"URL",																		IEF_ARTIFACT_FIREFOX_INPUT_HISTORY, "", ""},
	//{"Use Count",																IEF_ARTIFACT_FIREFOX_INPUT_HISTORY, "", ""},

	//{"Referrer URL",															IEF_ARTIFACT_FIREFOX_SESSIONSTORE_ARTIFACTS, "", ""},
	//{"Title",																		IEF_ARTIFACT_FIREFOX_SESSIONSTORE_ARTIFACTS, "", ""},
	//{"URL",																		IEF_ARTIFACT_FIREFOX_SESSIONSTORE_ARTIFACTS, "", ""},

	//{"Is Typed",																	IEF_ARTIFACT_FIREFOX_WEB_HISTORY, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_WEB_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Title",																		IEF_ARTIFACT_FIREFOX_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_FIREFOX_WEB_HISTORY + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_FIREFOX_WEB_HISTORY, "", ""},

	{"Date Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_FIREFOX_WEB_VISITS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Is Typed",																	IEF_ARTIFACT_FIREFOX_WEB_VISITS, "", ""},
	{"Title",																		IEF_ARTIFACT_FIREFOX_WEB_VISITS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Transition Type",														IEF_ARTIFACT_FIREFOX_WEB_VISITS, "", ""},
	{"URL",																			IEF_ARTIFACT_FIREFOX_WEB_VISITS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Content",																	IEF_ARTIFACT_FLASH_COOKIES, "", ""},
	//{"Cookie Name",																IEF_ARTIFACT_FLASH_COOKIES, "", ""},
	//{"Domain",																	IEF_ARTIFACT_FLASH_COOKIES, "", ""},

	{"2nd Most Recent Visit Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Creation Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Hits",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED, "", ""},
	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Most Recent Visit Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES_CARVED + IEF_PRIMARY + IEF_ATIME, "", ""},

	{"2nd Most Recent Visit Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Creation Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Hits",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES, "", ""},
	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Most Recent Visit Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_GOOGLE_ANALYTICS_FIRST_VISIT_COOKIES + IEF_PRIMARY + IEF_ATIME, "", ""},

	//{"Access Method",															IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED, "", ""},
	//{"Campaign",																	IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED, "", ""},
	//{"Cookie Source",															IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED, "", ""},
	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Keyword",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Last Update Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED + IEF_PRIMARY + IEF_MTIME, "", ""},
	//{"Path to Page",															IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES_CARVED, "", ""},

	//{"Access Method",															IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES, "", ""},
	//{"Cookie Source",															IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES, "", ""},
	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Keyword",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Updated Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_GOOGLE_ANALYTICS_REFERRAL_COOKIES + IEF_PRIMARY + IEF_MTIME, "", ""},

	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Outbound Link Events Left",											IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED, "", ""},
	//{"Page Views",																IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED, "", ""},
	{"Start Current Session Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES_CARVED + IEF_PRIMARY + IEF_BTIME, "", ""},

	{"Host",																			IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Outbound Link Events Left",											IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES, "", ""},
	//{"Page Views",																IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES, "", ""},
	{"Start Current Session Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_GOOGLE_ANALYTICS_SESSION_COOKIES + IEF_PRIMARY + IEF_BTIME, "", ""},

	//{"Host Name",																IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED, "", ""},
	//{"Page Requested",															IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED, "", ""},
	//{"Page Title",																IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED, "", ""},
	//{"Referrer URL",															IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED, "", ""},
	//{"URL",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS_CARVED, "", ""},

	//{"Host Name",																IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS, "", ""},
	//{"Page Requested",															IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS, "", ""},
	//{"Page Title",																IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS, "", ""},
	//{"Referrer URL",															IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS, "", ""},
	//{"URL",																		IEF_ARTIFACT_GOOGLE_ANALYTICS_URLS, "", ""},

	//{"Additional Address",													IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	//{"Business Latitude and Longitude",									IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	//{"Center of Map",															IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_GOOGLE_MAPS_QUERIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Destination Address",														IEF_ARTIFACT_GOOGLE_MAPS_QUERIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Route Type",																IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	{"Search Query",																IEF_ARTIFACT_GOOGLE_MAPS_QUERIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Source Address",															IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	//{"Starting Location",														IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},
	//{"Street View Latitude/Longitude",									IEF_ARTIFACT_GOOGLE_MAPS_QUERIES, "", ""},

	//{"Image",																		IEF_ARTIFACT_GOOGLE_MAPS_TILES, "", ""},
	//{"X Coordinate",															IEF_ARTIFACT_GOOGLE_MAPS_TILES, "", ""},
	//{"Y Coordinate",															IEF_ARTIFACT_GOOGLE_MAPS_TILES, "", ""},
	//{"Zoom Level",																IEF_ARTIFACT_GOOGLE_MAPS_TILES, "", ""},

	//{"Additional Address",													IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Business Latitude and Longitude",									IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Center of Map",															IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Destination Address",													IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Route Type",																IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Search Query",															IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Source Address",															IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Starting Location",														IEF_ARTIFACT_GOOGLE_MAPS, "", ""},
	//{"Street View Latitude/Longitude",									IEF_ARTIFACT_GOOGLE_MAPS, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_SECONDARY+ IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_GOOGLE_SEARCHES, "", ""},
	{"Original Search Query",													IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_SECONDARY + IEF_DETAIL2, "", ""},
	//{"Previous Queries",														IEF_ARTIFACT_GOOGLE_SEARCHES, "", ""},
	{"Search Session Start Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Search Term",																IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_PRIMARY +IEF_DETAIL2, "", ""},
	{"Web Page Title",															IEF_ARTIFACT_GOOGLE_SEARCHES + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Description",																IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"File Creation Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Local MAC Address",														IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS, "", ""},
	{"URL",																			IEF_ARTIFACT_IE_INPRIVATE_RECOVERY_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Column Name",																IEF_ARTIFACT_IDENTIFIERS, "", ""},
	//{"Identifier",																IEF_ARTIFACT_IDENTIFIERS, "", ""},

	{"Application Name",															IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Company",																	IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS, "", ""},
	{"Created Date",																IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Install Size (Bytes)",													IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Key Last Updated Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Potential Location",														IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Version",																	IEF_ARTIFACT_INSTALLED_MICROSOFT_PROGRAMS, "", ""},

	{"Application Name",															IEF_ARTIFACT_INSTALLED_PROGRAMS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Company",																	IEF_ARTIFACT_INSTALLED_PROGRAMS, "", ""},
	{"Created Date",																IEF_ARTIFACT_INSTALLED_PROGRAMS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Install Size (Bytes)",													IEF_ARTIFACT_INSTALLED_PROGRAMS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Key Last Updated Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_INSTALLED_PROGRAMS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Potential Location",														IEF_ARTIFACT_INSTALLED_PROGRAMS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Version",																	IEF_ARTIFACT_INSTALLED_PROGRAMS, "", ""},

	//{"Access Count",															IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT, "", ""},
	//{"Content",																	IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT, "", ""},
	{"Creation Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Filename",																	IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT, "", ""},
	//{"Image",																		IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT, "", ""},
	{"Last Modified by Web Server Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_CONTENT + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Access Count",															IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES, "", ""},
	{"Accessed Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Filename",																	IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Updated Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"User",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_COOKIES + IEF_PRIMARY + IEF_FROM, "", ""},

	//{"Access Count",															IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY, "", ""},
	{"Accessed Date/Time - Local Time (yyyy-mm-dd)",					IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"User",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DAILY_WEEKLY_HISTORY + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DEPENDENCY_ENTRIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DEPENDENCY_ENTRIES + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Download Location",														IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Redirect URL",																IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"Temporary Download Location",										IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_DOWNLOADS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Access Count",																IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY, "", ""},
	{"Accessed Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Page Title",																	IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"User",																			IEF_ARTIFACT_INTERNET_EXPLORER_10_11_MAIN_HISTORY, "", ""},

	//{"Cache Retrieval Count",												IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS, "", ""},
	{"Content Size (Bytes)",													IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Content",																	IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS, "", ""},
	//{"File Type",																IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS, "", ""},
	//{"Filename",																	IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS, "", ""},
	//{"Image",																		IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS, "", ""},
	{"Last Checked by Local Host Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified by Web Server Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"User",																			IEF_ARTIFACT_INTERNET_EXPLORER_CACHE_RECORDS + IEF_PRIMARY + IEF_FROM, "", ""},

	//{"Downloaded Icon",														IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES, "", ""},
	{"Favorite Name",																IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Favorites Root Location",												IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES, "", ""},
	//{"Folder Structure",														IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES, "", ""},
	//{"Icon URL",																	IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES, "", ""},
	{"Modified Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"User",																			IEF_ARTIFACT_INTERNET_EXPLORER_FAVORITES + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Last Entered Date/Time - (MM/dd/yyyy)",								IEF_ARTIFACT_INTERNET_EXPLORER_TYPED_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"URL",																			IEF_ARTIFACT_INTERNET_EXPLORER_TYPED_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"App ID",																	IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Arguments",																IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Data",																		IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Drive Type",																IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Entry ID",																	IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Jump List Type",															IEF_ARTIFACT_JUMP_LISTS, "", ""},
	{"Last Access Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_JUMP_LISTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Linked Path",																IEF_ARTIFACT_JUMP_LISTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"NetBIOS Name",															IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Pin Status",																IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Potential App Name",													IEF_ARTIFACT_JUMP_LISTS, "", ""},
	{"Source",																		IEF_ARTIFACT_JUMP_LISTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Target File Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_JUMP_LISTS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Target File Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_JUMP_LISTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Target File Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_JUMP_LISTS + IEF_SECONDARY + IEF_MTIME, "", ""},
	{"Target File Size (Bytes)",												IEF_ARTIFACT_JUMP_LISTS + IEF_SECONDARY + IEF_SIZE, "", ""},
	//{"Target MAC Address",													IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Target NetBIOS Name",													IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Volume Name",																IEF_ARTIFACT_JUMP_LISTS, "", ""},
	//{"Volume Serial Number",													IEF_ARTIFACT_JUMP_LISTS, "", ""},

	{"Search Term",																IEF_ARTIFACT_KEYWORD_SEARCHES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Searched Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_KEYWORD_SEARCHES + IEF_PRIMARY + IEF_ATIME, "", ""},

	{"Source",																		IEF_ARTIFACT_LNK_FILES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_LNK_FILES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_LNK_FILES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_LNK_FILES + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Arguments",																IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Drive Type",																IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Mac Address",																IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Net Bios Name",															IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Show Command",															IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Target Attributes",														IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Volume Name",																IEF_ARTIFACT_LNK_FILES, "", ""},
	//{"Volume Serial Number",													IEF_ARTIFACT_LNK_FILES, "", ""},
	{"Linked Path",																IEF_ARTIFACT_LNK_FILES + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Target File Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_LNK_FILES + IEF_SECONDARY + IEF_MTIME, "", ""},
	{"Target File Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_LNK_FILES + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Target File Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_LNK_FILES + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Target File Size (Bytes)",												IEF_ARTIFACT_LNK_FILES + IEF_SECONDARY + IEF_SIZE, "", ""},

	//{"Fragment",																	IEF_ARTIFACT_MSN_PLUS, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_MALWARE_PHISHING_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Site Name",																	IEF_ARTIFACT_MALWARE_PHISHING_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_MALWARE_PHISHING_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Adapter Name",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_DETAIL,  "", ""},
	//{"DHCP DNS Domain",														IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP DNS Server(s)",													IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP Default Gateway(s)",												IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP Enabled?",															IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP IPv4 Address",														IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP IPv4 Subnet Mask",												IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DHCP Server",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DNS Domain",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	//{"DNS Server(s)",															IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	{"Default Gateway(s)",														IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_TO,  "", ""},
	{"Description",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_DETAIL2,  "", ""},
	{"IPv4 Address",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_FROM,  "", ""},
	//{"IPv4 Subnet Mask",														IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},
	{"Lease Expires Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Lease Obtained Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"MAC Address",																IEF_ARTIFACT_NETWORK_INTERFACES_REGISTRY,  "", ""},

	//{"Authentication",															IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	{"Broadcast SSID",															IEF_ARTIFACT_NETWORK_PROFILES +  IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Connection Mode",														IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	//{"Connection Type",														IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	//{"DNS",																		IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	//{"Default Gateway MAC",													IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	//{"Encryption",																IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	{"Last Connected Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_NETWORK_PROFILES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Network Name (SSID)",														IEF_ARTIFACT_NETWORK_PROFILES + IEF_PRIMARY + IEF_FROM, "", ""},
	{"Password",																	IEF_ARTIFACT_NETWORK_PROFILES + IEF_PRIMARY + IEF_TO, "", ""},
	//{"Profile Created Date/Time - Local Time (yyyy-mm-dd)",		IEF_ARTIFACT_NETWORK_PROFILES, "", ""},
	{"Profile Name",																IEF_ARTIFACT_NETWORK_PROFILES + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Account",																	IEF_ARTIFACT_NETWORK_SHARE_INFORMATION, "", ""},
	//{"Connection Type",														IEF_ARTIFACT_NETWORK_SHARE_INFORMATION, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_NETWORK_SHARE_INFORMATION + IEF_PRIMARY + IEF_MTIME, "", ""},
	//{"Mapped Drive Letter",													IEF_ARTIFACT_NETWORK_SHARE_INFORMATION, "", ""},
	{"Network Name",																IEF_ARTIFACT_NETWORK_SHARE_INFORMATION + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Provider Name",																IEF_ARTIFACT_NETWORK_SHARE_INFORMATION + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Address Line 1",															IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"Address Line 2",															IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"City",																		IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"Company",																	IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"Country",																	IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	{"Date Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Email",																		IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Name",																			IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Number",																	IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"State",																		IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},
	//{"Zipcode",																	IEF_ARTIFACT_OPERA_AUTOFILL_PROFILES, "", ""},

	//{"Build Number",															IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Computer Name",															IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Displayed Computer Name",												IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	{"Installed/Updated Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Last Access Time Enabled",											IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Last Service Pack",														IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	{"Last Shutdown Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Operating System Version",												IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Operating System",															IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Organization",															IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Owner",																		IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Path",																		IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Product ID",																IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Product Key",																IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"System Root",																IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},
	//{"Version Number",															IEF_ARTIFACT_OPERATING_SYSTEM_INFORMATION, "", ""},

	{"Title",																		IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Subject",																		IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Authors",																		IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL3, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Saved Size (Bytes)",														IEF_ARTIFACT_PDF_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Authors",																	IEF_ARTIFACT_PDF_DOCUMENTS, "", ""},
	//{"File",																		IEF_ARTIFACT_PDF_DOCUMENTS, "", ""},
	//{"Keywords",																	IEF_ARTIFACT_PDF_DOCUMENTS, "", ""},
	{"Filename",																	IEF_ARTIFACT_PDF_DOCUMENTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"File System Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_PDF_DOCUMENTS + IEF_SECONDARY + IEF_MTIME, "", ""},
	{"File System Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_PDF_DOCUMENTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"File System Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_PDF_DOCUMENTS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_PDF_DOCUMENTS + IEF_SECONDARY + IEF_SIZE, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_SECONDARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_PARSED_SEARCH_QUERIES, "", ""},
	//{"Google Original Search Query",										IEF_ARTIFACT_PARSED_SEARCH_QUERIES, "", ""},
	{"Search Engine",																IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_SECONDARY + IEF_DETAIL2, "", ""},
	{"Search Term",																IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Web Page Title",															IEF_ARTIFACT_PARSED_SEARCH_QUERIES + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Category",																	IEF_ARTIFACT_PICTURES, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_PICTURES, "", ""},
	//{"File Extension",															IEF_ARTIFACT_PICTURES, "", ""},
	{"File Name",																	IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"GPS Latitude Reference",												IEF_ARTIFACT_PICTURES, "", ""},
	//{"GPS Latitude",															IEF_ARTIFACT_PICTURES, "", ""},
	//{"GPS Longitude Reference",												IEF_ARTIFACT_PICTURES, "", ""},
	//{"GPS Longitude",															IEF_ARTIFACT_PICTURES, "", ""},
	//{"Image",																		IEF_ARTIFACT_PICTURES, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"MD5 Hash",																	IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_HASH, "", ""},
	//{"Make",																		IEF_ARTIFACT_PICTURES, "", ""},
	//{"Model",																		IEF_ARTIFACT_PICTURES, "", ""},
	//{"Original Date/Time - Local Time (yyyy-mm-dd)",					IEF_ARTIFACT_PICTURES, "", ""},
	//{"Original Height",														IEF_ARTIFACT_PICTURES, "", ""},
	//{"Original Width",															IEF_ARTIFACT_PICTURES, "", ""},
	//{"PhotoDNA Hash",															IEF_ARTIFACT_PICTURES, "", ""},
	//{"Potential Original Media",											IEF_ARTIFACT_PICTURES, "", ""},
	//{"SHA1 Hash",																IEF_ARTIFACT_PICTURES, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_PICTURES + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Skin Tone Percentage",													IEF_ARTIFACT_PICTURES, "", ""},
	//{"Software",																	IEF_ARTIFACT_PICTURES, "", ""},
	//{"_externalFiles",															IEF_ARTIFACT_PICTURES, "", ""},
	//{"_rawData",																	IEF_ARTIFACT_PICTURES, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_PORNOGRAPHY_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Site Name",																	IEF_ARTIFACT_PORNOGRAPHY_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_PORNOGRAPHY_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"URL",																		IEF_ARTIFACT_POTENTIAL_BROWSER_ACTIVITY, "", ""},
	//{"User Agent",																IEF_ARTIFACT_POTENTIAL_BROWSER_ACTIVITY, "", ""},

	//{"Authors",																	IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	//{"Comments",																	IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	//{"Company",																	IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File System Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"File System Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"File System Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_SECONDARY + IEF_MTIME, "", ""},
	//{"File",																		IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	{"Filename",																	IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"Keywords",																	IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	//{"Last Author",																IEF_ARTIFACT_POWERPOINT_DOCUMENTS, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Last Printed Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Saved Size (Bytes)",														IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_SECONDARY + IEF_SIZE, "", ""},
	{"Subject",																		IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Title",																		IEF_ARTIFACT_POWERPOINT_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	//{"Message",																	IEF_ARTIFACT_QQ, "", ""},

	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_RTF_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"File Content",															IEF_ARTIFACT_RTF_DOCUMENTS, "", ""},
	{"File Size (Bytes)",														IEF_ARTIFACT_RTF_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Filename",																	IEF_ARTIFACT_RTF_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_RTF_DOCUMENTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_RTF_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},

	//{"Cache RowID",																IEF_ARTIFACT_REBUILT_WEBPAGES, "", ""},
	//{"Cache Table",																IEF_ARTIFACT_REBUILT_WEBPAGES, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_REBUILT_WEBPAGES + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Domain",																	IEF_ARTIFACT_REBUILT_WEBPAGES, "", ""},
	{"Page Title",																	IEF_ARTIFACT_REBUILT_WEBPAGES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_REBUILT_WEBPAGES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"_Rebuilt",																	IEF_ARTIFACT_REBUILT_WEBPAGES, "", ""},
	//{"_Report",																	IEF_ARTIFACT_REBUILT_WEBPAGES, "", ""},

	//{"Current Location",														IEF_ARTIFACT_RECYCLE_BIN, "", ""},
	{"Deleted Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_RECYCLE_BIN + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"File Name",																	IEF_ARTIFACT_RECYCLE_BIN + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"File Size (bytes)",														IEF_ARTIFACT_RECYCLE_BIN + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Original Path",																IEF_ARTIFACT_RECYCLE_BIN + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Type",																		IEF_ARTIFACT_RECYCLE_BIN, "", ""},
	//{"User Security Identifier",											IEF_ARTIFACT_RECYCLE_BIN, "", ""},

	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_SAFARI_HISTORY + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Visited Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_SAFARI_HISTORY + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Redirect URL",																IEF_ARTIFACT_SAFARI_HISTORY + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Title",																		IEF_ARTIFACT_SAFARI_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_SAFARI_HISTORY + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Visit Count",																IEF_ARTIFACT_SAFARI_HISTORY, "", ""},

	{"Chat Partner",																IEF_ARTIFACT_SECOND_LIFE + IEF_PRIMARY + IEF_TO, "", ""},
	{"From User",																	IEF_ARTIFACT_SECOND_LIFE + IEF_PRIMARY + IEF_FROM, "", ""},
	{"Message Sent Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_SECOND_LIFE + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Message",																		IEF_ARTIFACT_SECOND_LIFE + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"File System Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File System Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"File System Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Last Explored Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Mode",																			IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Path",																			IEF_ARTIFACT_SHELLBAGS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_SHIPPING_SITE_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Page Title",																	IEF_ARTIFACT_SHIPPING_SITE_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Tracking Number",														IEF_ARTIFACT_SHIPPING_SITE_URLS, "", ""},
	{"URL",																			IEF_ARTIFACT_SHIPPING_SITE_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_SOCIAL_MEDIA_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_SOCIAL_MEDIA_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_SOCIAL_MEDIA_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_SOCIAL_MEDIA_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_STARTUP_ITEMS + IEF_PRIMARY + IEF_MTIME, "", ""},
	//{"Path",																		IEF_ARTIFACT_STARTUP_ITEMS, "", ""},
	{"Program Name",																IEF_ARTIFACT_STARTUP_ITEMS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Type",																			IEF_ARTIFACT_STARTUP_ITEMS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_TAX_SITE_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_TAX_SITE_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_TAX_SITE_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_TAX_SITE_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_TEXT_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"File Content",															IEF_ARTIFACT_TEXT_DOCUMENTS, "", ""},
	{"Filename",																	IEF_ARTIFACT_TEXT_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_TEXT_DOCUMENTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_TEXT_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_TEXT_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},

	//{"Current Control Set",														IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Current Timezone Offset (Minutes)",									IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Daylight Timezone Name",													IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Daylight Timezone Offset (Minutes)",									IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Daylight Timezone Start Date/Time - Local Time (yyyy-mm-dd)",	IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Display",																		IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Failure Control Set",														IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Last Known Good Control Set",											IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Standard Timezone Name",													IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Standard Timezone Offset (Minutes)",									IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},
	//{"Standard Timezone Start Date/Time - Local Time (yyyy-mm-dd)",	IEF_ARTIFACT_TIMEZONE_INFORMATION, "", ""},

	//{"Bytes Uploaded",															IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS, "", ""},
	{"Download Completed Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"Download Location",														IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS, "", ""},
	{"Download Start Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Download URL",																IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Downloaded Bytes",															IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_MTIME, "", ""},
	//{"Potential App Name",													IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS, "", ""},
	{"Torrent Name",																IEF_ARTIFACT_TORRENT_ACTIVE_TRANSFERS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_TORRENT_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_TORRENT_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_TORRENT_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_TORRENT_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	//{"Associated User Accounts",											IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"Class",																		IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"Device Class ID",														IEF_ARTIFACT_USB_DEVICES, "", ""},
	{"Device Description",														IEF_ARTIFACT_USB_DEVICES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Device Description",														IEF_ARTIFACT_USB_DEVICES + IEF_SECONDARY + IEF_DETAIL2, "", ""},
	{"First Connect Since Reboot Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_USB_DEVICES + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"First Connected Date/Time - Local Time (yyyy-mm-dd)",		IEF_ARTIFACT_USB_DEVICES, "", ""},
	{"First Install Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_USB_DEVICES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Friendly Name",																IEF_ARTIFACT_USB_DEVICES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Friendly Name",																IEF_ARTIFACT_USB_DEVICES + IEF_SECONDARY + IEF_DETAIL, "", ""},
	{"Install Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_USB_DEVICES + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"Last Assigned Drive Letter",											IEF_ARTIFACT_USB_DEVICES, "", ""},
	{"Last Connected Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_USB_DEVICES + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"Last Insertion Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_USB_DEVICES + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Last Removal Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_USB_DEVICES + IEF_SECONDARY + IEF_CTIME, "", ""},
	//{"Manufacturer",															IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"Serial Number",															IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"VSN Decimal",																IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"VSN Hex",																	IEF_ARTIFACT_USB_DEVICES, "", ""},
	//{"Volume GUID",																IEF_ARTIFACT_USB_DEVICES, "", ""},

	{"Account Description",															IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Account Disabled",															IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	{"Full Name",																		IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"LM Hash",																		IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	{"Last Incorrect Password Login Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Last Login Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Password Change Date/Time - (UTC) (MM/dd/yyyy)",				IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	//{"Login Count",																	IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"Login Script",																IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"NTLM Hash",																	IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"Password Required",															IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"Profile Path",																IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"Security Identifier",														IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	//{"Type of User",																IEF_ARTIFACT_USER_ACCOUNTS, "", ""},
	{"User Group(s)",																	IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_TO, "", ""},
	{"User Name",																		IEF_ARTIFACT_USER_ACCOUNTS + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Application Run Count",													IEF_ARTIFACT_USERASSIST + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"File Name",																	IEF_ARTIFACT_USERASSIST + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Last Run Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_USERASSIST + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"User Name",																	IEF_ARTIFACT_USERASSIST + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_WEB_CHAT_URLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Date/Time - Local Time (yyyy-mm-dd)",								IEF_ARTIFACT_WEB_CHAT_URLS, "", ""},
	{"Site Name",																	IEF_ARTIFACT_WEB_CHAT_URLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"URL",																			IEF_ARTIFACT_WEB_CHAT_URLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},

	{"2nd Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME2, "", ""},
	{"3rd Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME3, "", ""},
	{"4th Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME4, "", ""},
	{"5th Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME5, "", ""},
	{"6th Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME6, "", ""},
	{"7th Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME7, "", ""},
	{"8th Last Run Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME8, "", ""},
	{"Application Name",															IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Application Run Count",													IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Last Run Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_WINDOWS_8_10_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME, "", ""},

	{"Computer",																	IEF_ARTIFACT_WINDOWS_EVENT_LOGS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_WINDOWS_EVENT_LOGS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Event Data",																IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},
	{"Event Description Summary",												IEF_ARTIFACT_WINDOWS_EVENT_LOGS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Event ID",																	IEF_ARTIFACT_WINDOWS_EVENT_LOGS + IEF_PRIMARY + IEF_FROM, "", ""},
	//{"Keywords",																	IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},
	//{"Level",																		IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},
	//{"Provider Name",															IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},
	//{"Security User ID",														IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},
	//{"Task Category",															IEF_ARTIFACT_WINDOWS_EVENT_LOGS, "", ""},

	{"Message Sent Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Message",																		IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Recipient",																	IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN + IEF_PRIMARY + IEF_TO, "", ""},
	{"Sender",																		IEF_ARTIFACT_WINDOWS_LIVE_MESSENGER_MSN + IEF_PRIMARY + IEF_FROM, "", ""},

	{"Application Name",															IEF_ARTIFACT_WINDOWS_PREFETCH_FILES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Application Run Count",													IEF_ARTIFACT_WINDOWS_PREFETCH_FILES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Last Run Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_WINDOWS_PREFETCH_FILES + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"OS Version",																IEF_ARTIFACT_WINDOWS_PREFETCH_FILES, "", ""},

	//{"Authors",																	IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	//{"Comments",																	IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	//{"Company",																	IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"File System Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_WORD_DOCUMENTS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"File System Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_WORD_DOCUMENTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"File System Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_WORD_DOCUMENTS + IEF_SECONDARY + IEF_MTIME, "", ""},
	//{"File",																		IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	{"Filename",																	IEF_ARTIFACT_WORD_DOCUMENTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"Keywords",																	IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	//{"Last Author",																IEF_ARTIFACT_WORD_DOCUMENTS, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Last Printed Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Saved Size (Bytes)",														IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_WORD_DOCUMENTS + IEF_SECONDARY + IEF_SIZE, "", ""},
	{"Subject",																		IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Title",																		IEF_ARTIFACT_WORD_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},

	{"Message Sent Date/Time - Local Time",								IEF_ARTIFACT_YAHOO_NON_ENCRYPTED_CHAT + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Sent Message Text (Recipient Unknown)",								IEF_ARTIFACT_YAHOO_NON_ENCRYPTED_CHAT + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"UserName",																	IEF_ARTIFACT_YAHOO_NON_ENCRYPTED_CHAT + IEF_PRIMARY + IEF_FROM, "", ""},

	//{"Attachment Local File Path",											IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	{"Attachment Name",															IEF_ARTIFACT_ZOOM_CHAT_MESSAGES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Attachment",																IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	//{"Buddy ID",																	IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	//{"Group Chat ID",															IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	{"Message Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_ZOOM_CHAT_MESSAGES + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Message ID",																IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	//{"Message Type",															IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	{"Message",																		IEF_ARTIFACT_ZOOM_CHAT_MESSAGES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Read",																		IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	//{"Sender Name",																IEF_ARTIFACT_ZOOM_CHAT_MESSAGES, "", ""},
	{"Sender",																		IEF_ARTIFACT_ZOOM_CHAT_MESSAGES + IEF_PRIMARY + IEF_FROM, "", ""},

	////{"Sender Name",																IEF_ARTIFACT_OUTLOOK_EMAILS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Recipients",																IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	////{"Subject",																		IEF_ARTIFACT_OUTLOOK_EMAILS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Sender Exchange Account",												IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	////{"Creation Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_OUTLOOK_EMAILS + IEF_PRIMARY + IEF_BTIME, "", ""},
	////{"Delivery Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_OUTLOOK_EMAILS + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"Body",																		IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	////{"Folder Name",																IEF_ARTIFACT_OUTLOOK_EMAILS + IEF_PRIMARY + IEF_DETAIL3, "", ""},
	//{"CC",																			IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"BCC",																		IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"Attachments",																IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"_attachmentData",														IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"Headers",																	IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"Priority",																	IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},
	//{"Importance",																IEF_ARTIFACT_OUTLOOK_EMAILS, "", ""},

	//{"Sender Name",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Sender Exchange Account",												IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Recipients",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	{"Subject",																		IEF_ARTIFACT_OUTLOOK_APPOINTMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Start Date/Time - (UTC) (MM/dd/yyyy)",								IEF_ARTIFACT_OUTLOOK_APPOINTMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"End Date/Time - (UTC) (MM/dd/yyyy)",									IEF_ARTIFACT_OUTLOOK_APPOINTMENTS + IEF_PRIMARY + IEF_CTIME, "", ""},
	//{"Body",																		IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"CC",																			IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"BCC",																		IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Companies",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Attachments",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"_attachmentData",														IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	{"Location",																	IEF_ARTIFACT_OUTLOOK_APPOINTMENTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Is All-day Event",														IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Is Recurring",															IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Recurrence Pattern Description",									IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Sensitivity",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Is Hidden",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},
	//{"Is Private",																IEF_ARTIFACT_OUTLOOK_APPOINTMENTS, "", ""},

	{"Display Name",																IEF_ARTIFACT_OUTLOOK_CONTACTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Customer ID",																IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	{"Email Address 1",															IEF_ARTIFACT_OUTLOOK_CONTACTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Email Display As 1",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_OUTLOOK_CONTACTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Company Name",																IEF_ARTIFACT_OUTLOOK_CONTACTS + IEF_PRIMARY + IEF_DETAIL3, "", ""},
	//{"Department Name",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Title",																		IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Profession",																IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Manager Name",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Office Location",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Business Address",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Business Phone",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Business Phone 2",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Business Fax",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Business Homepage",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Display Name 1",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Address 2",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Display As 2",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Display Name 2",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Address 3",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Display As 3",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Email Display Name 3",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Cellular Phone",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Home Address",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Home Phone",																IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Home Phone 2",															IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Home Fax",																	IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"FTP Site",																	IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Body",																		IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Attachments",																IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"_attachmentData",														IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},
	//{"Last Modifier Name",													IEF_ARTIFACT_OUTLOOK_CONTACTS, "", ""},

	//{"To", 																		IEF_ARTIFACT_EML_EMAILS, "", ""},
	////{"From", 																		IEF_ARTIFACT_EML_EMAILS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	////{"Date/Time - (UTC) (MM/dd/yyyy)", 										IEF_ARTIFACT_EML_EMAILS + IEF_PRIMARY + IEF_BTIME, "", ""},
	////{"Subject", 																	IEF_ARTIFACT_EML_EMAILS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	//{"Body", 																		IEF_ARTIFACT_EML_EMAILS, "", ""},
	//{"Cc", 																		IEF_ARTIFACT_EML_EMAILS, "", ""},
	//{"Bcc", 																		IEF_ARTIFACT_EML_EMAILS, "", ""},
	//{"Headers", 																	IEF_ARTIFACT_EML_EMAILS, "", ""},
	//{"Importance", 																IEF_ARTIFACT_EML_EMAILS, "", ""},
	////{"Last Read Date/Time - (UTC) (MM/dd/yyyy)", 						IEF_ARTIFACT_EML_EMAILS + IEF_PRIMARY + IEF_ATIME, "", ""},
	//{"Attachment Name(s)", 													IEF_ARTIFACT_EML_EMAILS, "", ""},
	//{"_attachmentData", 														IEF_ARTIFACT_EML_EMAILS, "", ""},

	{"Filename",																	IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Software",																	IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS + IEF_PRIMARY + IEF_CTIME, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_ENCRYPTION_ANTIFORENSIC_TOOLS + IEF_PRIMARY + IEF_MTIME, "", ""},

	{"Filename",																	IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_SECONDARY + IEF_DETAIL, "", ""},
	//{"File",																		IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},
	{"File System Last Modified Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_SECONDARY + IEF_MTIME, "", ""},
	{"File System Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",	IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_SECONDARY + IEF_ATIME, "", ""},
	{"File System Created Date/Time - (UTC) (MM/dd/yyyy)",			IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_SECONDARY + IEF_BTIME, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_SIZE, "", ""},
	{"Title",																		IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Saved Size (Bytes)",														IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_SECONDARY + IEF_SIZE, "", ""},
	{"Subject",																		IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Authors",																	IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},
	//{"Keywords",																	IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},
	//{"Comments",																	IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},
	//{"Last Author",																IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},
	{"Last Printed Date/Time - (UTC) (MM/dd/yyyy)",						IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Last Modified Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_MTIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_EXCEL_DOCUMENTS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"Company",																	IEF_ARTIFACT_EXCEL_DOCUMENTS, "", ""},

	{"File Name",																	IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Potential Profile ID or Picture ID",									IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Image",																		IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	{"Last Accessed Date/Time - (UTC) (MM/dd/yyyy)",					IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_ATIME, "", ""},
	{"Created Date/Time - (UTC) (MM/dd/yyyy)",							IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_BTIME, "", ""},
	{"Size (Bytes)",																IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_SIZE, "", ""},
	//{"Original Width",															IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	//{"Original Height",															IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	//{"Skin Tone Percentage",													IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	{"MD5 Hash",																	IEF_ARTIFACT_FACEBOOK_PICTURES + IEF_PRIMARY + IEF_HASH, "", ""},
	//{"SHA1 Hash",																	IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	//{"PhotoDNA Hash",																IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	//{"Category",																	IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},
	//{"_externalFiles",															IEF_ARTIFACT_FACEBOOK_PICTURES, "", ""},

	{"Sender Name",																IEF_ARTIFACT_YAHOO_EMAILS + IEF_PRIMARY + IEF_DETAIL3, "", ""},
	{"Sender Email",																IEF_ARTIFACT_YAHOO_EMAILS + IEF_PRIMARY + IEF_DETAIL2, "", ""},
	//{"Receiver Name",																IEF_ARTIFACT_YAHOO_EMAILS, "", ""},
	//{"Receiver Email",															IEF_ARTIFACT_YAHOO_EMAILS, "", ""},
	{"Subject",																		IEF_ARTIFACT_YAHOO_EMAILS + IEF_PRIMARY + IEF_DETAIL, "", ""},
	{"Date/Time - (UTC) (MM/dd/yyyy)",										IEF_ARTIFACT_YAHOO_EMAILS + IEF_PRIMARY + IEF_BTIME, "", ""},
	//{"HTML Fragment",																IEF_ARTIFACT_YAHOO_EMAILS, "", ""},
	{"Type",																			IEF_ARTIFACT_YAHOO_EMAILS + IEF_PRIMARY + IEF_TYPE, "", ""}
};

#endif //_IEFTYPES_H_

