GL=-lGL -lglut
PROVIDEDFILES=pixel.o readbmp.o savebmp.o vector.o image.o
HANDINFILES=*.cpp *.h images/created/*.jpg Makefile README

all: program1

program1: $(PROVIDEDFILES) main_release.o
	g++ -o prog1 $(GL) *.o

mkpackage: program1
	tar -cf mgius_program1.tar $(HANDINFILES)

clean:
	rm -f *.o prog1 mgius_program1.tar
