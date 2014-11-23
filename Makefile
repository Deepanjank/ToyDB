all:
	# cd pflayer && make
	g++ -c -x c pflayer/pf.c pflayer/buf.c pflayer/hash.c
	g++ -c main.cpp snapshot.cpp raid01.cpp defs.cpp -I. -I./pflayer/
	g++ -o analyze main.o snapshot.o raid01.o defs.o pf.o buf.o hash.o

run:
	rm -f disk* snapshot

clean:
	rm -f analyze a.out *.o disk* snapshot
