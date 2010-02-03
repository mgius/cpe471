GL=-lGL -lglut
HANDINFILES=*.cpp *.h images/created/*.jpg Makefile README

all: program2

program2: program2.o
	g++ -o prog2 $(GL) *.o

#mkpackage: program2
#	tar -cf mgius_program1.tar $(HANDINFILES)
#
#handin: mkpackage
#	handin zwood csc471p1 mgius_program1.tar
#
clean:
	rm -f *.o prog2 mgius_program2.tar
