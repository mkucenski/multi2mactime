# fw2mactime
Utility to massage firewall logs into The Sleuthkit's mactime/body format to correlate file records with firewall logs.

//Normal mactime for files
//"    |FILENAME | |INODE   | |PERMISSIONS| |UID       |GID       |       |SIZE     |ATIME  |MTIME  |CTIME    | | "

//Mactime for event records
//"REC#|EVENTFILE| |EVENTID | |SOURCE     | |SID       |COMPUTER  |       |TYPE     |       |WRITTEN|GENERATED| | "

//Mactime for firewall records
//"FWL |MESSAGE  | |MSG-TYPE| |SERVICE    | |SRCIP     |DESTIP    |RAWDATA|SENT/RECV|       |       |GENERATED| | "

