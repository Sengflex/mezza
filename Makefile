#
#
#
#
#

OUTDIR := build
SRCDIRS  := base collections collections/TMap collections/TMapEntry dbase interfaces/dbase  str
SRCs   :=  $(wildcard *.c) $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJs   := $(patsubst %.c,$(OUTDIR)/%.o,$(SRCs))
OUTDIR.subdirs := $(addprefix $(OUTDIR)/,$(SRCDIRS))

TGT    := $(OUTDIR)/libmezza.a
PREFIX := /usr/local
CFLAGS := -g -Wall -I../sqlite3

$(TGT): $(OUTDIR) $(OUTDIR.subdirs) $(OBJs)
	ar -cr $(TGT) $(OBJs)
	
$(OUTDIR)/%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
	
$(OUTDIR)/base/%.o: base/%.c
	gcc $(CFLAGS) -c -o $@ $<
	
$(OUTDIR)/collections/%.o: collections/%.c
	gcc $(CFLAGS) -c -o $@ $<
	
$(OUTDIR)/dbase/%.o: dbase/%.c
	gcc $(CFLAGS) -c -o $@ $<
	
$(OUTDIR)/interfaces/dbase/%.o: interfaces/dbase/%.c
	gcc $(CFLAGS) -c -o $@ $<
	
$(OUTDIR)/str/%.o: str/%.c
	gcc $(CFLAGS) -c -o $@ $<
		
collections-TMap-strvd:
	./generate.sh collections/TMap/strvd.c
	./generate.sh collections/TMap/strvd.h
		
collections-TMapEntry-strvd.c:
	./generate.sh collections/TMapEntry/strvd.c
	./generate.sh collections/TMapEntry/strvd.h
	
clean:
	rm -f $(TGT) $(OBJs)

install:
	./install.sh $(PREFIX)

$(OUTDIR.subdirs) $(OUTDIR):
	mkdir -p $@

info:
	@echo "OUTDIR = $(OUTDIR)"
	@echo "SRCDIRS = $(SRCDIRS)"
	@echo "SRCs = $(SRCs)"
	@echo "OBJs = $(OBJs)"
	@echo "OUTDIR.subdirs = $(OUTDIR.subdirs)"
	@echo "TGT = $(TGT)"
	@echo "PREFIX = $(PREFIX)"
	@echo "CFLAGS = $(CFLAGS)"
