#
#
#
#
#
NAME := mezza
VERSION := 0.2.0

OUTDIR := build
SRCDIRS  := base collections collections/TMap collections/TMapEntry dbase interfaces/dbase  str
SRCs   :=  $(wildcard *.c) $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJs   := $(patsubst %.c,$(OUTDIR)/%.o,$(SRCs))
OUTDIR.subdirs := $(addprefix $(OUTDIR)/,$(SRCDIRS))

PACKDIR := $(OUTDIR)/package
HEADERS := $(wildcard *.h) $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.h))
DESTHEADERS := $(patsubst %.h,$(PACKDIR)/include/$(NAME)/%.h,$(HEADERS))
DESTHEADERS.subdirs := $(addprefix $(PACKDIR)/include/$(NAME)/,$(SRCDIRS))
TGT.intalldir := $(PACKDIR)/lib

TGT    := $(OUTDIR)/libmezza.a
PREFIX := /usr/local
CFLAGS := -g -Wall -I../sqlite3

$(TGT): $(OUTDIR) $(OUTDIR.subdirs) $(OBJs)
	ar -cr $(TGT) $(OBJs)

release: clean
	make CFLAGS="-Wall -I../sqlite3" pack

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
	rm -fR $(OUTDIR)

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
	@echo "HEADERS = $(HEADERS)"
	@echo "DESTHEADERS = $(DESTHEADERS)"
	@echo "DESTHEADERS.subdirs = $(DESTHEADERS.subdirs)"

pack: $(TGT) $(DESTHEADERS.subdirs) $(TGT.intalldir)
	make $(DESTHEADERS)
	cp $(TGT) $(TGT.intalldir)
	cd $(PACKDIR) && tar -zcvf $(NAME)_$(VERSION).tar.gz include lib

$(PACKDIR)/include/$(NAME)/%.h: %.h
	cp $< $@

$(DESTHEADERS.subdirs) $(TGT.intalldir):
	mkdir -p $@
