all: pnmdump.exe

pnmdump.exe: pnmdumpmain.c
	gcc -Wall -std=c99 pnmdumpmain.c -o pnmdump.exe
test: pnmdump.exe
	python tests/runtests-1.0.py ./pnmdump.exe