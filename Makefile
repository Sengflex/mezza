NAME:=mezza

# Tools config
CC:=gcc
RM:=del /f /q
AR:=ar -cr
MAKE:=mingw32-make

# Flags config
DFLAGS:=-Wall -g03 -I../sqlite3
CFLAGS:=

# Base targets
SRCS:=
PREFIX:=
SUFFIX:=
EXT:=
TGT:=$(PREFIX)$(NAME)$(SUFFIX).$(EXT)
COMMAND:=
OBJS:=$(patsubst %.c,%$(SUFFIX).o,$(SRCS))

$(TGT): $(OBJS)
	$(COMMAND) $(COMMAND_OPT) $(TGT) $(OBJS) $(COMMAND_POS_OPT)

run:
	./$(TGT)

%$(SUFFIX).o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(TGT)

# Static Lib Target
STCLIB_SRCS:= base\TMemMgr.c base\Exceptions.c base\TList.c base\TObject.c \
				str\TString.c str\TStringVector.c str\StringBuilder.c str\StringUtils.c \
				collections\Map.c collections\TokenSet.c \
				dbase\TSQLite3Conn.c dbase\TDBField.c dbase\TDBRow.c dbase\TDBTable.c \
				Conf.c FileUtil.c

StaticLib-Debug:
	$(MAKE) PREFIX="lib" SUFFIX="d" EXT="a" COMMAND="$(AR)" SRCS="$(STCLIB_SRCS)" CFLAGS:="-g03 -Wall -I../sqlite -Wno-unused-variable -Wno-unused-but-set-variable"

clean-StaticLib-Debug:
	$(MAKE) PREFIX="lib" SUFFIX="d" EXT="a" SRCS="$(STCLIB_SRCS)" clean

# Tests executable
TEST_SRCS:= _tests\main.c _tests\mem.c _tests\exceptions.c \
			_tests\objects.c _tests\list.c _tests\str.c \
			 _tests\file.c _tests\dbase.c _tests\collections.c \
			  _tests\conf.c

Tests: StaticLib-Debug
	$(MAKE) SUFFIX="_tests" EXT="exe" COMMAND="$(CC)" COMMAND_OPT="-o" COMMAND_POS_OPT="-L. -lmezzad -L../sqlite -lsqlite3" SRCS="$(TEST_SRCS)" CFLAGS:="-g03 -Wall -I.. -I../sqlite -Wno-unused-variable -Wno-unused-but-set-variable"
	$(MAKE) SUFFIX="_tests" EXT="exe" run

clean-Tests:
	$(MAKE) PREFIX="lib" SUFFIX="d" EXT="a" SRCS="$(STCLIB_SRCS)" clean

