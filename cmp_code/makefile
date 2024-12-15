cmp: clean cmp.o
	g++ -o cmp cmp.o -lsqlite3

clean:
	rm -f *.o *.out cmp

cmp.o: cmp.cpp Config.h Database.h
	g++ -c -g cmp.cpp