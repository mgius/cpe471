GL=-lGL -lglut 

all: lab6

lab6: test lab6.o Vector3D.o
	g++ $(GL) -lm -o lab6 lab6.o Vector3D.o

test: Vector3D.o VectorTests.o
	g++ -lm -o test Vector3D.o VectorTests.o
	@echo "Running Vector Tests"
	@./test

clean:
	rm -f lab6 test *.o
