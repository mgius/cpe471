GL=-lGL -lglut
PROVIDEDFILES=pixel.o readbmp.o savebmp.o vector.o image.o
all: program1

program1: $(PROVIDEDFILES) main_release.cpp
	g++ -o prog1 $(GL) main_release.cpp *.o

clean:
	rm -f *.o prog1
