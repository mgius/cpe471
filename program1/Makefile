GL=-lGL -lglut
PROVIDEDFILES=pixel.o readbmp.o savebmp.o vector.o image.o
all: program1

program1: $(PROVIDEDFILES) main_release.o
	g++ -o prog1 $(GL) *.o

clean:
	rm -f *.o prog1
