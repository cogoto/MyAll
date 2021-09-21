# MyAll

[MyAll](https://cogoto.github.io/MyAll/annotated.html) is class libraries for programming c++ more easily (personally).

| class | brief | related class of MyAll | header file |
|:---|:---|:---|:---|
| [MyChronos](https://cogoto.github.io/MyAll/classMyAll_1_1MyChronos.html)   | Deals with time. | MyString | [MyChronos.hpp](https://cogoto.github.io/MyAll/MyChronos_8hpp_source.html) |
| [MyFFT](https://cogoto.github.io/MyAll/classMyAll_1_1MyFFT.html)       | Calculates FFT(Fast Fourier Transform). | - | [MyFFT.hpp](https://cogoto.github.io/MyAll/MyFFT_8hpp_source.html) |
| [MyI](https://cogoto.github.io/MyAll/classMyAll_1_1MyI.html)         | Inputs data from the file. | MyString | [MyIO.hpp](https://cogoto.github.io/MyAll/MyIO_8hpp_source.html) |
| [MyO](https://cogoto.github.io/MyAll/classMyAll_1_1MyO.html)         | Outputs data into the file. | - | [MyIO.hpp](https://cogoto.github.io/MyAll/MyIO_8hpp_source.html) |
| [MyLog](https://cogoto.github.io/MyAll/classMyAll_1_1MyLog.html)       | Logging, printing and debugging status in your software. | MyO, MyChronos, MyString | [MyLog.hpp](https://cogoto.github.io/MyAll/MyLog_8hpp_source.html) |
| [MyMsg](https://cogoto.github.io/MyAll/classMyAll_1_1MyMsg.html)       | Deals with message in a unified manner(maybe, need not to use). | - | [MyMsg.hpp](https://cogoto.github.io/MyAll/MyMsg_8hpp_source.html) |
| [MyMsgS](https://cogoto.github.io/MyAll/classMyAll_1_1MyMsgS.html)      | Deals with messages in a unified manner. | - | [MyMsg.hpp](https://cogoto.github.io/MyAll/MyMsg_8hpp_source.html) |
| [MyOption](https://cogoto.github.io/MyAll/classMyAll_1_1MyOption.html)    | Deals with options and arguments of the command line. | - | [MyOption.hpp](https://cogoto.github.io/MyAll/MyOption_8hpp_source.html) |
| [MyRandom](https://cogoto.github.io/MyAll/classMyAll_1_1MyRandom.html)    | creates and sorts random number arrays. | - | [MyRandom.hpp](https://cogoto.github.io/MyAll/MyRandom_8hpp_source.html) |
| [MyShmS](https://cogoto.github.io/MyAll/classMyAll_1_1MyShmS.html)      | Deals with shared memories. | MyI, MyO, MyString | [MyShmS.hpp](https://cogoto.github.io/MyAll/MyShmS_8hpp_source.html) |
| [MySignal](https://cogoto.github.io/MyAll/classMyAll_1_1MySignal.html)    | Deals with system signals. | - | [MySignal.hpp](https://cogoto.github.io/MyAll/MySignal_8hpp_source.html) |
| [MyStatus](https://cogoto.github.io/MyAll/classMyAll_1_1MyStatus.html)    | Manages your software status. | MyI, MyO, MyShmS, MyString | [MyStatus.hpp](https://cogoto.github.io/MyAll/MyStatus_8hpp_source.html) |
| [MyString](https://cogoto.github.io/MyAll/classMyAll_1_1MyString.html)    | Manipulates strings. | - | [MyString.hpp](https://cogoto.github.io/MyAll/MyString_8hpp_source.html) |
| [MySysCmd](https://cogoto.github.io/MyAll/classMyAll_1_1MySysCmd.html)    | Executes system commands. | - | [MySysCmd.hpp](https://cogoto.github.io/MyAll/MySysCmd_8hpp_source.html) |
| [MyTcpClient](https://cogoto.github.io/MyAll/classMyAll_1_1MyTcpClient.html) | Deals with connection as a TCP client. | MyTcpMaster | [MyTcp.hpp](https://cogoto.github.io/MyAll/MyTcp_8hpp_source.html) |
| [MyTcpServer](https://cogoto.github.io/MyAll/classMyAll_1_1MyTcpServer.html) | Deals with connection as a TCP server. | MyTcpMaster | [MyTcp.hpp](https://cogoto.github.io/MyAll/MyTcp_8hpp_source.html) |
| [MyTelnet](https://cogoto.github.io/MyAll/classMyAll_1_1MyTelnet.html)    | Deals with telnet connection. | MyString, MyTcpClient, MyTcpMaster | [MyTelnet.hpp](https://cogoto.github.io/MyAll/MyTelnet_8hpp_source.html) |
| [MyUdpComm](https://cogoto.github.io/MyAll/classMyAll_1_1MyUdpComm.html)   | Deals with UDP communications. | MyUdpMaster | [MyUdp.hpp](https://cogoto.github.io/MyAll/MyUdp_8hpp_source.html) |
| [MyUdpServer](https://cogoto.github.io/MyAll/classMyAll_1_1MyUdpServer.html) | Deals with connection as a UDP server. | MyUdpMaster | [MyUdp.hpp](https://cogoto.github.io/MyAll/MyUdp_8hpp_source.html) |

# Concepts

Because I am a lazy person,
[MyAll](https://cogoto.github.io/MyAll/namespaceMyAll.html) libraries are created with the aim of making c++ coding as easily as Python.

* Easy and intuitive class(member functions).
* Using only standard library (like c++11) internally.
* Working reasonably fast, if not 100% of c++.
* Readable code, this makes it easy to improve later.

# DEMO

A client with TCP connection is written as below,

```
#include "MyTcp.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {

    MyAll::MyTcpClient client;

    client.Init("127.0.0.1", 2021);  // IP address, port No.

    client.Send("Hello");

    string buff;

    if (client.Recv(buff) > 0) { cout << buff << endl; }

};
```

# Installation

## For UNIX(Linux, WSL etc.) systems

Please change to install directory, command as below,

```
git clone https://github.com/cogoto/MyAll.git
```

Alternatively, please download the zip file and unzip to the install directory.

Next, "make" operation is required like below,

```
cd MyAll
make all
```

After waiting for a while, the compilation will success with the string "Be enjoy!".
It will be created shared and static libraries,

| type | library |
|:---|:---|
| static library | MyAll/lib/libMyAll.a |
| shared library | MyAll/My*/libMy*.so (ex. MyAll/MyTcp/libMyTcp.so) |

and it will be also created sample programs.
(*.exe; of course, I know this name format is irregular for Linux manner. So please rename it if you want to.)

I recommend to make a PATH for convenient use.

First, the MyAll directory should be linked to the appropriate directory(ex. /usr/local/lib/).

```
cd /usr/local/lib/
ln -s [path of MyAll directory]
```

Second, please add it to the tail of ".bash_profile" like below,

```
MyAllDir=/usr/local/lib/MyAll

export CPATH=$MyLibDir/include:$CPATH
export CPLUS_INCLUDE_PATH=$MyAllDir/include:$CPLUS_INCLUDE_PATH:
export CPLUS_LIBRARY_PATH=$MyAllDir/lib:$CPLUS_LIBRARY_PATH
export LIBRARY_PATH=$MyAllDir/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=$MyAllDir/lib:$LD_LIBRARY_PATH
```

If you want this setting to reflect immediately, please executes

```
source ~/.bash_profile
```

## For others

I am preparing to compile with "cmake". Please wait...

# Usage

Please see "My*.sample.cpp" in each directory.
Although they are not perfect, I think they are guidelines.
(Of course, I will improve these samples in the future.)

# Requirement

* g++, clang ...
* c++11

# Version

2021/09/18 Ver. 0.1

# Note

These codes are currently available, but they are under development.  
Please your opinions!
(It may be slow to respond...)

# Author

* **cogoto**  (Email : cogoto.ogoto _ _atmark_ _ gmail.com)

# License

This software is released under the MIT License(https://en.wikipedia.org/wiki/MIT_License),
see "LICENSE".
