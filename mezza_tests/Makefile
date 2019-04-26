#
#
#
#
#

PROJDIR :=../..
OUTDIR := build
SRCs   :=  $(wildcard *.c) $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJs   := $(patsubst %.c,$(OUTDIR)/%.o,$(SRCs))

TGT    := $(OUTDIR)/mezza_tests.exec
PREFIX := /usr/local
CFLAGS := -g -Wall -Wno-unused-variable -Wno-unused-but-set-variable -I$(PROJDIR)/sqlite3 -I$(PROJDIR)
LDFLAGS := -L$(PROJDIR)/sqlite3/build -L$(PROJDIR)/mezza/build -lmezza -lsqlite3 -ldl -lpthread

$(TGT): $(OUTDIR) $(OBJs)
	gcc -o $(TGT) $(OBJs) $(LDFLAGS)

run: $(TGT)
	LD_LIBRARY_PATH=../sqlite3/build ./$(TGT)
	
$(OUTDIR)/%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
	
clean:
	rm -f $(TGT) $(OBJs)

install:
	./install.sh $(PREFIX)

$(OUTDIR):
	mkdir -p $@

info:
	@echo "OUTDIR = $(OUTDIR)"
	@echo "SRCs = $(SRCs)"
	@echo "OBJs = $(OBJs)"
	@echo "TGT = $(TGT)"
	@echo "PREFIX = $(PREFIX)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
