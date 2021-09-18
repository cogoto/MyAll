##
## Makefile for "MyAll"
##

##--------##
## SYSTEM ##
##--------##

export MY_SYSTEM_ = linux

#export MY_SYSTEM_ = cygwin

#--------------------------#
# Parameters common to all #
#--------------------------#

#
# Select library for compiling sample programs.
#  0 ... no create.
#  1 ... use sources.
#  2 ... use static library.
#  3 ... use shared library.
#  4 ... use shared library at a current directory.
#
export LIB_SELECT_ = 1

#
# Clean all?
#  0 ... only "./*.o ./*~"
#  1 ... erase "./*.o ./*~ lib*.so *.sample.exe"
#
export CLEAN_COMPLETE_BEFORE_ = 1

#
# Compile programs of MyThread. (0 = No, 1 = Yes)
#
MAKE_MYTHREAD = 0


##=================================##

MYALL_DIR = $(CURDIR)

export MYALL_DIR_ = $(CURDIR)

##=================================##

MY_TARGET = MyChronos

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyComm

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyIO

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyLog

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyFFT

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyMsg

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyOption

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyRandom

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyShm

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyShmS

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MySignal

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MySparse

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyStatus

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyStrChg

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyString

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MySysCmd # Depending on gnu++11 in cygwin

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyTcp

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyTelnet

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyTime

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

MY_TARGET = MyUdp

.PHONY: $(MY_TARGET)

$(MY_TARGET):
	cd $@ && make all

##----##

##===========#

MY_TARGET = MyThread_TcpMediationServer

.PHONY: $(MY_TARGET)

$(MY_TARGET):

ifeq ($(MAKE_MYTHREAD),1)
	cd MyThread/$@ && make all
endif

##----##

MY_TARGET = MyThread_TcpCmdServer

.PHONY: $(MY_TARGET)

$(MY_TARGET):

ifeq ($(MAKE_MYTHREAD),1)
	cd MyThread/$@ && make all
endif

##----##

##===========#

.PHONY: all clean primary secondary third MyThread

##===========##

clean:

	rm -f cpp/*.cpp
	rm -f lib/lib*.so
	rm -f include/*.hpp

ifeq ($(CLEAN_COMPLETE_BEFORE_),1)
	rm -f lib/lib*
endif

ifdef $(MY_SYSTEM_)
	unset MY_SYSTEM_
endif

ifdef $(MY_CXX_)
	unset MY_CXX_
endif

ifdef $(MY_CXXFLAGS_)
	unset MY_CXXFLAGS_
endif

ifdef $(MY_LIBS_)
	unset MY_LIBS_
endif

ifdef $(LIB_SELECT_)
	unset LIB_SELECT_
endif

ifdef $(CLEAN_COMPLETE_BEFORE_)
	unset CLEAN_COMPLETE_BEFORE_
endif

##===========##

preparing:

	mkdir -p cpp/
	mkdir -p lib/
	mkdir -p include/

##----##

info_before:
	$(info ###)
	$(info ### Starting to create MyAll class librallies.)
	$(info ###)

##----##

info_after:
	$(info ###)
	$(info ###-------------------------------------------------------------------------------)
	$(info ###)
	$(info ### When you use MyAll libraries, include header files in the source.)
	$(info ###)
	$(info ###  ex. > #include "My*.hpp")
	$(info ###)
	$(info ### And then, after you set the PATH to MyAll libraries,)
	$(info ### you could compile :)
	$(info ###)
	$(info ###  g++ [source] -lMy* -std=c++11 ...)
	$(info ###)
	$(info ### If you did not set the PATH, you should compile with options :)
	$(info ###)
	$(info ###  g++ [source] -lMy* -L$(MYALL_DIR)/lib/ -I$(MYALL_DIR)/include/ -std=c++11 ... )
	$(info ###)
	$(info ###   or)
	$(info ###)
	$(info ###  g++ [source] [MyAll libs/srcs] -I$(MYALL_DIR)/include/ -std=c++11 ... )
	$(info ###)
	$(info ###-------------------------------------------------------------------------------)
	$(info )
	$(info ###)
	$(info ###  Created all the MyAll class libraries. Be enjoy!)
	$(info ###)
	$(info )

##----##

##
## clasification considering dependence.
##
## primary   : no dependence of other MyAll libraries.
## secondary : depending on primary.
## third     : ...
##

primary: MyFFT MyMsg MyOption MyRandom MySignal MyString MySysCmd MyTcp MyUdp

##----##

secondary: MyChronos MyIO MyTelnet

##----##

third: MyLog MyShmS

##----##

fourth: MyStatus

##----##

#calc: MySparse

##----##

#MyThread: MyThread_TcpMediationServer MyThread_TcpCmdServer

##----##

all: info_before clean preparing primary secondary third fourth clean info_after

#all: info_before clean preparing primary secondary third fourth calc MyThread clean info_after

##----##
