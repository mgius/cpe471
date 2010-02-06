VPATH = Vector3D
GL=-lGL -lglut
HANDINFILES=*.cpp Vector3D/* Makefile README
TARFILE=mgius_program2.tar
HANDINDIR=csc471p2

all: program2

program2: program2.o Vector3D.o
	g++ -o prog2 $(GL) program2.o Vector3D.o

mkpackage: program2
	tar -cf $(TARFILE) $(HANDINFILES)

handin: mkpackage
	handin zwood $(HANDINDIR) $(TARFILE)


test: Vector3D.o VectorTests.o
	g++ $(GL) -lm -o test Vector3D.o VectorTests.o
	@echo "Running Vector Tests"
	@./test

clean:
	rm -f *.o prog2 $(TARFILE) test
