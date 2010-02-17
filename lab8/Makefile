GL=-lGL -lglut 

all: lab8

lab8: MeshParser_release3.o
	g++ $(GL) -lm -o lab8 MeshParser_release3.o

clean:
	rm -f lab8 test *.o
