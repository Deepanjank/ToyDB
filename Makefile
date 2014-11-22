all: clean
	# cd pflayer && make
	g++ -c -x c pflayer/pf.c pflayer/buf.c pflayer/hash.c
	g++ -c main.cpp snapshot.cpp raid01.cpp  -I. -I./pflayer/
	g++ -o test main.o snapshot.o raid01.o pf.o buf.o hash.o

run:
	rm -f disk* snapshot
	./test

clean:
	rm -f *.o disk* snapshot
