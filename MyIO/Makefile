##
## Makefile for "MyAll"
##

##-- SYSTEM --##

SYSTEM      = linux

#SYSTEM      = cygwin

##-- Setting for Linux --##

TARGET      = MyIO

MAIN        = $(TARGET).cpp

USE_MYALL   = ../MyString/MyString.cpp

MYALL_OPT   = -I../include

SAMPLE      = $(TARGET).sample.cpp

CXX         = g++

CXXFLAGS    = -std=c++11 -fPIC -O2 -Wall $(MYALL_OPT)

UNIQUE_LIBS =

#BOOST_LIBS  = -lboost_regex -L/usr/local/boost/lib/

# Select library for compiling sample programs.
#  0 ... no create.
#  1 ... use sources.
#  2 ... use static library.
#  3 ... use shared library.
#  4 ... use shared library at a current directory.
#
LIB_SELECT  = 1

# Clean all?
#  0 ... only "./*.o ./*~"
#  1 ... erase "./*.o ./*~ lib*.so *.sample.exe"
#
CLEAN_COMPLETE_BEFORE = 1


##-- Replacing parameters with Enviroment valiables --##

ifneq ($(MY_CXX_),)
	CXX = $(MY_CXX_)
endif

ifneq ($(MY_SYSTEM_),)
	SYSTEM = $(MY_SYSTEM_)
endif

ifneq ($(MY_CXXFLAGS_),)
	CXXFLAGS = $(MY_CXXFLAGS_)
endif

ifneq ($(LIB_SELECT_),)
	LIB_SELECT = $(LIB_SELECT_)
endif

ifneq ($(CLEAN_COMPLETE_BEFORE_),)
	CLEAN_COMPLETE_BEFORE = $(CLEAN_COMPLETE_BEFORE_)
endif

ifneq ($(MYALL_DIR_),)
ifneq ($(USE_MYALL),)
	#MYALL_DIR = $(MYALL_DIR_)
	#MYALL_OPT = -L$(MYALL_DIR_)/lib -I$(MYALL_DIR_)/include
	#MYALL_OPT = -I$(MYALL_DIR_)/include
	MYALL_OPT = -I../include
endif
endif

##---- Setting for Cygwin ----##

ifeq ($(SYSTEM),cygwin)
	#CXXFLAGS   := $(CXXFLAGS:c++11=gnu++11)
	#UNIQUE_LIBS = -lboost_regex
	#UNIQUE_LIBS = -lboost_regex-mt # cygwin(old)
endif


##=============##
##=============##
##=============##

.PHONY: install sample clean cleanall info all

MAIN_OBJS   := $(MAIN:.cpp=.o) #MAINの各ファイルのサフィックスの.cppを.oに変換
SHARED_MAIN := lib$(MAIN:.cpp=.so)
EXE_SAMPLE  := $(SAMPLE:.cpp=.exe)

##===========##

$(TARGET): $(MAIN_OBJS)
	$(CXX) $(MAIN) $(USE_MYALL) $(CXXFLAGS) $(UNIQUE_LIBS) $(BOOST_LIBS) -o $(SHARED_MAIN) -shared # 共有(動的)ライブラリ
	ar rcs ../lib/libMyAll.a $(MAIN_OBJS) # 静的ライブラリ

##----##

install:
	ln -sf "$(CURDIR)/$(TARGET).hpp" ../include/
	ln -sf "$(CURDIR)/$(TARGET).cpp" ../cpp/
	ln -sf "$(CURDIR)/lib$(TARGET).so"  ../lib/

##----##

sample:

ifeq ($(LIB_SELECT),1)
	$(CXX) $(SAMPLE) $(MAIN) $(USE_MYALL) $(CXXFLAGS) $(UNIQUE_LIBS) $(BOOST_LIBS) -o $(EXE_SAMPLE) # カレントディレクトリのものを使用．
endif

ifeq ($(LIB_SELECT),2)
	$(CXX) $(SAMPLE) -lMyAll              $(CXXFLAGS) $(UNIQUE_LIBS) $(BOOST_LIBS) -o $(EXE_SAMPLE) -static # 静的ライブラリを使用．
endif

ifeq ($(LIB_SELECT),3)
	$(CXX) $(SAMPLE) -l$(TARGET)          $(CXXFLAGS) $(UNIQUE_LIBS) $(BOOST_LIBS) -o $(EXE_SAMPLE) # 共有ライブラリを使用．共有ライブラリは既に LIBS へリンクされている．
endif

ifeq ($(LIB_SELECT),4)
	$(CXX) $(SAMPLE) $(SHARED_MAIN)       $(CXXFLAGS) $(UNIQUE_LIBS) $(BOOST_LIBS) -o $(EXE_SAMPLE) # カレントディレクトリのヘッダ・共有ライブラリを使用．
endif

##----##

clean:
	rm -f ./*.o ./*~

##----##

cleanall:

ifeq ($(CLEAN_COMPLETE_BEFORE),0)
	rm -f ./*.o ./*~
endif

ifeq ($(CLEAN_COMPLETE_BEFORE),1)
	rm -f ./*.o ./*~ lib$(TARGET).so $(EXE_SAMPLE)
endif

##----##

info:
	$(info ###)
	$(info ### TARGET      : $(TARGET))
	$(info ### SRCS        : $(MAIN))
	$(info ### USE_MYALL   : $(USE_MYALL))
	$(info ### MYALL_OPT   : $(MYALL_OPT))
	$(info ### SYSTEM      : $(SYSTEM))
	$(info ### CXX         : $(CXX))
	$(info ### CXXFLAGS    : $(CXXFLAGS))
	$(info ### UNIQUE_LIBS : $(UNIQUE_LIBS))
	$(info ### BOOST_LIBS  : $(BOOST_LIBS))
	$(info ###)

##----##

all: info cleanall $(TARGET) install sample clean
