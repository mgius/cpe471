VPATH = Vector3D
GL=-lGL -lglut
HANDINFILES=*.cpp Vector3D/* Makefile README
HANDINDIR=csc471p4
PROGRAMNAME=plinko
TARFILE=mgius_$(PROGRAMNAME).tar

all: $(PROGRAMNAME)

$(PROGRAMNAME): $(PROGRAMNAME).o Vector3D.o
	g++ -o $(PROGRAMNAME) $(GL) $(PROGRAMNAME).o Vector3D.o

mkpackage: $(PROGRAMNAME)
	tar -cf $(TARFILE) $(HANDINFILES)

handin: mkpackage
	handin zwood $(HANDINDIR) $(TARFILE)
	handin zwood $(HANDINDIR)

test:
	python Vector3D/VectorTests.py

clean:
	rm -f *.o $(PROGRAMNAME) $(TARFILE) test
	find . -iname '*.pyc' -exec rm {} \;
