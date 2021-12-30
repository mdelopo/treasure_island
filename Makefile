treasure_island: main.o maputils.o terminalutils.o
	gcc *.o -o treasure_island -lncurses

main.o: main.c
	gcc -c main.c

maputils.o: maputils.c maputils.h
	gcc -c maputils.c

terminalutils.o: terminalutils.c terminalutils.h
	gcc -c terminalutils.c

run:
	echo "make run"
	make treasure_island
	./treasure_island

clean:
	rm *.o treasure_island