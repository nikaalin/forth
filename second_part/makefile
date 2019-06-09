all: build

build: clean for.o
	ld -o for for.o

for.o: for.asm
	nasm -f elf64 -g -F dwarf for.asm -o for.o

clean:
	rm -f *.o
	rm -f for

run: build
	./for