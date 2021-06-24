CLANG=../llvm-mos/build/bin/clang --config ../llvm-mos-sdk/build/atari/800xl.cfg -O2
all:	a.out

create_ferris: create_ferris.c
	gcc create_ferris.c -o create_ferris

ferris.c: create_ferris
	./create_ferris > ferris.c

a.out:	dlist.s ferris.c main.c
	${CLANG} main.c ferris.c dlist.s

a.elf:	ferris.c main.c dlist.s
	${CLANG} main.c dlist.s ferris.c -Wl,--oformat,elf -o a.elf

clean:
	rm -f a.out ferris.c create_ferris
