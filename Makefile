# Completati va rog urmatoarele 3 linii cu informatiile voastre personale.
# Daca aveti mai multe prenume, alegeti doar unul dintre ele.
NUME=Stefan
PRENUME=Bogdan
GRUPA=316CC
# ^ Doar cele trei de sus.

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lcurses

.PHONY: build run pack clean

build:
	$(CC) $(CFLAGS) src/*.c -o bin/Minesweeper $(LDFLAGS) 

run: build
	./bin/Minesweeper

pack: clean
	zip -r $(GRUPA)_$(NUME)_$(PRENUME)_tema3.zip logo.txt Makefile README src bin

clean:
	rm -rf bin/* *.zip
	clear

