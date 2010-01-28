GL=-lGL -lglut 

all: lab6

lab6: test lab6.o Vector3D/Vector3D.o
	g++ $(GL) -lm -o lab6 lab6.o Vector3D/Vector3D.o

test: Vector3D/Vector3D.o Vector3D/VectorTests.o
	g++ -lm -o test Vector3D/Vector3D.o Vector3D/VectorTests.o
	@echo "Running Vector Tests"
	@./test

clean:
	rm -f lab6 test *.o **/*.o
