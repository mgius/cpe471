VPATH = Vector3D
GL=-lGL -lglut
HANDINFILES=*.cpp Vector3D/* Makefile README
HANDINDIR=csc471p2
PROGRAMNAME=program3
TARFILE=mgius_$(PROGRAMNAME).tar

all: $(PROGRAMNAME)

$(PROGRAMNAME): $(PROGRAMNAME).o Vector3D.o
	g++ -o $(PROGRAMNAME) $(GL) $(PROGRAMNAME).o Vector3D.o

mkpackage: $(PROGRAMNAME)
	tar -cf $(TARFILE) $(HANDINFILES)

handin: mkpackage
	handin zwood $(HANDINDIR) $(TARFILE)


test: Vector3D.o VectorTests.o
	g++ $(GL) -lm -o test Vector3D.o VectorTests.o
	@echo "Running Vector Tests"
	@./test

clean:
	rm -f *.o $(PROGRAMNAME) $(TARFILE) test
