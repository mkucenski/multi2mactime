fw2mactime
==========
This utility was designed to understand/process several types of firewall logs into a format compatible with [The SleuthKit's](https://github.com/sleuthkit/sleuthkit) 'mactime' such that filesystem timeline data and network activity timeline data can be viewed together. This utility was created to be used with my [datatime](https://github.com/mkucenski/datatime) alternative to mactime.

NOTE: While this code compiles, it was written several years ago based on an older version of The SleuthKit's "body" format. It needs to be udated based on the TSK3.x format.

NOTE: This utility should be updated to support "plug-in" definitions of firewall logs, instead of hard-coding. It was originally written to specifally support only a limited number of firewall logs.

License
-------
Copyright &copy; 2007 Matthew A. Kucenski

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Detailed Description
--------------------

TODO

Dependencies
------------
* [popt](http://www.freecode.com/projects/popt/)
* [Boost Date-Time](http://www.boost.org)
* [My libdelimText](https://github.com/mkucenski/libdelimText)
* [My libtimeUtils](https://github.com/mkucenski/libtimeUtils)

Notes
-----
```
Normal mactime for files
"    |FILENAME | |INODE   | |PERMISSIONS| |UID       |GID       |       |SIZE     |ATIME  |MTIME  |CTIME    | |

Mactime for event records
REC#|EVENTFILE| |EVENTID | |SOURCE     | |SID       |COMPUTER  |       |TYPE     |       |WRITTEN|GENERATED| |

Mactime for firewall records
FWL |MESSAGE  | |MSG-TYPE| |SERVICE    | |SRCIP     |DESTIP    |RAWDATA|SENT/RECV|       |       |GENERATED| |
```