CC=gcc
MKDIR=mkdir

CFLAGS=-W -Wall -D_GNU_SOURCE -g
INCPATH=-Isrc -Ilibs -I/usr/include/libxml2
LDFLAGS=-lxml2 -lssl libs/libutf8.a

# Testing environment
ifeq ($(shell getconf LONG_BIT),64)
	CFLAGS+=-DENVIRONMENT_64
else
	CFLAGS+=-DENVIRONMENT_32
endif

# Common objects
OBJS=src/spcdef.o \
	src/utils/array.o \
	src/utils/config.o \
	src/utils/cpu.o \
	src/utils/file.o \
	src/utils/mte.o \
	src/utils/pointers.o \
	src/utils/playlet.o \
	src/utils/ptrlist.o \
	src/utils/script.o \
	src/utils/spc_md5.o \
	src/utils/spc_utf8.o \
	src/utils/table.o \
	src/utils/tags.o \
	src/utils/unicode_converter.o \
	src/utils/util.o

# DECoder objects
SCRIPT_DEC_OBJS=src/script_dec/main.o \
	src/script_dec/script_decoding_cfg.o \
	src/script_dec/spc_mte.o \
	src/script_dec/xml_script_writer.o

# ENCoder objects
SCRIPT_ENC_OBJS=src/script_enc/main.o \
	src/script_enc/encoder.o \
	src/script_enc/script_encoding_cfg.o \
	src/script_enc/xml_script_reader.o

# EXPand objects
EXP_OBJS=src/expand/main.o

# PLAYLET_DECoder objects
PLAYLET_DEC_OBJS=src/playlet_dec/main.o \
	src/playlet_dec/playlet_decoding_cfg.o \
	src/playlet_dec/xml_playlet_writer.o

# PLAYLET_ENCoder objects
PLAYLET_ENC_OBJS=src/playlet_enc/main.o \
	src/playlet_enc/playlet_encoding_cfg.o \
	src/playlet_enc/xml_playlet_reader.o

all: script_dec script_enc expand playlet_dec playlet_enc

# expand
expand: $(OBJS) $(EXP_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/expand/main.o: src/expand/main.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

# playlet decoder
playlet_dec: $(OBJS) $(PLAYLET_DEC_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/playlet_dec/main.o: src/playlet_dec/main.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/playlet_dec/playlet_decoding_cfg.o: src/playlet_dec/playlet_decoding_cfg.c src/playlet_dec/playlet_decoding_cfg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/playlet_dec/xml_playlet_writer.o: src/playlet_dec/xml_playlet_writer.c src/playlet_dec/xml_playlet_writer.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

# playlet encoder
playlet_enc: $(OBJS) $(PLAYLET_ENC_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/playlet_enc/main.o: src/playlet_enc/main.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/playlet_enc/playlet_encoding_cfg.o: src/playlet_enc/playlet_encoding_cfg.c src/playlet_enc/playlet_encoding_cfg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/playlet_enc/xml_playlet_reader.o: src/playlet_enc/xml_playlet_reader.c src/playlet_enc/xml_playlet_reader.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

# script decoder
script_dec: $(OBJS) $(SCRIPT_DEC_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/script_dec/main.o: src/script_dec/main.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_dec/script_decoding_cfg.o: src/script_dec/script_decoding_cfg.c src/script_dec/script_decoding_cfg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_dec/spc_mte.o: src/script_dec/spc_mte.c src/script_dec/spc_mte.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_dec/xml_script_writer.o: src/script_dec/xml_script_writer.c src/script_dec/xml_script_writer.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

# script encoder
script_enc: $(OBJS) $(SCRIPT_ENC_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

src/script_enc/main.o: src/script_enc/main.c
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_enc/encoder.o: src/script_enc/encoder.c src/script_enc/encoder.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_enc/script_encoding_cfg.o: src/script_enc/script_encoding_cfg.c src/script_enc/script_encoding_cfg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/script_enc/xml_script_reader.o: src/script_enc/xml_script_reader.c src/script_enc/xml_script_reader.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

# common objs

src/spcdef.o: src/spcdef.c src/spcdef.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/array.o: src/utils/array.c src/utils/array.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/config.o: src/utils/config.c src/utils/config.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/cpu.o: src/utils/cpu.c src/utils/cpu.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/file.o: src/utils/file.c src/utils/file.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/mte.o: src/utils/mte.c src/utils/mte.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/playlet.o: src/utils/playlet.c src/utils/playlet.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/pointers.o: src/utils/pointers.c src/utils/pointers.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/ptrlist.o: src/utils/ptrlist.c src/utils/ptrlist.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/script.o: src/utils/script.c src/utils/script.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/spc_md5.o: src/utils/spc_md5.c src/utils/spc_md5.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/spc_utf8.o: src/utils/spc_utf8.c src/utils/spc_utf8.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/table.o: src/utils/table.c src/utils/table.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/tags.o: src/utils/tags.c src/utils/tags.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/unicode_converter.o: src/utils/unicode_converter.c src/utils/unicode_converter.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

src/utils/util.o: src/utils/util.c src/utils/util.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

clean:
	-rm -f $(OBJS) $(EXP_OBJS) $(PLAYLET_DEC_OBJS) $(PLAYLET_ENC_OBJS) $(SCRIPT_DEC_OBJS) $(SCRIPT_ENC_OBJS)

mrproper: clean
	-rm -f script_dec script_enc playlet_dec playlet_enc
