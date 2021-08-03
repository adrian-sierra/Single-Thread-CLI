all: main.out

%.o: %.c command.h string_parser.h
	gcc -c -o $@ $<

main.out: main.o command.o string_parser.o
	gcc -o main.out main.o command.o string_parser.o
