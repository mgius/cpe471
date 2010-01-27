GL=-lGL -lglut 

all: simple_release

simple_release: simple_release.cpp
	g++ $(GL) -lm -o simple_release simple_release.cpp

clean:
	rm -f simple_release
