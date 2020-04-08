
CC=gcc

CFLAGS=-O3 -Wall -g -pg

all : gen_multibarchart multibarchart

/tmp/input_multibarchart.txt : gen_multibarchart
	@./$^ > $@.new
	mv $@.new $@

/tmp/output_multibarchart.png : RENDER_RES=1920x1080

/tmp/output_multibarchart.png : INPUT_MULTIBARCHART=/tmp/input_multibarchart.txt
#/tmp/output_multibarchart.png : INPUT_MULTIBARCHART=./input_multibarchart.txt

/tmp/output_multibarchart.png : multibarchart
	@./$^ $(RENDER_RES) < $(INPUT_MULTIBARCHART) | convert -size $(RENDER_RES) rgb:- -depth 32 $@

multibarchart : readfile.o writefile.o multibarchart.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

gen_multibarchart : readfile.o writefile.o gen_multibarchart.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

.PHONY:
clean :
	@rm *.o gen_multibarchart multibarchart
