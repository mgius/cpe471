GL=-lGL -lglut 
LAB=lab11
CXXFLAGS=-Wall

all: $(LAB)

$(LAB): $(LAB).o
	g++ $(GL) -lm -o $(LAB) $(LAB).o

clean:
	rm -f $(LAB) *.o
