VPATH = Vector3D
GL=-lGL -lglut
HANDINFILES=*.cpp *.h images/created/*.jpg Makefile README

all: program2

program2: program2.o Vector3D.o
	g++ -o prog2 $(GL) program2.o Vector3D.o

#mkpackage: program2
#	tar -cf mgius_program1.tar $(HANDINFILES)
#
#handin: mkpackage
#	handin zwood csc471p1 mgius_program1.tar
#

test: Vector3D.o VectorTests.o
	g++ $(GL) -lm -o test Vector3D.o VectorTests.o
	@echo "Running Vector Tests"
	@./test

clean:
	rm -f *.o prog2 mgius_program2.tar test
