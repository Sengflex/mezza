name:=mezza

cc:=gcc -c
rm:=del /f /q
ar:=ar -cr
MAKE:=mingw32-make

dflags:=-Wall -g03 -I../sqlite3
rflags:=-I../sqlite3

cflags:=


srcs:= $(wildcard *.c) \
	$(wildcard base/*.c) \
	$(wildcard collections/*.c) \
	$(wildcard dbase/*.c) \
	$(wildcard str/*.c)

objs:=$(patsubst %.c,%.o,$(srcs))

stclib:=lib$(name).a
stclib_tests:=$(name).exe

stclib-release: clean
	$(MAKE) cflags="$(rflags)" $(stclib)

stclib-debug:
	$(MAKE) cflags="$(dflags)" $(stclib)

test-stclib:

$(stclib): $(objs)
	$(ar) $@ $(objs)

%.o: %.c
	$(cc) -o $@ $< $(cflags)
base\%.o: base\%.c
	$(cc) -o $@ $< $(cflags)
collections\%.o: collections\%.c
	$(cc) -o $@ $< $(cflags)
dbase\%.o: dbase\%.c
	$(cc) -o $@ $< $(cflags)
str\%.o: str\%.c
	$(cc) -o $@ $< $(cflags)

clean:
	$(rm) $(objs)