# MyAll

Class libraries for programming c++ more easily (personally).

| class | brief | related class of MyAll | header file |
|:---|:---|:---|:---|
| MyChronos   | Deals with time. | MyString | MyChronos.hpp |
| MyFFT       | Calculates FFT(Fast Fourier Transform). | - | MyFFT.hpp |
| MyI         | Inputs data from the file. | MyString | MyIO.hpp |
| MyO         | Outputs data into the file. | - | MyIO.hpp |
| MyLog       | Logging, printing and debugging status in your software. | MyO, MyChronos, MyString | MyLog.hpp |
| MyMsg       | Deals with message in a unified manner(maybe, need not to use). | - | MyMsg.hpp |
| MyMsgS      | Deals with messages in a unified manner. | - | MyMsg.hpp |
| MyOption    | Deals with options and arguments of the command line. | - | MyOption.hpp |
| MyRandom    | creates and sorts random number arrays. | - | MyRandom.hpp |
| MyShmS      | Deals with shared memories. | MyI, MyO, MyString | MyShmS.hpp |
| MySignal    | Deals with system signals. | - | MySignal.hpp |
| MyStatus    | Manages your software status. | MyI, MyO, MyShmS, MyString | MyStatus.hpp |
| MyString    | Manipulates strings. | - | MyString.hpp |
| MySysCmd    | Executes system commands. | - | MySysCmd.hpp |
| MyTcpClient | Deals with connection as a TCP client. | MyTcpMaster | MyTcp.hpp |
| MyTcpServer | Deals with connection as a TCP server. | MyTcpMaster | MyTcp.hpp |
| MyTelnet    | Deals with telnet connection. | MyString, MyTcpClient, MyTcpMaster | MyTelnet.hpp |
| MyUdpComm   | Deals with UDP communications. | MyUdpMaster | MyUdp.hpp |
| MyUdpServer | Deals with connection as a UDP server. | MyUdpMaster | MyUdp.hpp |

# Concepts

Because I am a lazy person,
these libraries are created with the aim of making c++ coding as easily as Python.

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
