multi2mactime
==========
This utility was designed to understand/process several types of logs into a format compatible with [The SleuthKit's](https://github.com/sleuthkit/sleuthkit) 'mactime' such that filesystem timeline data and network activity timeline data can be viewed together. This utility was created to be used with my [datatime](https://github.com/mkucenski/datatime) alternative to mactime.

NOTE: This utility should be updated to support "plug-in" definitions of logs, instead of hard-coding. It was originally written to specifally support only a limited number of logs.

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

Build Instructions
------------------
./bootstrap.sh
mkdir build && cd build
../configure --prefix=...
make

Notes
-----
```
Sleuthkit TSK3.x body format
0		|1			|2			|3			|4		|5		|6		|7			|8			|9			|10
MD5	|NAME		|INODE	|PERMS	|UID	|GID	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME

multi2mactime
HASH	|DETAIL	|TYPE		|LOG-SRC	|FROM	|TO	|SIZE	|ATIME	|MTIME	|CTIME	|CRTIME
```

