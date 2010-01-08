GL=-lGL -lglut
PROVIDEDFILES=pixel.cpp readbmp.cpp savebmp.cpp vector.cpp image.cpp
all: program1

program1: provided
	g++ -o prog1 $(GL) main_release.cpp *.o

provided: $(PROVIDEDFILES)
	g++ -c $(PROVIDEDFILES)

clean:
	rm -f *.o prog1
